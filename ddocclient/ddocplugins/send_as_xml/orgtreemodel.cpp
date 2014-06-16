
/*
    treemodel.cpp

    Provides a simple tree model to show how to create and use hierarchical
    models.
*/

#include <QtGui>

#include "orgtreeitem.h"
#include "orgtreemodel.h"
#include <KKSMap.h>

OrgTreeModel::OrgTreeModel(const KKSMap<qint64, KKSEIOData * > & data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    //int fieldsCount = data.fieldsCount();
    //for(int i=0; i<fieldsCount; i++){
    
    //QList<QString> headers = data.fields().keys();
    
    //for(int i=0; i<headers.count(); i++)
    //    rootData << headers.at(i);
    //}
    
    rootItem = new OrgTreeItem(rootData);
    //setupModelData(data, rootItem);
}

OrgTreeModel::~OrgTreeModel()
{
    delete rootItem;
}

int OrgTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<OrgTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant OrgTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    OrgTreeItem *item = static_cast<OrgTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags OrgTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant OrgTreeModel::headerData(int section, 
                                  Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex OrgTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    OrgTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<OrgTreeItem*>(parent.internalPointer());

    OrgTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex OrgTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    OrgTreeItem *childItem = static_cast<OrgTreeItem*>(index.internalPointer());
    OrgTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int OrgTreeModel::rowCount(const QModelIndex &parent) const
{
    OrgTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<OrgTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void OrgTreeModel::setupModelData(const KKSMap<qint64, KKSEIOData *> &data, OrgTreeItem *parent)
{
    /*
    QList<OrgTreeItem*> parents;
    //QList<int> indentations;
    parents << parent;
    //indentations << 0;

    int number = 0;

    while (number < lines.count()) {

        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        //data.
        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
        }

        number++;
    }
    */
}
