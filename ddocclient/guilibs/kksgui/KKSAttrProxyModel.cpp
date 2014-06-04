#include <KKSAttribute.h>
#include "KKSAttrProxyModel.h"
#include "defines.h"

KKSAttrProxyModel :: KKSAttrProxyModel (QObject * parent) :
    QAbstractProxyModel (parent)
{
    attrsGroups.clear();
}

KKSAttrProxyModel :: ~KKSAttrProxyModel (void)
{
}

QVariant KKSAttrProxyModel :: data (const QModelIndex& index, int role) const
{
    if (index.isValid())
    {
        QModelIndex sIndex = this->mapToSource (index);
        QVariant wData = this->sourceModel()->data(index, role);
        return wData;
    }
    else
        return QVariant();
}

bool KKSAttrProxyModel :: setData (const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() && role == Qt::UserRole+1)
    {
        attrsGroups.clear();
        QMap<QString, QVariant> vattrsg = value.toMap();
        KKSMap<int, KKSAGroup*> attrg;
        for (QMap<QString, QVariant>::const_iterator pv = vattrsg.constBegin();
             pv != vattrsg.constEnd();
             pv++)
        {
            int id = pv.key().toInt();
            if (pv.value().canConvert<KKSAGroup*>())
            {
                KKSAGroup * aGroup = pv.value().value<KKSAGroup*>();
                attrsGroups.insert (id, aGroup);
            }
        }
        return true;
    }
    else if (index.isValid() && role == Qt::UserRole+1)
    {
        if (index.data (Qt::UserRole+USER_ENTITY).toInt() ==0 && value.canConvert<KKSAGroup*>())
        {
            KKSAGroup * aGroup = value.value<KKSAGroup*>();
            int idAGroup = aGroup->id();
            if (!index.parent().isValid())
            {
                KKSMap<int, KKSAGroup*>::iterator pg = attrsGroups.begin();
                pg += index.row();
                if (pg != attrsGroups.end())
                    attrsGroups.erase (pg);
                attrsGroups.insert (idAGroup, aGroup);
            }
        }
        else if (value.canConvert<KKSAttribute*>())
        {
            
        }
        return true;
    }
    return false;
}

QVariant KKSAttrProxyModel :: headerData (int section, Qt::Orientation orientation, int role) const
{
    return this->sourceModel()->headerData (section, orientation, role);
}

int KKSAttrProxyModel :: rowCount (const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 0;
}

bool KKSAttrProxyModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(row);
    Q_UNUSED(count);

    return true;
}

bool KKSAttrProxyModel :: removeRows (int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(row);
    Q_UNUSED(count);
    
    return true;
}

QModelIndex KKSAttrProxyModel :: mapFromSource (const QModelIndex& sourceIndex) const
{
    if (sourceIndex.data(Qt::UserRole+USER_ENTITY).toInt() == 0 || attrsGroups.isEmpty())
        return sourceIndex;
    else
    {
        bool isFound = false;
        for (KKSMap<int, KKSAGroup*>::const_iterator p = attrsGroups.constBegin();
             p != attrsGroups.constEnd() && !isFound;
             p++)
        {
            
        }
    }

    return sourceIndex;
}

QModelIndex KKSAttrProxyModel :: mapToSource (const QModelIndex& proxyIndex) const
{
    return proxyIndex;
}

