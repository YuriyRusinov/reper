#include <QPainter>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QRect>
#include <QPixmap>
#include <QtDebug>

#include <KKSCategory.h>
#include <KKSAttrType.h>
#include <KKSTemplate.h>
#include "KKSItemDelegate.h"

KKSItemDelegate :: KKSItemDelegate (QObject *parent)
    : QItemDelegate (parent),
    cat (0),
    templ (0),
    iBackGroundColorSet (-1),
    iTextColorSet (-1)
{
}

KKSItemDelegate :: ~KKSItemDelegate (void)
{
    if (cat)
        cat->release ();
    
    if (templ)
        templ->release ();
}

void KKSItemDelegate :: paint (QPainter * painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //qDebug () << __PRETTY_FUNCTION__ << index << option.rect << option.decorationSize;
    //QItemDelegate::paint (painter, option, index);
    if (!templ)
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
    QColor bkcol = cInd.data (Qt::BackgroundRole).value<QColor>();
    QColor fgcol = cInd.data (Qt::ForegroundRole).value<QColor>();
//    qDebug () << __PRETTY_FUNCTION__ << cInd.row () << cat->id () << bkcol << fgcol;
    if (!bkcol.isValid() && !fgcol.isValid())
    {
        QItemDelegate::paint (painter, option, index);
        return;
    }

    QVariant decVal = index.data(Qt::DecorationRole);
    QPixmap pMap;
    QRect decRect;
    switch (decVal.type())
    {
        case QVariant::Icon:
        {
            if (option.state & QStyle::State_Selected)
                pMap = decVal.value<QIcon>().pixmap(option.decorationSize, QIcon::Selected);
            else
                pMap = decVal.value<QIcon>().pixmap(option.decorationSize, QIcon::Normal);
            decRect = QRect(QPoint(0, 0), option.decorationSize);
            break;
        }
        case QVariant::Color:
        {
            pMap = QPixmap (option.decorationSize);
            pMap.fill (decVal.value<QColor>());
            decRect = QRect(QPoint(0, 0), option.decorationSize);
            break;
        }
        default: break;
    }
    painter->save ();
    qDebug () << __PRETTY_FUNCTION__ << pMap.size() << decRect << index << decVal;
    if (!pMap.isNull() && decRect.isValid())
    {
        QPoint p = QStyle::alignedRect(option.direction, option.decorationAlignment,
                                pMap.size(), option.rect).topLeft();
        painter->drawPixmap (p, pMap);
    }
    
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

}

QSize KKSItemDelegate :: sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    QSize rSize (QItemDelegate::sizeHint (option, index));
    //if (index.row() == 0)
    //    qDebug () << __PRETTY_FUNCTION__ << index << rSize;
    return rSize;
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

const KKSTemplate * KKSItemDelegate :: getTemplate (void) const
{
    return templ;
}

void KKSItemDelegate :: setTemplate (const KKSTemplate * t)
{
    if (templ)
        templ->release ();
    
    templ = t;
    
    if (templ)
        templ->addRef ();
    
    if (!cat || t->category()->id() != cat->id())
        setCategory (t->category());
}
