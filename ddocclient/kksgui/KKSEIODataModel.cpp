#include "KKSTemplate.h"
#include "KKSEIOData.h"
#include "KKSEIODataModel.h"

KKSEIODataModel :: KKSEIODataModel (KKSTemplate * t, const KKSMap<qint64, KKSEIOData *>& objRecs, QObject *parent)
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
}

QModelIndex KKSEIODataModel :: index (int row, int column, const QModelIndex& parent) const
{
}

QModelIndex KKSEIODataModel :: parent (const QModelIndex& index ) const
{
}

QVariant KKSEIODataModel :: data (const QModelIndex& index, int role) const
{
}

bool KKSEIODataModel :: setData (const QModelIndex& index, const QVariant& value, int role)
{
}

bool KKSEIODataModel :: insertRows (int row, int count, const QModelIndex& parent)
{
}

bool KKSEIODataModel :: removeRows (int row, int count, const QModelIndex& parent)
{
}