#include "defines.h"
#include "KKSSearchTemplateFilterProxyModel.h"

KKSSearchTemplateFilterProxyModel :: KKSSearchTemplateFilterProxyModel (QObject *parent)
    : QSortFilterProxyModel (parent)
{
}

KKSSearchTemplateFilterProxyModel :: ~KKSSearchTemplateFilterProxyModel (void)
{
}

bool KKSSearchTemplateFilterProxyModel :: filterAcceptsRow (int source_row, const QModelIndex& source_parent) const
{
    if (!sourceModel())
        return false;
    QModelIndex wIndex = sourceModel()->index (source_row, 0, source_parent);
    if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
        return true;
    else
        return QSortFilterProxyModel::filterAcceptsRow (source_row, source_parent);
}

bool KKSSearchTemplateFilterProxyModel :: lessThan (const QModelIndex& left, const QModelIndex& right) const
{
    if (left.data (Qt::UserRole+USER_ENTITY).toInt() != right.data (Qt::UserRole+USER_ENTITY).toInt())
        return left.data (Qt::UserRole+USER_ENTITY).toInt() < right.data (Qt::UserRole+USER_ENTITY).toInt();
    else
        return left.data (Qt::DisplayRole).toString() < right.data (Qt::DisplayRole).toString();
}
