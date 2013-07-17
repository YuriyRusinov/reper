/* 
 * File:   KKSAttrModel.h
 * Author: yuriyrusinov
 *
 * Created on 17 Èþëü 2013 ã., 13:07
 */

#ifndef KKSATTRMODEL_H
#define	KKSATTRMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include "kksgui_config.h"

class KKSAttribute;

class _GUI_EXPORT KKSAttrModel : public QAbstractItemModel
{
public:
    KKSAttrModel(const KKSAttribute * _attr, QObject *parent = 0);
    virtual ~KKSAttrModel();

    virtual int columnCount (const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

    virtual QModelIndex index (int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex& index) const;
    
    virtual Qt::ItemFlags flags (const QModelIndex& index) const;

    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData (const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    
    virtual QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    virtual bool insertRows (int row, int count, const QModelIndex& parent = QModelIndex() );
    virtual bool removeRows (int row, int count, const QModelIndex& parent = QModelIndex() );

private:
    //
    // Variables
    //
    const KKSAttribute * attr;
   
private:
    Q_OBJECT
};

#endif	/* KKSATTRMODEL_H */

