/***********************************************************************
 * Module:  KKSAttrAttrValue.h
 * Author:  sergey
 * Modified: 02 04 2012 ã. 14:01:33
 * Purpose: Declaration of the class KKSAttrAttrValue
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrAttrValue_h)
#define __KKSSITOOOM_KKSAttrAttrValue_h

#include "KKSValue.h"
#include "kkspp_config.h"
#include "KKSData.h"
#include <QDateTime>

class KKSAttrAttr;
//class KKSObject;

class _PP_EXPORT KKSAttrAttrValue : public KKSData
{
    public:
        KKSAttrAttrValue();
        KKSAttrAttrValue(const KKSAttrAttrValue & av);
        KKSAttrAttrValue(const KKSValue & v, KKSAttrAttr * a);
        virtual ~KKSAttrAttrValue();

        int id() const;
        void setId(int _id);

        const KKSValue & value() const;
        KKSValue & value();
        void setValue(const KKSValue & value);

/*
        const QString & desc() const;
        QString & desc();
        void setDesc(const QString & desc);
*/

        const KKSAttrAttr * attribute() const;
        KKSAttrAttr * attribute();
        void setAttribute(KKSAttrAttr * attr);

/*
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
*/
        bool isValid() const;

/*
        static void initDefId();
        static int getDefId();
        static void decDefId();
*/
        


    protected:
    private:
        int m_id;
        KKSValue m_value;
        KKSAttrAttr* m_attribute;

/*
        KKSObject * m_ioSrc;
        KKSObject * m_ioSrc1;
        QDateTime m_measDateTime;
        QDateTime m_insertDateTime;
        QDateTime m_startDateTime;
        QDateTime m_stopDateTime;
        QString m_desc;

        bool m_isActual;
*/

        bool m_valid;
        void verify();

        

};

#endif
