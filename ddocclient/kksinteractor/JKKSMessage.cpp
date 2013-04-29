#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QBuffer>
#include <QtDebug>

#include "JKKSMessage.h"

JKKSAddress :: JKKSAddress(const QString & addr, int port) : m_address(addr), m_port(port)
{

}

JKKSAddress :: JKKSAddress() : m_address(QString()), m_port(0)
{

}

JKKSAddress::JKKSAddress(const JKKSAddress & a) : m_address(a.m_address), m_port(a.m_port)
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

void JKKSAddress::setAddress(const QString & addr)
{
    m_address = addr;
}

void JKKSAddress::setPort(int p)
{
    m_port = p;
}

QDataStream& operator<< (QDataStream& out, const JKKSAddress& T)
{
    out << T.m_address;
    out << T.m_port;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSAddress& T)
{
    in >> T.m_address;
    in >> T.m_port;

    return in;
}


JKKSMessage :: JKKSMessage (const JKKSAddress & address, const QString& code)
    : m_addr (address),
    m_kvs (code),
    c (QMap<int, JKKSCategory>())
{
}

JKKSMessage :: JKKSMessage (const JKKSMessage& mess)
    : m_addr (mess.m_addr),
    m_kvs (mess.m_kvs),
    c (mess.c)
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

const QMap<int, JKKSCategory>& JKKSMessage :: getCategory (void) const
{
    return c;
}

void JKKSMessage :: setCategory (const QMap<int, JKKSCategory>& catMap)
{
    Q_UNUSED (catMap);
}
