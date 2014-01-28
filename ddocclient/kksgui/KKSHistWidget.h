/* 
 * File:   KKSHistWidget.h
 * Author: yuriyrusinov
 *
 * Created on 12 ������� 2013 �., 16:57
 */

#ifndef _KKSHISTWIDGET_H
#define	_KKSHISTWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QMap>
#include <KKSMap.h>

#include "kksgui_config.h"
#include "KKSAttrWidget.h"

class QPaintEvent;

class QGroupBox;
class QLineEdit;
class QComboBox;
class QPushButton;
class QModelIndex;

class KKSCharts;
class KKSCategory;
class KKSObject;
class KKSHistogram;
class KKSHistDrawWidget;
class KKSValue;

namespace Ui
{
    class histogram_widget;
};

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
    
    void loadScenario (const QMap<int, QString>& scList);
    void loadVariants (const QMap<int, QString>& varList);
    void loadCategories (const KKSMap<int, KKSCategory *>& catList);
    void loadIOList (const KKSMap<int, KKSObject *>& IOList);
    void clearIO (void);
    
    KKSValue getVal (void);

    void init (void);

public slots:
    void setHist (const KKSHistogram& hist);
    void calcHist (void);
    void saveHist (KKSValue & v);
    
private slots:
/*    void catChanged (int cIndex);
    void ioChanged (int ioIndex);
*/
    void scenarioSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void variantSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void catSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void ioSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void servSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);

protected:
    //
    // Override functions of basic class
    //
    //void paintEvent(QPaintEvent *event);

signals:
    void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);
    void loadCategory (int idCat, KKSHistogram * h);
    void loadIO (int idIO, KKSHistogram * h);
    void updateVarsList (QComboBox *, FiltersType);
    void updateWidgetPars (KKSHistWidget *);

private:
    //
    // Functions
    //

    
private:
    //
    // Variables
    //
    KKSHistogram * hist;
    Ui::histogram_widget *UI;
 
    QWidget * wHistDrawW;

private:
    Q_OBJECT
};

#endif	/* _KKSHISTWIDGET_H */
