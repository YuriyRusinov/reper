/* 
 * File:   KKSRecProxyModel.cpp
 * Author: yuriy
 * 
 * Created on 17 Èþëü 2012 ã., 22:20
 */

#include <KKSEIOData.h>
#include "KKSRecProxyModel.h"

KKSRecProxyModel :: KKSRecProxyModel(QObject * parent) : QAbstractProxyModel (parent)
{
    fixModel ();
}


KKSRecProxyModel :: ~KKSRecProxyModel()
{
}

QModelIndex KKSRecProxyModel :: index (int row, int column, const QModelIndex& parent) const
{
/*    QModelIndex sourceParent;
    if (parent.isValid())
        sourceParent = mapToSource(parent);
    QMapIterator<QPersistentModelIndex, QPersistentModelIndex> it(proxySourceParent);
    while (it.hasNext()) {
        it.next();
        if (it.value() == sourceParent && it.key().row() == row) //&&
            //it.key().column() == column)
            return it.key();//.sibling (row, column);
    }
    return QModelIndex();*/
    if (!hasIndex (row, column, parent))
        return QModelIndex ();
    
    QModelIndex sparent = mapToSource (parent);
    QAbstractItemModel * sModel = sourceModel ();
    if (!sModel)
        return QModelIndex ();
    
    QModelIndex sindex = sModel->index (row, column, sparent);
//    KKSEIOData * dw = sindex.data(Qt::UserRole+1).value<KKSEIOData *>();
//    if (!dw || !dw->isVisible())
//    {
//        qDebug () << __PRETTY_FUNCTION__ << "Data is invisible";
//        return QModelIndex ();
//    }
    
    return mapFromSource (sindex);

}

QModelIndex KKSRecProxyModel :: parent (const QModelIndex& index) const
{
    QModelIndex mi = proxySourceParent.value(index);
    if (mi.isValid())
        return mapFromSource(mi);
    return QModelIndex();
/*    if (!sourceModel())
        return QModelIndex();
    const QModelIndex sourceIndex = mapToSource(index);
    KKSEIOData * dw = sourceIndex.data(Qt::UserRole+1).value<KKSEIOData *>();
    if (!dw || !dw->isVisible())
        return QModelIndex ();
	
    const QModelIndex sourceParent = sourceIndex.parent();
    KKSEIOData * dwp = sourceParent.isValid() ? sourceParent.data(Qt::UserRole+1).value<KKSEIOData *>() : 0;
    if (!dwp || !dwp->isVisible())
        return QModelIndex ();
    return mapFromSource(sourceParent);*/
}

int KKSRecProxyModel :: rowCount (const QModelIndex& parent) const
{
    if (!sourceModel())
        return 0;
    QModelIndex sourceParent;
    if (parent.isValid())
        sourceParent = mapToSource(parent);
    int count = 0;
    QMapIterator<QPersistentModelIndex, QPersistentModelIndex> it(proxySourceParent);
    while (it.hasNext()) {
        it.next();
        KKSEIOData * d = it.value().data (Qt::UserRole+1).value<KKSEIOData *>();
        if (it.value() == sourceParent && (!d || (d && d->isVisible())) )
            count++;
    }
    if (parent.isValid())
        qDebug () << __PRETTY_FUNCTION__ << count << parent;
    else
        qDebug () << __PRETTY_FUNCTION__ << count;
        
    return count;
 
/*    QAbstractItemModel * sModel = sourceModel ();
    int nr = sModel->rowCount(mapToSource(parent));
    int nRes (nr);
    return nRes;*/
}

int KKSRecProxyModel :: columnCount (const QModelIndex& parent) const
{
    if (!sourceModel())
        return 0;
    QAbstractItemModel * sModel = sourceModel ();
    QModelIndex sParent = mapToSource(parent);
/*    KKSEIOData * dwp = sParent.isValid() ? sParent.data(Qt::UserRole+1).value<KKSEIOData *>() : 0;
    if (!dwp || !dwp->isVisible())
        return 0;*/
    return sModel->columnCount(sParent);
}

QModelIndex KKSRecProxyModel::mapFromSource (const QModelIndex& sourceIndex) const
{
    if (!sourceModel() || !sourceIndex.isValid())
        return QModelIndex();

    return mapping.value(sourceIndex);
/*    QAbstractItemModel * sModel = sourceModel ();
    qDebug () << __PRETTY_FUNCTION__ << sourceIndex;
    KKSEIOData * d = sModel->data(sourceIndex, Qt::UserRole+1).value<KKSEIOData *>();
    if (!d || !d->isVisible())
    {
        //if (d)
            qDebug () << __PRETTY_FUNCTION__ << "Record is not visible";
        return QModelIndex();
    }
    
    bool v = d->isVisible();
    if (!v)
        qDebug () << __PRETTY_FUNCTION__ << sourceIndex << v;
    QModelIndex wIndex (sourceIndex.parent());

    QPersistentModelIndex spIndex (sourceIndex);
    if (mapping.contains(spIndex) && spIndex.data(Qt::UserRole+1).value<KKSEIOData *>()->isVisible())
    {
        qDebug () << __PRETTY_FUNCTION__ << "Mapping";
        return mapping.value(spIndex);
    }
    
    if (!wIndex.isValid())
    {
        QModelIndex resIndex = createIndex (sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
        //qDebug () << __PRETTY_FUNCTION__ << sourceIndex << resIndex;
        mapping.insert (QPersistentModelIndex (sourceIndex), QPersistentModelIndex (resIndex));
        return resIndex;//createIndex (sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
    }
    
    KKSEIOData * dw = sModel->data(wIndex, Qt::UserRole+1).value<KKSEIOData *>();
    if (!dw || !dw->isVisible())
    {
        qDebug () << __PRETTY_FUNCTION__ << "Record is not visible";
        v = false;
    }
    if (!v)
        return QModelIndex();
    
    QModelIndex resIndex = createIndex (sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
    //qDebug () << __PRETTY_FUNCTION__ << sourceIndex << resIndex;
    mapping.insert (QPersistentModelIndex (sourceIndex), QPersistentModelIndex (resIndex));
    return resIndex;//createIndex (sourceIndex.row(), sourceIndex.column(), sourceIndex.internalPointer());
 */
}

QModelIndex KKSRecProxyModel::mapToSource (const QModelIndex& proxyIndex) const
{
    if (!proxyIndex.isValid())
        return QModelIndex();
    return mapping.key(proxyIndex);
/*    QAbstractItemModel * sModel = sourceModel ();
    //qDebug () << __PRETTY_FUNCTION__ << proxyIndex;
    if ( !sModel || !proxyIndex.isValid())
        return QModelIndex();
    QModelIndex sInd (mapping.key(proxyIndex));
    return sInd;*/
/*    if (!proxyIndex.parent().isValid())
        return sModel->index (proxyIndex.row(), proxyIndex.column());
    else
        return sModel->index(proxyIndex.row(), proxyIndex.column(), mapToSource(proxyIndex.parent()));*/
}

void KKSRecProxyModel::setSourceModel (QAbstractItemModel * sourceMod)
{
    QAbstractProxyModel::setSourceModel (sourceMod);
    fixModel ();
    if (sourceMod)
        connect (sourceMod, SIGNAL (dataChanged(const QModelIndex&, const QModelIndex&)),
                 this,
                 SLOT (sourceDataChanged(const QModelIndex&, const QModelIndex&)) );
}

void KKSRecProxyModel::sourceDataChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    qDebug () << __PRETTY_FUNCTION__ << topLeft << bottomRight;
    fixModel ();//topLeft, true);
}

void KKSRecProxyModel::fixModel (const QModelIndex& psIndex, bool withClear)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (withClear)//!psIndex.isValid())
    {
        mapping.clear();
        proxySourceParent.clear();
    }
    QAbstractItemModel * sModel = sourceModel ();
    if (!sModel)
        return;
    int nr = sModel->rowCount (psIndex);
    int nc = sModel->columnCount (psIndex);
    int ir = 0;
    for (int i=0; i<nr; i++)
    {
        QModelIndex wIndex = sModel->index (i, 0, psIndex);
        KKSEIOData * d = sModel->data (wIndex, Qt::UserRole+1).value<KKSEIOData *>();
        if (!d || !d->isVisible())
        {
            qDebug () << __PRETTY_FUNCTION__ << "Data is invisible" << i << wIndex << psIndex;
            continue;
        }
        for (int j=0; j<nc; j++)
        {
            QModelIndex proxy = createIndex (ir, j, wIndex.internalPointer());
            mapping.insert (QPersistentModelIndex (wIndex.sibling(i, j)), proxy);
            //QModelIndex sourceParent;
            //if (psIndex.isValid())
            //    sourceParent = psIndex;
            if (j==0)
                proxySourceParent.insert (proxy, psIndex);//sourceParent);
        }
        if (sModel->rowCount (wIndex) > 0)
            fixModel (wIndex, false);
        ir++;
    }
}