/***********************************************************************
 * Module:  KKSAttrRefWidget.h
 * Author:  yuriyrusinov
 * Modified: 17 ��� 2010 �. 11:10
 * Purpose: Declaration of the class KKSAttrCheckWidget
 * ������ ������ ������������ ����� ��������� ������ �� ������� ��������
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
class QLabel;

class KKSAttrValue;

class _GUI_EXPORT KKSAttrCheckWidget : public QWidget, public KKSAttrWidget
{
public:
    KKSAttrCheckWidget (const KKSAttrValue* attr, KKSIndAttrClass isSys, QWidget * parent=0, Qt::WFlags f=0);
    ~KKSAttrCheckWidget (void);

    void setModel (QAbstractItemModel * sourceModel);
    
    QLabel * getHistLabel (void) const;

signals:
    void addAttrRef (const KKSAttrValue * attribute, KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel);
    void delAttrRef (const KKSAttrValue * attribute, KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd);
    void refIOOpen (QString tableName);
    void refRecOpen (QString tableName, qint64 id);
    
    void valueChanged (qint64 id, KKSIndAttrClass sys, QVariant val);

private slots:
    void addAttrRef (void);
    void delAttrRef (void);
    void refIO (void);
    void refRec (void);
    void updateModel (const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void removeRows (const QModelIndex & parent, int start, int end);
    void refEdit (const QModelIndex& wInd);
    
    void viewHist (const QString& link);

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
    QLabel * lHist;

private:
    Q_OBJECT
};

#endif
