/***********************************************************************
 * Module:  KKSCategoryAttr.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:12:50
 * Purpose: Implementation of the class KKSCategoryAttr
 ***********************************************************************/

#include "KKSValue.h"
#include "KKSAttribute.h"
#include "KKSAttrType.h"

int m_defIdRow = -2;

void KKSCategoryAttr::initDefIdRow(){ m_defIdRow = -2;}
int  KKSCategoryAttr::getDefIdRow(){ return m_defIdRow;}
void KKSCategoryAttr::decDefIdRow() {m_defIdRow--;}

////////////////////////////////////////////////////////////////////////
// Name:       KKSCategoryAttr::KKSCategoryAttr()
// Purpose:    Implementation of KKSCategoryAttr::KKSCategoryAttr()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSCategoryAttr::KKSCategoryAttr() : KKSAttribute()
{
    //m_defValue = NULL;
    m_isMandatory = false;
    m_isReadOnly = false;

    m_idRow = getDefIdRow();
    decDefIdRow();

    m_order = 1;
}

KKSCategoryAttr::KKSCategoryAttr(const KKSAttribute & a) : KKSAttribute(a)
{
    //m_defValue = NULL;
    m_isMandatory = false;
    m_isReadOnly = false;

    m_idRow = getDefIdRow();
    decDefIdRow();

    m_order = 1;
}

KKSCategoryAttr::KKSCategoryAttr(const KKSCategoryAttr & a) : KKSAttribute(a)
{
    m_defValue = a.defValue();
    m_isMandatory = a.isMandatory();
    m_isReadOnly = a.isReadOnly();
    m_idRow = a.idRow();
    m_order = a.order();
    m_directives = a.directives();
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSCategoryAttr::~KKSCategoryAttr()
// Purpose:    Implementation of KKSCategoryAttr::~KKSCategoryAttr()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSCategoryAttr::~KKSCategoryAttr()
{
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSCategoryAttr::getIsMandatory()
// Purpose:    Implementation of KKSCategoryAttr::getIsMandatory()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool KKSCategoryAttr::isMandatory(void) const
{
   return m_isMandatory;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSCategoryAttr::setIsMandatory(bool newIsMandatory)
// Purpose:    Implementation of KKSCategoryAttr::setIsMandatory()
// Parameters:
// - newIsMandatory
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSCategoryAttr::setMandatory(bool newIsMandatory)
{
   m_isMandatory = newIsMandatory;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSCategoryAttr::getIsReadOnly()
// Purpose:    Implementation of KKSCategoryAttr::getIsReadOnly()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool KKSCategoryAttr::isReadOnly(void) const
{
   return m_isReadOnly;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSCategoryAttr::setIsReadOnly(bool newIsReadOnly)
// Purpose:    Implementation of KKSCategoryAttr::setIsReadOnly()
// Parameters:
// - newIsReadOnly
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSCategoryAttr::setReadOnly(bool newIsReadOnly)
{
   m_isReadOnly = newIsReadOnly;
}

const KKSValue & KKSCategoryAttr::defValue() const
{
    return m_defValue;
}

KKSValue & KKSCategoryAttr::defValue()
{
    return m_defValue;
}

void KKSCategoryAttr::setDefValue(const KKSValue & _defValue)
{
    m_defValue = _defValue;
}

KKSCategoryAttr * KKSCategoryAttr::create(KKSAttribute * a, 
                                          bool bMandatory, 
                                          bool bReadOnly, 
                                          const QString & defValue,
                                          int order,
                                          const QString & directives,
                                          bool * bBadValue)
{
    KKSCategoryAttr * ca = NULL;
    bool badValue = false;

    ca = new KKSCategoryAttr(*a);
    ca->setMandatory(bMandatory);
    ca->setReadOnly(bReadOnly);
    ca->setOrder(order);
    ca->setDirectives(directives);

    KKSValue v;
    v.setValue(defValue, a->type()->attrType());
    ca->setDefValue(v);

    if(!v.isValid())
        badValue = true;
    
    if(bBadValue)
        * bBadValue = badValue;
    
    return ca;
}

int KKSCategoryAttr::idRow() const
{
    return m_idRow;
}

int KKSCategoryAttr::idRow()
{
    return m_idRow;
}

void KKSCategoryAttr::setIdRow(int id)
{
    m_idRow = id;
}

int KKSCategoryAttr::idParent() const 
{
    return m_idParent;
}

int KKSCategoryAttr::idParent() 
{
    return m_idParent;
}

void KKSCategoryAttr::setIdParent(int id)
{
    m_idParent = id;
}

int KKSCategoryAttr::order() const
{
    return m_order;
}

void KKSCategoryAttr::setOrder(int o)
{
    m_order = o;
}

const QString & KKSCategoryAttr::directives() const
{
    return m_directives;
}

void KKSCategoryAttr::setDirectives(const QString & d)
{
    m_directives = d;
}
