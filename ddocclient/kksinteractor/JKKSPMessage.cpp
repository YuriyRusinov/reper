/*
 * Implementation of JKKSPMessage class
 * Класс занимается распаковкой сообщений в зависимости от данных и их типа
 */

#include "JKKSPMessage.h"
#include <QCryptographicHash>

JKKSPMessage::JKKSPMessage (const QByteArray& messData, 
                            const int & type, 
                            const QString& sender_uid,
                            const QString& receiver_uid)
    :data(messData),
    messType(type),
    m_senderUID (sender_uid),
    m_receiverUID (receiver_uid),
    m_verifyReceiver(true),
    m_urgencyLevel('C'),
    m_macLabel('0')
{
}

JKKSPMessage :: JKKSPMessage (void)
    : data (QByteArray()),
    messType (-1),
    m_senderUID (QString()),
    m_receiverUID (QString()),
    m_verifyReceiver(true),
    m_urgencyLevel('C'),
    m_macLabel('0')
{
}

JKKSPMessage :: JKKSPMessage (const JKKSPMessage& pmess)
    : data (pmess.data),
    messType (pmess.messType),
    urlList (pmess.urlList),
    m_senderUID (pmess.senderUID()),
    m_receiverUID (pmess.receiverUID()),
    m_verifyReceiver(pmess.verifyReceiver()),
    m_urgencyLevel(pmess.m_urgencyLevel),
    m_macLabel(pmess.m_macLabel),
    m_cryptoHash(pmess.m_cryptoHash)
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
    in >> m_senderUID;
    in >> m_receiverUID;
    in >> m_verifyReceiver;
    in >> m_urgencyLevel;
    in >> m_macLabel;

    in >> m_cryptoHash;
}

QByteArray JKKSPMessage :: getData (void) const
{
    return data;
}

int JKKSPMessage :: getType (void) const
{
    return messType;
}

void JKKSPMessage :: setType(int t)
{
    messType = t;
}

QByteArray JKKSPMessage :: serialize (void) const
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);    
    out << messType;
    out << data;
    out << m_senderUID;
    out << m_receiverUID;
    out << m_verifyReceiver;
    out << m_urgencyLevel;
    out << m_macLabel;

    out << m_cryptoHash;

    return buffer.data();
}

const QByteArray & JKKSPMessage :: cryptoHash() const //hash-сумма. Возвращается сохраненное значение. Если оно пустое, то сначала пересчитывается
{
    if(m_cryptoHash.isEmpty())
        return calculateCryptoHash();

    return m_cryptoHash;
}

const QByteArray & JKKSPMessage :: calculateCryptoHash() const //вычисление hash-суммы. Вызывается метод serialize()
{
    QByteArray d = this->serialize();
    m_cryptoHash = QCryptographicHash::hash(d, QCryptographicHash::Sha1);
    
    return m_cryptoHash;
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

JKKSPMessWithAddr :: JKKSPMessWithAddr (const JKKSPMessage& pm, const JKKSAddress & a, const qint64 & _id)
    : pMess (pm),
    addr (a),
    id (_id)
{
}

JKKSPMessWithAddr::JKKSPMessWithAddr (JKKSPMessage * message, const JKKSAddress & a, const qint64 & c) :
    pMess(*message),
    addr(a),
    id(c)
{
}

JKKSPMessWithAddr :: ~JKKSPMessWithAddr (void)
{
}
