/* 
 * File:   KKSSearchTemplateModel.cpp
 * Author: rusinov
 * 
 * Created on 14 январь 2013 г., 13:52
 */

#include <KKSSearchTemplateType.h>
#include <KKSSearchTemplate.h>
#include "KKSSearchTreeItem.h"
#include "KKSSearchTemplateModel.h"

KKSSearchTemplateModel::KKSSearchTemplateModel(const KKSMap<int, KKSSearchTemplateType *>& sTypes, const KKSList<KKSSearchTemplate *>& sTemplates, QObject * parent)
    : QAbstractItemModel (parent),
      searchTemplateTypes (sTypes),
      searchTemplates (sTemplates),
      rootItem (new KKSSearchTreeItem (-1, (const KKSSearchTemplateType *)(0), 0))
{
    setupData (rootItem);
}

KKSSearchTemplateModel::~KKSSearchTemplateModel()
{
    delete rootItem;
}

QVariant KKSSearchTemplateModel :: data (const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::UserRole)
        return QVariant();
    
    KKSSearchTreeItem *item = static_cast<KKSSearchTreeItem*>(index.internalPointer());
    if (role == Qt::UserRole)
        return item->id();

    if (item->getSearchTemplateType() && index.column() == 0)
        return item->getSearchTemplateType ()->name();
    else if (item->getSearchTemplate())
    {
        switch (index.column())
        {
            case 0: return item->getSearchTemplate()->name(); break;
            case 1: return item->getSearchTemplate()->authorName(); break;
            case 2: return item->getSearchTemplate()->creationDatetime(); break;
            case 3: return item->getSearchTemplate()->categoryName(); break;
            case 4: return item->getSearchTemplate()->type()->name(); break;
            default: return QVariant();
        }
    }
    else
        return QVariant ();
}

QVariant KKSSearchTemplateModel :: headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
            case 0: return tr ("Search criteria"); break;
            case 1: return tr ("Author"); break;
            case 2: return tr ("Date of creation"); break;
            case 3: return tr ("Category"); break;
            case 4: return tr ("Type"); break;
            default: return QVariant (); break;
        }
    }
    else
        return QVariant ();
}

QModelIndex KKSSearchTemplateModel :: index (int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    KKSSearchTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<KKSSearchTreeItem*>(parent.internalPointer());

    KKSSearchTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();    
}

QModelIndex KKSSearchTemplateModel :: parent (const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

     KKSSearchTreeItem *childItem = static_cast<KKSSearchTreeItem*>(index.internalPointer());
     const KKSSearchTreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, (void *)parentItem);
}

int KKSSearchTemplateModel :: rowCount (const QModelIndex& parent) const
{
     KKSSearchTreeItem *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<KKSSearchTreeItem*>(parent.internalPointer());

     return parentItem->childCount();
}

int KKSSearchTemplateModel :: columnCount (const QModelIndex& parent) const
{
    Q_UNUSED (parent);
    return 5;
}

Qt::ItemFlags KKSSearchTemplateModel :: flags (const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;   
}

bool KKSSearchTemplateModel :: setData (const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED (index);
    Q_UNUSED (value);
    Q_UNUSED (role);
    return false;
}

bool KKSSearchTemplateModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    Q_UNUSED (row);
    Q_UNUSED (count);
    Q_UNUSED (parent);
    return false;
}

bool KKSSearchTemplateModel :: removeRows (int row, int count, const QModelIndex& parent)
{
    Q_UNUSED (row);
    Q_UNUSED (count);
    Q_UNUSED (parent);
    return false;
    
}

void KKSSearchTemplateModel :: setupData (KKSSearchTreeItem *parent)
{
    if (!parent)
        return;

    if (parent->getSearchTemplate())
    {
        //
        // ќшибка: подитемы есть только у типов шаблонов
        //
        return;
    }

}