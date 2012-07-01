/* 
 * File:   KKSTreeItem.h
 * Author: yuriy rusinov
 *
 * Created on 27 Èþíü 2012 ã., 11:02
 */

#ifndef KKSTREEITEM_H
#define	KKSTREEITEM_H

#include <QMap>
#include <QList>

#include "kksgui_config.h"

class KKSEIOData;
class KKSTemplate;

class _GUI_EXPORT KKSTreeItem
{
public:
    KKSTreeItem (qint64 id, KKSEIOData * d, KKSTreeItem * parent=0);
    ~KKSTreeItem (void);

    KKSTreeItem *child(int number);
    int childCount() const;
    KKSEIOData * getData() const;
    void setData (KKSEIOData * d);
    bool insertChildren(int position, int count);
    KKSTreeItem *parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    void appendChild (KKSTreeItem * ch);
    void setParent (KKSTreeItem * p);
    qint64 id () const;
    void setId (qint64 newId);
    void clearChildren (void);
    
private:
    qint64 idItem;
    KKSEIOData * data;
    KKSTreeItem *parentItem;
    QList<KKSTreeItem*> childItems;
};

#endif	/* KKSTREEITEM_H */

