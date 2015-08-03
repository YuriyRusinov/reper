#include "KKSIndicator.h"
#include "KKSAttrType.h"


KKSIndicator :: KKSIndicator (int id, const QString & name, const QString & description)
: KKSIndAttr (id, name, description),
    m_parent (0),
    m_indType(0),
    m_childIndicators (KKSMap<int, KKSIndicator*>())
{
}

KKSIndicator :: KKSIndicator (const KKSIndicator & Ind) 
:   KKSIndAttr (Ind),
    m_parent(Ind.m_parent),
    m_indType(Ind.m_indType),
    m_childIndicators (Ind.m_childIndicators)
{
}

KKSIndicator :: ~KKSIndicator ()
{
    if(m_parent)
        m_parent->release();

    if(m_indType)
        m_indType->release();

}
const KKSMap<int, KKSIndicator*>& KKSIndicator :: getChildIndicators (void) const
{
    return m_childIndicators;
}

KKSMap<int, KKSIndicator*>& KKSIndicator :: getChildIndicators (void)
{
    return m_childIndicators;
}

void KKSIndicator :: setChildIndicators (const KKSMap<int, KKSIndicator*>& childInds)
{
    m_childIndicators = childInds;
}

KKSIndicator * KKSIndicator :: parent ()
{
    return m_parent;
}

const KKSIndicator * KKSIndicator :: parent () const
{
    return m_parent;
}

void KKSIndicator :: setParent (KKSIndicator * parent)
{
    if(m_parent)
        m_parent->release();

    m_parent = parent;
    if(m_parent)
        m_parent->addRef();
}

KKSIndicatorType * KKSIndicator :: indType ()
{
    return m_indType;
}

const KKSIndicatorType * KKSIndicator :: indType () const
{
    return m_indType;
}

void KKSIndicator :: setIndType (KKSIndicatorType * type)
{
    if(m_indType)
        m_indType->release();

    m_indType = type;
    if(m_indType)
        m_indType->addRef();
}

bool KKSIndicator :: isAttribute (void) const
{
    return false;
}