#include "KKSCheckableModel.h"

KKSCheckableModel :: KKSCheckableModel (QObject *parent)
    : QStandardItemModel (parent)
{
}

KKSCheckableModel :: KKSCheckableModel (int rows, int columns, QObject * parent)
    : QStandardItemModel (rows, columns, parent)
{
}

KKSCheckableModel :: ~KKSCheckableModel (void)
{
}

Qt::ItemFlags KKSCheckableModel :: flags (const QModelIndex & index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant KKSCheckableModel :: data (const QModelIndex& index, int role) const
{
    QVariant wData = QStandardItemModel::data (index, role);
    if (role == Qt::CheckStateRole && index.isValid () && index.column()==0)
    {
        bool isChecked = QStandardItemModel::data (index, Qt::CheckStateRole).toBool();
        return isChecked ? Qt::Checked : Qt::Unchecked;
    }
    else
        return wData;
}

bool KKSCheckableModel :: setData (const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::CheckStateRole && index.isValid ())
    {
        bool retVal = QStandardItemModel::setData (index, value.toBool(), Qt::CheckStateRole);
        emit dataChanged (index, index);
        emit checkStateChanged(index, index);
        return retVal;
    }
    else
        return QStandardItemModel::setData (index, value, role);
}
