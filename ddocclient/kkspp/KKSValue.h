/***********************************************************************
 * Module:  KKSValue.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:11:30
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
       //������ ����� ����� ���������� �������� m_columnValue, ������������ ��� � value.toString()
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

       //�������� ��������. 
       //���� ���������, ��� ��� ��������� ���� "������" �������� 
       //- ��� ������������� ������ � ��������������� �������.
       //��� ���� ���� �������� m_columnValue, ������� ��� ����� ���������
       //�������� �������� �������� �� ��� ������� �������, ������������� ������ ������� ������ � ��������� value
       QVariant m_value;
       QString m_columnValue;
       int m_type;
       mutable bool m_isValid;
       mutable bool m_isLiteral;
       mutable bool m_isNull;


};

#endif
