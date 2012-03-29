#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QBuffer>
#include <QtDebug>

#include "JKKSMessage.h"

JKKSMessage :: JKKSMessage (const QString& address, const QString& code)
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

QString JKKSMessage :: getAddr (void) const
{
    return m_addr;
}

QString JKKSMessage :: getCode (void) const
{
    return m_kvs;
}

void JKKSMessage :: setAddr (const QString& addr)
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
