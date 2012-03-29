/***********************************************************************
 * Module:  KKSAttrValue.h
 * Author:  sergey
 * Modified: 27 но€бр€ 2008 г. 14:01:33
 * Purpose: Declaration of the class KKSAttrValue
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrValue_h)
#define __KKSSITOOOM_KKSAttrValue_h

#include "KKSValue.h"
#include "kkspp_config.h"
#include "KKSData.h"
#include <QDateTime>

class KKSCategoryAttr;
class KKSObject;

class _PP_EXPORT KKSAttrValue : public KKSData
{
    public:
        KKSAttrValue();
        KKSAttrValue(const KKSAttrValue & av);
        KKSAttrValue(const KKSValue & v, KKSCategoryAttr * a);
        virtual ~KKSAttrValue();

        int id() const;
        void setId(int _id);

        const KKSValue & value() const;
        KKSValue & value();
        void setValue(const KKSValue & value);

        const QString & desc() const;
        QString & desc();
        void setDesc(const QString & desc);


        const KKSCategoryAttr * attribute() const;
        KKSCategoryAttr * attribute();
        void setAttribute(KKSCategoryAttr * attr);

        KKSObject * ioSrc();
        const KKSObject * ioSrc() const;
        void setIOSrc(KKSObject * io);

        KKSObject * ioSrc1();
        const KKSObject * ioSrc1() const;
        void setIOSrc1(KKSObject * io);

        const QDateTime & measDateTime() const;
        void setMeasDateTime(const QDateTime & dt);

        const QDateTime & insertDateTime() const;
        void setInsertDateTime(const QDateTime & dt);

        const QDateTime & startDateTime() const;
        void setStartDateTime(const QDateTime & dt);
        const QDateTime & stopDateTime() const;
        void setStopDateTime(const QDateTime & dt);

        bool isActual() const;
        void setActual(bool a);

        bool isValid() const;

        static void initDefId();
        static int getDefId();
        static void decDefId();

        


    protected:
    private:
        int m_id;
        KKSValue m_value;
        KKSCategoryAttr* m_attribute;
        KKSObject * m_ioSrc;
        KKSObject * m_ioSrc1;
        QDateTime m_measDateTime;
        QDateTime m_insertDateTime;
        QDateTime m_startDateTime;
        QDateTime m_stopDateTime;
        QString m_desc;

        bool m_isActual;

        bool m_valid;
        void verify();

        

};

#endif
