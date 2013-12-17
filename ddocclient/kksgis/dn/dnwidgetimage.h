#ifndef DNWIDGETIMAGE_H
#define DNWIDGETIMAGE_H
#include <QPainter>
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QList>
#include <QMessageBox>

class DNImgPoly
{
/*������� ��������*/
public:
    DNImgPoly();
    QPoint GetMinP();
    QPoint GetMaxP();
    void SelectPolygon();

//    void PolygonToFile(QString FileName); //������ � ������ ������ �����

private:
    bool IsPointInside(int xp,int yp);


/*�������� ��������*/
public:
    int /*KolvoPix,*/minX,minY,maxX,maxY;
    int *SelPoly; //������� �����������
    QList <QPoint> pt; //���������� ����� �������� � �������� ��������
    QPoint Point;
    bool IsPolygonSelect;

    bool IsPolyClassif; //����������� �� � �������� ������ ������������� (�� ��������� FALSE)
    int *ClassifMass; //������ �������������
};

class DNWidgetImage : public QWidget
{
    Q_OBJECT
public:
    explicit DNWidgetImage(QWidget *parent = 0);

    void ChangedBMP(/*QString PathToBMP*/);
    bool isImgLoad; //��������� �� ��������
    bool IsCretaePolyOn; //������� �� ����� �������� ��������
    bool IsSelPoly; //������� �� �������


    int ViewAreaX,ViewAreaY; //������� ������� ������� ���� ��������� �����������
    QList <DNImgPoly> Polygons; //������ ���������
    DNImgPoly Polygon; //������� �������
//    PropPolygonSel Polygon; //������� �������
//    QPoint Point;
//    int KolvoPixPoly;
//    int ProzrPol; //�������������� ������������ ��������
    QImage img;
    QImage imgScale;
protected:
    virtual void paintEvent(QPaintEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent* mEvent);
    virtual void mousePressEvent(QMouseEvent* mEvent);
//    virtual void mouseDoubleClickEvent(QMouseEvent *);

private:
    float NewKof; //����������� ������� �����������
    int MouseX,MouseY;
    int SliderX,SliderY;
    int ColorClassR[50],ColorClassG[50],ColorClassB[50];

    int GetNumCurPoly();
    void DeselectPoly();

signals:
//    void MouseClicked(int x,int y);
//    void MouseDoubleCliced(int x,int y);
      void MouseMove(int x,int y);
      void OnRightButton(bool On);
public slots:
//    void ImgReduc();
//    void ImgIncrease();
//    void ImgZoomScreen();
//    void SelectPoly(bool param);
//    void HSliderMoved(int x);
//    void VSliderMoved(int y);
};

#endif // DNWIDGETIMAGE_H
