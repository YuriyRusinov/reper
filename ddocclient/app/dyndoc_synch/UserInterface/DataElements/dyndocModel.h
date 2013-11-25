#ifndef DYNDOC_ITEMMODEL_H
#define DYNDOC_ITEMMODEL_H

#include <QStandardItemModel>

class dyndocModel : public QStandardItemModel
{
    Q_OBJECT

    public:
        explicit dyndocModel(QObject *parent = 0);
};

#endif // DYNDOC_ITEMMODEL_H
