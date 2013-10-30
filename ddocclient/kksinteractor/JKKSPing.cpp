#include <QBuffer>
#include <QDataStream>
#include <QtDebug>

#include "JKKSPing.h"
#include "JKKSLoader.h"
#include "defines.h"


JKKSPing:: JKKSPing (qint64 id, const JKKSAddress & addr, const QString & kvs)
    : JKKSMessage (addr, kvs), 
    m_id(id),
    m_idOrgFrom(-1),
    m_idOrgTo(-1),
    m_state1(0),
    m_state2(0),
    m_state3(0),
    m_state4(0),
    m_created(0),
    m_completed(false),
    m_versionFrom(QString()),
    m_versionTo(QString()),
    m_uidFrom(QString()),
    m_uidTo(QString()),
    m_nameFrom(QString()),
    m_nameTo(QString())
{
}

JKKSPing :: JKKSPing(const JKKSPing & ping)
    : JKKSMessage (ping), 
    m_id(ping.m_id),
    m_idOrgFrom(ping.m_idOrgFrom),
    m_idOrgTo(ping.m_idOrgTo),
    m_state1(ping.m_state1),
    m_state2(ping.m_state2),
    m_state3(ping.m_state3),
    m_state4(ping.m_state4),
    m_created(ping.m_created),
    m_completed(ping.m_completed),
    m_versionFrom(ping.m_versionFrom),
    m_versionTo(ping.m_versionTo),
    m_uidFrom(ping.m_uidFrom),
    m_uidTo(ping.m_uidTo),
    m_nameFrom(ping.m_nameFrom),
    m_nameTo(ping.m_nameTo)
{
}

JKKSPing :: ~JKKSPing (void)
{
}

QByteArray JKKSPing::serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    out << getAddr();
    out << getCode();
    out << m_id;

    out << m_idOrgFrom;
    out << m_idOrgTo;

    out << m_state1;
    out << m_state2;
    out << m_state3;
    out << m_state4;

    out << m_created;
    out << m_completed;

    out << m_versionFrom;
    out << m_versionTo;

    out << m_uidFrom;
    out << m_uidTo;

    out << m_nameFrom;
    out << m_nameTo;

    return qBuffer.buffer();
}

int JKKSPing::unserialize (const QByteArray& mess)
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

    in >> m_idOrgFrom;
    in >> m_idOrgTo;

    in >> m_state1;
    in >> m_state2;
    in >> m_state3;
    in >> m_state4;

    in >> m_created;
    in >> m_completed;

    in >> m_versionFrom;
    in >> m_versionTo;

    in >> m_uidFrom;
    in >> m_uidTo;

    in >> m_nameFrom;
    in >> m_nameTo;

    setAddr (addr);
    setCode (code);

    return OK_CODE;
}


int JKKSPing::writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    Q_UNUSED (receiverUID);
    return loader->writeMessage (this, senderUID);
}

JKKSMessage::JKKSMessageType JKKSPing::getMessageType (void) const
{
    return JKKSMessage::atPing;
}

qint64 JKKSPing::id (void) const
{
    return m_id;
}

void JKKSPing::setId (qint64 the_id)
{
    m_id = the_id;
}

qint64 JKKSPing::idOrgTo() const
{
    return m_idOrgTo;
}

void JKKSPing::setIdOrgTo(qint64 id)
{
    m_idOrgTo = id;
}

qint64 JKKSPing::idOrgFrom() const
{
    return m_idOrgFrom;
}

void JKKSPing::setIdOrgFrom(qint64 id)
{
    m_idOrgFrom = id;
}

int JKKSPing::state1() const
{
    return m_state1;
}

int JKKSPing::state2() const
{
    return m_state2;
}

int JKKSPing::state3() const
{
    return m_state3;
}

int JKKSPing::state4() const
{
    return m_state4;
}

void JKKSPing::setState1(int s)
{
    m_state1 = s;
}

void JKKSPing::setState2(int s)
{
    m_state2 = s;
}

void JKKSPing::setState3(int s)
{
    m_state3 = s;
}

void JKKSPing::setState4(int s)
{
    m_state4 = s;
}

int JKKSPing::created() const
{
    return m_created;
}

bool JKKSPing::completed() const
{
    return m_completed;
}

void JKKSPing::setCreated(int c)
{
    m_created = c;
}

void JKKSPing::setCompleted(bool c)
{
    m_completed = c;
}

const QString & JKKSPing::uidFrom() const
{
    return m_uidFrom;
}

const QString & JKKSPing::uidTo() const
{
    return m_uidTo;
}

void JKKSPing::setUidFrom(const QString & u)
{
    m_uidFrom = u;
}

void JKKSPing::setUidTo(const QString & u)
{
    m_uidTo = u;
}

const QString & JKKSPing::nameFrom() const
{
    return m_nameFrom;
}

const QString & JKKSPing::nameTo() const
{
    return m_nameTo;
}

void JKKSPing::setNameFrom(const QString & n)
{
    m_nameFrom = n;
}

void JKKSPing::setNameTo(const QString & n)
{
    m_nameTo = n;
}
