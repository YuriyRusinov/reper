/* 
 * File:   KKSAttrHistModel.h
 * Author: yuriyrusinov
 *
 * Created on 30 Август 2013 г., 16:16
 */

#ifndef KKSATTRHISTMODEL_H
#define	KKSATTRHISTMODEL_H

#include <QAbstractItemModel>

#include <KKSList.h>
#include "kksgui_config.h"

class KKSAttrValue;

class _GUI_EXPORT KKSAttrHistModel : public QAbstractItemModel
{
public:
    KKSAttrHistModel(const KKSList<KKSAttrValue*>& _histlist, QObject * parent=0);
    virtual ~KKSAttrHistModel();

    virtual int columnCount (const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

    virtual QModelIndex index (int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex& index) const;
    
    virtual Qt::ItemFlags flags (const QModelIndex& index) const;

    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    
    virtual QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    //
    // Variables
    //
    KKSList<KKSAttrValue*> histList;
private:
    Q_OBJECT

};

#endif	/* KKSATTRHISTMODEL_H */

