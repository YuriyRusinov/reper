#include <QBuffer>
#include <QDataStream>
#include <QtDebug>

#include "defines.h"
#include "JKKSLoader.h"
#include "JKKSRefRecord.h"

JKKSRefRecord :: JKKSRefRecord (qint64 id_queue, 
                                qint64 id_rec, 
                                int id_entity_type, 
                                const QString& tName, 
                                int sync_type, 
                                const JKKSCategory& aCat, 
                                const QStringList& attrsVals, 

                                const JKKSAddress & addr, 
                                const QString& mess_code, 

                                const QString & uid,
                                const QString & t_uid, 
                                const QString & uuid,
                                qint64 idState, 
                                const QColor bkCol,
                                const QColor fgCol,
                                const QString & rIcon)
    : JKKSMessage (addr, mess_code),
      JKKSUID (uid, uuid, idState, bkCol, fgCol, rIcon),
      idQueue (id_queue),
      idRec (id_rec),
      idEntityType (id_entity_type),
      tableName (tName),
      tableUID (t_uid),
      syncType (sync_type),
      aVals (attrsVals),
      ioDoc (JKKSDocument()),
      ioTable (JKKSIOTable()),
      senderAddr (JKKSAddress())
{
    cat.clear ();
    cat.insert (aCat.id(), aCat);
}

JKKSRefRecord :: JKKSRefRecord (const JKKSRefRecord & RR)
    : JKKSMessage (RR),
      JKKSUID (RR),
      idQueue (RR.idQueue),
      idRec (RR.idRec),
      idEntityType (RR.idEntityType),
      tableName (RR.tableName),
      tableUID (RR.tableUID),
      syncType (RR.syncType),
      cat (RR.cat),
      aVals (RR.aVals),
      ioDoc (RR.ioDoc),
      ioTable (RR.ioTable),
      senderAddr (RR.senderAddr)
{
}

JKKSRefRecord :: ~JKKSRefRecord (void)
{
}

QByteArray JKKSRefRecord :: serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    QByteArray ba;
    out << (*this);
    ba = qBuffer.buffer();

    return ba;
}

int JKKSRefRecord :: unserialize (const QByteArray& mess)
{
    QBuffer qBuffer;
    qBuffer.setData (mess);
    qBuffer.open (QIODevice::ReadOnly);
    QDataStream in (&qBuffer);
    in >> (*this);

    return OK_CODE;
}

int JKKSRefRecord :: writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    //qDebug() << __PRETTY_FUNCTION__;
    Q_UNUSED (receiverUID);
    int ier = loader->writeMessage (this, senderUID);
    return ier;
}

qint64 JKKSRefRecord :: id (void) const
{
    return idRec;
}

JKKSMessage::JKKSMessageType JKKSRefRecord :: getMessageType (void) const
{
    return JKKSMessage::atRecord;
}

void JKKSRefRecord :: setId (qint64 id)
{
    idRec = id;
}

qint64 JKKSRefRecord :: getIDQueue (void) const
{
    return idQueue;
}

void JKKSRefRecord :: setIDQueue (qint64 idq)
{
    idQueue = idq;
}

int JKKSRefRecord :: getEntityType (void) const
{
    return idEntityType;
}

void JKKSRefRecord :: setEntityType (int id_entity_type)
{
    idEntityType = id_entity_type;
}

QString JKKSRefRecord :: getTableName (void) const
{
    return tableName;
}

void JKKSRefRecord :: setTableName (const QString& tName)
{
    tableName = tName;
}

QString JKKSRefRecord :: getTableUID (void) const
{
    return tableUID;
}

void JKKSRefRecord :: setTableUID (const QString& t_uid)
{
    tableUID = t_uid;
}

int JKKSRefRecord :: getSyncType (void) const
{
    return syncType;
}

void JKKSRefRecord :: setSyncType (int sync_type)
{
    syncType = sync_type;
}

const QMap<qint64, JKKSCategory>& JKKSRefRecord :: getCategory (void) const
{
    return cat;
}

void JKKSRefRecord :: setCategory (const QMap<qint64, JKKSCategory>& aCats)
{
    cat = aCats;
}

const QStringList& JKKSRefRecord :: attrsValues (void) const
{
    return aVals;
}

void JKKSRefRecord :: setAttrsValues (const QStringList& attrs_vals)
{
    aVals = attrs_vals;
}

const QMap<QString, QString> & JKKSRefRecord :: indValues (void) const
{
    return indVals;
}

void JKKSRefRecord :: setIndValues (const QMap<QString, QString> & ind_vals)
{
    indVals = ind_vals;
}

const JKKSDocument& JKKSRefRecord :: getIODoc (void) const
{
    return ioDoc;
}

void JKKSRefRecord :: setIODoc (const JKKSDocument& doc)
{
    ioDoc = doc;
}

const JKKSIOTable& JKKSRefRecord :: getAddTable (void) const
{
    return ioTable;
}

void JKKSRefRecord :: setAddTable (const JKKSIOTable& table)
{
    ioTable = table;
}

const JKKSAddress & JKKSRefRecord :: getSenderAddr (void) const
{
    return senderAddr;
}

void JKKSRefRecord :: setSenderAddr (const JKKSAddress & addr)
{
    senderAddr = addr;
}

QDataStream& operator<< (QDataStream& out, const JKKSRefRecord& RR)
{
    out << RR.getAddr();
    out << RR.getCode();

    out << RR.idQueue;
    out << RR.idRec;
    out << RR.idEntityType;
    out << RR.tableName;
    out << RR.tableUID;
    out << RR.syncType;
    out << RR.cat;
    out << RR.aVals;
    out << RR.ioDoc.serialize ();
    out << RR.ioTable;
    out << RR.senderAddr;

    out << RR.uid();
    out << RR.uuid();
    out << RR.idState();
    out << RR.bgColor();
    out << RR.fgColor();
    out << RR.rIcon();

    out << RR.indValues();

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSRefRecord& RR)
{
    JKKSAddress addr;
    QString code;

    in >> addr;
    in >> code;
    RR.setAddr (addr);// = JKKSRefRecord (avals, uid);
    RR.setCode (code);

    in >> RR.idQueue;
    in >> RR.idRec;
    in >> RR.idEntityType;
    in >> RR.tableName;
    in >> RR.tableUID;
    in >> RR.syncType;
    in >> RR.cat;
    in >> RR.aVals;

    QByteArray docArr;
    in >> docArr;
    RR.ioDoc.unserialize (docArr);

    in >> RR.ioTable;
    in >> RR.senderAddr;

    //from jkksuid
    QString uid;
    QString uuid;
    qint64 idState;
    QColor fColor;
    QColor bColor;
    QString rIc;

    in >> uid;
    in >> uuid;
    in >> idState;
    in >> fColor;
    in >> bColor;
    in >> rIc;

    RR.setUid (uid);
    RR.setUuid(uuid);
    RR.setIdState(idState);
    RR.setBgColor(bColor);
    RR.setFgColor(fColor);
    RR.setRIcon(rIc);

    QMap<QString, QString> iV;
    in >> iV;
    RR.setIndValues(iV);


    return in;
}

JKKSQueueResponse :: JKKSQueueResponse (qint64 local_id, 
                                        qint64 id_queue, 
                                        int sync_result, 
                                        const JKKSAddress & addr, 
                                        const QString& type)
    : JKKSMessage (addr, type),
    localId (local_id),
    idQueue (id_queue),
    syncResult (sync_result)
{
}

JKKSQueueResponse :: JKKSQueueResponse (const JKKSQueueResponse& QR)
    : JKKSMessage (QR),
    localId (QR.localId),
    idQueue (QR.idQueue),
    syncResult (QR.syncResult),
    m_orgUid(QR.m_orgUid)
{
}

JKKSQueueResponse :: ~JKKSQueueResponse (void)
{
}

QByteArray JKKSQueueResponse :: serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    QByteArray ba;
    out << (*this);
    ba = qBuffer.buffer();

    return ba;
}

int JKKSQueueResponse :: unserialize (const QByteArray& mess)
{
    QBuffer qBuffer;
    qBuffer.setData (mess);
    qBuffer.open (QIODevice::ReadOnly);
    QDataStream in (&qBuffer);
    in >> (*this);

    return OK_CODE;
}

int JKKSQueueResponse :: writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    Q_UNUSED (senderUID);
    Q_UNUSED (receiverUID);
    int ier = loader->writeMessage (this);
    return ier;
}

qint64 JKKSQueueResponse :: id (void) const
{
    return localId;//idQueue;
}

JKKSMessage::JKKSMessageType JKKSQueueResponse :: getMessageType (void) const
{
    return JKKSMessage::atRecConfirmation;
}

void JKKSQueueResponse :: setId (qint64 id)
{
    localId = id;//idQueue = id;
}

int JKKSQueueResponse :: getResult (void) const
{
    return syncResult;
}

void JKKSQueueResponse :: setResult (int sRes)
{
    syncResult = sRes;
}

qint64 JKKSQueueResponse :: getExternalId (void) const
{
    return idQueue;
}

void JKKSQueueResponse :: setExternalId (qint64 idq)
{
    idQueue = idq;
}

void JKKSQueueResponse :: setOrgUid(const QString & orgUid)
{
    m_orgUid = orgUid;
}

const QString & JKKSQueueResponse :: orgUid() const
{
    return m_orgUid;
}


QDataStream& operator<< (QDataStream& out, const JKKSQueueResponse& RR)
{
    out << RR.getAddr();
    out << RR.getCode();

    out << RR.localId;
    out << RR.idQueue;
    out << RR.syncResult;

    out << RR.m_orgUid;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSQueueResponse& RR)
{
    JKKSAddress addr;
    QString code;

    in >> addr;
    in >> code;

    in >> RR.localId;
    in >> RR.idQueue;
    in >> RR.syncResult;
    in >> RR.m_orgUid;

    RR.setAddr (addr);// = JKKSRefRecord (avals, uid);
    RR.setCode (code);

    return in;
}
