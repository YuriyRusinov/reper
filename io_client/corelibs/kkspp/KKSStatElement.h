/***********************************************************************
 * Module:  KKSType.h
 * Author:  sergey
 * Modified: 25 но€бр€ 2008 г. 17:46:06
 * Purpose: Declaration of the class KKSType
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSStatElement_h)
#define __KKSSITOOOM_KKSStatElement_h

#include <QDateTime>
#include <KKSData.h>
#include "kkspp_config.h"

class _PP_EXPORT KKSStatElement : public KKSData
{
public:

    enum KKSStatOperation
    {
        soUndefined = 0,
        soInsertIO = 1,
        soUpdateIO = 2,
        soInsertAttr = 3,
        soRemoveAttr = 4,
        soUpdateAttrList = 5
    };
    
    KKSStatElement();
    KKSStatElement(const KKSStatElement & t);
    KKSStatElement(int idUser, 
                   const QString & userName, 
                   KKSStatOperation operType, 
                   QDateTime operTime);
    virtual ~KKSStatElement();

    const QString & userName() const;
    KKSStatOperation operType() const;
    QString operTypeName() const;
    const QDateTime & operTime() const;

protected:
private:
    int m_idUser;
    QString m_userName;
    KKSStatOperation m_operType;
    QDateTime m_operTime;

};

#endif
