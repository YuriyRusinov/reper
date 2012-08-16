/* 
 * File:   KKSRecProxyModel.h
 * Author: yuriy
 *
 * Created on 17 Èþëü 2012 ã., 22:20
 */

#ifndef KKSRECPROXYMODEL_H
#define	KKSRECPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QModelIndex>
#include <QPersistentModelIndex>
#include <QMap>

#include "kksgui_config.h"

class _GUI_EXPORT KKSRecProxyModel : public QAbstractProxyModel
{
public:
    KKSRecProxyModel(QObject * parent=0);
    virtual ~KKSRecProxyModel();

    virtual QModelIndex index (int row, int column, const QModelIndex& parent) const;
    virtual QModelIndex parent (const QModelIndex& index) const;
    virtual int rowCount (const QModelIndex& parent) const;
    virtual int columnCount (const QModelIndex& parent) const;

    virtual QModelIndex mapFromSource (const QModelIndex& sourceIndex) const;
    virtual QModelIndex mapToSource (const QModelIndex& proxyIndex) const;

    void setSourceModel (QAbstractItemModel * sourceMod);

private slots:
    void sourceDataChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    //
    // Functions
    //
    void fixModel (void);
private:
    //
    // Variables
    //
    mutable QMap<QPersistentModelIndex, QPersistentModelIndex> mapping;
    mutable QMap<QPersistentModelIndex, QPersistentModelIndex> mapParents;
private:
    Q_OBJECT

};

#endif	/* KKSRECPROXYMODEL_H */

