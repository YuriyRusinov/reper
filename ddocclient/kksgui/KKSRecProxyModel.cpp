/* 
 * File:   KKSRecProxyModel.cpp
 * Author: yuriy
 * 
 * Created on 17 Èþëü 2012 ã., 22:20
 */

#include "KKSRecProxyModel.h"

KKSRecProxyModel::KKSRecProxyModel(QObject * parent) : QAbstractProxyModel (parent)
{
}


KKSRecProxyModel::~KKSRecProxyModel()
{
}

QModelIndex KKSRecProxyModel::mapFromSource (const QModelIndex& sourceIndex) const
{
    return sourceIndex;
}

QModelIndex KKSRecProxyModel::mapToSource (const QModelIndex& proxyIndex) const
{
    return proxyIndex;
}

