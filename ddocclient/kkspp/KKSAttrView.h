/***********************************************************************
 * Module:  KKSAttrView.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:12:32
 * Purpose: Declaration of the class KKSAttrView
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrView_h)
#define __KKSSITOOOM_KKSAttrView_h

#include "KKSValue.h"

#include <KKSCategoryAttr.h>
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
   
        KKSAttrView();
        KKSAttrView(const KKSAttrView & av);
        KKSAttrView(const KKSCategoryAttr & ca);
        virtual ~KKSAttrView();

    protected:
    private:
        bool m_readOnly;
        int m_order;
        bool m_visible;
   
        KKSValue m_defValue;


};

inline bool compareAttrViews(KKSAttrView * item1, KKSAttrView * item2)
{
    if(item1->order() < item2->order()) 
        return true; 
    
    return false;
}

#endif
