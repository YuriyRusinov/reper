/***********************************************************************
 * Module:  KKSTimeEdit.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:31
 * Purpose: Implementation of the class KKSTimeEdit
 ***********************************************************************/

#include "KKSTimeEdit.h"
#include "KKSAttrValue.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSTime::KKSTime()
// Purpose:    Implementation of KKSTime::KKSTime()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSTimeEdit :: KKSTimeEdit (const KKSAttrValue *attr, bool isSys, QWidget* parent) :
    QTimeEdit (parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (timeChanged (const QTime&)), this, SLOT (setTime (const QTime&)) );
}

KKSTimeEdit :: KKSTimeEdit (const KKSAttrValue *attr, bool isSys, const QTime& time, QWidget * parent)
    : QTimeEdit (time, parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (timeChanged (const QTime&)), this, SLOT (setTime (const QTime&)) );
}

KKSTimeEdit :: ~KKSTimeEdit (void)
{
}

void KKSTimeEdit :: setTime (const QTime& dt)
{
    QVariant v (dt);
    emit valueChanged (m_av->id(), m_isSystem, v);
}
