/***********************************************************************
 * Module:  KKSTimeEdit.h
 * Author:  yuriy_rusinov
 * Modified: 14 ������ 2011 �. 15:30:31
 * Purpose: Declaration of the class KKSTimeEdit
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSTimeEdit_h)
#define __KKSSITOOOM_KKSTimeEdit_h

#include <KKSAttrWidget.h>
#include <QTimeEdit>
#include <QVariant>
#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSTimeEdit : public QTimeEdit, public KKSAttrWidget
{
    public:
        KKSTimeEdit (const KKSAttrValue*attr, bool isSys, QWidget *parent=NULL);
        KKSTimeEdit (const KKSAttrValue*attr, bool isSys, const QTime& time, QWidget * parent=NULL);
        virtual ~KKSTimeEdit (void);

    signals:
        void valueChanged (int id, bool isSys, QVariant val);

    public slots:
        void setTime (const QTime& dt);

    private:
        //
        // Variables
        //
//        const KKSIndAttr *attribute;
//        bool isSystem;

    private:
        Q_OBJECT
};

#endif
