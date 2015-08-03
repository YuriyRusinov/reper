// -*- C++ -*-
/* 
 * File:   KKSOrgModel.h
 * Author: yuriy
 *
 * Created on 2 Февраль 2012 г., 23:10
 */

#ifndef _KKSORGMODEL_H
#define	_KKSORGMODEL_H

#include <QStandardItemModel>
#include "kksgui_config.h"

class _GUI_EXPORT KKSOrgModel : public QStandardItemModel
{
public:
    KKSOrgModel (QObject *parent=0);
    KKSOrgModel (int rows, int columns, QObject * parent=0);
    ~KKSOrgModel (void);

    virtual Qt::ItemFlags flags (const QModelIndex& index) const;
    virtual QVariant data (const QModelIndex& index, int role) const;
    virtual bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
private:
    Q_OBJECT
};

#endif	/* _KKSORGMODEL_H */

