/* 
 * File:   KKSSearchTreeItem.cpp
 * Author: rusinov
 * 
 * Created on 4 Февраль 2013 г., 12:56
 */

#include <KKSSearchTemplateType.h>
#include <KKSSearchTemplate.h>
#include "KKSSearchTreeItem.h"

KKSSearchTreeItem::KKSSearchTreeItem(qint64 id, const KKSSearchTemplateType * sType, KKSSearchTreeItem * parent)
    : idItem (id),
      sParentItem (parent),
      sChildItems (QList<KKSSearchTreeItem *>()),
      searchType (sType),
      searchTemplate (0)
{
    if (searchType)
        searchType->addRef ();
}

KKSSearchTreeItem::KKSSearchTreeItem(qint64 id, const KKSSearchTemplate * sTemplate, KKSSearchTreeItem * parent)
    : idItem (id),
      sParentItem (parent),
      sChildItems (QList<KKSSearchTreeItem *>()),
      searchType (0),
      searchTemplate (sTemplate)
{
    if (searchTemplate)
        searchTemplate->addRef ();
}

KKSSearchTreeItem::~KKSSearchTreeItem()
{
    if (searchType)
        searchType->release ();
    
    if (searchTemplate)
        searchTemplate->release ();

    qDeleteAll (sChildItems);
}

KKSSearchTreeItem * KKSSearchTreeItem::child (int number)
{
    return sChildItems.value(number);
}

int KKSSearchTreeItem::childCount() const
{
    return sChildItems.count();
}

const KKSSearchTemplate * KKSSearchTreeItem::getSearchTemplate() const
{
    return searchTemplate;
}

const KKSSearchTemplateType * KKSSearchTreeItem::getSearchTemplateType() const
{
    return searchType;
}

QVariant KKSSearchTreeItem::columnData (int column)
{
    if (searchType && column == 0)
        return searchType->name();
    else if (searchTemplate)
    {
        switch (column)
        {
            case 0: return searchTemplate->name(); break;
            case 1: return searchTemplate->authorName(); break;
            case 2: return searchTemplate->creationDatetime(); break;
        }
    }

    return QVariant ();

}

void KKSSearchTreeItem::setData (const KKSSearchTemplateType * d)
{
    if (searchType)
        searchType->release ();
    
    searchType = d;
    
    if (searchType)
        searchType->addRef ();
}

void KKSSearchTreeItem::setData (const KKSSearchTemplate * d)
{
    if (searchTemplate)
        searchTemplate->release ();
    
    searchTemplate = d;
    
    if (searchTemplate)
        searchTemplate->addRef();
}

bool KKSSearchTreeItem::insertChildren(int position, int count)
{
    if (position < 0 || position > sChildItems.size())
        return false;

    for (int row = 0; row < count; ++row)
    {
         KKSSearchTemplate * dS (0);//data;
         KKSSearchTreeItem *item = new KKSSearchTreeItem(-1-row, dS, this);
         sChildItems.insert(position, item);
     }

     return true;    
}

const KKSSearchTreeItem * KKSSearchTreeItem::parent() const
{
    return sParentItem;
}

bool KKSSearchTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > sChildItems.size())
        return false;
    
    for (int row=0; row<count; ++row)
    {
        delete sChildItems.takeAt(position);
    }
    
    return true;

}

int KKSSearchTreeItem::childNumber() const
{
    if (sParentItem)
        return sParentItem->sChildItems.indexOf(const_cast<KKSSearchTreeItem*>(this));

    return 0;
    
}

void KKSSearchTreeItem::appendChild (KKSSearchTreeItem * ch)
{
    if (!ch)
        return;
    ch->setParent (this);
    sChildItems.append(ch);    
}

void KKSSearchTreeItem::setParent (KKSSearchTreeItem * p)
{
    if (sParentItem && sParentItem != p)
        delete sParentItem;
    
    sParentItem = p;
}

qint64 KKSSearchTreeItem::id () const
{
    return idItem;
}

void KKSSearchTreeItem::setId (qint64 newId)
{
    idItem = newId;
}

void KKSSearchTreeItem::clearChildren (void)
{
    qDeleteAll (sChildItems);
    sChildItems.clear ();
    
}