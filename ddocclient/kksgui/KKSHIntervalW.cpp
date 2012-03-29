/***********************************************************************
 * Module:  KKSHIntervalW.h
 * Author:  yuriy
 * Modified: 25 но€бр€ 2010 г. 13:20
 * Purpose: Implementation of the class KKSHIntervalW
 ***********************************************************************/

#include <QSpinBox>
#include <QHBoxLayout>

#include <KKSAttrValue.h>

#include "KKSHIntervalW.h"

KKSHIntervalW :: KKSHIntervalW (const KKSAttrValue *attr, bool isSys, QWidget * parent, Qt::WindowFlags f)
    : QWidget (parent, f), KKSAttrWidget(attr, isSys),
    spHour (new QSpinBox (this)),
    spMinute (new QSpinBox (this)),
    spSecond (new QSpinBox (this)),
    hLay (new QHBoxLayout (this))
{

    hLay->addWidget (spHour);
    hLay->addWidget (spMinute);
    hLay->addWidget (spSecond);

    spHour->setMinimum (0);
    spMinute->setRange (0, 59);
    spSecond->setRange (0, 59);

    connect (spHour, SIGNAL (valueChanged(int)), this, SLOT (hourChanged (int)) );
    connect (spMinute, SIGNAL (valueChanged(int)), this, SLOT (minChanged (int)) );
    connect (spSecond, SIGNAL (valueChanged(int)), this, SLOT (secChanged (int)) );
}

KKSHIntervalW :: ~KKSHIntervalW (void)
{
}

void KKSHIntervalW :: setValue (int h, int m, int s)
{
    if (h<0 || m<0 || s<0)
    {
        spHour->setValue (0);
        spMinute->setValue (0);
        spSecond->setValue (0);
    }
    else
    {
        disconnect (spHour);
        disconnect (spMinute);
        disconnect (spSecond);
        spHour->setValue (h);
        spMinute->setValue (m);
        spSecond->setValue (s);
        connect (spHour, SIGNAL (valueChanged(int)), this, SLOT (hourChanged (int)) );
        connect (spMinute, SIGNAL (valueChanged(int)), this, SLOT (minChanged (int)) );
        connect (spSecond, SIGNAL (valueChanged(int)), this, SLOT (secChanged (int)) );
    }
}

void KKSHIntervalW :: hourChanged (int hour)
{
    int m = spMinute->value();
    int s = spSecond->value();
    QString val=QString("{%1,%2,%3}").arg (hour).arg (m).arg (s);
    emit valueChanged (m_av->id(), m_isSystem, QVariant(val));
}

void KKSHIntervalW :: minChanged (int min)
{
    int h = spHour->value();
    int s = spSecond->value();
    QString val=QString("{%1,%2,%3}").arg (h).arg (min).arg (s);
    emit valueChanged (m_av->id(), m_isSystem, QVariant(val));
}

void KKSHIntervalW :: secChanged (int sec)
{
    int h = spHour->value();
    int m = spMinute->value();
    QString val=QString("{%1,%2,%3}").arg (h).arg (m).arg (sec);
    emit valueChanged (m_av->id(), m_isSystem, QVariant(val));
}
