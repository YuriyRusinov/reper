#ifndef _KKSEIODataModel_H
#define _KKSEIODataModel_H

#include <QAbstractItemModel>
#include <QModelIndex>

#include "KKSMap.h"
#include "kksgui_config.h"

class KKSTemplate;
class KKSEIOData;

class _GUI_EXPORT KKSEIODataModel : public QAbstractItemModel
{
public:
    KKSEIODataModel (KKSTemplate * t, const KKSMap<qint64, KKSEIOData *>& objRecs, QObject *parent = 0);
    virtual ~KKSEIODataModel ();

    virtual int columnCount (const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

    virtual QModelIndex index (int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex& index ) const;

    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData (const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

private:
    //
    // Variables
    //
    KKSTemplate * tRef;
    KKSMap<qint64, KKSEIOData *> objRecords;

private:
    Q_OBJECT
};

#endif //_KKSEIODataModel_H
