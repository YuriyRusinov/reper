#ifndef _KKSATTRPROXYMODEL_H
#define _KKSATTRPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QModelIndex>

#include "kksgui_config.h"
#include <KKSMap.h>

class KKSAGroup;

class _GUI_EXPORT KKSAttrProxyModel : public QAbstractProxyModel
{
public:
    KKSAttrProxyModel (QObject * parent = 0);
    ~KKSAttrProxyModel (void);

    QVariant data (const QModelIndex& index, int role=Qt::DisplayRole) const;
    bool setData (const QModelIndex& index, const QVariant& value, int role=Qt::EditRole);
    QVariant headerData (int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

    int rowCount (const QModelIndex& parent=QModelIndex()) const;

    bool insertRows (int row, int count, const QModelIndex& parent=QModelIndex() );
    bool removeRows (int row, int count, const QModelIndex& parent=QModelIndex() );

    virtual QModelIndex	mapFromSource (const QModelIndex& sourceIndex) const;
    virtual QModelIndex	mapToSource (const QModelIndex& proxyIndex) const;

private:
    //
    // Variables
    //
    KKSMap<int, KKSAGroup *> attrsGroups;

private:
    Q_OBJECT
};
#endif