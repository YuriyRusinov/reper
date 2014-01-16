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
#include <QMap>
#include <KKSMap.h>

#include "kksgui_config.h"
#include "KKSAttrWidget.h"

class QPaintEvent;

class QGroupBox;
class QLineEdit;
class QComboBox;
class QPushButton;

class KKSCharts;
class KKSCategory;
class KKSObject;
class KKSHistogram;
class KKSHistDrawWidget;

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
    void loadRecvList (const QMap<int, QString>& posList);
    void clearIO (void);

public slots:
    void setHist (const KKSHistogram& hist);
    
private slots:
/*    void fromChanged ();
    void toChanged ();
    void numChanged ();
*/
    void catChanged (int cIndex);
    void ioChanged (int ioIndex);
    
    void calcHist (void);

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
    
    QWidget * wHistDrawW;
    KKSHistogram * hist;
    
    QPushButton * pbCalc;
private:
    Q_OBJECT
};

#endif	/* _KKSHISTWIDGET_H */
