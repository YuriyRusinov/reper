#ifndef DNPOLY2_H
#define DNPOLY2_H

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
#include "Added/First/structs.h"
#include "Added/First/dnmathadd.h"
#include "Added/First/dncalcstring.h"
#include "Added/First/dntheam.h"
#include "Added/dnvector.h"
#include "Added/dnbdspectr.h"

class DNPoly2 : public QObject
{
 Q_OBJECT
/*Функции*/
public:
    struct ClassToPoly
    {
     QList <QPoint> Pix;
     int ClassNum;
    };
    DNPoly2(QString SerFileName);
    ~DNPoly2();
    bool* FillMaskCh(QList <int> nCh);
//Определить окружение точки
    QList <QPoint> DefinePixels(int NumPixels,int x,int y);
//Функции определения спектральных характеристик
    void GetSpectrPoint(int x,int y,float *DataSpec,bool *MaskCh);
    void GetDerivatSpectrPoint(int x,int y,int nCh1,int nCh2, float *LamMass, float *DerivatData);
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

    /*Вычислить корреляцию точки Если возвращает -2, то значит не вычислила*/
    float GetAKFPoint(int x,int y,int Side,float *Mask, int NumCh);

//Функции работы с полигонами
    /*Выделить полигон к которому принадлежит заданная точка (получить массив NoBlackPixels)*/
    quint64 GetKolvoPointPoly(int x,int y,/*координаты точки на изображении*/int *MassIsh,bool *NoBlackPixels);

    /*Взять средний спектр полигона*/
    void GetMidlSpecPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *MidlSpecPoly, bool *MaskCh);

    /*Взять СКО полигона*/
    void GetSKOPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *SKOPoly, bool *MaskCh);

    /*Взять энтропию полигона*/
    void GetEntropPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *EntropPoly, quint64 KDistr, bool *MaskCh);

//    /*Функчия превращения результатов классификации в новые полигоны*/
    QList <DNPoly2::ClassToPoly> ClassResultToPoly(float ProcentDetal);
//Функции экспорта и импорта полигонов
    void PolygonExpGeo(QString FileName, double xTopLeft, double XD, double XAngle, double yTopLeft, double YD, double yAngle,QString NameClassif="");
    static QList <QPoint> PolygonImpGeo(QString FileName, double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle, bool **ClMass, QString *ClassName);
    static QList <QPoint> PolygonImpGeo(QString FileName, double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle);

//Функции работы с классами
    quint64 GetKolvoPointClass(int NumKl,int *MassIsh,bool *NoBlackPixels);
    QList <int> GetNumClassMass(int *MassIsh,bool *NoBlackPixels);
    void GetMidleSpectrClass(float *DataSpec,quint64 KPoint,int NumClass, int *MassIsh, bool *NoBlackPixels, bool *MaskCh);

    /*Добавить канал в полигон*/
    void AddChanal(QString Formula);
    void AddChanal(float *Brigth);
    void AddIntegralChanal(int nCh1,int nCh2);
    void AddIntegralChanal(int nCh1,int nCh2,float *LamMass); //Размер LamMass должен быть (nCh2-nCh1+1)
    /*Создать изображение на основе показателя энтропии с соседними точками*/
    void AddEntropChanal(int nPixels, quint64 KDiskr, int NumCh);
    void AddSKOChanal(int nPixels, int NumCh);

    /*Создать изображение на основе раассчёта автокорреляционной функции*/
    void AddAKFChanal(int Side, int NumCh,int xb, int yb);

    /*Создать индексное изображение*/
    void CreateIndexImg(QString BMPFileName,int NumCh);

    /*Вычислить среднее значение в интервале спектральных каналов*/
    float CalcMidleBrigth(int NumCh1,int NumCh2,float *SpecData /*Массив яркостей точки во всех каналах*/);

//Функции фильтрации
    /*Линейная фильтрация*/
    float *LinearFilter(int side,float *Apert,float Kof,int NumCh);
    void LinearFilter(int side,int *Apert,float Kof); //В качестве фильтруемого массива используется ClassifMass

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

    void MaxLike(bool *NoBlackPixels,QStringList PolyFileNames,bool *MaskCh);

    float *SubPixAnalis(QList <float*> SpectrObj,int NumCh /*Количество спектральных каналов*/, bool *MaskCh);
    /*Количество элементов в массивах SpectrObj должно быть равным NumCh, и количеству учитываемых спектральных каналов MaskCh. Образцы спектров SpectrObj должны поканально
    совпадать с каналами изображения*/

//Фильтры
    void FilterPix(bool *NoBlackPixels,QString Formula,int Usl, float Value2);
    void FilterPoly(int *MassIsh,QString Formula,int Usl, float Value2);


/**********************************************************************************************************************************/
//Тематические задачи
    void SelectWater(); //Выделение воды (RGB)
    void Batinometr();  //Батинометрия (WorldView 2)
    void Batinometr(int N590,int N830,int N900,int N580,int N620,int N485,int N560,int N660,int N450,int N545,int N605);  //Батинометрия (Лептон)

    void MaskAreaMS(float DeltaMS,int TypeFone,int Nnir1,int Nnir2,int Nred,int Ngr,float IndexPor, float LRM, float AreaPor); //Выделение маскировочных покрытий
    //TypeFone=0 - песок, 1 - снег, 2 - растительность
    //LRM - Линейный размер пикселя
    void MaskAreaGS(QString ClsId,QList<QString> ClsBackgroId,float DeltaGS,int TypeFone,int N705,int N750,int N1030,float GSIndexPor,float SubPixPor,float *LamMass,float MaxDerivant,int NDif1, int NDif2, float *LamMassSubPix, QList<int> NumChSubPix);
    void RoutMoveTap1(int Ngreen,int Nred,int Nnir1,int NredEdge,float NDVIpor,int NumCl,float MaxSKO,float Qc,int I);
/**********************************************************************************************************************************/
    static void PixToGeo(int xp,int yp,double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle,double *xGeo,double *yGeo);
    static void GeoToPix(double *xp,double *yp,double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle,double xGeo,double yGeo);
    QList <DNVector> RastrToVector(double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double YAngle,float Kof,float MinV, QString FileName,
                                   QList <QString> NameFields,QList <QString> TypeFields,QString NameLayer);
public:
    int W,H,xn,yn,Ch,KolvoPix;
    int IshCh; //Исходное количество каналов, присутствующее на изображении
    int *MassPoly; //Массив исходного полигона, 1 - точка пренадлежит исходному полигону, -1 - точка не пренадлежит исходному полигону
    bool *CurrentPoly; //Массив текущего полигона Если точка пренадлежит полигону, то TRUE, заполняется в функции GetKolvoPointZone

    bool IsPolyClassif; //Применялись ли к полигону методы классификации (по умолчанию FALSE)
    int *ClassifMass;   //Массиф классификации

    //База данных спектров
    DNBDSpectr *BdSpectr;

    QQueue <QPoint> pt;
    quint64 px; //Количество пикселей в многоугольнике
    qint64 OffsetData;
    QString SerFileName;
    QString PathTempFile;

    /*Переменные для вектаризации*/
    float KofV;
    float MinV;

    void RoutMoveTap2(int N630,int N690,int N750,int N860,int N720,int N800,int N900,int N970,
                      float NDVIPor1, float DeltaNDVIPor1, float NDVIPor2, float DeltaNDVIPor2,
                      float NDVIPor3, float DeltaNDVIPor3, float WBIPor);
    void SmocesTap1(int Nred, int Nnir, float NDVIPor);
    void SmocesTap2(float R600Por, float KofCorrPor, int N600, int N500);
    void EmbedObjMS(int Nred, int Nnir, float NDVIPor, float DNDVI, int Nnir2, float B630_690, float B800_1100, float Cpor630_690, float Cpor800_1100);
private:
     DNMathAdd Math;



 signals:
     void ChangeProgressVal(int PrgR,int PrgV);
    
};

#endif // DNPOLY2_H
