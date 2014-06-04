#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QBuffer>
#include <QtDebug>

#include "JKKSMessage.h"

JKKSAddress :: JKKSAddress(const QString & addr, int port, bool useGateway) : m_address(addr), m_port(port), m_useGateway(useGateway)
{

}

JKKSAddress :: JKKSAddress() : m_address(QString()), m_port(0), m_useGateway(false)
{

}

JKKSAddress::JKKSAddress(const JKKSAddress & a) : m_address(a.m_address), m_port(a.m_port), m_useGateway(a.m_useGateway)
{

}

JKKSAddress::~JKKSAddress()
{

}


const QString & JKKSAddress::address() const
{
    return m_address;
}

int JKKSAddress::port() const
{
    if(m_port <= 0)
        return 0;

    return m_port;
}

bool JKKSAddress::useGateway() const
{
    return m_useGateway;
}

void JKKSAddress::setAddress(const QString & addr)
{
    m_address = addr;
}

void JKKSAddress::setPort(int p)
{
    m_port = p;
}

void JKKSAddress::setUseGateway(bool b)
{
    m_useGateway = b;
}

QDataStream& operator<< (QDataStream& out, const JKKSAddress& T)
{
    out << T.m_address;
    out << T.m_port;
    out << T.m_useGateway;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSAddress& T)
{
    in >> T.m_address;
    in >> T.m_port;
    in >> T.m_useGateway;

    return in;
}

/*****************************************/
/*****************************************/
JKKSMessage :: JKKSMessage (const JKKSAddress & address, const QString& code)
    : m_addr (address),
    m_kvs (code),
    c (QMap<qint64, JKKSCategory>())
{
}

JKKSMessage :: JKKSMessage (const JKKSMessage& mess)
    : m_addr (mess.m_addr),
    m_kvs (mess.m_kvs),
    c (mess.c),
    m_senderAddr(mess.m_senderAddr)
{
}

JKKSMessage :: ~JKKSMessage (void)
{
}

const JKKSAddress & JKKSMessage :: getAddr (void) const
{
    return m_addr;
}

QString JKKSMessage :: getCode (void) const
{
    return m_kvs;
}

void JKKSMessage :: setAddr (const JKKSAddress & addr)
{
    m_addr = addr;
}

const JKKSAddress & JKKSMessage :: getSenderAddr() const
{
    return m_senderAddr;
}

void JKKSMessage :: setSenderAddr(const JKKSAddress & addr)
{
    m_senderAddr = addr;
}

void JKKSMessage :: setCode (const QString& code)
{
    m_kvs = code;
}

JKKSMessage::JKKSMessageType JKKSMessage :: getMessageType (void) const
{
    return JKKSMessage::atUnknownType;
}

QByteArray JKKSMessage :: serialize (void) const
{
    QByteArray res;
    QBuffer *devS = new QBuffer (&res);
    devS->open (QIODevice::WriteOnly);
    devS->write (QString("%1 ").arg (this->getMessageType ()).toLocal8Bit());
    devS->close ();
    delete devS;
    qDebug () << __PRETTY_FUNCTION__ << res;
    return res;
}

const QMap<qint64, JKKSCategory>& JKKSMessage :: getCategory (void) const
{
    return c;
}

void JKKSMessage :: setCategory (const QMap<qint64, JKKSCategory>& catMap)
{
    Q_UNUSED (catMap);
}
