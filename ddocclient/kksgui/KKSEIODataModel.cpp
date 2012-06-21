#include <QtDebug>

#include "KKSTemplate.h"
#include "KKSCategory.h"
#include "KKSAttrType.h"
#include "KKSCategoryAttr.h"
#include "KKSAttrView.h"
#include "KKSEIOData.h"
#include "KKSEIODataModel.h"

KKSEIODataModel :: KKSEIODataModel (const KKSTemplate * t, const KKSMap<qint64, KKSEIOData *>& objRecs, QObject *parent)
    : QAbstractItemModel (parent),
    tRef (t),
    objRecords (objRecs)
{
    if (tRef)
        tRef->addRef();
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
    if (!tRef)
        return 0;
    QList<int> pList = tRef->category()->searchAttributesByType (KKSAttrType::atParent);
    if (!parent.isValid() && pList.empty())
        return objRecords.count();
    else if (!parent.isValid() && !pList.empty())
    {
        int idAttr = pList[0];
        const KKSCategoryAttr * cAttr = tRef->category()->attribute(idAttr);
        int ncount = 0;
        for (KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
            if (p.value()->fieldValue(cAttr->code(false)).isEmpty())
                ncount++;
        return ncount;
    }
    else if (parent.isValid() && !pList.empty())
    {
        int idAttr = pList[0];
        const KKSCategoryAttr * cAttr = tRef->category()->attribute(idAttr);
        qint64 val = parent.internalId();//data (Qt::UserRole).toInt();
        QString valStr = objRecords.constFind (val).value()->fieldValue(cAttr->columnName(false));
        //qDebug () << __PRETTY_FUNCTION__ << val << valStr << cAttr->columnName (false) << cAttr->code(false); 
        int ncount = 0;
        for (KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
        {
            //qDebug () << __PRETTY_FUNCTION__ << ncount << p.value()->fieldValue(cAttr->columnName(false)) << p.value()->fieldValue(cAttr->code(false));
            if (QString::compare (p.value()->fieldValue(cAttr->code(false)), valStr)==0)
                ncount++;
        }
        qDebug () << __PRETTY_FUNCTION__ << ncount;
        return ncount;
    }
    else if (parent.isValid() && pList.empty())
        return 0;
    
    return 0;//objRecords.count();
}

QModelIndex KKSEIODataModel :: index (int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex (row, column, parent))
        return QModelIndex ();
    //qDebug () << __PRETTY_FUNCTION__ << row << column << parent << parent.internalId();
    if (parent.isValid())
    {
        qint64 idp = parent.internalId();
        QList<int> pList = tRef->category()->searchAttributesByType (KKSAttrType::atParent);
        if (pList.isEmpty())
            return QModelIndex ();
        int idAttr = pList[0];
        QList<qint64> vals;
        const KKSCategoryAttr * cAttr = tRef->category()->attribute(idAttr);
        KKSMap<qint64, KKSEIOData*>::const_iterator par = objRecords.constFind (idp);
        if (par == objRecords.constEnd())
            return QModelIndex ();
        QString valStr = par.value()->fieldValue(cAttr->columnName(false));
        for (KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
                p != objRecords.constEnd();
                p++)
            if (QString::compare (p.value()->fieldValue(cAttr->code(false)), valStr)==0)
                vals += p.key();
        //qDebug () << __PRETTY_FUNCTION__ << vals << valStr << cAttr->name() << par.key() << cAttr->columnName(false);
        return QModelIndex ();
        if (row >= 0 && row<vals.count())
        {
            //qDebug () << __PRETTY_FUNCTION__ << row << column << vals[row];
            return createIndex (row, column, (quint32)vals[row]);
        }
        else
            return QModelIndex();
    }
    KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
    p += row;
    if (p != objRecords.constEnd())
        return createIndex (row, column, (quint32)p.key());
    else
        return QModelIndex ();
}

QModelIndex KKSEIODataModel :: parent (const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();
    qint64 idw = index.internalId();
    //qDebug () << __PRETTY_FUNCTION__ << index << idw;
    QList<int> pList = tRef->category()->searchAttributesByType (KKSAttrType::atParent);
    if (pList.isEmpty())
        return QModelIndex ();
    int idAttr = pList[0];
    int irow (-1);
    int i = 0;
    const KKSCategoryAttr * cAttr = tRef->category()->attribute(idAttr);
    QString valStr = objRecords.constFind (idw).value()->fieldValue(cAttr->columnName(false));
    KKSMap<qint64, KKSEIOData*>::const_iterator p;
    for (p = objRecords.constBegin();
            p != objRecords.constEnd() && irow < 0;
            p++)
    {
        if (QString::compare (p.value()->fieldValue(cAttr->code(false)), valStr)==0)
            irow = i;
        i++;
    }
    if (irow < 0 || p == objRecords.constEnd())
        return QModelIndex ();
    else
    {
        QModelIndex pIndex = createIndex (irow, 0, (quint32)p.key());
        //qDebug () << __PRETTY_FUNCTION__ << pIndex << p.key();
        return pIndex;
    }
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
    if (orientation == Qt::Horizontal && section < avList.count() && role == Qt::DisplayRole)
    {
        QVariant v = avList[section]->code(false);
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
}

bool KKSEIODataModel :: removeRows (int row, int count, const QModelIndex& parent)
{
}
 */