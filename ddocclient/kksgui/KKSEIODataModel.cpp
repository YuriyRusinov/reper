#include <QtDebug>

#include "KKSTemplate.h"
#include "KKSCategory.h"
#include "KKSAttrType.h"
#include "KKSCategoryAttr.h"
#include "KKSAttrView.h"
#include "KKSEIOData.h"
#include "KKSTreeItem.h"
#include "KKSEIODataModel.h"

KKSEIODataModel :: KKSEIODataModel (const KKSTemplate * t, const KKSMap<qint64, KKSEIOData *>& objRecs, QObject *parent)
    : QAbstractItemModel (parent),
    tRef (t),
    cAttrP (0),
    objRecords (objRecs),
    //parIndexList (QMap<qint64, QModelIndex>()),
    //indList (QMap<qint64, QList<qint64> >()),
    rootItem (new KKSTreeItem(-1, 0))
{
    if (tRef)
        tRef->addRef();
    cAttrP = getFirstAttribute (KKSAttrType::atParent);
    setupData (rootItem);
}

KKSEIODataModel :: ~KKSEIODataModel ()
{
    if (tRef)
        tRef->release();
    delete rootItem;
}

int KKSEIODataModel :: columnCount (const QModelIndex& parent) const
{
    Q_UNUSED (parent);
    if (!tRef)
        return 0;
    return tRef->attrsCount();
}

int KKSEIODataModel :: rowCount (const QModelIndex& parent) const
{
    KKSTreeItem * parentItem = getItem (parent);

    return parentItem->childCount();
}
 

QModelIndex KKSEIODataModel :: index (int row, int column, const QModelIndex& parent) const
{

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    KKSTreeItem *parentItem = getItem(parent);

    KKSTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex KKSEIODataModel :: parent (const QModelIndex& index) const
{

    if (!index.isValid())
        return QModelIndex();

    KKSTreeItem *childItem = getItem(index);
    KKSTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

Qt::ItemFlags KKSEIODataModel :: flags (const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 0 && tRef && tRef->category()->searchAttributesByType(KKSAttrType::atCheckListEx).size() > 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    
}

QVariant KKSEIODataModel :: data (const QModelIndex& index, int role) const
{
    if (!index.isValid() && role <= Qt::UserRole+1)
        return QVariant();

    if (role == Qt::UserRole+2)
        return QVariant::fromValue<KKSTemplate>(*tRef);
    else if (role == Qt::UserRole+3)
        return QVariant::fromValue<KKSCategoryAttr>(*cAttrP);
    KKSTreeItem * wItem = static_cast<KKSTreeItem*>(index.internalPointer());
    if (!wItem)
        return QVariant ();
    qint64 idw = wItem->id();//index.internalId();
    //if (!index.parent().isValid())
    //    qDebug () << __PRETTY_FUNCTION__ << index << idw;
    //if (index.parent().isValid())
    //    qDebug () << __PRETTY_FUNCTION__ << idw << index.parent().internalId();
    //KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constFind(idw);
    //if (p == objRecords.constEnd())
    //    return QVariant ();
    if (role == Qt::UserRole)
    {
        return idw;//wItem->id();//p.key();
    }
    else if (role == Qt::DisplayRole)
    {
        KKSList<KKSAttrView*> avList = tRef ? tRef->sortedAttrs() : KKSList<KKSAttrView*>();
        if (index.column() >= avList.count())
            return QVariant ();
        KKSAttrView * v = avList[index.column()];
        QString aCode = avList[index.column()]->code(false);
        int i = index.row();
        QString attrValue;
        if( v->type()->attrType() == KKSAttrType::atJPG || 
            (v->refType() && v->refType()->attrType() == KKSAttrType::atJPG)
            )
        {
            attrValue = QObject::tr("<Image data %1>").arg (i);
        }
        else if( v->type()->attrType() == KKSAttrType::atSVG || 
                (v->refType() && v->refType()->attrType() == KKSAttrType::atSVG)
                )
        {
            attrValue = QObject::tr("<SVG data %1>").arg (i);
        }
        else if( v->type()->attrType() == KKSAttrType::atXMLDoc || 
                (v->refType() && v->refType()->attrType() == KKSAttrType::atXMLDoc)
                )
        {
            attrValue = QObject::tr("<XML document %1>").arg (i);
        }
        else if( v->type()->attrType() == KKSAttrType::atVideo || 
                (v->refType() && v->refType()->attrType() == KKSAttrType::atVideo)
                )
        {
            attrValue = QObject::tr("<Video data %1>").arg (i);
        }
        else
        {
            if (!wItem->getData())
                attrValue = QString();
            else
            {
                attrValue = wItem->getData()->fieldValue (aCode).isEmpty() ? wItem->getData()->sysFieldValue (aCode) : wItem->getData()->fieldValue (aCode);
                if (attrValue.isEmpty())
                {
                    attrValue = wItem->getData()->fields().value (aCode.toLower());
                    if (attrValue.isEmpty())
                        attrValue = wItem->getData()->sysFields().value (aCode.toLower());
                }
                else if (attrValue.contains ("\n"))
                    attrValue = attrValue.mid (0, attrValue.indexOf("\n")) + "...";
            }
        }
        return attrValue;
    }
    return QVariant();
}

QVariant KKSEIODataModel :: headerData (int section, Qt::Orientation orientation, int role) const
{
    KKSList<KKSAttrView*> avList = tRef ? tRef->sortedAttrs() : KKSList<KKSAttrView*>();
    if (orientation == Qt::Horizontal && section >= 0 && section < avList.count() && role == Qt::DisplayRole)
    {
        QVariant v = avList[section]->title();
        return v;
    }
    else
        return QVariant ();
}

bool KKSEIODataModel :: setData (const QModelIndex& index, const QVariant& value, int role)
{
    KKSTreeItem * wItem = getItem (index);
    if (!wItem && role != Qt::UserRole+1)
        return false;
    
    int irow = index.row ();
    QModelIndex pIndex = index.parent();
    QModelIndex topL = this->index (irow, 0, pIndex);
    QModelIndex bottomR = this->index (irow, columnCount(pIndex)-1, pIndex);
    //qDebug () << __PRETTY_FUNCTION__ << topL << bottomR;
    if (role == Qt::UserRole)
    {
        qint64 id = value.value<qint64>();
        wItem->setId (id);
        if (!objRecords.contains(id))
            objRecords.insert(id, 0);
        emit dataChanged (topL, bottomR);
    }
    else if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        KKSEIOData d = value.value<KKSEIOData>();
        KKSEIOData * dVal = new KKSEIOData (d);
        if (objRecords.contains(wItem->id()))
        {
            objRecords.remove (wItem->id());
            objRecords.insert (wItem->id(), dVal);
        }
        wItem->setData (dVal);
        dVal->release ();
        emit dataChanged (topL, bottomR);
    }
    else if (role == Qt::UserRole+1)
    {
        QMap<QString, QVariant> objData = value.toMap ();
        //rootItem->clearChildren();
        //rootItem = new KKSTreeItem(-1, 0);
/*        for (KKSMap<qint64, KKSEIOData *>::iterator p = objRecords.begin();
             p != objRecords.end();
             p++
        )
        {
            KKSEIOData * d = p.value();
            if (d)
                d->release ();
        }*/
        objRecords.clear();

        for (QMap<QString, QVariant>::const_iterator pv = objData.constBegin();
             pv != objData.constEnd();
             pv++)
        {
            bool ok;
            qint64 id = pv.key().toLongLong(&ok);
            if (!ok)
                continue;
            KKSEIOData * d = new KKSEIOData (pv.value().value<KKSEIOData>());
            objRecords.insert (id, d);
        }
        setupData (rootItem);
        int nr = rowCount();
        int nc = columnCount();
        QModelIndex topL = this->index(0, 0);
        QModelIndex botR = this->index(nr-1, nc-1);
        //qDebug () << __PRETTY_FUNCTION__ << topL << botR;
        emit dataChanged (topL, botR);
    }
    else
        return false;
    
    return true;
}

bool KKSEIODataModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    //qDebug () << __PRETTY_FUNCTION__ << row << count << parent;
    KKSTreeItem * pItem = getItem (parent);
    bool ok (true);
    beginInsertRows (parent, row, row+count-1);

    ok = pItem->insertChildren(row, count);
    endInsertRows ();
    return ok;
}

bool KKSEIODataModel :: removeRows (int row, int count, const QModelIndex& parent)
{
    //qDebug () << __PRETTY_FUNCTION__ << row << count << parent;
    KKSTreeItem * pItem = getItem (parent);
    bool ok (true);
    beginRemoveRows (parent, row, row+count-1);
    ok = pItem->removeChildren (row, count);
    endRemoveRows ();
    return ok;
}


const KKSCategoryAttr * KKSEIODataModel :: getFirstAttribute (KKSAttrType::KKSAttrTypes aType)
{
    if (!tRef)
        return 0;
    const KKSList<KKSAttrView*> tAttrs = tRef->sortedAttrs();
    int idAttr (-1);
    for (int i=0; i<tAttrs.count() && idAttr < 0; i++)
    {
        if (tAttrs[i]->type()->attrType() == aType)
            idAttr = tAttrs[i]->id();
    }
    if (idAttr < 0)
    {
        QList<int> pList = tRef->category()->searchAttributesByType(aType);
        if (!pList.empty())
            idAttr = pList[0];
    }
    const KKSCategoryAttr * cAttr = idAttr >= 0 ? tRef->category()->attribute(idAttr) : 0;
    return cAttr;
}

void KKSEIODataModel :: setupData (KKSTreeItem * parent)
{
    int nr = rowCount();
    parent->clearChildren ();
    if (!cAttrP)
    {
        for (KKSMap<qint64, KKSEIOData* >::const_iterator p = objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
        {
            KKSTreeItem * t = new KKSTreeItem (p.key(), p.value());
            parent->appendChild (t);
            //qDebug () << __PRETTY_FUNCTION__ << nr << parent->childCount();
        }
        if (nr > 0 && parent->childCount() > nr)
        {
            beginInsertRows (QModelIndex(), nr, parent->childCount()-1);
            endInsertRows ();
        }
        else if (nr > 0 && parent->childCount() < nr )
        {
            beginRemoveRows (QModelIndex(), parent->childCount(), nr-1);
            endRemoveRows ();
        }
        return;
    }

    QModelIndex pIndex = QModelIndex ();
    for (KKSMap<qint64, KKSEIOData* >::const_iterator p = objRecords.constBegin();
            p != objRecords.constEnd();
            p++)
    {
        KKSTreeItem * t = new KKSTreeItem (p.key(), p.value());
        QString valStr = p.value()->sysFieldValue(cAttrP->code(false));
        //qDebug () << __PRETTY_FUNCTION__ << p.key() << valStr << p.value()->sysFields();
        if (valStr.isEmpty())
            parent->appendChild (t);
        else
        {
            bool ok;
            qint64 iVal = valStr.toLongLong(&ok);
            KKSTreeItem * parent1 = getModelItem (iVal, rootItem, pIndex);
            if (!ok || !parent1)
                parent->appendChild (t);
            else
            {
                parent1->appendChild (t);
                //qDebug () << __PRETTY_FUNCTION__ << QString ("parent id is") << parent->id();
            }
        }
    }

}

KKSTreeItem * KKSEIODataModel :: getModelItem (qint64 val, KKSTreeItem * parent, QModelIndex & pIndex)
{
    if (!parent)
        return rootItem;

    //if (parent->getData() && QString::compare (parent->getData()->fieldValue(cAttrP->columnName(false)), valStr) == 0)
    //    return parent;
    for (int i=0; i<parent->childCount(); i++)
    {
        KKSTreeItem * item = parent->child(i);
        QString refCol = cAttrP->refColumnName(false);
        if (refCol.isEmpty())
            refCol = QString ("id");
        QString pStr = item->getData()->sysFieldValue(refCol);
        //qDebug () << __PRETTY_FUNCTION__ << pStr << refCol << cAttrP->code(false) << item->getData()->fields();
        bool ok (true);
        qint64 iVal (-1);
        if (QString::compare(refCol, QString("id")) == 0)
            iVal = item->id();
        else
            pStr.toLongLong(&ok);// cAttrP->columnName(false)
        if (!ok)
            continue;
        if (iVal == val)
        {
            pIndex = this->index (i, 0, pIndex);
            return item;
        }
        else if (item->childCount() > 0)
        {
            KKSTreeItem * chItem = getModelItem (val, item, pIndex);
            if (chItem)
                return chItem;
        }
        else
            continue;
    }
    
    return 0;
}

KKSTreeItem * KKSEIODataModel :: getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        KKSTreeItem *item = static_cast<KKSTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;   
}