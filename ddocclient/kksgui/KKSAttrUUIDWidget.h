/***********************************************************************
 * Module:  KKSAttrUUIDWidget.h
 * Author:  yuriyrusinov
 * Modified: 10 августа 2012 г. 17:59
 * Purpose: Declaration of the class KKSAttrUUIDWidget
 ***********************************************************************/
#if !defined(__KKSSITOOOM_KKSAttrUUIDWidget_h)
#define __KKSSITOOOM_KKSAttrUUIDWidget_h

#include "kksgui_config.h"

#include <QWidget>
#include <QVariant>

#include "KKSIndAttr.h"

class QGridLayout;
class QCheckBox;
class QToolButton;
class QLineEdit;

class _GUI_EXPORT KKSAttrUUIDWidget : public QWidget
{
public:
    KKSAttrUUIDWidget (QWidget *parent=0, Qt::WindowFlags f=0);
    ~KKSAttrUUIDWidget (void);

    int getIdAttrValue (void) const;
    KKSIndAttr::KKSIndAttrClass isSysAttr (void) const;
    const QVariant& getValue (void) const;
    QToolButton * getButton (void) const;
    Qt::CheckState checkState (void) const;

public slots:
    void setVal (int id, KKSIndAttr::KKSIndAttrClass sys, QVariant val);
    void setUUID (QString uuid);
    void generate_uuid (void);

signals:
    void valueChanged (int id, KKSIndAttr::KKSIndAttrClass sys, QVariant val);
    void generateUUID (int id);

private:
    //
    // Variables
    //
    QLineEdit * uuidWidget;
    QToolButton * tbGen;
    QCheckBox * chAuto;
    QGridLayout * gAttrLay;
    int idAttrValue;
    QVariant cVal;
    KKSIndAttr::KKSIndAttrClass isSystem;

private:
    Q_OBJECT
};

#endif
