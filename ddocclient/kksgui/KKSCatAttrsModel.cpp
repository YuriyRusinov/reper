#include <QMimeData>
#include <QtDebug>

#include "KKSCatAttrsModel.h"

KKSCatAttrsModel :: KKSCatAttrsModel (QObject *parent)
    : QStandardItemModel (parent)
{
}

KKSCatAttrsModel :: KKSCatAttrsModel (int rows, int cols, QObject *parent)
    : QStandardItemModel (rows, cols, parent)
{
}

KKSCatAttrsModel :: ~KKSCatAttrsModel (void)
{
}

Qt::ItemFlags KKSCatAttrsModel :: flags (const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

bool KKSCatAttrsModel :: dropMimeData (const QMimeData *data, Qt::DropAction action,
                                       int row, int column, const QModelIndex &parent)
{
    qDebug () << __PRETTY_FUNCTION__ << data->formats () << row << column << action << parent;
    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat("text/plain"))
        return false;

    int beginRow;

    if (row != -1)
        beginRow = row;
    else if (parent.isValid())
        beginRow = 0;
    else
        beginRow = rowCount (QModelIndex());

    QByteArray encodedData = data->data("text/plain");
    QDataStream stream (&encodedData, QIODevice::ReadOnly);
    QMap<int, QString> attrsData;
    while (!stream.atEnd())
    {
        int idAttr;
        QString attrName;
        stream >> idAttr >> attrName;
        qDebug () << __PRETTY_FUNCTION__ << idAttr << attrName;
        attrsData.insert (idAttr, attrName);
    }

    int rows = attrsData.count ();
    insertRows (beginRow, rows, parent);
    QMap<int, QString>::const_iterator p=attrsData.constBegin();
    for (int i=beginRow; i<beginRow+rows && p!=attrsData.constEnd(); i++)
    {
        QModelIndex wInd = this->index (i, 0);
        setData (wInd, p.value(), Qt::DisplayRole);
        setData (wInd, p.key(), Qt::UserRole);
        p++;
    }

    return true;
}

Qt::DropActions KKSCatAttrsModel :: supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction; 
}
