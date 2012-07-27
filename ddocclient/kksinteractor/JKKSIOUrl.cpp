#include <QDataStream>
#include <QBuffer>
#include "JKKSIOUrl.h"
#include <defines.h>
#include "JKKSLoader.h"

JKKSIOUrl :: JKKSIOUrl (int id, int id_url, const QString& name1, const QString& name2, int type, const QString& Src_ext, int l, const char * d)
    : idObject (id),
    idURL (id_url),
    IO_URL (name1),
    URL (name2),
    urlType (type),
    src_ext (Src_ext),
    length (l),
    URLdata (d)
{
}

JKKSIOUrl :: JKKSIOUrl (const JKKSIOUrl& url)
    : JKKSUID(url),
    idObject (url.idObject),
    idURL (url.idURL),
    IO_URL (url.IO_URL),
    URL (url.URL),
    urlType (url.urlType),
    src_ext (url.src_ext),
    length (url.length),
    URLdata (url.URLdata)
{
}

JKKSIOUrl :: ~JKKSIOUrl (void)
{
}

int JKKSIOUrl :: getIOId (void) const
{
    return idObject;
}

void JKKSIOUrl :: setIOId (int id)
{
    idObject = id;
}

int JKKSIOUrl :: getURLId (void) const
{
    return idURL;
}

void JKKSIOUrl :: setURLId (int idUrl)
{
    idURL = idUrl;
}

const QString& JKKSIOUrl :: getIOURL (void) const
{
    return IO_URL;
}

void JKKSIOUrl :: setIOURL (const QString& io_url)
{
    IO_URL = io_url;
}

const QString& JKKSIOUrl :: getURL (void) const
{
    return URL;
}

void JKKSIOUrl :: setURL (const QString& url)
{
    URL = url;
}

int JKKSIOUrl :: getType (void) const
{
    return urlType;
}

void JKKSIOUrl :: setType (int type)
{
    urlType = type;
}

const QString& JKKSIOUrl :: getSrcExt (void) const
{
    return src_ext;
}

void JKKSIOUrl :: setSrcExt (const QString& s)
{
    src_ext = s;
}

int JKKSIOUrl :: getDataLength (void) const
{
    return length;
}

void JKKSIOUrl :: setDataLength (int l)
{
    length = l;
}

const QByteArray& JKKSIOUrl :: getData (void) const
{
    return URLdata;
}

void JKKSIOUrl :: setData (const QByteArray& d)
{
    URLdata = d;
}

QDataStream& operator<< (QDataStream& out, const JKKSIOUrl& URL)
{
    out << URL.idObject;
    out << URL.idURL;
    out << URL.IO_URL;
    out << URL.URL;
    out << URL.urlType;
    out << URL.src_ext;
    out << URL.length;
    out << URL.uid();
    out << URL.URLdata;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSIOUrl& URL)
{
    in >> URL.idObject;
    in >> URL.idURL;
    in >> URL.IO_URL;
    in >> URL.URL;
    in >> URL.urlType;
    in >> URL.src_ext;
    in >> URL.length;
    QString uid;
    in >> uid;
    URL.setUid(uid);
    in >> URL.URLdata;

    return in;
}


/*
------------------------------------------
------------------------------------------
------------------------------------------
*/

JKKSFilePart::JKKSFilePart() : JKKSMessage()
{
    m_isLast = true;
    m_idUrl = -1;
    m_idQueue = -1;
}

JKKSFilePart::JKKSFilePart(const JKKSFilePart & part) : JKKSMessage(part)
{
    m_isLast = part.m_isLast;
    m_idUrl = part.m_idUrl;
    m_uid = part.m_uid;
    m_data = part.m_data;
    m_idQueue = part.m_idQueue;
    m_senderAddr = part.m_senderAddr;
    m_absUrl = part.m_absUrl;
}

JKKSFilePart::JKKSFilePart(QString uid, bool isLast) : JKKSMessage()
{
    m_uid = uid;
    m_isLast = isLast;
    m_idUrl = -1;
    m_idQueue = -1;
}

JKKSFilePart::~JKKSFilePart()
{

}

void JKKSFilePart::setData(const QByteArray & data)
{
    m_data = data;
}

const QByteArray & JKKSFilePart::getData() const
{
    return m_data;
}

void JKKSFilePart::setAbsUrl(const QString & url)
{
    m_absUrl = url;
}

const QString & JKKSFilePart::getAbsUrl() const
{
    return m_absUrl;
}

void JKKSFilePart::setIdUrl(int idUrl)
{
    m_idUrl = idUrl;
}

int JKKSFilePart::getIdUrl() const
{
    return m_idUrl;
}

void JKKSFilePart::setId(int idQueue)
{
    m_idQueue = idQueue;
}

int JKKSFilePart::id() const
{
    return m_idQueue;
}

void JKKSFilePart::setIsLast(bool isLast)
{
    m_isLast = isLast;
}

bool JKKSFilePart::isLast() const
{
    return m_isLast;
}

const QString & JKKSFilePart::getSenderAddr() const
{
    return m_senderAddr;
}

void JKKSFilePart::setSenderAddr(const QString & addr)
{
    m_senderAddr = addr;
}


QDataStream& operator<< (QDataStream& out, const JKKSFilePart& part)
{
    out << part.m_idUrl;
    out << part.m_uid;
    out << part.m_data;
    out << part.m_isLast;
    out << part.m_absUrl;
    out << part.getAddr();
    out << part.m_senderAddr;
    out << part.m_idQueue;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSFilePart& part)
{
    in >> part.m_idUrl;
    in >> part.m_uid;
    in >> part.m_data;
    in >> part.m_isLast;
    in >> part.m_absUrl;
    QString addr;
    in >> addr;
    part.setAddr(addr);

    in >> part.m_senderAddr;
    
    in >> part.m_idQueue;

    return in;
}

QByteArray JKKSFilePart :: serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open(QIODevice::WriteOnly);
    QDataStream out(&qBuffer);

    out << getAddr();

    out << m_senderAddr;

    out << m_idUrl;
    out << m_uid;
    out << m_data;
    out << m_isLast;
    out << m_absUrl;
    out << m_idQueue;

    return qBuffer.buffer();
}

int JKKSFilePart :: unserialize (const QByteArray& mess)
{
    
    QBuffer buffer;
    buffer.setData (mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    QString addr;
    in >> addr;
    setAddr(addr);

    in >> m_senderAddr;
    
    in >> m_idUrl;
    in >> m_uid;
    in >> m_data;
    in >> m_isLast;
    in >> m_absUrl;
    in >> m_idQueue;

    return OK_CODE;
}

int JKKSFilePart :: writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    int ier = loader->writeMessage (this, senderUID);
    return ier;
}
