#include <QMimeData>
#include <QDataStream>
#include <QByteArray>
#include <QStringList>
#include <QtDebug>

#include "KKSAttributesModel.h"

KKSAttributesModel :: KKSAttributesModel (QObject *parent)
    : QStandardItemModel (parent)
{
}

KKSAttributesModel :: KKSAttributesModel (int rows, int cols, QObject *parent)
    : QStandardItemModel (rows, cols, parent)
{
}

KKSAttributesModel :: ~KKSAttributesModel (void)
{
}

Qt::ItemFlags KKSAttributesModel :: flags (const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags (index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled  | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

QMimeData * KKSAttributesModel :: mimeData (const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData ();
    QByteArray encodedData;

    QDataStream stream (&encodedData, QIODevice::WriteOnly);
    for (int i=0; i<indexes.count(); i++)
    {
        QModelIndex ind = indexes[i];
        if (ind.isValid() && ind.column()==0)
        {
            int idAttr = ind.data (Qt::UserRole).toInt ();
            QString attrName = ind.data (Qt::DisplayRole).toString ();
            stream << idAttr;
            stream << attrName;
        }
    }

    qDebug () << __PRETTY_FUNCTION__ << encodedData.length();// << qBuffer.buffer();
    mimeData->setText (encodedData);// setData("plain/text", encodedData);
    if (mimeData)
        qDebug () << __PRETTY_FUNCTION__ << mimeData << mimeData->text () <<  mimeData->data("plain/text");
    return mimeData;
}

QStringList KKSAttributesModel :: mimeTypes (void) const
{
    QStringList types;
    types << "plain/text";
    return types;
}

Qt::DropActions KKSAttributesModel :: supportedDropActions (void) const
{
    return Qt::CopyAction | Qt::MoveAction;
}
