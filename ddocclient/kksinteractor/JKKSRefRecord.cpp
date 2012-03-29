#include <QBuffer>
#include <QDataStream>
#include <QtDebug>

#include "defines.h"
#include "JKKSLoader.h"
#include "JKKSRefRecord.h"

JKKSRefRecord :: JKKSRefRecord (int id_queue, int id_rec, int id_entity_type, const QString& tName, const QString& t_uid, int sync_type, const JKKSCategory& aCat, const QStringList& attrsVals, const QString& addr, const QString& mess_code, const QString& uid)
    : JKKSMessage (addr, mess_code),
      JKKSUID (uid),
      idQueue (id_queue),
      idRec (id_rec),
      idEntityType (id_entity_type),
      tableName (tName),
      tableUID (t_uid),
      syncType (sync_type),
      aVals (attrsVals),
      ioDoc (JKKSDocument()),
      ioTable (JKKSIOTable()),
      senderAddr (QString())
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

int JKKSRefRecord :: id (void) const
{
    return idRec;
}

JKKSMessage::JKKSMessageType JKKSRefRecord :: getMessageType (void) const
{
    return JKKSMessage::atRecord;
}

void JKKSRefRecord :: setId (int id)
{
    idRec = id;
}

int JKKSRefRecord :: getIDQueue (void) const
{
    return idQueue;
}

void JKKSRefRecord :: setIDQueue (int idq)
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

const QMap<int, JKKSCategory>& JKKSRefRecord :: getCategory (void) const
{
    return cat;
}

void JKKSRefRecord :: setCategory (const QMap<int, JKKSCategory>& aCats)
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

QString JKKSRefRecord :: getSenderAddr (void) const
{
    return senderAddr;
}

void JKKSRefRecord :: setSenderAddr (QString addr)
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

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSRefRecord& RR)
{
    QString addr;
    QString code;

    in >> addr;
    in >> code;

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
    QString uid;
    in >> uid;
    RR.setUid (uid);

    RR.setAddr (addr);// = JKKSRefRecord (avals, uid);
    RR.setCode (code);

    return in;
}

JKKSQueueResponse :: JKKSQueueResponse (int local_id, int id_queue, int sync_result, const QString& addr, const QString& type)
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
    syncResult (QR.syncResult)
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

int JKKSQueueResponse :: id (void) const
{
    return localId;//idQueue;
}

JKKSMessage::JKKSMessageType JKKSQueueResponse :: getMessageType (void) const
{
    return JKKSMessage::atRecConfirmation;
}

void JKKSQueueResponse :: setId (int id)
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
int JKKSQueueResponse :: getExternalId (void) const
{
    return idQueue;
}

void JKKSQueueResponse :: setExternalId (int idq)
{
    idQueue = idq;
}

QDataStream& operator<< (QDataStream& out, const JKKSQueueResponse& RR)
{
    out << RR.getAddr();
    out << RR.getCode();

    out << RR.localId;
    out << RR.idQueue;
    out << RR.syncResult;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSQueueResponse& RR)
{
    QString addr;
    QString code;

    in >> addr;
    in >> code;

    in >> RR.localId;
    in >> RR.idQueue;
    in >> RR.syncResult;

    RR.setAddr (addr);// = JKKSRefRecord (avals, uid);
    RR.setCode (code);

    return in;
}
