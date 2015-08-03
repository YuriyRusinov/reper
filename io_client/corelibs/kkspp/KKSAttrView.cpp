/***********************************************************************
 * Module:  KKSAttrView.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:12:32
 * Purpose: Implementation of the class KKSAttrView
 ***********************************************************************/

#include "KKSValue.h"
#include "KKSAttrView.h"


KKSAttrView::KKSAttrView() : KKSCategoryAttr()
{
    m_visible = true;
    m_order = 0;
    m_readOnly = false;
    m_idViewAttr = -1;
}

KKSAttrView::KKSAttrView(const KKSAttrView & av) : KKSCategoryAttr(av)
{
    m_visible = av.isVisible();
    m_order = av.order();
    m_readOnly = av.isReadOnly();
    m_defValue = av.defValue();
    m_idViewAttr = av.idViewAttr();
}

KKSAttrView::KKSAttrView(const KKSCategoryAttr & ca) : KKSCategoryAttr(ca)
{
    m_visible = true;
    m_order = 0;
    m_readOnly = ca.isReadOnly();
    m_defValue = ca.defValue();
    m_idViewAttr = -1;
}

KKSAttrView::~KKSAttrView()
{
}



////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrView::getIsReadOnly()
// Purpose:    Implementation of KKSAttrView::getIsReadOnly()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool KKSAttrView::isReadOnly(void) const
{
   return m_readOnly;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrView::setIsReadOnly(bool newIsReadOnly)
// Purpose:    Implementation of KKSAttrView::setIsReadOnly()
// Parameters:
// - newIsReadOnly
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSAttrView::setReadOnly(bool newIsReadOnly)
{
   m_readOnly = newIsReadOnly;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrView::getOrder()
// Purpose:    Implementation of KKSAttrView::getOrder()
// Return:     int
////////////////////////////////////////////////////////////////////////

int KKSAttrView::order(void) const
{
   return m_order;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrView::setOrder(int newOrder)
// Purpose:    Implementation of KKSAttrView::setOrder()
// Parameters:
// - newOrder
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSAttrView::setOrder(int newOrder)
{
   m_order = newOrder;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrView::getIsVisible()
// Purpose:    Implementation of KKSAttrView::getIsVisible()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool KKSAttrView::isVisible(void) const
{
   return m_visible;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrView::setIsVisible(bool newIsVisible)
// Purpose:    Implementation of KKSAttrView::setIsVisible()
// Parameters:
// - newIsVisible
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSAttrView::setVisible(bool newIsVisible)
{
   m_visible = newIsVisible;
}

void KKSAttrView::setDefValue(const KKSValue & v)
{
    m_defValue = v;
}

const KKSValue & KKSAttrView::defValue() const
{
    return m_defValue;
}

KKSValue & KKSAttrView::defValue()
{
    return m_defValue;
}

int KKSAttrView::idViewAttr() const
{
    return m_idViewAttr;
}

int KKSAttrView::idViewAttr()
{
    return m_idViewAttr;
}

void KKSAttrView::setIdViewAttr(int id)
{
    m_idViewAttr = id;
}

