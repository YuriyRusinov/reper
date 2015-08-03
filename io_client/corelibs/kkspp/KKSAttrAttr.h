/***********************************************************************
 * Module:  KKSAttrAttr.h
 * Author:  sergey
 * Modified: 02 ������ 2012 �. 12:12:50
 * Purpose: Declaration of the class KKSAttrAttr
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrAttr_h)
#define __KKSSITOOOM_KKSAttrAttr_h

#include "KKSValue.h"

#include <KKSAttribute.h>
#include "kkspp_config.h"
/*
class _PP_EXPORT KKSAttrAttr : public KKSAttribute
{
public:
    bool isMandatory(void) const;
    void setMandatory(bool newIsMandatory);
    bool isReadOnly(void) const;
    void setReadOnly(bool newIsReadOnly);

    int idParentAttr();
    int idParentAttr() const;
    void setIdParentAttr(int id);

    const KKSValue & defValue() const;
    KKSValue  & defValue();
    void setDefValue(const KKSValue & _defValue);

    KKSAttrAttr();
    KKSAttrAttr(const KKSAttribute & a);
    KKSAttrAttr(const KKSAttrAttr & a);
    virtual ~KKSAttrAttr();

    //����� ������� ��������� ������� ������ �� �������� ����������
    //���� ��������� ������������� �������� �� ��������� � ������ ��������
    //�� ������� �������� � ���� ��������, �� bBadValue ��������������� � TRUE
    //� ��������� ������ � FALSE
    static KKSAttrAttr * create(KKSAttribute * a, 
                                bool bMandatory, 
                                bool bReadOnly, 
                                const QString & defValue = QString::null,
                                bool * bBadValue = NULL);


protected:
private:

    int m_id;//id � ������� attrs_attrs
    int m_idParentAttr;//�������������� ������������ ��������

    bool m_isMandatory;
    bool m_isReadOnly;
    KKSValue m_defValue;

};
*/
#endif
