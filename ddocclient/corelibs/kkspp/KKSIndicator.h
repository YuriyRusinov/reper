/* 
 * File:   KKSIndicator.h
 * Author: ksa
 *
 * Created on 17 Февраль 2012 г., 13:39
 */

#ifndef _KKSINDICATOR_H
#define	_KKSINDICATOR_H

#include "kkspp_config.h"
#include "KKSRecord.h"
#include "KKSMap.h"
#include "KKSIndAttr.h"
#include "KKSAttrType.h"

class KKSObject;
class KKSAttrType;

class _PP_EXPORT KKSIndicatorType : public KKSRecord
{
public:
    //KKSIndicatorType ();
    //KKSIndicatorType (const KKSIndicatorType& IType);// : m_indType (IType.m_indType) {};
    virtual ~KKSIndicatorType (){}

};

class _PP_EXPORT KKSIndicator : public KKSIndAttr//KKSRecord
{
public:
    KKSIndicator (int id, const QString & name, const QString & desc = QString());
    KKSIndicator (const KKSIndicator & Ind);
    virtual ~KKSIndicator ();

    KKSIndicator * parent ();
    const KKSIndicator * parent () const;
    void setParent (KKSIndicator * parent);

    const KKSIndicatorType * indType() const;
    KKSIndicatorType * indType();
    void setIndType(KKSIndicatorType * _type);


    const KKSMap<int, KKSIndicator*>& getChildIndicators (void) const;
    KKSMap<int, KKSIndicator*>& getChildIndicators(void);
    void setChildIndicators (const KKSMap<int, KKSIndicator*>& childInds);

    virtual bool isAttribute (void) const;

private:
    KKSIndicator * m_parent;
    KKSIndicatorType * m_indType;
    
    KKSMap<int, KKSIndicator*> m_childIndicators;

};

#endif	/* _KKSINDICATOR_H */

