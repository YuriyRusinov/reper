#include <QString>
#include <QDataStream>

#include "JKKSRubric.h"
#include "JKKSSearchTemplate.h"

JKKSRubric :: JKKSRubric (qint64 id, 
                          const QString& uid, 
                          qint64 idp, 
                          qint64 idc, 
                          const QString& name, 
                          const QString& code, 
                          const QString& desc)
    : idRubric (id),
    uidRubric (uid),
    idParent (idp),
    idEntity (idc),
    rubricName (name),
    rubricCode (code),
    rubricDesc (desc)
//    category (QMap<int, JKKSCategory>()),
//    searchTemplate (JKKSSearchTemplate())
{
}

JKKSRubric :: JKKSRubric (const JKKSRubric& R)
    : idRubric (R.idRubric),
    uidRubric (R.uidRubric),
    idParent (R.idParent),
    idEntity (R.idEntity),
    rubricName (R.rubricName),
    rubricCode (R.rubricCode),
    rubricDesc (R.rubricDesc)
//    category (R.category),
//    searchTemplate (R.searchTemplate)
{
}

JKKSRubric :: ~JKKSRubric (void)
{
}

JKKSRubric& JKKSRubric :: operator= (const JKKSRubric& R)
{
    idRubric = R.idRubric;
    uidRubric = R.uidRubric;
    idParent = R.idParent;
    idEntity = R.idEntity;
    rubricName = R.rubricName;
    rubricCode = R.rubricCode;
    rubricDesc = R.rubricDesc;

//    category = R.category;
//    searchTemplate = R.searchTemplate;
    return *this;
}

qint64 JKKSRubric :: getIdRubric (void) const
{
    return idRubric;
}

void JKKSRubric :: setIdRubric (qint64 idr)
{
    idRubric = idr;
}

const QString& JKKSRubric :: getUid (void) const
{
    return uidRubric;
}

void JKKSRubric :: setUid (const QString& uid)
{
    uidRubric = uid;
}


qint64 JKKSRubric :: getParent (void) const
{
    return idParent;
}

void JKKSRubric :: setParent (qint64 idp)
{
    idParent = idp;
}

qint64 JKKSRubric :: getEntity (void) const
{
    return idEntity;
}

void JKKSRubric :: setEntity (qint64 idc)
{
    idEntity = idc;
}

const QString& JKKSRubric :: getName (void) const
{
    return rubricName;
}

void JKKSRubric :: setName (const QString& name)
{
    rubricName = name;
}

const QString& JKKSRubric :: getCode (void) const
{
    return rubricCode;
}

void JKKSRubric :: setCode (const QString& code)
{
    rubricCode = code;
}

const QString& JKKSRubric :: getDesc (void) const
{
    return rubricDesc;
}

void JKKSRubric :: setDesc (const QString& desc)
{
    rubricDesc = desc;
}

QDataStream& operator<< (QDataStream& out, const JKKSRubric& R)
{
    out << R.idRubric;
    out << R.idParent;
    out << R.idEntity;
    out << R.rubricName;
    out << R.rubricCode;
    out << R.rubricDesc;
    out << R.uidRubric;
//    out << R.category;
//    out << R.searchTemplate;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSRubric& R)
{
    in >> R.idRubric;
    in >> R.idParent;
    in >> R.idEntity;
    in >> R.rubricName;
    in >> R.rubricCode;
    in >> R.rubricDesc;
    in >> R.uidRubric;
//    in >> R.category;
//    in >> R.searchTemplate;

    return in;
}

