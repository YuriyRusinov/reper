#ifndef DLGSPECGRAPH_H
#define DLGSPECGRAPH_H

#include <QDialog>
#include <QQueue>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QTextEdit>
#include <QPoint>
#include <QRegExp>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include "Added/dnpoly2.h"
#include "Added/First/dnmathadd.h"

struct ParamOs
{
 int KolvoDel;
 float CenaDel;
 float MaxZnach;
};

namespace Ui {
class DlgSpecGraph;
}

class DlgSpecGraph : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgSpecGraph(QWidget *parent = 0);
    ~DlgSpecGraph();

    QQueue <float*> SpecGraphics;
    QQueue <float*> GraphBrigth;
    QQueue <float*> GraphSKO;
    QQueue <float*> GraphEntrop;

    QQueue <int> RCol;
    QQueue <int> GCol;
    QQueue <int> BCol;

    int Ch;
    int CurrentVal;
    float MinSpec,MaxSpec;
    float XKof,YKof;
    int XRec1,XRec2,YRec1,YRec2;
    int WGraph,HGraph;
    int DBottom,DLeft; //–ассто€ние точки начала координат от низа окна и левого кра€ окна

   DNPoly2 *ThisPoly;


public slots:
    void MouseDoubleCliced(int x,int y);

protected:
    virtual void paintEvent(QPaintEvent *pe);

private:
    Ui::DlgSpecGraph *ui;
    DNMathAdd Math;
    ParamOs OsX;
    ParamOs OsY;

    QQueue <QPoint> ptPoly;

    int CurrentGraph;

    float MaxThisSpec();
    ParamOs Okrugl(float var); //ќпределение параметров оси
    void FillStackGraph();
    void AddStackGraph(int x, int y);


protected:
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *);
private slots:
    void on_comboTypeValue_currentIndexChanged(int index);
    void on_ListPolygon_currentRowChanged(int currentRow);
//    void on_ButtonReCalc_clicked();
    void on_pushButton_clicked();
};

#endif // DLGSPECGRAPH_H
