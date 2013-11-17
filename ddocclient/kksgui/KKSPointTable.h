/***********************************************************************
 * Module:  KKSPointTable.h
 * Author:  yuriy
 * Modified: 27 но€бр€ 2010 г. 16:42
 * Purpose: Declaration of the class KKSPointTable
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSPointTable_h)
#define __KKSSITOOOM_KKSPointTable_h

#include <QTableView>
#include <QVariant>
#include <QList>

#include "kksgui_config.h"
#include "KKSAttrWidget.h"

class QModelIndex;
class QMenu;
class QContextMenuEvent;

class KKSAttrValue;

class _GUI_EXPORT KKSPointTable : public QTableView, public KKSAttrWidget
{
public:
    KKSPointTable (const KKSAttrValue*attr, KKSIndAttrClass isSys, QWidget *parent=0);
    KKSPointTable (const KKSAttrValue*attr, KKSIndAttrClass isSys, const QList<QVariant>& v, QWidget *parent=0);
    virtual ~KKSPointTable (void);

    void setData (const QList<QVariant>& pvList);
    void clearData (void);

private slots:
    void addPoint (const QModelIndex& parent, int start, int end);
    void delPoint (const QModelIndex& parent, int start, int end);
    void pointChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight);

    void appNewPoint (void);
    void insertBefore (void);
    void insertAfter (void);
    void delCurrPoint (void);

signals:
    void valueChanged (qint64 id, KKSIndAttrClass isSys, QVariant val);

protected:
    //
    // Virtual functions reimplementation
    //
    void contextMenuEvent (QContextMenuEvent * event);

private:
    //
    // Functions
    //
    void initModel (const QList<QVariant>& v=QList<QVariant>());

private:
    //
    // Variables
    //
    //const KKSIndAttr *attribute;
    //int isSystem;
    QVariant currVal;

    QMenu *popupMenu;

private:
    Q_OBJECT
};

#endif
