/***********************************************************************
 * Module:  KKSHSvgWidget.h
 * Author:  yuriy
 * Modified: 25 но€бр€ 2010 г. 19:20
 * Purpose: Declaration of the class KKSSvgWidget
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSSvgWidget_h)
#define __KKSSITOOOM_KKSSvgWidget_h

#include <QWidget>
#include <QByteArray>
#include <QVariant>

#include "kksgui_config.h"
#include "KKSAttrWidget.h"

class QSvgRenderer;
class QPaintEvent;
class QWheelEvent;

class KKSAttrValue;

class _GUI_EXPORT KKSSvgWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSSvgWidget (const KKSAttrValue*attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget *parent=NULL, Qt::WindowFlags flags=0);
    KKSSvgWidget (const KKSAttrValue*attr, KKSIndAttr::KKSIndAttrClass isSys, const QByteArray& contents, QWidget *parent=NULL, Qt::WindowFlags flags=0);
    ~KKSSvgWidget (void);

    const QByteArray& getContents (void) const;
    void setContents (const QByteArray& c);

    virtual QSize sizeHint (void) const;

public slots:
    void setVal (const QByteArray & newSVG);
    void openFile (void);

private slots:
    void updateSVG (void);

signals:
    void valueChanged (int id, KKSIndAttr::KKSIndAttrClass isSys, QVariant val);

protected:
    virtual void paintEvent (QPaintEvent *event);
    virtual void wheelEvent (QWheelEvent *event);

private:
    //
    // Variables
    //
//    const KKSIndAttr *attribute;
//    int isSystem;
    QByteArray SVGContents;
    QSvgRenderer * dRend;

private:
    Q_OBJECT
};

#endif
