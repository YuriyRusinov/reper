/***********************************************************************
 * Module:  KKSValue.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:11:30
 * Purpose: Declaration of the class KKSValue
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSValue_h)
#define __KKSSITOOOM_KKSValue_h

#include <QVariant>
#include <QString>
#include "kkspp_config.h"
#include "KKSData.h"

class _PP_EXPORT KKSValue : public KKSData
{
    public:
       //данный метод также сбрасывает значение m_columnValue, устанавливая его в value.toString()
       int setValue(const QString & value, int type);
       QString value(void) const;
       const QVariant & valueVariant() const;
       QString valueForInsert() const;
   
       int type(void) const;

       bool isValid(void) const;
       bool isLiteral() const;
       bool isNull() const;

       void setColumnValue(const QString & _columnValue);
       const QString & columnValue() const;
   
       KKSValue();
       KKSValue(const KKSValue & v);
       KKSValue(const QString & value, int type);
       virtual ~KKSValue();
       KKSValue & operator = (const KKSValue & v);

       static QDateTime stringToDateTime(const QString & s);
	   static QDate stringToDate(const QString & s);
	   static QTime stringToTime(const QString & s);
#ifdef Q_WS_WIN
       //for web

       int setValue_w(const std::string & value, int type);
       std::string value_w() const;	
       void setColumnValue_w(const std::string & columnValue);
       std::string columnValue_w() const;
#endif
    private:
       void verify(void) const;

       //значение атрибута. 
       //Надо учитывать, что для атрибутов типа "Список" значение 
       //- это идентификатор записи в соответствующей колонке.
       //При этом есть параметр m_columnValue, который для таких атрибутов
       //содержит истинное значение из той колонки таблицы, идентификатор записи которой указан в параметре value
       QVariant m_value;
       QString m_columnValue;
       int m_type;
       mutable bool m_isValid;
       mutable bool m_isLiteral;
       mutable bool m_isNull;


};

#endif
