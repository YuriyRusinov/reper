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


JKKSLoader :: JKKSLoader (const QString& host,
                          const QString& name, 
                          const QString& user, 
                          const QString& pass, 
                          int p,
                          int idTransport)
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
      local_address (QString()),
      senderUID(QString()),
      receiverUID(QString())
{
}

JKKSLoader :: ~JKKSLoader (void)
{
}

int JKKSLoader::setLocalAddress(const QString & address) const
{
    int result = ERROR_CODE;
    if(dbRead){
        KKSResult * res = dbRead->execute(QString("SELECT usetlocaladdress('%1');").arg(address));
        if(res && res->getRowCount() == 1)
            result = res->getCellAsInt(0, 0);
        delete res;
    }
    return result;
}

const QString & JKKSLoader::getLocalAddress() const
{
    if(!local_address.isEmpty())
        return local_address;
	
    if(dbRead){
        KKSResult * res = dbRead->execute(QString("SELECT uGetLocalAddress(%1);").arg(m_idTransport));
        if(res && res->getRowCount() == 1)
            local_address = res->getCellAsString(0, 0);
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

int JKKSLoader :: getIdTransport (void) const
{
    return m_idTransport;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readMessages (void) const
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
        messList += readCommands();
        //
        // TODO process documents, etc...
        //
        messList += readDocuments();
        messList += readMails ();

        messList += readMailConfirmations();
        messList += readCmdConfirmations();

        messList += readTableRecords();
    }
    
    messList += readQueueResults ();

    return messList;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readCmdConfirmations(void) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uGetCmdsConfirm () union select * from uGetCmdsConfirm1();");
    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 4)
    {
        for (int i=0; i<res->getRowCount(); i++) {
            JKKSCmdConfirmation cfm(res->getCellAsInt (i, 1),//src_id
                                    res->getCellAsString (i, 2),//extra_id
                                    res->getCellAsInt (i, 3),//idJrState
                                    res->getCellAsDateTime(i, 4),//accepted_datetime
                                    res->getCellAsDateTime(i, 5),//receive_datetime
                                    res->getCellAsString (i, 0)//address
                                    );
            
            JKKSPMessage pM(cfm.serialize(), cfm.getMessageType());
            pM.verifyReceiver = false;
            JKKSPMessWithAddr * pMessWithAddr(new JKKSPMessWithAddr(pM, cfm.getAddr(), cfm.id()));
            result.append(pMessWithAddr);
        }
    }
    if (res)
        delete res;

    return result;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readMailConfirmations(void) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uGetMsgsConfirm ();");
    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 4)
    {
        for (int i=0; i<res->getRowCount(); i++) {
            JKKSMailConfirmation cfm(res->getCellAsInt(i, 1),//src_id
                                     res->getCellAsInt (i, 2),//extra_id
                                     res->getCellAsDateTime (i, 3),//read_datetime,
                                     res->getCellAsDateTime(i, 4),//receive_datetime
                                     res->getCellAsString (i, 0)//address
                                     );
            
            JKKSPMessage pM(cfm.serialize(), cfm.getMessageType());
            pM.verifyReceiver = false;
            JKKSPMessWithAddr * pMessWithAddr(new JKKSPMessWithAddr(pM, cfm.getAddr(), cfm.id()));
            result.append(pMessWithAddr);
        }
    }
    if (res)
        delete res;

    return result;
}


QList<JKKSPMessWithAddr *> JKKSLoader :: readCommands (void) const
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

            int idCat = res->getCellAsInt (i, 10);

            QString s;//empty string
            JKKSCommand command(res->getCellAsInt (i, 1),//id
                                res->getCellAsString (i, 22),//dl_from_uid
                                res->getCellAsString (i, 3),//dl_from_name
                                res->getCellAsString (i, 23),//dl_executor_uid
                                res->getCellAsString (i, 5),//dl_executor_name
                                res->getCellAsString (i, 6),//dl_executor_address
                                res->getCellAsString (i, 7),//exec_org_address
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
                                res->getCellAsString (i, 0),//full_address
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
                JKKSDocument ioDoc = readDocument (command.idObject());
                command.setAttachment (ioDoc);
            }
            command.setCategory(readCategories(idCat));
            JKKSPMessage pM(command.serialize(), command.getMessageType());
            pM.setUrgencyLevel(res->getCellAsString(i, 27));
            pM.verifyReceiver = false;
            JKKSPMessWithAddr * pMessWithAddr(new JKKSPMessWithAddr(pM, command.getAddr(), command.id()));
            result.append(pMessWithAddr);
        }
    }
    if (res)
        delete res;

    return result;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readDocuments (void) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uGetOutObjects();");

    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 8)
    {
        for (int i=0; i<res->getRowCount(); i++)
        {
            QString fullAddr = res->getCellAsString (i, 0);
            int idObject = res->getCellAsInt (i, 1);
            int idCommand = res->getCellAsInt (i, 6);
            int idJournal = res->getCellAsInt (i, 8);
            QDateTime rt = res->getCellAsDateTime (i, 4);
            JKKSDocument doc = readDocument (idObject);
            doc.setCommandId (idCommand);
            doc.setJournal (idJournal);
            doc.setRealTime (rt);
            doc.setAddr (fullAddr);
            
            JKKSPMessage pM(doc.serialize(), doc.getMessageType());
            pM.verifyReceiver = false;
            JKKSPMessWithAddr * pMessWithAddr(new JKKSPMessWithAddr(pM, doc.getAddr(), doc.getJournal()));
            result.append(pMessWithAddr);
        }
    }

    if (res)
        delete res;

    return result;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readMails (void) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uGetOutMsgs();");

    if (res && res->getRowCount() > 0 && res->getColumnCount() >= 11)
    {
        for (int i=0; i<res->getRowCount(); i++)
        {
            QString fullAddr = res->getCellAsString (i, 0);
            int idMessage = res->getCellAsInt (i, 1);
            QString u_idDlFrom = res->getCellAsString (i, 15);
            QString u_idDlTo = res->getCellAsString (i, 16);
            QDateTime sentTime = res->getCellAsDateTime (i, 9);
            int idObject = res->getCellAsInt (i, 8);
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
                JKKSDocument doc (readDocument (idObject));
                doc.setCommandId (-1);//idCommand);
                doc.setJournal (-1);//idJournal);
                doc.setRealTime (sentTime);
                mMessage.setAttachment (doc);
            }

            JKKSPMessage pM(mMessage.serialize(), mMessage.getMessageType());
            pM.setUrgencyLevel(res->getCellAsString(i, 17));
            pM.verifyReceiver = false;
            JKKSPMessWithAddr * pMessWithAddr (new JKKSPMessWithAddr (pM, mMessage.getAddr(), mMessage.id()));

            result.append (pMessWithAddr);
         }
   }

    if (res)
        delete res;

    return result;
}

JKKSDocument JKKSLoader :: readDocument (int idObject) const
{
    QString docSql = QString ("select * from ioGetObject(%1)").arg (idObject);
    KKSResult * dRes = dbRead->execute (docSql);
    if (!dRes)
    {
        return JKKSDocument ();
    }

    int idCat = dRes->getCellAsInt (0, 1);
    QString s;//empty string

    JKKSDocument doc (dRes->getCellAsString (0, 4), // name
                      //dRes->getCellAsString (0, 5), // code
                      idCat, // category
                      dRes->getCellAsInt (0, 2), // author
                      dRes->getCellAsInt (0, 3), // io_state
                      dRes->getCellAsString (0, 6), // tableName
                      dRes->getCellAsString (0, 7), // description
                      dRes->getCellAsString (0, 8), // info
                      dRes->getCellAsInt (0, 13), // mac label
                      s,
                      s,
                      dRes->getCellAsString(0, 14), // uniqueID
                      dRes->getCellAsInt (0, 15), // id_sync_type
                      dRes->getCellAsString (0, 16), // sync_name
                      dRes->getCellAsString (0, 17), // sync_desc
                      dRes->getCellAsInt (0, 18), // id_owner_org
                      dRes->getCellAsBool (0, 19), // is_global
                      dRes->getCellAsString (0, 24), //email_prefix (owner org UID)
                      dRes->getCellAsString (0, 20).isEmpty() ? QColor() : QColor::fromRgba (dRes->getCellAsInt64 (0, 20)), // background color
                      dRes->getCellAsString (0, 21).isEmpty() ? QColor() : QColor::fromRgba (dRes->getCellAsInt64 (0, 21)) // foreground color
                      );
    
    doc.setId (idObject);
    QMap<int, JKKSCategory> cat = readCategories (idCat);
    doc.setCategory (cat);

    JKKSType type (dRes->getCellAsInt (0, 25), 
                   dRes->getCellAsString (0, 26), 
                   dRes->getCellAsString (0, 27),
                   dRes->getCellAsString (0, 28));

    doc.setType(type);

    delete dRes;
 
    QString attrSql = QString ("select * from ioGetObjectAttrsEx(%1)").arg (idObject);
    KKSResult * aRes = dbRead->execute (attrSql);
    QMap<QString, QString> aVals;
    if (!aRes)
        return JKKSDocument ();

    for (int ii=0; ii<aRes->getRowCount(); ii++)
    {
    /*    if (aRes->getCellAsInt (ii, 5) == 2  ||
            aRes->getCellAsInt (ii, 5) == 3  ||
            aRes->getCellAsInt (ii, 5) == 7 ||
            aRes->getCellAsInt (ii, 5) == 12  ||
            aRes->getCellAsInt (ii, 5) == 17  ||
            aRes->getCellAsInt (ii, 5) == 19 ||
            aRes->getCellAsInt (ii, 5) == 26 )
        {
            QStringList strL = aRes->getCellAsStringList (ii, 3);
            QList<int> ids;
            for (int iii=0; iii<strL.count(); iii++)
            {
                int nnn = strL[iii].toInt();
                ids += nnn;
            }
            QString rstr = this->getUIDSbyIDs(aRes->getCellAsString(ii, 4), dRes->getCellAsString(0, 6), ids);
            aVals.insert (aRes->getCellAsString (ii, 4), rstr);
        }
        else*/
            aVals.insert (aRes->getCellAsString (ii, 4), aRes->getCellAsString (ii, 3));//здесь в качестве ключа используется поле unique_id
    }

    delete aRes;

    doc.setAttrValues (aVals);
    QString rubrSql = QString ("select * from iogetrubrics (%1);").arg (idObject);
    
    KKSResult * rubrRes = dbRead->execute (rubrSql);
    if (!rubrRes)
        return JKKSDocument ();

    QMap<int, JKKSGlobalRubric> rubrs;
    for (int ii=0; ii<rubrRes->getRowCount(); ii++)
    {
        if (rubrRes->getCellAsInt (ii, 7) == 2)
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
        int idCategory = rubrRes->getCellAsInt (ii, 4);
        if (idCategory > 0)
        {
            QMap<int, JKKSCategory> rubrCats = readCategories (idCategory);
            wRubr.setCategory (rubrCats);
        }
        int idSearchTemplate = rubrRes->getCellAsInt (ii, 3);
        if (idSearchTemplate > 0)
        {
            JKKSSearchTemplate st = readSearchTemplate (idSearchTemplate);
            wRubr.setSearchTemplate (st);
        }
        rubrs.insert (rubrRes->getCellAsInt (ii, 0), wRubr);
    }
    doc.setRubrics (rubrs);
    QMap<int, JKKSIOUrl> files = readDocumentFiles (doc.id());
    doc.setUrls (files);

    QMap<int, JKKSIOTable> tables = readDocumentTables (doc.id());
    doc.setTables (tables);

    return doc;
}

int JKKSLoader :: writeMessage (const JKKSPMessage & pMessage) const
{
    int result = ERROR_CODE;
    
    if(pMessage.verifyReceiver){
        if(receiverUID.isEmpty()){
            QString sql =  QString ("select * from getlocalorg();");
            KKSResult * r1 = dbWrite->execute (sql);
            if (!r1 || r1->getRowCount() != 1)
            {
                qWarning() << "ERROR! ";
                return IGNORE_CODE;
            }
            else
                receiverUID = r1->getCellAsString (0, 3);
            
            if(r1)
                delete r1;
        }
        if(pMessage.receiverUID != receiverUID)
            return IGNORE_CODE;//message was addressed NOT to this organization
    }

    JKKSMessage * message (unpackMessage(pMessage));
    if (message)
    {
        result = message->writeToDB (this, pMessage.senderUID, pMessage.receiverUID);
        delete message;
        if(result == ERROR_CODE){
            int a=0;
            Q_UNUSED (a);
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
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! Document (UID = " << doc->uid() << ") has no categories!";
        return ERROR_CODE;
    }

	doc->setCategory (pairToMap(cCats));
    
    doc->setIdIoCat(cCats.mainCategory().id());//задаем категорию документа (в приемной БД, ее ИД отличается от ИД БД-источника)

    int realId = checkForExist(doc->uid());

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

int JKKSLoader :: checkForExist(const QString & docUID) const
{
    QString sql = QString ("select * from checkForExist('%1');").arg (docUID);
    
    KKSResult * res = dbWrite->execute (sql);
    if (!res || res->getRowCount() != 1){
        if(res)
            delete res;
        
        return -1;
    }

    int id = res->getCellAsInt(0, 0);
    delete res;

    return id;

}

int JKKSLoader :: updateDocument (JKKSDocument *doc) const
{
    int result = ERROR_CODE;
    if(!doc)
        return result;

    QString sql = QString("select ioSetUpdating(%1)").arg(doc->id());
    KKSResult * res = dbWrite->execute(sql);
    if(res && res->getRowCount() == 1){
        result = res->getCellAsInt(0, 0);
    }
    delete res;
    if(result != 1){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! ioSetUpdating(" << doc->id() << ") for document with UID = " << doc->uid() << " failed!";
        return ERROR_CODE;
    }

    result = ERROR_CODE;

    JKKSType type = doc->getType();
    int idType;
    if(type.getName().isEmpty() || type.uid().isEmpty()){
        idType = 1;
    }
    else{
        idType = writeIOType(type);
    }


    //
    //запись в таблице io_objects
    //
    sql = QString ("select * from ioUpdate (%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11);")
        .arg (doc->id())
        .arg (doc->getName().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getName()))//name
        .arg (doc->getIdState () < 0 ? QString ("NULL") : QString::number (doc->getIdState()) )//id_state
        //.arg (doc->getTableName().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getTableName()))//table_name
        .arg (doc->getDescription().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getDescription())) //desc
        .arg (doc->getInfo().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getInfo()))//info
        .arg (QString::number (doc->getIdMaclabel ()))//id_maclabel
        .arg (doc->getSyncType() < 0 ? QString ("NULL") : QString::number (doc->getSyncType()) ) //id_sync_type
        .arg (doc->getGlobal() ? QString ("TRUE") : QString ("FALSE")) //is_global
        .arg (doc->getBkCol().isValid() ? QString::number (doc->getBkCol().rgba()) : QString ("NULL"))
        .arg (doc->getFgCol().isValid() ? QString::number (doc->getFgCol().rgba()) : QString ("NULL"))
        .arg ( QString::number(idType) ); //id_type


    res = dbWrite->execute (sql);

    if (res && res->getRowCount() == 1)
        result = res->getCellAsInt (0, 0);
    delete res;
    
    if (result <= 0){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! ioUpdate() for document with UID = " << doc->uid() << " failed!";
        return ERROR_CODE;
    }
    
    //
    //значения атрибутов
    //
    QMap<QString, QString> attrVals = doc->getAttrValues ();
    for (QMap<QString, QString>::const_iterator pa = attrVals.constBegin(); pa != attrVals.constEnd(); pa++)
    {
        QString attrSql = QString ("select * from ioUpdateAttrEx (%1, %2, %3, NULL::timestamp, NULLL::timestamp, NULL::timestamp, NULL::int4, NULL::int4, NULL::varchar);")
            .arg (doc->id())
            .arg (QString("'")+pa.key()+QString("'")) //здесь в качестве ключа используется unique_id
            .arg (pa.value().isEmpty() ? QString ("NULL") : QString("'")+pa.value()+QString("'"));
        KKSResult * attrRes = dbWrite->execute (attrSql);

        int attr_ier = ERROR_CODE;
        if (attrRes)
        {
            attr_ier = attrRes->getCellAsInt (0, 0);
            delete attrRes;
        }
        if (attr_ier <= 0){
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! ioUpdateAttrEx() for document with UID = " << doc->uid() << " failed!";
            return ERROR_CODE;
        }
    }

    //
    //рубрики
    // в настоящее время только добавляются новые рубрики и обновляются существующие.
    // удаления удаленных не происходит
    //
    QMap<int, JKKSGlobalRubric> rubrs = doc->rubrics ();
    QMap<int, int> idpRub;
    //qDebug () << __PRETTY_FUNCTION__ << rubrs.count();
    for (QMap<int, JKKSGlobalRubric>::const_iterator pr = rubrs.constBegin (); 
            pr != rubrs.constEnd (); 
            pr++)
    {
        if (pr.key () < 0)
            continue;

        JKKSGlobalRubric wRubr = pr.value ();
        if (wRubr.getParent () >= 0 && idpRub.constFind (wRubr.getParent ()) != idpRub.constEnd ())
            wRubr.setParent (idpRub.constFind (wRubr.getParent ()).value ());

        int idChild = -1;
        if (wRubr.getCategory().count() != 0)
        {
            JKKSCategoryPair cCats = parseCategories (wRubr.getCategory());
            if(cCats.isNull()){
                qWarning() << __PRETTY_FUNCTION__ << "ERROR! parseCategories() for rubrics for document with UID = " << doc->uid() << " failed!";
                return ERROR_CODE;
            }
            
            idChild = cCats.mainCategory().id();
        }

        int idSearchT = -1;
        if (wRubr.getSearchTemplate().id() > 0)
        {
            idSearchT = writeSearchTemplate (wRubr.getSearchTemplate());
        }
        QString sql =  QString ("select * from ioUpdateInclude ('%1', '%2', '%3', '%4', NULL, %7, %5, %6 );")
                                .arg (wRubr.getUid())
                                .arg (wRubr.getName ())
                                .arg (wRubr.getCode ())
                                .arg (wRubr.getDesc ())
                                .arg (idChild <= 0 ? QString ("NULL") : QString::number (idChild))
                                .arg (idSearchT < 0 ? QString ("NULL") : QString::number (idSearchT))
                                .arg (result);
        //qDebug () << __PRETTY_FUNCTION__ << sql;


        KKSResult * res = dbWrite->execute (sql);
        if(!res || res->getRowCount () != 1 || res->getCellAsInt(0, 0) <= 0){
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! ioUpdateInclude() for document with UID = " << doc->uid() << " failed!";
            if(res)
                delete res;
            return ERROR_CODE;
        }
        else
            idpRub.insert (wRubr.getId (), res->getCellAsInt (0, 0));

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
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! rRemoveObjUrl() for document with UID = " << doc->uid() << " failed!";
        return ERROR_CODE;
    }
    
    //создаем новые
    QMap<int, JKKSIOUrl> urls = doc->urls ();
    for (QMap<int, JKKSIOUrl>::const_iterator pf = urls.constBegin (); 
            pf != urls.constEnd (); 
            pf++)
    {
        JKKSIOUrl url = pf.value ();
        url.setIOId (doc->id ());
        this->writeDocumentFile (url);//pf.value());
    }

    //
    //дополнительные таблицы 
    //
    QMap<int, JKKSIOTable> tables = doc->ref_tables ();
    for (QMap<int, JKKSIOTable>::const_iterator pt = tables.constBegin (); 
            pt != tables.constEnd(); 
            pt++)
    {
        JKKSIOTable t = pt.value ();
        t.setIDTable (doc->id());
        
        JKKSCategoryPair cCats = parseCategories(t.getCategory());
        if(cCats.isNull()){
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! parseCategories() for addition tables for document with UID = " << doc->uid() << " failed!";
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
                qWarning () << __PRETTY_FUNCTION__ << "ERROR! ioupdatetable() for addition tables for document with UID = " << doc->uid() << " failed!";
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
        qWarning () << __PRETTY_FUNCTION__ << "ERROR! ioSetCompleted() for document with UID = " << doc->uid() << " failed!";
        return ERROR_CODE;
    }
    if(complRes)
        delete complRes;

    return result;
}

int JKKSLoader :: deleteDocument (JKKSDocument *doc) const
{
    int result = ERROR_CODE;
    if(!doc)
        return result;

    return result;
}

int JKKSLoader :: insertDocument (JKKSDocument *doc) const
{
    int result = ERROR_CODE;
    if(!doc)
        return result;

    QString ioTableName = doc->getTableName();

    JKKSType type = doc->getType();
    int idType;
    if(type.getName().isEmpty() || type.uid().isEmpty()){
        idType = 1;
    }
    else{
        idType = writeIOType(type);
    }

    QString sql = QString ("select * from ioInsert (%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, NULL, NULL, %13, %14, %15);")
        .arg (doc->getName().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getName()))//name
        .arg (doc->getIdIoCat())//id_io_category
        .arg (QString ("NULL"))// ибо в БД-приемнике автора (пользователя) может не быть (как правило не будет)
        .arg (doc->getIdState () < 0 ? QString ("NULL") : QString::number (doc->getIdState()) )//id_state
        .arg (QString ("NULL"))//table_name
        .arg (doc->getDescription().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getDescription())) //desc
        .arg (doc->getInfo().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->getInfo()))//info
        .arg (QString::number (doc->getIdMaclabel ()))//id_maclabel
        .arg (doc->uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (doc->uid()))//unique_id
        .arg (doc->getSyncType() < 0 ? QString ("NULL") : QString::number (doc->getSyncType()) ) //id_sync_type
        .arg (doc->getOwnerOrgUID().isEmpty() ? QString ("NULL") : QString::number (getIdByUID("organization", doc->getOwnerOrgUID()))) //id_owner_org
        .arg (doc->getGlobal() ? QString ("TRUE") : QString ("FALSE")) //is_global
        .arg (doc->getBkCol().isValid() ? QString::number (doc->getBkCol().rgba()) : QString ("NULL"))
        .arg (doc->getFgCol().isValid() ? QString::number (doc->getFgCol().rgba()) : QString ("NULL"))
        .arg (QString::number(idType));//id_io_type

    KKSResult * res = dbWrite->execute (sql);

    if (res && res->getRowCount() == 1)
        result = res->getCellAsInt (0, 0);
    
    if(res)
        delete res;
    
    if (result <= 0 ){
        qWarning () << __PRETTY_FUNCTION__ << "ERROR! ioInsert() for document with UID = " << doc->uid() << " failed!";
        return ERROR_CODE;
    }
    
    doc->setId (result);

    if (!ioTableName.isEmpty())
    {
        QString tSql (QString("select * from ioGetObject (%1)").arg (result));
        KKSResult * tableRes = dbWrite->execute (tSql);
        if (!tableRes || tableRes->getRowCount() == 0)
        {
            qWarning () << __PRETTY_FUNCTION__ << "ERROR! ioGetObject(" << result << ") for document with UID = " << doc->uid() << " failed!";
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
        QString attrSql = QString ("select * from ioInsertAttrEx (%1, %2, %3, NULL::timestamp, NULLL::timestamp, NULL::timestamp, NULL::int4, NULL::int4, NULL::varchar);")
            .arg (doc->id())
            .arg (QString("'")+pa.key()+QString("'")) //здесь в качестве ключа используется unique_id
            .arg (pa.value().isEmpty() ? QString ("NULL") : QString("'")+pa.value()+QString("'"));
        KKSResult * attrRes = dbWrite->execute (attrSql);

        int attr_ier = ERROR_CODE;
        if (attrRes)
        {
            attr_ier = attrRes->getCellAsInt (0, 0);
            delete attrRes;
        }
        if (attr_ier <= 0){
            qWarning () << __PRETTY_FUNCTION__ << "ERROR! ioInsertAttrEx() for document with UID = " << doc->uid() << " failed!";
            return ERROR_CODE;
        }
    }

    QMap<int, JKKSGlobalRubric> rubrs = doc->rubrics ();
    QMap<int, int> idpRub;
    
    //qDebug () << __PRETTY_FUNCTION__ << rubrs.count();
    for (QMap<int, JKKSGlobalRubric>::const_iterator pr = rubrs.constBegin (); \
            pr != rubrs.constEnd (); \
            pr++)
    {
        if (pr.key () < 0)
            continue;

        JKKSGlobalRubric wRubr = pr.value ();
        if (wRubr.getParent () >= 0 && idpRub.constFind (wRubr.getParent ()) != idpRub.constEnd ())
            wRubr.setParent (idpRub.constFind (wRubr.getParent ()).value ());

        int idChild = -1;
        if (wRubr.getCategory().count() != 0)
        {
            JKKSCategoryPair cCats = parseCategories (wRubr.getCategory());
            if(cCats.isNull()){
                qWarning() << __PRETTY_FUNCTION__ << "ERROR! parseCategories() for rubrics for document with UID = " << doc->uid() << " failed!";
                return ERROR_CODE;
            }
            
            idChild = cCats.mainCategory().id();
        }

        int idSearchT = -1;
        if (wRubr.getSearchTemplate().id() > 0)
        {
            idSearchT = writeSearchTemplate (wRubr.getSearchTemplate());
        }
        QString sql =  QString ("select * from ioinsertinclude (%1, %2, '%3', '%4', '%5', %8, %7, '%6');")
                                .arg (wRubr.getParent () <= 0 ? QString ("NULL") : QString::number (wRubr.getParent ()))
                                .arg (pr == rubrs.constBegin () ? QString::number (doc->id()) : QString ("NULL"))
                                .arg (wRubr.getName ())
                                .arg (wRubr.getCode ())
                                .arg (wRubr.getDesc ())
                                .arg (wRubr.getUid  ())
                                .arg (idChild<0 ? QString("NULL::int4") : QString::number(idChild))
                                .arg (idSearchT<0 ? QString ("NULL::int4") : QString::number (idSearchT));

        KKSResult * res = dbWrite->execute (sql);
        
        if (!res || res->getRowCount () != 1 || res->getCellAsInt(0, 0) <= 0){
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! ioinsertinclude() for document with UID = " << doc->uid() << " failed!";
            if(res)
                delete res;
            return ERROR_CODE;
        }
        else
            idpRub.insert (wRubr.getId (), res->getCellAsInt (0, 0));

        if (res)
            delete res;
    }

    QMap<int, JKKSIOUrl> urls = doc->urls ();
    for (QMap<int, JKKSIOUrl>::const_iterator pf = urls.constBegin (); \
            pf != urls.constEnd (); \
            pf++)
    {
        JKKSIOUrl url = pf.value ();
        url.setIOId (doc->id ());
        this->writeDocumentFile (url);//pf.value());
    }

    QMap<int, JKKSIOTable> tables = doc->ref_tables ();
    for (QMap<int, JKKSIOTable>::const_iterator pt = tables.constBegin (); \
            pt != tables.constEnd(); \
            pt++)
    {
        JKKSIOTable t = pt.value ();
        t.setIDTable (doc->id());

        JKKSCategoryPair cCats = parseCategories(t.getCategory());
        if(cCats.isNull()){
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! parseCategories() for addition tables for document with UID = " << doc->uid() << " failed!";
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
                qWarning() << __PRETTY_FUNCTION__ << "ERROR! ioinserttable() for document with UID = " << doc->uid() << " failed!";
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
        
        //qDebug () << __PRETTY_FUNCTION__ << cjSql;

        KKSResult *cjRes = dbWrite->execute (cjSql);

        if (!cjRes || cjRes->getRowCount() != 1 || cjRes->getCellAsInt(0, 0) <= 0){
            if(cjRes)
                delete cjRes;
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! cjInsertEx() for document with UID = " << doc->uid() << " failed!";
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
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! cjSetAsResult() for document with UID = " << doc->uid() << " failed!";
            return ERROR_CODE;
        }
        
        if(cr)
            delete cr;
        
        cjSql = QString ("select id_dl_from from command_journal where id=%1").arg (doc->getCommandId());
        
        KKSResult *cPriv = dbWrite->execute (cjSql);
        if (!cPriv)
            qWarning () << __PRETTY_FUNCTION__ << QString ("Error in command journal");
        else
        {
            int idRole = cPriv->getCellAsInt (0, 0);
            int idObject = doc->id();
            cjSql = QString ("select setprivileges (%1, %2, TRUE, TRUE, TRUE, TRUE, TRUE);").arg (idRole).arg (idObject);
            KKSResult *cSPriv = dbWrite->execute (cjSql);
            if (!cSPriv)
                qDebug () << __PRETTY_FUNCTION__ << QString ("Cannot set privilegies");
            else
                delete cSPriv;
            delete cPriv;
        }
    }

    QString sqlComplete (QString ("select * from ioSetCompleted (%1, 1);").arg (doc->id()));
    
    //qDebug () << __PRETTY_FUNCTION__ << sqlComplete;
    
    KKSResult * complRes = dbWrite->execute (sqlComplete);
    
    if (!complRes)
        qDebug () << __PRETTY_FUNCTION__ << QString ("Cannot complete record");
    else
        delete complRes;

    return result;
}

int JKKSLoader :: writeAddTable (int idObject, JKKSIOTable& table) const
{
    JKKSCategoryPair cCats = parseCategories(table.getCategory());
    if(cCats.isNull()){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! parseCategories() for addition tables IdObject = " << idObject << " failed!";
        return ERROR_CODE;
    }
        
    table.setCategory (pairToMap(cCats));

    int idt = -1;
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
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! ioinserttable() for addition tables IdObject = " << idObject << " failed!";
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

int JKKSLoader::getIdByUID(const QString & tableName, const QString & uid) const
{
    int id = -1;
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
        id = tRes->getCellAsInt (0, 0);
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

        QString sql = QString ("select * from msginsertin (%1, %2, %3, %4, %5, %6, %7, %8, %9)")
                                .arg (mMess->getIO() > 0 ? QString::number (mMess->getIO()) : QString ("NULL::integer"))
                                .arg (mMess->getMessageBody().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (mMess->getMessageBody()))
                                .arg (getIdByUID("position", mMess->getIdDlTo()))
                                .arg (getIdByUID("position", mMess->getIdDlFrom()))
                                .arg (mMess->getSentDateTime().isNull() ? QString("NULL") :
                                        QString ("to_timestamp('") +
                                        mMess->getSentDateTime().toString ("dd.MM.yyyy hh:mm:ss") +
                                        QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp"))
                                .arg(mMess->id()) //запишется как extra_id
                                .arg(mMess->uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (mMess->uid()))
                                .arg(mMess->outputNumber().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (mMess->outputNumber()))
                                .arg(mMess->idUrgencyLevel() > 0 ? mMess->idUrgencyLevel() : 1);

        //qDebug() << __PRETTY_FUNCTION__ << sql;

        KKSResult * res = dbWrite->execute (sql);
        
        if (res && res->getRowCount() == 1)
            result = res->getCellAsInt (0, 0);
        if (res)
            delete res;

        if (doc && result != ERROR_CODE)
        {
            QString idRole = mMess->getIdDlTo();
            int idObject = mMess->getIO();

            QString mMessSql = QString ("select setprivileges ('%1', %2, TRUE, TRUE, TRUE, TRUE, TRUE);") .arg (idRole).arg (idObject);
            KKSResult *cSPriv = dbWrite->execute (mMessSql);
            if (!cSPriv || cSPriv->getCellAsInt(0, 0) <= 0)
                qDebug () << __PRETTY_FUNCTION__ << QString ("Cannot set privilegies");
            if(cSPriv)
                delete cSPriv;

            delete doc;
        }
    }

    return result;
}

JKKSMessage * JKKSLoader::unpackMessage (const JKKSPMessage & pMessage) const
{
    //qDebug() << __PRETTY_FUNCTION__ << "type: " << pMessage.getType();
    JKKSMessage * message = 0;
    switch (pMessage.getType())
    {
        case JKKSMessage::atCommand: message = new JKKSCommand(); break;
        //TODO process other message types
        case JKKSMessage::atDocument: message = new JKKSDocument(); break;
        case JKKSMessage::atMailMessage: message = new JKKSMailMessage (); break;
        case JKKSMessage::atMailConfirmation: message = new JKKSMailConfirmation(); break;
        case JKKSMessage::atCmdConfirmation: message = new JKKSCmdConfirmation(); break;
        case JKKSMessage::atRecord: message = new JKKSRefRecord(); break;
        case JKKSMessage::atRecConfirmation: message = new JKKSQueueResponse(); break;
        case JKKSMessage::atOrganization: message = new JKKSOrganization (); break;
        case JKKSMessage::atPosition: message = new JKKSRefRecord(); break;
        case JKKSMessage::atOrgPackage: message = new JKKSOrgPackage(); break;
        default: qDebug("Error: unknown message type");
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

    return result;
}

JKKSCategoryPair JKKSLoader::parseCategories(const QMap<int, JKKSCategory> & oldCats) const
{
    JKKSCategoryPair cCats;
    JKKSCategoryPair cNull;

	int cnt = oldCats.count();

	if(cnt < 1 || cnt > 2)
		return cNull;
	

    QMap<int, JKKSCategory>::const_iterator pc = oldCats.constBegin();
    for (; pc != oldCats.constEnd(); ++pc )
    {
        JKKSCategory c = pc.value();
        c.setState(1);
        if(cnt == 1){
            c.setIDChild(-1);
            cCats.setMainCategory(c);
            break;
        }

        if(c.getIDChild() > 0)
            cCats.setMainCategory(c);
        else            
            cCats.setChildCategory(c);
    }

    if(cCats.isNull())
        return cNull;

    int idChild = -1;
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

    c = cCats.mainCategory();
    c.setIDChild(idChild);
    res = writeCategory (c);
    if (res != ERROR_CODE){
        cCats.setMainCategory(c);
    }
    else
        return cNull;
    
	return cCats;
}
QMap<int, JKKSCategory> JKKSLoader::pairToMap(const JKKSCategoryPair & pair) const
{
    QMap<int, JKKSCategory> newCats;
    newCats.insert(pair.mainCategory().id(), pair.mainCategory());
    if(!pair.isAlone())
        newCats.insert(pair.childCategory().id(), pair.childCategory());

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
            command->setIdObject (resDoc);
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
                            .arg (command->getMessageBody()) //message_body
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
                            .arg (command->getMessageBody()) //message_body
                            .arg (command->uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (command->uid())) //unique_id
                            .arg (command->outputNumber().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (command->outputNumber())) //output_number
                            .arg (command->idUrgencyLevel() > 0 ? command->idUrgencyLevel() : 1) //id_urgency_level
                            .arg (command->idObject() > 0 ? QString::number (command->idObject()) : QString("NULL")); //id_io_object_att
        }

        //qDebug() << sql;

        KKSResult * res = dbWrite->execute (sql);
        
        if (res && res->getRowCount() == 1)
            result = res->getCellAsInt (0, 0);
        delete res;
    }
    if(result <= 0)
        qDebug() << "ERROR! " << __PRETTY_FUNCTION__ << sql;
    return result;
}

QPair<int, JKKSCategory> JKKSLoader :: readCategory (int idCat) const
{
    QPair<int, JKKSCategory> result;

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
                                res->getCellAsBool (0, 7), // main category
                                res->getCellAsString(0, 9), //uniqueID
                                res->getCellAsInt(0, 11), //id_io_state
                                res->getCellAsBool(0, 14) //is_global
                                );

        QMap<int, JKKSCategoryAttr> attrs = readCategoryAttrs(idCat);
        QMap<int, JKKSRubric> rubrs = readCategoryRubrics (idCat);
        
        result.second.setAttributes (attrs);
        result.second.setRubrics (rubrs);
    }

    delete res;
    return result;
}

QMap<int, JKKSCategory> JKKSLoader :: readCategories (int idCat) const
{
    QMap<int, JKKSCategory> result;
    if(idCat > 0){

        QPair<int, JKKSCategory> category = readCategory(idCat);
        result.insert(category.first, category.second);

        while(category.second.getIDChild() > 0 ){
            category = readCategory(category.second.getIDChild());
            result.insert(category.first, category.second);
        }
    }
    return result;
}

int JKKSLoader :: writeCategory (JKKSCategory& cat) const
{
    int result = ERROR_CODE;

    if (!cat.getName().isEmpty())
    {
        QString sql = QString ("select * from cinsert ('%1', %2, %3, %4, %5, %6, %7, %8, %9);")
                                .arg (cat.getName())
                                .arg (QString ("NULL"))//FIXME
                                .arg (cat.getDescription().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (cat.getDescription()))
                                .arg (cat.getType())
                                .arg (cat.getIDChild() < 0 ? QString("NULL") : QString::number (cat.getIDChild()))
                                .arg (cat.isMain() ? QString("true") : QString("false"))
                                .arg (cat.uid().isEmpty() ? QString ("NULL") : QString ("'%1'").arg (cat.uid()))
                                .arg (cat.isGlobal() ? QString("true") : QString("false"))
                                .arg (cat.getState());

        KKSResult *res = dbWrite->execute (sql);

        if (res && res->getRowCount () == 1)
        {
            int idc = res->getCellAsInt (0, 0);
            cat.setId (idc);
            writeCategoryAttrs (cat);
            writeCategoryRubrics (cat);
            result = OK_CODE;
        }

        if (res)
            delete res;
    }

    if(result == ERROR_CODE){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! cinsert() for categoryUID = " << cat.uid()<< " failed!";
    }

    return result;
}

QMap<int, JKKSCategoryAttr> JKKSLoader :: readCategoryAttrs (int idCat) const
{
    QMap<int, JKKSCategoryAttr> attrs;
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
                //qDebug () << __PRETTY_FUNCTION__ << tableUid;
                attr.setTableUid (tableUid);
                delete tRes;
            }
            attrs.insert (res->getCellAsInt (i, 0), attr);
        }

        delete res;
    }
    return attrs;
}

JKKSCategoryAttr JKKSLoader :: readAttribute (int id) const
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
        qDebug () << __PRETTY_FUNCTION__ << tableUid;
        attr.setTableUid (tableUid);
        delete tRes;
    }
    delete res;
    return attr;
}

void JKKSLoader :: writeCategoryAttrs (const JKKSCategory& cat) const
{
    if (cat.id() < 0)
        return;

    QMap<int, JKKSCategoryAttr> attributes = cat.attributes ();
    for (QMap<int, JKKSCategoryAttr>::const_iterator pa = attributes.constBegin();
         pa != attributes.constEnd();
         pa++) 
    {
        if (pa.value().code().isEmpty() || pa.value().name().isEmpty() || pa.value().title().isEmpty())
            continue;

        QString tableUid = pa.value().getTableUID ();
/*        QString tableName = pa.value().table();

        if (!tableUid.isEmpty())
        {
            QString tableSql = QString ("select ioGetTableNameByUid('%1');").arg (tableUid);
            KKSResult * tRes = dbWrite->execute (tableSql);
            if (!tRes || tRes->getRowCount () != 1)
            {
                if (tRes)
                    delete tRes;
                continue;
            }
            tableName = tRes->getCellAsString (0, 0);
            if(tableName.isEmpty()){
                qWarning() << "ERROR! WriteCategoryAttrs: ioGetTableNameByUid() returns NULL value! TableUID = " << tableUid;
            }
            delete tRes;
        }
        qDebug () << __PRETTY_FUNCTION__ << pa.value().uid();
 */
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
        if (res)
            delete res;
    }
}

QMap<int, JKKSRubric> JKKSLoader :: readCategoryRubrics (int idCat) const
{
    QMap<int, JKKSRubric> rubrs;

    if (idCat < 0)
        return rubrs;

    QString sql = QString("select * from cgetrubrics(%1)").arg (idCat);
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

void JKKSLoader :: writeCategoryRubrics (const JKKSCategory& cat) const
{
    //qDebug () << __PRETTY_FUNCTION__;
    int idCat = cat.id ();
    if (idCat < 0)
        return;

    QMap<int, JKKSRubric> rubrs = cat.rubrics ();
    QMap<int, int> idpRub;
    for (QMap<int, JKKSRubric>::const_iterator pr = rubrs.constBegin (); \
            pr != rubrs.constEnd (); \
            pr++)
    {
        if (pr.key () < 0)
            continue;

        JKKSRubric wRubr = pr.value ();
        if (wRubr.getParent () >= 0 && idpRub.constFind (wRubr.getParent ()) != idpRub.constEnd ())
            wRubr.setParent (idpRub.constFind (wRubr.getParent ()).value ());

        QString sql =  QString ("select * from cinsertrubric (%1, %2, '%3');")
                                .arg (wRubr.getParent () <= 0 ? QString ("NULL") : QString::number (wRubr.getParent ()))
                                .arg (pr == rubrs.constBegin () ? QString::number (cat.id()) : QString ("NULL"))
                                .arg (wRubr.getName ());

        KKSResult * res = dbWrite->execute (sql);
        if (res && res->getRowCount () == 1)
            idpRub.insert (wRubr.getId (), res->getCellAsInt (0, 0));

        if (res)
            delete res;
    }
}

int JKKSLoader :: setAsSended (int id, int idType) const
{
    QString sql = QString ("select * from uSetAsSended (%1, %2);").arg (id).arg (idType);
    KKSResult *res = dbRead->execute (sql);

    int ier = 0;
    if (res && res->getRowCount()==1)
        ier = res->getCellAsInt (0, 0);
    else
        qDebug() << __PRETTY_FUNCTION__ << "ERROR! setAsSended failed! SQL = " << sql;
    
    if(ier <= 0)
        qDebug () << __PRETTY_FUNCTION__ << "ERROR! Result setAsSended is " << ier << ". SQL = " << sql;
    
    if (res)
        delete res;

    return ier;
}

QMap<int, JKKSIOUrl> JKKSLoader :: readDocumentFiles (int idObject) const
{
    QMap<int, JKKSIOUrl> urls;
    QString sql = QString ("select * from iogetfiles (%1);").arg (idObject);
    KKSResult *res = dbRead->execute (sql);

    if (res)
    {
        for (int i=0; i<res->getRowCount (); i++)
        {
            int idUrl = res->getCellAsInt (i, 1);
            JKKSIOUrl res_url (idObject, // idObject
                               idUrl,
                               res->getCellAsString (i, 2),
                               res->getCellAsString (i, 3),
                               res->getCellAsInt (i, 4),
                               res->getCellAsString (i, 8)
                               );
            QByteArray b = getFileData (idUrl);
            //qDebug () << __PRETTY_FUNCTION__ << b;
            res_url.setData (b);
            urls.insert (idUrl, res_url);
        }
        delete res;
    }

    return urls;
}

int JKKSLoader :: writeDocumentFile (JKKSIOUrl& url) const
{
    QString url_ins_sql = QString ("select * from rinserturl ('%1', 'not assigned', %2, '%3');")
                                  .arg (url.getIOURL())
                                  //.arg (url.getURL())
                                  .arg (url.getType())
                                  .arg (url.getSrcExt ());

    KKSResult * url_res = dbWrite->execute (url_ins_sql);
    if (!url_res)
        return ERROR_CODE;

    int id = url_res->getCellAsInt (0, 0);
    if(id <= 0)
        return ERROR_CODE;

    url.setURLId (id);

    delete url_res;

    QString sql = QString ("select * from ioinserturl (%1, %2, '%3');")
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

QByteArray JKKSLoader :: getFileData (int idUrl, int blockSize) const
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

    int idUrl = url.getURLId ();
    QByteArray fPath = url.getURL().toUtf8();
    const char * filePath = fPath.constData();

    char * command = new char[100 + strlen(filePath)];
    int nParams = 2;
    sprintf(command, "select rWriteFile(%d, $1, $2);", idUrl);
    //qDebug () << __PRETTY_FUNCTION__ << command << blockSize;
    
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

    return OK_CODE;
}

QMap<int, JKKSIOTable> JKKSLoader :: readDocumentTables (int idObject) const
{
    QMap<int, JKKSIOTable> tables;
    QString sql = QString("select * from iogetobjecttables (%1);").arg (idObject);
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return tables;

    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSIOTable T = JKKSIOTable (idObject, 
                                     res->getCellAsString (i, 1), 
                                     res->getCellAsString (i, 4), 
                                     res->getCellAsString (i, 5));
        QMap<int, JKKSCategory> cats = this->readPCategories (res->getCellAsInt (i, 3));
        T.setCategory (cats);
        tables.insert (res->getCellAsInt (i, 0), T);
    }

    return tables;
}

QMap<int, JKKSCategory> JKKSLoader :: readPCategories (int idCatChild) const
{
    QMap<int, JKKSCategory> cats;
    QPair<int, JKKSCategory> C = readCategory (idCatChild);
    QPair<int, JKKSCategory> result;

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
    return cats;
}

int JKKSLoader :: writeMessage (JKKSRefRecord *refRec, const QString& sender_uid) const
{
    if (!refRec){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! refRecord isNULL!";
        return ERROR_CODE;
    }

    qWarning() << "JKKSRefRecord->getSenderAddr() = " << refRec->getSenderAddr();
    qWarning() << "JKKSRefRecord->getEntityType() = " << refRec->getEntityType();
    
    JKKSQueueResponse recResp (-1, refRec->getIDQueue(), 2, refRec->getSenderAddr());
    if (refRec->getEntityType () == 1)//category
    {
        JKKSCategoryPair cCats = parseCategories(refRec->getCategory());
        if (cCats.isNull())
            recResp.setResult(4);
        else
            recResp.setResult(3);

        generateQueueResponse (recResp);

        refRec->setCategory (pairToMap(cCats));
        
        return OK_CODE;
    }
    else if (refRec->getEntityType () == 2)//io_object
    {
        JKKSDocument ioDoc = refRec->getIODoc ();
        ioDoc.setIdState (1);
        int syncType = refRec->getSyncType();//тип синхронизации (1 - инсерт, 2 - апдейт, 3 - удаление)
        
        int res = writeMessage (&ioDoc, syncType);
        if (res != ERROR_CODE)
            recResp.setResult (3);
        else
            recResp.setResult (4);
        
        generateQueueResponse (recResp);
        return res;
    }
    else if (refRec->getEntityType () == 3 || refRec->getEntityType () == 4) // ЭИО (запись справочника)
                                                                             // ЭИО (запись доп. таблицы справочника)
    {
        QString tableUID = refRec->getTableUID ();
        //qDebug () << __PRETTY_FUNCTION__ << tableUID;
        QString tableSql = QString ("select ioGetTableNameByUid ('%1');").arg (tableUID);
        KKSResult * tRes = dbWrite->execute (tableSql);

        if (!tRes || tRes->getRowCount () != 1)
        {
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! writeMessage: ioGetTableNameByUid() fails! SQL = " << tableSql;
            if (tRes)
                delete tRes;

            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }

        QString newTableName = tRes->getCellAsString (0, 0);
        if(newTableName.isEmpty()){
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! writeMessage: ioGetTableNameByUid() returns NULL value! TableUID = " << tableUID;
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }

        delete tRes;

        if (refRec->uid().isEmpty()){
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! refRecord UID is empty!";
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
            qDebug () << "ERROR! " << __PRETTY_FUNCTION__ << recSql;
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
            generateQueueResponse (recResp);
            return OK_CODE;
        }
        else if (cnt == 0 && refRec->getSyncType() == 3)
        {
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! writeMessage: refRec sync type = " << refRec->getSyncType() << " but count of records with equal UID in database = " << cnt;
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }

        QString sql;

        if (refRec->getSyncType() == 1 || (cnt == 0 && refRec->getSyncType() == 2))
        {
            QPair<int, int> id_table_map = getIDMap (tableUID, *refRec);
            if (id_table_map.second > 0)
            {
                QPair<QString, int> rec (newTableName, id_table_map.first);
                records.insert (rec, id_table_map.second);
                recResp.setResult (3);
            }
            else
                recResp.setResult (4);
            generateQueueResponse (recResp);
            sql = QString();
        }
        else if (cnt > 0 && refRec->getSyncType() == 2)
        {
            sql = QString ("select recUpdate ('%1',").arg (tableUID);//QString ("UPDATE %1 SET ").arg (newTableName);
            QMap<int, JKKSCategory> cat = refRec->getCategory();
            if (cat.isEmpty())
            {
                qWarning() << __PRETTY_FUNCTION__ << "ERROR! writeMessage: category is empty!";
                recResp.setResult (4);
                generateQueueResponse (recResp);
                return ERROR_CODE;
            }

            JKKSCategory C = cat.constBegin().value();
            if (C.getType() != 10)
            {
                for (QMap<int, JKKSCategory>::const_iterator pc = cat.constBegin();
                     C.getType() != 10 && pc != cat.constEnd();
                     pc++)
                    C = pc.value();
            }
            QStringList attrsVals = refRec->attrsValues ();
            int ic = attrsVals.count () - C.attributes().count();

            QString attrsUids = QString ("ARRAY[");
            QString attrsValues = QString ("ARRAY[");
            for (QMap<int, JKKSCategoryAttr>::const_iterator pa = C.attributes().constBegin(); 
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
            qDebug () << __PRETTY_FUNCTION__ << sql;
        }
        else if (cnt > 0 && refRec->getSyncType() == 3)
            sql = QString ("DELETE FROM %1 WHERE unique_id = '%2'").arg (newTableName).arg (refRec->uid());

        if (!sql.isEmpty())
        {
            KKSResult * res = dbWrite->execute (sql);
            if (!res || res->resultStatus() >= KKSResult::BadResponse || res->resultStatus() == KKSResult::EmptyQuery)
            {
                recResp.setResult (4);
                generateQueueResponse (recResp);
                qDebug () << "ERROR! " << __PRETTY_FUNCTION__ << sql;
                return ERROR_CODE;
            }

            delete res;
        }
        recResp.setResult (3);
        generateQueueResponse (recResp);
    }
    else if (refRec->getEntityType () == 5)
    {
        QString ioUID= refRec->getTableUID ();
        QString sql = QString ("select iogetobjectidbyuid ('%1');").arg (ioUID);

        KKSResult * ioIDRes = dbWrite->execute (sql);
        if (!ioIDRes || ioIDRes->getRowCount() != 1)
        {
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! iogetobjectidbyuid() for object UID = " << ioUID << " failed!";
            if (ioIDRes)
                delete ioIDRes;
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }
        int idObject = ioIDRes->getCellAsInt (0, 0);
        delete ioIDRes;
        JKKSIOTable table = refRec->getAddTable ();
        if (refRec->getSyncType() == 2)
        {
            //
            // Update для таблицы не положено
            //
            recResp.setResult (4);
            generateQueueResponse (recResp);
            qDebug () << "ERROR! " << __PRETTY_FUNCTION__ << refRec->getEntityType () << " object UID = " << ioUID << " sync type = " << refRec->getSyncType();
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
            generateQueueResponse (recResp);

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
            generateQueueResponse (recResp);
            delete res;
        }
    }
    else if (refRec->getEntityType () == 6)
    {
        return this->writeOrganization (recResp);
    }
    else if (refRec->getEntityType () == 8 || refRec->getEntityType () == 9 )
    {
        //qDebug () << __PRETTY_FUNCTION__ << refRec->getEntityType ();
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
            qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << sql;
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
            qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << sql;
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
            qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << sql;
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
        
        //qDebug () << __PRETTY_FUNCTION__ << sql;

        KKSResult * pRes = dbWrite->execute (sql);
        if (!pRes)
        {
            qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << sql;
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }

        recResp.setResult (3);
        generateQueueResponse (recResp);
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
            qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << sql;
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }
        int cpos = res->getCellAsInt (0, 0);
        delete res;
        if(cpos != 1)
            recResp.setResult (4);
        else
            recResp.setResult (3);
        
        generateQueueResponse (recResp);

    }//end of refRec->getEntityType()


    return OK_CODE;
}

void JKKSLoader :: generateQueueResponse (JKKSQueueResponse & resp) const
{
    writeReceipt (resp);
}

int JKKSLoader :: writeOrganization (JKKSQueueResponse & resp) const
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
    generateQueueResponse (resp);
    delete res;
    return OK_CODE;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readTableRecords (void) const
{
    QList<JKKSPMessWithAddr *> result;
    KKSResult * res = dbRead->execute ("select * from uQueryOutQueue () order by 2;");
    if (res && res->getRowCount() > 0 )//&& res->getColumnCount() >= 10)
    {
        for (int i=0; i<res->getRowCount(); i++)
        {
            int entity_type = res->getCellAsInt (i, 7);
            int id = res->getCellAsInt (i, 3);
            QString uid = res->getCellAsString (i, 4);
            QString addr = res->getCellAsString (i, 0);
            int sync_type = res->getCellAsInt (i, 8);
            JKKSRefRecord rec (res->getCellAsInt (i, 1), // idQueue
                               id, // idRec
                               entity_type, // entity_type
                               res->getCellAsString (i, 6), // table name
                               res->getCellAsString (i, 5), // table UID
                               sync_type // sync type
                    );
            rec.setSenderAddr (getLocalAddress());
            QString receiverUID = res->getCellAsString (i, 10);

            rec.setUid (uid);
            rec.setAddr (addr);
	    
	    qWarning() << __PRETTY_FUNCTION__ << "senderAddress = " << rec.getSenderAddr();

            if (entity_type == 1)
            {
                QMap<int, JKKSCategory> cat = readCategories (id);
                rec.setCategory (cat);
            }
            else if (entity_type == 2)
            {
                JKKSDocument doc (readDocument (id));
                doc.setCommandId (-1);//idCommand);
                doc.setJournal (-1);//idJournal);
                QDateTime sentTime = QDateTime::currentDateTime();
                doc.setRealTime (sentTime);

                rec.setIODoc (doc);
            }
            else if (entity_type == 3 || entity_type == 4)
            {
                QString tableName = res->getCellAsString (i, 6);
                int ier = readRecordFromTable (tableName, rec);
                if (ier < 0)
                    continue;
            }
            else if (entity_type == 5)
            {
                int idObject;
                JKKSIOTable table = this->readIOTable (rec.uid(), idObject);
                
                JKKSDocument doc (readDocument (idObject));
                doc.setCommandId (-1);//idCommand);
                doc.setJournal (-1);//idJournal);
                QDateTime sentTime = QDateTime::currentDateTime();
                doc.setRealTime (sentTime);

                rec.setIODoc (doc);
                rec.setAddTable (table);
            }
            else if (entity_type == 8)
            {
                JKKSOrgPackage OP = readOrgs (rec.getIDQueue(), receiverUID);
                OP.setAddr (getLocalAddress());
                JKKSPMessage pM (OP.serialize(), OP.getMessageType());
                pM.receiverUID = receiverUID;
                pM.senderUID = senderUID;
                pM.verifyReceiver = false;
                JKKSPMessWithAddr * pMessOrg = new JKKSPMessWithAddr (pM, rec.getAddr(), OP.id());
                if (pMessOrg)
                    result.append (pMessOrg);
            }
            else if (entity_type == 9)
            {
                int idOrg = rec.id();
                QMap<int, JKKSOrganization> orgs = readOrganizations (idOrg);
                QMap<int, JKKSOrganization>::const_iterator po = orgs.constBegin();
                for (; po != orgs.constEnd(); po++)
                {
                    QList<JKKSOrganization> orgList = orgs.values (po.key());
                    for (int ii=0; ii<orgList.count(); ii++)
                    {
                        JKKSOrganization org = orgList[ii];//po.value ();
                        org.setAddr (rec.getAddr());
                        JKKSPMessage pM (org.serialize(), org.getMessageType());
                        pM.receiverUID = receiverUID;//res->getCellAsString (i, 10);
                        if(entity_type == 8)
                            pM.verifyReceiver = false;
                        JKKSPMessWithAddr * pMessOrg = new JKKSPMessWithAddr (pM, rec.getAddr(), rec.getIDQueue());
                        if (pMessOrg)
                            result.append (pMessOrg);
                    }
                }
            }
            else if (entity_type == 10 || entity_type == 11) //position create or delete
            {
                QString tableName = res->getCellAsString (i, 6);
                int ier = readRecordFromTable (tableName, rec);
                if (ier < 0)
                    continue;
            }
            
            QString orgReceiverUID = res->getCellAsString(i, 10);
            
            JKKSPMessage pM(rec.serialize(), rec.getMessageType(), senderUID, orgReceiverUID);
            if(entity_type == 6 || entity_type == 8)
                pM.verifyReceiver = false;
            
            JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr (pM,
                                                                        rec.getAddr(),
                                                                        rec.getIDQueue());

            if (pMessWithAddr && (entity_type != 8 && entity_type != 9))
                result.append (pMessWithAddr);
        }
    }
    if (res)
        delete res;

    return result;
}

int JKKSLoader :: readRecordFromTable (const QString& tableName, JKKSRefRecord& rec) const
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
    int io_id = io_id_res->getCellAsInt (0, 0);
    delete io_id_res;
    QString io_sql = QString("select * from ioGetObject(%1)").arg(io_id);
    KKSResult * io_res = dbRead->execute(io_sql);
    if (!io_res || io_res->getRowCount() != 1)
    {
        if (io_res)
            delete io_res;
        return ERROR_CODE;
    }
    int idCat = io_res->getCellAsInt(0, 1);
    delete io_res;
    QMap<int, JKKSCategory> c_result = readCategories (idCat);
    if (c_result.count () != 2)
        return ERROR_CODE;
    rec.setCategory (c_result);

    if (sync_type == 1 || sync_type == 2)
    {
        QString dataSql;
        QString querySql = QString ("select * from geteio (%1, %2)").arg (rec.id()).arg (io_id);
        KKSResult * qRes = dbRead->execute (querySql);
        if (!qRes || qRes->getRowCount() != 1)
        {
            if (qRes)
                delete qRes;
            return ERROR_CODE;
        }
        dataSql = qRes->getCellAsString (0, 0);
        delete qRes;
            //QString ("select * from %1 where id=%2").arg (tableName).arg (rec.id());
        //qDebug () << __PRETTY_FUNCTION__ << dataSql;
        KKSResult *dRes = dbRead->execute (dataSql);
        if (!dRes || dRes->getRowCount() != 1)
        {
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! SQL = " << dataSql;
            if (dRes)
                delete dRes;
            return ERROR_CODE;
        }
        QStringList values;
        QMap<int, JKKSCategory>::const_iterator pc = c_result.constBegin();
        while (pc.value().getType () != 10)
            pc++;
        JKKSCategory tc = pc.value ();
        QMap<int, JKKSCategoryAttr>::const_iterator pa = tc.attributes().constBegin();
        for (int i=0; i < dRes->getColumnCount() && pa != tc.attributes().constEnd(); i++)
        {
            //qDebug () << __PRETTY_FUNCTION__ << pa.value().code () << pa.value().idAttrType () << dRes->getCellAsString (0, i) << i;
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
                    r_uid_sql = QString ("select email_prefix from %1 where id=%2").arg (pa.value().table()).arg (dRes->getCellAsString (0, i));
                else {
                    if(pa.value().idAttrType() == 2)
                        r_uid_sql = QString ("select unique_id from %1 where id=%2").arg (pa.value().table()).arg (dRes->getCellAsString (0, i));
                    else{
                        QString s = QString("select table_name from io_objects where id = %1").arg(io_id);
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
            
            if (i>0 || tableName == QString("position"))
                pa++;
        }

        //qDebug () << __PRETTY_FUNCTION__ << values.count();

        rec.setAttrsValues (values);
    }

    return OK_CODE;
}

JKKSIOTable JKKSLoader :: readIOTable (QString entityuid, int& idObject) const
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

    int idCat = res->getCellAsInt (0, 3);
    QMap<int, JKKSCategory> cats = readPCategories (idCat);
    T.setCategory (cats);

    delete res;
    return T;
}

QList<JKKSPMessWithAddr *> JKKSLoader :: readQueueResults (void) const
{
    QList<JKKSPMessWithAddr *> result;// (queueResults);
    
    //if (result.count() > 0)
        //qDebug () << __PRETTY_FUNCTION__ << result.count();

//    queueResults.clear ();
    QString sql = QString ("select * from uGetQueueResults()");
    KKSResult * res = dbRead->execute (sql);
    if (!res)
        return result;
    for (int i=0; i<res->getRowCount(); i++)
    {
        JKKSQueueResponse * resp = new JKKSQueueResponse (res->getCellAsInt (i, 1),
                                                          res->getCellAsInt (i, 3),
                                                          res->getCellAsInt (i, 4),
                                                          res->getCellAsString (i, 0));

        JKKSPMessage pM(resp->serialize(), resp->getMessageType());
        pM.verifyReceiver = false;

        qWarning() << "Response Address = " << resp->getAddr();

        JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr (pM, resp->getAddr(), resp->id());
        if (pMessWithAddr)
            result.append (pMessWithAddr);
        //this->setAsSended (res->getCellAsInt (i, 1), 6);
    }
    return result;
}

/*
QString JKKSLoader :: getLocalAddr (void) const
{
    return local_address;
}

void JKKSLoader :: setLocalAddr (const QString& localAddr)
{
    local_address = localAddr;
}
*/

int JKKSLoader :: writeMessage (JKKSQueueResponse *response) const
{
    if (!response)
        return ERROR_CODE;
    QString sql (QString("select setsyncresult (%1, %2);").arg (response->getExternalId()).arg (response->getResult()));
    qDebug () << __PRETTY_FUNCTION__ << sql;

    KKSResult * res = dbWrite->execute (sql);
    if (!res)
        return ERROR_CODE;

    int ier = res->getCellAsInt (0, 0);
    delete res;
    return ier;
}

int JKKSLoader :: writeReceipt (JKKSQueueResponse& response) const
{
    
//    qWarning () << __PRETTY_FUNCTION__ << response.id();
    
    if (response.getExternalId() < 0)
        return ERROR_CODE;

    QString sql (QString("select addQueueResult (%1, %2, '%3');").arg (response.getExternalId()).arg (response.getResult()).arg (response.getAddr()));
    KKSResult * res = dbWrite->execute (sql);
    if (!res){
        qWarning() << "ERROR! " << __PRETTY_FUNCTION__ << " Cannot execute query " << sql << " Function error";
        return ERROR_CODE;
    }

    int ier = res->getCellAsInt (0, 0);
    if(ier <= 0)
        qWarning() << "ERROR! " << __PRETTY_FUNCTION__ << " Cannot execute query " << sql << " Result is " << ier;

    response.setId(ier);
    delete res;
    return ier;
}

QMap<int, JKKSIOTable> JKKSLoader :: dependencyTables (const JKKSRefRecord& RR) const
{
    Q_UNUSED (RR);

    return QMap<int, JKKSIOTable>();
}

QPair<int, int> JKKSLoader :: getIDMap (const QString& ref_uid, const JKKSRefRecord& RR) const
{
    QPair<int, int> id_old_new(RR.id(), -1);
    QString table_sql (QString("select iogettablenamebyuid ('%1');").arg (ref_uid));
    KKSResult *tRes = dbWrite->execute (table_sql);
    if (!tRes || tRes->getRowCount() != 1)
    {
        qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << table_sql;
        if (tRes)
            delete tRes;
        return id_old_new;
    }

    QString tableName = tRes->getCellAsString (0, 0);
    delete tRes;
    if (tableName.isEmpty())
    {
        qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << table_sql;
        return id_old_new;
    }

    //
    // Сравнение структуры таблиц источника и назначения
    // (категорий)
    //
    QMap<int, JKKSCategory> tCat = RR.getCategory ();
    QMap<int, JKKSCategory>::const_iterator pc (tCat.constBegin());
    for (; pc != tCat.constEnd() && pc.value ().getType () != 10; pc++)
        ;
    JKKSCategory tc = pc.value ();
    QString attrs_uids (QString("ARRAY["));
    //
    // если таблица не является таблицей организаций, т.е. мегасистемной 
    // то в ней есть атрибут unique_id
    //
    QString attrs_vals (QString("ARRAY["));
    if (QString::compare (tableName, "organization", Qt::CaseInsensitive) != 0)
    {
        attrs_uids += QString("'unique_id',");
        attrs_vals += QString ("'%1',").arg (RR.uid());
    }

    QMap<int, JKKSCategoryAttr> attrs = tc.attributes ();
    QStringList aVals (RR.attrsValues());
    QMap<int, JKKSCategoryAttr>::const_iterator pa (attrs.constBegin());
    int nAttrsC = attrs.count ();
    int nAttrsV = aVals.count ();

    for (int i=nAttrsV-nAttrsC; pa != attrs.constEnd(); pa++)
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
            (pa.value().idAttrType() == 2 || 
             pa.value().idAttrType() == 3 || 
             pa.value().idAttrType() == 12 || 
             pa.value().idAttrType() == 17 ||
             pa.value().idAttrType() == 19 ||
             pa.value().idAttrType() == 26)
           )
        {
            value = QString ("NULL::int4");
        }
        else if (pa != attrs.constEnd() && 
                 !aVals[i].isEmpty() && 
                     (pa.value().idAttrType() == 2 || //atList
                      pa.value().idAttrType() == 3 || //atParent
                      pa.value().idAttrType() == 12 || //чекбоксы (старые)
                      pa.value().idAttrType() == 17 ||  //чекбоксы новые
                      pa.value().idAttrType() == 19 ||  //ссылка на цвет (фон)
                      pa.value().idAttrType() == 26)  //ссылка на цвет (текст)
                )
        {
/*            QString newAttrTableName;
            QString sqlTN = QString ("select * from agetattribute('%1');").arg (pa.value().code());
            KKSResult * tAttrRes = dbWrite->execute (sqlTN);
            if (!tAttrRes || tAttrRes->getRowCount() !=1)
            {
                qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << sqlTN;
                if (tAttrRes)
                    delete tAttrRes;
                return id_old_new;
            }

            newAttrTableName =  pa.value().idAttrType() != 3 ? tAttrRes->getCellAsString (0, 5) : tableName;
            if(newAttrTableName.isEmpty()){
                qWarning() << __PRETTY_FUNCTION__ << " ERROR! Cannot get real reference table for attribute CODE = " << pa.value().code();
                if (tAttrRes)
                    delete tAttrRes;
                
                return id_old_new;
            }
            
            delete tAttrRes;
            QStringList aRefValues = aVals[i].split(",");
            qDebug () << __PRETTY_FUNCTION__ << aVals[i] << aVals[i].split(",");
            QString recSql;
            if (aRefValues.count() == 1){
                if(newAttrTableName == "organization"){
                    recSql = QString ("select id from %1 where email_prefix='%2'").arg (newAttrTableName).arg (aVals[i]);
                }
                else{
                    recSql = QString ("select id from %1 where unique_id='%2'").arg (newAttrTableName).arg (aVals[i]);
                }                
            }
            else
            {
                if(newAttrTableName == "organization"){
                    recSql = QString ("select id from %1 where email_prefix in (").arg (newAttrTableName);
                }
                else{
                    recSql = QString ("select id from %1 where unique_id in (").arg (newAttrTableName);
                }
                
                for (int ii=0; ii<aRefValues.count(); ii++)
                {
                    recSql += QString ("'%1'").arg (aRefValues[ii]);
                    if (ii<aRefValues.count()-1)
                        recSql += ",";
                }
                recSql += ")";
            }

            KKSResult * refRes = dbWrite->execute (recSql);
            if (!refRes || refRes->getRowCount() != aRefValues.count())
            {
                qWarning() << __PRETTY_FUNCTION__ << " ERROR! SQL = " << recSql;
                if (refRes)
                    delete refRes;
                return id_old_new;
            }

            int idARef = refRes->getCellAsInt (0, 0);
            if (idARef > 0 && refRes->getRowCount() == 1)
                value = QString::number (idARef);
            else if (idARef > 0 && refRes->getRowCount() > 1)
            {
                value = QString ();
                for (int ii=0; ii<refRes->getRowCount(); ii++)
                {
                    value += QString::number (refRes->getCellAsInt (ii, 0));
                    if (ii<refRes->getRowCount()-1)
                        value += QString (",");
                }
            }
            else
                return id_old_new;
 */
        }

        attrs_vals += QString("'%1'%2")
                        .arg (value)//.isEmpty() ? "NULL::varchar" : value)
                        .arg (i < nAttrsV-1 ? QString (",") : QString ("]"));
        i++;
    }

    QString ts_sql (QString("select recInsert ('%1', %2, %3);").arg (ref_uid).arg (attrs_uids).arg (attrs_vals));
    qDebug () << __PRETTY_FUNCTION__ << ts_sql;

    KKSResult *tRecRes = dbWrite->execute (ts_sql);
    if (!tRecRes || tRecRes->getRowCount() != 1 || tRecRes->getCellAsInt(0, 0) <= -1)
    {
        qDebug () << __PRETTY_FUNCTION__ << "Cannot insert record! SQL = " << ts_sql;
        if (tRecRes)
            delete tRecRes;
        return id_old_new;
    }

    id_old_new.second = tRecRes->getCellAsInt (0, 0);
    delete tRecRes;

    return id_old_new;
}

int JKKSLoader :: writeMessage (JKKSOrgPackage * OrgPack, const QString& senderUID, const QString& receiverUID) const
{
    Q_UNUSED (receiverUID);
    JKKSQueueResponse recResp (-1, OrgPack->id(), 2, OrgPack->getAddr());
    qDebug () << __PRETTY_FUNCTION__ << senderUID << OrgPack->getAddr ();

    QMap<int, JKKSTransport> T = OrgPack->getTransports();
    for (QMap<int, JKKSTransport>::const_iterator pt = T.constBegin();
         pt != T.constEnd();
         pt++)
    {
        JKKSTransport TR = pt.value();
        int iert = writeTransport (TR, TR.getAddr());
        if (iert < 0)
        {
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }
    }
    QMap<int, JKKSOrganization> O = OrgPack->getOrgs();
    int norg = O.count();
    qDebug () << __PRETTY_FUNCTION__ << norg;
    for (QMap<int, JKKSOrganization>::const_iterator po = O.constBegin();
         po != O.constEnd();
         po++)
    {
        JKKSOrganization ORG = po.value();
        int iero = writeMessage (&ORG, QString());
        if (iero < 0)
        {
            recResp.setResult (4);
            generateQueueResponse (recResp);
            return ERROR_CODE;
        }
    }
    recResp.setResult (3);
    generateQueueResponse (recResp);

    return OK_CODE;
}

int JKKSLoader :: writeMessage (JKKSOrganization * org, const QString& receiverUID) const
{
    if (!org)
        return ERROR_CODE;

    JKKSOrgType ot = org->getType ();
    int idType = writeOrgType (ot);
    if (idType < 0){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! writeOrgType Error!";
        return ERROR_CODE;
    }

    org->setType (ot);
    JKKSWorkMode cMode = org->getCurrentMode ();
    int idC = writeOrgWM (cMode);
    if (idC < 0){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! writeOrgWM Error!";
        return ERROR_CODE;
    }

    org->setCurrentMode (cMode);
    JKKSWorkMode pMode = org->getPreviousMode ();
    int idP = writeOrgWM (pMode);
    if (idP < 0){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! writeOrgWM Error! 1";
        return ERROR_CODE;
    }
    org->setPreviousMode (pMode);

    QMap<int, JKKSWorkMode> wmList = org->getWorkModes();
    QMap<int, JKKSWorkMode> wmOrg;
    for (QMap<int, JKKSWorkMode>::const_iterator pw = wmList.constBegin();
         pw != wmList.constEnd();
         pw++)
    {
        JKKSWorkMode wm (pw.value());
        int idwm = writeOrgWM (wm);
        if (idwm > 0)
            wmOrg.insert (idwm, wm);
    }
    org->setWorkModes (wmOrg);

    QString uidParent (org->getParent());
    QString uidParent1 (org->getParentPart());
    QString uidSubst (org->getSubstitutor());
    int idParent = getIdByUID ("organization", uidParent);
    int idParent1 = getIdByUID ("organization", uidParent1);
    int idSubst = getIdByUID ("organization", uidSubst);
    qDebug () << __PRETTY_FUNCTION__ << uidParent << idParent;
/*
    if (idParent > 0 && records.constFind(QPair<QString,int>("organization", idParent)) != records.constEnd())
        idParent = records.constFind(QPair<QString,int>("organization", idParent)).value ();
    if (idParent1 > 0 && records.constFind(QPair<QString,int>("organization", idParent1)) != records.constEnd())
        idParent1 = records.constFind(QPair<QString,int>("organization", idParent1)).value ();
    if (idSubst > 0 && records.constFind(QPair<QString,int>("organization", idSubst)) != records.constEnd())
        idSubst = records.constFind(QPair<QString,int>("organization", idSubst)).value ();
*/
    QString orgAttrsCodes = QString ("ARRAY['localorg-attributes-1', 'localorg-attributes-83', 'localorg-attributes-27', 'localorg-attributes-84', 'localorg-attributes-85', 'localorg-attributes-75', 'localorg-attributes-86', 'localorg-attributes-76', 'localorg-attributes-2', 'localorg-attributes-19', 'localorg-attributes-77', 'localorg-attributes-87', 'localorg-attributes-88', 'localorg-attributes-89', 'localorg-attributes-91', 'localorg-attributes-92', 'localorg-attributes-90', 'localorg-attributes-78', 'localorg-attributes-82']");
    double oLong;
    double oLat;
    org->getCoords (oLong, oLat);
    QString wmStr;
    int nwm = wmOrg.count();
    if (nwm > 0)
    {
        int i=0;
        wmStr = QString ("{");
        for (QMap<int, JKKSWorkMode>::const_iterator pw = wmOrg.constBegin();
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
    
    QString orgAttrsVals = QString ("ARRAY['%1', '%2', '%3', '%4', '%5', '%6', '%7', 'null', '%8', '%9', '%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17', '%18']")
        .arg (org->id())
        .arg (idType)
        .arg (idParent <= 0 ? QString("null") : uidParent)
        .arg (idParent1 <= 0 ? QString("null") : uidParent1)
        .arg (idSubst <= 0 ? QString("null") : uidSubst)
        .arg (idC)
        .arg (idP)
        .arg (org->getName())
        .arg (org->getShortName())
        .arg (org->getCode())
        .arg (org->getEMail())
        .arg (oLat)
        .arg (oLong)
        .arg (org->getTimeCMode().toString(Qt::ISODate))
        .arg (org->getTimePMode().toString(Qt::ISODate))
        .arg (org->getMapSymbol())
        .arg (org->getTreeSymbol())
        .arg (wmStr.isEmpty() ? QString("null::int4[]"): wmStr);
    
    QString sql = QString ("select recInsert('localorg-io_objects-27', %1, %2);").arg (orgAttrsCodes).arg (orgAttrsVals);
    qDebug () << __PRETTY_FUNCTION__ << sql;

    KKSResult * oRes = dbWrite->execute (sql);
    if (!oRes || oRes->getRowCount () != 1)
    {
        if (oRes)
            delete oRes;
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! recInsert() failed! SQL = " << sql;
        return ERROR_CODE;
    }

    int idOrg = oRes->getCellAsInt (0, 0);
    org->setId (idOrg);
    delete oRes;
    if (idOrg < 0){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! recInsert() failed! SQL = " << sql;
        return ERROR_CODE;
    }

    JKKSTransport T = org->getTransport ();
    if (!receiverUID.isEmpty())
    {
        qDebug () << __PRETTY_FUNCTION__ << T.id() << T.uid() << T.getAddr() << org->id() << receiverUID;
        int idTr = writeTransport (T, T.getAddr ());
        if (idTr < 0){
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! writeTransport Error!";
            return ERROR_CODE;
        }

        org->setTransport (T);
    }

    if (!T.uid().isEmpty())
    {
        QString trOrgSql = QString ("select recInsert ('localorg-io_objects-34', ARRAY['localorg-attributes-1', 'localorg-attributes-74', 'localorg-attributes-94', 'localorg-attributes-28'], ARRAY['1','%1', '%2', '%3']);")
                                      .arg (org->getEMail())
                                      .arg (T.uid())
                                      .arg (T.getAddr());

        KKSResult * otRes = dbWrite->execute (trOrgSql);
        if (!otRes || otRes->getRowCount () <= 0)
        {
            qWarning() << __PRETTY_FUNCTION__ << "ERROR! query failed! SQL = " << trOrgSql;
            if (otRes)
                delete otRes;
            return ERROR_CODE;
        }

        delete otRes;
    }

    QString is_create_sql = QString ("update organization set is_created=1 where id=%1").arg (idOrg);
    
    KKSResult * crRes = dbWrite->execute (is_create_sql);
    if (!crRes){
        qWarning() << __PRETTY_FUNCTION__ << "ERROR! query failed! SQL = " << is_create_sql;
        return ERROR_CODE;
    }
    delete crRes;

    QPair <QString, int> idOrgMap ("organization", org->id());

    records.insert (idOrgMap, idOrg);

    return idOrg;
}

int JKKSLoader :: writeTransport (JKKSTransport& T, const QString& locAddr) const
{
    QString sql = QString ("select uInsertTransport('%1', '%2', '%3', %4);")
                    .arg (T.uid())
                    .arg (T.getTransportName ())
                    .arg (locAddr.isEmpty() ? QString("NULL") : locAddr)
                    .arg (T.isTransportActive() ? QString ("TRUE") : QString ("FALSE"));

    KKSResult * tRes = dbWrite->execute (sql);
    if (!tRes || tRes->getRowCount() != 1)
    {
        if (tRes)
            delete tRes;
        return ERROR_CODE;
    }

    int idTr = tRes->getCellAsInt (0, 0);

    T.setId (idTr);
    delete tRes;
    return idTr;
}

int JKKSLoader :: writeOrgType (JKKSOrgType& OT) const
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

    int idType = tRes->getCellAsInt (0, 0);
    OT.setIdOrgType (idType);

    delete tRes;
    return idType;
}

int JKKSLoader :: writeIOType (JKKSType& ioType) const
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

    int idType = tRes->getCellAsInt (0, 0);
    ioType.setIdType(idType);

    delete tRes;

    return idType;
}

int JKKSLoader :: writeOrgWM (JKKSWorkMode& wm) const
{
    JKKSWorkModeType wmt = wm.getType();
    int idType = writeWMType (wmt);
    if (idType < 0)
        return ERROR_CODE;
    wm.setType (wmt);
    QString sql = QString ("select recInsert ('localorg-io_objects-29', ARRAY['unique_id', 'localorg-attributes-1', 'localorg-attributes-72', 'localorg-attributes-2', 'localorg-attributes-3', 'localorg-attributes-73'], ARRAY['%5', '0', '%2', '%1', '%3', '%4']);")
            .arg (wm.getName())
            .arg (wm.getType().uid())
            .arg (wm.getDesc())
            .arg (wm.getSymbol())
            .arg (wm.uid());

    qDebug () << __PRETTY_FUNCTION__ << sql;
    KKSResult * wRes = dbWrite->execute (sql);
    if (!wRes || wRes->getRowCount() != 1)
    {
        if (wRes)
            delete wRes;
        return ERROR_CODE;
    }

    int idWM = wRes->getCellAsInt (0, 0);
    wm.setId (idWM);
    delete wRes;
    return idWM;
}

int JKKSLoader :: writeWMType (JKKSWorkModeType& wmt) const
{
    QString sql = QString ("select recInsert ('localorg-io_objects-31', ARRAY ['unique_id', 'localorg-attributes-1', 'localorg-attributes-2', 'localorg-attributes-19'], ARRAY['%3', '0', '%1', '%2'])")
            .arg (wmt.getName())
            .arg (wmt.getShortName())
            .arg (wmt.uid());
    qDebug () << __PRETTY_FUNCTION__ << sql;
    KKSResult * wRes = dbWrite->execute (sql);
    if (!wRes || wRes->getRowCount() != 1)
    {
        if (wRes)
            delete wRes;
        return ERROR_CODE;
    }

    int idWMT = wRes->getCellAsInt (0, 0);
    wmt.setId (idWMT);
    delete wRes;
    return idWMT;
}

QMap<int, JKKSOrganization> JKKSLoader :: readOrganizations (int idOrg) const
{
    QString orgSql = QString ("select * from getOrgs(%1);")
                        .arg (idOrg < 0 ? QString ("null::int4") : QString::number (idOrg));
    
    KKSResult * orgRes = dbRead->execute (orgSql);
    QMap<int, JKKSOrganization> orgs;
    if (!orgRes || orgRes->getRowCount () <= 0)
    {
        if (orgRes)
            delete orgRes;
        return orgs;
    }

    int nOrgs = orgRes->getRowCount ();
    for (int i=0; i<nOrgs; i++)
    {
        JKKSOrganization org (orgRes->getCellAsInt (i, 0), \
                              QString(), //orgRes->getCellAsInt (i, 2),
                              QString(), //orgRes->getCellAsInt (i, 3),
                              QString(), //orgRes->getCellAsInt (i, 4),
                              orgRes->getCellAsString (i, 8), \
                              orgRes->getCellAsString (i, 9), \
                              orgRes->getCellAsString (i, 10), \
                              orgRes->getCellAsString (i, 11), \
                              orgRes->getCellAsDouble (i, 12), \
                              orgRes->getCellAsDouble (i, 13), \
                              orgRes->getCellAsString (i, 14), \
                              orgRes->getCellAsString (i, 15), \
                              orgRes->getCellAsDateTime (i, 17), \
                              orgRes->getCellAsDateTime (i, 16));
        int idParent = orgRes->getCellAsInt (i, 2);
        //QString aCode = QString("id_parent");
        QString tableName =  QString ("organization");
        QList <int> ids;
        ids << idParent;
        QString uidParent = getUIDSbyIDs (ATTR_ID_PARENT, tableName, ids);
        org.setParent (uidParent);

        //aCode = QString(ATTR_ID_PARENT1);
        int idParent1 = orgRes->getCellAsInt (i, 3);
        ids.clear ();
        ids << idParent1;
        QString uidParent1 = getUIDSbyIDs (ATTR_ID_PARENT1, tableName, ids);
        org.setParentPart (uidParent1);

        //aCode = QString(ATTR_ID_SUBSTITUTOR);
        int idSubst = orgRes->getCellAsInt (i, 4);
        ids.clear ();
        ids << idSubst;
        QString uidSubst = getUIDSbyIDs (ATTR_ID_SUBSTITUTOR, tableName, ids);
        org.setSubstitutor (uidSubst);
        qDebug () << __PRETTY_FUNCTION__ << uidParent << uidParent1 << uidSubst;

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
        QMap<int, JKKSWorkMode> wmList = this->readOrgWM (org.id());
        org.setWorkModes (wmList);

        JKKSTransport T (orgRes->getCellAsInt (i, 23), // id
                         orgRes->getCellAsString (i, 24), // name
                         orgRes->getCellAsString (i, 25), // address
                         orgRes->getCellAsBool (i, 27), // active
                         orgRes->getCellAsString (i, 26)); // unique_id

        org.setTransport (T);
        if (orgs.contains (org.id()))
            orgs.insertMulti (org.id(), org);
        else
            orgs.insert (org.id(), org);
    }

    delete orgRes;
    return orgs;
}

JKKSSearchTemplate JKKSLoader :: readSearchTemplate (int idSearchTemplate) const
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
    QMap<int, JKKSSearchGroup> stGroups;
    stGroups.clear ();
    readGroups (stRes->getCellAsInt (0, 1), stGroups);
    st.setGroups (stGroups);
    return st;
}

void JKKSLoader :: readGroups (int idMainGroup, QMap<int, JKKSSearchGroup>& stGroups) const
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

    //qDebug () << __PRETTY_FUNCTION__ << idMainGroup << gRes->getRowCount();
    for (int i=0; i<gRes->getRowCount(); i++)
    {
        JKKSSearchGroup sg (gRes->getCellAsInt (i, 0), gRes->getCellAsInt (i, 1), gRes->getCellAsInt (i, 2), gRes->getCellAsBool (i, 3));
        QMap<int, JKKSSearchCriterion> crit = readCriteriaForGroup (sg.id());
        sg.setCriteria (crit);
        stGroups.insert (sg.id(), sg);
        if (i>0)
            readGroups (sg.id(), stGroups);
    }
    delete gRes;
}

QMap<int, JKKSSearchCriterion> JKKSLoader :: readCriteriaForGroup (int idGroup) const
{
    if (idGroup <= 0)
        return QMap<int, JKKSSearchCriterion> ();

    QString sql = QString ("select * from ioGetCriterionFromGroup (%1);").arg (idGroup);
    KKSResult * cRes = dbRead->execute (sql);

    if (!cRes)
        return QMap<int, JKKSSearchCriterion> ();

    QMap<int, JKKSSearchCriterion> cResults;
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

int JKKSLoader :: writeSearchTemplate (const JKKSSearchTemplate& st) const
{
    QMap<int, JKKSSearchGroup> gr = st.getGroups ();
    QMap<int, JKKSSearchGroup> wGroups (writeGroups (-1, gr));
    QMap<int, JKKSSearchGroup>::const_iterator pg = wGroups.constBegin();
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

    if (!res)
        return ERROR_CODE;

    int id = res->getCellAsInt (0, 0);
    delete res;
    return id;
}

QMap<int, JKKSSearchGroup> JKKSLoader :: writeGroups (int idParentGr, const QMap<int, JKKSSearchGroup>& stGroups) const
{
    int idMainGr = -1;
    for (QMap<int, JKKSSearchGroup>::const_iterator pg = stGroups.constBegin(); \
         pg != stGroups.constEnd() && idMainGr < 0; \
         pg++)
    {
        qDebug () << __PRETTY_FUNCTION__ << idMainGr << idParentGr << pg.key() << pg.value().idParent();
        if ((idParentGr > 0 && pg.value().idParent() == idParentGr) ||
            (idParentGr < 0 && pg.value().idParent() <= 0))
            idMainGr = pg.key();
    }
    
    //qDebug () << __PRETTY_FUNCTION__ << idMainGr << idParentGr;

    if (idMainGr < 0)
        return QMap<int, JKKSSearchGroup>();
    
    QMap<int, JKKSSearchGroup> gRes;
    QMap<int, JKKSSearchGroup>::const_iterator pg = stGroups.constFind (idMainGr);
    JKKSSearchGroup g = pg.value();
    QString sql = QString("select ioInsertSearchGroup (%1, %2, %3);")
                        .arg (idParentGr < 0 ? QString("NULL::int4") : QString::number (idParentGr))
                        .arg (pg.value().getOper())
                        .arg (pg.value().isNegative() ? QString ("TRUE") : QString ("FALSE"));
    KKSResult * res = dbWrite->execute (sql);
    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        return gRes;
    }

    int idGroup = res->getCellAsInt (0, 0);
    delete res;

    if (idGroup < 0)
        return gRes;

    g.setId (idGroup);
    gRes.insert (idGroup, g);
    QMap<int, JKKSSearchGroup> subGroups = writeGroups (idMainGr, stGroups);
    for (pg = subGroups.constBegin(); pg != subGroups.constEnd(); pg++)
        gRes.insert (pg.key(), pg.value());

    for (QMap<int, JKKSSearchCriterion>::const_iterator pc = g.getCriteria().constBegin(); \
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
            if (gsres)
                delete gsres;
            continue;
        }

        delete gsres;
    }
    return gRes;
}

int JKKSLoader :: writeCriteriaForGroup (JKKSSearchCriterion& sc) const
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
        if (res)
            delete res;

        return ERROR_CODE;
    }
    int ok = res->getCellAsInt (0, 0);
    delete res;
    sc.setId (ok);

    return ok;
}

JKKSOrgPackage JKKSLoader :: readOrgs (int id, const QString& receiverUID) const
{
    JKKSOrgPackage OP(id);
    QMap<int, JKKSTransport> TS = readTransports (receiverUID);

    OP.setTransports (TS);
    QMap<int, JKKSOrganization> orgs = readOrganizations (-1);
    OP.setOrgs (orgs);
    return OP;
}

QMap<int, JKKSTransport> JKKSLoader :: readTransports (const QString& receiverUID) const
{
    QMap<int, JKKSTransport> TResult;
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
        JKKSTransport T (res->getCellAsInt (i, 1), // id
                        res->getCellAsString (i, 2), // name
                        res->getCellAsString (i, 3), // address
                        res->getCellAsBool (i, 4), // active
                        res->getCellAsString (i, 0) // unique_id
                       );
        TResult.insert (T.id(), T);
    }
    delete res;
    return TResult;
}

QString JKKSLoader :: getUIDSbyIDs (int attrId, const QString& tableName, const QList<int>& ids) const
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

QMap<int, JKKSWorkMode> JKKSLoader :: readOrgWM (int idOrg) const
{
    QString sql = QString ("select * from getOrgsWorkModes (%1)").arg (idOrg);
    KKSResult * res = dbRead->execute (sql);

    if (!res || res->getRowCount() == 0)
    {
        if (res)
            delete res;

        return QMap<int, JKKSWorkMode>();
    }

    QMap<int, JKKSWorkMode> wmList;
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

JKKSWorkMode JKKSLoader :: readWM (int idWM) const
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