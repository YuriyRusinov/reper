/***********************************************************************
 * Module:  KKSDateTimeEdit.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:31
 * Purpose: Implementation of the class KKSDateTime
 ***********************************************************************/

#include "KKSDateTimeEdit.h"
#include "KKSAttrValue.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSDateTime::KKSDateTime()
// Purpose:    Implementation of KKSDateTime::KKSDateTime()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSDateTimeEdit :: KKSDateTimeEdit (const KKSAttrValue *attr, int isSys, QWidget* parent) :
    QDateTimeEdit (parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (dateTimeChanged (const QDateTime&)), this, SLOT (setDateTime (const QDateTime&)) );
}

KKSDateTimeEdit :: KKSDateTimeEdit (const KKSAttrValue *attr, int isSys, const QDateTime& datetime, QWidget *parent)
    : QDateTimeEdit (datetime, parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (dateTimeChanged (const QDateTime&)), this, SLOT (setDateTime (const QDateTime&)) );
}

KKSDateTimeEdit :: KKSDateTimeEdit (const KKSAttrValue *attr, int isSys, const QDate& date, QWidget *parent)
    : QDateTimeEdit (date, parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (dateTimeChanged (const QDateTime&)), this, SLOT (setDateTime (const QDateTime&)) );
}

KKSDateTimeEdit :: KKSDateTimeEdit (const KKSAttrValue *attr, int isSys, const QTime& time, QWidget * parent)
    : QDateTimeEdit (time, parent), KKSAttrWidget(attr, isSys)
{
    connect (this, SIGNAL (dateTimeChanged (const QDateTime&)), this, SLOT (setDateTime (const QDateTime&)) );
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSDateTime::~KKSDateTime()
// Purpose:    Implementation of KKSDateTime::~KKSDateTime()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSDateTimeEdit :: ~KKSDateTimeEdit (void)
{
}

void KKSDateTimeEdit :: setDateTime (const QDateTime& dt)
{
    QVariant v (dt);
    emit valueChanged (m_av->id(), m_isSystem, v);
}
