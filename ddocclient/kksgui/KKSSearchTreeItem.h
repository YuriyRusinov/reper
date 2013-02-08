/* 
 * File:   KKSSearchTreeItem.h
 * Author: yuriy rusinov
 *
 * Created on 4 Февраль 2013 г., 12:56
 */

#ifndef KKSSEARCHTREEITEM_H
#define	KKSSEARCHTREEITEM_H

#include <QList>

#include "kksgui_config.h"

class KKSSearchTemplate;
class KKSSearchTemplateType;

class _GUI_EXPORT KKSSearchTreeItem
{
public:
    KKSSearchTreeItem(qint64 id, const KKSSearchTemplateType * sType=0, KKSSearchTreeItem * parent=0);
    KKSSearchTreeItem(qint64 id=-1, const KKSSearchTemplate * sTemplate=0, KKSSearchTreeItem * parent=0);
    virtual ~KKSSearchTreeItem();
    
    KKSSearchTreeItem * child (int number);
    int childCount() const;
    const KKSSearchTemplate * getSearchTemplate() const;
    const KKSSearchTemplateType * getSearchTemplateType() const;
    QVariant columnData (int column);
    void setData (const KKSSearchTemplateType * d);
    void setData (const KKSSearchTemplate * d);
    bool insertChildren(int position, int count);
    const KKSSearchTreeItem *parent() const;
    bool removeChildren(int position, int count);
    int childNumber() const;
    void appendChild (KKSSearchTreeItem * ch);
    void setParent (KKSSearchTreeItem * p);
    qint64 id () const;
    void setId (qint64 newId);
    void clearChildren (void);

private:
    qint64 idItem;
    const KKSSearchTreeItem * sParentItem;
    QList<KKSSearchTreeItem *> sChildItems;
    const KKSSearchTemplateType * searchType;
    const KKSSearchTemplate * searchTemplate;

};

#endif	/* KKSSEARCHTREEITEM_H */

