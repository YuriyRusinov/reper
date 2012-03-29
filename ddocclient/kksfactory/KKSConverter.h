/***********************************************************************
 * Module:  KKSConverter.h
 * Author:  sergey
 * Modified: 16 декабря 2008 г. 12:09:29
 * Purpose: Declaration of the class KKSConverter
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSConverter_h)
#define __KKSSITOOOM_KKSConverter_h

#include "kksfactory_config.h"
#include "KKSList.h"
//#include "KKSMap.h"
#include "KKSAttrType.h"
#include "KKSAttrValue.h"
#include "KKSValue.h"

class KKSLoader;
class KKSObjectExemplar;
class KKSAttribute;
class KKSCategory;
class KKSObject;
class KKSType;
class KKSSyncType;
class KKSState;
class KKSTemplate;

class _F_EXPORT KKSConverter
{
public:

    static KKSObjectExemplar * objToExemplar(const KKSLoader * loader, const KKSObject * io);
    static KKSObjectExemplar * categoryToExemplar(const KKSLoader * loader, const KKSCategory * c);
    static KKSObjectExemplar * typeToExemplar(const KKSLoader * loader, const KKSType * t);
    static KKSObjectExemplar * ioTypeToExemplar(const KKSLoader * loader, const KKSType * t);
    static KKSObjectExemplar * syncTypeToExemplar(const KKSLoader * loader, const KKSSyncType * t);
    static KKSObjectExemplar * stateToExemplar(const KKSLoader * loader, const KKSState * s);
    static KKSObjectExemplar * attributeToExemplar(const KKSLoader * loader, const KKSAttribute * a);
    static KKSObjectExemplar * templateToExemplar(const KKSLoader * loader, const KKSTemplate * t);

    static bool objectFromExemplar(const KKSLoader * loader, KKSObject * io, const KKSObjectExemplar * eio);


protected:
private:
    KKSConverter();
   ~KKSConverter();

    static int appendAttrValue(   KKSCategory * tc, 
                                  KKSList<KKSAttrValue *> &attrValues, 
                                  int attrId, 
                                  const QString & attrValue, 
                                  KKSAttrType::KKSAttrTypes attrType);
    
    static int appendAttrValue(   KKSCategory * tc, 
                                  KKSList<KKSAttrValue *> &attrValues, 
                                  int attrId, 
                                  const KKSValue & attrValue);

};

#endif
