/***********************************************************************
 * Module:  KKSDumper.h
 * Author:  gakonya
 * Modified: 13 декабря 2008 г. 12:59:59
 * Purpose: Declaration of the class KKSDumper
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSDumper_h)
#define __KKSSITOOOM_KKSDumper_h

#include "kksfactory_config.h"
#include <QString>

class KKSObject;
class KKSCategory;
class KKSObjectExemplar;
class KKSValue;
class KKSCategoryAttr;
class KKSAttrValue;
//class KKSLifeCycle;
class KKSState;
class KKSEIOData;
class KKSTemplate;
class KKSAttrGroup;
class KKSAttrView;

class _F_EXPORT KKSDumper
{
public:
    static QString dump(const KKSObject* io, const QString & prefix = "");
    static QString dump(const KKSCategory* c, const QString & prefix = "");
    static QString dump(const KKSObjectExemplar* eio, const QString & prefix = "");
    static QString dump(const KKSEIOData * eio, const QString & prefix = "");
    static QString dump(const KKSTemplate * t, const QString & prefix = "");

protected:
private:
    static QString dump(const KKSValue & v, const QString & prefix = "");
    static QString dump(const KKSAttrValue * av, const QString & prefix = "");
    static QString dump(const KKSCategoryAttr * ca, const QString & prefix = "");
    //static QString dump(const KKSLifeCycle * lc, const QString & prefix = "");
    static QString dump(const KKSState * s, const QString & prefix = "");
    static QString dump(const KKSAttrGroup * g, const QString & prefix = "");
    static QString dump(const KKSAttrView * av, const QString & prefix = "");

   KKSDumper();
   ~KKSDumper();


};

#endif
