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
#include <KKSAttrType.h>
#include "kksgui_config.h"

class QLabel;
class QToolButton;

class KKSAttrValue;

class _GUI_EXPORT KKSColorWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSColorWidget (const KKSAttrValue *attr, 
                    KKSIndAttrClass isSys, 
                    QColor rgb_col, 
                    KKSAttrType::KKSAttrTypes type,
                    QWidget *parent=NULL, 
                    Qt::WindowFlags flags=0);
    virtual ~KKSColorWidget (void);

    void hideToolButton (void);
    void showToolButton (void);
    void setColor (QColor rgb_col);

signals:
    void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);

public slots:
    void setColor (void);

private:
    //
    // Variables
    //
    QLabel * lSampleText;
    QToolButton * tbColor;
    KKSAttrType::KKSAttrTypes m_type;

private:
    Q_OBJECT
};

#endif
