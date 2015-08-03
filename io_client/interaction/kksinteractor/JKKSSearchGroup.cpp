#include <QDataStream>

#include "JKKSSearchCriterion.h"
#include "JKKSSearchGroup.h"

JKKSSearchGroup :: JKKSSearchGroup (qint64 id, qint64 id_parent, int oper, bool is_not, int gr_type)
    : idGroup (id),
    idParentGroup (id_parent),
    operGroup (oper),
    isNot (is_not),
    typeGroup (gr_type),
    searchCriteria (QMap<qint64, JKKSSearchCriterion>())
{
}

JKKSSearchGroup :: JKKSSearchGroup (const JKKSSearchGroup& SG)
    : idGroup (SG.idGroup),
    idParentGroup (SG.idParentGroup),
    operGroup (SG.operGroup),
    isNot (SG.isNot),
    typeGroup (SG.typeGroup),
    searchCriteria (SG.searchCriteria)
{
}

JKKSSearchGroup :: ~JKKSSearchGroup (void)
{
}

qint64 JKKSSearchGroup :: id (void) const
{
    return idGroup;
}

void JKKSSearchGroup :: setId (qint64 id)
{
    idGroup = id;
}

qint64 JKKSSearchGroup :: idParent (void) const
{
    return idParentGroup;
}

void JKKSSearchGroup :: setParent (qint64 id_parent)
{
    idParentGroup = id_parent;
}

int JKKSSearchGroup :: getOper (void) const
{
    return operGroup;
}

void JKKSSearchGroup :: setOper (int op)
{
    operGroup = op;
}

bool JKKSSearchGroup :: isNegative (void) const
{
    return isNot;
}

void JKKSSearchGroup :: setNegative (bool is_not)
{
    isNot = is_not;
}

int JKKSSearchGroup :: getType (void) const
{
    return typeGroup;
}

void JKKSSearchGroup :: setType (int type)
{
    typeGroup = type;
}

const QMap<qint64, JKKSSearchCriterion>& JKKSSearchGroup :: getCriteria (void) const
{
    return searchCriteria;
}

QMap<qint64, JKKSSearchCriterion>& JKKSSearchGroup :: getCriteria (void)
{
    return searchCriteria;
}

void JKKSSearchGroup :: setCriteria (const QMap<qint64, JKKSSearchCriterion>& crit)
{
    searchCriteria = crit;
}

QDataStream& operator<< (QDataStream& out, const JKKSSearchGroup& sg)
{
    out << sg.idGroup;
    out << sg.idParentGroup;
    out << sg.operGroup;
    out << sg.isNot;
    out << sg.typeGroup;
    out << sg.searchCriteria;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSSearchGroup& sg)
{
    in >> sg.idGroup;
    in >> sg.idParentGroup;
    in >> sg.operGroup;
    in >> sg.isNot;
    in >> sg.typeGroup;
    in >> sg.searchCriteria;

    return in;
}
