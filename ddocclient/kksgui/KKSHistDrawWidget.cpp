/* 
 * File:   KKSHistDrawWidget.cpp
 * Author: yuriyrusinov
 * 
 * Created on 23 ������� 2013 �., 22:18
 */
#include <QPaintEvent>

#include "KKSHistDrawWidget.h"
#include "KKSCharts.h"

KKSHistDrawWidget::KKSHistDrawWidget(QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    wCharts (new KKSCharts)
{
}

KKSHistDrawWidget::~KKSHistDrawWidget()
{
}

void KKSHistDrawWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent (event);
}
