#include <QDataStream>

#include "JKKSSearchTemplate.h"

JKKSSearchTemplate :: JKKSSearchTemplate (qint64 id, QString name, QString desc)
    : idSearchTemplate (id),
    searchTemplateName (name),
    searchTemplateDesc (desc),
    groups (QMap<qint64, JKKSSearchGroup>())
{
}

JKKSSearchTemplate :: JKKSSearchTemplate (const JKKSSearchTemplate& ST)
    : idSearchTemplate (ST.idSearchTemplate),
    searchTemplateName (ST.searchTemplateName),
    searchTemplateDesc (ST.searchTemplateDesc),
    groups (ST.groups)
{
}

JKKSSearchTemplate :: ~JKKSSearchTemplate (void)
{
    groups.clear();
}

qint64 JKKSSearchTemplate :: id (void) const
{
    return idSearchTemplate;
}

void JKKSSearchTemplate :: setId (qint64 id)
{
    idSearchTemplate = id;
}

QString JKKSSearchTemplate :: getName (void) const
{
    return searchTemplateName;
}

void JKKSSearchTemplate :: setName (QString name)
{
    searchTemplateName = name;
}

QString JKKSSearchTemplate :: getDesc (void) const
{
    return searchTemplateDesc;
}

void JKKSSearchTemplate :: setDesc (QString desc)
{
    searchTemplateDesc = desc;
}

const QMap<qint64, JKKSSearchGroup>& JKKSSearchTemplate :: getGroups (void) const
{
    return groups;
}

QMap<qint64, JKKSSearchGroup>& JKKSSearchTemplate :: getGroups (void)
{
    return groups;
}

void JKKSSearchTemplate :: setGroups (QMap<qint64, JKKSSearchGroup>& grs)
{
    groups = grs;
}

QDataStream& operator<< (QDataStream& out, const JKKSSearchTemplate& st)
{
    out << st.idSearchTemplate;
    out << st.searchTemplateName;
    out << st.searchTemplateDesc;
    out << st.groups;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSSearchTemplate& st)
{
    in >> st.idSearchTemplate;
    in >> st.searchTemplateName;
    in >> st.searchTemplateDesc;
    in >> st.groups;

    return in;
}
