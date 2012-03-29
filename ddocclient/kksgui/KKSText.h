/***********************************************************************
 * Module:  KKSText.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:26
 * Purpose: Declaration of the class KKSEdit
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSText_h)
#define __KKSSITOOOM_KKSText_h

#include <QTextEdit>
#include <QVariant>

#include <KKSAttrWidget.h>
#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSText : public QTextEdit, public KKSAttrWidget
{
    public:
        KKSText (const KKSAttrValue*attr, bool isSys, QWidget *parent=NULL);
        KKSText (const KKSAttrValue*attr, bool isSys, const QString& contents, QWidget *parent=NULL);
        virtual ~KKSText (void);

    signals:
        void valueChanged (int id, bool isSys, QVariant val);

    public slots:
        
        void setFixedSymCount(int count);
        void setVal (const QString & newText);
    
    private slots:
        void setValPrivate();

    private:
        //
        // Variables
        //
//        const KKSIndAttr *attribute;
//        bool isSystem;
        int m_fixedSymCount;

    private:
        Q_OBJECT
};

#endif
