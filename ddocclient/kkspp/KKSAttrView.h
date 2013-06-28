/***********************************************************************
 * Module:  KKSAttrView.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:12:32
 * Purpose: Declaration of the class KKSAttrView
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrView_h)
#define __KKSSITOOOM_KKSAttrView_h

#include "KKSValue.h"

#include <KKSAttribute.h>
#include "kkspp_config.h"

class _PP_EXPORT KKSAttrView : public KKSCategoryAttr
{
    public:
        bool isReadOnly(void) const;
        void setReadOnly(bool newIsReadOnly);
        int order(void) const;
        void setOrder(int newOrder);
        bool isVisible(void) const;
        void setVisible(bool newIsVisible);

        void setDefValue(const KKSValue & v);
        const KKSValue & defValue() const;
        KKSValue & defValue();

       int idViewAttr() const;
       int idViewAttr();
       void setIdViewAttr(int id);

   
        KKSAttrView();
        KKSAttrView(const KKSAttrView & av);
        KKSAttrView(const KKSCategoryAttr & ca);
        virtual ~KKSAttrView();

    protected:
    private:
        bool m_readOnly;
        int m_order;
        bool m_visible;

        int m_idViewAttr;
   
        KKSValue m_defValue;


};

inline bool compareAttrViews(KKSAttrView * item1, KKSAttrView * item2)
{
    if(item1->order() < item2->order()) 
        return true; 
    
    return false;
}

#endif
