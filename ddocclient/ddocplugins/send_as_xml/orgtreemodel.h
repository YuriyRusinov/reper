
#ifndef ORGTREEMODEL_H
#define ORGTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include <KKSEIOData.h>

class OrgTreeItem;

class OrgTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    OrgTreeModel(const KKSMap<qint64, KKSEIOData *> &data, QObject *parent = 0);
    ~OrgTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    void setupModelData(const KKSMap<qint64, KKSEIOData*> &orgs, OrgTreeItem *parent);

    OrgTreeItem *rootItem;
};

#endif
