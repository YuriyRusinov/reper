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


class _GUI_EXPORT KKSQwtPlot : public QWidget //public QwtPlot
{
private:
    Q_OBJECT

public:
    KKSQwtPlot(QWidget * parent = NULL);
    ~KKSQwtPlot();

    void init(){}

signals:
    void created();

private:
    QwtPlot plot;
};

#endif
