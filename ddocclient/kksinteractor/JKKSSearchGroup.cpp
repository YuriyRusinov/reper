#include <QDataStream>

#include "JKKSSearchCriterion.h"
#include "JKKSSearchGroup.h"

JKKSSearchGroup :: JKKSSearchGroup (int id, int id_parent, int oper, bool is_not, int gr_type)
    : idGroup (id),
    idParentGroup (id_parent),
    operGroup (oper),
    isNot (is_not),
    typeGroup (gr_type),
    searchCriteria (QMap<int, JKKSSearchCriterion>())
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

int JKKSSearchGroup :: id (void) const
{
    return idGroup;
}

void JKKSSearchGroup :: setId (int id)
{
    idGroup = id;
}

int JKKSSearchGroup :: idParent (void) const
{
    return idParentGroup;
}

void JKKSSearchGroup :: setParent (int id_parent)
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

const QMap<int, JKKSSearchCriterion>& JKKSSearchGroup :: getCriteria (void) const
{
    return searchCriteria;
}

QMap<int, JKKSSearchCriterion>& JKKSSearchGroup :: getCriteria (void)
{
    return searchCriteria;
}

void JKKSSearchGroup :: setCriteria (const QMap<int, JKKSSearchCriterion>& crit)
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
