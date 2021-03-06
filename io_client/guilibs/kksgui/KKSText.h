/***********************************************************************
 * Module:  KKSText.h
 * Author:  sergey
 * Modified: 1 ������� 2008 �. 12:13:26
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
        KKSText (const KKSAttrValue*attr, KKSIndAttrClass isSys, QWidget *parent=NULL);
        KKSText (const KKSAttrValue*attr, KKSIndAttrClass isSys, const QString& contents, QWidget *parent=NULL);
        virtual ~KKSText (void);

    signals:
        void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);

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
//        int isSystem;
        int m_fixedSymCount;

    private:
        Q_OBJECT
};

#endif
