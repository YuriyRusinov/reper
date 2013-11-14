/**********************************************************************
 * Module:  JKKSLoader.cpp
 * Author:  yuriyrusinov
 * Modified: 1 апреля 2008 г. 17:15
 * Purpose: Implementation of the class JKKSLoader
 ***********************************************************************/

#include <QStringList>
#include <QList>
#include <QtDebug>
#include <QPair>
#include <QRegExp>

#include "defines.h"
#include "kksdatabase.h"
#include "kksresult.h"
#include "kkspgdatabase.h"

#include <kksdebug.h>

#include "JKKSLoader.h"
#include "JKKSRubric.h"
#include "JKKSGlobalRubric.h"
#include "JKKSMessage.h"
#include "JKKSPMessage.h"
#include "JKKSCommand.h"
#include "JKKSCategory.h"
#include "JKKSCategoryAttr.h"
#include "JKKSDocument.h"
#include "JKKSMailMessage.h"
#include "JKKSIOUrl.h"
#include "JKKSIOTable.h"
#include "JKKSRefRecord.h"
#include "JKKSOrganization.h"
#include "JKKSTransport.h"
#include "JKKSOrgType.h"
#include "JKKSWorkMode.h"
#include "JKKSWorkModeType.h"
#include "JKKSSearchTemplate.h"
#include "JKKSSearchGroup.h"
#include "JKKSSearchCriterion.h"
#include "JKKSOrgPackage.h"
#include "JKKSType.h"
#include "JKKSPing.h"


JKKSLoader :: JKKSLoader (const QString& host,
                          const QString& name, 
                          const QString& user, 
                          const QString& pass, 
                          int p,
                          qint64 idTransport)
    : dbHost (host),
      dbName (name),
      dbUser (user),
      dbPassword (pass),
      dbPort (p),
      m_idTransport(idTransport),
      dbRead (new KKSPGDatabase()),
      dbWrite (new KKSPGDatabase()),
      idCurrentDl(-1),
      idCurrentUser(-1),
      m_localOrgId(-1),
      local_address (JKKSAddress()),
      senderUID(QString()),
      receiverUID(QString())
{
}

JKKSLoader :: ~JKKSLoader (void)
{
}

int JKKSLoader::setLocalAddress(const JKKSAddress & address) const
{
    int result = ERROR_CODE;
    if(dbRead){
        QString sql = QString("SELECT uSetLocalAddress('%1', %2, %3);")
                              .arg(address.address())
                              .arg(address.port() <= 0 ? QString("NULL") : QString::number(address.port()))
                              .arg(address.useGateway() ? QString("TRUE") : QString("FALSE"));
        
        KKSResult * res = dbRead->execute(sql);

        if(res && res->getRowCount() == 1)
            result = res->getCellAsInt(0, 0);

        delete res;
    }
    return result;
}

const JKKSAddress & JKKSLoader::getLocalAddress(bool reload) const
{
    if(!reload)
        if(!local_address.address().isEmpty())
            return local_address;
	
    if(dbRead){
        KKSResult * res = dbRead->execute(QString("SELECT * from uGetLocalAddress(%1);").arg(m_idTransport));
        if(res && res->getRowCount() == 1){
            QString addr = res->getCellAsString(0, 0);
            
            int port = 0;
            if(!res->isEmpty(0, 1)){
                port = res->getCellAsInt(0, 1);
            }
            
            local_address = JKKSAddress(addr, port, res->getCellAsBool(0, 2));
        }
        delete res;
    }

    return local_address;
}

int JKKSLoader :: getDlId (void) const
{
    if(idCurrentDl > 0)
        return idCurrentDl;

    if (dbRead){
        KKSResult * res = dbRead->execute("select getCurrentDl();");
        if (res && res->getRowCount() == 1)
            idCurrentDl = res->getCellAsInt(0, 0);
        delete res;
    }
    else
        return ERROR_CODE;

    return idCurrentDl;
}

int JKKSLoader :: getUserId (void) const
{
    if(idCurrentUser > 0)
        return idCurrentUser;

    if (dbRead){
        KKSResult * res = dbRead->execute("select getCurrentUser();");
        if (res && res->getRowCount() == 1)
            idCurrentUser = res->getCellAsInt (0, 0);
        delete res;
    }
    else
        return ERROR_CODE;
    
    return idCurrentUser;
}

QString JKKSLoader :: getDlName (void) const
{
    QString name(QString::null);
    int id = getDlId ();
    if(id != ERROR_CODE){
        QString sql = QString("select * from getDl(%1)").arg(id);
        KKSResult * res = dbRead->execute(sql);
        if(res && res->getRowCount() == 1)
            name = res->getCellAsString (0, 5);
        if(res)
            delete res;
    }
    return name;
}

QString JKKSLoader :: getUserName (void) const
{
    QString name(QString::null);
    int id = getUserId();
    if(id != ERROR_CODE){
        QString sql = QString("select fio from users where id = %1").arg(id);
        KKSResult * res = dbRead->execute(sql);
        if(res && res->getRowCount() == 1)
            name = res->getCellAsString (0, 0);
        if(res)
            delete res;
    }

    return name;
}

bool JKKSLoader :: connectToDb (void)
{
    bool res1 = dbRead->connect (dbHost, dbName, dbUser, dbPassword, QString::number (dbPort));
    bool res2 = res1 && dbWrite->connect (dbHost, dbName, dbUser, dbPassword, QString::number (dbPort));
    if (res1 && !res2)
    {
        dbRead->disconnect ();
        return false;
    }

    QString sql = QString("select registerTransport(%1)").arg(m_idTransport);
    KKSResult * r = dbRead->execute(sql);
    if (!r || r->getCellAsInt(0, 0) != 1)
    {
        if (r)
            delete r;

        dbRead->disconnect();
        dbWrite->disconnect();

        return false;
    }
    delete r;

    KKSResult * rw = dbWrite->execute(sql);
    if (!rw || rw->getCellAsInt(0, 0) != 1)
    {
        if (r)
            delete r;

        dbRead->disconnect();
        dbWrite->disconnect();

        return false;
    }
    delete rw;

    records.clear ();

    return res2;
}

KKSDatabase * JKKSLoader :: getDbRead (void) const
{
    return dbRead;
}

KKSDatabase * JKKSLoader :: getDbWrite (void) const
{
    return dbWrite;
}

qint64 JKKSLoader :: getIdTransport (void) const
{
    return m_idTransport;
}

int JKKSLoader :: getLocalOrgId() const
{
    if(m_localOrgId > 0)
        return m_localOrgId;

    QString sql =  QString ("select * from getLocalOrgId();");
    KKSResult * r1 = dbRead->execute (sql);
    if (!r1 || r1->getRowCount() != 1)
    {
        if (r1)
            delete r1;
        return m_localOrgId;
    }
    else
        m_localOrgId = r1->getCellAsInt(0, 0);
    
    if(r1)
        delete r1;

    return m_localOrgId;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readMessages (QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> messList;

    if(senderUID.isEmpty()){
        QString sql =  QString ("select * from getlocalorg();");
        KKSResult * r1 = dbRead->execute (sql);
        if (!r1 || r1->getRowCount() != 1)
        {
            //if (r1)
            //    delete r1;
            //return messList;
        }
        else
            senderUID = r1->getCellAsString (0, 3);
        
        if(r1)
            delete r1;
    }

    if(!senderUID.isEmpty()){
        messList += readCommands(receivers);
        //
        // TODO process documents, etc...
        //
        messList += readDocuments(receivers);
        messList += readMails (receivers);

        messList += readMailConfirmations(receivers);
        messList += readCmdConfirmations(receivers);

        messList += readTableRecords(receivers);
    }
    
    messList += readQueueResults (receivers);
    //messList += readPingResults(receivers);

    return messList;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readCmdConfirmations(QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uGetCmdsConfirm () union select * from uGetCmdsConfirm1();");
    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 4)
    {
        for (int i=0; i<res->getRowCount(); i++) {
            JKKSAddress addr;
            addr.setAddress(res->getCellAsString (i, 0));
            addr.setPort(res->getCellAsInt(i, 6));
            addr.setUseGateway(res->getCellAsBool(i, 8));

            JKKSCmdConfirmation cfm(res->getCellAsInt (i, 1),//src_id
                                    res->getCellAsString (i, 2),//extra_id
                                    res->getCellAsInt (i, 3),//idJrState
                                    res->getCellAsDateTime(i, 4),//accepted_datetime
                                    res->getCellAsDateTime(i, 5),//receive_datetime
                                    addr//address
                                    );
            
            JKKSPMessage pM(cfm.serialize(), cfm.getMessageType());
            pM.setVerifyReceiver(false);
            pM.setSenderUID(this->senderUID);
            pM.setReceiverUID(res->getCellAsString(i, 7));//email_prefix
            JKKSPMessWithAddr * pMessWithAddr(new JKKSPMessWithAddr(pM, cfm.getAddr(), cfm.id()));
            pMessWithAddr->unp = res->getCellAsString(i, 7);//email_prefix
            result.append(pMessWithAddr);

            if(!receivers.contains(pM.receiverUID()))
                receivers.append(pM.receiverUID());
        }
    }
    if (res)
        delete res;

    return result;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readMailConfirmations(QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uGetMsgsConfirm ();");
    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 4)
    {
        for (int i=0; i<res->getRowCount(); i++) {
            JKKSAddress addr;
            addr.setAddress(res->getCellAsString (i, 0));
            addr.setPort(res->getCellAsInt(i, 5));
            addr.setUseGateway(res->getCellAsBool(i, 7));

            JKKSMailConfirmation cfm(res->getCellAsInt(i, 1),//src_id
                                     res->getCellAsInt (i, 2),//extra_id
                                     res->getCellAsDateTime (i, 3),//read_datetime,
                                     res->getCellAsDateTime(i, 4),//receive_datetime
                                     addr//address
                                     );
            
            JKKSPMessage pM(cfm.serialize(), cfm.getMessageType());
            pM.setVerifyReceiver(false);
            pM.setSenderUID(this->senderUID);
            pM.setReceiverUID(res->getCellAsString(i, 6));//email_prefix
            JKKSPMessWithAddr * pMessWithAddr(new JKKSPMessWithAddr(pM, cfm.getAddr(), cfm.id()));
            pMessWithAddr->unp = res->getCellAsString(i, 6);//email_prefix
            result.append(pMessWithAddr);

            if(!receivers.contains(pM.receiverUID()))
                receivers.append(pM.receiverUID());
        }
    }
    if (res)
        delete res;

    return result;
}


QList<JKKSPMessWithAddr *> JKKSLoader :: readCommands (QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from ugetoutcmds () union select * from uGetOutCmds1();");
    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 16)
    {
        for (int i=0; i<res->getRowCount(); i++) {
            int amount = -1;//exec_period amount
            int unit = -1;//exec_period unit
            if(!res->isEmpty (i, 15)){//exec_period present!
                QStringList tiStr = res->getCellAsStringList (i, 15);
                amount = tiStr[0].toInt();
                unit = tiStr[1].toInt();
            }

            qint64 idCat = res->getCellAsInt (i, 10);

            JKKSAddress addr(res->getCellAsString (i, 0), res->getCellAsInt(i, 29), res->getCellAsBool(i, 31));

            QString s;//empty string
            JKKSCommand command(res->getCellAsInt (i, 1),//id
                                res->getCellAsString (i, 22),//dl_from_uid
                                res->getCellAsString (i, 3),//dl_from_name
                                res->getCellAsString (i, 23),//dl_executor_uid
                                res->getCellAsString (i, 5),//dl_executor_name
                                JKKSAddress(),//dl_executor_address
                                JKKSAddress(),//exec_org_address
                                res->getCellAsString (i, 24),//dl_to_uid
                                res->getCellAsString (i, 9),//dl_to_name
                                idCat,
                                res->getCellAsString (i, 11),//category_code
                                res->getCellAsInt (i, 12),//id_jr_state
                                res->getCellAsDateTime (i, 13),//insert_time
                                res->getCellAsDateTime (i, 14),//exec_datetime
                                amount,
                                unit,
                                res->getCellAsString (i, 16),//message_body
                                addr,//full_address
                                s,
                                res->getCellAsString(i, 17),//uniqueID
                                res->getCellAsString(i, 18),//inputNumber
                                res->getCellAsString(i, 19),//outputNumber
                                res->getCellAsInt(i, 20), //idUrgencyLevel
                                res->isEmpty(i, 21) ? -1 : res->getCellAsInt(i, 21) //id_io_object_att
                                //receive_datetime и accepted_datetime передавать не надо
                                );
            if (command.idObject() > 0)
            {
                qint64 idOrganization = res->getCellAsInt(i, 28);
                JKKSDocument ioDoc = readDocument (command.idObject(), idOrganization);
                command.setAttachment (ioDoc);
            }
            command.setCategory(readCategories(idCat));
            JKKSPMessage pM(command.serialize(), command.getMessageType());
            pM.setUrgencyLevel(res->getCellAsString(i, 27));
            pM.setVerifyReceiver(false);
            pM.setSenderUID(this->senderUID);
            pM.setReceiverUID(res->getCellAsString(i, 30)); //email_prefix
            JKKSPMessWithAddr * pMessWithAddr(new JKKSPMessWithAddr(pM, command.getAddr(), command.id()));
            pMessWithAddr->unp = res->getCellAsString(i, 30); //email_prefix
            result.append(pMessWithAddr);

            if(!receivers.contains(pM.receiverUID()))
                receivers.append(pM.receiverUID());
        }
    }
    if (res)
        delete res;

    return result;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readDocuments (QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uGetOutObjects();");

    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 8)
    {
        for (int i=0; i<res->getRowCount(); i++)
        {
            JKKSAddress fullAddr = JKKSAddress (res->getCellAsString (i, 0), res->getCellAsInt(i, 10), res->getCellAsBool(i, 12));
            
            qint64 idObject = res->getCellAsInt (i, 1);
            qint64 idCommand = res->getCellAsInt (i, 6);
            qint64 idJournal = res->getCellAsInt (i, 8);
            QDateTime rt = res->getCellAsDateTime (i, 4);
            qint64 idOrganization = res->getCellAsInt(i, 9);
            
            JKKSDocument doc = readDocument (idObject, idOrganization);
            doc.setCommandId (idCommand);
            doc.setJournal (idJournal);
            doc.setRealTime (rt);
            doc.setAddr (fullAddr);
            
            JKKSPMessage pM(doc.serialize(), doc.getMessageType());
            pM.setVerifyReceiver(false);
            pM.setSenderUID(this->senderUID);
            pM.setReceiverUID(res->getCellAsString(i, 11));//email_prefix
            JKKSPMessWithAddr * pMessWithAddr(new JKKSPMessWithAddr(pM, doc.getAddr(), doc.getJournal()));
            pMessWithAddr->unp = res->getCellAsString(i, 11);//email_prefix
            result.append(pMessWithAddr);

            if(!receivers.contains(pM.receiverUID()))
                receivers.append(pM.receiverUID());

        }
    }

    if (res)
        delete res;

    return result;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readMails (QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uGetOutMsgs();");

    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 11)
    {
        for (int i=0; i<res->getRowCount(); i++)
        {
            JKKSAddress fullAddr = JKKSAddress (res->getCellAsString (i, 0), res->getCellAsInt(i, 19), res->getCellAsBool(i, 21));

            qint64 idMessage = res->getCellAsInt (i, 1);
            QString u_idDlFrom = res->getCellAsString (i, 15);
            QString u_idDlTo = res->getCellAsString (i, 16);
            QDateTime sentTime = res->getCellAsDateTime (i, 9);
            qint64 idObject = res->getCellAsInt (i, 8);
            QString messBody = res->getCellAsString (i, 10);
            QString s;//empty string
            QString uid = res->getCellAsString (i, 11);
            QString inputNumber = res->getCellAsString (i, 12);
            QString outputNumber = res->getCellAsString (i, 13);
            int idUrgencyLevel = res->getCellAsInt(i, 14);
            
            JKKSMailMessage mMessage (idMessage, 
                                      idObject, 
                                      messBody, 
                                      u_idDlTo, 
                                      u_idDlFrom, 
                                      sentTime, 
                                      fullAddr, 
                                      s, 
                                      uid, 
                                      inputNumber, 
                                      outputNumber,
                                      idUrgencyLevel);
            
            if (idObject > 0)
            {
                qint64 idOrganization = res->getCellAsInt(i, 18);
                JKKSDocument doc (readDocument (idObject, idOrganization));
                doc.setCommandId (-1);//idCommand);
                doc.setJournal (-1);//idJournal);
                doc.setRealTime (sentTime);
                mMessage.setAttachment (doc);
            }

            JKKSPMessage pM(mMessage.serialize(), mMessage.getMessageType());
            pM.setUrgencyLevel(res->getCellAsString(i, 17));
            pM.setVerifyReceiver(false);
            pM.setSenderUID(this->senderUID);
            pM.setReceiverUID(res->getCellAsString(i, 20));//email_prefix

            JKKSPMessWithAddr * pMessWithAddr (new JKKSPMessWithAddr (pM, mMessage.getAddr(), mMessage.id()));
            pMessWithAddr->unp = res->getCellAsString(i, 20);//email_prefix
            result.append (pMessWithAddr);

            if(!receivers.contains(pM.receiverUID()))
                receivers.append(pM.receiverUID());

         }
   }

    if (res)
        delete res;

    return result;
}

JKKSDocument JKKSLoader :: readDocument (qint64 idObject, qint64 idOrganization) const
{
    QString docSql = QString ("select * from ioGetObject(%1)").arg (idObject);
    KKSResult * dRes = dbRead->execute (docSql);
    if (!dRes)
    {
        return JKKSDocument ();
    }

    qint64 idCat = dRes->getCellAsInt (0, 1);
    
    QString s;//empty string

    JKKSDocument doc (dRes->getCellAsString (0, 4), // name
                      idCat, // category
                      dRes->getCellAsInt (0, 2), // author
                      dRes->getCellAsString (0, 6), // tableName
                      dRes->getCellAsString (0, 7), // description
                      dRes->getCellAsString (0, 8), // info
                      dRes->getCellAsInt (0, 13), // mac label
                      dRes->getCellAsInt (0, 15), // id_sync_type
                      dRes->getCellAsString (0, 16), // sync_name
                      dRes->getCellAsString (0, 17), // sync_desc
                      dRes->getCellAsInt (0, 18), // id_owner_org
                      dRes->getCellAsBool (0, 19), // is_global
                      dRes->getCellAsString (0, 24), //email_prefix (owner org UID)

                      s,
                      s,

                      dRes->getCellAsString(0, 14), // uniqueID
                      dRes->getCellAsString(0, 30), //uuid
                      dRes->getCellAsInt (0, 3), // io_state
                      dRes->getCellAsString (0, 20).isEmpty() ? QColor() : QColor::fromRgba (dRes->getCellAsInt64 (0, 20)), // background color
                      dRes->getCellAsString (0, 21).isEmpty() ? QColor() : QColor::fromRgba (dRes->getCellAsInt64 (0, 21)), // foreground color
                      dRes->getCellAsString (0, 29) //r_icon
                      );
    
    doc.setId (idObject);
    QMap<qint64, JKKSCategory> cat = readCategories (idCat);
    doc.setCategory (cat);

    JKKSType type (dRes->getCellAsInt (0, 25), 
                   dRes->getCellAsString (0, 26), 
                   dRes->getCellAsString (0, 27),
                   dRes->getCellAsString (0, 28));

    doc.setType(type);

    delete dRes;
 
    //значения атрибутов
    QString attrSql = QString ("select * from ioGetObjectAttrsEx(%1)").arg (idObject);
    KKSResult * aRes = dbRead->execute (attrSql);
    QMap<QString, QString> aVals;
    if (!aRes)
        return JKKSDocument ();

    for (int ii=0; ii<aRes->getRowCount(); ii++)
    {
        aVals.insert (aRes->getCellAsString (ii, 4), aRes->getCellAsString (ii, 3));//здесь в качестве ключа используется поле unique_id атрибута
    }

    delete aRes;

    doc.setAttrValues (aVals);

    //рубрики
    QString rubrSql = QString ("select * from iogetrubrics (%1);").arg (idObject);
    
    KKSResult * rubrRes = dbRead->execute (rubrSql);
    if (!rubrRes)
        return JKKSDocument ();

    QMap<qint64, JKKSGlobalRubric> rubrs;
    for (int ii=0; ii<rubrRes->getRowCount(); ii++)
    {
        if (rubrRes->getCellAsInt (ii, 8) == 2)
        {
            //
            // rubric item cannot been sent
            //
            continue;
        }
        JKKSGlobalRubric wRubr (rubrRes->getCellAsInt (ii, 0), // id
                                rubrRes->getCellAsString(ii, 9), //uid
                                rubrRes->getCellAsInt (ii, 1) <= 0 ? -1 : rubrRes->getCellAsInt (ii, 1), // idParent
                                rubrRes->getCellAsInt (ii, 2) <= 0 ? -1 : idObject, // idObject
                                rubrRes->getCellAsString (ii, 5), // name
                                rubrRes->getCellAsString (ii, 6), // code
                                rubrRes->getCellAsString (ii, 7) // description
                                );
        qint64 idCategory = rubrRes->getCellAsInt (ii, 4);
        if (idCategory > 0)
        {
            QMap<qint64, JKKSCategory> rubrCats = readCategories (idCategory);
            wRubr.setCategory (rubrCats);
        }
        
        qint64 idSearchTemplate = rubrRes->getCellAsInt (ii, 3);
        if (idSearchTemplate > 0)
        {
            JKKSSearchTemplate st = readSearchTemplate (idSearchTemplate);
            wRubr.setSearchTemplate (st);
        }
        rubrs.insert (wRubr.getIdRubric(), wRubr);
    }

    doc.setRubrics (rubrs);

    //прикрепленные файлы
    QMap<qint64, JKKSIOUrl> files = readDocumentFiles (doc.id(), idOrganization);
    doc.setUrls (files);

    QMap<qint64, JKKSIOTable> tables = readDocumentTables (doc.id());
    doc.setTables (tables);

    return doc;
}

int JKKSLoader :: writeMessage (const JKKSPMessage & pMessage) const
{
    int result = ERROR_CODE;
    
    if(pMessage.verifyReceiver()){
        if(receiverUID.isEmpty()){
            QString sql =  QString ("select * from getlocalorg();");
            KKSResult * r1 = dbWrite->execute (sql);
            if (!r1 || r1->getRowCount() != 1)
            {
                kksCritical() << QObject::tr("Cannot execute getLocalOrg()");
                return IGNORE_CODE;
            }
            else
                receiverUID = r1->getCellAsString (0, 3);
            
            if(r1)
                delete r1;
        }
        if(pMessage.receiverUID() != receiverUID)
            return IGNORE_CODE;//message was addressed NOT to this organization
    }

    JKKSMessage * message (unpackMessage(pMessage));
    if (message)
    {
        result = message->writeToDB (this, pMessage.senderUID(), pMessage.receiverUID());
        delete message;
        if(result <= 0){
            result = ERROR_CODE;
        }
    }

    return result;
}

int JKKSLoader :: writeMessage (JKKSDocument *doc, int syncType) const
{
    int result = ERROR_CODE;
    if(!doc)
        return result;

    JKKSCategoryPair cCats = parseCategories(doc->getCategory());

    if(cCats.isNull()){
        kksCritical() << QObject::tr("Document (UID = %1) has no categories!").arg(doc->uid());
        return ERROR_CODE;
    }

    doc->setCategory (pairToMap(cCats));
    
    doc->setIdIoCat(cCats.mainCategory().id());//задаем категорию документа (в приемной БД, ее ИД отличается от ИД БД-источника)

    qint64 realId = checkForExist(doc->uid());

    if(syncType == 1){//insert
        if(realId <= 0)
            result = insertDocument(doc);
        else{
            doc->setId(realId);
            result = updateDocument(doc);
        }
    }
    else if(syncType == 2){//update
        if(realId <= 0)
            result = insertDocument(doc);
        else{
            doc->setId(realId);
            result = updateDocument(doc);
        }
    }
    else if(syncType == 3){//delete
        doc->setId(realId);
        result = deleteDocument(doc);
    }
    else
        return result; //error_code

    return result;
}

qint64 JKKSLoader :: checkForExist(const QString & docUID) const
{
    QString sql = QString ("select * from checkForExist('%1');").arg (docUID);
    
    KKSResult * res = dbWrite->execute (sql);
    if (!res || res->getRowCount() != 1){
        if(res)
            delete res;
        
        return -1;
    }

    qint64 id = res->getCellAsInt64(0, 0);
    delete res;

    return id;

}

qint64 JKKSLoader :: updateDocument (JKKSDocument *doc) const
{
    qint64 result = ERROR_CODE;
    if(!doc)
        return result;

    QString sql = QString("select ioSetUpdating(%1)").arg(doc->id());
    KKSResult * res = dbWrite->execute(sql);
    if(res && res->getRowCount() == 1){
        result = res->getCellAsInt(0, 0);
    }
    delete res;
    if(result != 1){
        kksCritical() << QObject::tr("ioSetUpdating(%1) for document with UID = %2 failed!").arg(doc->id()).arg(doc->uid());
        return ERROR_CODE;
    }

    result = ERROR_CODE;

    JKKSType type = doc->getType();
    qint64 idType;
    if(type.getName().isEmpty() || type.uid().isEmpty()){
        idType = 1;
    }
    else{
        idType = writeIOType(type);
    }


    //
    //запись в таблице io_objects
    //
    sql = QString ("select * from ioUpdate (%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12);")
        .arg (doc->id())
        .arg (doc->getName().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getName()))//name
        .arg (doc->idState () < 0 ? QString ("NULL") : QString::number (doc->idState()) )//id_state
        //.arg (doc->getTableName().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getTableName()))//table_name
        .arg (doc->getDescription().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getDescription())) //desc
        .arg (doc->getInfo().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getInfo()))//info
        .arg (QString::number (doc->getIdMaclabel ()))//id_maclabel
        .arg (doc->getSyncType() < 0 ? QString ("NULL") : QString::number (doc->getSyncType()) ) //id_sync_type
        .arg (doc->getGlobal() ? QString ("TRUE") : QString ("FALSE")) //is_global
        .arg (doc->bgColor().isValid() ? QString::number (doc->bgColor().rgba()) : QString ("NULL"))//record_fill_color
        .arg (doc->fgColor().isValid() ? QString::number (doc->fgColor().rgba()) : QString ("NULL")) //record_text_color
        .arg (QString::number(idType) ) //id_type
        .arg (doc->rIcon().isEmpty() ? QString("NULL") : QString("'%1'").arg(doc->rIcon()));//r_icon


    res = dbWrite->execute (sql);

    if (res && res->getRowCount() == 1)
        result = res->getCellAsInt (0, 0);
    
    if(res)
        delete res;
    
    if (result <= 0){
        kksCritical() << QObject::tr("ioUpdate() for document with UID = %1 failed").arg(doc->uid());
        return ERROR_CODE;
    }
    
    //
    //значения атрибутов
    //
    QMap<QString, QString> attrVals = doc->getAttrValues ();
    for (QMap<QString, QString>::const_iterator pa = attrVals.constBegin(); pa != attrVals.constEnd(); pa++)
    {
        QString val (pa.value());
        //сначала проверим, является ли строка содержащей только две кавычки (т.е. фактически пустой). Просто уберем их и проверим на isEmpty()
        if (val.startsWith("'")){
            val = val.mid (1);
            if (val.endsWith("'"))
                val = val.mid (0, val.length()-1);
        }
        val = val.trimmed();
        
        if(!val.isEmpty()){ //если строка не пустая, то заэскейпим служебные символы, если таковые имеются в строке
            val.replace("'", "''");
            //val.replace("\\", "\\\\");
            //val.replace("\"", "\\\"");
            val.prepend("'");
            val.append ("'");
        }

        QString attrSql = QString ("select * from ioUpdateAttrEx (%1, %2, %3, NULL::timestamp, NULL::timestamp, NULL::int4, NULL::int4, NULL::varchar);")
            .arg (doc->id())
            .arg (QString("'")+pa.key()+QString("'")) //здесь в качестве ключа используется unique_id
            .arg (val.isEmpty() ? QString ("NULL") : val);

        KKSResult * attrRes = dbWrite->execute (attrSql);

        int attr_ier = ERROR_CODE;
        if (attrRes)
        {
            attr_ier = attrRes->getCellAsInt (0, 0);
            delete attrRes;
        }
        if (attr_ier <= 0){
            kksCritical() << QObject::tr("ioUpdateAttrEx() for document with UID = %1 failed!").arg(doc->uid());
            return ERROR_CODE;
        }
    }

    //
    //рубрики
    // в настоящее время только добавляются новые рубрики и обновляются существующие.
    // удаления удаленных не происходит
    //
    QMap<qint64, JKKSGlobalRubric> rubrs = doc->rubrics ();
    QMap<qint64, qint64> idpRub; //здесь храним соответствие идентификатора рубрики в исходной БД и целевой БД (в процессе создания рубрик исходный родитель обновляется на целевой)
    
    for (QMap<qint64, JKKSGlobalRubric>::const_iterator pr = rubrs.constBegin (); 
            pr != rubrs.constEnd (); 
            pr++)
    {
        if (pr.key () < 0)
            continue;

        JKKSGlobalRubric wRubr = pr.value ();
        
        //если рубрика имеет родителя, обновляем информацию об этом родителе на актуальное значение
        if (wRubr.getParent () >= 0 && idpRub.constFind (wRubr.getParent ()) != idpRub.constEnd ())
            wRubr.setParent (idpRub.constFind (wRubr.getParent ()).value ());

        qint64 idChild = -1;
        if (wRubr.getCategory().count() != 0)
        {
            JKKSCategoryPair cCats = parseCategories (wRubr.getCategory());
            if(cCats.isNull()){
                kksCritical() << QObject::tr("parseCategories() for rubrics for document with UID = %1 failed!").arg(doc->uid());
                return ERROR_CODE;
            }
            
            idChild = cCats.mainCategory().id();
        }

        qint64 idSearchT = -1;
        if (wRubr.getSearchTemplate().id() > 0)
        {
            idSearchT = writeSearchTemplate (wRubr.getSearchTemplate());
            if(idSearchT <= 0)
                return ERROR_CODE;
        }

        QString sql =  QString ("select * from ioUpdateInclude ('%1', '%2', '%3', '%4', %5, %6, %7, %8, NULL );")
                                .arg (wRubr.getUid())
                                .arg (wRubr.getName ())
                                .arg (wRubr.getCode ())
                                .arg (wRubr.getDesc ())
                                .arg (wRubr.getParent () <= 0 ? QString ("NULL::int4") : QString::number (wRubr.getParent ()))
                                .arg (pr == rubrs.constBegin () ? QString::number (doc->id()) : QString ("NULL::int4"))//idObject
                                .arg (idChild <= 0 ? QString ("NULL::int4") : QString::number (idChild))
                                .arg (idSearchT < 0 ? QString ("NULL::int4") : QString::number (idSearchT));
                                
        KKSResult * res = dbWrite->execute (sql);
        if(!res || res->getRowCount () != 1 || res->getCellAsInt(0, 0) <= 0){
            kksCritical() << QObject::tr("ioUpdateInclude() for document with UID = %1 failed!").arg(doc->uid());
            if(res)
                delete res;
            return ERROR_CODE;
        }
        else
            idpRub.insert (wRubr.getIdRubric (), res->getCellAsInt (0, 0));

        if (res)
            delete res;
    }

    //
    //файлы
    //

    //сначала удаляем старые
    result = -1;
    sql = QString("select rRemoveObjUrl(%1, TRUE)").arg(doc->id());
    res = dbWrite->execute(sql);
    if(res && res->getRowCount() == 1){
        result = res->getCellAsInt(0, 0);
    }
    delete res;
    if(result != 1){
        kksCritical() << QObject::tr("rRemoveObjUrl() for document with UID = %1 failed!").arg(doc->uid());
        return ERROR_CODE;
    }
    
    //создаем новые
    QMap<qint64, JKKSIOUrl> urls = doc->urls ();
    for (QMap<qint64, JKKSIOUrl>::const_iterator pf = urls.constBegin (); 
            pf != urls.constEnd (); 
            pf++)
    {
        JKKSIOUrl url = pf.value ();
        url.setIOId (doc->id ());
        int res = this->writeDocumentFile (url);//pf.value());
        if(res <= 0)
            return ERROR_CODE;
    }

    //
    //дополнительные таблицы 
    //
    QMap<qint64, JKKSIOTable> tables = doc->ref_tables ();
    for (QMap<qint64, JKKSIOTable>::const_iterator pt = tables.constBegin (); 
            pt != tables.constEnd(); 
            pt++)
    {
        JKKSIOTable t = pt.value ();
        t.setIDTable (doc->id());
        
        JKKSCategoryPair cCats = parseCategories(t.getCategory());
        if(cCats.isNull()){
            kksCritical() << QObject::tr("parseCategories() for addition tables for document with UID = %1 failed!").arg(doc->uid());
            return ERROR_CODE;
        }
        
        t.setCategory (pairToMap(cCats));
                
        if (!cCats.isAlone())
        {
            JKKSCategory C = cCats.childCategory();
            QString table_sql = QString ("select ioupdatetable ('%1', %2, %3, '%4', '%5');")
                .arg (t.getName())
                .arg (doc->id())
                .arg (C.id())
                .arg (t.getTitle())
                .arg (t.uid());
            
            KKSResult * tRes = dbWrite->execute (table_sql);
            if (!tRes || tRes->getRowCount() != 1 || tRes->getCellAsInt(0, 0) < 1){
                kksCritical() << QObject::tr("ioupdatetable() for addition tables for document with UID = %1 failed!").arg(doc->uid());
                if(tRes)
                    delete tRes;
                return ERROR_CODE;
            }
            delete tRes;
        }
    }

    QString sqlComplete (QString ("select * from ioSetCompleted (%1, 2);").arg (doc->id()));
    
    KKSResult * complRes = dbWrite->execute (sqlComplete);
    
    if (!complRes || complRes->getRowCount() != 1 || complRes->getCellAsInt(0, 0) < 1){
        if(complRes)
            delete complRes;
        kksCritical() << QObject::tr("ioSetCompleted() for document with UID = %1 failed!").arg(doc->uid());
        return ERROR_CODE;
    }
    if(complRes)
        delete complRes;

    return result;
}

qint64 JKKSLoader :: deleteDocument (JKKSDocument *doc) const
{
    qint64 result = ERROR_CODE;
    if(!doc)
        return result;

    return result;
}

qint64 JKKSLoader :: insertDocument (JKKSDocument *doc) const
{
    qint64 result = ERROR_CODE;
    if(!doc)
        return result;

    QString ioTableName = doc->getTableName();

    JKKSType type = doc->getType();
    qint64 idType;
    if(type.getName().isEmpty() || type.uid().isEmpty()){
        idType = 1;
    }
    else{
        idType = writeIOType(type);
        if(idType <= 0)
            return ERROR_CODE;
    }

    QString sql = QString ("select * from ioInsert (%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, NULL, NULL, %13, %14, %15, %16, %17);")
        .arg (doc->getName().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getName()))//name
        .arg (doc->getIdIoCat())//id_io_category
        .arg (QString ("NULL"))// ибо в БД-приемнике автора (пользователя) может не быть (как правило не будет)
        .arg (doc->idState () < 0 ? QString ("NULL") : QString::number (doc->idState()) )//id_state
        .arg (QString ("NULL"))//table_name (она будет иметь свое собственное название в целевой БД)
        .arg (doc->getDescription().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getDescription())) //desc
        .arg (doc->getInfo().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getInfo()))//info
        .arg (QString::number (doc->getIdMaclabel ()))//id_maclabel
        .arg (doc->uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->uid()))//unique_id
        .arg (doc->getSyncType() < 0 ? QString ("NULL") : QString::number (doc->getSyncType()) ) //id_sync_type
        .arg (doc->getOwnerOrgUID().isEmpty() ? QString ("NULL") : QString::number (getIdByUID("organization", doc->getOwnerOrgUID()))) //id_owner_org
        .arg (doc->getGlobal() ? QString ("TRUE") : QString ("FALSE")) //is_global
        .arg (doc->bgColor().isValid() ? QString::number (doc->bgColor().rgba()) : QString ("NULL::int8")) //record_fill_color
        .arg (doc->fgColor().isValid() ? QString::number (doc->fgColor().rgba()) : QString ("NULL::int8")) //record_text_color
        .arg (QString::number(idType))//id_io_type
        .arg (doc->rIcon().isEmpty() ? QString ("NULL") : QString ("'%1'::varchar").arg (doc->rIcon()))//r_icon
        .arg (doc->uuid().isEmpty() ? QString ("NULL::uuid") : QString ("'%1'::uuid").arg (doc->uuid()));//uuid_t

    KKSResult * res = dbWrite->execute (sql);

    if (res && res->getRowCount() == 1)
        result = res->getCellAsInt (0, 0);
    
    if(res)
        delete res;
    
    if (result <= 0 ){
        kksCritical() << QObject::tr("ioInsert() for document with UID = %1 failed").arg(doc->uid());
        return ERROR_CODE;
    }
    
    doc->setId (result);

    if (!ioTableName.isEmpty())
    {
        QString tSql (QString("select * from ioGetObject (%1)").arg (result));
        KKSResult * tableRes = dbWrite->execute (tSql);
        if (!tableRes || tableRes->getRowCount() == 0)
        {
            kksCritical() << QObject::tr("ioGetObject(%1) for document with UID = %2 failed!").arg(result).arg(doc->uid());
            if (tableRes)
                delete tableRes;

            return ERROR_CODE;
        }

        doc->setTableName (tableRes->getCellAsString (0, 6));
        delete tableRes;
    }

    QMap<QString, QString> attrVals = doc->getAttrValues ();
    for (QMap<QString, QString>::const_iterator pa = attrVals.constBegin(); pa != attrVals.constEnd(); pa++)
    {
        QString val = pa.value();

        //сначала проверим, является ли строка содержащей только две кавычки (т.е. фактически пустой). Просто уберем их и проверим на isEmpty()
        if (val.startsWith("'")){
            val = val.mid (1);
            if (val.endsWith("'"))
                val = val.mid (0, val.length()-1);
        }
        val = val.trimmed();
        
        if(!val.isEmpty()){ //если строка не пустая, то заэскейпим служебные символы, если таковые имеются в строке
            val.replace("'", "''");
            //val.replace("\\", "\\\\");
            //val.replace("\"", "\\\"");
            val.prepend("'");
            val.append ("'");
        }

        QString attrSql = QString ("select * from ioInsertAttrEx (%1, %2, %3, NULL::timestamp, NULL::timestamp, NULL::int4, NULL::int4, NULL::varchar);")
            .arg (doc->id())
            .arg (QString("'")+pa.key()+QString("'")) //здесь в качестве ключа используется unique_id атрибута
            .arg (val.isEmpty() ? QString ("NULL") : val);
        KKSResult * attrRes = dbWrite->execute (attrSql);

        int attr_ier = ERROR_CODE;
        if (attrRes)
        {
            attr_ier = attrRes->getCellAsInt (0, 0);
            delete attrRes;
        }
        
        if (attr_ier < 0){ //если 0, то это означает, что в качестве значения атрибута использовался NULL
            kksCritical() << QObject::tr("ioInsertAttrEx() for document with UID = %1 failed! SQL = %2").arg(doc->uid()).arg(attrSql);
            return ERROR_CODE;
        }
        else if(attr_ier == 0 && !val.isEmpty()){
            kksCritical() << QObject::tr("ioInsertAttrEx() for document with UID = %1 failed! Result is 0, but value is not NULL! SQL = %2").arg(doc->uid()).arg(attrSql);
            return ERROR_CODE;
        }
    }

    QMap<qint64, JKKSGlobalRubric> rubrs = doc->rubrics ();
    QMap<qint64, qint64> idpRub;//здесь храним соответствие идентификатора рубрики в исходной БД и целевой БД (в процессе создания рубрик исходный родитель обновляется на целевой)
    
    for (QMap<qint64, JKKSGlobalRubric>::const_iterator pr = rubrs.constBegin (); \
            pr != rubrs.constEnd (); \
            pr++)
    {
        if (pr.key () < 0)
            continue;

        JKKSGlobalRubric wRubr = pr.value ();
        
        //если рубрика имеет родителя, обновляем информацию об этом родителе на актуальное значение
        if (wRubr.getParent () >= 0 && idpRub.constFind (wRubr.getParent ()) != idpRub.constEnd ())
            wRubr.setParent (idpRub.constFind (wRubr.getParent ()).value ());

        qint64 idChild = -1;
        if (wRubr.getCategory().count() != 0)
        {
            JKKSCategoryPair cCats = parseCategories (wRubr.getCategory());
            if(cCats.isNull()){
                kksCritical() << QObject::tr("parseCategories() for rubrics for document with UID = %1 failed!").arg(doc->uid());
                return ERROR_CODE;
            }
            
            idChild = cCats.mainCategory().id();
        }

        qint64 idSearchT = -1;
        if (wRubr.getSearchTemplate().id() > 0)
        {
            idSearchT = writeSearchTemplate (wRubr.getSearchTemplate());
            if(idSearchT <= 0)
                return ERROR_CODE;
        }
        QString sql =  QString ("select * from ioinsertinclude (%1, %2, '%3', '%4', '%5', %6, %7, '%8', NULL);")
                                .arg (wRubr.getParent () <= 0 ? QString ("NULL::int4") : QString::number (wRubr.getParent ()))
                                .arg (pr == rubrs.constBegin () ? QString::number (doc->id()) : QString ("NULL::int4"))
                                .arg (wRubr.getName ())
                                .arg (wRubr.getCode ())
                                .arg (wRubr.getDesc ())
                                .arg (idSearchT<0 ? QString ("NULL::int4") : QString::number (idSearchT))
                                .arg (idChild<0 ? QString("NULL::int4") : QString::number(idChild))
                                .arg (wRubr.getUid  ());

        KKSResult * res = dbWrite->execute (sql);
        
        if (!res || res->getRowCount () != 1 || res->getCellAsInt(0, 0) <= 0){
            kksCritical() << QObject::tr("ioinsertinclude() for document with UID = %1 failed!").arg(doc->uid());
            if(res)
                delete res;
            return ERROR_CODE;
        }
        else
            idpRub.insert (wRubr.getIdRubric (), res->getCellAsInt64 (0, 0));

        if (res)
            delete res;
    }

    QMap<qint64, JKKSIOUrl> urls = doc->urls ();
    for (QMap<qint64, JKKSIOUrl>::const_iterator pf = urls.constBegin (); \
            pf != urls.constEnd (); \
            pf++)
    {
        JKKSIOUrl url = pf.value ();
        url.setIOId (doc->id ());
        int res = this->writeDocumentFile (url);//pf.value());
        if(res <= 0)
            return ERROR_CODE;
    }

    QMap<qint64, JKKSIOTable> tables = doc->ref_tables ();
    for (QMap<qint64, JKKSIOTable>::const_iterator pt = tables.constBegin (); \
            pt != tables.constEnd(); \
            pt++)
    {
        JKKSIOTable t = pt.value ();
        t.setIDTable (doc->id());

        JKKSCategoryPair cCats = parseCategories(t.getCategory());
        if(cCats.isNull()){
            kksCritical() << QObject::tr("parseCategories() for addition tables for document with UID = %1 failed!").arg(doc->uid());
            return ERROR_CODE;
        }
                
        t.setCategory (pairToMap(cCats));

        if (!cCats.isAlone())
        {
            JKKSCategory C = cCats.childCategory();
            QString table_sql = QString ("select ioinserttable ('%1', %2, %3, '%4', '%5', NULL, NULL);")
                .arg (t.getName())
                .arg (doc->id())
                .arg (C.id())
                .arg (t.getTitle())
                .arg (t.uid());
            
            KKSResult * tRes = dbWrite->execute (table_sql);
            if (!tRes || tRes->getRowCount() != 1 || tRes->getCellAsInt(0, 0) <= 0){
                if(tRes)
                    delete tRes;
                kksCritical() << QObject::tr("ioinserttable() for document with UID = %1 failed!").arg(doc->uid());
                return ERROR_CODE;
            }
            
            if(tRes)
                delete tRes;
        }
    }

    if (doc->getCommandId() >= 0)
    {
        QString cjSql = QString ("select * from cjInsertEx (%1, %2, %3);")
            .arg (doc->id())
            .arg (doc->getCommandId())
            .arg (QString ("to_timestamp('") +
                      doc->getRealTime().toString ("dd.MM.yyyy hh:mm:ss") +
                  QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp"));
        
        KKSResult *cjRes = dbWrite->execute (cjSql);

        if (!cjRes || cjRes->getRowCount() != 1 || cjRes->getCellAsInt(0, 0) <= 0){
            if(cjRes)
                delete cjRes;
            kksCritical() << QObject::tr("cjInsertEx() for document with UID = %1 failed!").arg(doc->uid());
            return ERROR_CODE;
        }
        
        if(cjRes)
            delete cjRes;

        cjSql = QString("select cjSetAsResult(%1, %2);")
            .arg (doc->id())
            .arg (doc->getCommandId());
        
        KKSResult *cr = dbWrite->execute (cjSql);
        
        if (!cr || cr->getRowCount() != 1 || cr->getCellAsInt(0, 0) <= 0){
            if(cr)
                delete cr;
            kksCritical() << QObject::tr("cjSetAsResult() for document with UID = %1 failed!").arg(doc->uid());
            return ERROR_CODE;
        }
        
        if(cr)
            delete cr;
        
        cjSql = QString ("select id_dl_from from command_journal where id=%1").arg (doc->getCommandId());
        
        KKSResult *cPriv = dbWrite->execute (cjSql);
        if (!cPriv){
            kksCritical() << QObject::tr("Error in command journal");
            return ERROR_CODE;
        }
        else
        {
            int idRole = cPriv->getCellAsInt (0, 0);
            qint64 idObject = doc->id();
            cjSql = QString ("select setprivileges (%1, %2, TRUE, TRUE, TRUE, TRUE, TRUE);").arg (idRole).arg (idObject);
            KKSResult *cSPriv = dbWrite->execute (cjSql);
            if (!cSPriv){
                kksCritical() << QObject::tr("Cannot set privilegies");
                return ERROR_CODE;
            }

            delete cPriv;
        }
    }

    QString sqlComplete (QString ("select * from ioSetCompleted (%1, 1);").arg (doc->id()));
    
    KKSResult * complRes = dbWrite->execute (sqlComplete);
    
    if (!complRes){
        kksCritical() << QObject::tr("Cannot set document as completed");
        return ERROR_CODE;
    }

    delete complRes;

    return result;
}

qint64 JKKSLoader :: writeAddTable (qint64 idObject, JKKSIOTable& table) const
{
    JKKSCategoryPair cCats = parseCategories(table.getCategory());
    if(cCats.isNull()){
        kksCritical() << QObject::tr("parseCategories() for addition tables IdObject = %1 failed!").arg(idObject);
        return ERROR_CODE;
    }
        
    table.setCategory (pairToMap(cCats));

    qint64 idt = -1;
    if (!cCats.isAlone())
    {
        JKKSCategory C = cCats.childCategory();
        QString table_sql = QString ("select ioinserttable ('%1', %2, %3, '%4', '%5', NULL, NULL);")
            .arg (table.getName())
            .arg (idObject)
            .arg (C.id())
            .arg (table.getTitle())
            .arg (table.uid());
        
        KKSResult * tRes = dbWrite->execute (table_sql);
        if (!tRes || tRes->getRowCount() != 1 || tRes->getCellAsInt(0, 0) <= 0)
        {
            if(tRes)
                delete tRes;
            kksCritical() << QObject::tr("ioinserttable() for addition tables IdObject = %1 failed!").arg(idObject);
            return ERROR_CODE;
        }

        if (tRes->getRowCount() == 1 && tRes->getColumnCount()==1)
        {
            idt = tRes->getCellAsInt (0, 0);
            table.setIDTable (idt);
        }
        delete tRes;
    }

    return idt;
}

qint64 JKKSLoader::getIdByUID(const QString & tableName, const QString & uid) const
{
    qint64 id = -1;
    QString sql = QString ("select getIdByUID('%1', '%2');")
            .arg (tableName)
            .arg (uid);
    
    KKSResult * tRes = dbWrite->execute (sql);
    if (!tRes)
    {
        return id;
    }
    if (tRes->getRowCount() == 1 && !tRes->isEmpty(0, 0))
    {
        id = tRes->getCellAsInt64 (0, 0);
    }

    delete tRes;

    return id;
}

int JKKSLoader::writeMessage (JKKSMailMessage *mMess) const
{
    int result = ERROR_CODE;
    if (mMess)
    {
        JKKSDocument * doc = 0;
        int resDoc = ERROR_CODE;
        if (mMess->getIO() > 0)
        {
            doc = new JKKSDocument (mMess->getAttachedDoc());
            if (!doc || (resDoc = this->writeMessage(doc)) == ERROR_CODE)
            {
                if (doc)
                {
                    delete doc;
                    doc = 0;
                }
                return result;
            }
            mMess->setIO (doc->id());
        }

        QString msgBody = mMess->getMessageBody();
        if(!msgBody.isEmpty()){
            msgBody.replace("'", "''");
        }

        QString sql = QString ("select * from msginsertin (%1, %2, %3, %4, %5, %6, %7, %8, %9)")
                                .arg (mMess->getIO() > 0 ? QString::number (mMess->getIO()) : QString ("NULL::integer"))
                                .arg (msgBody.isEmpty() ? QString ("NULL") : QString ("'%1'").arg (msgBody))
                                .arg ((int)getIdByUID("position", mMess->getIdDlTo()))
                                .arg ((int)getIdByUID("position", mMess->getIdDlFrom()))
                                .arg (mMess->getSentDateTime().isNull() ? QString("NULL") :
                                        QString ("to_timestamp('") +
                                        mMess->getSentDateTime().toString ("dd.MM.yyyy hh:mm:ss") +
                                        QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp"))
                                .arg(mMess->id()) //запишется как extra_id
                                .arg(mMess->uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (mMess->uid()))
                                .arg(mMess->outputNumber().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (mMess->outputNumber()))
                                .arg(mMess->idUrgencyLevel() > 0 ? mMess->idUrgencyLevel() : 1);

        KKSResult * res = dbWrite->execute (sql);
        
        if (res && res->getRowCount() == 1)
            result = res->getCellAsInt (0, 0);
        if (res)
            delete res;

        if (doc && result > 0)
        {
            QString idRole = mMess->getIdDlTo();
            qint64 idObject = mMess->getIO();

            QString mMessSql = QString ("select setprivileges ('%1', %2, TRUE, TRUE, TRUE, TRUE, TRUE);") .arg (idRole).arg (idObject);
            KKSResult *cSPriv = dbWrite->execute (mMessSql);
            if (!cSPriv || cSPriv->getCellAsInt(0, 0) <= 0){
                kksCritical() << QObject::tr("Cannot set privilegies for the document with ID = %1").arg(idObject);
                return ERROR_CODE;
            }
            if(cSPriv)
                delete cSPriv;

            delete doc;
        }
    }

    return result;
}

JKKSMessage * JKKSLoader::unpackMessage (const JKKSPMessage & pMessage) const
{
    JKKSMessage * message = 0;
    switch (pMessage.getType())
    {
        case JKKSMessage::atCommand: message = new JKKSCommand(); break;
        case JKKSMessage::atDocument: message = new JKKSDocument(); break;
        case JKKSMessage::atMailMessage: message = new JKKSMailMessage (); break;
        case JKKSMessage::atMailConfirmation: message = new JKKSMailConfirmation(); break;
        case JKKSMessage::atCmdConfirmation: message = new JKKSCmdConfirmation(); break;
        case JKKSMessage::atEcho:
        case JKKSMessage::atRecord: message = new JKKSRefRecord(); break;
        case JKKSMessage::atRecConfirmation: message = new JKKSQueueResponse(); break;
        case JKKSMessage::atOrganization: message = new JKKSOrganization (); break;
        case JKKSMessage::atPosition: message = new JKKSRefRecord(); break;
        case JKKSMessage::atOrgPackage: message = new JKKSOrgPackage(); break;
        case JKKSMessage::atFilePart: message = new JKKSFilePart(); break;
        case JKKSMessage::atPing: message = new JKKSPing(); break;
        case JKKSMessage::atPingResponse: message = new JKKSPing(); break;
        default: kksCritical() << QObject::tr("Error: unknown message type");
    }

    if (message)
        message->unserialize (pMessage.getData());

    return message;
}

int JKKSLoader::writeMessage (JKKSCmdConfirmation *cfm) const
{
    int result = ERROR_CODE;
    if (cfm)
    {

        QString sql = QString ("select * from uConfirmCmd('%1', %2, %3, %4);")
            .arg (cfm->extraId())
            .arg (cfm->idState())
            .arg (cfm->acceptedDatetime().isNull() ? QString("NULL") :
                                                 QString ("to_timestamp('") +
                                                    cfm->acceptedDatetime().toString ("dd.MM.yyyy hh:mm:ss") +
                                                         QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp"))
            .arg (cfm->receiveDatetime().isNull() ? QString("NULL") :
                                                 QString ("to_timestamp('") +
                                                    cfm->receiveDatetime().toString ("dd.MM.yyyy hh:mm:ss") +
                                                         QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp"));

        KKSResult * res = dbWrite->execute (sql);
        if (res && res->getRowCount() == 1)
            result = res->getCellAsInt (0, 0);
        delete res;
    }

    if(result <= 0)
        return ERROR_CODE;

    return result;
}

int JKKSLoader::writeMessage (JKKSMailConfirmation *cfm) const
{
    int result = ERROR_CODE;
    if (cfm)
    {

        QString sql = QString ("select * from uConfirmMsg(%1, %2, %3);")
            .arg (cfm->extraId())
            .arg (cfm->readDatetime().isNull() ? QString("NULL") :
                                                 QString ("to_timestamp('") +
                                                    cfm->readDatetime().toString ("dd.MM.yyyy hh:mm:ss") +
                                                        QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp"))
            .arg (cfm->receiveDatetime().isNull() ? QString("NULL") :
                                                 QString ("to_timestamp('") +
                                                    cfm->receiveDatetime().toString ("dd.MM.yyyy hh:mm:ss") +
                                                        QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp"))                                                        ;

        KKSResult * res = dbWrite->execute (sql);
        if (res && res->getRowCount() == 1)
            result = res->getCellAsInt (0, 0);
        delete res;
    }

    if(result <= 0)
        return ERROR_CODE;

    return result;
}

int JKKSLoader::writeMessage (JKKSPing *ping, const QString & senderUID) const
{
    int result = ERROR_CODE;
    if (!ping)
        return ERROR_CODE;

    int idOrgTo = ping->idOrgTo();
    idOrgTo *= -1;

    JKKSQueueResponse resp (-1, 
                            idOrgTo, 
                            3, 
                            ping->senderAddress());

    resp.setOrgUid(ping->uidFrom());
    
    QString sql = QString("select kkssitoversion1()");
    KKSResult * res = dbWrite->execute(sql);
    if (res && res->getRowCount() == 1){
        QString ver = res->getCellAsString (0, 0);
        if(ver == ping->versionFrom())
            resp.setResult(3);
        else
            resp.setResult(4);
    }
    else
        resp.setResult(4);

    if(res)
        delete res;

    int res1 = generateQueueResponse(resp);
    if(res1 <= 0)
        return ERROR_CODE;

    return OK_CODE;
}

JKKSCategoryPair JKKSLoader::mapToPair(const QMap<qint64, JKKSCategory> & cats) const
{
    JKKSCategoryPair cCats;
    JKKSCategoryPair cNull;

    int cnt = cats.count();

    if(cnt < 1 || cnt > 3)
        return cNull;

    QMap<qint64, JKKSCategory>::const_iterator pc = cats.constBegin();
    for (; pc != cats.constEnd(); ++pc )
    {
        JKKSCategory c = pc.value();
        c.setState(1);
        if(cnt == 1){
            c.setIDChild(-1);
            c.setIDChild2(-1);
            cCats.setMainCategory(c);
            break;
        }

        if(c.getIDChild() <= 0)
            continue;
        else{
            cCats.setMainCategory(c);
                    
            cCats.setChildCategory(cats.value(c.getIDChild()));
            if(c.getIDChild2() > 0)
                cCats.setChild2Category(cats.value(c.getIDChild2()));

            break;
        }
    }

    if(cCats.isNull())
        return cNull;

    return cCats;

}

JKKSCategoryPair JKKSLoader::parseCategories(const QMap<qint64, JKKSCategory> & oldCats) const
{
    JKKSCategoryPair cCats;
    JKKSCategoryPair cNull;

    cCats = mapToPair(oldCats);
    if(cCats.isNull())
        return cCats;

    qint64 idChild = -1;
    qint64 idChild2 = -1;
    int res = 0;
	
    if(cCats.isAlone()){
        JKKSCategory c = cCats.mainCategory();
        res = writeCategory (c);
        if (res != ERROR_CODE){
            cCats.setMainCategory(c);
        }
        else
            return cNull;

        return cCats;
    }

    
    JKKSCategory c = cCats.childCategory();
    res = writeCategory (c);
    if (res != ERROR_CODE){
        cCats.setChildCategory(c);
        idChild = c.id();
    }
    else
        return cNull;

    if(cCats.hasChild2()){
        JKKSCategory c = cCats.child2Category();
        res = writeCategory (c);
        if (res != ERROR_CODE){
            cCats.setChild2Category(c);
            idChild2 = c.id();
        }
        else
            return cNull;
    }

    c = cCats.mainCategory();
    c.setIDChild(idChild);
    c.setIDChild2(idChild2);

    res = writeCategory (c);
    if (res != ERROR_CODE){
        cCats.setMainCategory(c);
    }
    else
        return cNull;
    
    return cCats;
}

QMap<qint64, JKKSCategory> JKKSLoader::pairToMap(const JKKSCategoryPair & pair) const
{
    QMap<qint64, JKKSCategory> newCats;
    
    newCats.insert(pair.mainCategory().id(), pair.mainCategory());
    
    if(!pair.isAlone()){
        newCats.insert(pair.childCategory().id(), pair.childCategory());
        if(pair.hasChild2())
            newCats.insert(pair.child2Category().id(), pair.child2Category());
    }

    return newCats;
}

int JKKSLoader::writeMessage (JKKSCommand *command) const
{
    int result = ERROR_CODE;
    QString sql;
    if (command && !command->getCategory().empty())
    {
	
		JKKSCategoryPair cCats = parseCategories(command->getCategory());        
		if(cCats.isNull())
			return ERROR_CODE;

        command->setCategory(pairToMap(cCats));

        JKKSDocument * doc = 0;
        int resDoc = ERROR_CODE;
        if (command->idObject() > 0)
        {
            doc = new JKKSDocument (command->getAttachedDoc());
            if (!doc || (resDoc = this->writeMessage(doc)) == ERROR_CODE)
            {
                if (doc)
                {
                    delete doc;
                    doc = 0;
                }
                return result;
            }
            command->setIdObject (doc->id());
        }
        
        QString msgBody = command->getMessageBody();
        if(!msgBody.isEmpty()){
            msgBody.replace("'", "''");
        }

        if(command->getJournalState() == 7){ //virtual
            sql = QString ("select * from uInsertVirtualOutCmd (%1, %2, %3, %4, %5, '%6', %7, %8, %9);")
                            .arg (getIdByUID("position", command->getDlFrom()))//dl_from_uid
                            .arg (getIdByUID("position", command->getDlExec()))//dl_exec_uid
                            .arg (getIdByUID("position", command->getDlTo()))//dl_to_uid
                            .arg (cCats.mainCategory().id())//id_category
                            .arg (command->getExecTime().isNull() ? QString("NULL") :
                                QString ("to_timestamp('") +
                                command->getExecTime().toString ("dd.MM.yyyy hh:mm:ss") +
                                QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp")) //exec_datetime
                            .arg (msgBody) //message_body
                            .arg (command->uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (command->uid())) //unique_id
                            .arg (command->idUrgencyLevel() > 0 ? command->idUrgencyLevel() : 1) //id_urgency_level
                            .arg (command->idObject() > 0 ? QString::number (command->idObject()) : QString("NULL")); //id_io_object_att
        }
        else{
            sql = QString ("select * from uInsertInCmd (%1, %2, '%3', %4, '%5', %6, '%7', %8, '%9', %10, %11, %12, '%13', %14, %15, %16, %17);")
                            .arg (command->id())//запишется как extra_id
                            .arg (getIdByUID("position", command->getDlFrom()))//dl_from_uid
                            .arg (command->getDlFromName()) //dl_from_name
                            .arg (getIdByUID("position", command->getDlExec()))//dl_exec_uid
                            .arg (command->getDlExecutorName())//dl_exec_name
                            .arg (getIdByUID("position", command->getDlTo()))//dl_to_uid
                            .arg (command->getDlToName())//dl_to_name
                            .arg (cCats.mainCategory().id())//id_category
                            .arg ("NULL")//categoryCode
                            .arg (command->getInsertTime().isNull() ? QString("NULL") :
                                QString ("to_timestamp('") +
                                command->getInsertTime().toString ("dd.MM.yyyy hh:mm:ss") +
                                QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp")) //insert_datetime
                            .arg (command->getExecTime().isNull() ? QString("NULL") :
                                QString ("to_timestamp('") +
                                command->getExecTime().toString ("dd.MM.yyyy hh:mm:ss") +
                                QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp")) //exec_datetime
                            .arg (command->getTimeInterval()) //exec_interval
                            .arg (msgBody) //message_body
                            .arg (command->uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (command->uid())) //unique_id
                            .arg (command->outputNumber().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (command->outputNumber())) //output_number
                            .arg (command->idUrgencyLevel() > 0 ? command->idUrgencyLevel() : 1) //id_urgency_level
                            .arg (command->idObject() > 0 ? QString::number (command->idObject()) : QString("NULL")); //id_io_object_att
        }

        KKSResult * res = dbWrite->execute (sql);
        
        if (res && res->getRowCount() == 1)
            result = res->getCellAsInt (0, 0);
        delete res;
    }
    
    if(result <= 0)
        kksCritical() << QObject::tr("Error in command creation! SQL = %1").arg(sql);
    
    return result;
}

QPair<qint64, JKKSCategory> JKKSLoader :: readCategory (qint64 idCat) const
{
    QPair<qint64, JKKSCategory> result;

    QString sql = QString ("select * from cGetCategory(%1)").arg (idCat);
    KKSResult * res = dbRead->execute (sql);

    if (res && res->getRowCount () == 1) {
        result.first = idCat;
        result.second = JKKSCategory(idCat,
                                res->getCellAsString (0, 3), // name
                                res->getCellAsString (0, 8), // code
                                res->getCellAsString (0, 4), // description
                                res->getCellAsInt (0, 1), // type
                                res->isEmpty(0, 2) ? -1 : res->getCellAsInt (0, 2), // child
                                res->isEmpty(0, 16) ? -1 : res->getCellAsInt(0, 16), //id_child2
                                res->getCellAsBool (0, 7), // main category
                                res->getCellAsString(0, 9), //uniqueID
                                res->getCellAsInt(0, 11), //id_io_state
                                res->getCellAsBool(0, 14) //is_global
                                );

        QMap<qint64, JKKSCategoryAttr> attrs = readCategoryAttrs(idCat);
        QMap<qint64, JKKSRubric> rubrs = readCategoryRubrics (idCat);
        
        result.second.setAttributes (attrs);
        result.second.setRubrics (rubrs);
    }

    delete res;
    return result;
}

QMap<qint64, JKKSCategory> JKKSLoader :: readCategories (qint64 idCat) const
{
    QMap<qint64, JKKSCategory> result;
    if(idCat > 0){

        QPair<qint64, JKKSCategory> category = readCategory(idCat);
        result.insert(category.first, category.second);

        if(category.second.getIDChild() > 0 ){
            QPair<qint64, JKKSCategory> cat = readCategory(category.second.getIDChild());
            result.insert(cat.first, cat.second);
        }

        if(category.second.getIDChild2() > 0){
            QPair<qint64, JKKSCategory> cat = readCategory(category.second.getIDChild2());
            result.insert(cat.first, cat.second);
        }
    }

    return result;
}

int JKKSLoader :: writeCategory (JKKSCategory& cat) const
{
    int result = ERROR_CODE;

    if (!cat.getName().isEmpty())
    {
        QString sql = QString ("select * from cInsertEx('%1', %2, %3, %4, %5, %6, %7, %8, %9, %10, %11);")
                                .arg (cat.getName())
                                .arg (QString ("NULL"))//FIXME
                                .arg (cat.getDescription().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (cat.getDescription()))
                                .arg (cat.getType())
                                .arg (cat.getIDChild() < 0 ? QString("NULL") : QString::number (cat.getIDChild()))
                                .arg (cat.isMain() ? QString("true") : QString("false"))
                                .arg (cat.uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (cat.uid()))
                                .arg (cat.isGlobal() ? QString("true") : QString("false"))
                                .arg (cat.getState())
                                .arg (cat.getIDChild2() < 0 ? QString("NULL") : QString::number (cat.getIDChild2()))
                                .arg (QString("NULL") );


        KKSResult *res = dbWrite->execute (sql);

        if (res && res->getRowCount () == 1)
        {
            int idc = res->getCellAsInt (0, 0);
            delete res;

            cat.setId (idc);
            int ok = writeCategoryAttrs (cat);
            if(ok <= 0)
                return ERROR_CODE;

            ok = writeCategoryRubrics (cat);
            if(ok <= 0)
                return ERROR_CODE;

            result = OK_CODE;
        }
        else{
            if (res)
                delete res;
        }
    }

    if(result <= 0){
        kksCritical() << QObject::tr("cinsert() for categoryUID = %1 failed!").arg(cat.uid());
    }

    return result;
}
QMap<qint64, JKKSCategoryAttr> JKKSLoader :: readAttrAttrs(qint64 idAttr) const
{
    QMap<qint64, JKKSCategoryAttr> attrs;
    QString sql = QString("select * from aGetAttrAttrs(%1) order by id_attribute").arg (idAttr);
    KKSResult * res = dbRead->execute(sql);

    if (res){
        int count = res->getRowCount();

        for (int i = 0; i < count; i++) {
            QString tableUid = QString();
            QString table = res->getCellAsString (i, 5);
            JKKSCategoryAttr attr (JKKSCategoryAttr (res->getCellAsInt (i, 0), // id
                                                     res->getCellAsInt (i, 1), // type
                                                     res->getCellAsString (i, 2), // code
                                                     res->getCellAsString (i, 3), // name
                                                     res->getCellAsString (i, 4), // title
                                                     table, // table
                                                     tableUid, // table UID
                                                     res->getCellAsString (i, 6), // column
                                                     res->getCellAsInt (i, 7), // default width
                                                     res->getCellAsString (i, 10), // def value
                                                     res->getCellAsBool (i, 11), // mandatory
                                                     res->getCellAsBool (i, 12), // read only
                                                     res->getCellAsString (i, 16) //uniqueID
                                                ));
            if (!table.isEmpty())
            {
                QString tSql = QString ("select unique_id from io_objects where table_name = '%1'").arg (table);
                KKSResult * tRes = dbRead->execute (tSql);
                if (!tRes || tRes->getRowCount() != 1)
                {
                    if (tRes)
                        delete tRes;
                    continue;
                }
                tableUid = tRes->getCellAsString (0, 0);
                attr.setTableUid (tableUid);
                delete tRes;
            }

            attr.setAttrAttrUid(res->getCellAsString(i, 22));

            attrs.insert (res->getCellAsInt (i, 0), attr);
        }

        delete res;
    }
    return attrs;
}

QMap<qint64, JKKSCategoryAttr> JKKSLoader :: readCategoryAttrs (qint64 idCat) const
{
    QMap<qint64, JKKSCategoryAttr> attrs;
    QString sql = QString("select * from cGetCategoryAttrs(%1) order by id_attribute").arg (idCat);
    KKSResult * res = dbRead->execute(sql);

    if (res){
        int count = res->getRowCount();

        for (int i = 0; i < count; i++) {
            QString tableUid = QString();
            QString table = res->getCellAsString (i, 5);
            JKKSCategoryAttr attr (JKKSCategoryAttr (res->getCellAsInt (i, 0), // id
                                                     res->getCellAsInt (i, 1), // type
                                                     res->getCellAsString (i, 2), // code
                                                     res->getCellAsString (i, 3), // name
                                                     res->getCellAsString (i, 4), // title
                                                     table, // table
                                                     tableUid, // table UID
                                                     res->getCellAsString (i, 6), // column
                                                     res->getCellAsInt (i, 7), // default width
                                                     res->getCellAsString (i, 10), // def value
                                                     res->getCellAsBool (i, 11), // mandatory
                                                     res->getCellAsBool (i, 12), // read only
                                                     res->getCellAsString (i, 16) //uniqueID
                                                ));
            if (!table.isEmpty())
            {
                QString tSql = QString ("select unique_id from io_objects where table_name='%1'").arg (table);
                KKSResult * tRes = dbRead->execute (tSql);
                if (!tRes || tRes->getRowCount() != 1)
                {
                    if (tRes)
                        delete tRes;
                    continue;
                }
                tableUid = tRes->getCellAsString (0, 0);
                attr.setTableUid (tableUid);
                delete tRes;
            }

            if(attr.idAttrType() == 32){ // atComplex
                QMap<qint64, JKKSCategoryAttr> attrs = readAttrAttrs(attr.id());
                attr.setAttrs(attrs);
            }

            attrs.insert (res->getCellAsInt (i, 0), attr);
        }

        delete res;
    }
    return attrs;
}

JKKSCategoryAttr JKKSLoader :: readAttribute (qint64 id) const
{
    QString sql = QString("select * from aGetAttribute ('%1');").arg (id);
    KKSResult * res = dbRead->execute(sql);

    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return JKKSCategoryAttr();
    }
    int i=0;
    QString tableUid = QString();
    QString table = res->getCellAsString (i, 5);
    JKKSCategoryAttr attr (JKKSCategoryAttr (res->getCellAsInt (i, 0), // id
                                             res->getCellAsInt (i, 1), // type
                                             res->getCellAsString (i, 2), // code
                                             res->getCellAsString (i, 3), // name
                                             res->getCellAsString (i, 4), // title
                                             table, // table
                                             tableUid, // table UID
                                             res->getCellAsString (i, 6), // column
                                             res->getCellAsInt (i, 7), // default width
                                             res->getCellAsString (i, 10), // def value
                                             res->getCellAsBool (i, 11), // mandatory
                                             res->getCellAsBool (i, 12), // read only
                                             res->getCellAsString (i, 16) //uniqueID
                                        ));

    if (!table.isEmpty())
    {
        QString tSql = QString ("select unique_id from io_objects where table_name='%1'").arg (table);
        KKSResult * tRes = dbRead->execute (tSql);
        if (!tRes || tRes->getRowCount() != 1)
        {
            if (tRes)
                delete tRes;
            return JKKSCategoryAttr();
        }
        tableUid = tRes->getCellAsString (0, 0);
        attr.setTableUid (tableUid);
        delete tRes;
    }

    if(attr.idAttrType() == 32){ // atComplex
        QMap<qint64, JKKSCategoryAttr> attrs = readAttrAttrs(attr.id());
        attr.setAttrs(attrs);
    }

    delete res;
    return attr;
}

int JKKSLoader :: writeCategoryAttrs (const JKKSCategory& cat) const
{
    if (cat.id() < 0)
        return ERROR_CODE;

    QMap<qint64, JKKSCategoryAttr> attributes = cat.attributes ();
    for (QMap<qint64, JKKSCategoryAttr>::const_iterator pa = attributes.constBegin();
         pa != attributes.constEnd();
         pa++) 
    {
        if (pa.value().code().isEmpty() || pa.value().name().isEmpty() || pa.value().title().isEmpty())
            continue;

        QString tableUid = pa.value().getTableUID ();

        QString sql = QString ("select * from acInsert (%1, %2, '%3', '%4', '%5', %6, %7, %8, %9, %10, %11, %12);")
                                .arg (cat.id())
                                .arg (pa.value().idAttrType())
                                .arg (pa.value().code())
                                .arg (pa.value().name())
                                .arg (pa.value().title())
                                .arg (tableUid.isEmpty() ? QString ("NULL") : QString ("'")+tableUid+QString("'"))
                                .arg (pa.value().column().isEmpty() ? QString ("NULL") : QString ("'")+pa.value().column()+QString("'"))
                                .arg (pa.value().defWidth())
                                .arg (pa.value().defValue().isEmpty() ? QString ("NULL") : QString ("'")+pa.value().defValue()+QString("'"))
                                .arg (pa.value().isMandatory() ? QString("true") : QString("false"))
                                .arg (pa.value().isReadOnly() ? QString("true") : QString("false"))
                                .arg (pa.value().uid().isEmpty() ? QString ("NULL") : QString ("'")+pa.value().uid()+QString("'"));

        KKSResult *res = dbWrite->execute (sql);
        if(!res || res->getRowCount() != 1 || res->getCellAsInt(0, 0) <= 0){
            
            kksCritical() << QObject::tr("acInsert() failed! Result = %1 SQL = %2").arg( res ? res->getCellAsString(0, 0) : "NULL" ).arg(sql);
            if(res)
                delete res;
            
            return ERROR_CODE;
            //continue;
        }

        JKKSCategoryAttr attr = pa.value();
        attr.setId(res->getCellAsInt(0, 0));
        delete res;

        if(attr.idAttrType() == 32){ //atComplex
            int res = writeAttrAttrs(attr);
            if(res == ERROR_CODE)
                return ERROR_CODE;
        }
    }

    return OK_CODE;
}

int JKKSLoader :: writeAttrAttrs (const JKKSCategoryAttr & attr) const
{
    if (attr.id() < 0)
        return ERROR_CODE;

    QMap<qint64, JKKSCategoryAttr> attributes = attr.attrs();
    for (QMap<qint64, JKKSCategoryAttr>::const_iterator pa = attributes.constBegin();
         pa != attributes.constEnd();
         pa++) 
    {
        if (pa.value().code().isEmpty() || pa.value().name().isEmpty() || pa.value().title().isEmpty())
            continue;

        QString tableUid = pa.value().getTableUID ();

        QString sql = QString ("select * from aaInsert (%1, %2, '%3', '%4', '%5', %6, %7, %8, %9, %10, %11, %12, %13);")
                                .arg (attr.id())
                                .arg (pa.value().idAttrType())
                                .arg (pa.value().code())
                                .arg (pa.value().name())
                                .arg (pa.value().title())
                                .arg (tableUid.isEmpty() ? QString ("NULL") : QString ("'")+tableUid+QString("'"))
                                .arg (pa.value().column().isEmpty() ? QString ("NULL") : QString ("'")+pa.value().column()+QString("'"))
                                .arg (pa.value().defWidth())
                                .arg (pa.value().defValue().isEmpty() ? QString ("NULL") : QString ("'")+pa.value().defValue()+QString("'"))
                                .arg (pa.value().isMandatory() ? QString("true") : QString("false"))
                                .arg (pa.value().isReadOnly() ? QString("true") : QString("false"))
                                .arg (pa.value().uid().isEmpty() ? QString ("NULL") : QString ("'")+pa.value().uid()+QString("'"))
                                .arg (pa.value().attrAttrUid().isEmpty() ? QString ("NULL") : QString ("'")+pa.value().attrAttrUid()+QString("'"));

        KKSResult *res = dbWrite->execute (sql);
        if(!res || res->getRowCount() != 1 || res->getCellAsInt(0, 0) <= 0){
            kksCritical() << QObject::tr("aaInsert() for complex attribute fail! SQL = %1").arg(sql);
            if(res)
                delete res;
            
            return ERROR_CODE;
            //continue;
        }

        delete res;
    }

    return OK_CODE;
}

QMap<qint64, JKKSRubric> JKKSLoader :: readCategoryRubrics (qint64 idCat) const
{
    QMap<qint64, JKKSRubric> rubrs;

    if (idCat < 0)
        return rubrs;

    QString sql = QString("select * from cGetRubrics(%1)").arg (idCat);
    KKSResult * res = dbRead->execute(sql);

    if (res)
    {
        int count = res->getRowCount();

        for (int i = 0; i < count; i++) {
            JKKSRubric rubr(JKKSRubric (res->getCellAsInt (i, 0), // id
                                        QString(), //uid
                                        res->getCellAsInt (i, 1), // id_parent
                                        res->getCellAsInt (i, 2), // id_category
                                        res->getCellAsString (i, 3) // name
                                        ));
            rubrs.insert (res->getCellAsInt (i, 0), rubr);
        }

        delete res;
    }

    return rubrs;
}

int JKKSLoader :: writeCategoryRubrics (const JKKSCategory& cat) const
{
    qint64 idCat = cat.id ();
    if (idCat < 0)
        return ERROR_CODE;

    QMap<qint64, JKKSRubric> rubrs = cat.rubrics ();
    QMap<qint64, qint64> idpRub;
    for (QMap<qint64, JKKSRubric>::const_iterator pr = rubrs.constBegin (); \
            pr != rubrs.constEnd (); \
            pr++)
    {
        if (pr.key () < 0)
            continue;

        JKKSRubric wRubr = pr.value ();
        if (wRubr.getParent () >= 0 && idpRub.constFind (wRubr.getParent ()) != idpRub.constEnd ())
            wRubr.setParent (idpRub.constFind (wRubr.getParent ()).value ());

        QString sql =  QString ("select * from cInsertRubric (%1, %2, '%3');")
                                .arg (wRubr.getParent () <= 0 ? QString ("NULL") : QString::number (wRubr.getParent ()))
                                .arg (pr == rubrs.constBegin () ? QString::number (cat.id()) : QString ("NULL"))
                                .arg (wRubr.getName ());

        KKSResult * res = dbWrite->execute (sql);
        if(!res || res->getRowCount() != 1 || res->getCellAsInt64(0, 0) <= 0){
            kksCritical() << QObject::tr("cInsertRubric() fail! SQL = %1").arg(sql);
            if(res)
                delete res;
            
            return ERROR_CODE;
        }

        idpRub.insert (wRubr.getIdRubric (), res->getCellAsInt64 (0, 0));

        if (res)
            delete res;
    }

    return OK_CODE;
}

int JKKSLoader :: setAsSended (qint64 id, int idType, bool sended) const
{
    
    QString sql;
    
    if(sended)
        sql = QString ("select * from uSetAsSended (%1, %2);").arg (id).arg (idType);
    else
        sql = QString ("select * from uSetAsNotSended (%1, %2);").arg (id).arg (idType);

    KKSResult *res = dbRead->execute (sql);

    int ier = ERROR_CODE;
    if (res && res->getRowCount()==1)
        ier = res->getCellAsInt (0, 0);
    else{
        if(sended)
            kksCritical() << QObject::tr("setAsSended failed! SQL = %1").arg(sql);
        else
            kksCritical() << QObject::tr("setAsNotSended failed! SQL = %1").arg(sql);
    }
    
    if(ier <= 0){
        if(sended)
            kksCritical() << QObject::tr("Result setAsSended is %1! SQL = %2").arg(ier).arg(sql);
        else
            kksCritical() << QObject::tr("Result setAsNotSended is %1! SQL = %2").arg(ier).arg(sql);
    }
    
    if (res)
        delete res;

    return ier;
}

//В данном методе мы не вычитываем содержимое блока файла, поскольку файл передается поблочно и все его части передаются отдельно путем вызова метода readFilePartData()
//Здесь мы просто получаем необходимую информацию о том, какие файлы требуют поблочной передачи 
//ВАЖНО: файлы размером менее _MAX_FILE_BLOCK2 передаются полностью в методе readDocumentFiles() 
QList<JKKSFilePart*> JKKSLoader :: readFileParts(QStringList & receivers) const
{
    QList<JKKSFilePart *> parts;
    
    QString sql = QString ("select * from getOutFileParts ();");
    KKSResult *res = dbRead->execute (sql);

    if (res)
    {
        for (int i=0; i<res->getRowCount (); i++)
        {
            JKKSAddress addr (res->getCellAsString(i, 0), res->getCellAsInt(i, 9), res->getCellAsBool(i, 11));

            JKKSFilePart * part = new JKKSFilePart();
            part->setAddr(addr);//full_addres of receiver
            part->setSenderAddr (getLocalAddress());//address of local org (sender)
            part->setId(res->getCellAsInt64(i, 1));//id_queue
            
            part->setIdUrl(res->getCellAsInt (i, 3));
            part->setAbsUrl(res->getCellAsString(i, 5));
            part->setIsLast(false);
            part->setUid(res->getCellAsString(i, 4));

            part->setReceiverUID(res->getCellAsString(i, 8));
            part->setSenderUID(this->senderUID);
                        
            if(!receivers.contains(part->receiverUID()))
                receivers.append(part->receiverUID());


            parts.append(part);
        }
        delete res;
    }

    return parts;
}

QByteArray JKKSLoader :: readFilePartData(const QString & absUrl, qint64 blockSize, qint64 position, qint64 * readed) const
{
    QByteArray bytea;

    QByteArray fPath = absUrl.toUtf8();
    const char * filePath = fPath.constData();

    char * command = new char[120 + strlen(filePath)];

    sprintf (command, "select rGetFileByUrl('%s', %lld, $1::int8);", filePath, blockSize);

    int  paramTypes[1];
    int  paramLengths[1]; 
    int  paramFormats[1];
    char ** paramValues = new char * [1];
    paramValues[0] = new char[40];

    sprintf (paramValues[0], "%lld", position);
    paramTypes[0]   = KKSResult::dtInt8;
    paramFormats[0] = 0; //text
    paramLengths[0]  = 0;

    KKSResult * res = dbRead->execParams (command, 
                                          1, 
                                          paramTypes, 
                                          paramValues, 
                                          paramLengths, 
                                          paramFormats, 
                                          1);

    if (!res || res->getRowCount() < 1)
    {
        if(res) 
            delete res;
        delete[] paramValues[0];
        delete[] paramValues;
        delete[] command;
        if(readed)
            *readed = -1;
        
        return bytea;
    }

    bytea = res->getCellAsByteArray(0, 0);
    qint64 size = bytea.size();

    if(res)
        delete res;

    delete[] paramValues[0];
    delete[] paramValues;
    delete[] command;
    
    if(readed)
        *readed = size;
    
    return bytea;
}

QMap<qint64, JKKSIOUrl> JKKSLoader :: readDocumentFiles (qint64 idObject, qint64 idOrganization) const
{
    QMap<qint64, JKKSIOUrl> urls;
    QString sql = QString ("select * from ioGetFiles (%1);").arg (idObject);
    KKSResult *res = dbRead->execute (sql);

    if (res)
    {
        for (int i=0; i<res->getRowCount (); i++)
        {
            qint64 idUrl = res->getCellAsInt (i, 1);
            JKKSIOUrl res_url (idObject, // idObject
                               idUrl,
                               res->getCellAsString (i, 2),
                               res->getCellAsString (i, 3),
                               res->getCellAsInt (i, 4),
                               res->getCellAsString (i, 8)
                               );
            res_url.setUid(res->getCellAsString (i, 9));
            
            qint64 size = getFileDataSize(idUrl);
            
            //если размер файла больше данного значения, то содержимое файла передается отдельно блоками (readFileParts())
            //в таблицу out_sync_queue записывается информация о файле, который надо поблочно передать
            //в этом случае res_url.m_data имеет пустое значение. На приемном конце этот факт должен анализироваться, и сам файл не записываться в ФС сервера
            //если же размер файла меньше данного значения, то содержимое файла передается вместе с пакетом (традиционный вариант), поблочной передачи не происходит
            if(size < _MAX_FILE_BLOCK2){
                QByteArray b = getFileData (idUrl);
                res_url.setData (b);
            }
            else{
                QString sql = QString("select addSyncRecord(%1, %2, '%3', '%4', '%5', %6, %7)")
                                .arg(idOrganization)
                                .arg(idUrl)
                                .arg(res_url.uid())
                                .arg("localorg-io_objects-7")//в настоящее время справочника прикрепленных файлов в системе не существует. 
                                                              //Тем не менее в любом случае это значение использоваться не будет, 
                                                              //поэтому задаем любое, например, UID справочника ИО
                                .arg("io_urls")//название таблицы, содержащей пересылаемую сущность (для нас io_urls)
                                .arg(2)//тип синхронизации (обновляем, или создаем новый, если не существует)
                                .arg(12);//тип пересылаемой сущности (для нас - прикрепленные файлы (частями))

                KKSResult * res = dbRead->execute(sql);
                if(!res || res->getRowCount() <= 0){
                    kksCritical() << QObject::tr("addSyncRecord() error! %1").arg(res ? res->errorMessage() : "");
                    if(res)
                        delete res;
                    //return 
                }

                delete res;
            }
            
            urls.insert (idUrl, res_url);
        }
        delete res;
    }

    return urls;
}
qint64 JKKSLoader::getFileDataSize(qint64 idUrl) const
{
    QString sql = QString ("select * from rGetFileSize(%1);").arg (idUrl);

    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return 0;

    qint64 size = res->getCellAsInt64 (0, 0);
    if(size <= 0)
        return 0;

    return size;
}

qint64 JKKSLoader :: writeDocumentFile (JKKSIOUrl& url) const
{
    QString url_ins_sql = QString ("select * from rInsertUrl ('%1', '%2', 'not assigned', %3, '%4');")
                                  .arg (url.uid())
                                  .arg (url.getIOURL())
                                  //.arg (url.getURL())
                                  .arg (url.getType())
                                  .arg (url.getSrcExt ());

    KKSResult * url_res = dbWrite->execute (url_ins_sql);
    if (!url_res)
        return ERROR_CODE;

    qint64 id = url_res->getCellAsInt (0, 0);
    if(id <= 0)
        return ERROR_CODE;

    url.setURLId (id);

    delete url_res;

    QString sql = QString ("select * from ioInsertUrl (%1, %2, '%3');")
                          .arg (url.getIOId ())
                          .arg (url.getURLId ())
                          .arg (url.getURL ());

    KKSResult * res = dbWrite->execute (sql);

    if (!res)
        return ERROR_CODE;

    delete res;

    int ier = this->writeFileData (url);
    
    return ier <= 0 ? ERROR_CODE : OK_CODE;
}

qint64 JKKSLoader :: writeRecordFile (JKKSIOUrl& url) const
{
    QString url_ins_sql = QString ("select * from rInsertUrl ('%1', '%2', 'not assigned', %3, '%4');")
                                  .arg (url.uid())
                                  .arg (url.getIOURL())
                                  //.arg (url.getURL())
                                  .arg (url.getType())
                                  .arg (url.getSrcExt ());

    KKSResult * url_res = dbWrite->execute (url_ins_sql);
    if (!url_res)
        return ERROR_CODE;

    qint64 id = url_res->getCellAsInt (0, 0);
    if(id <= 0)
        return ERROR_CODE;

    url.setURLId (id);

    delete url_res;

    QString sql = QString ("select * from recInsertUrl (%1, %2, '%3');")
                          .arg (url.getIOId ())
                          .arg (url.getURLId ())
                          .arg (url.getURL ());

    KKSResult * res = dbWrite->execute (sql);

    if (!res)
        return ERROR_CODE;

    delete res;

    int ier = this->writeFileData (url);
    
    return ier <= 0 ? ERROR_CODE : OK_CODE;
}


QByteArray JKKSLoader :: getFileData (qint64 idUrl, int blockSize) const
{
    QByteArray bytea;
    QString sql = QString("select rGetAbsUrl(%1);").arg(idUrl);

    KKSResult * url_res = dbRead->execute (sql);
    if (!url_res)
        return bytea;

    QString tmp = url_res->getCellAsString (0, 0);
    delete url_res;

    QByteArray fPath = tmp.toUtf8();
    const char * filePath = fPath.constData();

    char * command = new char[120 + strlen(filePath)];

    sprintf (command, "select rGetFileByUrl('%s', %d, $1::int8);", filePath, blockSize);

    qint64 position = 0;
    int  paramTypes[1];
    int  paramLengths[1]; 
    int  paramFormats[1];
    char ** paramValues = new char * [1];
    paramValues[0] = new char[40];

    do{
        sprintf (paramValues[0], "%lld", position);
        paramTypes[0]   = KKSResult::dtInt8;
        paramFormats[0] = 0; //text
        paramLengths[0]  = 0;

        KKSResult * res = dbRead->execParams (command, 
                                          1, 
                                          paramTypes, 
                                          paramValues, 
                                          paramLengths, 
                                          paramFormats, 
                                          1);

        position += blockSize;
        if (!res)
        {
            delete[] paramValues[0];
            delete[] paramValues;
            delete[] command;
            return bytea;
        }

        if (res->getRowCount() < 1)
        { 
            delete res;
            break;
        }

        if (res->isEmpty(0, 0))
        {
            delete res;
            break;
        }
        QByteArray buff (res->getCellAsByteArray(0, 0));
        bytea += buff;//res->getCellAsByteArray(0, 0);
        if  (buff.isEmpty())
        {
            delete res;
            break;
        }
        delete res;
    }while(1);

    return bytea;
}

int JKKSLoader :: writeFileData (const JKKSIOUrl& url, int blockSize) const
{
    QByteArray bytea = url.getData ();

    if (bytea.isEmpty ())
        return ERROR_CODE;

    qint64 idUrl = url.getURLId ();
    QByteArray fPath = url.getURL().toUtf8();
    const char * filePath = fPath.constData();

    char * command = new char[100 + strlen(filePath)];
    int nParams = 2;
    sprintf(command, "select rWriteFile(%d, $1, $2);", idUrl);
    
    int mode = 2;//safe ? 2 : 0;

    int  * paramTypes = new int[nParams];
    int  * paramLengths = new int[nParams]; 
    int  * paramFormats = new int[nParams];
    char ** paramValues = new char * [nParams];
    paramValues[0] = new char[blockSize];

    QBuffer file (&bytea);
    bool ok = file.open (QIODevice::ReadOnly);
    if (!ok)
        return ERROR_CODE;

    while (!file.atEnd() )
    {
        qint64 size = file.read (paramValues[0], blockSize);
        if(size < 1){
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;
            file.seek(0);
            file.close();
            return ERROR_CODE;
        }
        paramTypes[0]   = KKSResult::dtBytea;
        paramFormats[0] = 1; //binary
        paramLengths[0]  = size;

        paramTypes[1]   = KKSResult::dtInt4;
        paramFormats[1] = 0; //non binary
        paramLengths[1]  = 0;
        if(mode == 0)
            paramValues[1] = ((char *)"0");
        else if(mode == 1)
            paramValues[1] = ((char *)"1");
        else 
            paramValues[1] = ((char *)"2");

        KKSResult * res = dbWrite->execParams (command, 
                                          nParams, 
                                          paramTypes, 
                                          paramValues, 
                                          paramLengths, 
                                          paramFormats, 
                                          0);

        if (!res)
        {
            file.seek(0);
            file.close();
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;
            return ERROR_CODE;
        }
        int status = res->getCellAsInt(0, 0);
        if (status <= 0 )
        {
            file.seek(0);
            file.close();
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;
            return ERROR_CODE;
        }
        mode = 1;
        delete res;
    }
    delete[] paramValues[0];
    delete[] paramTypes;
    delete[] paramLengths;
    delete[] paramFormats;
    delete[] paramValues;
    delete[] command;

    file.seek(0);
    file.close();

    QString sql = QString("select rSetUploaded(%1, true)").arg(idUrl);
    
    KKSResult * res = dbWrite->execute(sql);
    
    if(!res || res->getRowCount() < 1){
        kksCritical() << QObject::tr("Cannot set file as uploaded! ID = %1").arg(idUrl);
        if(res)
            delete res;
        
        return ERROR_CODE;
    }

    if(res)
        delete res;

    return OK_CODE;
}

QMap<qint64, JKKSIOTable> JKKSLoader :: readDocumentTables (qint64 idObject) const
{
    QMap<qint64, JKKSIOTable> tables;
    QString sql = QString("select * from ioGetObjectTables (%1);").arg (idObject);
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return tables;

    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSIOTable T = JKKSIOTable (idObject, 
                                     res->getCellAsString (i, 1), 
                                     res->getCellAsString (i, 4), 
                                     res->getCellAsString (i, 5));
        QMap<qint64, JKKSCategory> cats = this->readPCategories (res->getCellAsInt (i, 3));
        T.setCategory (cats);
        tables.insert (res->getCellAsInt (i, 0), T);
    }

    return tables;
}

QMap<qint64, JKKSCategory> JKKSLoader :: readPCategories (qint64 idCatChild) const
{
    QMap<qint64, JKKSCategory> cats;
    QPair<qint64, JKKSCategory> C = readCategory (idCatChild);
    QPair<qint64, JKKSCategory> result;

    QString sql = QString ("select * from cGetParentCategory (%1)").arg (idCatChild);
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return cats;
    
    if (res && res->getRowCount () == 1)
    {
        int idCat = res->getCellAsInt (0, 0);
        result = readCategory (idCat);
    }

    delete res;
    cats.insert (result.first, result.second);
    cats.insert (C.first, C.second);

    if(result.second.getIDChild2() > 0){
        QPair<qint64, JKKSCategory> child2 = readCategory(result.second.getIDChild2());
        cats.insert(child2.first, child2.second);
    }
    
    return cats;
}

int JKKSLoader :: writeMessage (JKKSRefRecord *refRec, const QString& sender_uid) const
{
    if (!refRec){
        kksCritical() << QObject::tr("refRecord is NULL!");
        return ERROR_CODE;
    }

    kksDebug() << QString("JKKSRefRecord->getSenderAddr() = %1 port = %2").arg(refRec->getSenderAddr().address()).arg(refRec->getSenderAddr().port());
    kksDebug() << QString("JKKSRefRecord->getEntityType() = %1").arg(refRec->getEntityType());
    
    JKKSQueueResponse recResp (-1, refRec->getIDQueue(), 2, refRec->getSenderAddr());
    recResp.setOrgUid(sender_uid);

    if (refRec->getEntityType () == 1)//category
    {
        //parseCategories в том числе и записывает категории в БД
        JKKSCategoryPair cCats = parseCategories(refRec->getCategory());
        if (cCats.isNull())
            recResp.setResult(4);
        else
            recResp.setResult(3);

        int res1 = generateQueueResponse (recResp);
        if(res1 == ERROR_CODE)
            return ERROR_CODE;

        refRec->setCategory (pairToMap(cCats));
        
        return OK_CODE;
    }
    else if (refRec->getEntityType () == 2)//io_object
    {
        JKKSDocument ioDoc = refRec->getIODoc ();
        ioDoc.setIdState (1);
        int syncType = refRec->getSyncType();//тип синхронизации (1 - инсерт, 2 - апдейт, 3 - удаление)
        
        int res = writeMessage (&ioDoc, syncType);
        if (res > 0)
            recResp.setResult (3);
        else
            recResp.setResult (4);
        
        int res1 = generateQueueResponse (recResp);
        if(res1 == ERROR_CODE)
            return ERROR_CODE;        
        
        return res;
    }
    else if (refRec->getEntityType () == 3 || refRec->getEntityType () == 4) // ЭИО (запись справочника)
                                                                             // ЭИО (запись доп. таблицы справочника)
    {
        QString tableUID = refRec->getTableUID ();//unique_id информационного объекта
        QString tableSql = QString ("select ioGetTableNameByUid ('%1');").arg (tableUID);
        KKSResult * tRes = dbWrite->execute (tableSql);

        if (!tRes || tRes->getRowCount () != 1)
        {
            kksCritical() << QObject::tr("writeMessage: ioGetTableNameByUid() fails! SQL = %1").arg(tableSql);
            if (tRes)
                delete tRes;

            recResp.setResult (4);
            
            int res1 = generateQueueResponse (recResp);
            if(res1 == ERROR_CODE)
                return ERROR_CODE;
            
            return ERROR_CODE;
        }

        QString newTableName = tRes->getCellAsString (0, 0);
        if(newTableName.isEmpty()){
            kksCritical() << QObject::tr("writeMessage: ioGetTableNameByUid() returns NULL value! TableUID = %1").arg(tableUID);
            recResp.setResult (4);

            int res1 = generateQueueResponse (recResp);
            if(res1 == ERROR_CODE)
                return ERROR_CODE;

            return ERROR_CODE;
        }

        delete tRes;

        if (refRec->uid().isEmpty()){
            kksCritical() << QObject::tr("refRecord UID is empty!");
            return ERROR_CODE;
        }
        
        QString recSql;
        if(newTableName == "organization"){
            recSql = QString ("select count(*) from %1 where email_prefix='%2'").arg (newTableName).arg (refRec->uid());
        }
        else{
            recSql = QString ("select count(*) from %1 where unique_id='%2'").arg (newTableName).arg (refRec->uid());
        }
                
        
        KKSResult * idRes = dbWrite->execute (recSql);
        
        if (!idRes || idRes->getRowCount () != 1)
        {
            kksCritical() << QObject::tr("SQL query failed! SQL = %1").arg(recSql);
            if (idRes)
                delete idRes;

            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }

        int cnt = idRes->getCellAsInt (0, 0);
        delete idRes;

        if (cnt != 0 && refRec->getSyncType() == 1)
        {
            recResp.setResult (3);
            
            int res1 = generateQueueResponse (recResp);
            if(res1 == ERROR_CODE)
                return ERROR_CODE;
            
            return OK_CODE;
        }
        else if (cnt == 0 && refRec->getSyncType() == 3)
        {
            kksCritical() << QObject::tr("writeMessage: refRec sync type = %1 but count of records with equal UID in database = %2").arg(refRec->getSyncType()).arg(cnt);
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }

        QString sql;//запрос для табличных атрибутов записи справочника
        QString sysSql;//запрос для системных параметров записи справочника (in q_base_table)

        if (refRec->getSyncType() == 1 || (cnt == 0 && refRec->getSyncType() == 2))
        {
            QPair<qint64, qint64> id_table_map = getIDMap (tableUID, *refRec);
            if (id_table_map.second > 0)
            {
                QPair<QString, qint64> rec (newTableName, id_table_map.first);
                records.insert (rec, id_table_map.second);
                recResp.setResult (3);
            }
            else
                recResp.setResult (4);
            
            int res1 = generateQueueResponse (recResp);
            if(res1 == ERROR_CODE)
                return ERROR_CODE;

            sql = QString();
                
            sysSql = QString();
        }
        else if (cnt > 0 && refRec->getSyncType() == 2) //обновление записи справочника
        {
            sql = QString ("select recUpdate ('%1',").arg (tableUID);//QString ("UPDATE %1 SET ").arg (newTableName);
            QMap<qint64, JKKSCategory> cat = refRec->getCategory();
            JKKSCategoryPair p = mapToPair(cat);
            if (p.isNull() || p.isAlone())
            {
                kksCritical() << QObject::tr("writeMessage: category is empty or inconsistent!");
                recResp.setResult (4);
                generateQueueResponse (recResp);
                return ERROR_CODE;
            }

            JKKSCategory C = p.childCategory();

            QStringList attrsVals = refRec->attrsValues ();
            int ic = attrsVals.count () - C.attributes().count();

            QString attrsUids = QString ("ARRAY[");
            QString attrsValues = QString ("ARRAY[");
            for (QMap<qint64, JKKSCategoryAttr>::const_iterator pa = C.attributes().constBegin(); 
                    pa != C.attributes().constEnd(); 
                    pa++)
            {
                if (pa.value().code() == "id")
                {
                    ic++;
                    continue;
                }

                if (pa.value().transferrable())
                {
                    attrsUids += QString ("'%1'").arg (pa.value().uid());
                    int a_type = pa.value().idAttrType();
                    if (a_type == 9 || //KKSAttrType::atString ||
                        a_type == 14 || //KKSAttrType::atFixString ||
                        a_type == 13 || //KKSAttrType::atText ||
                        a_type == 20) //KKSAttrType::atXMLDoc)
                    {
                        QString sVal (attrsVals[ic]);
                        QString escVal (sVal);
                        escVal.replace("'", "''");
                        escVal.replace("\\", "\\\\");
                        escVal.replace("\"", "\\\"");
                        kksDebug () << escVal;
                        attrsValues += QString("'%1'")
                                        .arg (escVal);//.isEmpty() ? "NULL::varchar" : value)
                    }
                    else
                        attrsValues += QString ("'%1'").arg (attrsVals[ic]);
                    if (ic < attrsVals.count()-1)
                    {
                        attrsUids += QString (",");
                        attrsValues += QString (",");
                    }
                    else
                    {
                        attrsUids += QString ("]");
                        attrsValues += QString ("]");
                    }
                }

                ic++;
            }
            sql += QString ("%1, %2, 'unique_id', '%3');").arg (attrsUids)
                                                        .arg (attrsValues)
                                                        .arg (refRec->uid());//QString (" WHERE unique_id = '%1'").arg (refRec->uid());
            kksDebug () << sql;

            sysSql = QString("select recSetSysParams(");
        }
        else if (cnt > 0 && refRec->getSyncType() == 3){//удаление записи справочника
            sql = QString ("DELETE FROM %1 WHERE unique_id = '%2'").arg (newTableName).arg (refRec->uid());
            sysSql = QString();
        }

        if (!sql.isEmpty())
        {
            KKSResult * res = dbWrite->execute (sql);
            if (!res || res->resultStatus() >= KKSResult::BadResponse || res->resultStatus() == KKSResult::EmptyQuery)
            {
                recResp.setResult (4);
                generateQueueResponse (recResp);
                kksCritical() << QObject::tr("SQL query failed! SQL = %1").arg(sql);
                if(res)
                    delete res;
                return ERROR_CODE;
            }

            qint64 idRec = 0;
            if(res->resultStatus() == KKSResult::TuplesOk){
                idRec = res->getCellAsInt64(0, 0);
            }
            delete res;
            
            if(idRec > 0){
                refRec->setId(idRec);
                //
                // Системные атрибуты
                //
                int ok = writeRefRecordSysParams(refRec);
                if(ok != OK_CODE)
                    return ERROR_CODE;

                //
                //обновление значений показателей (если показатель не существует, он будет создан)
                //
                ok = writeRefRecordIndicators(refRec);
                if(ok != OK_CODE)
                    return ERROR_CODE;

                //
                // обновляем рубрики записи справочника
                //
                ok = writeRefRecordRubrics(refRec);
                if(ok != OK_CODE)
                    return ERROR_CODE;

                //
                // обновляем прикрепленные файлы для записи справочника
                //
                ok = writeRefRecordFiles(refRec);
                if(ok != OK_CODE)
                    return ERROR_CODE;

            }

        }

        recResp.setResult (3);

        int res1 = generateQueueResponse (recResp);
        if(res1 == ERROR_CODE)
            return ERROR_CODE;
    }
    //Дополнительная таблица справочника (передается вместе с категорией, на основе которой она создана и UID информационного объекта, к которому она относится)
    else if (refRec->getEntityType () == 5)
    {
        QString ioUID= refRec->getTableUID ();
        QString sql = QString ("select iogetobjectidbyuid ('%1');").arg (ioUID);

        KKSResult * ioIDRes = dbWrite->execute (sql);
        if (!ioIDRes || ioIDRes->getRowCount() != 1)
        {
            kksCritical() << QObject::tr("iogetobjectidbyuid() for object UID = %1 failed!").arg(ioUID);
            if (ioIDRes)
                delete ioIDRes;
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }

        qint64 idObject = ioIDRes->getCellAsInt (0, 0);
        delete ioIDRes;
        JKKSIOTable table = refRec->getAddTable ();
        if (refRec->getSyncType() == 2)
        {
            //
            // Update для таблицы не положено
            //
            recResp.setResult (4);
            generateQueueResponse (recResp);
            kksCritical() << QString("ERROR in query! %1 object UID = %2 sync type = %3").arg(refRec->getEntityType()).arg(ioUID).arg(refRec->getSyncType());
            return ERROR_CODE;
        }
        else if (refRec->getSyncType() == 1)
        {
            JKKSIOTable table (refRec->getAddTable ());
            int wres = writeAddTable (idObject, table);
            if (wres == ERROR_CODE)
                recResp.setResult (4);
            else
                recResp.setResult (3);

            int res1 = generateQueueResponse (recResp);
            if(res1 == ERROR_CODE)
                return ERROR_CODE;

            return wres;
        }
        else if (refRec->getSyncType() == 3)
        {
            sql = QString ("DELETE FROM object_ref_tables WHERE unique_id = '%1'").arg (refRec->uid());
            KKSResult * res = dbWrite->execute (sql);
            if (!res)
            {
                recResp.setResult (4);
                generateQueueResponse (recResp);
                return ERROR_CODE;
            }

            recResp.setResult (3);

            int res1 = generateQueueResponse (recResp);
            if(res1 == ERROR_CODE)
                return ERROR_CODE;

            delete res;
        }
    }
    else if (refRec->getEntityType () == 6)
    {
        return this->writeOrganization (recResp);
    }
    else if (refRec->getEntityType () == 8 || refRec->getEntityType () == 9 )
    {
/*        JKKSDocument * orgDoc = new JKKSDocument (refRec->getIODoc());
        int ier = writeMessage (orgDoc);
        delete orgDoc;
        return ier;
*/
    }
    else if (refRec->getEntityType() == 10)
    {
        //
        // position
        //
        QString sql = QString ("select id from organization where email_prefix = '%1'").arg (sender_uid);
        KKSResult * res = dbWrite->execute (sql);
        if (!res || res->getRowCount() != 1)
        {
            if (res)
                delete res;
            recResp.setResult (4);
            kksCritical() << QObject::tr("ERROR in SQL query! SQL = %1").arg(sql);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }
        int id_org = res->getCellAsInt (0, 0);
        delete res;

        sql = QString ("select * from getpublicunit (%1);").arg (id_org);
        KKSResult * u_res = dbWrite->execute (sql);
        if (!u_res || u_res->getRowCount () != 1)
        {
            if (u_res)
                delete u_res;
            kksCritical() << QObject::tr("ERROR in SQL query! SQL = %1").arg(sql);
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }
        int id_unit = u_res->getCellAsInt (0, 0);
        delete u_res;

        sql = QString("select count(*) from \"position\" p where p.unique_id='%1'").arg (refRec->uid());
        KKSResult * cp_res = dbWrite->execute (sql);
        if (!cp_res || cp_res->getRowCount() != 1)
        {
            if (cp_res)
                delete cp_res;
            recResp.setResult (4);
            kksCritical() << QObject::tr("ERROR in SQL query! SQL = %1").arg(sql);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }
        int cpos = cp_res->getCellAsInt (0, 0);
        delete cp_res;

        if (cpos == 0)
            sql = QString("INSERT INTO \"position\" (unique_id, id_unit, id_maclabel, id_user_vrio, id_user, name, short_name, is_io, email_prefix, phone, is_public) VALUES ('%8', %1, %2, NULL, NULL, '%3', '%4', %5, '%6', '%7', TRUE);")
                .arg (id_unit)
                .arg (refRec->attrsValues ().at (7)) // id_maclabel
                .arg (refRec->attrsValues ().at (1)) // name
                .arg (refRec->attrsValues ().at (2)) // short_name
                .arg (refRec->attrsValues ().at (5) == "t" ? QString ("TRUE") : QString ("FALSE"))
                .arg (refRec->attrsValues ().at (10)) // email_prefix
                .arg (refRec->attrsValues ().at (12)) // phone
                .arg (refRec->uid()); // unique_id
        else
            sql = QString ("UPDATE \"position\" set id_unit=%1, id_maclabel=%2, name='%3', short_name='%4', is_io=%5, email_prefix='%6', phone='%7', is_public=%8 where unique_id='%9';")
                .arg (id_unit)
                .arg (refRec->attrsValues ().at (7)) // id_maclabel
                .arg (refRec->attrsValues ().at (1)) // name
                .arg (refRec->attrsValues ().at (2)) // short_name
                .arg (refRec->attrsValues ().at (5) == "t" ? QString ("TRUE") : QString ("FALSE"))
                .arg (refRec->attrsValues ().at (10)) // email_prefix
                .arg (refRec->attrsValues ().at (12)) // phone
                .arg (refRec->attrsValues ().at (11) == "t" ? QString ("TRUE") : QString ("FALSE"))
                .arg (refRec->uid()); // unique_id
        
        KKSResult * pRes = dbWrite->execute (sql);
        if (!pRes)
        {
            kksCritical() << QObject::tr("ERROR in SQL query! SQL = %1").arg(sql);
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }

        recResp.setResult (3);

        int res1 = generateQueueResponse (recResp);
        if(res1 == ERROR_CODE)
            return ERROR_CODE;

        delete pRes;
    }
    else if (refRec->getEntityType() == 11)
    {
        //
        // delete of position
        //
        QString sql = QString("select deletePublicDl('%1')").arg (refRec->uid());
        KKSResult * res = dbWrite->execute (sql);
        if (!res || res->getRowCount() != 1)
        {
            if (res)
                delete res;
            recResp.setResult (4);
            kksCritical() << QObject::tr("ERROR in SQL query! SQL = %1").arg(sql);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }
        int cpos = res->getCellAsInt (0, 0);
        delete res;
        if(cpos != 1)
            recResp.setResult (4);
        else
            recResp.setResult (3);
        
        int res1 = generateQueueResponse (recResp);
        if(res1 == ERROR_CODE)
            return ERROR_CODE;

    }
    else if(refRec->getEntityType() == 12 || refRec->getEntityType() == 13)
    {
        ; //файлы, передаваемые блоками. Такого быть здесь не может, поскольку стоит в хранимой процедуре фильтр. Данные типы обрабатываются отдельно
    } //end of refRec->getEntityType()


    return OK_CODE;
}

int JKKSLoader :: writeRefRecordSysParams(JKKSRefRecord * refRec) const
{
    
    if(!refRec || refRec->id() <= 0)
        return ERROR_CODE;

    QString sysSql = QString("select recSetSysParams(%1, %2, %3, %4, %5, %6, %7)")
                                       .arg(refRec->id())
                                       .arg(refRec->idState() <= 0 ? QString("1") : QString("%1").arg(refRec->idState()))
                                       .arg(refRec->uuid().isEmpty() ? QString("NULL") : QString("'%1'").arg(refRec->uuid()))
                                       .arg(refRec->uid().isEmpty() ? QString("NULL") : QString("'%1'").arg(refRec->uid()))
                                       .arg(refRec->rIcon().isEmpty() ? QString("NULL") : QString("'%1'").arg(refRec->rIcon()))
                                       .arg(!refRec->bgColor().isValid() ? QString("NULL") : QString("'%1'").arg(QString::number(refRec->bgColor().rgba())))
                                       .arg(!refRec->fgColor().isValid() ? QString("NULL") : QString("'%1'").arg(QString::number(refRec->fgColor().rgba())));

    KKSResult * tRecRes = dbWrite->execute(sysSql);
    if (!tRecRes || tRecRes->getRowCount() != 1 || tRecRes->getCellAsInt64(0, 0) <= -1)
    {
        kksCritical() << QObject::tr("Cannot insert system parameters for the record! SQL = %1").arg(sysSql);
        if (tRecRes)
            delete tRecRes;
        
        return ERROR_CODE;
    }

    delete tRecRes;
    
    return OK_CODE;
}

int JKKSLoader :: writeRefRecordIndicators(JKKSRefRecord *refRec) const
{
    if(!refRec || refRec->id() <= 0)
        return ERROR_CODE;
    
    QMap<QString, QString> attrVals = refRec->indValues ();
    for (QMap<QString, QString>::const_iterator pa = attrVals.constBegin(); pa != attrVals.constEnd(); pa++)
    {
        QString val (pa.value());
        if (val.startsWith("'")){
            val = val.mid (1);
            if (val.endsWith("'"))
                val = val.mid (0, val.length()-1);
        }
        val = val.trimmed();
        
        if(!val.isEmpty()){ //если строка не пустая, то заэскейпим служебные символы, если таковые имеются в строке
            val.replace("'", "''");
            //val.replace("\\", "\\\\");
            //val.replace("\"", "\\\"");
            val.prepend("'");
            val.append ("'");
        }
        QString indSql = QString ("select * from eioUpdateIndicatorEx (%1, %2, %3, NULL::timestamp, NULL::timestamp, NULL::int4, NULL::int4, NULL::varchar);")
            .arg (refRec->id())
            .arg (QString("'")+pa.key()+QString("'")) //здесь в качестве ключа используется unique_id
            .arg (val.isEmpty() ? QString ("NULL") : val);
        
        KKSResult * indRes = dbWrite->execute (indSql);

        qint64 idRecAttrValue = ERROR_CODE;
        if (indRes)
        {
            idRecAttrValue = indRes->getCellAsInt64 (0, 0);
            delete indRes;
        }
        if (idRecAttrValue <= 0){
            kksCritical() << QObject::tr("eioUpdateIndicatorEx() for record with UID = %1 failed!").arg(refRec->uid());
            return ERROR_CODE;
        }
    }

    return OK_CODE;
}

int JKKSLoader :: writeRefRecordRubrics(JKKSRefRecord * refRec) const
{
    if(!refRec || refRec->id() <= 0)
        return ERROR_CODE;

    QMap<qint64, JKKSGlobalRubric> rubrs = refRec->rubrics ();
    QMap<qint64, qint64> idpRub;//здесь храним соответствие идентификатора рубрики в исходной БД и целевой БД (в процессе создания рубрик исходный родитель обновляется на целевой)
    
    for (QMap<qint64, JKKSGlobalRubric>::const_iterator pr = rubrs.constBegin (); \
            pr != rubrs.constEnd (); \
            pr++)
    {
        if (pr.key () < 0)
            continue;

        JKKSGlobalRubric wRubr = pr.value ();
        
        //если рубрика имеет родителя, обновляем информацию об этом родителе на актуальное значение
        if (wRubr.getParent () >= 0 && idpRub.constFind (wRubr.getParent ()) != idpRub.constEnd ())
            wRubr.setParent (idpRub.constFind (wRubr.getParent ()).value ());

        //пока у рубрик в записях справочников нет категории и поискового запроса
        /*
        qint64 idChild = -1;
        if (wRubr.getCategory().count() != 0)
        {
            JKKSCategoryPair cCats = parseCategories (wRubr.getCategory());
            if(cCats.isNull()){
                kksCritical() << __PRETTY_FUNCTION__ << "ERROR! parseCategories() for rubrics for refRecord with UID = " << refRec->uid() << " failed!";
                return ERROR_CODE;
            }
            
            idChild = cCats.mainCategory().id();
        }

        qint64 idSearchT = -1;
        if (wRubr.getSearchTemplate().id() > 0)
        {
            idSearchT = writeSearchTemplate (wRubr.getSearchTemplate());
        }
        */

        QString sql =  QString ("select * from recUpdateRubricEx('%1', '%2', '%3', %4, %5, NULL);")
                                .arg (wRubr.getUid  ())                    
                                .arg (wRubr.getName ())
                                .arg (wRubr.getDesc ())
                                .arg (wRubr.getParent () <= 0 ? QString ("NULL::int8") : QString::number (wRubr.getParent ()))
                                .arg (pr == rubrs.constBegin () ? QString::number (refRec->id()) : QString ("NULL::int8"));

        KKSResult * res = dbWrite->execute (sql);
        
        if (!res || res->getRowCount () != 1 || res->getCellAsInt64(0, 0) <= 0){
            kksCritical() << QObject::tr("recUpdateRubricEx() for refRecord with UID = %1 failed!").arg(refRec->uid());
            if(res)
                delete res;
            return ERROR_CODE;
        }
        else
            idpRub.insert (wRubr.getIdRubric (), res->getCellAsInt64 (0, 0));

        if (res)
            delete res;
    }

    return OK_CODE;
}

int JKKSLoader :: writeRefRecordFiles(JKKSRefRecord * refRec) const
{
    if(!refRec || refRec->id() <= 0)
        return ERROR_CODE;

    QMap<qint64, JKKSIOUrl> urls = refRec->urls ();
    for (QMap<qint64, JKKSIOUrl>::const_iterator pf = urls.constBegin (); \
            pf != urls.constEnd (); \
            pf++)
    {
        JKKSIOUrl url = pf.value ();
        url.setIOId (refRec->id ());
        int res = writeRecordFile (url);
        if(res <= 0)
            return ERROR_CODE;
    }

    return OK_CODE;
}

int JKKSLoader :: writeMessage (JKKSFilePart *filePart, const QString& sender_uid) const
{
    if (!filePart){
        kksCritical() << QObject::tr("filePart is NULL!");
        return ERROR_CODE;
    }

    kksDebug() << QString("JKKSFilePart->getSenderAddr() = ") << filePart->getSenderAddr().address() << QString(" port = ") << filePart->getSenderAddr().port();
    
    
    JKKSQueueResponse recResp (-1, filePart->id(), 2, filePart->getSenderAddr());
    recResp.setOrgUid(sender_uid);

    int ok = writeFilePartData(filePart);

    if(filePart->isLast()){
        if(ok != OK_CODE){ //все плохо
            recResp.setResult(4);
            return ERROR_CODE;
        }
        else{//все хорошо
            QString sql = QString("select rSetUploaded(%1, true)").arg(filePart->id());
            KKSResult * res = dbWrite->execute(sql);
            if(!res || res->getRowCount() < 1){
                kksCritical() << QObject::tr("Cannot set file as uploaded! ID = %1").arg(filePart->id());
                return ERROR_CODE;
            }
            if(res)
                delete res;

            recResp.setResult(3);
        }

        int res1 = generateQueueResponse (recResp);
        if(res1 == ERROR_CODE)
            return ERROR_CODE;
    }
    if(ok <= 0)
        return ERROR_CODE;

    return OK_CODE;
}

int JKKSLoader::writeFilePartData(JKKSFilePart * part) const
{
    if(!part)
        return ERROR_CODE;

    QByteArray bytea = part->getData();

    if (bytea.isEmpty ()){
        //возможно надо выставить флаг "файл записан"
        return OK_CODE;
    }

    QString uid = part->uid();
    QString sql = QString("select rGetAbsUrl('%1')").arg(uid);
    KKSResult * res = dbWrite->execute(sql);
    if(!res || res->getRowCount() < 1){
        if(res)
            delete res;

        kksCritical() << QObject::tr("Cannot get abs_url for the file! UID = %1").arg( uid );
        return ERROR_CODE;
    }

    QString absUrl = res->getCellAsString(0, 0);
    if(absUrl.isEmpty()){
        absUrl = "not assigned";
    }
    
    delete res;

    sql = QString("select getIdByUID('io_urls', '%1')").arg(uid);
    res = dbWrite->execute(sql);
    if(!res || res->getRowCount() < 1){
        if(res)
            delete res;

        kksCritical() << QObject::tr("Cannot get ID by UNIQUE_ID for the io_urls record! UID = %1").arg( uid );
        return ERROR_CODE;
    }

    qint64 idUrl = res->getCellAsInt64(0, 0);
    part->setId(idUrl);
    delete res;

    if(idUrl <= 0){
        kksCritical() << QObject::tr("Cannot get ID by UNIQUE_ID for the io_urls record! UID = %1").arg(uid);
        return ERROR_CODE;
    }
    
    QByteArray fPath = absUrl.toUtf8();
    const char * filePath = fPath.constData();

    
    
    char * command = new char[100 + strlen(filePath)];
    int nParams = 2;
    sprintf(command, "select rWriteFile(%d, $1, $2);", (int)idUrl);
    
    //int mode = 2; //это означает, что сначала будет осуществлена попытка чтения файла, и если онабудет успешной (файл существует) будет возврат с ошибкой (т.н. safe-mode)
                    //Однако этот вариант нам не подходит!! Поскольку мы получаем данные порциями. 
                    //Поэтому если пришла первая часть файла (part.isFirst() = true), то делаем mode = 0 (файл будет перезаписан)
                    //В противном случае и далее делаем mode = 1
    int mode = 1;
    if(part->isFirst())
        mode = 0;

    int  * paramTypes = new int[nParams];
    int  * paramLengths = new int[nParams]; 
    int  * paramFormats = new int[nParams];
    char ** paramValues = new char * [nParams];
    paramValues[0] = new char[_MAX_FILE_BLOCK2];

    QBuffer file (&bytea);
    bool ok = file.open (QIODevice::ReadOnly);
    if (!ok)
        return ERROR_CODE;

    while (!file.atEnd() )
    {
        qint64 size = file.read (paramValues[0], _MAX_FILE_BLOCK2);//в этом случае цикл будет отрабатывать не более одного раза, 
                                                                   //поскольку на конце-отправителе также читается по _MAX_FILE_BLOCK2 байт
        if(size < 1){
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;
            file.seek(0);
            file.close();

            return ERROR_CODE;
        }

        paramTypes[0]   = KKSResult::dtBytea;
        paramFormats[0] = 1; //binary
        paramLengths[0]  = size;

        paramTypes[1]   = KKSResult::dtInt4;
        paramFormats[1] = 0; //non binary
        paramLengths[1]  = 0;
        if(mode == 0)
            paramValues[1] = ((char *)"0");
        else if(mode == 1)
            paramValues[1] = ((char *)"1");
        else //данный вариант не работает (mode = 0 или 1)
            paramValues[1] = ((char *)"2");

        KKSResult * res = dbWrite->execParams (command, 
                                          nParams, 
                                          paramTypes, 
                                          paramValues, 
                                          paramLengths, 
                                          paramFormats, 
                                          0);

        if (!res)
        {
            file.seek(0);
            file.close();
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;

            return ERROR_CODE;
        }
        int status = res->getCellAsInt(0, 0);
        if (status <= 0 )
        {
            file.seek(0);
            file.close();
            delete[] paramValues[0];
            delete[] paramTypes;
            delete[] paramLengths;
            delete[] paramFormats;
            delete[] paramValues;
            delete[] command;

            return ERROR_CODE;
        }
        mode = 1;//далее в рамках работы этого метода считаем, что прочитанное из пришедшего буфера дописываем в конец файла, хотя в настоящее время на следующий шаг цикл переходить не должен
        delete res;
    }
    delete[] paramValues[0];
    delete[] paramTypes;
    delete[] paramLengths;
    delete[] paramFormats;
    delete[] paramValues;
    delete[] command;

    file.seek(0);
    file.close();

    return OK_CODE;
}

int JKKSLoader :: generateQueueResponse (JKKSQueueResponse & resp) const
{
    int res = writeReceipt (resp);
    if(res <= 0)
        return ERROR_CODE;

    return OK_CODE;
}

qint64 JKKSLoader :: writeOrganization (JKKSQueueResponse & resp) const
{
    QString sql = QString ("select kkssitoversion();");
    KKSResult * res = dbWrite->execute (sql);
    if (!res)
    {
        resp.setResult (4);
        generateQueueResponse (resp);
        return ERROR_CODE;
    }

    resp.setResult (3);

    int res1 = generateQueueResponse (resp);
    if(res1 == ERROR_CODE)
        return ERROR_CODE;

    delete res;
    return OK_CODE;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readTableRecords (QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uQueryOutQueue () order by 2;");
    if (res && res->getRowCount() > 0 )//&& res->getColumnCount() >= 10)
    {
        for (int i=0; i<res->getRowCount(); i++)
        {
            int entity_type = res->getCellAsInt (i, 7);
            qint64 id = res->getCellAsInt64 (i, 3);
            QString uid = res->getCellAsString (i, 4);
            
            QString a = res->getCellAsString (i, 0);
            int port = res->getCellAsInt(i, 11);
            bool useGateway = res->getCellAsBool(i, 12);
            JKKSAddress addr(a, port, useGateway);
            
            int sync_type = res->getCellAsInt (i, 8);
            qint64 idOrganization = res->getCellAsInt(i, 2);
            JKKSRefRecord rec (res->getCellAsInt64 (i, 1), // idQueue
                               id, // idRec
                               entity_type, // entity_type
                               res->getCellAsString (i, 6), // table name
                               sync_type, //sync_type
                               JKKSCategory(), //JKKSCategory
                               QStringList(), //attrsVals

                               QString(), //addr
                               QString(), //mess_code

                               res->getCellAsString (i, 5)// table UID (entity_io_uid)
                    );

            rec.setSenderAddr (getLocalAddress());
            QString receiverUID = res->getCellAsString (i, 10);//email_prefix

            rec.setUid (uid);
            rec.setAddr (addr);
	    
            kksDebug() << QString("senderAddress = ") << rec.getSenderAddr().address() << QString(" senderPort = ") << rec.getSenderAddr().port();

            //1 - категория (системная). Пересылается вместе с атрибутами.
            if (entity_type == 1)
            {
                QMap<qint64, JKKSCategory> cat = readCategories (id);
                rec.setCategory (cat);
            }
            //2 - ИО (системный).
            else if (entity_type == 2)
            {
                JKKSDocument doc (readDocument (id, idOrganization));
                doc.setCommandId (-1);//idCommand);
                doc.setJournal (-1);//idJournal);
                QDateTime sentTime = QDateTime::currentDateTime();
                doc.setRealTime (sentTime);

                rec.setIODoc (doc);
            }
            //3 - ЭИО (запись справочника). В этом случае необходимо указывать название таблицы БД, в которой хранятся записи справочника.
            //4 - ЭИО (запись дополнительной таблицы справочника). В этом случае необходимо указывать название таблицы БД, в которой хранятся записи справочника.            
            else if (entity_type == 3 || entity_type == 4) 
            {
                QString tableName = res->getCellAsString (i, 6);
                int ier = readRecordFromTable (tableName, rec, idOrganization);
                if (ier < 0)
                    continue;
            }
            //5 - Дополнительная таблица справочника (передается вместе с категорией, на основе которой она создана и UID информационного объекта, к которому она относится)
            else if (entity_type == 5)
            {
                qint64 idObject;
                JKKSIOTable table = this->readIOTable (rec.uid(), idObject);
                
                JKKSDocument doc (readDocument (idObject, idOrganization));
                doc.setCommandId (-1);//idCommand);
                doc.setJournal (-1);//idJournal);
                QDateTime sentTime = QDateTime::currentDateTime();
                doc.setRealTime (sentTime);

                rec.setIODoc (doc);
                rec.setAddTable (table);
            }
            //8 - передача полного справочника организаций на новый объект
            else if (entity_type == 8)
            {
                JKKSOrgPackage OP = readOrgs (rec.getIDQueue(), receiverUID);
                OP.setAddr (getLocalAddress());
                JKKSPMessage pM (OP.serialize(), OP.getMessageType());
                pM.setReceiverUID(receiverUID);
                pM.setSenderUID(this->senderUID);
                pM.setVerifyReceiver(false);
                JKKSPMessWithAddr * pMessOrg = new JKKSPMessWithAddr (pM, rec.getAddr(), OP.id());
                pMessOrg->unp = receiverUID;
                result.append (pMessOrg);

                if(!receivers.contains(pM.receiverUID()))
                    receivers.append(pM.receiverUID());
            }
            //9 - передача записи справочника организаций на все участвующие в инф. обмене объекты
            else if (entity_type == 9)
            {
                qint64 idOrg = rec.id(); //это id in organizations (id_entity in out_sync_queue)
                
                QMap<qint64, JKKSOrganization> orgs = readOrganizations (idOrg, rec.getIDQueue());
                QMap<qint64, JKKSOrganization>::const_iterator po = orgs.constBegin();
                for (; po != orgs.constEnd(); po++)
                {
                    QList<JKKSOrganization> orgList = orgs.values (po.key());//могут быть вставлены в QMap как insertMulti()
                    for (int ii=0; ii<orgList.count(); ii++)
                    {
                        JKKSOrganization org = orgList[ii];//po.value ();
                        org.setAddr (rec.getAddr());
                       
                        JKKSPMessage pM (org.serialize(), org.getMessageType());
                        pM.setSenderUID(this->senderUID);
                        pM.setReceiverUID(receiverUID);
                        
                        JKKSPMessWithAddr * pMessOrg = new JKKSPMessWithAddr (pM, rec.getAddr(), rec.getIDQueue());
                        pMessOrg->unp = receiverUID;
                        result.append (pMessOrg);

                        if(!receivers.contains(pM.receiverUID()))
                            receivers.append(pM.receiverUID());
                    }
                }
            }
            //10 - публичное ДЛ (создание, обновление)
            //11 - публичное ДЛ (удаление)
            else if (entity_type == 10 || entity_type == 11) //position create or delete
            {
                QString tableName = res->getCellAsString (i, 6);
                int ier = readRecordFromTable (tableName, rec, idOrganization);
                if (ier < 0)
                    continue;
            }
            //12 - файл, передаваемый блоками (создание, обновление)
            //13 - файл, передаваемый блоками (удаление)
            else if(entity_type == 12 || entity_type == 13)
            {
                ; //файлы, передаваемые блоками. Такого быть здесь не может, поскольку стоит в хранимой процедуре фильтр. Данные типы обрабатываются отдельно
            }
                        
            if(entity_type == 8 || entity_type == 9){//пакеты на отправку в этих случаях уже сформированы
                continue;
            }

            JKKSPMessage pM(rec.serialize(), rec.getMessageType(), this->senderUID, receiverUID);
            
            if(entity_type == 6){//запрос на проверку связи. В этом  случае надо тип пересылаемого сообщения установить в atEcho
                pM.setVerifyReceiver(false);               
                pM.setType(JKKSMessage::atEcho);
            }
            
            JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr (pM,
                                                                        rec.getAddr(),
                                                                        rec.getIDQueue());
            pMessWithAddr->unp = receiverUID;

            result.append (pMessWithAddr);

            if(!receivers.contains(pM.receiverUID()))
                receivers.append(pM.receiverUID());
        }
    }
    if (res)
        delete res;

    return result;
}

int JKKSLoader :: readRecordFromTable (const QString& tableName, JKKSRefRecord& rec, qint64 idOrganization) const
{
    int sync_type = rec.getSyncType();
    QString io_id_sql = QString("select ioGetObjectIDByTableName ('%1');").arg(tableName);
    KKSResult * io_id_res = dbRead->execute(io_id_sql);
    if (!io_id_res || io_id_res->getRowCount() != 1)
    {
        if (io_id_res)
            delete io_id_res;
        return ERROR_CODE;
    }
    qint64 io_id = io_id_res->getCellAsInt (0, 0);
    delete io_id_res;

    QString io_sql = QString("select * from ioGetObject(%1)").arg(io_id);
    KKSResult * io_res = dbRead->execute(io_sql);
    if (!io_res || io_res->getRowCount() != 1)
    {
        if (io_res)
            delete io_res;
        return ERROR_CODE;
    }
    
    qint64 idCat = io_res->getCellAsInt(0, 1);
    QString tableUID = io_res->getCellAsString(0, 14);//unique_id информационного объекта
    
    delete io_res;
    
    QMap<qint64, JKKSCategory> c_result = readCategories (idCat);
    if (c_result.count () < 2 || c_result.count () > 3) //у записи справочника могут быть также и показатели, в этом случае имеем одну категорию главную и две категории дочерние
        return ERROR_CODE;
    
    rec.setCategory (c_result);
    rec.setTableUID(tableUID);


    //1 - создать новую сущность на целевом объекте
    //2 - заменить сущность на данную на целевом объекте (если она не существует, создать новую)
    if (sync_type == 1 || sync_type == 2)
    {
        int ok = readRefRecordTableValues(rec, io_id);
        if(ok != OK_CODE)
            return ERROR_CODE;


        //показатели записи справочника
        ok = readRefRecordIndicators(rec);
        if(ok != OK_CODE)
            return ERROR_CODE;

        //рубрики
        ok = readRefRecordRubrics(rec);
        if(ok != OK_CODE)
            return ERROR_CODE;

        //прикрепленные файлы
        ok = readRefRecordFiles(rec, idOrganization);
        if(ok != OK_CODE)
            return ERROR_CODE;
    }

    return OK_CODE;
}

int JKKSLoader :: readRefRecordRubrics(JKKSRefRecord & rec) const
{
    //рубрики
    QString rubrSql = QString ("select * from recGetRubrics (%1);").arg (rec.id());
    
    KKSResult * rubrRes = dbRead->execute (rubrSql);
    if (!rubrRes)
        return ERROR_CODE;

    QMap<qint64, JKKSGlobalRubric> rubrs;
    for (int ii=0; ii<rubrRes->getRowCount(); ii++)
    {
        if (rubrRes->getCellAsInt (ii, 5) == 2)
        {
            //
            // rubric item cannot been sent
            //
            continue;
        }
        JKKSGlobalRubric wRubr (rubrRes->getCellAsInt64 (ii, 0), // id
                                rubrRes->getCellAsString(ii, 7), //uid
                                rubrRes->getCellAsInt64 (ii, 1) <= 0 ? -1 : rubrRes->getCellAsInt64 (ii, 1), // idParent
                                rubrRes->getCellAsInt64 (ii, 2) <= 0 ? -1 : rec.id(), // idRecord
                                rubrRes->getCellAsString (ii, 3), // name
                                rubrRes->getCellAsString (ii, 4) // description
                                );
        /*
        qint64 idCategory = rubrRes->getCellAsInt (ii, 4);
        if (idCategory > 0)
        {
            QMap<qint64, JKKSCategory> rubrCats = readCategories (idCategory);
            wRubr.setCategory (rubrCats);
        }
        
        qint64 idSearchTemplate = rubrRes->getCellAsInt (ii, 3);
        if (idSearchTemplate > 0)
        {
            JKKSSearchTemplate st = readSearchTemplate (idSearchTemplate);
            wRubr.setSearchTemplate (st);
        }
        */
        rubrs.insert (wRubr.getIdRubric(), wRubr);
    }

    rec.setRubrics (rubrs);

    return OK_CODE;
}

int JKKSLoader :: readRefRecordFiles(JKKSRefRecord & rec, qint64 idOrganization) const
{
    //прикрепленные файлы


    QMap<qint64, JKKSIOUrl> urls;
    QString sql = QString ("select * from recGetFiles (%1);").arg (rec.id());
    KKSResult *res = dbRead->execute (sql);

    if (res)
    {
        for (int i=0; i<res->getRowCount (); i++)
        {
            qint64 idUrl = res->getCellAsInt64 (i, 1);
            JKKSIOUrl res_url( rec.id(), // idRecord
                               idUrl,
                               res->getCellAsString (i, 2),//urls_records.name
                               res->getCellAsString (i, 3), //io_urls.name
                               res->getCellAsInt (i, 4), //type
                               res->getCellAsString (i, 8) //src_ext
                               );
            res_url.setUid(res->getCellAsString (i, 9));
            
            qint64 size = getFileDataSize(idUrl);
            
            //если размер файла больше данного значения, то содержимое файла передается отдельно блоками (readFileParts())
            //в таблицу out_sync_queue записывается информация о файле, который надо поблочно передать
            //в этом случае res_url.m_data имеет пустое значение. На приемном конце этот факт должен анализироваться, и сам файл не записываться в ФС сервера
            //если же размер файла меньше данного значения, то содержимое файла передается вместе с пакетом (традиционный вариант), поблочной передачи не происходит
            if(size < _MAX_FILE_BLOCK2){
                QByteArray b = getFileData (idUrl);
                res_url.setData (b);
            }
            else{
                QString sql = QString("select addSyncRecord(%1, %2, '%3', '%4', '%5', %6, %7)")
                                .arg(idOrganization)
                                .arg(idUrl)
                                .arg(res_url.uid())
                                .arg("localorg-io_objects-7")//в настоящее время справочника прикрепленных файлов в системе не существует. 
                                                              //Тем не менее в любом случае это значение использоваться не будет, 
                                                              //поэтому задаем любое, например, UID справочника ИО
                                .arg("io_urls")//название таблицы, содержащей пересылаемую сущность (для нас io_urls)
                                .arg(2)//тип синхронизации (обновляем, или создаем новый, если не существует)
                                .arg(12);//тип пересылаемой сущности (для нас - прикрепленные файлы (частями))

                KKSResult * res = dbRead->execute(sql);
                if(!res || res->getRowCount() <= 0){
                    kksCritical() << QObject::tr("addSyncRecord() error! %1").arg(res ? res->errorMessage() : "");
                    if(res)
                        delete res;
                    //return 
                }

                delete res;
            }
            
            urls.insert (idUrl, res_url);
        }
        delete res;
    }

    rec.setUrls (urls);

    return OK_CODE;
}


int JKKSLoader :: readRefRecordTableValues(JKKSRefRecord & rec, qint64 idObject) const
{
    QString dataSql;
    QString querySql = QString ("select * from getEIO (%1, %2)").arg (rec.id()).arg (idObject);

    KKSResult * qRes = dbRead->execute (querySql);
    if (!qRes || qRes->getRowCount() != 1)
    {
        if (qRes)
            delete qRes;
        return ERROR_CODE;
    }
    dataSql = qRes->getCellAsString (0, 0);
    delete qRes;

    KKSResult *dRes = dbRead->execute (dataSql);
    if (!dRes || dRes->getRowCount() != 1)
    {
        kksCritical() << QObject::tr("Error in SQL query! SQL = %1").arg(dataSql);
        if (dRes)
            delete dRes;
        return ERROR_CODE;
    }

    QStringList values;
    JKKSCategoryPair p = mapToPair(rec.getCategory());
    if(p.isNull() || p.isAlone()){
        kksCritical() << QObject::tr("Category is empty or inconsistent!");
        return ERROR_CODE;
    }

    JKKSCategory tc = p.childCategory();
    QMap<qint64, JKKSCategoryAttr>::const_iterator pa = tc.attributes().constBegin();
    
    rec.setUid(dRes->getCellAsString(0, 0));//unique_id
    rec.setUuid(dRes->getCellAsString(0, 1));//uuid_t
    rec.setIdState(dRes->getCellAsInt(0, 2));//id_io_state
    rec.setRIcon(dRes->getCellAsString(0, 3));//r_icon
    //bgColor
    QColor bkcolor = QColor();
    if (dRes->getCell (0, 4).isValid())
    {
        unsigned int vlc = dRes->getCellAsInt64(0, 4);
        bkcolor = QColor::fromRgba (vlc);
        rec.setBgColor(bkcolor);
    }
    //fgColor
    QColor fgcolor = QColor();
    if (dRes->getCell (0, 5).isValid())
    {
        unsigned int vltc = dRes->getCellAsInt64(0, 5);
        fgcolor = QColor::fromRgba (vltc);
        rec.setFgColor(fgcolor);
    }

    //пропустим первые 6 колонок из рез-та запроса - они системные и есть всегда
    for (int i=6; i < dRes->getColumnCount() && pa != tc.attributes().constEnd(); i++)
    {
        if (!dRes->getCellAsString (0, i).isEmpty() && 
               (pa.value().idAttrType () == 2 ||  //atList
                pa.value().idAttrType() == 3 ) && //atParent
            pa.value().table() != "maclabels")
        {
            //
            // т.е. если поле таблицы представляет собой ссылку на справочник и при этом не NULL
            //
            QString r_uid_sql;
            if(pa.value().table() == "organization")
                r_uid_sql = QString ("select email_prefix from %1 where id = %2").arg (pa.value().table()).arg (dRes->getCellAsString (0, i));
            else {
                if(pa.value().idAttrType() == 2)
                    r_uid_sql = QString ("select unique_id from %1 where id = %2").arg (pa.value().table()).arg (dRes->getCellAsString (0, i));
                else{
                    QString s = QString("select table_name from io_objects where id = %1").arg(idObject);
                     KKSResult * r = dbRead->execute(s);
                     if(!r || r->getRowCount() != 1){
                         if(r)
                             delete r;
                         return ERROR_CODE;
                     }
                     QString theTable = r->getCellAsString(0, 0);
                     delete r;
                     
                     if(theTable == "organization")
                         r_uid_sql = QString ("select email_prefix from %1 where id=%2").arg (theTable)  .arg  (dRes->getCellAsString (0, i));
                     else
                         r_uid_sql = QString ("select unique_id from %1 where id=%2").arg (theTable)  .arg  (dRes->getCellAsString (0, i));
                }
            }
            
            KKSResult * refres = dbRead->execute (r_uid_sql);
            if (!refres || refres->getRowCount () != 1)
            {
                if (refres)
                    delete refres;
                return ERROR_CODE;
            }

            values += refres->getCellAsString (0, 0);
            delete refres;
        }
        else if (!dRes->getCellAsString (0, i).isEmpty() && 
                   (//pa.value().idAttrType() == 12 ||  //atCheckList
                    pa.value().idAttrType() == 17) && //atCheckListEx
                pa.value().table() != "maclabels")
        {
            //
            // т.е. если поле таблицы представляет собой ссылку на справочник и при этом не NULL
            //
            QString r_uid_sql;
            if(pa.value().table() == "organization")
                r_uid_sql = QString ("select array_to_string(array_agg(email_prefix), ',') from %1 where id in (%2)").arg (pa.value().table()).arg (dRes->getCellAsString (0, i));
            else
                r_uid_sql = QString ("select array_to_string(array_agg(unique_id), ',') from %1 where id in (%2)").arg (pa.value().table()).arg (dRes->getCellAsString (0, i));
            
            KKSResult * refres = dbRead->execute (r_uid_sql);
            if (!refres || refres->getRowCount () != 1)
            {
                if (refres)
                    delete refres;
                return ERROR_CODE;
            }
            values += refres->getCellAsString (0, 0);
            delete refres;
        }
        else{
            values += dRes->getCellAsString (0, i);
        }
        
        //if (i>0 || tableName == QString("position"))
            pa++;
    }

    rec.setAttrsValues (values);

    return OK_CODE;
}

int JKKSLoader :: readRefRecordIndicators(JKKSRefRecord & rec) const
{
    
    QString indSql = QString ("select * from eioGetIndicatorsEx(%1)").arg (QString::number(rec.id()));
    KKSResult * aRes = dbRead->execute (indSql);
    QMap<QString, QString> indVals;
    if (!aRes)
        return ERROR_CODE;

    for (int ii=0; ii<aRes->getRowCount(); ii++)
    {
        indVals.insert (aRes->getCellAsString (ii, 4), aRes->getCellAsString (ii, 3));//здесь в качестве ключа используется поле unique_id
    }

    delete aRes;

    rec.setIndValues(indVals);

    return OK_CODE;
}

JKKSIOTable JKKSLoader :: readIOTable (QString entityuid, qint64& idObject) const
{
    QString sql = QString ("select * from object_ref_tables objt where objt.unique_id='%1';").arg (entityuid);

    KKSResult * res = dbRead->execute (sql);
    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;
        return JKKSIOTable();
    }

    idObject = res->getCellAsInt (0, 2);
    JKKSIOTable T (res->getCellAsInt (0, 0), \
                   res->getCellAsString (0, 1), \
                   res->getCellAsString (0, 4), \
                   res->getCellAsString (0, 5)
                   );

    qint64 idCat = res->getCellAsInt (0, 3);
    QMap<qint64, JKKSCategory> cats = readPCategories (idCat);
    T.setCategory (cats);

    delete res;
    return T;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readQueueResults (QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> result;// (queueResults);
    
    QString sql = QString ("select * from uGetQueueResults()");
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return result;
    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSAddress addr(res->getCellAsString (i, 0), 
                         res->getCellAsInt (i, 5),
                         res->getCellAsBool(i, 7));

        JKKSQueueResponse * resp = new JKKSQueueResponse (res->getCellAsInt64 (i, 1),
                                                          res->getCellAsInt64 (i, 3),
                                                          res->getCellAsInt (i, 4),
                                                          addr);
        resp->setOrgUid(res->getCellAsString(i, 6));

        JKKSPMessage pM(resp->serialize(), resp->getMessageType());
        pM.setVerifyReceiver(false);
        pM.setSenderUID(this->senderUID);
        pM.setReceiverUID(res->getCellAsString(i, 6)); //email_prefix

        kksDebug() << QString("Response Address = ") << resp->getAddr().address() << QString("Response port = ") << resp->getAddr().port();

        JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr (pM, resp->getAddr(), resp->id());
        pMessWithAddr->unp = res->getCellAsString(i, 6); //email_prefix
        if (pMessWithAddr)
            result.append (pMessWithAddr);

        if(!receivers.contains(pM.receiverUID()))
            receivers.append(pM.receiverUID());
    }
    return result;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readPingResults (QStringList & receivers) const
{
    QList<JKKSPMessWithAddr *> result;
    
    QString sql = QString ("select * from uGetPingResults()");
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return result;

    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSAddress addr(res->getCellAsString (i, 0), 
                         res->getCellAsInt (i, 5),
                         res->getCellAsBool(i, 7));

        int idOrgTo = res->getCellAsInt (i, 3);//здесь будет получено отрицательное число
        idOrgTo *= -1;

        JKKSPing * ping = new JKKSPing (res->getCellAsInt (i, 1),
                                        addr,
                                        QString(),
                                        true);
        
        ping->setCompleted(true);
        ping->setCreated(1);
        ping->setIdOrgTo(idOrgTo);
        ping->setState1(1);
        ping->setState2(1);
        ping->setState3(1);
        ping->setState4(res->getCellAsInt(i, 4) == 3 ? 1 : 0);
        ping->setUidFrom(res->getCellAsString(i, 6));//email_prefix
        ping->setUidTo(this->senderUID);//email_prefix
        //ping->setVersionTo();


        JKKSPMessage pM(ping->serialize(), ping->getMessageType());
        pM.setVerifyReceiver(false);
        pM.setSenderUID(this->senderUID);
        pM.setReceiverUID(res->getCellAsString(i, 6)); //email_prefix

        JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr (pM, ping->getAddr(), ping->id());
        pMessWithAddr->unp = res->getCellAsString(i, 6); //email_prefix
        if (pMessWithAddr)
            result.append (pMessWithAddr);

        if(!receivers.contains(pM.receiverUID()))
            receivers.append(pM.receiverUID());
    }
    
    return result;
}

int JKKSLoader :: writeMessage (JKKSQueueResponse *response) const
{
    if (!response)
        return ERROR_CODE;
    QString sql (QString("select setsyncresult (%1, %2);").arg (response->getExternalId()).arg (response->getResult()));
    kksDebug () << sql;

    KKSResult * res = dbWrite->execute (sql);
    if (!res)
        return ERROR_CODE;

    int ier = res->getCellAsInt (0, 0);
    delete res;
    return ier;
}

qint64 JKKSLoader :: writeReceipt (JKKSQueueResponse& response) const
{
    
    //external_id Может быть отрицательным для ответов на пинги!
    //if (response.getExternalId() < 0)
    //    return ERROR_CODE;

    QString sql (QString("select addQueueResult (%1, %2, '%3', %4, '%5', %6);")
                         .arg (response.getExternalId())
                         .arg (response.getResult())
                         .arg (response.getAddr().address())
                         .arg (response.getAddr().port())
                         .arg (response.orgUid())
                         .arg (response.getAddr().useGateway() ? "TRUE" : "FALSE")
                 );
    KKSResult * res = dbWrite->execute (sql);
    if (!res){
        kksCritical() << QObject::tr("Cannot execute query %1. Function error").arg(sql);
        return ERROR_CODE;
    }

    qint64 ier = res->getCellAsInt64 (0, 0);
    if(ier <= 0)
        kksCritical() << QObject::tr("Cannot execute query %1. Result is %2").arg(sql).arg(ier);

    response.setId(ier);
    delete res;
    return ier;
}

QMap<qint64, JKKSIOTable> JKKSLoader :: dependencyTables (const JKKSRefRecord& RR) const
{
    Q_UNUSED (RR);

    return QMap<qint64, JKKSIOTable>();
}

QPair<qint64, qint64> JKKSLoader :: getIDMap (const QString& ref_uid, const JKKSRefRecord& RR) const
{
    QPair<qint64, qint64> id_old_new(RR.id(), -1);
    QString table_sql (QString("select iogettablenamebyuid ('%1');").arg (ref_uid));
    KKSResult *tRes = dbWrite->execute (table_sql);
    if (!tRes || tRes->getRowCount() != 1)
    {
        kksCritical() << QObject::tr("ERROR in SQL query! SQL = %1").arg(table_sql);
        if (tRes)
            delete tRes;
        return id_old_new;
    }

    QString tableName = tRes->getCellAsString (0, 0);
    delete tRes;
    if (tableName.isEmpty())
    {
        kksCritical() << QObject::tr("ERROR in SQL query! SQL = %1").arg(table_sql);
        return id_old_new;
    }

    //
    // Сравнение структуры таблиц источника и назначения
    // (категорий)
    //
    QMap<qint64, JKKSCategory> tCat = RR.getCategory ();
    
    //здесь мы пытаемся создать в БД структуру категорий. Фактически, это должно привести лишь к тому, что в JKKSCategoryPair будут записаны реальные ИДы
    //Однако возможна ситуация, когда будет явно создана дочерняя категория для показателей.
    JKKSCategoryPair p = parseCategories(tCat);
    if(p.isNull() || p.isAlone()){
        kksCritical() << QObject::tr("Bad category in refRecord!");
        return id_old_new;
    }

    JKKSCategory tc = p.childCategory();

    QString attrs_uids (QString("ARRAY["));
    //
    // если таблица не является таблицей организаций, т.е. мегасистемной 
    // то в ней есть атрибут unique_id
    //
    QString attrs_vals (QString("ARRAY["));
    //if (QString::compare (tableName, "organization", Qt::CaseInsensitive) != 0)
    //{
        attrs_uids += QString("'unique_id',");
        attrs_vals += QString ("'%1',").arg (RR.uid());
    //}

    QMap<qint64, JKKSCategoryAttr> attrs = tc.attributes ();
    QStringList aVals (RR.attrsValues());
    QMap<qint64, JKKSCategoryAttr>::const_iterator pa (attrs.constBegin());
    int nAttrsC = attrs.count ();
    int nAttrsV = aVals.count ();

    for (int i=nAttrsV-nAttrsC; pa != attrs.constEnd(); pa++) //???????
    {
        attrs_uids += QString("'%1'%2")
                        .arg (pa.value().uid())
                        .arg (i < nAttrsV-1 ? QString (",") : QString ("]"));

        QString value;
        if (i< nAttrsV)
            value = (pa.value().code()=="id" && tableName != "organization" ? QString::number (RR.id()) : aVals[i]);
        int idRef = -1;
        Q_UNUSED (idRef);
        if (pa != attrs.constEnd() && 
            aVals[i].isEmpty() && 
            (pa.value().idAttrType() == 2 ||  //atList
             pa.value().idAttrType() == 3 ||  //atParent
             pa.value().idAttrType() == 12 || //чекбоксы (старые)
             pa.value().idAttrType() == 17 || //чекбоксы новые
             pa.value().idAttrType() == 19 || //ссылка на цвет (фон)
             pa.value().idAttrType() == 26)   //ссылка на цвет (текст)
           )
        {
            value = QString ("NULL::int4");
        }
        else if (pa != attrs.constEnd() && 
                 !aVals[i].isEmpty() && 
                     (pa.value().idAttrType() == 2 ||   //atList
                      pa.value().idAttrType() == 3 ||   //atParent
                      pa.value().idAttrType() == 12 ||  //чекбоксы (старые)
                      pa.value().idAttrType() == 17 ||  //чекбоксы новые
                      pa.value().idAttrType() == 19 ||  //ссылка на цвет (фон)
                      pa.value().idAttrType() == 26)    //ссылка на цвет (текст)
                )
        {

        }

        int a_type = pa.value().idAttrType();
        if (a_type == 9 || //KKSAttrType::atString ||
            a_type == 14 || //KKSAttrType::atFixString ||
            a_type == 13 || //KKSAttrType::atText ||
            a_type == 20) //KKSAttrType::atXMLDoc)
        {
            QString sVal (value);
            QString escVal (sVal);
            escVal.replace("'", "''");
            escVal.replace("\\", "\\\\");
            escVal.replace("\"", "\\\"");

            attrs_vals += QString("'%1'%2")
                            .arg (escVal)//.isEmpty() ? "NULL::varchar" : value)
                            .arg (i < nAttrsV-1 ? QString (",") : QString ("]"));
        }
        else
            attrs_vals += QString("'%1'%2")
                            .arg (value)//.isEmpty() ? "NULL::varchar" : value)
                            .arg (i < nAttrsV-1 ? QString (",") : QString ("]"));
        i++;
    }

    QString ts_sql (QString("select recInsert ('%1', %2, %3);").arg (ref_uid).arg (attrs_uids).arg (attrs_vals));
    kksDebug () << ts_sql;

    KKSResult *tRecRes = dbWrite->execute (ts_sql);
    if (!tRecRes || tRecRes->getRowCount() != 1 || tRecRes->getCellAsInt(0, 0) <= -1)
    {
        kksCritical() << QObject::tr("Cannot insert record! SQL = %1").arg(ts_sql);
        if (tRecRes)
            delete tRecRes;
        return id_old_new;
    }

    qint64 idRecNew = tRecRes->getCellAsInt64 (0, 0);
    id_old_new.second = idRecNew;
    delete tRecRes;

    QString sysSql = QString("select recSetSysParams(%1, %2, %3, %4, %5, %6, %7)")
                                       .arg(idRecNew)
                                       .arg(RR.idState() <= 0 ? QString("1") : QString("%1").arg(RR.idState()))
                                       .arg(RR.uuid().isEmpty() ? QString("NULL") : QString("'%1'").arg(RR.uuid()))
                                       .arg(RR.uid().isEmpty() ? QString("NULL") : QString("'%1'").arg(RR.uid()))
                                       .arg(RR.rIcon().isEmpty() ? QString("NULL") : QString("'%1'").arg(RR.rIcon()))
                                       .arg(!RR.bgColor().isValid() ? QString("NULL") : QString("'%1'").arg(QString::number(RR.bgColor().rgba())))
                                       .arg(!RR.fgColor().isValid() ? QString("NULL") : QString("'%1'").arg(QString::number(RR.fgColor().rgba())));

    tRecRes = dbWrite->execute(sysSql);
    if (!tRecRes || tRecRes->getRowCount() != 1 || tRecRes->getCellAsInt(0, 0) <= -1)
    {
        kksCritical() << QObject::tr("Cannot insert system parameters for the record! SQL = %1").arg(sysSql);
        if (tRecRes)
            delete tRecRes;
        return id_old_new;
    }
    delete tRecRes;
    
    //добавляем показатели
    if(!p.hasChild2()){
        return id_old_new;
    }

    QMap<QString, QString> attrVals = RR.indValues ();
    for (QMap<QString, QString>::const_iterator pa = attrVals.constBegin(); pa != attrVals.constEnd(); pa++)
    {
        QString attrSql = QString ("select * from eioInsertIndicatorEx (%1, %2, %3, NULL::timestamp, NULL::timestamp, NULL::int4, NULL::int4, NULL::varchar);")
            .arg (idRecNew)
            .arg (QString("'")+pa.key()+QString("'")) //здесь в качестве ключа используется unique_id атрибута
            .arg (pa.value().isEmpty() ? QString ("NULL") : QString("'")+pa.value()+QString("'"));
        
        KKSResult * attrRes = dbWrite->execute (attrSql);

        qint64 idRecAttrValue = ERROR_CODE;
        if (attrRes)
        {
            idRecAttrValue = attrRes->getCellAsInt64 (0, 0);
            delete attrRes;
        }
        if (idRecAttrValue <= 0){
            kksCritical() << QObject::tr("eioInsertIndicatorEx() for record with UID = %1 failed!").arg(RR.uid());
            return id_old_new;
        }
    }


    return id_old_new;
}

int JKKSLoader :: writeMessage (JKKSOrgPackage * OrgPack, const QString& senderUID, const QString& receiverUID) const
{

    JKKSQueueResponse recResp (-1, OrgPack->id(), 2, OrgPack->getAddr());
    recResp.setOrgUid(senderUID);

    kksDebug () << senderUID << OrgPack->getAddr ().address() << OrgPack->getAddr ().port();

    QMap<qint64, JKKSTransport> T = OrgPack->getTransports();
    for (QMap<qint64, JKKSTransport>::const_iterator pt = T.constBegin();
         pt != T.constEnd();
         pt++)
    {
        JKKSTransport TR = pt.value();
        int iert = writeTransport (TR);
        if (iert < 0)
        {
            recResp.setResult (4);

            int res1 = generateQueueResponse (recResp);
            if(res1 == ERROR_CODE)
                return ERROR_CODE;

            return ERROR_CODE;
        }
    }

    QMap<qint64, JKKSOrganization> O = OrgPack->getOrgs();
    int norg = O.count();
    kksDebug () << norg;
    for (QMap<qint64, JKKSOrganization>::const_iterator po = O.constBegin();
         po != O.constEnd();
         po++)
    {
        JKKSOrganization ORG = po.value();
        int iero = writeMessage (&ORG, senderUID, receiverUID, false);
        if (iero < 0)
        {
            recResp.setResult (4);

            int res1 = generateQueueResponse (recResp);
            if(res1 == ERROR_CODE)
                return ERROR_CODE;

            return ERROR_CODE;
        }
    }

    recResp.setResult (3);

    int res1 = generateQueueResponse (recResp);
    if(res1 == ERROR_CODE)
        return ERROR_CODE;

    return OK_CODE;
}

/*
создание организации заключается в следующих действиях:
1) создать все необходимые записи в таблицах, на которые есть ссылки из organization
2) создать новую запись в organization. С учетом того, что на целевой БД не может встретиться орг-я с email-prefix, равный пришедшему 
(за исключением случая, если админ на главной машине задал email_prefix = localorg_prefix_slave, но это приведет к аварии), 
в таблице организаций целевой БД будет на 1 орг-ю больше.
3) занести в organization_transport взаимюсвязи орг-й с транспортами
4) !! проанализировать, какая теперь организация стала локальной. Удалить предыдущую локальную (она имеет email_prefix = localorg_prefix_slave)
5) !! заменить во всех соответствующих таблицах все ссылки на предыдущую локальную новой локальной
6) обновить для jkksloader'а информацию о текущем локальном адресе из БД
*/
int JKKSLoader :: writeMessage (JKKSOrganization * org, const QString & senderUID, const QString& receiverUID, bool bGenResponse) const
{
    if (!org)
        return ERROR_CODE;

    JKKSQueueResponse recResp (-1, org->id(), 2, org->getAddr());
    recResp.setOrgUid(senderUID);

    //тип организации
    JKKSOrgType ot = org->getType ();
    int idType = writeOrgType (ot);
    if (idType < 0){
        kksCritical() << QObject::tr("writeOrgType Error!");
        
        if(bGenResponse){
            recResp.setResult(4);
            generateQueueResponse (recResp);
        }

        return ERROR_CODE;
    }
    org->setType (ot);

    //текущий режим функционирования организации
    JKKSWorkMode cMode = org->getCurrentMode ();
    int idC = writeOrgWM (cMode);
    if (idC < 0){
        kksCritical() << QObject::tr("writeOrgWM Error!");

        if(bGenResponse){
            recResp.setResult(4);
            generateQueueResponse (recResp);
        }

        return ERROR_CODE;
    }
    org->setCurrentMode (cMode);
    
    //предыдущий режим функционирования организации
    JKKSWorkMode pMode = org->getPreviousMode ();
    int idP = writeOrgWM (pMode);
    if (idP < 0){
        kksCritical() << QObject::tr("writeOrgWM Error! 1");

        if(bGenResponse){
            recResp.setResult(4);
            generateQueueResponse (recResp);
        }

        return ERROR_CODE;
    }
   
    org->setPreviousMode (pMode);

    //режимы функционирования организации
    QMap<qint64, JKKSWorkMode> wmList = org->getWorkModes();
    QMap<qint64, JKKSWorkMode> wmOrg;
    for (QMap<qint64, JKKSWorkMode>::const_iterator pw = wmList.constBegin();
         pw != wmList.constEnd();
         pw++)
    {
        JKKSWorkMode wm (pw.value());
        int idwm = writeOrgWM (wm);
        if (idwm > 0)
            wmOrg.insert (idwm, wm);
    }
    org->setWorkModes (wmOrg);

    double oLong;
    double oLat;
    org->getCoords (oLong, oLat);
    QString wmStr;
    int nwm = wmOrg.count();
    if (nwm > 0)
    {
        int i=0;
        wmStr = QString ("{");
        for (QMap<qint64, JKKSWorkMode>::const_iterator pw = wmOrg.constBegin();
                pw != wmOrg.constEnd();
                pw++)
        {
            wmStr += pw.value().uid();//QString::number (pw.key());
            if (i < nwm-1)
                wmStr += QString(",");
            else
                wmStr += QString("}");
            i++;
        }
    }

    QString uidParent (org->getParent());
    QString uidParent1 (org->getParentPart());
    QString uidSubst (org->getSubstitutor());
    
    //создаем организацию
    QString sql = QString("select insertOrganizationEx('%1', %2, %3, %4, %5, %6, %7, '%8', '%9', '%10', '%11', %12, %13, %14, %15, %16, %17, %18)")
                        .arg(org->uid())
                        .arg(idType)
                        .arg(uidParent.isEmpty() ? QString("NULL") : QString("'%1'").arg(uidParent))
                        .arg(uidParent1.isEmpty() ? QString("NULL") : QString("'%1'").arg(uidParent1))
                        .arg(uidSubst.isEmpty() ? QString("NULL") : QString("'%1'").arg(uidSubst))
                        .arg(idC <= 0 ? QString("NULL") : QString("%1").arg(idC))
                        .arg(idP <= 0 ? QString("NULL") : QString("%1").arg(idP))
                        .arg (org->getName())
                        .arg (org->getShortName())
                        .arg (org->getCode())
                        .arg (org->getEMail())
                        .arg (oLat)
                        .arg (oLong)
                        .arg ((org->getTimeCMode().isNull() || !org->getTimeCMode().isValid() ) ? QString("NULL") : QString("'%1'").arg(org->getTimeCMode().toString(Qt::ISODate)))
                        .arg ((org->getTimePMode().isNull() || !org->getTimePMode().isValid() ) ? QString("NULL") : QString("'%1'").arg(org->getTimePMode().toString(Qt::ISODate)))
                        .arg (org->getMapSymbol().isEmpty() ? QString("NULL") : QString("'%1'").arg(org->getMapSymbol()))
                        .arg (org->getTreeSymbol().isEmpty() ? QString("NULL") : QString("'%1'").arg(org->getTreeSymbol()))
                        .arg (wmStr.isEmpty() ? QString("null::int4[]") : QString("'%1'").arg(wmStr));


    KKSResult * oRes = dbWrite->execute (sql);
    if (!oRes || oRes->getRowCount () != 1)
    {
        if (oRes)
            delete oRes;

        kksCritical() << QObject::tr("insertOrganizationEx() failed! SQL = %1").arg(sql);

        if(bGenResponse){
            recResp.setResult(4);
            generateQueueResponse (recResp);
        }

        return ERROR_CODE;
    }

    qint64 idOrg = oRes->getCellAsInt (0, 0);
    org->setId (idOrg);
    delete oRes;
    
    if (idOrg < 0){
        kksCritical() << QObject::tr("insertOrganizationEx() failed! SQL = %1").arg(sql);

        if(bGenResponse){
            recResp.setResult(4);
            generateQueueResponse (recResp);
        }

        return ERROR_CODE;
    }

    //транспорт, на котором функционирует организация
    JKKSTransport T = org->getTransport ();
    if (!receiverUID.isEmpty())
    {
        kksDebug () << T.id() << T.uid() << T.getAddress().address() << org->id() << receiverUID;
        int idTr = writeTransport (T);
        if (idTr < 0){
            kksCritical() << QObject::tr("writeTransport Error!");

            if(bGenResponse){
                recResp.setResult(4);
                generateQueueResponse (recResp);
            }

            return ERROR_CODE;
        }

        org->setTransport (T);
    }

    //задаем адрес организации на данном транспорте
    if (!T.uid().isEmpty())
    {
        QString trOrgSql = QString("select insertOrgAddressEx('%1', '%2', '%3', %4, true, %5)")
                                .arg(org->getEMail())
                                .arg(T.uid())
                                .arg(T.getAddress().address())
                                .arg(QString::number(T.getAddress().port()))
                                .arg(T.getAddress().useGateway() ? "true" : "false");

        KKSResult * otRes = dbWrite->execute (trOrgSql);
        if (!otRes || otRes->getRowCount () <= 0)
        {
            kksCritical() << QObject::tr("query failed! SQL = %1").arg(trOrgSql);
            if (otRes)
                delete otRes;

            if(bGenResponse){
                recResp.setResult(4);
                generateQueueResponse (recResp);
            }
    
            return ERROR_CODE;
        }

        delete otRes;
    }

    //помечаем организацию как созданную
    QString is_create_sql = QString ("update organization set is_created = 1 where id=%1").arg (idOrg);
    
    KKSResult * crRes = dbWrite->execute (is_create_sql);
    if (!crRes){
        kksCritical() << QObject::tr("query failed! SQL = %1").arg(is_create_sql);

        if(bGenResponse){
            recResp.setResult(4);
            generateQueueResponse (recResp);
        }

        return ERROR_CODE;
    }
    delete crRes;

    QPair <QString, qint64> idOrgMap ("organization", org->id());

    records.insert (idOrgMap, (qint64)idOrg);

    if(bGenResponse){
        recResp.setResult(3);
        generateQueueResponse (recResp);
    }

    getLocalAddress(true);
    sql = QString ("select * from getlocalorg();");
    KKSResult * r1 = dbWrite->execute (sql);
    if (!r1 || r1->getRowCount() != 1)
    {
        if(r1)
            delete r1;
        
        return idOrg;
    }
    else{
        this->senderUID = r1->getCellAsString (0, 3);
        this->receiverUID = r1->getCellAsString (0, 3);
    }
    
    if(r1)
        delete r1;

    return idOrg;
}

qint64 JKKSLoader :: writeTransport (JKKSTransport& T) const
{
    //здесь транспорт будет создан, если в БД целевого объекта нет такого транспорта
    //причем, поскольку объект-отправитель не знает, функционирует ли этот транспорт на целевом объекте
    //необходимо is_active ему выставить в false
    QString sql = QString ("select uInsertTransport('%1', '%2', '%3', FALSE, %4, %5);")
                    .arg (T.uid())
                    .arg (T.getTransportName ())
                    .arg (T.getAddress().address().isEmpty() ? QString("NULL") : T.getAddress().address())
                    //.arg (T.isTransportActive() ? QString ("TRUE") : QString ("FALSE"))
                    .arg (T.getAddress().port() <= 0 ? QString("NULL") : QString::number(T.getAddress().port()))
                    .arg (T.getAddress().useGateway() ? QString("TRUE") : QString("FALSE"));

    KKSResult * tRes = dbWrite->execute (sql);
    if (!tRes || tRes->getRowCount() != 1)
    {
        if (tRes)
            delete tRes;
        return ERROR_CODE;
    }

    qint64 idTr = tRes->getCellAsInt (0, 0);

    T.setId (idTr);
    delete tRes;
    return idTr;
}

qint64 JKKSLoader :: writeOrgType (JKKSOrgType& OT) const
{
    QString sql = QString ("select recInsert ('localorg-io_objects-32', ARRAY['unique_id', 'localorg-attributes-2','localorg-attributes-19'], ARRAY['%3', '%1', '%2']);")
                    .arg (OT.getOrgTypeName())
                    .arg (OT.getOrgTypeShortName())
                    .arg (OT.uid());

    KKSResult * tRes = dbWrite->execute (sql);
    if (!tRes || tRes->getRowCount() != 1)
    {
        if (tRes)
            delete tRes;
        return ERROR_CODE;
    }

    qint64 idType = tRes->getCellAsInt (0, 0);
    OT.setIdOrgType (idType);

    delete tRes;
    return idType;
}

qint64 JKKSLoader :: writeIOType (JKKSType& ioType) const
{
    QString sql = QString ("select recInsert ('localorg-io_objects-9', ARRAY['unique_id', 'localorg-attributes-2','localorg-attributes-3'], ARRAY['%3', '%1', '%2']);")
                    .arg (ioType.getName())
                    .arg (ioType.getDesc())
                    .arg (ioType.uid());

    KKSResult * tRes = dbWrite->execute (sql);
    if (!tRes || tRes->getRowCount() != 1)
    {
        if (tRes)
            delete tRes;
        return ERROR_CODE;
    }

    qint64 idType = tRes->getCellAsInt (0, 0);
    ioType.setIdType(idType);

    delete tRes;

    return idType;
}

qint64 JKKSLoader :: writeOrgWM (JKKSWorkMode& wm) const
{
    JKKSWorkModeType wmt = wm.getType();
    qint64 idType = writeWMType (wmt);
    if (idType < 0)
        return ERROR_CODE;

    wm.setType (wmt);
    QString sql = QString ("select recInsert ('localorg-io_objects-29', ARRAY['unique_id', 'localorg-attributes-1', 'localorg-attributes-72', 'localorg-attributes-2', 'localorg-attributes-3', 'localorg-attributes-73'], ARRAY['%5', '0', '%2', '%1', '%3', '%4']);")
            .arg (wm.getName())
            .arg (wm.getType().uid())
            .arg (wm.getDesc())
            .arg (wm.getSymbol())
            .arg (wm.uid());

    kksDebug () << sql;
    KKSResult * wRes = dbWrite->execute (sql);
    if (!wRes || wRes->getRowCount() != 1)
    {
        if (wRes)
            delete wRes;
        return ERROR_CODE;
    }

    qint64 idWM = wRes->getCellAsInt (0, 0);
    wm.setId (idWM);
    delete wRes;
    return idWM;
}

qint64 JKKSLoader :: writeWMType (JKKSWorkModeType& wmt) const
{
    QString sql = QString ("select recInsert ('localorg-io_objects-31', ARRAY ['unique_id', 'localorg-attributes-1', 'localorg-attributes-2', 'localorg-attributes-19'], ARRAY['%3', '0', '%1', '%2'])")
            .arg (wmt.getName())
            .arg (wmt.getShortName())
            .arg (wmt.uid());
    kksDebug () << sql;
    KKSResult * wRes = dbWrite->execute (sql);
    if (!wRes || wRes->getRowCount() != 1)
    {
        if (wRes)
            delete wRes;
        return ERROR_CODE;
    }

    qint64 idWMT = wRes->getCellAsInt (0, 0);
    wmt.setId (idWMT);
    delete wRes;
    return idWMT;
}

QMap<qint64, JKKSOrganization> JKKSLoader :: readOrganizations (qint64 idOrg, qint64 idQueue) const
{
    QString orgSql = QString ("select * from getOrgs(%1);")
                        .arg (idOrg < 0 ? QString ("null::int4") : QString::number (idOrg));
    
    KKSResult * orgRes = dbRead->execute (orgSql);
    QMap<qint64, JKKSOrganization> orgs;
    if (!orgRes || orgRes->getRowCount () <= 0)
    {
        if (orgRes)
            delete orgRes;
        return orgs;
    }

    int nOrgs = orgRes->getRowCount ();
    for (int i=0; i<nOrgs; i++)
    {
        JKKSOrganization org (orgRes->getCellAsInt (i, 0), 
                              QString(), //orgRes->getCellAsInt (i, 2),
                              QString(), //orgRes->getCellAsInt (i, 3),
                              QString(), //orgRes->getCellAsInt (i, 4),
                              orgRes->getCellAsString (i, 8), 
                              orgRes->getCellAsString (i, 9),
                              orgRes->getCellAsString (i, 10),
                              orgRes->getCellAsString (i, 11),
                              orgRes->getCellAsDouble (i, 12),
                              orgRes->getCellAsDouble (i, 13),
                              orgRes->getCellAsString (i, 14),
                              orgRes->getCellAsString (i, 15),
                              orgRes->getCellAsDateTime (i, 17),
                              orgRes->getCellAsDateTime (i, 16));

        org.setIdQueue(idQueue);

        qint64 idParent = orgRes->getCellAsInt (i, 2);

        QString tableName =  QString ("organization");
        QList <qint64> ids;
        ids << idParent;
        QString uidParent = getUIDSbyIDs (ATTR_ID_PARENT, tableName, ids);
        org.setParent (uidParent);

        qint64 idParent1 = orgRes->getCellAsInt (i, 3);
        ids.clear ();
        ids << idParent1;
        QString uidParent1 = getUIDSbyIDs (ATTR_ID_PARENT1, tableName, ids);
        org.setParentPart (uidParent1);

        qint64 idSubst = orgRes->getCellAsInt (i, 4);
        ids.clear ();
        ids << idSubst;
        QString uidSubst = getUIDSbyIDs (ATTR_ID_SUBSTITUTOR, tableName, ids);
        org.setSubstitutor (uidSubst);
        kksDebug () << uidParent << uidParent1 << uidSubst;

        JKKSOrgType orgType (orgRes->getCellAsInt (i, 1), \
                             orgRes->getCellAsString (i, 19), \
                             orgRes->getCellAsString (i, 20),
                             orgRes->getCellAsString(i, 18));

        org.setType (orgType);
        JKKSWorkMode currMode = readWM (orgRes->getCellAsInt (i, 5));
                               //orgRes->getCellAsString (i, 19));

        JKKSWorkMode prevMode = readWM (orgRes->getCellAsInt (i, 6));
                               //orgRes->getCellAsString (i, 20));

        org.setCurrentMode (currMode);
        org.setPreviousMode (prevMode);
        QMap<qint64, JKKSWorkMode> wmList = this->readOrgWM (org.id());
        org.setWorkModes (wmList);

        JKKSAddress addr(orgRes->getCellAsString (i, 25),//address 
                         orgRes->getCellAsInt(i, 28),//port
                         orgRes->getCellAsBool(i, 29));//use_gateway

        JKKSTransport T (orgRes->getCellAsInt (i, 23), // id
                         orgRes->getCellAsString (i, 24), // name
                         addr, // address
                         orgRes->getCellAsBool (i, 27), // active
                         orgRes->getCellAsString (i, 26)// unique_id
                         ); 

        org.setTransport (T);

        if (orgs.contains (org.id()))
            orgs.insertMulti (org.id(), org);
        else
            orgs.insert (org.id(), org);
    }

    delete orgRes;

    return orgs;
}

JKKSSearchTemplate JKKSLoader :: readSearchTemplate (qint64 idSearchTemplate) const
{
    if (idSearchTemplate <= 0)
        return JKKSSearchTemplate();

    QString sql = QString ("select * from ioGetSearchTemplate(%1);").arg (idSearchTemplate);
    KKSResult * stRes = dbRead->execute (sql);

    if (!stRes || stRes->getRowCount() != 1)
    {
        if (stRes)
            delete stRes;
        return JKKSSearchTemplate();
    }

    JKKSSearchTemplate st (stRes->getCellAsInt (0, 0), // id
                           stRes->getCellAsString (0, 2));
    QMap<qint64, JKKSSearchGroup> stGroups;
    stGroups.clear ();
    readGroups (stRes->getCellAsInt (0, 1), stGroups);
    st.setGroups (stGroups);
    return st;
}

void JKKSLoader :: readGroups (qint64 idMainGroup, QMap<qint64, JKKSSearchGroup>& stGroups) const
{
    if (idMainGroup < 0)
        return;

    QString sql = QString ("select * from getGroups (%1, true);").arg (idMainGroup);
    KKSResult * gRes = dbRead->execute (sql);

    if (!gRes || gRes->getRowCount() == 0)
    {
        if (gRes)
            delete gRes;
        return;
    }

    for (int i=0; i<gRes->getRowCount(); i++)
    {
        JKKSSearchGroup sg (gRes->getCellAsInt (i, 0), gRes->getCellAsInt (i, 1), gRes->getCellAsInt (i, 2), gRes->getCellAsBool (i, 3));
        QMap<qint64, JKKSSearchCriterion> crit = readCriteriaForGroup (sg.id());
        sg.setCriteria (crit);
        stGroups.insert (sg.id(), sg);
        if (i>0)
            readGroups (sg.id(), stGroups);
    }
    delete gRes;
}

QMap<qint64, JKKSSearchCriterion> JKKSLoader :: readCriteriaForGroup (qint64 idGroup) const
{
    if (idGroup <= 0)
        return QMap<qint64, JKKSSearchCriterion> ();

    QString sql = QString ("select * from ioGetCriterionFromGroup (%1);").arg (idGroup);
    KKSResult * cRes = dbRead->execute (sql);

    if (!cRes)
        return QMap<qint64, JKKSSearchCriterion> ();

    QMap<qint64, JKKSSearchCriterion> cResults;
    for (int i=0; i<cRes->getRowCount(); i++)
    {
        int id = cRes->getCellAsInt (i, 0);
        int attrId= cRes->getCellAsInt(i, 1);
        JKKSCategoryAttr attr = readAttribute (attrId);
        QString value = cRes->getCellAsString (i, 2);
        int id_type = cRes->getCellAsInt (i, 3);
        int oper = cRes->getCellAsInt (i, 4);
        bool cs = cRes->getCellAsBool (i, 5);
        bool isnot = cRes->getCellAsBool (i, 6);
        JKKSSearchCriterion sc (id, attr, value, id_type, oper, cs, isnot);
        cResults.insert (id, sc);
    }

    delete cRes;
    return cResults;
}

qint64 JKKSLoader :: writeSearchTemplate (const JKKSSearchTemplate& st) const
{
    QMap<qint64, JKKSSearchGroup> gr = st.getGroups ();
    QMap<qint64, JKKSSearchGroup> wGroups (writeGroups (-1, gr));
    QMap<qint64, JKKSSearchGroup>::const_iterator pg = wGroups.constBegin();
    int idMainGroup = -1;
    for (; pg != wGroups.constEnd() && idMainGroup < 0; pg++)
    {
        if ( pg.value().idParent() <= 0)
            idMainGroup = pg.key ();
    }

    if (idMainGroup < 0)
        return ERROR_CODE;

    QString sql = QString ("select ioInsertSearchTemplate('%1', %2)").arg (st.getName()).arg (idMainGroup);
    KKSResult * res = dbWrite->execute (sql);

    if (!res || res->getCellAsInt64(0, 0) <= 0){
        kksCritical() << QObject::tr("ioInsertSearchTemplate() failed! SQL = %1").arg(sql);
        if(res)
            delete res;

        return ERROR_CODE;
    }

    qint64 id = res->getCellAsInt (0, 0);
    delete res;

    return id;
}

QMap<qint64, JKKSSearchGroup> JKKSLoader :: writeGroups (qint64 idParentGr, const QMap<qint64, JKKSSearchGroup>& stGroups) const
{
    qint64 idMainGr = -1;
    for (QMap<qint64, JKKSSearchGroup>::const_iterator pg = stGroups.constBegin(); \
         pg != stGroups.constEnd() && idMainGr < 0; \
         pg++)
    {
        kksDebug () << idMainGr << idParentGr << pg.key() << pg.value().idParent();
        if ((idParentGr > 0 && pg.value().idParent() == idParentGr) ||
            (idParentGr < 0 && pg.value().idParent() <= 0))
            idMainGr = pg.key();
    }
    
    if (idMainGr < 0)
        return QMap<qint64, JKKSSearchGroup>();
    
    QMap<qint64, JKKSSearchGroup> gRes;
    QMap<qint64, JKKSSearchGroup>::const_iterator pg = stGroups.constFind (idMainGr);
    JKKSSearchGroup g = pg.value();
    QString sql = QString("select ioInsertSearchGroup (%1, %2, %3);")
                        .arg (idParentGr < 0 ? QString("NULL::int4") : QString::number (idParentGr))
                        .arg (pg.value().getOper())
                        .arg (pg.value().isNegative() ? QString ("TRUE") : QString ("FALSE"));
    KKSResult * res = dbWrite->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        kksCritical() << QObject::tr("ioInsertSearchGroup() failed! SQL = %1").arg(sql);
        if (res)
            delete res;

        return gRes;
    }

    qint64 idGroup = res->getCellAsInt (0, 0);
    delete res;

    if (idGroup < 0)
        return gRes;

    g.setId (idGroup);
    gRes.insert (idGroup, g);
    QMap<qint64, JKKSSearchGroup> subGroups = writeGroups (idMainGr, stGroups);
    for (pg = subGroups.constBegin(); pg != subGroups.constEnd(); pg++)
        gRes.insert (pg.key(), pg.value());

    for (QMap<qint64, JKKSSearchCriterion>::const_iterator pc = g.getCriteria().constBegin(); \
         pc != g.getCriteria().constEnd(); \
         pc++)
    {
        JKKSSearchCriterion sc = pc.value();
        int ier = writeCriteriaForGroup (sc);
        if (ier < 0)
            continue;

        QString gscSql = QString ("select ioInsertCriterionIntoGroup (%1, %2);").arg (ier).arg (idGroup);
        KKSResult * gsres = dbWrite->execute (gscSql);
        if (!gsres || gsres->getRowCount() == 0)
        {
            kksCritical() << QObject::tr("ioInsertCriterionIntoGroup() failed! SQL = %1").arg(gscSql);
            if (gsres)
                delete gsres;
            continue;
        }

        delete gsres;
    }

    return gRes;
}

qint64 JKKSLoader :: writeCriteriaForGroup (JKKSSearchCriterion& sc) const
{
    QString sql = QString ("select ioInsertSearchCriterion ('%1', '%2', %3, %4, %5, %6);")
                            .arg (sc.getAttribute() < 0 ? QString ("NULL::varchar") : sc.getAttribute().uid())
                            .arg (sc.getValue().isEmpty() ? QString ("NULL::varchar") : sc.getValue())
                            .arg (sc.getType() < 0 ? QString ("NULL::int4") : QString::number (sc.getType()))
                            .arg (sc.getOperation() < 0 ? QString ("NULL::int4") : QString::number (sc.getOperation()))
                            .arg (sc.isNegative () ? QString ("TRUE") : QString ("FALSE"))
                            .arg (sc.isCaseSensitive () ? QString ("TRUE") : QString ("FALSE"));

    KKSResult * res = dbWrite->execute (sql);
    if (!res || res->getRowCount() == 0 || res->getCellAsInt (0, 0) < 0)
    {
        kksCritical() << QObject::tr("ioInsertSearchCriterion() failed! SQL = %1").arg(sql);
        if (res)
            delete res;

        return ERROR_CODE;
    }
    int ok = res->getCellAsInt (0, 0);
    delete res;
    sc.setId (ok);

    return ok;
}

JKKSOrgPackage JKKSLoader :: readOrgs (qint64 id, const QString& receiverUID) const
{
    JKKSOrgPackage OP(id);//это id in out_sync_queue !!!
    QMap<qint64, JKKSTransport> TS = readTransports (receiverUID);

    OP.setTransports (TS);
    QMap<qint64, JKKSOrganization> orgs = readOrganizations (-1, id);
    OP.setOrgs (orgs);
    return OP;
}

QMap<qint64, JKKSTransport> JKKSLoader :: readTransports (const QString& receiverUID) const
{
    QMap<qint64, JKKSTransport> TResult;

    QString sql = QString ("select * from getTransportAddresses ('%1');").arg (receiverUID);
    KKSResult * res = dbRead->execute (sql);
    
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        return TResult;
    }

    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSAddress addr(res->getCellAsString (i, 3),
                         res->getCellAsInt (i, 5),
                         res->getCellAsBool (i, 6));

        JKKSTransport T (res->getCellAsInt (i, 1), // id
                        res->getCellAsString (i, 2), // name
                        addr, // address
                        res->getCellAsBool (i, 4), // active
                        res->getCellAsString (i, 0) // unique_id
                       );

        TResult.insert (T.id(), T);
    }
    delete res;
    return TResult;
}

QString JKKSLoader :: getUIDSbyIDs (qint64 attrId, const QString& tableName, const QList<qint64>& ids) const
{
    JKKSCategoryAttr attr = readAttribute (attrId);
    QString tName = attr.table ();
    if (attr.idAttrType() == 3)
        tName = tableName;
    QString resStr;
    int n = ids.count();

    for (int i=0; i<n; i++)
    {
        QString sql;
        if (tName == "organization")
            sql = QString ("select %1 from %2 where id=%3").arg ("email_prefix").arg (tName).arg (ids[i]);
        else
            sql = QString ("select %1 from %2 where id=%3").arg ("unique_id").arg (tName).arg (ids[i]);
        KKSResult * res = dbRead->execute (sql);
        if (!res || res->getRowCount() != 1)
        {
            if (res)
                delete res;
            continue;
        }
        if (n>1)
        {
            if (i==0)
                resStr = "{" + res->getCellAsString(0, 0);
            else if (i==n-1)
                resStr += res->getCellAsString(0, 0) + "}";
            else
                resStr += res->getCellAsString(0, 0) + ",";
        }
        else
            resStr = QString("%1").arg (res->getCellAsString(0, 0));
        delete res;
    }

    return resStr;
}

QMap<qint64, JKKSWorkMode> JKKSLoader :: readOrgWM (qint64 idOrg) const
{
    QString sql = QString ("select * from getOrgsWorkModes (%1)").arg (idOrg);
    KKSResult * res = dbRead->execute (sql);

    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        return QMap<qint64, JKKSWorkMode>();
    }

    QMap<qint64, JKKSWorkMode> wmList;
    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSWorkModeType wmType (res->getCellAsInt(i, 5),     // id
                                 res->getCellAsString (i, 6), // name
                                 res->getCellAsString (i, 7), // short name
                                 res->getCellAsString (i, 8)  // uid
                                );

        JKKSWorkMode wm (res->getCellAsInt(i, 0),     // id
                         res->getCellAsString (i, 1), // name
                         res->getCellAsString (i, 2), // description
                         res->getCellAsString (i, 3), // symbol
                         wmType,
                         res->getCellAsString (i, 4)  // uid
                        );
        wmList.insert (wm.id(), wm);
    }
    delete res;
    return wmList;
}

JKKSWorkMode JKKSLoader :: readWM (qint64 idWM) const
{
    QString sql = QString ("select * from getWorkMode (%1)").arg (idWM);
    KKSResult * res = dbRead->execute (sql);

    if (!res || res->getRowCount() != 1)
    {
        if (res)
            delete res;

        return JKKSWorkMode ();
    }
    JKKSWorkModeType wmType (res->getCellAsInt(0, 5),     // id
                             res->getCellAsString (0, 6), // name
                             res->getCellAsString (0, 7), // short name
                             res->getCellAsString (0, 8)  // uid
                            );

    JKKSWorkMode wm (res->getCellAsInt(0, 0),     // id
                     res->getCellAsString (0, 1), // name
                     res->getCellAsString (0, 2), // description
                     res->getCellAsString (0, 3), // symbol
                     wmType,
                     res->getCellAsString (0, 4)  // uid
                    );
    delete res;
    return wm;
}

QMap<QString, JKKSPing> JKKSLoader :: createPings(const QStringList & receivers) const 
{
    QMap<QString, JKKSPing> pings;

    QString emailPrefixes;
    for(int i=0; i<receivers.count(); i++){
        emailPrefixes.append(QString("'") + receivers.at(i) + QString("'"));
        if(i < receivers.count()-1)
            emailPrefixes.append(", ");
    }

    QString sql = QString ("select * from uGetPings(ARRAY[%1])").arg(emailPrefixes);
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return pings;
    
    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSAddress addr(res->getCellAsString (i, 6), 
                         res->getCellAsInt (i, 8),
                         res->getCellAsBool(i, 9));

        JKKSPing ping = JKKSPing(res->getCellAsInt64 (i, 0),
                                 addr);

        ping.setIdOrgTo(res->getCellAsInt64(i, 0));
        ping.setNameTo(res->getCellAsString(i, 1));
        ping.setUidTo(res->getCellAsString(i, 3));
        
        ping.setSenderAddress(getLocalAddress());
        
        ping.setCreated(res->getCellAsInt(i, 10));
        ping.setCompleted(res->getCellAsBool(i, 11));

        ping.setVersionFrom(res->getCellAsString(i, 13));
        ping.setIdOrgFrom(res->getCellAsInt(i, 14));
        ping.setNameFrom(res->getCellAsString(i, 15));
        ping.setUidFrom(res->getCellAsString(i, 19));
        
        pings.insert(ping.uidTo(), ping);
    }
    
    delete res;

    return pings;
}

QMap<QString, JKKSPing> JKKSLoader :: createPings() const
{
    QMap<QString, JKKSPing> pings;

    QString sql = QString ("select * from uGetPings()");
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return pings;
    
    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSAddress addr(res->getCellAsString (i, 6), 
                         res->getCellAsInt (i, 8),
                         res->getCellAsBool(i, 9));
        
        JKKSPing ping = JKKSPing(res->getCellAsInt64 (i, 0),
                                 addr);

        ping.setSenderAddress(getLocalAddress());
        ping.setCompleted(res->getCellAsBool(i, 11));
        ping.setCreated(res->getCellAsInt(i, 10));
        ping.setIdOrgFrom(res->getCellAsInt(i, 14));
        ping.setIdOrgTo(res->getCellAsInt64(i, 0));
        ping.setNameFrom(res->getCellAsString(i, 15));
        ping.setNameTo(res->getCellAsString(i, 1));
        ping.setUidFrom(res->getCellAsString(i, 19));
        ping.setUidTo(res->getCellAsString(i, 3));
        ping.setVersionFrom(res->getCellAsString(i, 13));
        
        pings.insert(ping.uidTo(), ping);
    }

    delete res;
    
    return pings;
}

JKKSPing JKKSLoader :: createPing(const QString & uidOrg) const
{
    JKKSPing ping;

    QString sql = QString ("select * from uGetPings('%1')").arg(uidOrg);
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return ping;
    
    JKKSAddress addr(res->getCellAsString (0, 6), 
                     res->getCellAsInt (0, 8),
                     res->getCellAsBool(0, 9));

    ping = JKKSPing(res->getCellAsInt64 (0, 0),
                    addr);

    ping.setSenderAddress(getLocalAddress());
    ping.setCompleted(res->getCellAsBool(0, 11));
    ping.setCreated(res->getCellAsInt(0, 10));
    ping.setIdOrgFrom(res->getCellAsInt(0, 14));
    ping.setIdOrgTo(res->getCellAsInt64(0, 0));
    ping.setNameFrom(res->getCellAsString(0, 15));
    ping.setNameTo(res->getCellAsString(0, 1));
    ping.setUidFrom(res->getCellAsString(0, 19));
    ping.setUidTo(res->getCellAsString(0, 3));
    ping.setVersionFrom(res->getCellAsString(0, 13));
        
    delete res;

    return ping;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: pingsToPMessWithAddr(const QMap<QString, JKKSPing> pings)
{
    QList<JKKSPMessWithAddr *> result;

    for (QMap<QString, JKKSPing>::const_iterator pa = pings.constBegin(); pa != pings.constEnd(); pa++)
    {
        JKKSPing ping (pa.value());
    
        JKKSPMessage pM(ping.serialize(), ping.getMessageType());
        pM.setVerifyReceiver(false);
        pM.setSenderUID(ping.uidFrom());
        pM.setReceiverUID(ping.uidTo()); //email_prefix

        
        JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr (pM, ping.getAddr(), ping.id());
        pMessWithAddr->unp = ping.uidTo(); //email_prefix
        if (pMessWithAddr)
            result.append (pMessWithAddr);
    }

    return result;
}

QString JKKSLoader :: getReceiverEmailPrefix(qint64 id, qint64 type) const//получить email_prefix организации-получателя для заданного сообщения (с заданным типом)
{
    QString emailPrefix;

    if(id <= 0 || type <= 0)
        return emailPrefix;

    QString sql = QString("select uGetReceiverEmailPrefix(%1, %2)").arg(id).arg(type);
    KKSResult * res = dbRead->execute(sql);
    if(!res || res->getRowCount() != 1){
        kksCritical() << QObject::tr("uGetReceiverEmailPrefix() failed! SQL = %1").arg(sql);
        if(res)
            delete res;
        return emailPrefix;
    }

    emailPrefix = res->getCellAsString(0, 0);

    return emailPrefix;
}


bool JKKSLoader :: beginTransaction() const
{
    return dbWrite->begin();
}

bool JKKSLoader :: commitTransaction() const
{
    return dbWrite->commit();
}

bool JKKSLoader :: rollbackTransaction() const
{
    return dbWrite->rollback();
}
