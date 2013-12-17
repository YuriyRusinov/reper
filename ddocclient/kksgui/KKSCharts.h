/*
 * NightCharts
 * Copyright (C) 2010 by Alexander A. Avdonin, Artem N. Ivanov / ITGears Co.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * Please contact gordos.kund@gmail.com with any questions on this license.
 */
#ifndef KKSCHARTS_H
#define KKSCHARTS_H
#define PerConst = 3.6;
#include <QObject>
#include <QPainter>
#include <QVector>
#include <QDebug>
#include <QLinearGradient>
#include <math.h>

#include "kksgui_config.h"


class _GUI_EXPORT pieceNC
{
public:
    explicit pieceNC();
    void addName(QString name);
    void setColor(Qt::GlobalColor);
    void setColor(QColor color);
    void setPerc(float Percentage);

    QString pname;
    QColor rgbColor;
    float pPerc;

private:

};

class _GUI_EXPORT KKSCharts
{
public:

    explicit KKSCharts();
    ~KKSCharts();
    enum type { Histogramm , Pie, Dpie };
    enum legend_type{ /*Horizontal,*/ Vertical, Round };
    int addPiece(QString name,Qt::GlobalColor,float Percentage);
    int addPiece(QString name,QColor, float Percentage);
    int setCords(double x, double y, double w, double h);
    int setLegendCords(double x, double y);
    int setType(KKSCharts::type t);
    int setLegendType(KKSCharts::legend_type t);
    int setShadows(bool ok = true);
    int setFont(QFont f);
    int draw(QPainter *painter);
    int drawLegend(QPainter *painter);
    double palpha;

private:
    double cX,cY,cW,cH,pW,lX,lY;
    int nPiece;
    bool shadows;
    QVector<pieceNC> pieces;
    int ctype, cltype;
    QFont font;
    //QPainter *cpainter;
    QPointF GetPoint(double angle, double R1 = 0, double R2 = 0);
    int GetQuater(double angle);
    double Angle360(double angle);


signals:

public slots:

};

/*

Example :
void MainWindow::paintEvent(QPaintEvent e*)
{
QWidget::paintEvent(e);
QPainter painter;
QFont font;
painter.begin(this);
KKSCharts PieChart;
PieChart.setType(Nightcharts::DPie);//{Histogramm,Pie,DPie};
PieChart.setLegendType(Nightcharts::Round);//{Round,Vertical}
PieChart.setCords(100,100,this->width()/1.5,this->height()/1.5);
PieChart.addPiece("Item1",QColor(200,10,50),34);
PieChart.addPiece("Item2",Qt::green,27);
PieChart.addPiece("Item3",Qt::cyan,14);
PieChart.addPiece("Item4",Qt::yellow,7);
PieChart.addPiece("Item5",Qt::blue,4);
PieChart.draw(&painter);
PieChart.drawLegend(&painter);
}

Updated : remove memory leak;

Initial angle can be set. so if you change it you can achieve the effect of motion of pie on its axis.

var : palpha = ;

Updated : Now the vertical legend has its own coordinates.
Use KKSCharts::setLegendCords(double x,double y);
Also changed rendering 3d Pie by removing the black frame. His screenshot was updated.

Updated : Added new option : KKSCharts::setShadows(bool);
Also changed rendering histogram and normal pie. Screenshots was updated.

Updated(2010/09/16) : Now when you create object does not need to pass QPainter; it is transmitted only in the methods draw/drawLegend. Also removed all memory leaks.
*/


#endif // NIGHTCHARTS_H
