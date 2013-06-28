/***********************************************************************
 * Module:  KKSAttrRefWidget.h
 * Author:  yuriyrusinov
 * Modified: 17 мая 2010 г. 11:10
 * Purpose: Declaration of the class KKSAttrCheckWidget
 * Данный виджет представляет собой урезанный виджет со списком значений
 ***********************************************************************/
#if !defined(__KKSSITOOOM_KKSAttrCheckWidget_h)
#define __KKSSITOOOM_KKSAttrCheckWidget_h

#include "kksgui_config.h"

#include <QWidget>
#include <QVariant>
#include "KKSAttrWidget.h"

class QTreeView;
class QPushButton;
class QSortFilterProxyModel;
class QAbstractItemModel;
class QModelIndex;
class QToolBar;
class QAction;

class KKSAttrValue;

class _GUI_EXPORT KKSAttrCheckWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSAttrCheckWidget (const KKSAttrValue* attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget * parent=0, Qt::WFlags f=0);
    ~KKSAttrCheckWidget (void);

    void setModel (QAbstractItemModel * sourceModel);

signals:
    void addAttrRef (const KKSAttrValue * attribute, KKSIndAttr::KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel);
    void delAttrRef (const KKSAttrValue * attribute, KKSIndAttr::KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd);
    void refIOOpen (QString tableName);
    void refRecOpen (QString tableName, qint64 id);
    
    void valueChanged (qint64 id, KKSIndAttr::KKSIndAttrClass sys, QVariant val);

private slots:
    void addAttrRef (void);
    void delAttrRef (void);
    void refIO (void);
    void refRec (void);
    void updateModel (const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void removeRows (const QModelIndex & parent, int start, int end);
    void refEdit (const QModelIndex& wInd);

private:
    //
    // Functions
    //
    void setupWidget (void);

private:
    //
    // Variables
    //

    QTreeView * tv;
    QToolBar * tbActions;
    QAction * actAdd;
    QAction * actDel;
    QAction * actRefIO;
    QAction * actRefRec;
/*
    QPushButton *pbAdd;
    QPushButton *pbDel;
    QPushButton *pbRef;
*/
    QSortFilterProxyModel *viewModel;

private:
    Q_OBJECT
};

#endif
