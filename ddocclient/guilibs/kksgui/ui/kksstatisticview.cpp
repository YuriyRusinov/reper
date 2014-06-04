#include "kksstatisticview.h"

#include <defines.h>

#include <QHeaderView>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QtDebug>
#include <QPainter>

KKSStatisticView::KKSStatisticView(KKSList<KKSStatElement*> items, 
                                   QWidget * parent)
:QTreeView(parent)
{
    
    QStandardItemModel * model = new QStandardItemModel(0, 3);
    model->setHeaderData(0, Qt::Horizontal, tr("User"), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, tr("Operation"), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, tr("Datetime"), Qt::DisplayRole);

    setModel (model);
    header()->resizeSection(0, 150);
    header()->resizeSection(1, 250);
    header()->resizeSection(2, 150);

    setSelectionBehavior(QAbstractItemView::SelectRows);


    KKSStatisticDelegate * delegate = new KKSStatisticDelegate();
    setItemDelegate(delegate);

    setRootIsDecorated(false);
/*    
    QHeaderView * hv = new QHeaderView(Qt::Horizontal);
    setHeader(hv);
    hv->resizeSection(0, 150);
    hv->resizeSection(1, 250);
    hv->resizeSection(2, 150);
    hv->setSortIndicator(1, Qt::AscendingOrder);
    hv->setSortIndicatorShown(true);
    hv->setClickable(true);
*/
    m_items = items;

    init();

    //connect(hv, SIGNAL(sectionClicked(int)), this, SLOT(sortByColumnEx(int)));
}

KKSStatisticView::~KKSStatisticView()
{
}

void KKSStatisticView::init()
{
    
    QAbstractItemModel * mdl = model();
    if(!mdl)
        return;

    for(int i=0; i<m_items.count(); i++){

        mdl->insertRow(i);
        if(i == 0){
            if(mdl->columnCount() == 0)
                mdl->insertColumns(0, 3);
        }

        mdl->setData (mdl->index(i, 0), m_items.at(i)->userName(), Qt::DisplayRole);
        mdl->setData (mdl->index(i, 1), m_items.at(i)->operTypeName(), Qt::DisplayRole);
        mdl->setData (mdl->index(i, 2), m_items.at(i)->operTime().toString("dd.MM.yyyy hh:mm"), Qt::DisplayRole);
        mdl->setData (mdl->index(i, 0), m_items.at(i)->operType(), Qt::UserRole);
    
    }
}

/*=================*/
KKSStatisticDelegate::KKSStatisticDelegate(QObject * parent) : QAbstractItemDelegate(parent)
{

}

KKSStatisticDelegate::~KKSStatisticDelegate ()
{

}

void KKSStatisticDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & ind) const
{
    painter->save();
/*
    QPixmap px;
    QRect px_rect;*/
    QRect bound = option.rect;
    QPen pen;

    int shift = 0;
    QModelIndex index = ind.sibling(ind.row(), 0);
    
    if(index.data(Qt::UserRole).toInt() == KKSStatElement::soInsertIO)
        pen = QPen(Qt::darkBlue);
    if(index.data(Qt::UserRole).toInt() == KKSStatElement::soUpdateIO)
        pen = QPen(Qt::darkGreen);
    if(index.data(Qt::UserRole).toInt() == KKSStatElement::soInsertAttr)
        pen = QPen(Qt::darkRed);
    if(index.data(Qt::UserRole).toInt() == KKSStatElement::soRemoveAttr)
        pen = QPen(Qt::darkYellow);
    if(index.data(Qt::UserRole).toInt() == KKSStatElement::soUpdateAttrList)
        pen = QPen(Qt::red);

    bound = QRect(option.rect.left()+5+shift, option.rect.top(), option.rect.width()-5-shift, option.rect.height());

    painter->setPen(pen);

    if (option.state & QStyle::State_Selected){
        painter->setBrush(option.palette.highlight());
        painter->drawRect(option.rect);
        painter->setPen(option.palette.highlightedText().color());

    }
    //else{
    //    painter->setBrush(Qt::NoBrush);
    //    painter->save();
    //    painter->setPen(Qt::NoPen);
    //    painter->drawRect(option.rect);
    //    painter->restore();
    //}

    QString text = ind.data(Qt::DisplayRole).toString();
    int flags = Qt::AlignVCenter;
    painter->drawText(bound, flags, text);

    painter->restore();
}

QSize KKSStatisticDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    return QSize(30, 19);
}
