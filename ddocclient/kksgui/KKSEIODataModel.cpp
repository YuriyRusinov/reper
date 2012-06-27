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
    parIndexList (QMap<qint64, QModelIndex>()),
    indList (QMap<qint64, QList<qint64> >()),
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
    if (!cAttrP)
        return parent.isValid() ? 0 : objRecords.count();

    if (!parent.isValid() )
    {
        int ncount = 0;
        for (KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
            if (p.value()->fieldValue(cAttrP->code(false)).isEmpty())
                ncount++;
        return ncount;
    }
    else //if (parent.isValid() )
    {
        qint64 val = parent.internalId();//data (Qt::UserRole).toInt();
        QString valStr = objRecords.constFind (val).value()->fieldValue(cAttrP->columnName(false));
        int ncount = 0;
        for (KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
        {
            if (QString::compare (p.value()->fieldValue(cAttrP->code(false)), valStr)==0)
                ncount++;
        }
        return ncount;
    }
}
 

QModelIndex KKSEIODataModel :: index (int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex (row, column, parent))
        return QModelIndex ();
//    if (parent.internalId() >= 5 && parent.internalId() <= 9)
//        qDebug () << __PRETTY_FUNCTION__ << row << column << parent << parent.internalId();

    if (cAttrP)
    {
        qint64 idp = parent.isValid() ? parent.internalId() : -1;
        if (indList.contains(idp))
        {
            QList<qint64> vals = indList.value (idp);
            if (row >= 0 && row<vals.count())
            {
                //qDebug () << __PRETTY_FUNCTION__ << row << column << vals[row];
                QModelIndex ind = createIndex (row, column, (quint32)vals[row]);
                return ind;
            }
        }
        KKSMap<qint64, KKSEIOData*>::const_iterator par = objRecords.constFind (idp);
        QString valStr = (par == objRecords.constEnd() ? QString() : par.value()->fieldValue(cAttrP->columnName(false)));
        QList<qint64> vals;
        for (KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
            if (QString::compare (p.value()->fieldValue(cAttrP->code(false)), valStr)==0)
                vals += p.key();
        //qDebug () << __PRETTY_FUNCTION__ << idp << valStr << vals << row;
        indList.insert (idp, vals);
        
        if (row >= 0 && row<vals.count())
        {
            //qDebug () << __PRETTY_FUNCTION__ << row << column << vals[row];
            QModelIndex ind = createIndex (row, column, (quint32)vals[row]);
            return ind;
        }
        else
        {
            //qDebug () << __PRETTY_FUNCTION__ << row << vals;
            return QModelIndex ();
        }
 
    }
    else
    {
        KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
        for (int i=0; i<row && p != objRecords.constEnd(); i++ )
            p++;
        //p += row;
        if (p != objRecords.constEnd())
            return createIndex (row, column, (quint32)p.key());
        else
            return QModelIndex ();
    }
}

QModelIndex KKSEIODataModel :: parent (const QModelIndex& index) const
{
//    Q_UNUSED (index);
//    return QModelIndex ();
    if (!index.isValid())
        return QModelIndex();
    qint64 idw = index.internalId();
    if (!cAttrP)
        return QModelIndex ();
    if (parIndexList.contains(idw))
        return parIndexList.value (idw);
    KKSMap<qint64, KKSEIOData*>::const_iterator par = objRecords.constFind (idw);
    if (par == objRecords.constEnd())
        return QModelIndex ();
    QString valStr = par.value()->fieldValue(cAttrP->code(false));
    //qDebug () << __PRETTY_FUNCTION__ << index << idw << valStr << par.key() << par.value()->fields();
    if (valStr.isEmpty())
        return QModelIndex ();
    qint64 ival (-1);
    QList<qint64> vals;
    QString valStr1;
    QMultiMap<QString, qint64> pVals;
    for (KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
            p != objRecords.constEnd();
            p++)
    {
        pVals.insert(p.value()->fieldValue(cAttrP->code(false)), p.key());
        if (QString::compare (p.value()->fieldValue(cAttrP->columnName(false)), valStr)==0)
        {
            ival = p.key();
            //qDebug () << __PRETTY_FUNCTION__ << ival;
            KKSMap<qint64, KKSEIOData*>::const_iterator par1 = objRecords.constFind (ival);
            if (par1 == objRecords.constEnd())
                return QModelIndex ();
            valStr1 = par1.value()->fieldValue(cAttrP->columnName(false));
            if (valStr1.isEmpty())
                return QModelIndex ();
        }
    }
    vals = pVals.values(valStr1);
/*    for (KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
            p != objRecords.constEnd();
            p++)
        if (QString::compare (p.value()->fieldValue(cAttrP->code(false)), valStr1)==0)
            vals += p.key();
 */
    if (!vals.contains(idw))
        return QModelIndex ();
    
    int prow = vals.indexOf(idw);
    QModelIndex pIndex = createIndex (prow, 0, (quint32)ival);
    parIndexList.insert (idw, pIndex);
    //qDebug () << __PRETTY_FUNCTION__ << pIndex << ival << index;
    return pIndex;
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
    if (!index.isValid())
        return QVariant();

    qint64 idw = index.internalId();
    //if (!index.parent().isValid())
    //    qDebug () << __PRETTY_FUNCTION__ << index << idw;
    //if (index.parent().isValid())
    //    qDebug () << __PRETTY_FUNCTION__ << idw << index.parent().internalId();
    KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constFind(idw);
    if (p == objRecords.constEnd())
        return QVariant ();
    if (role == Qt::UserRole)
    {
        return p.key();
    }
    else if (role == Qt::DisplayRole)
    {
        KKSList<KKSAttrView*> avList = tRef ? tRef->sortedAttrs() : KKSList<KKSAttrView*>();
        if (index.column() >= avList.count())
            return QVariant ();
        QString aCode = avList[index.column()]->code(false);
        QString attrValue = p.value()->fieldValue (aCode);
        if (attrValue.isEmpty())
            attrValue = p.value()->fields().value (aCode.toLower());
        else if (attrValue.contains ("\n"))
            attrValue = attrValue.mid (0, attrValue.indexOf("\n")) + "...";
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
    Q_UNUSED (index);
    Q_UNUSED (value);
    Q_UNUSED (role);
    return false;
}
/*
bool KKSEIODataModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    return false;
}

bool KKSEIODataModel :: removeRows (int row, int count, const QModelIndex& parent)
{
}
 */

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
    if (!cAttrP)
    {
        for (KKSMap<qint64, KKSEIOData* >::const_iterator p = objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
        {
            KKSTreeItem * t = new KKSTreeItem (p.key(), p.value());
            rootItem->appendChild (t);
        }
        return;
    }

    for (KKSMap<qint64, KKSEIOData* >::const_iterator p = objRecords.constBegin();
            p != objRecords.constEnd();
            p++)
    {
        KKSTreeItem * t = new KKSTreeItem (p.key(), p.value());
        QString valStr = p.value()->fieldValue(cAttrP->code(false));
        if (valStr.isEmpty())
            rootItem->appendChild (t);
        else
        {
            KKSTreeItem * parent = getModelItem (valStr, rootItem);
            if (!parent)
                rootItem->appendChild (t);
            else
                parent->appendChild (t);
        }
    }

}

KKSTreeItem * KKSEIODataModel :: getModelItem (QString valStr, KKSTreeItem * parent)
{
    if (!parent || valStr.isEmpty() || !parent->getData())
        return rootItem;

    if (QString::compare (parent->getData()->fieldValue(cAttrP->columnName(false)), valStr) == 0)
        return parent;
    for (int i=0; i<parent->childCount(); i++)
    {
        KKSTreeItem * item = parent->child(i);
        if (QString::compare (item->getData()->fieldValue(cAttrP->columnName(false)), valStr) == 0)
            return item;
        else if (item->childCount() > 0)
        {
            KKSTreeItem * chItem = getModelItem (valStr, item);
            if (chItem)
                return chItem;
        }
        else
            continue;
    }
    
    return 0;
}