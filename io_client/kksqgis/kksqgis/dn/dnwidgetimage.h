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
    ~DNImgPoly();
    QPoint GetMinP();
    QPoint GetMaxP();
    void SelectPolygon();

//    void PolygonToFile(QString FileName); //������ � ������ ������ �����

//private:
    bool IsPointInside(int xp,int yp);
    void CreateImg(float *ImgPolyR,float *ImgPolyG,float *ImgPolyB, int *MassPoly, quint64 px);


/*�������� ��������*/
public:
    int /*KolvoPix,*/minX,minY,maxX,maxY;
    int *SelPoly; //������� �����������
    QList <QPoint> pt; //���������� ����� �������� � �������� ��������
    QPoint Point;
    bool IsPolygonSelect; //���������� �� ����� ��������
    bool IsPolygonCurrent; //���� ������� �������� �������
    bool IsPolyClassif; //����������� �� � �������� ������ ������������� (�� ��������� FALSE)
    bool IsPolyCreateImg; //����������� �� � �������� ������ ��������� ���������
    int *PImgR,*PImgG,*PImgB;
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

    bool IsDlgAIHidden;
    int DlgAiSizeSide;

    int ViewAreaX,ViewAreaY; //������� ������� ������� ���� ��������� �����������
    QList <DNImgPoly> Polygons; //������ ���������
    QList <QPoint> pt; //������ ����� ��� ��������
//    DNImgPoly *Polygon; //������� �������
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
    virtual void mouseDoubleClickEvent(QMouseEvent *);

private:
    float NewKof; //����������� ������� �����������
    int SliderX,SliderY;
    int ColorClassR[50],ColorClassG[50],ColorClassB[50];
    int MouseX,MouseY;
    int XMouseCl,YMouseCl;

    int GetNumCurPoly();
    void DeselectPoly();

signals:
      void MouseLClicked(int x,int y);
      void MouseDoubleCliced(int x,int y);
      void MouseMove(int x,int y);
      void OnRightButton(bool On);
public slots:
      int NewPolygon();
      void ChangeCurPoly(int nPoly);
//    void ImgReduc();
//    void ImgIncrease();
//    void ImgZoomScreen();
//    void SelectPoly(bool param);
//    void HSliderMoved(int x);
//    void VSliderMoved(int y);
};

#endif // DNWIDGETIMAGE_H
