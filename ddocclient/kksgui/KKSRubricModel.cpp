/* 
 * File:   KKSRubricModel.cpp
 * Author: rusinov
 * 
 * Created on 14 Сентябрь 2012 г., 11:44
 */
#include <QIcon>
#include <QPixmap>
#include <QtDebug>

#include "KKSRubric.h"
#include "KKSRubricModel.h"

KKSRubricModel::KKSRubricModel(const KKSRubricBase * rootRubr, QObject * parent)
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
    
    const KKSRubricBase * rubr = getRubricEntity (index);
    
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
    if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return tr("Name");
    
    return QVariant();
}

QModelIndex KKSRubricModel :: index (int row, int column, const QModelIndex& parent) const
{
     if (parent.isValid() && parent.column() != 0)
         return QModelIndex();

     const KKSRubricBase *parentItem = getRubricEntity(parent);
     if (parentItem->rubricType() == KKSRubricBase::atRubricItem)
         return QModelIndex();

     const KKSRubric * pRubr = static_cast<const KKSRubric *>(parentItem);
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

     const KKSRubricBase *childItem = this->getRubricEntity(index);
     const KKSRubricBase *parentItem = static_cast<const KKSRubricBase *>(childItem->parent());

     if (!parentItem || parentItem == rootRubric)
         return QModelIndex();

     int idc = parentItem->childNumber();
     qDebug () << __PRETTY_FUNCTION__ << idc;
     return createIndex(idc, 0, (void *)parentItem);
}

int KKSRubricModel :: rowCount (const QModelIndex& parent) const
{
     const KKSRubric *parentItem = static_cast<const KKSRubric *>(getRubricEntity (parent));
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
    Q_UNUSED (index);
    Q_UNUSED (value);
    Q_UNUSED (role);
    return false;
}

bool KKSRubricModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    Q_UNUSED (row);
    Q_UNUSED (count);
    Q_UNUSED (parent);
    return false;
}

bool KKSRubricModel :: removeRows (int row, int count, const QModelIndex& parent)
{
    Q_UNUSED (row);
    Q_UNUSED (count);
    Q_UNUSED (parent);
    return false;
}

const KKSRubricBase * KKSRubricModel :: getRubricEntity (const QModelIndex& index) const
{
     if (index.isValid())
     {
         const KKSRubricBase *item = static_cast<const KKSRubricBase*>(index.internalPointer());
         if (item) return item;
     }
     return rootRubric;
}