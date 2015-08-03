/*
    treeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include <QStringList>

#include "orgtreeitem.h"

OrgTreeItem::OrgTreeItem(const QList<QVariant> &data, OrgTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}

OrgTreeItem::~OrgTreeItem()
{
    qDeleteAll(childItems);
}

void OrgTreeItem::appendChild(OrgTreeItem *item)
{
    childItems.append(item);
}

OrgTreeItem *OrgTreeItem::child(int row)
{
    return childItems.value(row);
}

int OrgTreeItem::childCount() const
{
    return childItems.count();
}

int OrgTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant OrgTreeItem::data(int column) const
{
    return itemData.value(column);
}

OrgTreeItem *OrgTreeItem::parent()
{
    return parentItem;
}

int OrgTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<OrgTreeItem*>(this));

    return 0;
}
