/* 
 * File:   KKSRubricModel.cpp
 * Author: yuriyrusinov
 * 
 * Created on 14 Сентябрь 2012 г., 11:44
 */
#include <QIcon>
#include <QPixmap>
#include <QtDebug>

#include <KKSRubric.h>
#include <KKSList.h>
#include "KKSRubricTreeItem.h"
#include "KKSRubricModel.h"

KKSRubricModel::KKSRubricModel(const KKSRubric * rootRubr, bool forRecs, QObject * parent)
    : QAbstractItemModel (parent),
      rootItem (new KKSRubricTreeItem (-1, rootRubr)),
      forRecords (forRecs)
{
    setupRubrData (rootItem, forRecs);
}

KKSRubricModel::KKSRubricModel(const KKSMap< qint64, KKSEIOData * >& rubrRecs, QObject * parent)
    : QAbstractItemModel (parent),
      rootItem (new KKSRubricTreeItem (-1, new KKSRubric(-1, "root rubric for all tree"))),
      forRecords (false)
{
//    KKSRubric * rootRubric = new KKSRubric(-1, "root rubric for all tree");
//    rootItem->setData(rootRubric);
    setupData (rubrRecs);
}

KKSRubricModel::~KKSRubricModel()
{
    delete rootItem;
//    if (rootRubric)
//        rootRubric->release();
}

QVariant KKSRubricModel :: data (const QModelIndex &index, int role) const
{
    if (!index.isValid() && role == Qt::UserRole+4 && this->rootItem)
    {
        const KKSRubricBase * rubr = rootItem->getData();
        return QVariant::fromValue<const KKSRubricBase *>(rubr);
    }

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
        case Qt::UserRole+3:
        {
            return forRecords;
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
            {
                //qDebug () << __PRETTY_FUNCTION__ << rubr->getIcon().availableSizes();
                QPixmap px = rubr->getDefaultIcon(); 
                //QByteArray bytes (rubr->iconAsString().toUtf8());
                //px.loadFromData (bytes);
                return QVariant (rubr->getIcon().pixmap (px.size()));
            }
            break;
        }
        case Qt::ToolTipRole:
        {
            return rubr->name();
            break;
        }
//        case Qt::SizeHintRole:
//        {
//            return rubr->getDefaultIcon().size();
//            break;
//        }
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
//    if (!hasIndex (row, column, parent))
//        return QModelIndex ();

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    KKSRubricTreeItem * rItem = getRubricEntity (parent);
    if (!rItem)
        return QModelIndex ();
/*    const KKSRubricBase *parentItem = rItem->getData();
    if (!parentItem ||
        parentItem->rubricType() == KKSRubricBase::atRubricItem ||
        (parentItem->rubricType() != KKSRubricBase::atOthers && ((const KKSRubric *)parentItem) && ((const KKSRubric *)parentItem)->getCategory ()))
        return QModelIndex();
*/
    const KKSRubricTreeItem *childItem (rItem->child(row));

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
     if (!parentItem || (parentItem->getData() && !forRecords && parentItem->getData()->rubricType() == KKSRubricBase::atRubricItem))
         return 0;

     const KKSRubricBase * parentRubr = parentItem->getData ();
     const KKSRubric * pRubr = (const KKSRubric *)parentRubr;
     Q_UNUSED (pRubr);
/*     if (parentRubr && parentRubr->rubricType() != KKSRubricBase::atOthers && parentRubr->rubricType() == KKSRubricBase::atRubricCategory && !forRecords)//&& pRubr && pRubr->getCategory ())
         return 0;
     //else if (pRubr && pRubr->rubricType() != KKSRubricBase::atRubricCategory && pRubr->rubricType() != KKSRubricBase::atRootRubric)
     //{
     //    qDebug () << __PRETTY_FUNCTION__ << pRubr->rubricType();
     //    return pRubr->rubrics().count();
     //}
     else*/
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
        //setupRubrData (wRubr, forRecords);

        emit dataChanged (index, index);
        return true;
    }
    else if (role == Qt::DecorationRole)
    {
        QIcon rIcon = value.value<QIcon>();
        const KKSRubricBase * wRubrE = wRubr->getData ();
        if (!wRubrE)
            return false;
        (const_cast<KKSRubricBase *>(wRubrE))->setIcon (rIcon);
        wRubr->setData (wRubrE);
        emit dataChanged (index, index);
        return true;
    }
    return false;
}

bool KKSRubricModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    KKSRubricTreeItem * parentItem = getRubricEntity(parent);//->getData();
    if (!parentItem || (parent.isValid() && !parentItem->getData()))
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

void KKSRubricModel :: setupRubrData (KKSRubricTreeItem * parent, bool forRecs)
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
            setupRubrData (wrItem, forRecs);
        }
/*        
        if (forRecs)
        {
            int nItems = wRubric->items().count();
            for (int i=0; i<nItems; i++)
            {
                KKSRubricTreeItem * wrItem = new KKSRubricTreeItem (wRubric->item(i)->id(), wRubric->item(i), parent);
                parent->appendChild (wrItem);
            }
        }
 */
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
            setupRubrData (wItem);
        }
    }
}

void KKSRubricModel :: setupData (const KKSMap< qint64, KKSEIOData * >& rubrRecs)
{
    //int nr = rowCount();
    //QModelIndex pIndex = QModelIndex ();

    //KKSRubricTreeItem * prevItem (0);
    KKSRubric * rootRubric = /*(const_cast<KKSRubricBase *>(parent->getData())) ? (const_cast<KKSRubricBase *>(parent->getData())) :*/ new KKSRubric(-1, "root rubric for all tree");
        
    for (KKSMap<qint64, KKSEIOData* >::const_iterator p = rubrRecs.constBegin();
            p != rubrRecs.constEnd();
            p++)
    {
        QIcon tIcon;
        if (p.value())
        {
            QString strIcon = p.value()->sysFieldValue("r_icon");
            //qDebug () << __PRETTY_FUNCTION__ << strIcon;
            QPixmap pIcon;
            pIcon.loadFromData (strIcon.toUtf8());
            tIcon = QIcon (pIcon);
        }
        //qDebug () << __PRETTY_FUNCTION__ << p.value()->sysFieldValue("id") << p.key();
        KKSRubric * r = new KKSRubric (p.key(), p.value()->fieldValue("name"));
        r->setIcon(tIcon);
        r->setInitialized(false);
        QString valStr = p.value()->sysFieldValue("id_parent");
        bool ok;
        int idParent = valStr.isEmpty() ? -1 : valStr.toLongLong(&ok);
        KKSRubric * pRubr = rootRubric->rubricForId(idParent);
        if (!pRubr)
        {
            rootRubric->addRubric(r);
            r->setParent (rootRubric);
        }
        else
        {
            pRubr->addRubric (r);
            r->setParent (pRubr);
        }
    }
    rootItem->setData (rootRubric);
    setupRubrData (rootItem, false);
}

KKSRubricTreeItem * KKSRubricModel :: getModelItem (qint64 val, KKSRubricTreeItem * parent, QModelIndex & pIndex)
{
    if (!parent)
        return rootItem;

    //if (parent->getData() && QString::compare (parent->getData()->fieldValue(cAttrP->columnName(false)), valStr) == 0)
    //    return parent;
    for (int i=0; i<parent->childCount(); i++)
    {
        KKSRubricTreeItem * item = parent->child(i);
        //QString refCol = QString ("id");
        QString pStr = QString::number (item->getData()->id());//sysFieldValue(refCol);
        //qDebug () << __PRETTY_FUNCTION__ << pStr << refCol << cAttrP->code(false) << item->getData()->fields();
        bool ok (true);
        qint64 iVal (-1);
        iVal = item->id();
        if (iVal == val)
        {
            pIndex = this->index (i, 0, pIndex);
            return item;
        }
        else if (item->childCount() > 0)
        {
            KKSRubricTreeItem * chItem = getModelItem (val, item, pIndex);
            if (chItem)
                return chItem;
        }
        else
            continue;
    }
    
    return 0;
    
}

KKSRubricTreeItem * KKSRubricModel :: getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        KKSRubricTreeItem *item = static_cast<KKSRubricTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;   
    
}
