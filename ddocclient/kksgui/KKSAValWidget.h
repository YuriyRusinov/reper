/* 
 * File:   KKSAValWidget.h
 * Author: yuriyrusinov
 *
 * Created on 5 Сентябрь 2013 г., 10:52
 */

#ifndef KKSAVALWIDGET_H
#define	KKSAVALWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QList>

#include "kksgui_config.h"

class QDateEdit;
class QDateTimeEdit;
class QTimeEdit;
class QLineEdit;
class QTreeView;
class QCheckBox;
class QGroupBox;
class QToolButton;
class QTextEdit;
class QGridLayout;

class QDate;
class QTime;
class QDateTime;
class QAbstractItemModel;
class QStackedLayout;

class KKSAttrValue;
class KKSAttribute;

class _GUI_EXPORT KKSAValWidget : public QWidget {
public:
    KKSAValWidget(KKSAttrValue * _av, QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~KKSAValWidget();

    void setValModel (QAbstractItemModel * valMod);

private slots:
    void setValue (const KKSAttribute * a, QVariant val);

    void upVal (void);
    void downVal (void);

signals:
    void prevVal (void);
    void nextVal (void);
    void updateMod (const KKSAttrValue *, const QVariant&, QAbstractItemModel *);
    void updateComplexAttr (KKSAttrValue *, const QVariant&, QWidget *);

private:
    //
    // Functions
    //
    void initComplexWidget (KKSAttrValue * av, QGridLayout * gLay, QWidget * parent=0, Qt::WindowFlags flags=0);
    void setComplexVals (const QVariant& val);
private:
    //
    // Variables
    //
    KKSAttrValue * pAttrValue;
    QWidget * valWidget;
    QLineEdit * lEVal;
    QDateEdit * dEVal;
    QTimeEdit * tEVal;
    QDateTimeEdit * dtEVal;
    QTreeView * tvVal;
    QCheckBox * chVal;
    QWidget * gbVal;
    QList<QWidget *> aWVals;
    QWidget * macVal;
    QLineEdit * lE [2];
    QTextEdit * textEVal;

    QToolButton * tbUp;
    QToolButton * tbDown;
private:
    Q_OBJECT

};

#endif	/* KKSAVALWIDGET_H */

