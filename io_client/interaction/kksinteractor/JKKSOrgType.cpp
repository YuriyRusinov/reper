#include <QDataStream>
#include "JKKSOrgType.h"

JKKSOrgType :: JKKSOrgType (qint64 id_org_type, 
                            const QString& orgTypeName, 
                            const QString& orgTypeSName, 
                            const QString& uid)
    : JKKSUID (uid, QString()),
    idOrgType (id_org_type),
    organizationTypeName (orgTypeName),
    organizationTypeShortName (orgTypeSName)
{
}

JKKSOrgType :: JKKSOrgType (const JKKSOrgType& orgType)
    : JKKSUID (orgType),
    idOrgType (orgType.idOrgType),
    organizationTypeName (orgType.organizationTypeName),
    organizationTypeShortName (orgType.organizationTypeShortName)
{
}

JKKSOrgType :: ~JKKSOrgType (void)
{
}

qint64 JKKSOrgType :: getIdOrgType (void) const
{
    return idOrgType;
}

void JKKSOrgType :: setIdOrgType (qint64 id)
{
    idOrgType = id;
}

QString JKKSOrgType :: getOrgTypeName (void) const
{
    return organizationTypeName;
}

void JKKSOrgType :: setOrgTypeName (const QString& orgTypeName)
{
    organizationTypeName = orgTypeName;
}

QString JKKSOrgType :: getOrgTypeShortName (void) const
{
    return organizationTypeShortName;
}

void JKKSOrgType :: setOrgTypeShortName (const QString& orgTypeSName)
{
    organizationTypeShortName = orgTypeSName;
}

QDataStream& operator<< (QDataStream& out, const JKKSOrgType& orgType)
{
    out << orgType.idOrgType;
    out << orgType.organizationTypeName;
    out << orgType.organizationTypeShortName;

    out << orgType.uid();
    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSOrgType& orgType)
{
    in >> orgType.idOrgType;
    in >> orgType.organizationTypeName;
    in >> orgType.organizationTypeShortName;

    QString uid;
    in >> uid;
    orgType.setUid (uid);
    return in;
}
