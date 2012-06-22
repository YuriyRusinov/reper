/***********************************************************************
 * Module:  KKSAttrRefWidget.h
 * Author:  yuriyrusinov
 * Modified: 07 ��� 2008 �. 11:10
 * Purpose: Declaration of the class KKSAttrRefWidget
 ***********************************************************************/
#if !defined(__KKSSITOOOM_KKSAttrRefWidget_h)
#define __KKSSITOOOM_KKSAttrRefWidget_h

#include "kksgui_config.h"

#include <QWidget>
#include <QVariant>

#include "KKSIndAttr.h"

class QGridLayout;

class _GUI_EXPORT KKSAttrRefWidget : public QWidget
{
    public:
        KKSAttrRefWidget (QWidget *parent=0, Qt::WindowFlags f=0);
        ~KKSAttrRefWidget (void);

        void setAttrWidget (QWidget *aw);
        QWidget * getAttrWidget (void) const;
        QGridLayout * getLayout (void) const;
        int getIdAttrValue (void) const;
        KKSIndAttr::KKSIndAttrClass isSysAttr (void) const;
        const QVariant& getValue (void) const;

    public slots:
        void setValue (int id, KKSIndAttr::KKSIndAttrClass sys, QVariant val);

    signals:
        void valueChanged (int id, KKSIndAttr::KKSIndAttrClass sys, QVariant val);

    private:
        //
        // Variables
        //
        QWidget * refWidget;
        QGridLayout * gAttrLay;
        int idAttrValue;
        QVariant cVal;
        KKSIndAttr::KKSIndAttrClass isSystem;

    private:
        Q_OBJECT
};

#endif
