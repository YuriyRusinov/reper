/*
 * File:   KKSHistWidget.cpp
 * Author: yuriyrusinov
 *
 * Created on 12 Декабрь 2013 г., 16:57
 */

#include <QPainter>

#include <KKSHistogram.h>
#include <KKSAttrValue.h>
#include "KKSHistWidget.h"
#include "KKSValue.h"

KKSHistWidget::KKSHistWidget(const KKSAttrValue *av, KKSIndAttrClass isSys, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags), KKSAttrWidget(av, isSys)
{
    
}

KKSHistWidget::~KKSHistWidget()
{
}

void KKSHistWidget::paintEvent(QPaintEvent *event)
{
    QVariant val = m_av->value().valueVariant();
    KKSHistogram m_hist = val.value<KKSHistogram>();
    if (m_hist.isEmpty())
    {
        QWidget::paintEvent(event);
        return;
    }

    QPainter painter(this);
    int n = m_hist.size();
    qreal barWidth = width() / (qreal)n;
    int h = height ();
    QMap<int, double> m_h = m_hist.getVec();
    int i=0;
    for (QMap<int, double>::const_iterator p = m_h.constBegin(); p != m_h.constEnd(); p++)
    {
        qreal y = p.value() * h;
        // draw level
        painter.fillRect(barWidth * i, h - y, barWidth * (i + 1), h, Qt::red);
        // clear the rest of the control
        painter.fillRect(barWidth * i, 0, barWidth * (i + 1), h - y, Qt::black);
        i++;
    }
}