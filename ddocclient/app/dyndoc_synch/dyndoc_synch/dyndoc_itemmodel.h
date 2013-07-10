#ifndef DYNDOC_ITEMMODEL_H
#define DYNDOC_ITEMMODEL_H

#include <QStandardItemModel>

class dyndoc_itemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit dyndoc_itemModel(QObject *parent = 0);
    
    //
    //Переопределенный метод data()
    //
    //QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
signals:
    
public slots:
    
};

#endif // DYNDOC_ITEMMODEL_H
