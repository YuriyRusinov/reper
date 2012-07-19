/***********************************************************************
 * Module:  KKSEIOData.h
 * Author:  ksa
 * Modified: 19 декабря 2008 г. 16:57:28
 * Purpose: Declaration of the class KKSEIOData
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSEIOData_h)
#define __KKSSITOOOM_KKSEIOData_h

#include <QMap>
#include <QString>
#include <QMetaType>

#include "KKSData.h"
#include "KKSMap.h"
#include "kkspp_config.h"


/*
В качестве ключа для QMap в данном классе используется код атрибута (не ИД)
Это допустимо (даже с учетом того, что код атрибута более не является уникальным), 
поскольку данный класс всегда описывает табличные данные 
и в качестве кода используется название колонки. 
А в одной таблице двух колонок с одинаковым названием быть не может.

Во всех остальных случаях использование кода в качестве ключа атрибута недопустимо.
*/
class _PP_EXPORT KKSEIOData : public KKSData
{
public:
    KKSEIOData();
    KKSEIOData(const KKSEIOData & copy);
    ~KKSEIOData();
    
   
    int fieldsCount() const;

    QString fieldValue(const QString & code) const;
    QString & fieldValue(const QString & code);

    const QMap<QString, QString> & fields() const;
    QMap<QString, QString> & fields();

    int addField(const QString & code, const QString & value);
    void setFields(const QMap<QString, QString> & fields);
    int removeField(const QString & code);
    int updateField(const QString & code, const QString & value);

    int sysFieldsCount() const;

    QString sysFieldValue(const QString & code) const;
    QString & sysFieldValue(const QString & code);

    const QMap<QString, QString> & sysFields() const;
    QMap<QString, QString> & sysFields();

    int addSysField(const QString & code, const QString & value);
    void setSysFields(const QMap<QString, QString> & fields);
    int removeSysField(const QString & code);
    int updateSysField(const QString & code, const QString & value);

    void clear();
    bool isVisible (void) const;
    bool setVisible (bool v);

protected:
private:
    
    QMap<QString, QString> m_fields;
    QMap<QString, QString> m_sysFields;

    QString m_null;
    bool visible;
};

typedef KKSMap<qint64, KKSEIOData *> KKSEIODataMap;
typedef KKSEIOData * pKKSEIOData;

Q_DECLARE_METATYPE (KKSEIOData);
Q_DECLARE_METATYPE (pKKSEIOData);
Q_DECLARE_METATYPE (KKSEIODataMap);

#endif
