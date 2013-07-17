/* 
 * File:   KKSAttrModel.cpp
 * Author: yuriyrusinov
 * 
 * Created on 17 ���� 2013 �., 13:07
 */
#include <KKSMap.h>
#include <KKSAttribute.h>
#include "KKSAttrModel.h"

/*
 * ����������� ������� ������ �������� �������� ��� ����������� �����������
 * � ���������� �������, ����������� QAbstractItemView *
 */
KKSAttrModel::KKSAttrModel(const KKSAttribute * _attr, QObject *parent) 
    : QAbstractItemModel (parent),
    attr (_attr)
{
}

KKSAttrModel::~KKSAttrModel()
{
}

int KKSAttrModel::columnCount (const QModelIndex& /*parent*/) const
{
    return 4;
}

int KKSAttrModel::rowCount (const QModelIndex& parent) const
{
    if (!attr || parent.isValid())
        return 0;
    return attr->attrs().count();
}

QModelIndex KKSAttrModel::index (int row, int column, const QModelIndex& parent) const
{
    if (!attr || parent.isValid() || !hasIndex (row, column, parent))
        return QModelIndex ();

    //qDebug () << __PRETTY_FUNCTION__ << row << parent;
    if (parent.isValid())
        return QModelIndex();

    const KKSMap<int, KKSCategoryAttr *> cAttrs = attr->attrs();
    if (row >= cAttrs.count())
        return QModelIndex();
    const KKSMap<int, KKSCategoryAttr *>::const_iterator pc = cAttrs.constBegin()+row;
    if (pc != cAttrs.constEnd())
        return createIndex (row, column, pc.value());
    else
        return QModelIndex();
}

QModelIndex KKSAttrModel::parent (const QModelIndex& index) const
{
    return QModelIndex();
}

Qt::ItemFlags KKSAttrModel::flags (const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        
}

QVariant KKSAttrModel::data (const QModelIndex& index, int role) const
{
    if (!attr)
        return QVariant();
    if (role == Qt::UserRole+2)
        return QVariant::fromValue<const KKSAttribute *>(attr);

    if (!index.isValid())
        return QVariant();
    int irow = index.row();
    const KKSMap<int, KKSCategoryAttr *> cAttrs = attr->attrs();
    if (irow >= cAttrs.count())
        return QVariant();
    const KKSMap<int, KKSCategoryAttr *>::const_iterator pc = cAttrs.constBegin()+irow;
    
    if (pc==cAttrs.constEnd())
        return QVariant();

    if (role == Qt::UserRole)
        return pc.key();

    if (role == Qt::UserRole+1)
        return QVariant::fromValue<KKSCategoryAttr *>(pc.value());

    KKSCategoryAttr * cAttr (pc.value());
    if (role == Qt::DisplayRole)
    {
        int iCol = index.column();
        switch (iCol)
        {
            case 0: return QString::number (pc.key()); break;
            case 1: return cAttr->name(); break;
            case 2: return (cAttr->isMandatory() ? tr("Mandatory") : QString()); break;
            case 3: return (cAttr->isReadOnly() ? tr("Read only") : QString()); break;
            default: return QVariant(); break;
        }
    }
    
    return QVariant();
}

bool KKSAttrModel::setData (const QModelIndex& index, const QVariant& value, int role)
{
    Q_UNUSED (index);
    Q_UNUSED (value);
    Q_UNUSED (role);
    return false;
}

QVariant KKSAttrModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant ();
    
    switch (section)
    {
        case 0: return tr("ID"); break;
        case 1: return tr("Name"); break;
        case 2: return tr("Mandatory"); break;
        case 3: return tr("Read only"); break;
        default: return QVariant(); break;
    }
}

bool KKSAttrModel::insertRows (int row, int count, const QModelIndex& parent )
{
    Q_UNUSED (row);
    Q_UNUSED (count);
    Q_UNUSED (parent);
    return false;
}

bool KKSAttrModel::removeRows (int row, int count, const QModelIndex& parent )
{
    Q_UNUSED (row);
    Q_UNUSED (count);
    Q_UNUSED (parent);
    return false;
    
}

