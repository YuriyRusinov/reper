#ifndef DNIMAGELIB_H
#define DNIMAGELIB_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QPixmap>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QMessageBox>
#include "dn/Added/First/structs.h"
#include "gdal_priv.h"

struct GeoDataStruct
{
 double XTopLeftPix;
 double XD;
 double XAngle;
 double YTopLeftPix;
 double YD;
 double YAngle;
};

class DnImageLib : public QObject
{
 Q_OBJECT
public:
    DnImageLib(QString FileName);
    ~DnImageLib();
/*Работа со спектральными данными*/
    float* GetBandZone(int NumBand,int xn,int yn,int xk,int yk);
    float* GetSpectrZone(int xn,int yn,int xk,int yk, bool *MaskCh);
    float* GetSpectrString(int y, bool *MaskCh);
    float* GetSpectrPoint(int xp,int yp, bool *MaskCh);
/*Работа с  географическими данными*/
    void GetGeoData(GeoDataStruct *GD);
    void DetermGeoCoord(int xp,int yp,GeoDataStruct GD,double &XGeo,double &YGeo);
/*Функции визуализации*/
    QImage GenerateImg(int Ch1,int Ch2,int Ch3,double B1,double B2,double B3,double Contrast);
/*Запись полигона в файл*/
    void GenerateSpectrFile(int x,int y,int W,int H,QList <QPoint> pt,QString PolyName);

/*Определение номеров спектральных каналов*/
    int DetermNCh(float Lam);
//Переменные
    int W,H; //Ширина, высота изображения
    int Ch; //Количество каналов
    float *Lamda;
    float *MinBrigth;
    float *MaxBrigth;
    float *MidleBrigth;
    bool *IsChanUsed;
    unsigned int *RasterData;
    QString FileBMPName; //Полное имя файла BMP
    QString PathImgFile; //Путь к открытому файлу
    QString NameImgFile; //Имя открытого файла без расширения

signals:
    void ChangeProgressVal(int PrgR,int PrgV);
public slots:

private:
    GDALDataset *GdalData;

    int GetQChanals(bool *MaskCh);
    void GetMinMaxBrigthChan(int nCh,float *BMin,float *BMax, float *BMidle);
    bool *FillMaskCh(QList <int> nCh);
    bool IsPointInside(int xp,int yp, QList <QPoint>pt);
};

#endif // DNIMAGELIB_H
