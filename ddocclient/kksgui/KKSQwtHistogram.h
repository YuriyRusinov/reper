/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef KKSQWTHISTOGRAM_H
#define KKSQWTHISTOGRAM_H

#include "kksgui_config.h"

#include <qglobal.h>
#include <qcolor.h>
#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_item.h>


class QwtIntervalData;
class QString;
class KKSHistogram;

class _GUI_EXPORT KKSQwtHistogramItem: public QwtPlotItem
{
public:
    explicit KKSQwtHistogramItem(const QString &title = QString::null);
    explicit KKSQwtHistogramItem(const QwtText &title);
    virtual ~KKSQwtHistogramItem();

    void setData(const QwtIntervalData &data);
    const QwtIntervalData &data() const;

    void setColor(const QColor &);
    QColor color() const;

    virtual QwtDoubleRect boundingRect() const;

    virtual int rtti() const;

    virtual void draw(QPainter *, const QwtScaleMap &xMap, 
        const QwtScaleMap &yMap, const QRect &) const;

    void setBaseline(double reference);
    double baseline() const;

    enum KKSQwtHistogramAttribute
    {
        Auto = 0,
        Xfy = 1
    };

    void setHistogramAttribute(KKSQwtHistogramAttribute, bool on = true);
    bool testHistogramAttribute(KKSQwtHistogramAttribute) const;

protected:
    virtual void drawBar(QPainter *,
        Qt::Orientation o, const QRect &) const;

private:
    void init();

    class PrivateData;
    PrivateData *d_data;
};


class _GUI_EXPORT KKSQwtPlotWidget : public QWidget //public QwtPlot
{
private:
    Q_OBJECT

public:
    KKSQwtPlotWidget(const QString & title, 
                     KKSHistogram * hist,
                     QWidget * parent, 
                     Qt::WindowFlags flags=0);
    ~KKSQwtPlotWidget();

    void init(KKSHistogram * hist);

signals:
    void created();

private:
    QwtPlot plot;
    KKSHistogram * m_histogramParams;
    KKSQwtHistogramItem * m_histogram;
};

#endif
