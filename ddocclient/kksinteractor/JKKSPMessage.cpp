/*
 * Implementation of JKKSPMessage class
 * Класс занимается распаковкой сообщений в зависимости от данных и их типа
 */

#include "JKKSPMessage.h"

JKKSPMessage::JKKSPMessage (const QByteArray& messData, 
                            const int & type, 
                            const QString& sender_uid,
                            const QString& receiver_uid)
    :data(messData),
    messType(type),
    senderUID (sender_uid),
    receiverUID (receiver_uid),
    verifyReceiver(true),
    m_urgencyLevel('C'),
    m_macLabel('0')
{
}

JKKSPMessage :: JKKSPMessage (void)
    : data (QByteArray()),
    messType (-1),
    senderUID (QString()),
    receiverUID (QString()),
    verifyReceiver(true),
    m_urgencyLevel('C'),
    m_macLabel('0')
{
}

JKKSPMessage :: JKKSPMessage (const JKKSPMessage& pmess)
    : data (pmess.data),
    messType (pmess.messType),
    urlList (pmess.urlList),
    senderUID (pmess.senderUID),
    receiverUID (pmess.receiverUID),
    verifyReceiver(pmess.verifyReceiver),
    m_urgencyLevel(pmess.m_urgencyLevel),
    m_macLabel(pmess.m_macLabel)
{
}

JKKSPMessage::JKKSPMessage(const QByteArray& mess)
{
    QBuffer buffer;
    buffer.setData(mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    in >> messType;
    in >> data;
    in >> senderUID;
    in >> receiverUID;
    in >> verifyReceiver;
    in >> m_urgencyLevel;
    in >> m_macLabel;
}

QByteArray JKKSPMessage :: getData (void) const
{
    return data;
}

int JKKSPMessage :: getType (void) const
{
    return messType;
}

QByteArray JKKSPMessage :: serialize (void) const
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);    
    out << messType;
    out << data;
    out << senderUID;
    out << receiverUID;
    out << verifyReceiver;
    out << m_urgencyLevel;
    out << m_macLabel;

    return buffer.data();
}

QStringList JKKSPMessage :: getFiles (void) const
{
    return urlList;
}

void JKKSPMessage :: setFiles (const QStringList& files)
{
    urlList = files;
}

void JKKSPMessage :: setUrgencyLevel(const QString & code)
{
    if(code.isEmpty())
        m_urgencyLevel = 'C';

    char c = code.at(0).toAscii();
    if(c != 'A' && c != 'B' && c != 'C')
        m_urgencyLevel = 'C';
    else
        m_urgencyLevel = (unsigned char)c;
}

void JKKSPMessage :: setMacLabel(const QString & mac)
{
    if(mac.isEmpty())
        m_macLabel = '0';

    char c = mac.at(0).toAscii();
    if(c != '0' && c != '1' && c != '2' && c != '3')
        m_macLabel = '0';
    else
        m_macLabel = (unsigned char)c;
}

JKKSPMessWithAddr :: JKKSPMessWithAddr (void)
    : pMess (JKKSPMessage()),
      addr (JKKSAddress())
{
}

JKKSPMessWithAddr :: JKKSPMessWithAddr (const JKKSPMessage& pm, const JKKSAddress & a, const int& _id)
    : pMess (pm),
    addr (a),
    id (_id)
{
}

JKKSPMessWithAddr::JKKSPMessWithAddr (JKKSPMessage * message, const JKKSAddress & a, const int& c) :
    pMess(*message),
    addr(a),
    id(c)
{
}

JKKSPMessWithAddr :: ~JKKSPMessWithAddr (void)
{
}
