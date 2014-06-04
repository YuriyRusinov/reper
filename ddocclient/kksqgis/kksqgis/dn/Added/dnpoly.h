/*
����� ��� ������ � ������ ������������ ������ �������������� �������
������ �����:
1. ���������, � ������� ���������� �������� ���������,
2. ������ �������� -1 - ����� �� ���������������, 1 - ����� ���������������
3. ������ ��������� ������ ��������������
4. ������������ ������
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
/*�������*/
public:
    DNPoly(QString SerFileName);
    ~DNPoly();
//������� ����������� ������������ �������������
    void GetSpectrPoint(int x,int y,float *DataSpec,bool *MaskCh);
    void GetSpectrString(int y,float *DataSpec,bool *MaskCh);

    /*����� ������ ������� �����������, ������������� � ������� CurentPoly*/
    void GetSpectrZone(float *DataSpec,bool *MaskCh);

    /*����� ������� ���� ����� �������� � ����������� ������*/
    void GetBrigthChanPoly(float *DataSpec,int nCh, bool *NoBlackPixels);

    /**/
    void GetBrigthSqare(float *DataSpec,int nCh);

    /*�������� ��� �������� ������� �� ���������*/
    void MultiCh(float Kof);

/*������� ����������� �������������� ���������� ��������� �����*/
    /*��������� �������� ��������� �����*/
    float GetEntropPoint(int x,int y,int nPixels,quint64 KDiskr, int NumCh);
    /*��������� ��� ��������� �����*/
    float GetSKOPoint(int x,int y,int nPixels,int NumCh);
//������� ������ � ����������
    /*�������� ������� � �������� ����������� �������� ����� (�������� ������ NoBlackPixels)*/
    quint64 GetKolvoPointPoly(int x,int y,/*���������� ����� �� �����������*/int *MassIsh,bool *NoBlackPixels);

    /*����� ������� ������ ��������*/
    void GetMidlSpecPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *MidlSpecPoly, bool *MaskCh);

    /*����� ��� ��������*/
    void GetSKOPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *SKOPoly, bool *MaskCh);

    /*����� �������� ��������*/
    void GetEntropPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *EntropPoly, quint64 KDistr, bool *MaskCh);

//������� ������ � ��������
    quint64 GetKolvoPointClass(int NumKl,int *MassIsh,bool *NoBlackPixels);
    QList <int> GetNumClassMass(int *MassIsh,bool *NoBlackPixels);
    void GetMidleSpectrClass(float *DataSpec,quint64 KPoint,int NumClass, int *MassIsh, bool *NoBlackPixels, bool *MaskCh);

    /*�������� ����� � �������*/
    void AddChanal(QString Formula);
    void AddChanal(float *Brigth);
    void AddIntegralChanal(int nCh1,int nCh2);
    /*������� ����������� �� ������ ���������� �������� � ��������� �������*/
    void AddEntropChanal(int nPixels, quint64 KDiskr, int NumCh);
    void AddSKOChanal(int nPixels, int NumCh);
    /*������� ��������� �����������*/
    void CreateIndexImg(QString BMPFileName,int NumCh);

    /*��������� ������� �������� � ��������� ������������ �������*/
    float CalcMidleBrigth(int NumCh1,int NumCh2,float *SpecData /*������ �������� ����� �� ���� �������*/);

//������� ����������
    /*�������� ����������*/
    float *LinearFilter(int side,float *Apert,float Kof,int NumCh);
    /*���������� ����������*/
    void MedianFilter(int side);

//������ �������������*/
    quint64 MagicWand(bool *NoBlackPixels,/*������� ������, �������������� ����� ������ �� ���������� TRUE*/
                      int x, int y,/*���������� ����� �� ����������� (� ����������� �����������)*/
                      float Contrast,/*�������� ��������� ��� ��������� �������*/
                      int NumCh,/*����� ������*/
                      int mode,/*0 - ������� �� ���������, 1 - ������� �� ��������� �������*/
                      bool *NoBlackPixels2/*�������� ������*/);


    int IsoData(bool *NoBlackPixels,/*������� ������, �������������� ����� ������ �� ���������� TRUE*/
                    int KolvoClass, /*����������� ����� ���������*/
                    float MaxSKO, /*������������ ��� ��������*/
                    float Qc, /*��������, ��������������� ������������ (����������� ���������� ����� �������� ���������)*/
                    int I, /*���������� ����� ������*/
                    bool *MaskCh );

    int SAM(bool *NoBlackPixels, /*������� ������, �������������� ����� ������ �� ���������� TRUE*/
            QStringList PolyFileNames, /*���� � ������ ��������� ���������*/
            bool IsSKOEnabled, /*���� TRUE ��������� ������� ������������ �����*/
            float LimitSKO, /*����� �� ������� ������������ �����, ���� -1 - ����� �� ����������*/
            bool *MaskCh);

//�������
    void FilterPix(bool *NoBlackPixels,QString Formula,int Usl, float Value2);
    void FilterPoly(int *MassIsh,QString Formula,int Usl, float Value2);


/**********************************************************************************************************************************/
//������������ ������
    void SelectWater(); //��������� ���� (RGB)
    void Batinometr();  //������������ (WorldView 2)
    void Batinometr(int N590,int N830,int N900,int N580,int N620,int N485,int N560,int N660,int N450,int N545,int N605);  //������������ (������)
/**********************************************************************************************************************************/
    void PixToGeo(int xp,int yp,double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle,double *xGeo,double *yGeo);
    QList <DNVector> RastrToVector(double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double YAngle, float Kof, float MinV,QString FileName);
public:
    int W,H,xn,yn,Ch,KolvoPix;
    int IshCh; //�������� ���������� �������, �������������� �� �����������
    int *MassPoly; //������ ��������� ��������, 1 - ����� ����������� ��������� ��������, -1 - ����� �� ����������� ��������� ��������
    bool *CurrentPoly; //������ �������� �������� ���� ����� ����������� ��������, �� TRUE, ����������� � ������� GetKolvoPointZone

    bool IsPolyClassif; //����������� �� � �������� ������ ������������� (�� ��������� FALSE)
    int *ClassifMass;   //������ �������������

    QQueue <QPoint> pt;
    quint64 px; //���������� �������� � ��������������
    qint64 OffsetData;
    QString SerFileName;
    QString PathTempFile;

    /*���������� ��� ������������*/
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
