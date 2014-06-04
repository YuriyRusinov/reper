/***********************************************************************
 * Module:  KKSAttrAttr.h
 * Author:  sergey
 * Modified: 02 апреля 2012 г. 12:12:50
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

    //метод создает экземпляр данного класса по заданным параметрам
    //если строковое представление значения по умолчанию в данном атрибуте
    //не удалось привести к типу атрибута, то bBadValue устанавливается в TRUE
    //в противном случае в FALSE
    static KKSAttrAttr * create(KKSAttribute * a, 
                                bool bMandatory, 
                                bool bReadOnly, 
                                const QString & defValue = QString::null,
                                bool * bBadValue = NULL);


protected:
private:

    int m_id;//id в таблице attrs_attrs
    int m_idParentAttr;//идентификуатор описываемого атрибута

    bool m_isMandatory;
    bool m_isReadOnly;
    KKSValue m_defValue;

};
*/
#endif
