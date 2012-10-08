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
#include <QVector>
#include <QIcon>
#include <QVariant>

#include "kksgui_config.h"

class KKSEIOData;
class KKSTemplate;

class _GUI_EXPORT KKSTreeItem
{
public:
    KKSTreeItem (qint64 id, KKSEIOData * d, const KKSTemplate * t, const QIcon& itIcon=QIcon(), KKSTreeItem * parent=0);
    ~KKSTreeItem (void);

    KKSTreeItem *child(int number);
    int childCount() const;
    KKSEIOData * getData() const;
    QVariant columnData (int column);
    void setData (KKSEIOData * d, const KKSTemplate * t);
    bool setData (int column, const QVariant& value);
    void appendColumns (int ncols);
    bool insertChildren(int position, int count);
    KKSTreeItem *parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    void appendChild (KKSTreeItem * ch);
    void setParent (KKSTreeItem * p);
    qint64 id () const;
    void setId (qint64 newId);
    void clearChildren (void);
    QIcon getIcon (void) const;
    void setIcon (const QIcon& icon);
    
private:
    qint64 idItem;
    KKSEIOData * data;
    QIcon itemIcon;
    QVector<QVariant> itemData;
    KKSTreeItem *parentItem;
    QList<KKSTreeItem*> childItems;
};

#endif	/* KKSTREEITEM_H */

