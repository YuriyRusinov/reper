/* 
 * File:   KKSIndAttr.h
 * Author: yuriyrusinov
 *
 * Created on 24 ‘еврал€ 2012 г., 17:47
 */

#ifndef _KKSINDATTR_H
#define	_KKSINDATTR_H

#include <QMetaType>

class KKSAttrType;

#include "kkspp_config.h"
#include <KKSRecord.h>
#include "KKSMap.h"
#include "KKSList.h"

class KKSSearchTemplate;
class KKSAttribute;

class _PP_EXPORT KKSIndAttr : public KKSRecord
{
public:
    KKSIndAttr();
    KKSIndAttr(int id, const QString & name, const QString & desc = QString(), const QString & code = QString());
    KKSIndAttr(const KKSIndAttr& ia);
    virtual ~KKSIndAttr();

    //заменитель старого атрибута isSystem.
    //јтрибуты бывают 3-х классов: 
    //0 - табличные. Ёто атрибуты записей справочников (т.е. колонки таблиц).   ним не примен€етс€ темпоральна€ модель и расширенные характеристики
    //1 - пользовательские атрибуты информационных объектов (их значени€ хран€тс€ в таблице attrs_values).   ним примен€етс€ темпоральна€ модель и расширенные характеристики
    //2 - пользовательские атрибуты (показатели) записей справочников(их значени€ хран€тс€ в таблице rec_attrs_values).   ним примен€етс€ темпоральна€ модель и расширенные характеристики
    //3 - системные атрибуты дл€ записей справочников (их значени€ хран€тс€ в базовых таблицах (root_table и т.п.), а визуализируютс€ они в отдельной вкладке)
    //4 - атрибуты, вход€щие в состав составных атрибутов
    enum KKSIndAttrClass 
    {
        iacTableAttr = 0,
        iacIOUserAttr = 1,
        iacEIOUserAttr = 2,
        iacEIOSysAttr = 3,
        iacAttrAttr = 4
    };
    
    const QString & refColumnName(bool quoted = false) const;
    void setRefColumnName(const QString & newColumnName);

    const QString & columnName(bool quoted = false) const;
    void setColumnName(const QString & newColumnName);
    const QString & tableName(void) const;
    void setTableName(const QString & newTableName);

    const KKSAttrType * type() const;
    KKSAttrType * type();
    void setType(KKSAttrType * _type);

    const KKSAttrType * refType() const;
    KKSAttrType * refType();
    void setRefType(KKSAttrType * _type);

    const KKSAttrType * refColumnType() const;
    KKSAttrType * refColumnType();
    void setRefColumnType(KKSAttrType * _type);

    virtual KKSIndAttrClass isSystem() const;
    virtual bool isAttribute (void) const=0;

private:
    QString m_tableName;
    QString m_columnName;

    QString m_refColumnName;
    KKSAttrType * m_refColumnType;

    mutable QString m_quotedColumnName;
    mutable QString m_quotedRefColumnName;

    KKSAttrType* m_type;
    KKSAttrType* m_refType;

};

#endif	/* _KKSINDATTR_H */

