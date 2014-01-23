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
    m_data (QMap<int, QPair<double, double> >()),
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
    QWidget::paintEvent (event);
    QPainter painter;
    painter.begin(this);
    
    wCharts = new KKSCharts;
    wCharts->setType(KKSCharts::Histogramm);

	int w1 = width();
	int h1 = height();
	wCharts->setCords(20, 20, w1-40, h1-40);

    int i = 0;
    int n = colors.size();

    for (QMap<int, QPair<double, double> >::const_iterator p=m_data.constBegin();
            p != m_data.constEnd();
            p++)
    {
        QPair<double, double> pair = p.value();
        wCharts->addPiece(QString::number(pair.first), colors[i%n], pair.second/maxVal*100);
        i++;
    }
    
    wCharts->draw(&painter);
    painter.end();
    
    delete wCharts;
    wCharts = 0;
}

QSize KKSHistDrawWidget :: sizeHint (void) const
{
    if (parentWidget())
        return parentWidget()->size();
    
	return QWidget::sizeHint ();
}

void KKSHistDrawWidget::setData (const QMap<int, QPair<double, double> >& hData)
{
    m_data = hData;
    maxVal = 0.0;
    for (QMap<int, QPair<double, double> >::const_iterator p=m_data.constBegin();
            p != m_data.constEnd();
            p++)
    {
        maxVal = qMax (maxVal, p.value ().second);
    }

    //show ();
}
