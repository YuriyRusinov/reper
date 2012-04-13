/***********************************************************************
 * Module:  KKSType.h
 * Author:  sergey
 * Modified: 25 ноября 2008 г. 17:46:06
 * Purpose: Declaration of the class KKSType
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSType_h)
#define __KKSSITOOOM_KKSType_h

#include <KKSRecord.h>
#include "kkspp_config.h"

class _PP_EXPORT KKSType : public KKSRecord
{
public:
    KKSType();
    KKSType(const KKSType & t);
    KKSType(int id, const QString & name, const QString & desc = QString::null);
    virtual ~KKSType();

    static KKSType * defType1();//использовать только для типов ИО (не для типов категорий ИО) !!!
    
    bool isQualifier() const; //используется только для типов категорий
    void setAsQualifier(bool yes);

    void setRName(const QString & rName);
    const QString & rName() const;

protected:
private:

    bool m_isQualifier;//определяет является ли категория данного типа справочником (т.е. может ли содержать подчиненные таблицы)
    QString m_rName;

};

#endif
