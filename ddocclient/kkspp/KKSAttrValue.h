/***********************************************************************
 * Module:  KKSAttrValue.h
 * Author:  sergey
 * Modified: 27 ������ 2008 �. 14:01:33
 * Purpose: Declaration of the class KKSAttrValue
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrValue_h)
#define __KKSSITOOOM_KKSAttrValue_h

#include "KKSValue.h"
#include "kkspp_config.h"
#include "KKSData.h"
#include <QDateTime>
#include "KKSMap.h"
#include <qglobal.h>

class KKSCategoryAttr;
class KKSObject;

class _PP_EXPORT KKSAttrValue : public KKSData
{
    public:
        KKSAttrValue();
        KKSAttrValue(const KKSAttrValue & av);
        KKSAttrValue(const KKSValue & v, KKSCategoryAttr * a);
        virtual ~KKSAttrValue();

        qint64 id() const;
        void setId(qint64 _id);

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
        static qint64 getDefId();
        static void decDefId();

        void setAttrsValues(const KKSMap<qint64, KKSAttrValue *> & attrsValues);
        const KKSMap<qint64, KKSAttrValue*> & attrsValues() const;
        KKSMap<qint64, KKSAttrValue*> & attrsValues();

        bool attrsValuesLoaded() const;
        


    protected:
    private:
        qint64 m_id; //id � ������� attrs_values ���  � attrs_attrs_values
        mutable KKSValue m_value;
        KKSCategoryAttr* m_attribute; //�� id_attr_category ��� �� id_attr_attr (�������������� ��. ������� attrs_values � attrs_attrs_values)


        KKSObject * m_ioSrc;
        KKSObject * m_ioSrc1;
        //QDateTime m_measDateTime;
        QDateTime m_insertDateTime;
        QDateTime m_startDateTime;
        QDateTime m_stopDateTime;
        QString m_desc;

        KKSMap<qint64, KKSAttrValue*> m_attrsValues;//������ �������� ����������� ���������, ���� ��������� ������� ������ ��������� ��������� �������. 
                                                             //��� ��������� ������ ������� ����������� ������ �� ��������� �������
                                                             //������� ���� m_attrsAttrsValuesLoaded, ������������ ���� ������������� ������ �������� ���������
                                                             //���� ������������� ������������ ���, ��������� �� ����� setAttrsAttrsValues(). 
                                                             //���� ���������, �� �������� ����������� ��������� ���� ���������
        bool m_attrsValuesLoaded;

        bool m_isActual;

        bool m_valid;
        void verify();

        

};

#endif
