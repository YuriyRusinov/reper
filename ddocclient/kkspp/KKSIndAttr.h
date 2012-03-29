/* 
 * File:   KKSIndAttr.h
 * Author: yuriyrusinov
 *
 * Created on 24 Февраля 2012 г., 17:47
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

    virtual bool isSystem() const;
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

