/* 
 * File:   KKSRecProxyModel.h
 * Author: yuriy
 *
 * Created on 17 ���� 2012 �., 22:20
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
    virtual QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public slots:
    void setVisibleItems (bool v=false);
    
private:
    //
    // Functions
    //
    void fixModel (const QModelIndex& psIndex= QModelIndex(), bool withClear=true);
private:
    //
    // Variables
    //
    mutable QMap<QPersistentModelIndex, QPersistentModelIndex> mapping;
    mutable QMap<QPersistentModelIndex, QPersistentModelIndex> proxySourceParent;
    bool hideItems;
private:
    Q_OBJECT

};

#endif	/* KKSRECPROXYMODEL_H */

