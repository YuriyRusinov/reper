/***********************************************************************
 * Module:  KKSCategoryAttr.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:12:50
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

   int idCategoryAttr() const;
   int idCategoryAttr();
   void setIdCategoryAttr(int id);

   KKSCategoryAttr();
   KKSCategoryAttr(const KKSAttribute & a);
   KKSCategoryAttr(const KKSCategoryAttr & a);
   virtual ~KKSCategoryAttr();

    //����� ������� ��������� ������� ������ �� �������� ����������
    //���� ��������� ������������� �������� �� ��������� � ������ ��������
    //�� ������� �������� � ���� ��������, �� bBadValue ��������������� � TRUE
    //� ��������� ������ � FALSE
    static KKSCategoryAttr * create(KKSAttribute * a, 
                                    bool bMandatory, 
                                    bool bReadOnly, 
                                    const QString & defValue = QString::null,
                                    bool * bBadValue = NULL);


protected:
private:

   bool m_isMandatory;
   bool m_isReadOnly;
   int m_idCategoryAttr;
   
   KKSValue m_defValue;


};

Q_DECLARE_METATYPE (KKSCategoryAttr);
Q_DECLARE_METATYPE (KKSCategoryAttr *);

#endif
