/***********************************************************************
 * Module:  KKSAttrWidget.h
 * Author:  sergey
 * Modified: 23 марта 2012 г. 12:13:26
 * Purpose: Declaration of the class KKSAttrWidget
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSAttrWidget_h)
#define __KKSSITOOOM_KKSAttrWidget_h

#include "kksgui_config.h"
#include "KKSIndAttr.h"

class KKSAttrValue;

class _GUI_EXPORT KKSAttrWidget 
{
    public:
        KKSAttrWidget(const KKSAttrValue *av, KKSIndAttrClass isSys);
        virtual ~KKSAttrWidget (void);

        void setAttrValue(const KKSAttrValue * av);
        const KKSAttrValue * attrValue() const;

    protected:
        //
        // Variables
        //
        const KKSAttrValue *m_av;
        KKSIndAttrClass m_isSystem;

    //private:
        //Q_OBJECT
};

#endif
