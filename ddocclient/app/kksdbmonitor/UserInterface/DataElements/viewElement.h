#ifndef DYNDOC_VIEW_ELEMENT_H
#define DYNDOC_VIEW_ELEMENT_H

#include <QStandardItem>

class viewElement : public QStandardItem
{
public:
    viewElement();
    viewElement(const QString & text);
    viewElement(const QIcon & icon,const QString & text);
    viewElement(int rows, int columns = 1);
    virtual ~viewElement();

    void setData(const QVariant & value,int role = Qt::UserRole + 1);
    QVariant data(int role = Qt::UserRole + 1) const;
};

#endif

