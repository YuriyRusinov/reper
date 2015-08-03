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

/*Функции полигона*/
public:
    DNImgPoly();
    ~DNImgPoly();
    QPoint GetMinP();
    QPoint GetMaxP();
    void SelectPolygon();

//    void PolygonToFile(QString FileName); //Строка с полным именем файла

//private:
    bool IsPointInside(int xp,int yp);
    void CreateImg(float *ImgPolyR,float *ImgPolyG,float *ImgPolyB, int *MassPoly, quint64 px);


/*Свойства полигона*/
public:
    int /*KolvoPix,*/minX,minY,maxX,maxY;
    int *SelPoly; //Участок изображения
    QList <QPoint> pt; //Координаты углов полигона в проекции картинки
    QPoint Point;
    bool IsPolygonSelect; //Определены ли точки полигона
    bool IsPolygonCurrent; //Этот полигон является текущим
    bool IsPolyClassif; //Применялись ли к полигону методы классификации (по умолчанию FALSE)
    bool IsPolyCreateImg; //Применялись ли к полигону особые параметры отрисовки
    int *PImgR,*PImgG,*PImgB;
    int *ClassifMass; //Массиф классификации
};

class DNWidgetImage : public QWidget
{
    Q_OBJECT
public:
    explicit DNWidgetImage(QWidget *parent = 0);

    void ChangedBMP(/*QString PathToBMP*/);
    bool isImgLoad; //Загружена ли картинка
    bool IsCretaePolyOn; //Включён ли режим создания полигона
    bool IsSelPoly; //Выделен ли полигон

    bool IsDlgAIHidden;
    int DlgAiSizeSide;

    int ViewAreaX,ViewAreaY; //Размеры рабочей области куда выводится изображение
    QList <DNImgPoly> Polygons; //Список полигонов
    QList <QPoint> pt; //Список точек без полигона
//    DNImgPoly *Polygon; //Текущий полигон
//    PropPolygonSel Polygon; //Текущий полигон
//    QPoint Point;
//    int KolvoPixPoly;
//    int ProzrPol; //Характеристика прозрачности полигона
    QImage img;
    QImage imgScale;

protected:
    virtual void paintEvent(QPaintEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent* mEvent);
    virtual void mousePressEvent(QMouseEvent* mEvent);
    virtual void mouseDoubleClickEvent(QMouseEvent *);

private:
    float NewKof; //Коэффициент размера изображения
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
