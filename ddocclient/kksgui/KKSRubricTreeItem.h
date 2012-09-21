/* 
 * File:   KKSRubricTreeItem.h
 * Author: yuriy rusinov
 *
 * Created on 27 Èþíü 2012 ã., 11:02
 */

#ifndef KKSRUBRICTREEITEM_H
#define	KKSRUBRICTREEITEM_H

#include <QMap>
#include <QList>
#include <QVector>
#include <QVariant>

#include "kksgui_config.h"

class KKSRubricBase;

class _GUI_EXPORT KKSRubricTreeItem
{
public:
    KKSRubricTreeItem (qint64 id=-1, const KKSRubricBase * d=0, const KKSRubricTreeItem * parent=0);
    ~KKSRubricTreeItem (void);

    KKSRubricTreeItem *child(int number);
    int childCount() const;
    const KKSRubricBase * getData() const;
    QVariant columnData (int column);
    void setData (const KKSRubricBase * d);
    bool insertChildren(int position, int count);
    const KKSRubricTreeItem *parent() const;
    bool removeChildren(int position, int count);
    int childNumber() const;
    void appendChild (KKSRubricTreeItem * ch);
    void setParent (KKSRubricTreeItem * p);
    qint64 id () const;
    void setId (qint64 newId);
    void clearChildren (void);
    
private:
    qint64 idItem;
    const KKSRubricBase * data;
    const KKSRubricTreeItem *parentItem;
    QList<KKSRubricTreeItem*> childItems;
};

#endif	/* KKSTREEITEM_H */

