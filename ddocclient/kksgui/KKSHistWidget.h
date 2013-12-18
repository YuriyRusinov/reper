/* 
 * File:   KKSHistWidget.h
 * Author: yuriyrusinov
 *
 * Created on 12 Декабрь 2013 г., 16:57
 */

#ifndef _KKSHISTWIDGET_H
#define	_KKSHISTWIDGET_H

#include <QWidget>
#include <QVariant>

#include "kksgui_config.h"
#include "KKSAttrWidget.h"

class QPaintEvent;

class QGroupBox;
class QLineEdit;
class QComboBox;

class KKSCharts;

class _GUI_EXPORT KKSHistWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSHistWidget(const KKSAttrValue *av, KKSIndAttrClass isSys, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSHistWidget();

    enum FiltersType
    {
        tScenario = 0,
        tVariant = 1,
        tCategory = 2,
        tSource = 3,
        tReceiver = 4
    };
protected:
    //
    // Override functions of basic class
    //
    void paintEvent(QPaintEvent *event);

signals:
    void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);
    void updateVarsList (QComboBox *, FiltersType);
    void updateWidgetPars (KKSHistWidget *);

private:
    //
    // Functions
    //
    void init (void);
    
private:
    //
    // Variables
    //
    QLineEdit * lEFrom;
    QLineEdit * lETo;
    QLineEdit * lECount;
    
    QComboBox * cbScenario;
    QComboBox * cbVariant;
    QComboBox * cbCategory;
    QComboBox * cbSource;
    QComboBox * cbReceiver;
    
    KKSCharts * wCharts;
private:
    Q_OBJECT
};

#endif	/* _KKSHISTWIDGET_H */
