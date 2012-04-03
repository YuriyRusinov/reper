/***********************************************************************
 * Module:  KKSCategoryAttr.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:12:50
 * Purpose: Implementation of the class KKSCategoryAttr
 ***********************************************************************/

#include "KKSValue.h"
#include "KKSCategoryAttr.h"
#include "KKSAttrType.h"

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
    m_idCategoryAttr = -1;
}

KKSCategoryAttr::KKSCategoryAttr(const KKSAttribute & a) : KKSAttribute(a)
{
    //m_defValue = NULL;
    m_isMandatory = false;
    m_isReadOnly = false;
    m_idCategoryAttr = -1;
}

KKSCategoryAttr::KKSCategoryAttr(const KKSCategoryAttr & a) : KKSAttribute(a)
{
    m_defValue = a.defValue();
    m_isMandatory = a.isMandatory();
    m_isReadOnly = a.isReadOnly();
    m_idCategoryAttr = a.idCategoryAttr();
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
                                          bool * bBadValue)
{
    KKSCategoryAttr * ca = NULL;
    bool badValue = false;

    ca = new KKSCategoryAttr(*a);
    ca->setMandatory(bMandatory);
    ca->setReadOnly(bReadOnly);
    KKSValue v;
    v.setValue(defValue, a->type()->attrType());
    ca->setDefValue(v);

    if(!v.isValid())
        badValue = true;
    
    if(bBadValue)
        * bBadValue = badValue;
    
    return ca;
}

int KKSCategoryAttr::idCategoryAttr() const
{
    return m_idCategoryAttr;
}

int KKSCategoryAttr::idCategoryAttr()
{
    return m_idCategoryAttr;
}

void KKSCategoryAttr::setIdCategoryAttr(int id)
{
    m_idCategoryAttr = id;
}

