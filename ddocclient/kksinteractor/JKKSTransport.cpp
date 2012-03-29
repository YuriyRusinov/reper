#include <QDataStream>
#include "JKKSTransport.h"

JKKSTransport :: JKKSTransport (int id_transport, const QString& trName, const QString& lAddr, bool isActive, const QString& uid)
    : JKKSUID (uid),
    idTransport (id_transport),
    transportName (trName),
    localAddr (lAddr),
    m_isActive (isActive)
{
}

JKKSTransport :: JKKSTransport (const JKKSTransport& T)
    : JKKSUID (T),
    idTransport (T.idTransport),
    transportName (T.transportName),
    localAddr (T.localAddr),
    m_isActive (T.m_isActive)
{
}

JKKSTransport :: ~JKKSTransport (void)
{
}

int JKKSTransport :: id (void) const
{
    return idTransport;
}

void JKKSTransport :: setId (int id)
{
    idTransport = id;
}

QString JKKSTransport :: getTransportName (void) const
{
    return transportName;
}

void JKKSTransport :: setTransportName (const QString& name)
{
    transportName = name;
}

QString JKKSTransport :: getAddr (void) const
{
    return localAddr;
}

void JKKSTransport :: setAddr (const QString& addr)
{
    localAddr = addr;
}

bool JKKSTransport :: isTransportActive (void) const
{
    return m_isActive;
}

void JKKSTransport :: setTransportActive (bool isActive)
{
    m_isActive = isActive;
}

QDataStream& operator<< (QDataStream& out, const JKKSTransport& T)
{
    out << T.idTransport;
    out << T.transportName;
    out << T.localAddr;
    out << T.m_isActive;
    out << T.uid();
    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSTransport& T)
{
    in >> T.idTransport;
    in >> T.transportName;
    in >> T.localAddr;
    in >> T.m_isActive;
    QString uid;
    in >> uid;
    T.setUid (uid);
    return in;
}
