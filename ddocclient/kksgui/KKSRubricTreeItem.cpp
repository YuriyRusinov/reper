#include "KKSRubricTreeItem.h"
#include "KKSTemplate.h"
#include "KKSAttribute.h"
#include "KKSAttrView.h"
#include "KKSList.h"
#include "KKSCategoryAttr.h"
#include "KKSAttrType.h"
#include "KKSRubric.h"

KKSRubricTreeItem :: KKSRubricTreeItem (qint64 id, KKSRubricBase * d, KKSRubricTreeItem * parent)
    : idItem (id),
      data (NULL),
      parentItem (parent),
      childItems (QList<KKSRubricTreeItem*>())
{
    setData(d);
}

KKSRubricTreeItem :: ~KKSRubricTreeItem (void)
{
    qDeleteAll (childItems);
}

KKSRubricTreeItem * KKSRubricTreeItem :: child(int number)
{
    return childItems.value(number);
/*
    QMap<qint64, KKSRubricTreeItem *>::iterator p = childItems.begin();
    p += number;
    if (p != childItems.end())
        return p.value();
    else
        return 0;
 */
}

int KKSRubricTreeItem :: childCount() const
{
    return childItems.count();
}

KKSRubricBase * KKSRubricTreeItem :: getData() const
{
    return data;
}

bool KKSRubricTreeItem :: insertChildren(int position, int count)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
         KKSRubricBase * d = 0;//data;
         KKSRubricTreeItem *item = new KKSRubricTreeItem(-1-row, d, this);
         childItems.insert(position, item);
     }

     return true;
}

KKSRubricTreeItem * KKSRubricTreeItem :: parent()
{
    return parentItem;
}

bool KKSRubricTreeItem :: removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;
    
    for (int row=0; row<count; ++row)
    {
        delete childItems.takeAt(position);
    }
    
    return true;
/*
    QMap<qint64, KKSRubricTreeItem*>::iterator p = childItems.begin();
    p += position;
    for (int row = 0; row < count; ++row)
    {
        KKSRubricTreeItem * t = p.value();
        if (t)
            delete t;
        childItems.erase (p);
    }
    return true;
 */
}

int KKSRubricTreeItem :: childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<KKSRubricTreeItem*>(this));

    return 0;
/*
    if (parentItem)
    {
        int n = 0;
        QMap<qint64, KKSRubricTreeItem*>::const_iterator p = parentItem->childItems.constFind(idItem);
        if (p == parentItem->childItems.constEnd())
            return 0;
        for (QMap<qint64, KKSRubricTreeItem*>::const_iterator p0 = parentItem->childItems.constBegin();
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

void KKSRubricTreeItem :: appendChild (KKSRubricTreeItem * ch)
{
    if (!ch)
        return;
    ch->setParent (this);
    childItems.append(ch);
}

void KKSRubricTreeItem :: setParent (KKSRubricTreeItem * p)
{
    if (parentItem && parentItem != p)
        delete parentItem;
    
    parentItem = p;
}

qint64 KKSRubricTreeItem :: id () const
{
    return idItem;
}

void KKSRubricTreeItem :: setId (qint64 newId)
{
    idItem = newId;
}

void KKSRubricTreeItem :: setData (KKSRubricBase * d)
{
    if (data == d)
        return;

    if (data)
        data->release ();

    data = d;

    if (data)
        data->addRef ();
}

void KKSRubricTreeItem :: clearChildren (void)
{
    qDeleteAll (childItems);
    childItems.clear ();
//    removeChildren (0, childItems.count());
}