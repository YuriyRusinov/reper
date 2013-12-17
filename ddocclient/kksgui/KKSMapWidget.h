/***********************************************************************
 * Module:  KKSMapWidget.h
 * Author:  sergey
 * Modified: 13 декабря 2013 г. 14:46:21
 * Purpose: Declaration of the class KKSMapWidget
 * виджет для работы с картой
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSMapWidget_h)
#define __KKSSITOOOM_KKSMapWidget_h

#include "kksgui_config.h"

#include <KKSAttrWidget.h>
#include <QString>
#include <QList>
#include <QListWidget>
#include <QVariant>
#include <QDockWidget>


class KKSAttrValue;

//#ifdef __USE_QGIS__
//class KKSGISWidget;
//#endif

class _GUI_EXPORT KKSMapWidget : public QWidget, public KKSAttrWidget
{
    Q_OBJECT
public:
    KKSMapWidget(const KKSAttrValue* attr = 0, KKSIndAttrClass isSys = iacIOUserAttr, QWidget *parent=NULL);
    virtual ~KKSMapWidget(void);

    QDockWidget * legendWidget() const;
    QDockWidget * layerOrderWidget() const;

signals:
    void valueChanged (qint64 id, KKSIndAttrClass sys, QVariant val);
    void aboutToDestroy(QDockWidget *, QDockWidget *);

private:
    QDockWidget * m_legendWidget;
    QDockWidget * m_layerOrderWidget;

//#ifdef __USE_QGIS__

//private:
//    void init(QWidget *parent);

//private:
//    KKSGISWidget * mpKKSGISWidget;
//#endif

};

#endif
