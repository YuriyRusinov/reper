/* 
 * File:   KKSHistDrawWidget.cpp
 * Author: yuriyrusinov
 * 
 * Created on 23 Декабрь 2013 г., 22:18
 */
#include <QPaintEvent>
#include <QtDebug>

#include "KKSHistDrawWidget.h"
#include "KKSCharts.h"

KKSHistDrawWidget::KKSHistDrawWidget(QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    wCharts (0),//new KKSCharts),
    m_data (QMap<int, double>()),
    maxVal (-1.0)
{
    colors.clear();
    colors << QColor(Qt::black)
           << QColor(Qt::red)
           << QColor(Qt::darkRed)
           << QColor(Qt::green)
           << QColor(Qt::darkGreen)
           << QColor(Qt::blue)
           << QColor(Qt::darkBlue)
           << QColor(Qt::cyan)
           << QColor(Qt::darkCyan)
           << QColor(Qt::magenta)
           << QColor(Qt::darkMagenta)
           << QColor(Qt::yellow)
           << QColor(Qt::darkYellow)
           << QColor(Qt::gray)
           << QColor(Qt::darkGray)
           << QColor(Qt::lightGray);
}

KKSHistDrawWidget::~KKSHistDrawWidget()
{
}

void KKSHistDrawWidget::paintEvent(QPaintEvent *event)
{
    //qDebug () << __PRETTY_FUNCTION__ ;
    QWidget::paintEvent (event);
    QPainter painter;
    painter.begin(this);
    wCharts = new KKSCharts;
    wCharts->setType(KKSCharts::Histogramm);
    wCharts->setCords(100,100,this->width()/1.5,this->height()/1.5);
    int i = 0;
    int n = colors.size();
    for (QMap<int, double>::const_iterator p=m_data.constBegin();
            p != m_data.constEnd();
            p++)
    {
        wCharts->addPiece(QString::number(p.key()), colors[i%n], p.value()/maxVal*100);
        i++;
    }
    wCharts->draw(&painter);
    painter.end();
    delete wCharts;
    wCharts = 0;
}

void KKSHistDrawWidget::setData (const QMap<int, double>& hData)
{
    m_data = hData;
    maxVal = 0.0;
    for (QMap<int, double>::const_iterator p=m_data.constBegin();
            p != m_data.constEnd();
            p++)
    {
        maxVal = qMax (maxVal, p.value ());
    }
    show ();
}
