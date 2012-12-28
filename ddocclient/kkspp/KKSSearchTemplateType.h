/* 
 * File:   KKSSearchTemplateType.h
 * Author: rusinov
 *
 * Created on 28 Декабрь 2012 г., 15:01
 */

#ifndef KKSSEARCHTEMPLATETYPE_H
#define	KKSSEARCHTEMPLATETYPE_H

#include <QMetaType>
#include <QVariant>

#include "kkspp_config.h"
#include "KKSRecord.h"

class _PP_EXPORT KKSSearchTemplateType
{
public:
    KKSSearchTemplateType();
    KKSSearchTemplateType(const KKSSearchTemplateType& stt);
    virtual ~KKSSearchTemplateType();
private:

};

Q_DECLARE_METATYPE (KKSSearchTemplateType *);
Q_DECLARE_METATYPE (const KKSSearchTemplateType *)

#endif	/* KKSSEARCHTEMPLATETYPE_H */

