#include <QDataStream>

#include "JKKSSearchCriterion.h"

JKKSSearchCriterion :: JKKSSearchCriterion (int idc, \
                                            const JKKSCategoryAttr& attr, \
                                            QString val, \
                                            int type, \
                                            int oper, \
                                            bool caseSensitive, \
                                            bool isNot)
    : idC (idc),
    attribute (attr),
    value (val),
    criterionType (type),
    criterionOperation (oper),
    criterionCaseSensitive (caseSensitive),
    criterionIsNot (isNot)
{
}

JKKSSearchCriterion :: JKKSSearchCriterion (const JKKSSearchCriterion& sc)
    : idC(sc.idC),
    attribute (sc.attribute),
    value (sc.value),
    criterionType (sc.criterionType),
    criterionOperation (sc.criterionOperation),
    criterionCaseSensitive (sc.criterionCaseSensitive),
    criterionIsNot (sc.criterionIsNot)
{
}

JKKSSearchCriterion :: ~JKKSSearchCriterion (void)
{
}

int JKKSSearchCriterion :: id (void) const
{
    return idC;
}

void JKKSSearchCriterion :: setId (int idc)
{
    idC = idc;
}

JKKSCategoryAttr JKKSSearchCriterion :: getAttribute (void) const
{
    return attribute;
}

void JKKSSearchCriterion :: setAttribute (const JKKSCategoryAttr& attr)
{
    attribute = attr;
}

QString JKKSSearchCriterion :: getValue (void) const
{
    return value;
}

void JKKSSearchCriterion :: setValue (QString val)
{
    value = val;
}

int JKKSSearchCriterion :: getType (void) const
{
    return criterionType;
}

void JKKSSearchCriterion :: setType (int type)
{
    criterionType = type;
}

int JKKSSearchCriterion :: getOperation (void) const
{
    return criterionOperation;
}

void JKKSSearchCriterion :: setOperation (int op)
{
    criterionOperation = op;
}

bool JKKSSearchCriterion :: isCaseSensitive (void) const
{
    return criterionCaseSensitive;
}

void JKKSSearchCriterion :: setCaseSensitive (bool cs)
{
    criterionCaseSensitive = cs;
}

bool JKKSSearchCriterion :: isNegative (void) const
{
    return criterionIsNot;
}

void JKKSSearchCriterion :: setNegative (bool is_not)
{
    criterionIsNot = is_not;
}

QDataStream& operator<< (QDataStream& out, const JKKSSearchCriterion& sc)
{
    out << sc.idC;
    out << sc.attribute;
    out << sc.value;
    out << sc.criterionType;
    out << sc.criterionOperation;
    out << sc.criterionCaseSensitive;
    out << sc.criterionIsNot;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSSearchCriterion& sc)
{
    in >> sc.idC;
    in >> sc.attribute;
    in >> sc.value;
    in >> sc.criterionType;
    in >> sc.criterionOperation;
    in >> sc.criterionCaseSensitive;
    in >> sc.criterionIsNot;

    return in;
}
