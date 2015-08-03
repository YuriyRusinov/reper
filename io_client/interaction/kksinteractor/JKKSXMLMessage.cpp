/**************************************************************************************
 * Реализация класса передаваемых команд
 **************************************************************************************/

#include <QDataStream>
#include <QTextStream>
#include <QStringList>
#include <QBuffer>
#include <QIODevice>
#include <QtDebug>

#include "JKKSXMLMessage.h"
#include "JKKSLoader.h"
#include "kksresult.h"
#include "defines.h"


JKKSXMLMessage::JKKSXMLMessage(qint64 idQueue, 
                               int idFormat,
                               int idOrganization,
                               int interactionType,
                               const QString & xmlData,
                               const JKKSAddress & addr,
                               const QString & kvs)
: JKKSMessage (addr, kvs),
m_id(idQueue),
m_idFormat(idFormat),
m_idOrganization(idOrganization),
m_interactionType(interactionType),
m_xmlData(xmlData)
{
}

JKKSXMLMessage::JKKSXMLMessage(const JKKSXMLMessage & xml)
: JKKSMessage (xml),
m_id(xml.id()),
m_idFormat(xml.idFormat()),
m_idOrganization(xml.idOrganization()),
m_interactionType(xml.interactionType()),
m_xmlData(xml.xmlData())
{

}

JKKSXMLMessage::~JKKSXMLMessage()
{

}

QByteArray JKKSXMLMessage::serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    out << getAddr();
    out << getSenderAddr();
    out << getCode();
    
    out << id();

    out << idFormat();
    out << idOrganization();
    out << interactionType();
    out << xmlData();

    return qBuffer.buffer();
}

int JKKSXMLMessage::unserialize (const QByteArray& mess)
{
    QBuffer buffer;
    buffer.setData(mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    JKKSAddress addr;
    JKKSAddress senderAddr;
    QString code;

    in >> addr;
    in >> senderAddr;
    in >> code;

    in >> m_id;

    in >> m_idFormat;
    in >> m_idOrganization;
    in >> m_interactionType;
    in >> m_xmlData;

    setAddr (addr);
    setSenderAddr(senderAddr);
    setCode (code);

    return OK_CODE;
}

int JKKSXMLMessage::writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    //Q_UNUSED (senderUID);
    Q_UNUSED (receiverUID);
    return loader->writeMessage (this, senderUID);
}

JKKSMessage::JKKSMessageType JKKSXMLMessage::getMessageType (void) const
{
    return JKKSMessage::atXMLMessage;
}

qint64 JKKSXMLMessage::id (void) const
{
    return m_id;
}

void JKKSXMLMessage::setId (qint64 the_id)
{
    m_id = the_id;
}

int JKKSXMLMessage::idFormat (void) const
{
    return m_idFormat;
}

void JKKSXMLMessage::setIdFormat (int idFmt)
{
    m_idFormat = idFmt;
}


int JKKSXMLMessage::idOrganization (void) const
{
    return m_idOrganization;
}

void JKKSXMLMessage::setIdOrganization (int idOrg)
{
    m_idOrganization = idOrg;
}
        
int JKKSXMLMessage::interactionType (void) const
{
    return m_interactionType;
}

void JKKSXMLMessage::setInteractionType (int type)
{
    m_interactionType = type;
}

const QString & JKKSXMLMessage::xmlData (void) const
{
    return m_xmlData;
}

void JKKSXMLMessage::setXMLData (const QString & xml)
{
    m_xmlData = xml;
}
