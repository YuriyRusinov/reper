/* 
 * File:   KKSHistWidgetEx.h
 * Author: ksa
 *
 * Created on 27.01.2014 �., 10:00
 */

#ifndef _KKSHISTWIDGETEX_H
#define	_KKSHISTWIDGETEX_H

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

//class KKSCharts;
class KKSCategory;
class KKSObject;
class KKSHistogram;

class KKSValue;
class KKSQwtPlotWidget;

namespace Ui
{
    class histogram_widget;
};

class _GUI_EXPORT KKSHistWidgetEx : public QWidget, public KKSAttrWidget
{
public:
    KKSHistWidgetEx(const KKSAttrValue *av, KKSIndAttrClass isSys, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSHistWidgetEx();

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
    void loadCategories (const QMap<int, QString>& cList);
    void loadIOList (const QMap<int, QString>& ioList);
    void loadPartLows(const QMap<int, QString>& plList);
    void loadServices(const QMap<int, QString>& sList);
    
    void clearIO (void);
    
    KKSValue getVal (void);

	void init (void);

public slots:
    void setHist (const KKSHistogram& hist);
    void calcHist (void);
    void saveHist (KKSValue & v);
    
private slots:
    void scenarioSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void variantSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void catSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void ioSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void serviceSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void partLowSet (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    
signals:
    void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);
    void loadCategory (int idCat, KKSHistogram * h);
    void loadIO (int idIO, KKSHistogram * h);
    void updateVarsList (QComboBox *, FiltersType);
    void getIdForHistogramParams(const QString & tableName, qint64 * id);

private:
    //
    // Functions
    //

    
private:
    //
    // Variables
    //
    Ui::histogram_widget *UI;
 
    KKSHistogram * m_hist;
    KKSQwtPlotWidget * m_qwtHistogramWidget;

private:
    Q_OBJECT
};

#endif	/* _KKSHISTWIDGETEX_H */
