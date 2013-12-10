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
/*������ �� ������������� �������*/
    float* GetBandZone(int NumBand,int xn,int yn,int xk,int yk);
    float* GetSpectrZone(int xn,int yn,int xk,int yk, bool *MaskCh);
    float* GetSpectrString(int y, bool *MaskCh);
    float* GetSpectrPoint(int xp,int yp, bool *MaskCh);
/*������ �  ��������������� �������*/
    void GetGeoData(GeoDataStruct *GD);
    void DetermGeoCoord(int xp,int yp,GeoDataStruct GD,double &XGeo,double &YGeo);
/*������� ������������*/
    QImage GenerateImg(int Ch1,int Ch2,int Ch3,double B1,double B2,double B3,double Contrast);
/*������ �������� � ����*/
    void GenerateSpectrFile(int x,int y,int W,int H,QList <QPoint> pt,QString PolyName);

/*����������� ������� ������������ �������*/
    int DetermNCh(float Lam);
//����������
    int W,H; //������, ������ �����������
    int Ch; //���������� �������
    float *Lamda;
    float *MinBrigth;
    float *MaxBrigth;
    float *MidleBrigth;
    bool *IsChanUsed;
    unsigned int *RasterData;
    QString FileBMPName; //������ ��� ����� BMP
    QString PathImgFile; //���� � ��������� �����
    QString NameImgFile; //��� ��������� ����� ��� ����������

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
