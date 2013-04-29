#include <QBuffer>
#include <QDataStream>
#include <QtDebug>

#include "JKKSMailMessage.h"
#include "JKKSLoader.h"
#include "defines.h"

JKKSMailMessage :: JKKSMailMessage (int idMess,
                                    int idObject, 
                                    const QString& messBody,
                                    const QString& u_idDlReceiver,
                                    const QString& u_idDlSender,
                                    const QDateTime& dt,
                                    const JKKSAddress & addr,
                                    const QString& mess_code,
                                    const QString& uid,
                                    const QString& inputNumber,
                                    const QString& outputNumber,
                                    int idUL)
    : JKKSMessage (addr, mess_code), JKKSUID(uid),
    idMessage (idMess),
    idIO (idObject),
    messageBody (messBody),
    u_idDlTo (u_idDlReceiver),
    u_idDlFrom (u_idDlSender),
    sentDateTime (dt),
    m_inputNumber(inputNumber),
    m_outputNumber(outputNumber),
    m_idUrgencyLevel(idUL),
    ioDoc (JKKSDocument())
{
}

JKKSMailMessage :: JKKSMailMessage (const JKKSMailMessage& mess)
    : JKKSMessage (mess), JKKSUID(mess),
    idMessage (mess.idMessage),
    idIO (mess.idIO),
    messageBody (mess.messageBody),
    u_idDlTo (mess.u_idDlTo),
    u_idDlFrom (mess.u_idDlFrom),
    sentDateTime (mess.sentDateTime),
    m_inputNumber(mess.inputNumber()),
    m_outputNumber(mess.outputNumber()),
    m_idUrgencyLevel(mess.idUrgencyLevel()),
    ioDoc (mess.ioDoc)
{
}

JKKSMailMessage :: ~JKKSMailMessage (void)
{
}

QByteArray JKKSMailMessage :: serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    out << idMessage;
    out << getAddr();
    out << getCode();
    out << idIO;
    out << messageBody;
    out << u_idDlTo;
    out << u_idDlFrom;
    out << sentDateTime;
    out << uid();
    out << m_inputNumber;
    out << m_outputNumber;
    out << m_idUrgencyLevel;

    if (idIO > 0)
    {
        QByteArray docArr = ioDoc.serialize ();
        out << docArr;
    }


    return qBuffer.buffer();
}

int JKKSMailMessage :: unserialize (const QByteArray& mess)
{
    QBuffer buffer;
    buffer.setData(mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    JKKSAddress addr;
    QString code;
    in >> idMessage;
    in >> addr;
    in >> code;
    in >> idIO;
    in >> messageBody;
    in >> u_idDlTo;
    in >> u_idDlFrom;
    in >> sentDateTime;
    QString uid;
    in >> uid;
    setUid (uid);
    in >> m_inputNumber;
    in >> m_outputNumber;
    in >> m_idUrgencyLevel;

    if (idIO > 0)
    {
        QByteArray docArr;
        in >> docArr;
        ioDoc.unserialize (docArr);
    }

    setAddr (addr);
    setCode (code);

    return OK_CODE;
}

int JKKSMailMessage :: writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    Q_UNUSED (senderUID);
    Q_UNUSED (receiverUID);
    return loader->writeMessage (this);
}

int JKKSMailMessage :: id (void) const
{
    return idMessage;
}

JKKSMessage::JKKSMessageType JKKSMailMessage :: getMessageType (void) const
{
    return JKKSMessage::atMailMessage;
}

void JKKSMailMessage :: setId (int id)
{
    idMessage = id;
}

int JKKSMailMessage :: getIO (void) const
{
    return idIO;
}

void JKKSMailMessage :: setIO (int idObject)
{
    idIO = idObject;
}

const QString& JKKSMailMessage :: getMessageBody (void) const
{
    return messageBody;
}

void JKKSMailMessage :: setMessageBody (const QString& mess)
{
    messageBody = mess;
}

QString JKKSMailMessage :: getIdDlTo (void) const
{
    return u_idDlTo;
}

void JKKSMailMessage :: setIdDlTo (const QString& u_idTo)
{
    u_idDlTo = u_idTo;
}

QString JKKSMailMessage :: getIdDlFrom (void) const
{
    return u_idDlFrom;
}

void JKKSMailMessage :: setIdDlFrom (const QString& u_idFrom)
{
    u_idDlFrom = u_idFrom;
}

const QDateTime& JKKSMailMessage :: getSentDateTime (void) const
{
    return sentDateTime;
}

void JKKSMailMessage :: setSentDateTime (const QDateTime& dt)
{
    sentDateTime = dt;
}

const JKKSDocument& JKKSMailMessage :: getAttachedDoc (void) const
{
    return ioDoc;
}

void JKKSMailMessage :: setAttachment (const JKKSDocument& doc)
{
    ioDoc = doc;
}



/***************************/

JKKSMailConfirmation::JKKSMailConfirmation(int idMess, 
                                           int extraId,
                                           const QDateTime & readDatetime,
                                           const QDateTime & receiveDatetime,
                                           const JKKSAddress & addr,
                                           const QString & kvs)
: JKKSMessage (addr, kvs),
m_id(idMess),
m_dst_id(extraId),
m_readDatetime(readDatetime),
m_receiveDatetime(receiveDatetime)
{
}

JKKSMailConfirmation::JKKSMailConfirmation(const JKKSMailConfirmation & cfm)
: JKKSMessage (cfm),
m_id(cfm.m_id),
m_dst_id(cfm.m_dst_id),
m_readDatetime(cfm.m_readDatetime),
m_receiveDatetime(cfm.m_receiveDatetime)
{

}

JKKSMailConfirmation::~JKKSMailConfirmation()
{

}

QByteArray JKKSMailConfirmation::serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    out << getAddr();
    out << getCode();
    out << id();
    out << extraId();
    out << readDatetime();
    out << receiveDatetime();

    return qBuffer.buffer();
}

int JKKSMailConfirmation::unserialize (const QByteArray& mess)
{
    QBuffer buffer;
    buffer.setData(mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    JKKSAddress addr;
    QString code;
    in >> addr;
    in >> code;
    in >> m_id;
    in >> m_dst_id;
    in >> m_readDatetime;
    in >> m_receiveDatetime;

    setAddr (addr);
    setCode (code);

    return OK_CODE;
}

int JKKSMailConfirmation::writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    Q_UNUSED (senderUID);
    Q_UNUSED (receiverUID);
    return loader->writeMessage (this);
}

JKKSMessage::JKKSMessageType JKKSMailConfirmation::getMessageType (void) const
{
    return JKKSMessage::atMailConfirmation;
}

int JKKSMailConfirmation::id (void) const
{
    return m_id;
}

void JKKSMailConfirmation::setId (int the_id)
{
    m_id = the_id;
}

int JKKSMailConfirmation::extraId (void) const
{
    return m_dst_id;
}

void JKKSMailConfirmation::setExtraId (int the_id)
{
    m_dst_id = the_id;
}

const QDateTime & JKKSMailConfirmation::readDatetime() const
{
    return m_readDatetime;
}

void JKKSMailConfirmation::setReadDatetime(const QDateTime & dt)
{
    m_readDatetime = dt;
}

const QDateTime & JKKSMailConfirmation::receiveDatetime() const
{
    return m_receiveDatetime;
}

void JKKSMailConfirmation::setReceiveDatetime(const QDateTime & dt)
{
    m_receiveDatetime = dt;
}
