/***********************************************************************
 * Module:  KKSAttrRefWidget.h
 * Author:  yuriyrusinov
 * Modified: 07 мая 2008 г. 11:10
 * Purpose: Declaration of the class KKSAttrRefWidget
 ***********************************************************************/
#if !defined(__KKSSITOOOM_KKSAttrRefWidget_h)
#define __KKSSITOOOM_KKSAttrRefWidget_h

#include "kksgui_config.h"

#include <QWidget>
#include <QVariant>

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
        int isSysAttr (void) const;
        const QVariant& getValue (void) const;

    public slots:
        void setValue (int id, int sys, QVariant val);

    signals:
        void valueChanged (int id, int sys, QVariant val);

    private:
        //
        // Variables
        //
        QWidget * refWidget;
        QGridLayout * gAttrLay;
        int idAttrValue;
        QVariant cVal;
        int isSystem;

    private:
        Q_OBJECT
};

#endif
