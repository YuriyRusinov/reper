/***********************************************************************
 * Module:  KKSTimeEdit.cpp
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:13:31
 * Purpose: Implementation of the class KKSTimeEdit
 ***********************************************************************/

#include "KKSTimeEdit.h"
#include "KKSAttrValue.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSTime::KKSTime()
// Purpose:    Implementation of KKSTime::KKSTime()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSTimeEdit :: KKSTimeEdit (const KKSAttrValue *attr, KKSIndAttrClass isSys, QWidget* parent) :
    QTimeEdit (parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (timeChanged (const QTime&)), this, SLOT (setTime (const QTime&)) );
}

KKSTimeEdit :: KKSTimeEdit (const KKSAttrValue *attr, KKSIndAttrClass isSys, const QTime& time, QWidget * parent)
    : QTimeEdit (parent), KKSAttrWidget(attr, isSys)
{
	if(time.isValid())
		QTimeEdit::setTime(time);

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
