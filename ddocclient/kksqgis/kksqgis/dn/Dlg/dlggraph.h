#ifndef DLGGRAPH_H
#define DLGGRAPH_H

#include <QDialog>
#include <QList>
#include <QPoint>
#include <QPainter>
#include <QMessageBox>


struct OsParam
{
 int KolvoDel;
 float CenaDel;
 float MaxZnach;
};

struct DataGraph
{
 QList <float> XValue;
 QList <float> YValue;
};

namespace Ui {
class DlgGraph;
}

class DlgGraph : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgGraph(QWidget *parent = 0);
    ~DlgGraph();

    QList <DataGraph> Graphs;

    void AddGraph(float *xv,float *yv, int n);

protected:
    virtual void paintEvent(QPaintEvent *pe);
    virtual void resizeEvent(QResizeEvent*);
    
private:
    Ui::DlgGraph *ui;
    OsParam OsX;
    OsParam OsY;

    int CurrentGraph;
    QList <QPoint> ptGraph;
    QList <int> RCol;
    QList <int> GCol;
    QList <int> BCol;

    int XRec1,XRec2,YRec1,YRec2;
    int WGraph,HGraph;
    int DBottom,DLeft;
    float XKof,YKof;
    float MaxXVal;
    float MaxYVal;

    OsParam Okrugl(float var); //Определение параметров оси
};

#endif // DLGGRAPH_H
