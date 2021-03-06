#include <QDataStream>
#include "JKKSTransport.h"

JKKSTransport :: JKKSTransport (qint64 id_transport, 
                                const QString& trName, 
                                const JKKSAddress & lAddr, 
                                bool isActive, 
                                const QString& uid)
    : JKKSUID (uid, QString()),
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

qint64 JKKSTransport :: id (void) const
{
    return idTransport;
}

void JKKSTransport :: setId (qint64 id)
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

const JKKSAddress & JKKSTransport :: getAddress() const
{
    return localAddr;
}

void JKKSTransport :: setAddress (const JKKSAddress & addr)
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
