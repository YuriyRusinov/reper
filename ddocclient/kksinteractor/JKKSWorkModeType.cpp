#include <QDataStream>
#include <QtDebug>

#include "JKKSWorkModeType.h"

JKKSWorkModeType :: JKKSWorkModeType (qint64 id, const QString& nameType, const QString& shortNameType, const QString& uid)
    : JKKSUID (uid, QString()),
      idWMType (id),
      wmNameType (nameType),
      wmShortNameType (shortNameType)
{
}

JKKSWorkModeType :: JKKSWorkModeType (const JKKSWorkModeType& wmType)
    : JKKSUID (wmType),
      idWMType (wmType.idWMType),
      wmNameType (wmType.wmNameType),
      wmShortNameType (wmType.wmShortNameType)
{
}

JKKSWorkModeType :: ~JKKSWorkModeType (void)
{
}

qint64 JKKSWorkModeType :: id (void) const
{
    return idWMType;
}

void JKKSWorkModeType :: setId (qint64 _id)
{
    idWMType = _id;
}

const QString& JKKSWorkModeType :: getName (void) const
{
    return wmNameType;
}

void JKKSWorkModeType :: setName (const QString& name)
{
    wmNameType = name;
}

const QString& JKKSWorkModeType :: getShortName (void) const
{
    return wmShortNameType;
}

void JKKSWorkModeType :: setShortName (const QString& name)
{
    wmShortNameType = name;
}

QDataStream& operator<< (QDataStream& out, const JKKSWorkModeType& wmType)
{
    out << wmType.idWMType;
    out << wmType.wmNameType;
    out << wmType.wmShortNameType;
    out << JKKSUID (wmType).uid();

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSWorkModeType& wmType)
{
    in >> wmType.idWMType;
    in >> wmType.wmNameType;
    in >> wmType.wmShortNameType;

    QString uid;
    in >> uid;
    wmType.setUid (uid);

    return in;
}


