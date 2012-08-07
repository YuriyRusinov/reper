/***********************************************************************
 * Module:  KKSAttrType.cpp
 * Author:  sergey
 * Modified: 25 ноября 2008 г. 17:41:17
 * Purpose: Implementation of the class KKSAttrType
 ***********************************************************************/
#include "KKSAttrType.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrType::KKSAttrType()
// Purpose:    Implementation of KKSAttrType::KKSAttrType()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttrType::KKSAttrType(int _type) : KKSRecord(),
    m_attrType ((KKSAttrType::KKSAttrTypes)_type)
{
//    m_attrType = atUndef;
}

KKSAttrType::KKSAttrType(const KKSAttrType & t) : KKSRecord(t)
{
    m_attrType = t.attrType();
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrType::~KKSAttrType()
// Purpose:    Implementation of KKSAttrType::~KKSAttrType()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttrType::~KKSAttrType()
{
   // TODO : implement
}

KKSAttrType::KKSAttrTypes KKSAttrType::attrType() const
{
    return m_attrType;
}

void KKSAttrType::setAttrType()
{
    if(id() < 1){
        m_attrType = atUndef;
        return;
    }

    switch (id()) 
    {
        case 1: m_attrType = atBool; break;               //логический
        case 2: m_attrType = atList; break;               //элемент справочника
        case 3: m_attrType = atParent;  break;            //родитель (ссылка на элемент того же справочника)
        case 4: m_attrType = atDate;  break;              //дата
        case 5: m_attrType = atDateTime;  break;          //дата-время
        case 6: m_attrType = atDouble; break;             //дробное значение
        case 7: m_attrType = atObjRef;  break;            //ссылка на информационный объект
        case 8: m_attrType = atInt;  break;               //целочисленное
        case 9: m_attrType = atString;  break;            //строка
        case 10: m_attrType = atInterval; break;          //временной интервал (лет, месяцев, дней)
        case 11: m_attrType = atUrl;  break;              //гиперссылка
        case 12: m_attrType = atCheckList; break;         //набор элементов справочника
        case 13: m_attrType = atText; break;              //Текст
        case 14: m_attrType = atFixString; break;         //Строка фиксированной длины
        case 15: m_attrType = atJPG; break;               //Изображение JPG
        case 16: m_attrType = atMaclabel; break;          //Мандатная метка
        case 17: m_attrType = atCheckListEx; break;       //Набор элементов справочника (модернизированный)
        case 18: m_attrType = atRecordColor; break;       //Цветовая гамма фона записи справочника (при отображении в списке)
        case 19: m_attrType = atRecordColorRef;break;     //Ссылка на цвет фона записи (при отображении в списке)
        case 20: m_attrType = atXMLDoc;break;             //XML-документ
        case 21: m_attrType = atIntervalH;break;          //временной интервал (часов, минут, секунд)
        case 22: m_attrType = atPoints;break;             //Набор пространственных точек
        case 23: m_attrType = atSVG;break;                //Объект SVG
        case 24: m_attrType = atVideo;break;              //Видео-ролик
        case 25: m_attrType = atRecordTextColor; break;   //Цветовая гамма текста записи справочника (при отображении в списке)
        case 26: m_attrType = atRecordTextColorRef;break; //Ссылка на цвет текста записи (при отображении в списке)
        case 27: m_attrType = atTime; break;              //время
        case 28: m_attrType = atGeometry; break;          //ГИС-объект (базовый)
        case 29: m_attrType = atGeometryPoly; break;      //ГИС-объект (стандартный полигон)
        case 30: m_attrType = atInt64; break;             //целочисленное (64 бита)
        case 31: m_attrType = atUUID; break;              //уникальный идентификатор (UUID)

        default: m_attrType = atUserDef; break;           //пользовательский (for future use)
    }
}

void KKSAttrType::setId(int newId)
{
    KKSRecord::setId(newId);
    setAttrType();
}

QStringList KKSAttrType::getIntervalUnits() 
{
    QStringList units;
    
    units << QObject::tr("years") 
          << QObject::tr("half-years") 
          << QObject::tr("quarters") 
          << QObject::tr("months") 
          << QObject::tr("weeks") 
          << QObject::tr("days") 
          << QObject::tr("hours");

    return units;
}

QString KKSAttrType::getIntervalValue(double amount, const QString & unit)
{
    QString v;
    
    if(unit == QObject::tr("years")){
        v = QString("%1 %2").arg(amount).arg("years");
    }

    if(unit == QObject::tr("half-years")){
        double a = amount / 2.0;
        v = QString("%1 %2").arg(a).arg("years");
    }

    if(unit == QObject::tr("quarters")){
        v = QString("%1 %2").arg(amount*3).arg("mons");
    }

    if(unit == QObject::tr("months")){
        v = QString("%1 %2").arg(amount).arg("mons");
    }

    if(unit == QObject::tr("weeks")){
        v = QString("%1 %2").arg(amount).arg("weeks");
    }

    if(unit == QObject::tr("days")){
        v = QString("%1 %2").arg(amount).arg("days");
    }

    if(unit == QObject::tr("hours")){
        v = QString("%1 %2").arg(amount).arg("hours");
    }
    
    return v;
}

bool KKSAttrType::isValidUnit(const QString & unit)
{
    if(unit == QObject::tr("year") || 
       unit == QObject::tr("years") ||
       unit == QObject::tr("half-year") ||
       unit == QObject::tr("half-years") ||
       unit == QObject::tr("quarter") ||
       unit == QObject::tr("quarters") ||
       unit == QObject::tr("mon") ||
       unit == QObject::tr("mons") ||
       unit == QObject::tr("month") ||
       unit == QObject::tr("months") ||
       unit == QObject::tr("week") ||
       unit == QObject::tr("weeks") ||
       unit == QObject::tr("day") ||
       unit == QObject::tr("days") ||
       unit == QObject::tr("hour") ||
       unit == QObject::tr("hours"))
    {
        return true;
    }

    return false;
}

int KKSAttrType::intervalNameToId(const QString & name)
{
    int id = 0;
    
    if(name == QObject::tr("years")){
        id = 1;
    }

    if(name == QObject::tr("half-years")){
        id = 2;
    }

    if(name == QObject::tr("quarters")){
        id = 3;
    }

    if(name == QObject::tr("months")){
        id = 4;
    }

    if(name == QObject::tr("weeks")){
        id = 5;
    }

    if(name == QObject::tr("days")){
        id = 6;
    }

    if(name == QObject::tr("hours")){
        id = 7;
    }

    return id;
}

QString KKSAttrType::intervalIdToName(int id)
{
    QString name;
    
    if(id == 1){
        name = QObject::tr("years");
    }

    if(id == 2){
        name = QObject::tr("half-years");
    }

    if(id == 3){ 
        name = QObject::tr("quarters");
    }

    if(id == 4){
        name = QObject::tr("months");
    }

    if(id == 5){
        name = QObject::tr("weeks");
    }

    if(id == 6){
        name = QObject::tr("days");
    }

    if(id == 7){
        name = QObject::tr("hours");
    }

    return name;
}
