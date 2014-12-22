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
/*�������*/
public:
    struct ClassToPoly
    {
     QList <QPoint> Pix;
     int ClassNum;
    };
    DNPoly2(QString SerFileName);
    ~DNPoly2();
    bool* FillMaskCh(QList <int> nCh);
//���������� ��������� �����
    QList <QPoint> DefinePixels(int NumPixels,int x,int y);
//������� ����������� ������������ �������������
    void GetSpectrPoint(int x,int y,float *DataSpec,bool *MaskCh);
    void GetDerivatSpectrPoint(int x,int y,int nCh1,int nCh2, float *LamMass, float *DerivatData);
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

    /*��������� ���������� ����� ���� ���������� -2, �� ������ �� ���������*/
    float GetAKFPoint(int x,int y,int Side,float *Mask, int NumCh);

//������� ������ � ����������
    /*�������� ������� � �������� ����������� �������� ����� (�������� ������ NoBlackPixels)*/
    quint64 GetKolvoPointPoly(int x,int y,/*���������� ����� �� �����������*/int *MassIsh,bool *NoBlackPixels);

    /*����� ������� ������ ��������*/
    void GetMidlSpecPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *MidlSpecPoly, bool *MaskCh);

    /*����� ��� ��������*/
    void GetSKOPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *SKOPoly, bool *MaskCh);

    /*����� �������� ��������*/
    void GetEntropPoly(bool *NoBlackPixels,unsigned long long KolvoPixPoly, float *EntropPoly, quint64 KDistr, bool *MaskCh);

//    /*������� ����������� ����������� ������������� � ����� ��������*/
    QList <DNPoly2::ClassToPoly> ClassResultToPoly(float ProcentDetal);
//������� �������� � ������� ���������
    void PolygonExpGeo(QString FileName, double xTopLeft, double XD, double XAngle, double yTopLeft, double YD, double yAngle,QString NameClassif="");
    static QList <QPoint> PolygonImpGeo(QString FileName, double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle, bool **ClMass, QString *ClassName);
    static QList <QPoint> PolygonImpGeo(QString FileName, double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle);

//������� ������ � ��������
    quint64 GetKolvoPointClass(int NumKl,int *MassIsh,bool *NoBlackPixels);
    QList <int> GetNumClassMass(int *MassIsh,bool *NoBlackPixels);
    void GetMidleSpectrClass(float *DataSpec,quint64 KPoint,int NumClass, int *MassIsh, bool *NoBlackPixels, bool *MaskCh);

    /*�������� ����� � �������*/
    void AddChanal(QString Formula);
    void AddChanal(float *Brigth);
    void AddIntegralChanal(int nCh1,int nCh2);
    void AddIntegralChanal(int nCh1,int nCh2,float *LamMass); //������ LamMass ������ ���� (nCh2-nCh1+1)
    /*������� ����������� �� ������ ���������� �������� � ��������� �������*/
    void AddEntropChanal(int nPixels, quint64 KDiskr, int NumCh);
    void AddSKOChanal(int nPixels, int NumCh);

    /*������� ����������� �� ������ ��������� ������������������ �������*/
    void AddAKFChanal(int Side, int NumCh,int xb, int yb);

    /*������� ��������� �����������*/
    void CreateIndexImg(QString BMPFileName,int NumCh);

    /*��������� ������� �������� � ��������� ������������ �������*/
    float CalcMidleBrigth(int NumCh1,int NumCh2,float *SpecData /*������ �������� ����� �� ���� �������*/);

//������� ����������
    /*�������� ����������*/
    float *LinearFilter(int side,float *Apert,float Kof,int NumCh);
    void LinearFilter(int side,int *Apert,float Kof); //� �������� ������������ ������� ������������ ClassifMass

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

    void MaxLike(bool *NoBlackPixels,QStringList PolyFileNames,bool *MaskCh);

    float *SubPixAnalis(QList <float*> SpectrObj,int NumCh /*���������� ������������ �������*/, bool *MaskCh);
    /*���������� ��������� � �������� SpectrObj ������ ���� ������ NumCh, � ���������� ����������� ������������ ������� MaskCh. ������� �������� SpectrObj ������ ����������
    ��������� � �������� �����������*/

//�������
    void FilterPix(bool *NoBlackPixels,QString Formula,int Usl, float Value2);
    void FilterPoly(int *MassIsh,QString Formula,int Usl, float Value2);


/**********************************************************************************************************************************/
//������������ ������
    void SelectWater(); //��������� ���� (RGB)
    void Batinometr();  //������������ (WorldView 2)
    void Batinometr(int N590,int N830,int N900,int N580,int N620,int N485,int N560,int N660,int N450,int N545,int N605);  //������������ (������)

    void MaskAreaMS(float DeltaMS,int TypeFone,int Nnir1,int Nnir2,int Nred,int Ngr,float IndexPor, float LRM, float AreaPor); //��������� ������������� ��������
    //TypeFone=0 - �����, 1 - ����, 2 - ��������������
    //LRM - �������� ������ �������
    void MaskAreaGS(QString ClsId,QList<QString> ClsBackgroId,float DeltaGS,int TypeFone,int N705,int N750,int N1030,float GSIndexPor,float SubPixPor,float *LamMass,float MaxDerivant,int NDif1, int NDif2, float *LamMassSubPix, QList<int> NumChSubPix);
    void RoutMoveTap1(int Ngreen,int Nred,int Nnir1,int NredEdge,float NDVIpor,int NumCl,float MaxSKO,float Qc,int I);
/**********************************************************************************************************************************/
    static void PixToGeo(int xp,int yp,double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle,double *xGeo,double *yGeo);
    static void GeoToPix(double *xp,double *yp,double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double yAngle,double xGeo,double yGeo);
    QList <DNVector> RastrToVector(double xTopLeft,double XD,double XAngle,double yTopLeft,double YD,double YAngle,float Kof,float MinV, QString FileName,
                                   QList <QString> NameFields,QList <QString> TypeFields,QString NameLayer);
public:
    int W,H,xn,yn,Ch,KolvoPix;
    int IshCh; //�������� ���������� �������, �������������� �� �����������
    int *MassPoly; //������ ��������� ��������, 1 - ����� ����������� ��������� ��������, -1 - ����� �� ����������� ��������� ��������
    bool *CurrentPoly; //������ �������� �������� ���� ����� ����������� ��������, �� TRUE, ����������� � ������� GetKolvoPointZone

    bool IsPolyClassif; //����������� �� � �������� ������ ������������� (�� ��������� FALSE)
    int *ClassifMass;   //������ �������������

    //���� ������ ��������
    DNBDSpectr *BdSpectr;

    QQueue <QPoint> pt;
    quint64 px; //���������� �������� � ��������������
    qint64 OffsetData;
    QString SerFileName;
    QString PathTempFile;

    /*���������� ��� ������������*/
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
