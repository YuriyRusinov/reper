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

#include "kksgui_config.h"

class QDateEdit;
class QDateTimeEdit;
class QTimeEdit;
class QLineEdit;
class QTreeView;
class QCheckBox;
class QGroupBox;

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

private slots:
    void setDate (const KKSAttribute * a, const QDate& D);
    void setTime (const KKSAttribute * a, const QTime& T);
    void setDateTime (const KKSAttribute * a, const QDateTime& DT);
    void setModel (const KKSAttribute * a, QAbstractItemModel * mod);
    void setCheck (const KKSAttribute * a, bool ch);
    void setText (const KKSAttribute * a, QString text);
    
    void setValue (const KKSAttribute * a, QVariant val);

    void upVal (void);
    void downVal (void);

signals:
    void prevVal (void);
    void nextVal (void);
    void updateMod (const KKSAttrValue *, const QVariant&, QAbstractItemModel *);

private:
    //
    // Variables
    //
    KKSAttrValue * pAttrValue;
    QWidget * valWidget;
    QStackedLayout * stLay;
    QLineEdit * lEVal;
    QDateEdit * dEVal;
    QTimeEdit * tEVal;
    QDateTimeEdit * dtEVal;
    QTreeView * tvVal;
    QCheckBox * chVal;
    QGroupBox * gbVal;
private:
    Q_OBJECT

};

#endif	/* KKSAVALWIDGET_H */

