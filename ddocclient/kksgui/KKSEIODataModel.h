#ifndef _KKSEIODataModel_H
#define _KKSEIODataModel_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QMap>
#include <QList>

#include "KKSMap.h"
#include "KKSAttrType.h"
#include "kksgui_config.h"

class KKSTemplate;
class KKSEIOData;
class KKSCategoryAttr;
class KKSTreeItem;

class _GUI_EXPORT KKSEIODataModel : public QAbstractItemModel
{
public:
    KKSEIODataModel (const KKSTemplate * t, const KKSMap<qint64, KKSEIOData *>& objRecs, QObject *parent = 0);
    virtual ~KKSEIODataModel ();

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
    // Functions
    //
    const KKSCategoryAttr * getFirstAttribute (KKSAttrType::KKSAttrTypes aType);
    void setupData (KKSTreeItem * parent);
    KKSTreeItem * getModelItem (qint64 val, KKSTreeItem * parent, QModelIndex & pIndex);
    KKSTreeItem * getItem(const QModelIndex &index) const;
    
private:
    //
    // Variables
    //
    const KKSTemplate * tRef;
    const KKSCategoryAttr * cAttrP;
    KKSMap<qint64, KKSEIOData *> objRecords;
    //mutable QMap<qint64, QModelIndex> parIndexList;
    //mutable QMap<qint64, QList<qint64> > indList;
    KKSTreeItem * rootItem;

private:
    Q_OBJECT
};

#endif //_KKSEIODataModel_H
