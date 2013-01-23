/* 
 * File:   KKSSearchTemplateModel.cpp
 * Author: rusinov
 * 
 * Created on 14 январь 2013 г., 13:52
 */

#include <KKSSearchTemplateType.h>
#include <KKSSearchTemplate.h>
#include "KKSSearchTemplateModel.h"

KKSSearchTemplateModel::KKSSearchTemplateModel(const KKSMap<int, KKSSearchTemplateType *>& sTypes, const KKSList<KKSSearchTemplate *>& sTemplates, QObject * parent)
    : QAbstractItemModel (parent),
      searchTemplateTypes (sTypes),
      searchTemplates (sTemplates)
{
}

KKSSearchTemplateModel::~KKSSearchTemplateModel() {
}

QVariant KKSSearchTemplateModel :: data (const QModelIndex &index, int role) const
{
    Q_UNUSED (index);
    Q_UNUSED (role);
    return QVariant ();
}

QVariant KKSSearchTemplateModel :: headerData (int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED (section);
    Q_UNUSED (orientation);
    Q_UNUSED (role);
    return QVariant ();
}

QModelIndex KKSSearchTemplateModel :: index (int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED (row);
    Q_UNUSED (column);
    Q_UNUSED (parent);
    return QModelIndex ();
}

QModelIndex KKSSearchTemplateModel :: parent (const QModelIndex &index) const
{
    Q_UNUSED (index);
    return QModelIndex ();
}

int KKSSearchTemplateModel :: rowCount (const QModelIndex& parent) const
{
    Q_UNUSED (parent);
    return 0;
}

int KKSSearchTemplateModel :: columnCount (const QModelIndex& parent) const
{
    Q_UNUSED (parent);
    return 3;
}

Qt::ItemFlags KKSSearchTemplateModel :: flags (const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;   
}

bool KKSSearchTemplateModel :: setData (const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED (index);
    Q_UNUSED (value);
    Q_UNUSED (role);
    return false;
}

bool KKSSearchTemplateModel :: insertRows (int row, int count, const QModelIndex& parent)
{
    Q_UNUSED (row);
    Q_UNUSED (count);
    Q_UNUSED (parent);
    return false;
}

bool KKSSearchTemplateModel :: removeRows (int row, int count, const QModelIndex& parent)
{
    Q_UNUSED (row);
    Q_UNUSED (count);
    Q_UNUSED (parent);
    return false;
    
}

void KKSSearchTemplateModel :: setupData (const KKSMap<int, KKSSearchTemplateType *> sTemplateTypes, const KKSList<KKSSearchTemplate *>& sTemplates)
{
    Q_UNUSED (sTemplateTypes);
    Q_UNUSED (sTemplates);


}