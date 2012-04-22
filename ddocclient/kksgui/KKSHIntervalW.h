/***********************************************************************
 * Module:  KKSHIntervalW.h
 * Author:  yuriy
 * Modified: 25 но€бр€ 2010 г. 13:20
 * Purpose: Declaration of the class KKSHIntervalW
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSHIntervalW_h)
#define __KKSSITOOOM_KKSHIntervalW_h

#include <KKSAttrWidget.h>
#include <QPixmap>
#include <QLabel>
#include <QVariant>

#include "kksgui_config.h"

class QSpinBox;
class QHBoxLayout;

class KKSAttrValue;

class _GUI_EXPORT KKSHIntervalW : public QWidget, public KKSAttrWidget
{
public:
    KKSHIntervalW (const KKSAttrValue *attr, int isSys, QWidget * parent=0, Qt::WindowFlags f=0);
    virtual ~KKSHIntervalW (void);

public slots:
    void setValue (int h=-1, int m=-1, int s=-1);

private slots:
    void hourChanged (int hour);
    void minChanged (int min);
    void secChanged (int sec);

signals:
    void valueChanged (int id, int isSys, QVariant val);

private:
    //
    // Variables
    //
//    const KKSIndAttr *attribute;
//    int isSystem;

    QSpinBox * spHour;
    QSpinBox * spMinute;
    QSpinBox * spSecond;

    QHBoxLayout * hLay;

private:
    Q_OBJECT
};

#endif
