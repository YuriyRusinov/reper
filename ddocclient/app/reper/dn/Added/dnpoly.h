/*
Класс для работы с файлом спектральных данных универсального размера
Формат файла:
1. структура, в которой определены основные параметры,
2. массив полигона -1 - точка не рассматривается, 1 - точка рассматривается
3. массив координат вершин многоугольника
4. спектральные данные
*/

#ifndef MULTISPECPOLYPROP_H
#define MULTISPECPOLYPROP_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QPoint>
#include <QQueue>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include "dn/Added/First/structs.h"
#include "dn/Added/First/dnmathadd.h"
#include "dn/Added/First/dncalcstring.h"
#include "dn/Added/First/dntheam.h"
#include "dn/Added/dnvector.h"

class DNPoly : public QObject
{
 Q_OBJECT
/*Функции*/
public:
    DNPoly(QString SerFileName);
    ~DNPoly();
//Функции определения спектральных характеристик
    void GetSpectrPoint(int x,int y,float *DataSpec,bool *MaskCh);
    void GetSpectrString(int y,float *DataSpec,bool *MaskCh);

    /*Взять спектр участка изображения, обозначенного в массиве CurentPoly*/
    void GetSpectrZone(float *DataSpec,bool *MaskCh);

    /*Взять яркость всех точек полигона в определённом канале*/
    void GetBrigthChanPoly(float *DataSpec,int nCh, bool *NoBlackPixels);

    /**/
    void GetBrigthSqare(float *DataSpec,int nCh);

    /*Умножить все значения яркости на множитель*/
    void MultiCh(float Kof);

/*Функции определения статистических параметров окружения точки*/
    /*Вычислить энтропию окружения точки*/
    float GetEntropPoint(int x,int y,int nPixels,quint64 KDiskr, int NumCh);
    /*Вычислить СКО окружения точки*/
    float GetSKOPoint(int x,int y,int nPixels,int NumCh);
//Функции работы с полигонами
    /*Выделить полигон к которому принадлежит заданная точка (получить массив NoBlackPixels)*/
    quint64 GetKolvoPointPoly(int x,int y,/*координаты точки на изображении*/int *MassIsh,bool *NoBlackPixels);

    /*Взять средний спектр полигона*/
    void GetMidlSpecPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *MidlSpecPoly, bool *MaskCh);

    /*Взять СКО полигона*/
    void GetSKOPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *SKOPoly, bool *MaskCh);

    /*Взять энтропию полигона*/
    void GetEntropPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *EntropPoly, quint64 KDistr, bool *MaskCh);

//Функции работы с классами
    quint64 GetKolvoPointClass(int NumKl,int *MassIsh,bool *NoBlackPixels);
    QList <int> GetNumClassMass(int *MassIsh,bool *NoBlackPixels);
    void GetMidleSpectrClass(float *DataSpec,quint64 KPoint,int NumClass, int *MassIsh, bool *NoBlackPixels, bool *MaskCh);

    /*Добавить канал в полигон*/
    void AddChanal(QString Formula);
    void AddChanal(float *Brigth);
    void AddIntegralChanal(int nCh1,int nCh2);
    /*Создать изображение на основе показателя энтропии с соседними точками*/
    void AddEntropChanal(int nPixels, quint64 KDiskr, int NumCh);
    void AddSKOChanal(int nPixels, int NumCh);
    /*Создать индексное изображение*/
    void CreateIndexImg(QString BMPFileName,int NumCh);

    /*Вычислить среднее значение в интервале спектральных каналов*/
    float CalcMidleBrigth(int NumCh1,int NumCh2,float *SpecData /*Массив яркостей точки во всех каналах*/);

//Функции фильтрации
    /*Линейная фильтрация*/
    float *LinearFilter(int side,float *Apert,float Kof,int NumCh);

//Методы классификации*/
    quint64 MagicWand(bool *NoBlackPixels,/*Входной массив, обрабатываются точки только со значениями TRUE*/
                      int x, int y,/*координаты точки на изображении (в размерности изображения)*/
                      float Contrast,/*Величина контраста или пороговой яркости*/
                      int NumCh,/*Номер канала*/
                      int mode,/*0 - считать по контрасту, 1 - считать по пороговой яркости*/
                      bool *NoBlackPixels2/*Выходной массив*/);


    int IsoData(bool *NoBlackPixels,/*Входной массив, обрабатываются точки только со значениями TRUE*/
                    int KolvoClass, /*необходимое число кластеров*/
                    float MaxSKO, /*максимальное СКО кластера*/
                    float Qc, /*Параметр, характеризующий компактность (минимальное расстояние между центрами кластеров)*/
                    int I, /*Допустимое число циклов*/
                    bool *MaskCh );

    int SAM(bool *NoBlackPixels, /*Входной массив, обрабатываются точки только со значениями TRUE*/
            QStringList PolyFileNames, /*Пути к файлам эталонных полигонов*/
            bool IsSKOEnabled, /*Если TRUE учитываем расброс спектральных углов*/
            float LimitSKO, /*Лимит на расброс спектральных углов, если -1 - лимит не установлен*/
            bool *MaskCh);

//Фильтры
    void FilterPix(bool *NoBlackPixels,QString Formula,int Usl, float Value2);
    void FilterPoly(int *MassIsh,QString Formula,int Usl, float Value2);


/**********************************************************************************************************************************/
//Тематические задачи
    void SelectWater(); //Выделение воды (RGB)
    void Batinometr();  //Батинометрия (WorldView 2)
    void Batinometr(int N590,int N830,int N900,int N580,int N620,int N485,int N560,int N660,int N450,int N545,int N605);  //Батинометрия (Лептон)
/**********************************************************************************************************************************/
    void PixToGeo(int xp,int yp,double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle,double *xGeo,double *yGeo);
    QList <DNVector> RastrToVector(double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double YAngle, float Kof, float MinV,QString FileName);
public:
    int W,H,xn,yn,Ch,KolvoPix;
    int IshCh; //Исходное количество каналов, присутствующее на изображении
    int *MassPoly; //Массив исходного полигона, 1 - точка пренадлежит исходному полигону, -1 - точка не пренадлежит исходному полигону
    bool *CurrentPoly; //Массив текущего полигона Если точка пренадлежит полигону, то TRUE, заполняется в функции GetKolvoPointZone

    bool IsPolyClassif; //Применялись ли к полигону методы классификации (по умолчанию FALSE)
    int *ClassifMass;   //Массиф классификации

    QQueue <QPoint> pt;
    quint64 px; //Количество пикселей в многоугольнике
    qint64 OffsetData;
    QString SerFileName;
    QString PathTempFile;

    /*Переменные для вектаризации*/
    float KofV;
    float MinV;
 private:
     DNMathAdd Math;
     QList <QPoint> DefinePixels(int NumPixels,int x,int y);
     bool* FillMaskCh(QList <int> nCh);


 signals:
     void ChangeProgressVal(int PrgR,int PrgV);

};

#endif // MULTISPECPOLYPROP_H
