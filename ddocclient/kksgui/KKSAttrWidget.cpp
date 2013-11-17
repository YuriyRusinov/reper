/***********************************************************************
 * Module:  KKSAttrWidget.cpp
 * Author:  sergey
 * Modified: 23 марта 20128 г. 12:13:26
 * Purpose: Implementation of the class KKSAttrWidget
 ***********************************************************************/

#include "KKSAttrValue.h"
#include "KKSAttrWidget.h"


KKSAttrWidget :: KKSAttrWidget (const KKSAttrValue * av, KKSIndAttrClass isSys):
    m_av (0),
    m_isSystem (isSys)
{
    setAttrValue(av);
}

KKSAttrWidget :: ~KKSAttrWidget (void)
{
    if (m_av)
        m_av->release ();
}

void KKSAttrWidget :: setAttrValue(const KKSAttrValue * av)
{
    if(m_av)
        m_av->release();

    m_av = av;

    if(m_av)
        m_av->addRef();
}

const KKSAttrValue * KKSAttrWidget:: attrValue() const
{
    return m_av;
}
