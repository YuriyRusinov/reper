/* 
 * File:   KKSRubricModel.cpp
 * Author: rusinov
 * 
 * Created on 14 Сентябрь 2012 г., 11:44
 */
#include <QIcon>
#include <QPixmap>

#include "KKSRubric.h"
#include "KKSRubricModel.h"

KKSRubricModel::KKSRubricModel(KKSRubricBase * rootRubr, QObject * parent)
    : QAbstractItemModel (parent),
      rootRubric (rootRubr)
{
    if (rootRubric)
        rootRubric->addRef();
}

KKSRubricModel::~KKSRubricModel()
{
    if (rootRubric)
        rootRubric->release();
}

QVariant KKSRubricModel :: data (const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant ();
    
    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::UserRole && role != Qt::UserRole+1 && role != Qt::DecorationRole)
        return QVariant ();
    
    KKSRubricBase * rubr = getRubricEntity (index);
    
    switch (role)
    {
        case Qt::DisplayRole:case Qt::EditRole:default:
        {
            return rubr->name();
            break;
        }
        case Qt::UserRole:
        {
            return rubr->rubricType();
            break;
        }
        case Qt::UserRole+1:
        {
            return rubr->id();
            break;
        }
        case Qt::DecorationRole:
        {
            return rubr->getIcon().isNull() ? rubr->getDefaultIcon() : rubr->getIcon();
            break;
        }
    }
}

QVariant KKSRubricModel :: headerData (int section, Qt::Orientation orientation, int role ) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr("Name");
}

QModelIndex KKSRubricModel :: index (int row, int column, const QModelIndex& parent) const
{
     if (parent.isValid() && parent.column() != 0)
         return QModelIndex();

     KKSRubricBase *parentItem = getRubricEntity(parent);
     if (parentItem->rubricType() == KKSRubricBase::atRubricItem)
         return QModelIndex();

     KKSRubric * pRubr = static_cast<KKSRubric *>(parentItem);
     if (!pRubr)
         return QModelIndex();
     const KKSRubricBase *childItem (0);
     if  (row >= pRubr->rubrics().count() && row < pRubr->rubrics().count()+pRubr->items().count()) 
         childItem = static_cast <const KKSRubricBase *>(pRubr->item(row-pRubr->rubrics().count()));
     else
         childItem = static_cast <const KKSRubricBase *> (pRubr->rubric(row));
     if (childItem)
         return createIndex(row, column, (void *)childItem);
     else
         return QModelIndex();
}

QModelIndex KKSRubricModel :: parent (const QModelIndex &index) const
{
     if (!index.isValid())
         return QModelIndex();

     KKSRubricBase *childItem = this->getRubricEntity(index);
     KKSRubricBase *parentItem = static_cast<KKSRubricBase *>(childItem->parent());

     if (!parentItem || parentItem == rootRubric)
         return QModelIndex();

     return createIndex(parentItem->childNumber(), 0, parentItem);
}

int KKSRubricModel :: rowCount (const QModelIndex& parent) const
{
     KKSRubric *parentItem = static_cast<const KKSRubric *>(getRubricEntity (parent));
     if (!parentItem)
         return 0;

     return parentItem->items().count() + parentItem->rubrics().count();
}

int KKSRubricModel :: columnCount (const QModelIndex& /*parent*/) const
{
    return 1;
}

Qt::ItemFlags KKSRubricModel :: flags (const QModelIndex& index) const
{
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool KKSRubricModel :: setData (const QModelIndex& index, const QVariant& value, int role)
{
}

bool KKSRubricModel :: insertRows (int row, int count, const QModelIndex& parent)
{
}

bool KKSRubricModel :: removeRows (int row, int count, const QModelIndex& parent)
{
}

KKSRubricBase * KKSRubricModel :: getRubricEntity (const QModelIndex& index) const
{
     if (index.isValid())
     {
         KKSRubricBase *item = static_cast<KKSRubricBase*>(index.internalPointer());
         if (item) return item;
     }
     return rootRubric;
}