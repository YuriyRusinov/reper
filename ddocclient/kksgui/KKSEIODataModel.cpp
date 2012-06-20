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
    else if (parent.isValid() )
        return 0;
    
    return 0;//objRecords.count();
}

QModelIndex KKSEIODataModel :: index (int row, int column, const QModelIndex& parent) const
{
    if (parent.isValid())
        return QModelIndex();
    KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
    p += row;
    return createIndex (row, column, (quint32)p.key());
}

QModelIndex KKSEIODataModel :: parent (const QModelIndex& index) const
{
    return QModelIndex();
}

QVariant KKSEIODataModel :: data (const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    KKSMap<qint64, KKSEIOData*>::const_iterator p=objRecords.constBegin();
    p += index.row();
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
        return p.value()->fieldValue (aCode);
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