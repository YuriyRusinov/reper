#include "./UserInterface/DataElements/viewElement.h"

viewElement::viewElement()
{
}

viewElement::viewElement(const QString & text):
    QStandardItem(text)
{
}

viewElement::viewElement(const QIcon & icon,const QString & text):
    QStandardItem(icon,text)
{
}

viewElement::viewElement(int rows,int columns):
    QStandardItem(rows,columns)
{
}

viewElement::~viewElement()
{
}

void viewElement::setData(const QVariant & value,int role)
{
    QStandardItem::setData(value,role);
}

QVariant viewElement::data(int role) const
{
    return QStandardItem::data(role);
}



