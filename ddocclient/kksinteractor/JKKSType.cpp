#include <QDataStream>
#include "JKKSType.h"

JKKSType :: JKKSType (int id, const QString& name, const QString& desc, const QString& uid)
    : JKKSUID (uid, QString()),
    m_id(id),
    m_name(name),
    m_desc(desc)
{
}

JKKSType :: JKKSType (const JKKSType& type)
    : JKKSUID (type),
    m_id(type.m_id),
    m_name(type.m_name),
    m_desc(type.m_desc)
{
}

JKKSType :: ~JKKSType (void)
{
}

int JKKSType :: getIdType (void) const
{
    return m_id;
}

void JKKSType :: setIdType (int id)
{
    m_id = id;
}

const QString & JKKSType :: getName (void) const
{
    return m_name;
}

void JKKSType :: setName (const QString & name)
{
    m_name = name;
}

const QString & JKKSType :: getDesc(void) const
{
    return m_desc;
}

void JKKSType :: setDesc(const QString & desc)
{
    m_desc = desc;
}

QDataStream& operator<< (QDataStream& out, const JKKSType& type)
{
    out << type.m_id;
    out << type.m_name;
    out << type.m_desc;

    out << type.uid();
    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSType& type)
{
    in >> type.m_id;
    in >> type.m_name;
    in >> type.m_desc;

    QString uid;
    in >> uid;
    type.setUid (uid);
    return in;
}
