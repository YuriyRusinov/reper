/***********************************************************************
 * Module:  KKSFilter.cpp
 * Author:  sergey
 * Modified: 25 декабря 2008 г. 17:22:35
 * Purpose: Implementation of the class KKSFilter
 * Comment: фильтр по атрибуту
 *    используется при фильтрации справочников, табелей, журналов и прочих  ИО, которые являются контейнерными.
 ***********************************************************************/

#include "KKSAttribute.h"
#include "KKSFilter.h"

KKSFilter::KKSFilter() : KKSRecord()//KKSData()
{
   m_attribute = NULL;
   m_oper = foEq;
   m_caseSensitive = false;
   m_recursive = false;
}

KKSFilter::KKSFilter(const KKSFilter & other) : KKSRecord()// KKSData()
{
    m_attribute = NULL;
    
    m_caseSensitive = other.caseSensitive();
    m_recursive = other.recursive();
    
    setAttribute(other.attribute());
    setValues(other.values());
    setOperation(other.operation());
}

KKSFilter::KKSFilter(const KKSAttribute * a, const KKSValue * v, KKSFilter::FilterOper op) : KKSRecord()// KKSData()
{
    m_attribute = NULL;

    setAttribute(a);
    setValue(v);
    setOperation(op);
    m_caseSensitive = false;
    m_recursive = false;
}

KKSFilter::~KKSFilter()
{
    if(m_attribute)
        m_attribute->release();

    m_values.clear();
}

//======================================

void KKSFilter::setAttribute(const KKSAttribute * a)
{
    if(m_attribute)
        m_attribute->release();

    m_attribute = a;

    if(m_attribute)
        m_attribute->addRef();
}

void KKSFilter::setValue(const KKSValue * value)
{
    m_values.clear();
    
    if(value)
        m_values.append(value);
}

void KKSFilter::setOperation(KKSFilter::FilterOper oper)
{
    m_oper = oper;
}

void KKSFilter::addValue(const KKSValue * value)
{
    m_values.append(value);
}

void KKSFilter::setValues(const KKSList<const KKSValue *> & values)
{
    m_values.clear();
    m_values = values;
}

void KKSFilter::removeValue(int index)
{
    if(index >= m_values.count())
        return;

    m_values.removeAt(index);
}

void KKSFilter::clearValues()
{
    m_values.clear();
}

const KKSAttribute  * KKSFilter::attribute() const
{
    return m_attribute;
}

const KKSValue * KKSFilter::value(int index) const
{
    if(index >= m_values.count())
        return NULL;

    return m_values.at(index);
}

const KKSList<const KKSValue *> & KKSFilter::values() const
{
    return m_values;
}

KKSFilter::FilterOper KKSFilter::operation() const
{
    return m_oper;
}

bool KKSFilter::isCorrect() const
{
    bool bCorrect = true;
    
    switch (m_oper){
        //для таких операций возможна работа только с одним значением
        case foEq:
        case foNotEq:
        case foGr:
        case foLess:
        case foGrEq:
        case foLessEq:
        
        case foLike:
        case foLikeIn:
        case foLikeStart:
        case foLikeEnd:
            if(m_values.count() != 1)
                bCorrect = false;
            break;
        case foIn:
            if(m_values.count() == 0)
                bCorrect = false;
            break;
        case foBetween:
            if(m_values.count() != 2)
                bCorrect = false;
            break;
        case foInSQL:
            if(m_values.count() != 1)
                bCorrect = false;
            if(m_values.at(0)->value().startsWith("select ", Qt::CaseInsensitive) == false)
                bCorrect = false;
            break;
        default:
            if(m_values.count() != 0)
                bCorrect = false;
            break;
    }

    return bCorrect;
}

QString KKSFilter::constructForIn() const
{
    QString str;

    int cnt = m_values.count();
    if( cnt == 0)
        return str;

    for (int i=0; i<m_values.count(); i++)
    {
        const KKSValue * v = m_values[i];
        str += v->valueForInsert();
        if(i<cnt-1)
            str += ", ";
    }

    return str;
}

bool KKSFilter::caseSensitive() const
{
    return m_caseSensitive;
}

void KKSFilter::setCaseSensitive(bool cs)
{
    m_caseSensitive = cs;
}

bool KKSFilter::recursive() const
{
    return m_recursive;
}

void KKSFilter::setRecursive(bool r)
{
    m_recursive = r;
}

bool KKSFilter :: operator== (const KKSFilter& anotherFilter) const
{
    bool wres = true;
    if (this->m_attribute && anotherFilter.m_attribute)
        wres = wres && this->m_attribute->id() == anotherFilter.m_attribute->id();
    else if (this->m_attribute && !anotherFilter.m_attribute ||
             !this->m_attribute && anotherFilter.m_attribute)
        return false;
    wres = wres && this->m_oper == anotherFilter.m_oper;
    wres = wres && this->m_caseSensitive == anotherFilter.m_caseSensitive;
    wres = wres && this->m_recursive == anotherFilter.m_recursive;
    wres = wres && this->m_values.count() == anotherFilter.m_values.count();
    for (int i=0; i<this->m_values.count() && wres; i++)
        wres = wres && this->m_values[i]->value() == anotherFilter.m_values[i]->value();

    return wres;
}

bool KKSFilter :: operator!= (const KKSFilter& anotherFilter) const
{
    bool wres (*this == anotherFilter);

    return !wres;
}
/*===========================================*/
KKSFilterGroup::KKSFilterGroup(bool AND)
    : m_AND (AND)
{
}

KKSFilterGroup::KKSFilterGroup(const KKSFilterGroup & other)
    : KKSRecord (other),
    m_AND (other.m_AND),
    m_filters (other.m_filters),
    m_groups (other.groups())
{
}

KKSFilterGroup::~KKSFilterGroup()
{

}

KKSFilterGroup & KKSFilterGroup::operator = (const KKSFilterGroup & other) 
{
    
    setAND(other.AND());
    setFilters(other.filters());
    setGroups(other.groups());
   
    return *this;
}

void KKSFilterGroup::setAND(bool _and)
{
    m_AND = _and;
}

bool KKSFilterGroup::AND() const
{
    return m_AND;
}

void KKSFilterGroup::addFilter(const KKSFilter * f)
{
    if(!f)
        return;
    
    m_filters.append(f);
}

void KKSFilterGroup::addFilters(const KKSList<const KKSFilter*> & filters)
{
    for (int i=0; i<filters.count(); i++)
    {
        const KKSFilter * f = filters[i];
        m_filters.append(f);
    }
}

void KKSFilterGroup::setFilters(const KKSList<const KKSFilter*> & filters)
{
    m_filters.clear();
    m_filters = filters;
}

void KKSFilterGroup::removeFilter(int index)
{
    m_filters.removeAt(index);
}

void KKSFilterGroup::removeFilters(int attrId)
{
    for (int i=0; i<m_filters.count(); i++)
    {
        const KKSFilter * f = m_filters [i];
        const KKSAttribute * a = f->attribute();
        if(a->id() == attrId)
            m_filters.removeAll(f);
    }
}

void KKSFilterGroup::clear()
{
    clearFilters();
    clearGroups();
}

void KKSFilterGroup::clearFilters()
{
    m_filters.clear();
}

void KKSFilterGroup::clearGroups()
{
    m_groups.clear();
}

const KKSFilter * KKSFilterGroup::filter(int index) const
{
    return m_filters.at(index);
}

const KKSList<const KKSFilter*> & KKSFilterGroup::filters() const
{
    return m_filters;
}


void KKSFilterGroup::addGroup(const KKSFilterGroup * g)
{
    if(!g)
        return;
    
    m_groups.append(g);
}

void KKSFilterGroup::addGroups(const KKSList<const KKSFilterGroup*> & groups)
{
    for (int i=0; i<groups.count(); i++)
    {
        const KKSFilterGroup * g = groups[i];
        m_groups.append(g);
    }
}

void KKSFilterGroup::setGroups(const KKSList<const KKSFilterGroup*> & groups)
{
    m_groups.clear();
    m_groups = groups;
}

void KKSFilterGroup::removeGroup(int index)
{
    m_groups.removeAt(index);
}

const KKSFilterGroup * KKSFilterGroup::group(int index) const
{
    return m_groups.at(index);
}

const KKSList<const KKSFilterGroup*> & KKSFilterGroup::groups() const
{
    return m_groups;
}
