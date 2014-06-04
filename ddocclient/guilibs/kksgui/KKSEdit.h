/***********************************************************************
 * Module:  KKSEdit.h
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:13:26
 * Purpose: Declaration of the class KKSEdit
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSEdit_h)
#define __KKSSITOOOM_KKSEdit_h

#include <QLineEdit>
#include <QVariant>

#include <KKSAttrWidget.h>
#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSEdit : public QLineEdit, public KKSAttrWidget
{
    public:
        KKSEdit (const KKSAttrValue *attr, KKSIndAttrClass isSys, QWidget *parent=NULL);
        KKSEdit (const KKSAttrValue *attr, KKSIndAttrClass isSys, const QString& contents, QWidget *parent=NULL);
        virtual ~KKSEdit (void);

    signals:
        void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);

    public slots:
        void setVal (const QString & newText);
        void setFixedSymCount(int count);

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
