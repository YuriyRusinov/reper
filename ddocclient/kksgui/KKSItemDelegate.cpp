#include <QPainter>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QtDebug>

#include <KKSCategory.h>
#include <KKSAttrType.h>
#include "KKSItemDelegate.h"

KKSItemDelegate :: KKSItemDelegate (QObject *parent)
    : QItemDelegate (parent),
    cat (0),
    iBackGroundColorSet (-1),
    iTextColorSet (-1)
{
}

KKSItemDelegate :: ~KKSItemDelegate (void)
{
    if (cat)
        cat->release ();
}

void KKSItemDelegate :: paint (QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QItemDelegate::paint (painter, option, index);
/*    if (!cat)
    {
        QItemDelegate::paint (painter, option, index);
        return;
    }
    QList<int> bkCols = cat->searchAttributesByType (KKSAttrType::atRecordColor);
    if (bkCols.isEmpty())
        bkCols += cat->searchAttributesByType (KKSAttrType::atRecordColorRef);
    QList<int> fgCols = cat->searchAttributesByType (KKSAttrType::atRecordTextColor);
    if (fgCols.isEmpty())
        fgCols += cat->searchAttributesByType (KKSAttrType::atRecordTextColorRef);
    if (bkCols.isEmpty() && fgCols.isEmpty())
    {
        QItemDelegate::paint (painter, option, index);
        return;
    }
    
    QModelIndex cInd = index.sibling (index.row(), 0);
    QColor bkcol = cInd.data (Qt::UserRole+1).value<QColor>();
    QColor fgcol = cInd.data (Qt::UserRole+2).value<QColor>();
//    qDebug () << __PRETTY_FUNCTION__ << cInd.row () << cat->id () << bkcol << fgcol;
    if (!bkcol.isValid() && !fgcol.isValid())
    {
        qDebug () << __PRETTY_FUNCTION__ << index.data (Qt::BackgroundRole);
        QItemDelegate::paint (painter, option, index);
        return;
    }

    painter->save ();
    if (!bkcol.isValid())
        bkcol = QColor(Qt::white);//option.palette.color (QPalette::Normal, QPalette::Window);
    QBrush fillBrush (bkcol);
    if (option.state & QStyle::State_Selected)
        painter->fillRect (option.rect, option.palette.highlight());
    else
    {
        painter->setBrush (fillBrush);
        painter->fillRect (option.rect, fillBrush);
    }

    if (fgCols.isEmpty() )//|| option.state & QStyle::State_Selected)
    {
        painter->drawText (option.rect, index.data (Qt::DisplayRole).toString());
        painter->restore ();
        return;
    }
    if (!fgcol.isValid())
        fgcol = option.palette.color (QPalette::Normal, QPalette::WindowText);

    QPen pen (fgcol);
    painter->setPen (pen);
    painter->drawText (option.rect, index.data (Qt::DisplayRole).toString());
    painter->restore ();
*/
}

QSize KKSItemDelegate :: sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    return QItemDelegate::sizeHint (option, index);
}

QWidget * KKSItemDelegate :: createEditor (QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED (parent);
    Q_UNUSED (option);
    Q_UNUSED (index);
    return 0;
}

const KKSCategory * KKSItemDelegate :: getCategory (void) const
{
    return cat;
}

void KKSItemDelegate :: setCategory (const KKSCategory * c)
{
    if (cat)
        cat->release ();

    cat = c;

    if (cat)
        cat->addRef ();
}
