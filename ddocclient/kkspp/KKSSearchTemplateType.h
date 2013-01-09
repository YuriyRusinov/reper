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
    KKSSearchTemplateType();
    KKSSearchTemplateType(const KKSSearchTemplateType& stt);
    virtual ~KKSSearchTemplateType();
private:

};

Q_DECLARE_METATYPE (KKSSearchTemplateType *);
Q_DECLARE_METATYPE (const KKSSearchTemplateType *);

#endif	/* KKSSEARCHTEMPLATETYPE_H */

