/* 
 * File:   KKSRubricModel.h
 * Author: yuriy rusinov
 *
 * Created on 14 �������� 2012 �., 11:44
 */

#ifndef KKSRUBRICMODEL_H
#define	KKSRUBRICMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "kksgui_config.h"

class KKSRubricBase;

class _GUI_EXPORT KKSRubricModel : public QAbstractItemModel
{
public:
    KKSRubricModel(const KKSRubricBase * rootRubr, QObject * parent=0);
    virtual ~KKSRubricModel();

    QVariant data (const QModelIndex &index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;

    QModelIndex index (int row, int column, const QModelIndex& parent=QModelIndex()) const;
    QModelIndex parent (const QModelIndex &index) const;

    int rowCount (const QModelIndex& parent=QModelIndex()) const;
    int columnCount (const QModelIndex& parent=QModelIndex()) const;

    Qt::ItemFlags flags (const QModelIndex& index) const;
    bool setData (const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

    bool insertRows (int row, int count, const QModelIndex& parent = QModelIndex() );
    bool removeRows (int row, int count, const QModelIndex& parent = QModelIndex() );

private:
    //
    // Functions
    //
    const KKSRubricBase * getRubricEntity (const QModelIndex& index) const;

private:
    //
    // Variables
    //
    const KKSRubricBase * rootRubric;

private:
    Q_OBJECT
};

#endif	/* KKSRUBRICMODEL_H */

