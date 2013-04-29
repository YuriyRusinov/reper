#include "KKSOrgModel.h"

KKSOrgModel :: KKSOrgModel (QObject *parent)
        : QStandardItemModel (parent)
{
}

KKSOrgModel :: KKSOrgModel (int rows, int columns, QObject * parent)
    : QStandardItemModel (rows, columns, parent)
{

}

KKSOrgModel :: ~KKSOrgModel (void)
{

}

Qt::ItemFlags KKSOrgModel :: flags (const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    if (index.column() == 0 || index.column() == 4)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    else if (index.column() == 2 || index.column() == 3)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    
}

QVariant KKSOrgModel :: data (const QModelIndex& index, int role) const
{
    QVariant wData = QStandardItemModel::data (index, role);
    if (role == Qt::CheckStateRole && index.isValid () && (index.column()==0 || index.column()==4))
    {
        bool isChecked = QStandardItemModel::data (index, Qt::CheckStateRole).toBool();
        return isChecked ? Qt::Checked : Qt::Unchecked;
    }
    else
        return wData;
    
}

bool KKSOrgModel :: setData (const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::CheckStateRole && index.isValid () && (index.column()==0 || index.column()==4))
    {
        bool retVal = QStandardItemModel::setData (index, value.toBool(), Qt::CheckStateRole);
        emit dataChanged (index, index);
        return retVal;
    }
    else if (role == Qt::EditRole && (index.column() == 2 || index.column() == 3))
    {
        bool retVal = QStandardItemModel::setData (index, value.toString(), Qt::DisplayRole);
        emit dataChanged (index, index);
        return retVal;
    }
    else
        return QStandardItemModel::setData (index, value, role);
}


