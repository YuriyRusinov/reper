/* 
 * File:   KKSSearchTemplateType.h
 * Author: rusinov
 *
 * Created on 28 Декабрь 2012 г., 15:01
 */

#ifndef KKSSEARCHTEMPLATE_TYPE_H
#define	KKSSEARCHTEMPLATE_TYPE_H

#include <QMetaType>
#include <QVariant>

#include "kkspp_config.h"
#include "KKSRecord.h"

class _PP_EXPORT KKSSearchTemplateType : public KKSRecord
{
public:
    KKSSearchTemplateType(int id=-1, const QString & name=QString(), const QString & desc=QString());
    KKSSearchTemplateType (const KKSSearchTemplateType & stt);
    ~KKSSearchTemplateType (void);

    const KKSSearchTemplateType * parent() const;
    KKSSearchTemplateType * parent();
    void setParent( KKSSearchTemplateType * p);

    static KKSSearchTemplateType * defaultType();

private:
    KKSSearchTemplateType * m_parent;
};

Q_DECLARE_METATYPE (KKSSearchTemplateType *);
Q_DECLARE_METATYPE (const KKSSearchTemplateType *);

#endif	/* KKSSEARCHTEMPLATE_TYPE_H */
