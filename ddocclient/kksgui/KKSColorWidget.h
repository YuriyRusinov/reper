/***********************************************************************
 * Module:  KKSColorWidget.h
 * Author:  yuriy_rusinov
 * Modified: 14 ÿםגאנÿ 2011 ד. 17:50
 * Purpose: Declaration of the class KKSColorWidget
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSColorWidget_h)
#define __KKSSITOOOM_KKSColorWidget_h

#include <QWidget>
#include <QVariant>
#include <QColor>
#include "KKSAttrWidget.h"
#include "kksgui_config.h"

class QLabel;
class QToolButton;

class KKSAttrValue;

class _GUI_EXPORT KKSColorWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSColorWidget (const KKSAttrValue *attr, bool isSys, QColor rgb_col, QWidget *parent=NULL, Qt::WindowFlags flags=0);
    virtual ~KKSColorWidget (void);

    void hideToolButton (void);
    void showToolButton (void);
    void setColor (QColor rgb_col);

signals:
    void valueChanged (int id, bool isSys, QVariant val);

public slots:
    void setColor (void);

private:
    //
    // Variables
    //
//    const KKSIndAttr *attribute;
//    bool isSystem;

    QLabel * lSampleText;
    QToolButton * tbColor;

private:
    Q_OBJECT
};

#endif
