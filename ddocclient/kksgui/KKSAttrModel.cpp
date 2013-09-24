/* 
 * File:   KKSAttrModel.cpp
 * Author: yuriyrusinov
 * 
 * Created on 17 Июль 2013 г., 13:07
 */
#include <QFont>
#include <KKSMap.h>
#include <KKSAttribute.h>
#include <KKSCategory.h>
#include "KKSAttrModel.h"

/*
 * Конструктор создает модель сложного атрибута для визуального отображения
 * в подходящем виджете, наследующем QAbstractItemView *
 */
KKSAttrModel::KKSAttrModel(const KKSAttribute * _attr, QObject *parent) 
    : QAbstractItemModel (parent),
    attr (_attr),
    cat (0)
{
}

KKSAttrModel::KKSAttrModel(const KKSCategory * _cat, QObject *parent)
    : QAbstractItemModel (parent),
    attr (0),
    cat (_cat)
{
    if (cat)
        cat->addRef();
}

KKSAttrModel::~KKSAttrModel()
{
    if (cat)
        cat->release();
}

int KKSAttrModel::columnCount (const QModelIndex& /*parent*/) const
{
    return 5;
}

int KKSAttrModel::rowCount (const QModelIndex& parent) const
{
    if ((!attr && !cat) || parent.isValid())
        return 0;
    return (attr ? attr->attrs().count() : cat->attributes().count());
}

QModelIndex KKSAttrModel::index (int row, int column, const QModelIndex& parent) const
{
    if ((!attr && !cat) || parent.isValid() || !hasIndex (row, column, parent))
        return QModelIndex ();

    //qDebug () << __PRETTY_FUNCTION__ << row << parent;
    if (parent.isValid())
        return QModelIndex();

    const KKSMap<int, KKSCategoryAttr *> cAttrs = attr ? attr->attrs() : cat->attributes();
    if (row >= cAttrs.count())
        return QModelIndex();
    const KKSMap<int, KKSCategoryAttr *>::const_iterator pc = cAttrs.constBegin()+row;
    if (pc != cAttrs.constEnd())
        return createIndex (row, column, pc.value());
    else
        return QModelIndex();
}

QModelIndex KKSAttrModel::parent (const QModelIndex& /*index*/) const
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
    if (!attr && !cat)
        return QVariant();
    if (role == Qt::UserRole+2)
    {
        if (attr)
            return QVariant::fromValue<const KKSAttribute *>(attr);
        else if (cat)
            return QVariant::fromValue<const KKSCategory *>(cat);
        else
            return QVariant();
    }

    if (!index.isValid())
        return QVariant();
    int irow = index.row();
    const KKSMap<int, KKSCategoryAttr *> cAttrs = attr ? attr->attrs() : cat->attributes();
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
            case 2: return cAttr->defValue().valueVariant(); break;
            case 3: return (cAttr->isMandatory() ? tr("Mandatory") : QString()); break;
            case 4: return (cAttr->isReadOnly() ? tr("Read only") : QString()); break;
            default: return QVariant(); break;
        }
    }
    else if (role == Qt::FontRole)
    {
        QFont defFont = QFont();//QAbstractItemModel::data(index, Qt::FontRole).value<QFont>();
        if (cAttr->isMandatory())
            defFont.setBold(true);
        return defFont;
    }
    
    return QVariant();
}

bool KKSAttrModel::setData (const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() && role != Qt::UserRole+2)
        return false;

    if (role == Qt::UserRole+1)
    {
        int irow = index.row();
        KKSMap<int, KKSCategoryAttr *> cAttrs = attr ? attr->attrs() : cat->attributes();
        if (irow >= cAttrs.count())
            return false;
        const KKSMap<int, KKSCategoryAttr *>::const_iterator pc = cAttrs.constBegin()+irow;
        int ikey = pc.key();
        KKSCategoryAttr * oldAttr = pc.value();
        KKSCategoryAttr * newAttr = value.value<KKSCategoryAttr *>();
        if (newAttr != oldAttr)
            oldAttr->release();
        cAttrs.insert (ikey, newAttr);
        if (attr)
            (const_cast<KKSAttribute *>(attr))->setAttrs (cAttrs);
        else
            (const_cast<KKSCategory *>(cat))->setAttributes(cAttrs);
        emit dataChanged (index.sibling(irow, 0), index.sibling(irow, 3));
        return true;
    }
    else if (role == Qt::UserRole+2 && cat)
    {
        KKSCategory * catNew = value.value<KKSCategory *>();
        if (!catNew)
            return false;
        if (cat && cat != catNew)
            cat->release();
        
        cat = catNew;
        int irow = 0;
        int nr = rowCount();
        emit dataChanged (this->index(irow, 0), this->index(nr, 4));
    }
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
        case 2: return tr("Default value"); break;
        case 3: return tr("Mandatory"); break;
        case 4: return tr("Read only"); break;
        default: return QVariant(); break;
    }
}

bool KKSAttrModel::insertRows (int row, int count, const QModelIndex& parent )
{
    if (parent.isValid())
        return false;
    bool ok (true);
    this->beginInsertRows(parent,row,row+count-1);
    KKSMap<int, KKSCategoryAttr*> cAttrs=attr ? attr->attrs() : cat->attributes();
    for (int i=0; i<count; i++)
        cAttrs.insert(-1-i, 0);
    if (attr)
        (const_cast<KKSAttribute *>(attr))->setAttrs(cAttrs);
    else
        (const_cast<KKSCategory *>(cat))->setAttributes(cAttrs);
    endInsertRows ();
    return ok;
}

bool KKSAttrModel::removeRows (int row, int count, const QModelIndex& parent )
{
    if (parent.isValid())
        return false;
    this->beginRemoveRows(parent,row,row+count-1);
    KKSMap<int, KKSCategoryAttr*> cAttrs=attr ? attr->attrs() : cat->attributes();
    KKSMap<int, KKSCategoryAttr*>::iterator p = cAttrs.begin()+row;
    for (int i=0; i<count; i++)
        cAttrs.erase(p);
    if (attr)
        (const_cast<KKSAttribute *>(attr))->setAttrs(cAttrs);
    else
        (const_cast<KKSCategory *>(cat))->setAttributes(cAttrs);
    this->endRemoveRows();
    return true;
    
}

