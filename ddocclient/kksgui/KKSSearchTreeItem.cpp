/* 
 * File:   KKSSearchTreeItem.cpp
 * Author: rusinov
 * 
 * Created on 4 Февраль 2013 г., 12:56
 */

#include <KKSSearchTemplateType.h>
#include <KKSSearchTemplate.h>
#include "KKSSearchTreeItem.h"

KKSSearchTreeItem::KKSSearchTreeItem(qint64 id, KKSSearchTemplateType * sType, KKSSearchTreeItem * parent)
    : idItem (id),
      sParentItem (parent),
      sChildItems (QList<KKSSearchTreeItem *>()),
      searchType (sType),
      searchTemplate (0)
{
    if (searchType)
        searchType->addRef ();
}

KKSSearchTreeItem::KKSSearchTreeItem(qint64 id, KKSSearchTemplate * sTemplate, KKSSearchTreeItem * parent)
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
}

KKSSearchTreeItem * KKSSearchTreeItem::child (int number)
{
    
}

int KKSSearchTreeItem::childCount() const
{
    
}

const KKSSearchTemplate * KKSSearchTreeItem::getSearchTemplate() const
{
    
}

const KKSSearchTemplateType * KKSSearchTreeItem::getSearchTemplateType() const
{
}

QVariant KKSSearchTreeItem::columnData (int column)
{
    
}

void KKSSearchTreeItem::setData (const KKSSearchTemplateType * d)
{
}

void KKSSearchTreeItem::setData (const KKSSearchTemplate * d)
{

}

bool KKSSearchTreeItem::insertChildren(int position, int count)
{
    
}

const KKSSearchTemplateType *KKSSearchTreeItem::parent() const
{
    
}

bool KKSSearchTreeItem::removeChildren(int position, int count)
{
    
}

int KKSSearchTreeItem::childNumber() const
{
    
}

void KKSSearchTreeItem::appendChild (KKSSearchTreeItem * ch)
{
    
}

void KKSSearchTreeItem::setParent (KKSSearchTreeItem * p)
{
    
}

qint64 KKSSearchTreeItem::id () const
{
    
}

void KKSSearchTreeItem::setId (qint64 newId)
{
    
}

void KKSSearchTreeItem::clearChildren (void)
{
    
}