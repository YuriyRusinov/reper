#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QList>
#include <QPointF>
#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QPoint>
#include <QAction>
#include <QtDebug>

#include "KKSAttrValue.h"
#include "KKSPointsItemDelegate.h"
#include "KKSPointTable.h"

KKSPointTable :: KKSPointTable (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, QWidget *parent)
    : QTableView (parent), KKSAttrWidget(attr, isSys),
    currVal (QVariant()),
    popupMenu (new QMenu())
{

    initModel ();
}

KKSPointTable :: KKSPointTable (const KKSAttrValue *attr, KKSIndAttr::KKSIndAttrClass isSys, const QList<QVariant>& v, QWidget *parent)
    : QTableView (parent), KKSAttrWidget(attr, isSys),
    currVal (v),
    popupMenu (new QMenu())
{

    initModel (v);
}

KKSPointTable :: ~KKSPointTable (void)
{
}

void KKSPointTable :: setData (const QList<QVariant>& pvList)
{
    currVal = QVariant (pvList);
    QAbstractItemModel * pointsModel = this->model ();
    int nr = pointsModel->rowCount();
    pointsModel->removeRows (0, nr);

    int np = pvList.count ();
    pointsModel->insertRows (0, np);
    for (int i=0; i<np; i++)
    {
        QPointF p = pvList[i].toPointF ();
        QModelIndex wIndex = pointsModel->index (i, 0);
        pointsModel->setData (wIndex, QString::number (p.x()), Qt::DisplayRole);

        wIndex = wIndex.sibling (i, 1);
        pointsModel->setData (wIndex, QString::number (p.y()), Qt::DisplayRole);
    }
}

void KKSPointTable :: clearData (void)
{
    currVal = QVariant ();
    QAbstractItemModel * pointsModel = this->model ();
    int nr = pointsModel->rowCount();
    pointsModel->removeRows (0, nr);
}

void KKSPointTable :: addPoint (const QModelIndex& parent, int start, int end)
{
    if (parent.isValid())
        return;

    QList<QVariant> pvList = currVal.toList ();
    for (int i=start; i<=end; i++)
    {
        QPointF p;
        if (start >= pvList.count())
            pvList.append (QVariant (p));
        else
            pvList.insert (start, QVariant (p));
    }
    currVal = QVariant (pvList);
    emit valueChanged (m_av->id(), m_isSystem, currVal);
}

void KKSPointTable :: delPoint (const QModelIndex& parent, int start, int end)
{
    if (parent.isValid())
        return;

    QList<QVariant> pvList = currVal.toList ();
    for (int i=start; i<=end; i++)
        pvList.removeAt (start);

    currVal = QVariant (pvList);
    emit valueChanged (m_av->id(), m_isSystem, currVal);
}

void KKSPointTable :: pointChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight)
{
    int startRow = topLeft.row ();
    int endRow = bottomRight.row ();

    QList<QVariant> pvList = currVal.toList ();
    QAbstractItemModel * pModel = model ();
    for (int i=startRow; i<=endRow; i++)
    {
        QModelIndex wIndex = pModel->index (i, 0);
        double x = wIndex.data (Qt::DisplayRole).toDouble ();

        wIndex = wIndex.sibling (i, 1);
        double y = wIndex.data (Qt::DisplayRole).toDouble ();
        QPointF p (x, y);
        pvList[i] = QVariant (p);
    }

    currVal = QVariant (pvList);
    qDebug () << __PRETTY_FUNCTION__ << currVal.toList().count();
    emit valueChanged (m_av->id(), m_isSystem, currVal);
}

void KKSPointTable :: initModel (const QList<QVariant>& v)
{
    QAbstractItemModel * pointsModel = new QStandardItemModel (0, 2);
    QStringList headers;
    headers << tr ("X") << tr ("Y");
    for (int i=0; i<2; i++)
        pointsModel->setHeaderData (i, Qt::Horizontal, headers[i], Qt::DisplayRole);
    setModel (pointsModel);

    KKSPointsItemDelegate * iDeleg = new KKSPointsItemDelegate ();
    setItemDelegate (iDeleg);
    QHeaderView * hHeader = this->horizontalHeader();
    hHeader->setStretchLastSection (true);

//    this->setContextMenuPolicy (Qt::CustomContextMenu);
//    this->setMouseTracking (true);
    if (!v.isEmpty())
        setData (v);

    connect (pointsModel, \
             SIGNAL (rowsInserted (const QModelIndex&, int, int)), \
             this, \
             SLOT (addPoint (const QModelIndex&, int, int)) \
            );

    connect (pointsModel, \
             SIGNAL (rowsRemoved (const QModelIndex&, int, int)), \
             this, \
             SLOT (delPoint (const QModelIndex&, int, int)) \
            );

    connect (pointsModel, \
             SIGNAL (dataChanged (const QModelIndex&, const QModelIndex&)), \
             this, \
             SLOT (pointChanged (const QModelIndex&, const QModelIndex&)) \
            );
}

void KKSPointTable :: contextMenuEvent (QContextMenuEvent * event)
{
    if (!event || !model ())
        return;

    qDebug () << __PRETTY_FUNCTION__ << event->pos();
    QModelIndex pIndex = indexAt (event->pos());
    //QAbstractItemModel * pModel = model ();
    popupMenu->clear ();
    QList<QAction *> actList = this->actions();
    for (int i=0; i<actList.count(); i++)
        this->removeAction (actList[i]);

    if (!pIndex.isValid())
    {
        QAction * pAddAct = popupMenu->addAction (tr ("Append point"), this, SLOT (appNewPoint(void)) );
        this->addAction (pAddAct);
    }
    else
    {
        QAction * pInsBeforeAct = popupMenu->addAction (tr ("Insert point before current"), this, SLOT (insertBefore(void)) );
        QAction * pInsAfterAct = popupMenu->addAction (tr ("Insert point after current"), this, SLOT (insertAfter(void)) );
        QAction * pDelAct = popupMenu->addAction (tr ("Delete current point"), this, SLOT (delCurrPoint(void)) );
        this->addAction (pInsBeforeAct);
        this->addAction (pInsAfterAct);
        this->addAction (pDelAct);
    }

    this->popupMenu->exec (event->globalPos());
}

void KKSPointTable :: appNewPoint (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QAbstractItemModel * pModel = model ();
    int nr = pModel->rowCount ();
    pModel->insertRows (nr, 1);
}

void KKSPointTable :: insertBefore (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QItemSelectionModel * selModel = this->selectionModel ();
    QModelIndex wIndex = selModel->currentIndex ();
    QAbstractItemModel * pModel = model ();
    int i = (wIndex.row() == 0 ? 0 : wIndex.row()-1);

    pModel->insertRows (i, 1);
}

void KKSPointTable :: insertAfter (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QItemSelectionModel * selModel = this->selectionModel ();
    QModelIndex wIndex = selModel->currentIndex ();
    QAbstractItemModel * pModel = model ();
    int i = wIndex.row()+1;

    pModel->insertRows (i, 1);
}

void KKSPointTable :: delCurrPoint (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QItemSelectionModel * selModel = this->selectionModel ();
    QModelIndex wIndex = selModel->currentIndex ();
    QAbstractItemModel * pModel = model ();

    pModel->removeRows (wIndex.row(), 1);
}
