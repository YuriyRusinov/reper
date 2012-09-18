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

KKSRubricModel::KKSRubricModel(const KKSRubric * rootRubr, QObject * parent)
    : QAbstractItemModel (parent),
      rootRubric (rootRubr)
{
//    if (rootRubric && static_cast<const KKSRubric *>(rootRubric))
//        rootRubric->addRef();
}

KKSRubricModel::~KKSRubricModel()
{
//    if (rootRubric)
//        rootRubric->release();
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
            if (rubr->getIcon().isNull())
            {
                if (rubr->rubricType() == KKSRubricBase::atRubricItem)
                    return KKSRubricItem::icon();
                else
                    return KKSRubric::icon();
                //return rubr->getDefaultIcon();
            }
            else
                return QIcon (rubr->getIcon());
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
    if (!hasIndex (row, column, parent))
        return QModelIndex ();

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    const KKSRubricBase *parentItem = getRubricEntity(parent);
    if (!parentItem || parentItem->rubricType() == KKSRubricBase::atRubricItem)
        return QModelIndex();

    const KKSRubric * pRubr = static_cast<const KKSRubric *>(parentItem);
    if (!pRubr)
        return QModelIndex();
    const KKSRubricBase *childItem (0);
    if (row >= 0 && row < pRubr->rubrics().count())
        childItem = static_cast <const KKSRubricBase *> (pRubr->rubric(row));
    else if  (row >= pRubr->rubrics().count() && row < pRubr->rubrics().count()+pRubr->items().count()) 
        childItem = static_cast <const KKSRubricBase *>(pRubr->item(row-pRubr->rubrics().count()));
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
     if (idc < 0)
         return QModelIndex();
     return createIndex(idc, 0, (void *)parentItem);
}

int KKSRubricModel :: rowCount (const QModelIndex& parent) const
{
     const KKSRubricBase *parentItem = getRubricEntity (parent);
     if (!parentItem || parentItem->rubricType() == KKSRubricBase::atRubricItem)
         return 0;

     const KKSRubric* pRubric = static_cast<const KKSRubric *>(parentItem);
     return pRubric->items().count() + pRubric->rubrics().count();
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
    const KKSRubricBase * parentItem = getRubricEntity(parent);
    if (!parentItem || parentItem->rubricType() == KKSRubricBase::atRubricItem)
        return false;
    KKSRubric * pRubric = const_cast<KKSRubric *>(static_cast<const KKSRubric *>(parentItem));
    if (!pRubric)
        return false;
    beginInsertRows (parent, row, row+count-1);
    if (row>=0 && row<=pRubric->rubrics().size())
    {
        for (int i=0; i<count; i++)
        {
            KKSRubric * r = new KKSRubric ();
            pRubric->insertRubric(row, r);
            r->release();
        }
    }
    else if (row<=pRubric->rubrics().size()+pRubric->items().size())
    {
        for (int i=0; i<count; i++)
        {
            KKSRubricItem * r = new KKSRubricItem ();
            pRubric->insertItem(row-pRubric->rubrics().size(), r);
            r->release();
        }
        
    }
    endInsertRows ();
    return true;
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

