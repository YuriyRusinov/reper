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
    return 7;
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

    if (parent.isValid())
        return QModelIndex();

    const KKSMap<int, KKSCategoryAttr *> cAttrs = attr ? attr->attrs() : cat->attributes();
    if (row >= cAttrs.count())
        return QModelIndex();
    
    const KKSMap<int, KKSCategoryAttr *>::const_iterator pc = cAttrs.constBegin()+row;
    if (pc != cAttrs.constEnd()){
        KKSCategoryAttr * ac = pc.value();
        QModelIndex cIndex = createIndex (row, column, ac);
        qint64 uid = cIndex.internalId();
        if(modelIndexesForIdList.count(uid) == 0)
            modelIndexesForIdList.insert(uid, pc.key());
        return cIndex;
    }
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

    //категория или атрибут, для которого модель хранить список прикрепленных атрибутов
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

    if(role == Qt::UserRole){
        qint64 uid = index.internalId();
        int id = modelIndexesForIdList.value(uid);
        return id;
    }

    
    int irow = index.row();
    
    QModelIndex idxForId = index.sibling(irow, 0); //id

    int id = idxForId.data(Qt::UserRole).toInt();
    if (role == Qt::UserRole)//идентификатор 
        return id;

    const KKSMap<int, KKSCategoryAttr *> cAttrs = attr ? attr->attrs() : cat->attributes();
    if (irow >= cAttrs.count())
        return QVariant();
    
    //const KKSMap<int, KKSCategoryAttr *>::const_iterator pc = cAttrs.constBegin()+irow;
    KKSCategoryAttr * cAttr = cAttrs.value(id);
    if(!cAttr)
        return QVariant();

    //if (pc==cAttrs.constEnd())
    //    return QVariant();


    //сам атрибут, ассоциированный с QModelIndex
    if (role == Qt::UserRole+1)
        return QVariant::fromValue<KKSCategoryAttr *>(cAttr); //QVariant::fromValue<KKSCategoryAttr *>(pc.value());

    //KKSCategoryAttr * cAttr (pc.value());
    if (role == Qt::DisplayRole)
    {
        int iCol = index.column();
        switch (iCol)
        {
            case 0: 
                return QString::number (id); 
                break;
            case 1: 
                return cAttr->name(); 
                break;
            case 2: 
                return cAttr->defValue().valueVariant(); 
                break;
            case 3: 
                return (cAttr->isMandatory() ? QObject::tr("Yes") : QObject::tr("No")); 
                break;
            case 4: 
                return (cAttr->isReadOnly() ? QObject::tr("Yes") : QObject::tr("No")); 
                break;
            case 5: 
                return QString::number(cAttr->order()); 
                break;
            case 6: 
                return cAttr->directives(); 
                break;
            default: 
                return QVariant(); 
                break;
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

    //идентификатор атрибута, ассоциированный с QModelIndex
    if(role == Qt::UserRole){
        qint64 uid = index.internalId();
        int id = value.toInt();
        modelIndexesForIdList.insert(uid, id);
        
        return true;
    }
    
    //сам атрибут, ассоциированный с QModelIndex
    else if (role == Qt::UserRole+1)
    {
        int irow = index.row();
        KKSMap<int, KKSCategoryAttr *> cAttrs = attr ? attr->attrs() : cat->attributes();
        if (irow >= cAttrs.count())
            return false;
        
        //const KKSMap<int, KKSCategoryAttr *>::const_iterator pc = cAttrs.constBegin()+irow;
        //int ikey = pc.key();
        //KKSCategoryAttr * oldAttr = pc.value();
    
        QModelIndex idxForId = index.sibling(irow, 0); //id
        int id = idxForId.data(Qt::UserRole).toInt();

        KKSCategoryAttr * oldAttr = cAttrs.value(id);
        if(!oldAttr)
            return false;
        
        KKSCategoryAttr * newAttr = value.value<KKSCategoryAttr *>();
        if (newAttr != oldAttr)
            oldAttr->release();

        cAttrs.insert (id, newAttr);
        if (attr)
            (const_cast<KKSAttribute *>(attr))->setAttrs (cAttrs);
        else
            (const_cast<KKSCategory *>(cat))->setAttributes(cAttrs);

        emit dataChanged (index.sibling(irow, 0), index.sibling(irow, 6));

        return true;
    }
    //категория, ассоциированная с моделью
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
        emit dataChanged (this->index(irow, 0), this->index(nr, 6));
        
        return true;
    }
    //сосотавной атрибут, ассоциированная с моделью
    else if (role == Qt::UserRole+2 && attr)
    {
        KKSAttribute * attrNew = value.value<KKSAttribute *>();
        if (!attrNew)
            return false;
        if (attr && attr != attrNew)
            attr->release ();

        attr = attrNew;
        int irow = 0;
        int nr = rowCount();
        
        emit dataChanged (this->index(irow, 0), this->index(nr, 6));
        
        return true;
    }
    
    else if (role == Qt::DisplayRole)
    {
        int irow = index.row();
        const KKSMap<int, KKSCategoryAttr *> cAttrs = attr ? attr->attrs() : cat->attributes();
        if (irow >= cAttrs.count())
            return false;

        QModelIndex idxForId = index.sibling(irow, 0); //id
        int id = idxForId.data(Qt::UserRole).toInt();

        KKSCategoryAttr * cAttr = cAttrs.value(id);
        if(!cAttr)
            return false;

        //const KKSMap<int, KKSCategoryAttr *>::const_iterator pc = cAttrs.constBegin()+irow;
        //KKSCategoryAttr * cAttr (pc.value());
        //if(!cAttr)
        //    return false;

        KKSValue defValue;
        int iCol = index.column();
        switch (iCol)
        {
            case 0: //id
            case 1: //name
            default: 
                return false;
                break;
            case 2: //def_value
                defValue.setValue(value.toString(), 9);
                cAttr->setDefValue(defValue);
                break;
            case 3:
                {
                    QString sValue = value.toString();
                    QString v = QObject::tr("No");
                    if(sValue.isEmpty() || sValue == QObject::tr("No") || sValue == QObject::tr("no") || sValue == QString("false") || sValue == QObject::tr("false"))
                        cAttr->setMandatory(false);
                    else
                        cAttr->setMandatory(true);
                }
                break;
            case 4: 
                {
                    QString sValue = value.toString();
                    if(sValue.isEmpty() || sValue == QObject::tr("No") || sValue == QObject::tr("no") || sValue == QString("false") || sValue == QObject::tr("false"))
                        cAttr->setReadOnly(false);
                    else
                        cAttr->setReadOnly(true);
                }
                break;
            case 5: 
                cAttr->setOrder(value.toInt());
                break;
            case 6: 
                cAttr->setDirectives(value.toString());
                break;
        }
    }

    return true;
}

QVariant KKSAttrModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant ();
    
    switch (section)
    {
        case 0: 
            return tr("ID"); 
            break;
        case 1: 
            return tr("Name"); 
            break;
        case 2: 
            return tr("Default value"); 
            break;
        case 3: 
            return tr("Mandatory"); 
            break;
        case 4: 
            return tr("Read only"); 
            break;
        case 5: 
            return tr("Order"); 
            break;
        case 6: 
            return tr("Directives"); 
            break;

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

