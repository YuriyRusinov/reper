#include <QTreeView>
#include <QAction>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QMessageBox>
#include <QSpacerItem>
#include <QToolBar>
#include <QSize>
#include <QLabel>
#include <QtDebug>

#include "KKSItemDelegate.h"
#include "KKSAttrCheckListWidget.h"
#include "KKSAttrValue.h"
#include "KKSAttribute.h"
#include "KKSSortFilterProxyModel.h"
#include "KKSAttrValueLabel.h"

KKSAttrCheckWidget :: KKSAttrCheckWidget (const KKSAttrValue * attr, KKSIndAttrClass isSys, QWidget * parent, Qt::WFlags f)
    : QWidget (parent, f), KKSAttrWidget(attr, isSys),
    tv (new QTreeView (this)),
    tbActions (new QToolBar (this)),
    actAdd (new QAction (QIcon(":/ddoc/add.png"), tr("&Add"), this)),
    actDel (new QAction (QIcon(":/ddoc/delete.png"), tr("&Delete"), this)),
    actRefIO (new QAction (QIcon(":/ddoc/copy_to_rubric.png"), tr ("Open base qualifier"), this)),
    actRefRec (new QAction (QIcon(":/ddoc/edit.png"), tr ("Open selected record"), this)),
    viewModel (new KKSSortFilterProxyModel (this))
{
    this->setupWidget ();
    tv->setModel (viewModel);
    tv->setSelectionBehavior (QAbstractItemView::SelectRows);
    tv->setSelectionMode (QAbstractItemView::SingleSelection);
    viewModel->setDynamicSortFilter (true);

    connect (tv, SIGNAL (doubleClicked (const QModelIndex&)), this, SLOT (refEdit (const QModelIndex&)) );
    connect (actAdd, SIGNAL (triggered()), this, SLOT (addAttrRef()) );
    connect (actDel, SIGNAL (triggered()), this, SLOT (delAttrRef()) );
    connect (actRefIO, SIGNAL (triggered()), this, SLOT (refIO()) );
    connect (actRefRec, SIGNAL (triggered()), this, SLOT (refRec()) );
}

KKSAttrCheckWidget :: ~KKSAttrCheckWidget (void)
{
}

void KKSAttrCheckWidget :: setModel (QAbstractItemModel * sourceModel)
{
    QAbstractItemModel * oldModel = viewModel->sourceModel ();
    viewModel->setSourceModel (sourceModel);

    if (oldModel && oldModel != sourceModel)
    {
        disconnect (oldModel);
        delete oldModel;
    }

    connect (sourceModel, SIGNAL (dataChanged (const QModelIndex&, const QModelIndex&)), this, SLOT (updateModel (const QModelIndex&, const QModelIndex&)) );
    connect (sourceModel, SIGNAL (rowsRemoved (const QModelIndex&, int, int)), this, SLOT (removeRows (const QModelIndex &, int, int)) );
}

void KKSAttrCheckWidget :: setupWidget (void)
{
    QGridLayout * gLay = new QGridLayout (this);

    gLay->addWidget (tbActions, 0, 0, 1, 1);
    QSize iconSize (24, 24);
    tbActions->setIconSize (iconSize);
    tbActions->addAction (actAdd);
    tbActions->addAction (actDel);
    tbActions->addSeparator ();
    tbActions->addAction (actRefRec);
    tbActions->addSeparator ();
    tbActions->addAction (actRefIO);
    tbActions->addSeparator ();
    lHist = new KKSAttrValueLabel(const_cast<KKSAttrValue *>(m_av), m_isSystem, this);//QLabel ("<a href=\"View history\">View history</a>");// tr("View history")
    lHist->setOpenExternalLinks(false);
    lHist->setTextInteractionFlags (Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard);
    connect (lHist, SIGNAL (linkActivated (const QString &)), this, SLOT (viewHist (const QString&)) );
    tbActions->addWidget (lHist);
 
    gLay->addWidget (tv, 1, 0, 1, 1);
    tv->header()->setClickable (true);
    tv->header()->setSortIndicatorShown (true);
    tv->header()->setSortIndicator (0, Qt::AscendingOrder);
    tv->setSortingEnabled (true);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (this);
    tv->setItemDelegate (itemDeleg);
}

void KKSAttrCheckWidget :: addAttrRef (void)
{
    emit addAttrRef (m_av, m_isSystem, viewModel->sourceModel ());
}

QLabel * KKSAttrCheckWidget :: getHistLabel (void) const
{
    return lHist;
}

void KKSAttrCheckWidget :: delAttrRef (void)
{
    QItemSelectionModel * selModel = tv->selectionModel ();
    QItemSelection sel = selModel->selection ();
    QModelIndexList sIndexes = sel.indexes ();
    if (sIndexes.isEmpty())
    {
        qWarning() <<  tr ("Please select value");
        QMessageBox::warning (this, tr ("Delete attribute value"), tr ("Please select value"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QModelIndex wVIndex = sIndexes.at (0).sibling (sIndexes.at (0).row(), 0);
    QModelIndex wIndex = viewModel->mapToSource (wVIndex);
    qDebug () << __PRETTY_FUNCTION__ << wIndex;
    emit delAttrRef (m_av, m_isSystem, viewModel->sourceModel (), wIndex);
}

void KKSAttrCheckWidget :: refIO (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QString tableName = m_av->attribute()->tableName();
    emit refIOOpen (tableName);
}

void KKSAttrCheckWidget :: refRec (void)
{
    qDebug () << __PRETTY_FUNCTION__;

    QItemSelectionModel * selModel = tv->selectionModel ();
    QItemSelection sel = selModel->selection ();
    QModelIndexList sIndexes = sel.indexes ();
    if (sIndexes.isEmpty())
    {
        qWarning() << tr ("Please select record");
        QMessageBox::warning (this, tr ("Open selected record"), tr ("Please select record"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QModelIndex wVIndex = sIndexes.at (0).sibling (sIndexes.at (0).row(), 0);
    QModelIndex wIndex = viewModel->mapToSource (wVIndex);

    QString tableName = m_av->attribute()->tableName();
    qint64 id = wIndex.data(Qt::UserRole).toLongLong();
    emit refRecOpen (tableName, id);
}

void KKSAttrCheckWidget :: refEdit (const QModelIndex& wInd)
{
    QModelIndex wIndex = viewModel->mapToSource (wInd);
    QString tableName = m_av->attribute()->tableName();
    qint64 id = wIndex.data(Qt::UserRole).toLongLong();
    emit refRecOpen (tableName, id);
}

void KKSAttrCheckWidget :: updateModel (const QModelIndex & topLeft, const QModelIndex & bottomRight)
{
//    qDebug () << __PRETTY_FUNCTION__ << topLeft << bottomRight;
    if (!topLeft.isValid() || !bottomRight.isValid())
        return;
    const QAbstractItemModel * mod = topLeft.model ();
    QStringList sl;
    int n = mod->rowCount (topLeft.parent());
    for (int i=0; i<n; i++)
    {
        QModelIndex wIndex = mod->index (i, 0, topLeft.parent());
        sl += QString::number (mod->data (wIndex, Qt::UserRole).toLongLong());
    }
    QVariant val (sl);
    emit valueChanged (m_av->id(), m_isSystem, val);
}

void KKSAttrCheckWidget :: removeRows (const QModelIndex& parent, int start, int end)
{
    Q_UNUSED (start);
    Q_UNUSED (end);
    QAbstractItemModel * mod = qobject_cast<QAbstractItemModel *>(this->sender());
    QStringList sl;
    int n = mod->rowCount (parent);
    for (int i=0; i<n; i++)
    {
        QModelIndex wIndex = mod->index (i, 0, parent);
        sl += QString::number (mod->data (wIndex, Qt::UserRole).toLongLong());
    }
    QVariant val (sl);
    emit valueChanged (m_av->id(), m_isSystem, val);
}

void KKSAttrCheckWidget :: viewHist (const QString& link)
{
    qDebug () << __PRETTY_FUNCTION__ << link;
}
