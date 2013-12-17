/***********************************************************************
 * Module:  KKSAttrType.h
 * Author:  sergey
 * Modified: 25 ноября 2008 г. 17:41:17
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
       atUndef = 0,               //не задан
       atBool = 1,                //логический
       atList = 2,                //элемент справочника
       atParent = 3,              //родитель (ссылка на элемент того же справочника)
       atDate = 4,                //дата
       atDateTime = 5,            //дата-время
       atDouble = 6,              //дробное значение
       atObjRef = 7,              //ссылка на информационный объект
       atInt = 8,                 //целочисленное
       atString = 9,              //строка
       atInterval = 10,           //временной интервал (лет, полугодий, кварталов, месяцев, недель, дней, часов)
       atUrl = 11,                //гиперссылка
       atCheckList = 12,          //Набор элементов справочника
       atText = 13,               //Текст
       atFixString = 14,          //Строка фиксированной длины
       atJPG = 15,                //Изображение JPG
       atMaclabel = 16,           //Мандатная метка
       atCheckListEx = 17,        //Набор элементов справочника (модернизированный)
       atRecordColor = 18,        //Цветовая гамма фона записи справочника (при отображении в списке)
       atRecordColorRef = 19,     //Ссылка на цвет фона записи (при отображении в списке)
       atXMLDoc=20,               //XML-документ
       atIntervalH=21,            //временной интервал (часов, минут, секунд)
       atPoints=22,               //Набор пространственных точек
       atSVG=23,                  //Объект SVG
       atVideo=24,                //Видео-ролик
       atRecordTextColor = 25,    //Цветовая гамма текста записи справочника (при отображении в списке)
       atRecordTextColorRef = 26, //Ссылка на цвет текста записи (при отображении в списке)
       atTime = 27,               //время
       //atGeometry = 28,           //ГИС-объект (базовый)
       //atGeometryPoly = 29,       //ГИС-объект (стандартный полигон)
       atInt64 = 30,              //целочисленное (64 бита)
       atUUID = 31,               //уникальный идентификатор (UUID)
       atComplex = 32,            //Составной атрибут
       atHistogram = 33,          //Гистограмма
       atVectorLayer = 34,        //ГИС-объект (векторный слой)
       atRasterLayer = 35,        //ГИС-объект (растровый слой)
       atGISMap = 36,             //ГИС-объект (карта)
       atUserDef = 37             //пользовательский (for future use)
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
