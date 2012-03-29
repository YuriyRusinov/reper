#include <QDataStream>
#include <QBuffer>
#include <QtDebug>

#include "JKKSLoader.h"
#include "JKKSOrgPackage.h"
#include "defines.h"

JKKSOrgPackage :: JKKSOrgPackage (int id, const QString& addr, const QString& code, const QString& uid)
    : JKKSMessage (addr, code),
      JKKSUID (uid),
      m_id (id)
{
}

JKKSOrgPackage :: JKKSOrgPackage (const JKKSOrgPackage& OP)
    : JKKSMessage (OP),
      JKKSUID (OP),
      m_id (OP.m_id),
      Transports (OP.Transports),
      Orgs (OP.Orgs)
{
}

QByteArray JKKSOrgPackage :: serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    QByteArray ba;
    out << (*this);
    ba = qBuffer.buffer();

    return ba;
}

int JKKSOrgPackage :: unserialize (const QByteArray& mess)
{
    QBuffer qBuffer;
    qBuffer.setData (mess);
    qBuffer.open (QIODevice::ReadOnly);
    QDataStream in (&qBuffer);
    in >> (*this);

    return OK_CODE;
}

int JKKSOrgPackage :: id (void) const
{
    return m_id;
}

void JKKSOrgPackage :: setId (int id)
{
    m_id = id;
}

int JKKSOrgPackage :: writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    int ier = loader->writeMessage (this, senderUID, receiverUID);
    return ier;
}

JKKSMessage::JKKSMessageType JKKSOrgPackage :: getMessageType (void) const
{
    return JKKSMessage::atOrgPackage;
}

const QMap<int, JKKSTransport>& JKKSOrgPackage :: getTransports (void) const
{
    return Transports;
}

void JKKSOrgPackage :: setTransports (const QMap<int, JKKSTransport>& T)
{
    Transports = T;
}

const QMap<int, JKKSOrganization>& JKKSOrgPackage :: getOrgs (void) const
{
    return Orgs;
}

void JKKSOrgPackage :: setOrgs (const QMap<int, JKKSOrganization>& orgs)
{
    Orgs = orgs;
}

QDataStream& operator<< (QDataStream& out, const JKKSOrgPackage& OP)
{
    out << OP.getAddr ();
    out << OP.getCode ();

    out << OP.m_id;
    out << OP.Transports;
    out << OP.Orgs;

    out << OP.uid ();
    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSOrgPackage& OP)
{
    QString addr;
    QString code;

    in >> addr;
    in >> code;

    in >> OP.m_id;
    in >> OP.Transports;
    in >> OP.Orgs;

    QString uid;
    in >> uid;
    OP.setUid (uid);
    OP.setAddr (addr);// = JKKSRefRecord (avals, uid);
    OP.setCode (code);
    return in;
}