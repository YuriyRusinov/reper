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

class _GUI_EXPORT KKSHistWidget : public QWidget
{
public:
    KKSHistWidget(QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSHistWidget();
private:
    Q_OBJECT
};

#endif	/* _KKSHISTWIDGET_H */
