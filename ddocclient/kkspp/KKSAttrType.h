/***********************************************************************
 * Module:  KKSAttrType.h
 * Author:  sergey
 * Modified: 25 ������ 2008 �. 17:41:17
 * Purpose: Declaration of the class KKSAttrType
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrType_h)
#define __KKSSITOOOM_KKSAttrType_h

#include <KKSRecord.h>
#include <QStringList>

#include "kkspp_config.h"

class _PP_EXPORT KKSAttrType : public KKSRecord
{
public:
   KKSAttrType(int _type=0);
   KKSAttrType(const KKSAttrType & t);
   virtual ~KKSAttrType();
   enum KKSAttrTypes
   {
       atUndef = 0,               //�� �����
       atBool = 1,                //����������
       atList = 2,                //������� �����������
       atParent = 3,              //�������� (������ �� ������� ���� �� �����������)
       atDate = 4,                //����
       atDateTime = 5,            //����-�����
       atDouble = 6,              //������� ��������
       atObjRef = 7,              //������ �� �������������� ������
       atInt = 8,                 //�������������
       atString = 9,              //������
       atInterval = 10,           //��������� �������� (���, ���������, ���������, �������, ������, ����, �����)
       atUrl = 11,                //�����������
       atCheckList = 12,          //����� ��������� �����������
       atText = 13,               //�����
       atFixString = 14,          //������ ������������� �����
       atJPG = 15,                //����������� JPG
       atMaclabel = 16,           //��������� �����
       atCheckListEx = 17,        //����� ��������� ����������� (�����������������)
       atRecordColor = 18,        //�������� ����� ���� ������ ����������� (��� ����������� � ������)
       atRecordColorRef = 19,     //������ �� ���� ���� ������ (��� ����������� � ������)
       atXMLDoc=20,               //XML-��������
       atIntervalH=21,            //��������� �������� (�����, �����, ������)
       atPoints=22,               //����� ���������������� �����
       atSVG=23,                  //������ SVG
       atVideo=24,                //�����-�����
       atRecordTextColor = 25,    //�������� ����� ������ ������ ����������� (��� ����������� � ������)
       atRecordTextColorRef = 26, //������ �� ���� ������ ������ (��� ����������� � ������)
       atTime = 27,               //�����
       //atGeometry = 28,           //���-������ (�������)
       //atGeometryPoly = 29,       //���-������ (����������� �������)
       atInt64 = 30,              //������������� (64 ����)
       atUUID = 31,               //���������� ������������� (UUID)
       atComplex = 32,            //��������� �������
       atHistogram = 33,          //�����������
       atVectorLayer = 34,        //���-������ (��������� ����)
       atRasterLayer = 35,        //���-������ (��������� ����)
       atGISMap = 36,             //���-������ (�����)
       atUserDef = 37             //���������������� (for future use)
   };

   KKSAttrTypes attrType() const;
   void setId(int newId);

   static QStringList getIntervalUnits();
   static QString getIntervalValue(double amount, const QString & unit);
   static QString intervalIdToName(int id);
   static int intervalNameToId(const QString & name);
   static bool isValidUnit(const QString & unit);

protected:
    virtual void setAttrType();
private:

    KKSAttrTypes m_attrType;
};

#endif
