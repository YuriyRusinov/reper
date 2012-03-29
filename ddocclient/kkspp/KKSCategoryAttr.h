/***********************************************************************
 * Module:  KKSCategoryAttr.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:12:50
 * Purpose: Declaration of the class KKSCategoryAttr
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCategoryAttr_h)
#define __KKSSITOOOM_KKSCategoryAttr_h

#include "KKSValue.h"

#include <KKSAttribute.h>
#include "kkspp_config.h"

class _PP_EXPORT KKSCategoryAttr : public KKSAttribute
{
public:
   bool isMandatory(void) const;
   void setMandatory(bool newIsMandatory);
   bool isReadOnly(void) const;
   void setReadOnly(bool newIsReadOnly);

   const KKSValue & defValue() const;
   KKSValue  & defValue();
   void setDefValue(const KKSValue & _defValue);

   KKSCategoryAttr();
   KKSCategoryAttr(const KKSAttribute & a);
   KKSCategoryAttr(const KKSCategoryAttr & a);
   virtual ~KKSCategoryAttr();

    //метод создает экземпляр данного класса по заданным параметрам
    //если строковое представление значения по умолчанию в данном атрибуте
    //не удалось привести к типу атрибута, то bBadValue устанавливается в TRUE
    //в противном случае в FALSE
    static KKSCategoryAttr * create(KKSAttribute * a, 
                                    bool bMandatory, 
                                    bool bReadOnly, 
                                    const QString & defValue = QString::null,
                                    bool * bBadValue = NULL);


protected:
private:

   bool m_isMandatory;
   bool m_isReadOnly;
   
   KKSValue m_defValue;


};

#endif
