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
    KKSAttrCheckWidget (const KKSAttrValue* attr, bool isSys, QWidget * parent=0, Qt::WFlags f=0);
    ~KKSAttrCheckWidget (void);

    void setModel (QAbstractItemModel * sourceModel);

signals:
    void addAttrRef (const KKSAttrValue * attribute, bool isSystem, QAbstractItemModel * sourceModel);
    void delAttrRef (const KKSAttrValue * attribute, bool isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd);
    void refIOOpen (QString tableName);
    
    void valueChanged (int id, bool sys, QVariant val);

private slots:
    void addAttrRef (void);
    void delAttrRef (void);
    void refIO (void);
    void updateModel (const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void removeRows (const QModelIndex & parent, int start, int end);

private:
    //
    // Functions
    //
    void setupWidget (void);

private:
    //
    // Variables
    //
//    const KKS* attribute;
//    bool isSystem;

    QTreeView * tv;
    QToolBar * tbActions;
    QAction * actAdd;
    QAction * actDel;
    QAction * actRef;
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
