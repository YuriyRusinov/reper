/* 
 * File:   KKSRubricModel.cpp
 * Author: yuriyrusinov
 * 
 * Created on 14 Сентябрь 2012 г., 11:44
 */
#include <QIcon>
#include <QPixmap>
#include <QtDebug>

#include "KKSRubric.h"
#include "KKSList.h"
#include "KKSRubricTreeItem.h"
#include "KKSRubricModel.h"

KKSRubricModel::KKSRubricModel(const KKSRubric * rootRubr, QObject * parent)
    : QAbstractItemModel (parent),
      rootItem (new KKSRubricTreeItem (-1, rootRubr))
{
    setupData (rootItem);
}

KKSRubricModel::~KKSRubricModel()
{
    delete rootItem;
//    if (rootRubric)
//        rootRubric->release();
}

QVariant KKSRubricModel :: data (const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant ();
    
    KKSRubricTreeItem * rItem = getRubricEntity (index);
    if (!rItem)
        return QVariant ();
    const KKSRubricBase * rubr = rItem->getData();
    if (!rubr)
        return QVariant ();
    
    switch (role)
    {
        case Qt::DisplayRole:case Qt::EditRole:
        {
            return rubr->name();
            break;
        }
        case Qt::UserRole+2:
        {
            return rubr->rubricType();
            break;
        }
        case Qt::UserRole+1:
        {
            return QVariant::fromValue<const KKSRubricBase *>(rubr);
            break;
        }
        case Qt::UserRole:
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
        case Qt::ToolTipRole:
        {
            return rubr->name();
            break;
        }
        default:
        {
            return QVariant();
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

    KKSRubricTreeItem * rItem = getRubricEntity (parent);
    if (!rItem)
        return QModelIndex ();
    const KKSRubricBase *parentItem = rItem->getData();
    if (!parentItem || parentItem->rubricType() == KKSRubricBase::atRubricItem)
        return QModelIndex();

//    const KKSRubric * pRubr = static_cast<const KKSRubric *>(parentItem);
//    if (!pRubr)
//        return QModelIndex();
    const KKSRubricTreeItem *childItem (rItem->child(row));
/*    if (row >= 0 && row < pRubr->rubrics().count())
        childItem = static_cast <const KKSRubricBase *> (pRubr->rubric(row));
    else if  (row >= pRubr->rubrics().count() && row < pRubr->rubrics().count()+pRubr->items().count()) 
        childItem = static_cast <const KKSRubricBase *>(pRubr->item(row-pRubr->rubrics().count()));
 */
    if (childItem)
        return createIndex(row, column, (void *)childItem);
    else
        return QModelIndex();
}

QModelIndex KKSRubricModel :: parent (const QModelIndex &index) const
{
     if (!index.isValid())
         return QModelIndex();

     const KKSRubricTreeItem *childItem = this->getRubricEntity(index);
     const KKSRubricTreeItem *parentItem = childItem->parent();

     if (!parentItem || parentItem == rootItem)
         return QModelIndex();

     int idc = parentItem->childNumber();
     return createIndex(idc, 0, (void *)parentItem);
}

int KKSRubricModel :: rowCount (const QModelIndex& parent) const
{
     const KKSRubricTreeItem *parentItem = getRubricEntity (parent);
     if (!parentItem || !parentItem->getData() || parentItem->getData()->rubricType() == KKSRubricBase::atRubricItem)
         return 0;

     return parentItem->childCount();
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
    KKSRubricTreeItem * wRubr = getRubricEntity (index);
    if (!wRubr)
        return false;
    
    if (role == Qt::UserRole+1)
    {
        const KKSRubricBase * wNewRubr = value.value<const KKSRubricBase *>();
        if (!wNewRubr)
            return false;
        
        wRubr->setData(wNewRubr);
        setupData (wRubr);

        emit dataChanged (index, index);
        return true;
    }
    return false;
}

bool KKSRubricModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    KKSRubricTreeItem * parentItem = getRubricEntity(parent);//->getData();
    if (!parentItem || !parentItem->getData())
        return false;
//    KKSRubric * pRubric = const_cast<KKSRubric *>(static_cast<const KKSRubric *>(parentItem));
//    if (!pRubric)
//        return false;
    beginInsertRows (parent, row, row+count-1);
    bool isok = parentItem->insertChildren(row, count);
    endInsertRows ();
    return isok;
}

bool KKSRubricModel :: removeRows (int row, int count, const QModelIndex& parent)
{
    KKSRubricTreeItem * parentItem = getRubricEntity(parent);//->getData();
    if (!parentItem || !parentItem->getData())
        return false;
    beginRemoveRows (parent, row, row+count-1);
    bool isok = parentItem->removeChildren(row, count);
    endRemoveRows ();
    return isok;
}

KKSRubricTreeItem * KKSRubricModel :: getRubricEntity (const QModelIndex& index) const
{
     if (index.isValid())
     {
         KKSRubricTreeItem *item = static_cast<KKSRubricTreeItem*>(index.internalPointer());
         if (item) return item;
     }
     return rootItem;
}

void KKSRubricModel :: setupData (KKSRubricTreeItem * parent)
{
    if (!parent || !parent->getData() || parent->getData()->rubricType()==KKSRubricBase::atRubricItem)
        return;
    
    const KKSRubricBase * wRubr = parent->getData ();
    if (wRubr->rubricType() != KKSRubricBase::atOthers)
    {
        //
        // это рубрика (или категорированная рубрика)
        //
        const KKSRubric * wRubric = static_cast<const KKSRubric *>(wRubr);
        for (int i=0; i<wRubric->rubrics().count(); i++)
        {
            KKSRubricTreeItem * wrItem = new KKSRubricTreeItem (wRubric->rubric(i)->id(), wRubric->rubric(i), parent);
            parent->appendChild (wrItem);
            setupData (wrItem);
        }
        for (int i=0; i<wRubric->items().count(); i++)
        {
            KKSRubricTreeItem * wrItem = new KKSRubricTreeItem (wRubric->item(i)->id(), wRubric->item(i), parent);
            parent->appendChild (wrItem);
        }
    }
    else
    {
        for (int i=0; i<wRubr->subnodes().count(); i++)
        {
            const KKSRubricBase * wr = wRubr->subnodes().at (i);
            if (!wr)
                continue;
            KKSRubricTreeItem * wItem = new KKSRubricTreeItem (wr->id(), wr, parent);
            parent->appendChild (wItem);
            setupData (wItem);
        }
    }
}
