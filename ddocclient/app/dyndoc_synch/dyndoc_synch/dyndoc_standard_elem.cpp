#ifndef DYNDOC_STANDARD_ELEM_H
#define DYNDOC_STANDARD_ELEM_H

#include <QStandardItem>

class dyndoc_standard_elem : public QStandardItem
{
    Q_OBJECT
public:
    explicit dyndoc_standard_elem(QObject *parent = 0);
private:
};

#endif // DYNDOC_ITEMMODEL_H

