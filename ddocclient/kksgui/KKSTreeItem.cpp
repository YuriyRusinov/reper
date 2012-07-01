#include "KKSTreeItem.h"
#include "KKSTemplate.h"
#include "KKSEIOData.h"

KKSTreeItem :: KKSTreeItem (qint64 id, KKSEIOData * d, KKSTreeItem * parent)
    : idItem (id),
      data (d),
      parentItem (parent),
      childItems (QList<KKSTreeItem*>())
{
    if (d)
        data->addRef ();
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
         KKSTreeItem *item = new KKSTreeItem(-1-row, d, this);
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
    childItems.append(ch);
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

void KKSTreeItem :: setData (KKSEIOData * d)
{
    if (data == d)
        return;
    if (data)
        data->release ();

    data = d;

    if (data)
        data->addRef ();
}

void KKSTreeItem :: clearChildren (void)
{
    qDeleteAll (childItems);
    childItems.clear ();
//    removeChildren (0, childItems.count());
}