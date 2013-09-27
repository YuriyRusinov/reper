#include <QDataStream>

#include "JKKSCategory.h"
#include "JKKSIOTable.h"

JKKSIOTable :: JKKSIOTable (int idIOTable,
                            const QString & table_name,
                            const QString & title,
                            const QString & uid
             )
    : JKKSUID (uid, QString()),
    id (idIOTable),
    tableName (table_name),
    category (QMap<int, JKKSCategory>()),
    Title (title)
{
}

JKKSIOTable :: JKKSIOTable (const JKKSIOTable& T)
    : JKKSUID (T),
    id (T.id),
    tableName (T.tableName),
    category (T.category),
    Title (T.Title)
{
}

JKKSIOTable :: ~JKKSIOTable (void)
{
}

int JKKSIOTable :: getIDTable (void) const
{
    return id;
}

void JKKSIOTable :: setIDTable (int _id)
{
    id = _id;
}

QString JKKSIOTable :: getName (void) const
{
    return tableName;
}

void JKKSIOTable :: setName (QString tName)
{
    tableName = tName;
}

const QMap<int, JKKSCategory>& JKKSIOTable :: getCategory (void) const
{
    return category;
}

void JKKSIOTable :: setCategory (const QMap<int, JKKSCategory>& catMap)
{
    category = catMap;
}

QString JKKSIOTable :: getTitle (void) const
{
    return Title;
}

void JKKSIOTable :: setTitle (QString tTitle)
{
    Title = tTitle;
}

QDataStream& operator<< (QDataStream& out, const JKKSIOTable& T)
{
    out << T.id;
    out << T.tableName;
    out << T.category;
    out << T.Title;
    out << T.uid();

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSIOTable& T)
{
    in >> T.id;
    in >> T.tableName;
    in >> T.category;
    in >> T.Title;
    QString uid;
    in >> uid;
    T.setUid (uid);

    return in;
}
