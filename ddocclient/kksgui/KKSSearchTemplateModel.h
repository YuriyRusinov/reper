/* 
 * File:   KKSSearchTemplateModel.h
 * Author: rusinov
 *
 * Created on 14 январь 2013 г., 13:52
 */

#ifndef KKSSEARCHTEMPLATEMODEL_H
#define	KKSSEARCHTEMPLATEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "KKSMap.h"
#include "KKSList.h"
#include "kksgui_config.h"

class KKSSearchTemplateType;
class KKSSearchTemplate;
class KKSSearchTreeItem;

class _GUI_EXPORT KKSSearchTemplateModel : public QAbstractItemModel
{
public:
    KKSSearchTemplateModel(const KKSMap<int, KKSSearchTemplateType *>& sTypes, const KKSList<KKSSearchTemplate *>& sTemplates, QObject * parent=0);
    virtual ~KKSSearchTemplateModel();

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
    void setupData (KKSSearchTreeItem *parent);
private:
    //
    // Variables
    //
    KKSMap<int, KKSSearchTemplateType *> searchTemplateTypes;
    KKSList<KKSSearchTemplate *> searchTemplates;
    KKSSearchTreeItem * rootItem;

};

#endif	/* KKSSEARCHTEMPLATEMODEL_H */

