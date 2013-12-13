/* 
 * File:   KKSHistWidget.h
 * Author: yuriyrusinov
 *
 * Created on 12 Декабрь 2013 г., 16:57
 */

#ifndef _KKSHISTWIDGET_H
#define	_KKSHISTWIDGET_H

#include <QWidget>

#include "kksgui_config.h"
#include "KKSAttrWidget.h"

class QPaintEvent;

class _GUI_EXPORT KKSHistWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSHistWidget(const KKSAttrValue *av, KKSIndAttrClass isSys, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSHistWidget();

protected:
    //
    // Override functions of basic class
    //
    void paintEvent(QPaintEvent *event);

private:
    Q_OBJECT
};

#endif	/* _KKSHISTWIDGET_H */
