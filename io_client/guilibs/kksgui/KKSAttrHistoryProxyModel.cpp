/* 
 * File:   KKSAttrHistoryProxyModel.cpp
 * Author: yuriyrusinov
 * 
 * Created on 31 Èþëü 2013 ã., 17:10
 */

#include <QDateTime>

#include "KKSAttrHistoryProxyModel.h"

KKSAttrHistoryProxyModel::KKSAttrHistoryProxyModel(QObject * parent)
    : QSortFilterProxyModel (parent)
{
}

KKSAttrHistoryProxyModel::~KKSAttrHistoryProxyModel()
{
}

bool KKSAttrHistoryProxyModel::lessThan (const QModelIndex & left, const QModelIndex & right) const
{
    switch (left.column())
    {
        case 0: return left.data(Qt::DisplayRole).toInt() < right.data(Qt::DisplayRole).toInt(); break;
        case 1: case 2: return QDateTime::fromString (left.data(Qt::DisplayRole).toString(), QString("dd.MM.yyyy H:mm:ss")) < QDateTime::fromString (right.data(Qt::DisplayRole).toString(),  QString("dd.MM.yyyy H:mm:ss")); break;
        default: return left.data(Qt::DisplayRole).toString() < right.data(Qt::DisplayRole).toString(); break;
    }
}