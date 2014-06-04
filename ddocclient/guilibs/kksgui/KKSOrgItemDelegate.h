/* 
 * File:   KKSOrgItemDelegate.h
 * Author: yuriy
 *
 * Created on 3 Февраль 2012 г., 0:21
 */

#ifndef _KKSORGITEMDELEGATE_H
#define	_KKSORGITEMDELEGATE_H

#include <QItemDelegate>
#include "kksgui_config.h"

class _GUI_EXPORT KKSOrgItemDelegate : public QItemDelegate
{
public:
    KKSOrgItemDelegate (QObject * parent=0);
    virtual ~KKSOrgItemDelegate (void);

    virtual QWidget * createEditor (QWidget * parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData (QWidget * editor, const QModelIndex& index) const;
    virtual void setModelData (QWidget * editor, QAbstractItemModel * model, const QModelIndex& index) const;
private:
    Q_OBJECT
};

#endif	/* _KKSORGITEMDELEGATE_H */

