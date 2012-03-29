#include <QDataStream>
#include "JKKSIOUrl.h"

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
    : idObject (url.idObject),
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
    in >> URL.URLdata;

    return in;
}
