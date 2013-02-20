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
#include "KKSMap.h"

class KKSSearchTemplate;

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

    void clearSubTypes (void);
    void addSubType (KKSSearchTemplateType* childType);
    const KKSMap<qint64, KKSSearchTemplateType *>& getSubTypes () const;
    KKSMap<qint64, KKSSearchTemplateType *>& getSubTypes ();
    void setSubTypes (const KKSMap<qint64, KKSSearchTemplateType *>& sTypes);
   
    void clearTemplates (void);
    void addTemplate (KKSSearchTemplate * childTempl);
    const KKSMap<qint64, KKSSearchTemplate *>& getTemplates (void) const;
    KKSMap<qint64, KKSSearchTemplate *>& getTemplates (void);
    void setChildTemplates (const KKSMap<qint64, KKSSearchTemplate *>& sTemplates);

private:
    KKSSearchTemplateType * m_parent;
    
    KKSMap<qint64, KKSSearchTemplateType*> childTypes;
    KKSMap<qint64, KKSSearchTemplate *> childTemplates;
};

Q_DECLARE_METATYPE (KKSSearchTemplateType *);
Q_DECLARE_METATYPE (const KKSSearchTemplateType *);

Q_DECLARE_METATYPE (KKSSearchTemplate *);
Q_DECLARE_METATYPE (const KKSSearchTemplate *);

#endif	/* KKSSEARCHTEMPLATE_TYPE_H */
