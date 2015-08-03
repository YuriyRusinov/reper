/***********************************************************************
 * Module:  KKSAttrAttr.cpp
 * Author:  sergey
 * Modified: 02 04 2012 ã. 12:12:50
 * Purpose: Implementation of the class KKSAttrAttr
 ***********************************************************************/

#include "KKSValue.h"
#include "KKSAttribute.h"
#include "KKSAttrType.h"

int m_defIdParent = -2;

void KKSAttrAttr::initDefIdParent(){ m_defIdParent = -2;}
int  KKSAttrAttr::getDefIdParent(){ return m_defIdParent;}
void KKSAttrAttr::decDefIdParent() {m_defIdParent--;}


KKSAttrAttr::KKSAttrAttr() : KKSAttribute()
{
    m_idParentAttr = -1;
    m_rowId = KKSAttrAttr::getDefIdParent();
    KKSAttrAttr::decDefIdParent();
    
    m_isMandatory = false;
    m_isReadOnly = false;
}

KKSAttrAttr::KKSAttrAttr(const KKSAttribute & a) : KKSAttribute(a)
{
    m_idParentAttr = -1;
    m_rowId = KKSAttrAttr::getDefIdParent();
    KKSAttrAttr::decDefIdParent();;

    m_isMandatory = false;
    m_isReadOnly = false;
}

KKSAttrAttr::KKSAttrAttr(const KKSAttrAttr& a) : KKSAttribute(a)
{
    m_rowId = a.idAttrAttr();
    m_idParentAttr = a.idParentAttr();
    
    m_defValue = a.defValue();
    m_isMandatory = a.isMandatory();
    m_isReadOnly = a.isReadOnly();
}

KKSAttrAttr::~KKSAttrAttr()
{
}

bool KKSAttrAttr::isMandatory(void) const
{
   return m_isMandatory;
}

void KKSAttrAttr::setMandatory(bool newIsMandatory)
{
   m_isMandatory = newIsMandatory;
}


bool KKSAttrAttr::isReadOnly(void) const
{
   return m_isReadOnly;
}

void KKSAttrAttr::setReadOnly(bool newIsReadOnly)
{
   m_isReadOnly = newIsReadOnly;
}

const KKSValue & KKSAttrAttr::defValue() const
{
    return m_defValue;
}

KKSValue & KKSAttrAttr::defValue()
{
    return m_defValue;
}

void KKSAttrAttr::setDefValue(const KKSValue & _defValue)
{
    m_defValue = _defValue;
}

int KKSAttrAttr::idParentAttr() const 
{
    return m_idParentAttr;
}

int KKSAttrAttr::idParentAttr() 
{
    return m_idParentAttr;
}

void KKSAttrAttr::setIdParentAttr(int id)
{
    m_idParentAttr = id;
}

int KKSAttrAttr::idAttrAttr() const 
{
    return m_rowId;
}

int KKSAttrAttr::idAttrAttr() 
{
    return m_rowId;
}

void KKSAttrAttr::setIdAttrAttr(int id)
{
    m_rowId = id;
}

KKSAttrAttr* KKSAttrAttr::create(KKSAttribute * a, 
                                 bool bMandatory, 
                                 bool bReadOnly, 
                                 const QString & defValue,
                                 bool * bBadValue)
{
    KKSAttrAttr * aa = NULL;
    bool badValue = false;

    aa = new KKSAttrAttr(*a);
    aa->setMandatory(bMandatory);
    aa->setReadOnly(bReadOnly);
    KKSValue v;
    v.setValue(defValue, a->type()->attrType());
    aa->setDefValue(v);

    //aa->m_idParentAttr = KKSAttrAttr::getDefIdParent();
    //KKSAttrAttr::decDefIdParent();;

    if(!v.isValid())
        badValue = true;
    
    if(bBadValue)
        * bBadValue = badValue;
    
    return aa;
}

