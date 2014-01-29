#include "KKSQwtHistogram.h"

#include <qstring.h>
#include <qpainter.h>
#include <qwt_interval_data.h>
#include <qwt_painter.h>
#include <qwt_scale_map.h>
#include <stdlib.h>
#include <qpen.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_interval_data.h>
#include <QGridLayout>
#include <QSizePolicy>
#include <QMap>
#include <QPair>

#include <KKSHistogram.h>

class KKSQwtHistogramItem::PrivateData
{
public:
    int attributes;
    QwtIntervalData data;
    QColor color;
    double reference;
};

KKSQwtHistogramItem::KKSQwtHistogramItem(const QwtText &title):
    QwtPlotItem(title)
{
    init();
}

KKSQwtHistogramItem::KKSQwtHistogramItem(const QString &title):
    QwtPlotItem(QwtText(title))
{
    init();
}

KKSQwtHistogramItem::~KKSQwtHistogramItem()
{
    delete d_data;
}

void KKSQwtHistogramItem::init()
{
    d_data = new PrivateData();
    d_data->reference = 0.0;
    d_data->attributes = KKSQwtHistogramItem::Auto;

    setItemAttribute(QwtPlotItem::AutoScale, true);
    setItemAttribute(QwtPlotItem::Legend, true);

    setZ(20.0);
}

void KKSQwtHistogramItem::setBaseline(double reference)
{
    if ( d_data->reference != reference )
    {
        d_data->reference = reference;
        itemChanged();
    }
}

double KKSQwtHistogramItem::baseline() const
{
    return d_data->reference;
}

void KKSQwtHistogramItem::setData(const QwtIntervalData &data)
{
    d_data->data = data;
    itemChanged();
}

const QwtIntervalData &KKSQwtHistogramItem::data() const
{
    return d_data->data;
}

void KKSQwtHistogramItem::setColor(const QColor &color)
{
    if ( d_data->color != color )
    {
        d_data->color = color;
        itemChanged();
    }
}

QColor KKSQwtHistogramItem::color() const
{
    return d_data->color;
}

QwtDoubleRect KKSQwtHistogramItem::boundingRect() const
{
    QwtDoubleRect rect = d_data->data.boundingRect();
    if ( !rect.isValid() ) 
        return rect;

    if ( d_data->attributes & Xfy ) 
    {
        rect = QwtDoubleRect( rect.y(), rect.x(), 
            rect.height(), rect.width() );

        if ( rect.left() > d_data->reference ) 
            rect.setLeft( d_data->reference );
        else if ( rect.right() < d_data->reference ) 
            rect.setRight( d_data->reference );
    } 
    else 
    {
        if ( rect.bottom() < d_data->reference ) 
            rect.setBottom( d_data->reference );
        else if ( rect.top() > d_data->reference ) 
            rect.setTop( d_data->reference );
    }

    return rect;
}


int KKSQwtHistogramItem::rtti() const
{
    return QwtPlotItem::Rtti_PlotHistogram;
}

void KKSQwtHistogramItem::setHistogramAttribute(KKSQwtHistogramAttribute attribute, bool on)
{
    if ( bool(d_data->attributes & attribute) == on )
        return;

    if ( on )
        d_data->attributes |= attribute;
    else
        d_data->attributes &= ~attribute;

    itemChanged();
}

bool KKSQwtHistogramItem::testHistogramAttribute(KKSQwtHistogramAttribute attribute) const
{
    return d_data->attributes & attribute;
}

void KKSQwtHistogramItem::draw(QPainter *painter, const QwtScaleMap &xMap, 
    const QwtScaleMap &yMap, const QRect &) const
{
    const QwtIntervalData &iData = d_data->data;

    painter->setPen(QPen(d_data->color));

    const int x0 = xMap.transform(baseline());
    const int y0 = yMap.transform(baseline());

    for ( int i = 0; i < (int)iData.size(); i++ )
    {
        if ( d_data->attributes & KKSQwtHistogramItem::Xfy )
        {
            const int x2 = xMap.transform(iData.value(i));
            if ( x2 == x0 )
                continue;

            int y1 = yMap.transform( iData.interval(i).minValue());
            int y2 = yMap.transform( iData.interval(i).maxValue());
            if ( y1 > y2 )
                qSwap(y1, y2);

            if ( i < (int)iData.size() - 2 )
            {
                const int yy1 = yMap.transform(iData.interval(i+1).minValue());
                const int yy2 = yMap.transform(iData.interval(i+1).maxValue());

                if ( y2 == qwtMin(yy1, yy2) )
                {
                    const int xx2 = xMap.transform(
                        iData.interval(i+1).minValue());
                    if ( xx2 != x0 && ( (xx2 < x0 && x2 < x0) ||
                                          (xx2 > x0 && x2 > x0) ) )
                    {
                       // One pixel distance between neighboured bars
                       y2++;
                    }
                }
            }

            drawBar(painter, Qt::Horizontal,
                QRect(x0, y1, x2 - x0, y2 - y1));
        }
        else
        {
            const int y2 = yMap.transform(iData.value(i));
            if ( y2 == y0 )
                continue;

            int x1 = xMap.transform(iData.interval(i).minValue());
            int x2 = xMap.transform(iData.interval(i).maxValue());
            if ( x1 > x2 )
                qSwap(x1, x2);

            if ( i < (int)iData.size() - 2 )
            {
                const int xx1 = xMap.transform(iData.interval(i+1).minValue());
                const int xx2 = xMap.transform(iData.interval(i+1).maxValue());

                if ( x2 == qwtMin(xx1, xx2) )
                {
                    const int yy2 = yMap.transform(iData.value(i+1));
                    if ( yy2 != y0 && ( (yy2 < y0 && y2 < y0) ||
                                    (yy2 > y0 && y2 > y0) ) )
                    {
                        // One pixel distance between neighboured bars
                        x2--;
                    }
                }
            }
            drawBar(painter, Qt::Vertical,
                QRect(x1, y0, x2 - x1, y2 - y0) );
        }
    }
}

void KKSQwtHistogramItem::drawBar(QPainter *painter,
   Qt::Orientation, const QRect& rect) const
{
   painter->save();

   const QColor color(painter->pen().color());
   const QRect r = rect.normalized();

   const int factor = 125;
   const QColor light(color.light(factor));
   const QColor dark(color.dark(factor));

   painter->setBrush(color);
   painter->setPen(Qt::NoPen);
   QwtPainter::drawRect(painter, r.x() + 1, r.y() + 1,
      r.width() - 2, r.height() - 2);
   painter->setBrush(Qt::NoBrush);

   painter->setPen(QPen(light, 2));
   QwtPainter::drawLine(painter,
      r.left() + 1, r.top() + 2, r.right() + 1, r.top() + 2);

   painter->setPen(QPen(dark, 2));
   QwtPainter::drawLine(painter, 
      r.left() + 1, r.bottom(), r.right() + 1, r.bottom());

   painter->setPen(QPen(light, 1));

   QwtPainter::drawLine(painter, 
      r.left(), r.top() + 1, r.left(), r.bottom());
   QwtPainter::drawLine(painter,
      r.left() + 1, r.top() + 2, r.left() + 1, r.bottom() - 1);

   painter->setPen(QPen(dark, 1));

   QwtPainter::drawLine(painter, 
      r.right() + 1, r.top() + 1, r.right() + 1, r.bottom());
   QwtPainter::drawLine(painter, 
      r.right(), r.top() + 2, r.right(), r.bottom() - 1);

   painter->restore();
}



///////////////////*****************************////////////////////////////
    //KKSQwtPlot plot;


KKSQwtPlotWidget::KKSQwtPlotWidget(const QString & title,
                                   KKSHistogram * hist,
                                   QWidget * parent, 
                                   Qt::WindowFlags flags) : QWidget(parent, flags)
{
    m_histogramParams = hist;
    if(m_histogramParams)
        m_histogramParams->addRef();
    
    plot.setCanvasBackground(QColor(Qt::white));
    plot.setTitle(title);
    plot.setParent(this);

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
    grid->attach(&plot);

    m_histogram = new KKSQwtHistogramItem();
    m_histogram->setColor(Qt::darkCyan);

    init(m_histogramParams);

/*
    const int numValues = 20;

    QwtArray<QwtDoubleInterval> intervals(numValues);
    QwtArray<double> values(numValues);

    double pos = 0.0;
    for ( int i = 0; i < (int)intervals.size(); i++ )
    {
        const int width = 5 + rand() % 15;
        const int value = rand() % 100;

        intervals[i] = QwtDoubleInterval(pos, pos + double(width));
        values[i] = value; 

        pos += width;
    }

    histogram->setData(QwtIntervalData(intervals, values));
    histogram->attach(&plot);

    plot.setAxisScale(QwtPlot::yLeft, 0.0, 100.0);
    plot.setAxisScale(QwtPlot::xBottom, 0.0, pos);
    plot.replot();
*/
    QGridLayout * gLay = new QGridLayout(this);
    gLay->addWidget(&plot, 0, 0);
    this->setLayout(gLay);
    
    plot.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

KKSQwtPlotWidget::~KKSQwtPlotWidget()
{
    if(m_histogramParams)
        m_histogramParams->release();
}


void KKSQwtPlotWidget::init(KKSHistogram * hist)
{
    if(!hist || hist->isEmpty()){
        plot.clear();
        plot.replot();
        return;
    }

    if(m_histogramParams)
        m_histogramParams->release();

    m_histogramParams = hist;
    m_histogramParams->addRef();

    const int numValues = m_histogramParams->size();

    QwtArray<QwtDoubleInterval> intervals(numValues);
    QwtArray<double> values(numValues);

    //double pos = 0.0;
    double yMax = 0.0;
    QMap<int, QPair<double, double> > hData = m_histogramParams->getVec();
    for ( int i = 0; i < (int)intervals.size(); i++ )
    {
        QPair<double, double> xyFrom = hData.value(i);
        QPair<double, double> xyTo = hData.value(i+1);

        intervals[i] = QwtDoubleInterval(xyFrom.first, xyTo.first);
        values[i] = xyFrom.second; 
        yMax = yMax < values[i] ? values[i] : yMax;
    }

    m_histogram->setData(QwtIntervalData(intervals, values));
    m_histogram->attach(&plot);

    plot.setAxisScale(QwtPlot::yLeft, 0.0, yMax);
    plot.setAxisScale(QwtPlot::xBottom, m_histogramParams->getXMin(), m_histogramParams->getXMax());
    plot.replot();

}
