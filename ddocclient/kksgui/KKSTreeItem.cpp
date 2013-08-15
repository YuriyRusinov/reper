#include "KKSTreeItem.h"
#include "KKSTemplate.h"
#include "KKSAttribute.h"
#include "KKSAttrView.h"
#include "KKSList.h"
#include "KKSAttribute.h"
#include "KKSAttrType.h"
#include "KKSEIOData.h"

KKSTreeItem :: KKSTreeItem (qint64 id, KKSEIOData * d, const KKSTemplate * t, const QIcon& itIcon, KKSTreeItem * parent)
    : idItem (id),
      templ (t),
      data (NULL),
      itemIcon (itIcon),
      parentItem (parent),
      childItems (QList<KKSTreeItem*>())
{
    setData(d, t);
}

KKSTreeItem :: ~KKSTreeItem (void)
{
    qDeleteAll (childItems);
    if (data)
        data->release ();
}

KKSTreeItem * KKSTreeItem :: child(int number)
{
    return childItems.value(number);
/*
    QMap<qint64, KKSTreeItem *>::iterator p = childItems.begin();
    p += number;
    if (p != childItems.end())
        return p.value();
    else
        return 0;
 */
}

int KKSTreeItem :: childCount() const
{
    return childItems.count();
}

KKSEIOData * KKSTreeItem :: getData() const
{
    return data;
}

bool KKSTreeItem :: insertChildren(int position, int count)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
         KKSEIOData * d = 0;//data;
         const KKSTemplate * t (0);
         QIcon ic;
         KKSTreeItem *item = new KKSTreeItem(-1-row, d, t, ic, this);
         childItems.insert(position, item);
     }

     return true;
}

KKSTreeItem * KKSTreeItem :: parent()
{
    return parentItem;
}

bool KKSTreeItem :: removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;
    
    for (int row=0; row<count; ++row)
    {
        delete childItems.takeAt(position);
    }
    
    return true;
/*
    QMap<qint64, KKSTreeItem*>::iterator p = childItems.begin();
    p += position;
    for (int row = 0; row < count; ++row)
    {
        KKSTreeItem * t = p.value();
        if (t)
            delete t;
        childItems.erase (p);
    }
    return true;
 */
}

int KKSTreeItem :: childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<KKSTreeItem*>(this));

    return 0;
/*
    if (parentItem)
    {
        int n = 0;
        QMap<qint64, KKSTreeItem*>::const_iterator p = parentItem->childItems.constFind(idItem);
        if (p == parentItem->childItems.constEnd())
            return 0;
        for (QMap<qint64, KKSTreeItem*>::const_iterator p0 = parentItem->childItems.constBegin();
                p0 != p;
                p0++)
        {
            n++;
        }
        return n;
    }

    return 0;
 */
}

void KKSTreeItem :: appendChild (KKSTreeItem * ch)
{
    if (!ch)
        return;
    ch->setParent (this);
    KKSEIOData * d = ch->data;
    childItems.append(ch);
    ch->initData (d, templ);
}

void KKSTreeItem :: setParent (KKSTreeItem * p)
{
    if (parentItem && parentItem != p)
        delete parentItem;
    
    parentItem = p;
}

qint64 KKSTreeItem :: id () const
{
    return idItem;
}

void KKSTreeItem :: setId (qint64 newId)
{
    idItem = newId;
}

void KKSTreeItem :: setData (KKSEIOData * d, const KKSTemplate * t)
{
    if (!t || data == d)
        return;

    if (data)
        data->release ();

    data = d;

    if (data)
        data->addRef ();
    
    if (!data)
        return;

    initData (d, t);
}

void KKSTreeItem :: initData (KKSEIOData * d, const KKSTemplate * t)
{
    if (!d || !t)
        return;
    itemData.clear ();
    int iNum = this->childNumber()+1;
    KKSList<KKSAttrView *> sAttrs = t->sortedAttrs();
    for (int i=0; i<sAttrs.size(); i++)
    {
        KKSAttrView * v = sAttrs[i];
        QString attrValue;
        if( v->type()->attrType() == KKSAttrType::atJPG || 
            (v->refType() && v->refType()->attrType() == KKSAttrType::atJPG)
            )
        {
            attrValue = QObject::tr("<Image data %1>").arg (iNum);
        }
        else if( v->type()->attrType() == KKSAttrType::atSVG || 
                (v->refType() && v->refType()->attrType() == KKSAttrType::atSVG)
                )
        {
            attrValue = QObject::tr("<SVG data %1>").arg (iNum);
        }
        else if( v->type()->attrType() == KKSAttrType::atXMLDoc || 
                (v->refType() && v->refType()->attrType() == KKSAttrType::atXMLDoc)
                )
        {
            attrValue = QObject::tr("<XML document %1>").arg (iNum);
        }
        else if( v->type()->attrType() == KKSAttrType::atVideo || 
                (v->refType() && v->refType()->attrType() == KKSAttrType::atVideo)
                )
        {
            attrValue = QObject::tr("<Video data %1>").arg (iNum);
        }
        else if (v->type()->attrType() == KKSAttrType::atComplex ||
                 (v->refType() && v->refType()->attrType() == KKSAttrType::atComplex)
                )
        {
            attrValue = QObject::tr("<Complex value %1>").arg (iNum);
        }
        else
        {
            QString aCode = v->code(false);
            attrValue = d->fieldValue (aCode).isEmpty() ? d->sysFieldValue (aCode) : d->fieldValue (aCode);
            if (attrValue.isEmpty())
            {
                attrValue = d->fields().value (aCode.toLower());
                if (attrValue.isEmpty())
                    attrValue = d->sysFields().value (aCode.toLower());
            }
            else if (attrValue.contains ("\n"))
                attrValue = attrValue.mid (0, attrValue.indexOf("\n")) + "...";
        }
        
        itemData.append(QVariant(attrValue));
    }
}

void KKSTreeItem :: clearChildren (void)
{
    qDeleteAll (childItems);
    childItems.clear ();
//    removeChildren (0, childItems.count());
}

QVariant KKSTreeItem :: columnData (int column)
{
    if (column >=0 && column < itemData.count())
        return itemData[column];
    return QVariant ();
}

bool KKSTreeItem :: setData (int column, const QVariant& value)
{
    if (column < 0 || column >= itemData.size())
        return false;

     itemData[column] = value;
     return true;
}

void KKSTreeItem :: appendColumns (int ncols)
{
    for (int i=0; i<ncols; i++)
        itemData.append(QVariant());
}

QIcon KKSTreeItem :: getIcon (void) const
{
    return itemIcon;
}

void KKSTreeItem :: setIcon (const QIcon& icon)
{
    itemIcon = icon;
}
