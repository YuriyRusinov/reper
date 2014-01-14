/* 
 * File:   KKSHistDrawWidget.cpp
 * Author: yuriyrusinov
 * 
 * Created on 23 Декабрь 2013 г., 22:18
 */
#include <QPaintEvent>

#include "KKSHistDrawWidget.h"
#include "KKSCharts.h"

KKSHistDrawWidget::KKSHistDrawWidget(QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    wCharts (new KKSCharts),
    m_data (QMap<int, double>()),
    maxVal (-1.0)
{
}

KKSHistDrawWidget::~KKSHistDrawWidget()
{
}

void KKSHistDrawWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent (event);
}

void KKSHistDrawWidget::setData (const QMap<int, double>& hData)
{
    m_data = hData;
    int i=0;
    for (QMap<int, double>::const_iterator p=m_data.constBegin();
            p != m_data.constEnd();
            p++)
    {
        if (i==0)
            maxVal = p.value ();
        else
            maxVal = qMax (maxVal, p.value());
        i++;
        
    }
}
