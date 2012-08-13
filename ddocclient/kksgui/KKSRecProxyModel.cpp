/* 
 * File:   KKSRecProxyModel.cpp
 * Author: yuriy
 * 
 * Created on 17 ���� 2012 �., 22:20
 */

#include <KKSEIOData.h>
#include "KKSRecProxyModel.h"

KKSRecProxyModel :: KKSRecProxyModel(QObject * parent) : QAbstractProxyModel (parent)
{
}


KKSRecProxyModel :: ~KKSRecProxyModel()
{
}

QModelIndex KKSRecProxyModel :: index (int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex (row, column, parent))
        return QModelIndex ();
    
    QModelIndex sparent = mapToSource (parent);
    QAbstractItemModel * sModel = sourceModel ();
    if (!sModel)
        return QModelIndex ();
    
    QModelIndex sindex = sModel->index (row, column, sparent);
    
    return mapFromSource (sindex);
}

QModelIndex KKSRecProxyModel :: parent (const QModelIndex& index) const
{
    if (!sourceModel())
        return QModelIndex();
    const QModelIndex sourceIndex = mapToSource(index);
	
    const QModelIndex sourceParent = sourceIndex.parent();
    return mapFromSource(sourceParent);
}

int KKSRecProxyModel :: rowCount (const QModelIndex& parent) const
{
    if (!sourceModel())
        return 0;
    QAbstractItemModel * sModel = sourceModel ();
    int nr = sModel->rowCount(mapToSource(parent));
    int nRes (nr);
//    for (int i=0; i<nr; i++)
//    {
//        KKSEIOData * d = sModel->data(sModel->index(i, 0, mapToSource(parent)), Qt::UserRole+1).value<KKSEIOData *> ();
//        if (!d->isVisible())
//            nRes--;
//    }
    return nRes;
}

int KKSRecProxyModel :: columnCount (const QModelIndex& parent) const
{
    if (!sourceModel())
        return 0;
    QAbstractItemModel * sModel = sourceModel ();
    return sModel->columnCount(mapToSource(parent));
}

QModelIndex KKSRecProxyModel::mapFromSource (const QModelIndex& sourceIndex) const
{
    if (!sourceModel() || !sourceIndex.isValid())
        return QModelIndex();
    
    KKSEIOData * d = sourceIndex.data(Qt::UserRole+1).value<KKSEIOData *>();
    if (!d || !d->isVisible())
        return QModelIndex();
    
    bool v = d->isVisible();
    QModelIndex wIndex (sourceIndex.parent());
    
    QPersistentModelIndex spIndex (sourceIndex);
    if (mapping.contains(spIndex))
        return mapping.value(spIndex);
    
    if (!wIndex.isValid())
    {
        QModelIndex resIndex = createIndex (sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
        //qDebug () << __PRETTY_FUNCTION__ << sourceIndex << resIndex;
        mapping.insert (QPersistentModelIndex (sourceIndex), QPersistentModelIndex (resIndex));
        return resIndex;//createIndex (sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
    }
    
    KKSEIOData * dw = wIndex.data(Qt::UserRole+1).value<KKSEIOData *>();
    if (!dw || !dw->isVisible())
        v = false;
    if (!v)
        return QModelIndex();
    
    QModelIndex resIndex = createIndex (sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
    //qDebug () << __PRETTY_FUNCTION__ << sourceIndex << resIndex;
    mapping.insert (QPersistentModelIndex (sourceIndex), QPersistentModelIndex (resIndex));
    return resIndex;//createIndex (sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
}

QModelIndex KKSRecProxyModel::mapToSource (const QModelIndex& proxyIndex) const
{
    QAbstractItemModel * sModel = sourceModel ();
    //qDebug () << __PRETTY_FUNCTION__ << proxyIndex;
    if ( !sModel || !proxyIndex.isValid())
        return QModelIndex();
    QModelIndex sInd (mapping.key(proxyIndex));
    return sInd;
/*    if (!proxyIndex.parent().isValid())
        return sModel->index (proxyIndex.row(), proxyIndex.column());
    else
        return sModel->index(proxyIndex.row(), proxyIndex.column(), mapToSource(proxyIndex.parent()));*/
}

