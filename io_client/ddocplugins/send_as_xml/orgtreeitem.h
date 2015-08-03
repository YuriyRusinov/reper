#ifndef ORGTREEITEM_H
#define ORGTREEITEM_H

#include <QList>
#include <QVariant>

//! [0]
class OrgTreeItem
{
public:
    OrgTreeItem(const QList<QVariant> &data, OrgTreeItem *parent = 0);
    ~OrgTreeItem();

    void appendChild(OrgTreeItem *child);

    OrgTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    OrgTreeItem *parent();

private:
    QList<OrgTreeItem*> childItems;
    QList<QVariant> itemData;
    OrgTreeItem *parentItem;
};

#endif
