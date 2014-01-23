/* 
 * File:   KKSHistDrawWidget.h
 * Author: yuriy
 *
 * Created on 23 Декабрь 2013 г., 22:18
 */

#ifndef KKSHISTDRAWWIDGET_H
#define	KKSHISTDRAWWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPair>
#include <QVector>
#include <QColor>

#include "kksgui_config.h"

class QPaintEvent;

class KKSCharts;

class _GUI_EXPORT KKSHistDrawWidget : public QWidget
{
public:
    KKSHistDrawWidget(QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSHistDrawWidget();

    void setData (const QMap<int, QPair<double, double> >& hData);
	virtual QSize sizeHint() const;

protected:
    //
    // Override functions of basic class
    //
    void paintEvent(QPaintEvent *event);

private:
    //
    // Variables
    //
    KKSCharts * wCharts;
    QMap<int, QPair<double, double> > m_data;
    double maxVal;
    QVector<QColor> colors;

private:
    Q_OBJECT

};

#endif	/* KKSHISTDRAWWIDGET_H */

