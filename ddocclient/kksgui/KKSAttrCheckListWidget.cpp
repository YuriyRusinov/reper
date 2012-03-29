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
#include <QtDebug>

#include "KKSItemDelegate.h"
#include "KKSAttrCheckListWidget.h"
#include "KKSAttrValue.h"
#include "KKSCategoryAttr.h"
#include "KKSSortFilterProxyModel.h"

KKSAttrCheckWidget :: KKSAttrCheckWidget (const KKSAttrValue * attr, bool isSys, QWidget * parent, Qt::WFlags f)
    : QWidget (parent, f), KKSAttrWidget(attr, isSys),
    tv (new QTreeView (this)),
    tbActions (new QToolBar (this)),
    actAdd (new QAction (QIcon(":/ddoc/add.png"), tr("&Add"), this)),
    actDel (new QAction (QIcon(":/ddoc/delete.png"), tr("&Delete"), this)),
    actRef (new QAction (QIcon(":/ddoc/edit.png"), tr ("to &IO"), this)),
    viewModel (new KKSSortFilterProxyModel (this))
{
    this->setupWidget ();
    tv->setModel (viewModel);
    tv->setSelectionBehavior (QAbstractItemView::SelectRows);
    tv->setSelectionMode (QAbstractItemView::SingleSelection);
    viewModel->setDynamicSortFilter (true);

    connect (actAdd, SIGNAL (triggered()), this, SLOT (addAttrRef()) );
    connect (actDel, SIGNAL (triggered()), this, SLOT (delAttrRef()) );
    connect (actRef, SIGNAL (triggered()), this, SLOT (refIO()) );
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
    tbActions->addAction (actRef);
 
    gLay->addWidget (tv, 1, 0, 1, 1);
    tv->header()->setClickable (true);
    tv->header()->setSortIndicatorShown (true);
    tv->header()->setSortIndicator (0, Qt::AscendingOrder);
    tv->setSortingEnabled (true);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (this);
    tv->setItemDelegate (itemDeleg);
/*
    QGroupBox * gbEdit = new QGroupBox (tr("Edit"), this);
    QSizePolicy sp (QSizePolicy::Preferred, QSizePolicy::Preferred);
    gbEdit->setSizePolicy (sp);
    QGridLayout *gEditLay = new QGridLayout (gbEdit);
    gEditLay->addWidget (pbAdd, 0, 0, 1, 1);
    gEditLay->addWidget (pbDel, 1, 0, 1, 1);
    gLay->addWidget (gbEdit, 0, 1, 1, 1);//, Qt::AlignJustify | Qt::AlignTop);

    QGroupBox * gbIO = new QGroupBox (tr ("Reference"), this);
    gbIO->setSizePolicy (sp);
    QGridLayout * gRefLay = new QGridLayout (gbIO);
    gRefLay->addWidget (pbRef, 0, 0, 1, 1);
    gLay->addWidget (gbIO, 1, 1, 1, 1);//, Qt::AlignLeft | Qt::AlignTop);

    QSpacerItem * verticalSpacer = new QSpacerItem(20, 94, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gLay->addItem (verticalSpacer, 2, 1, 1, 1);
*/
}

void KKSAttrCheckWidget :: addAttrRef (void)
{
    emit addAttrRef (m_av, m_isSystem, viewModel->sourceModel ());
}

void KKSAttrCheckWidget :: delAttrRef (void)
{
    QItemSelectionModel * selModel = tv->selectionModel ();
    QItemSelection sel = selModel->selection ();
    QModelIndexList sIndexes = sel.indexes ();
    if (sIndexes.isEmpty())
    {
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
        sl += QString::number (mod->data (wIndex, Qt::UserRole).toInt());
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
        sl += QString::number (mod->data (wIndex, Qt::UserRole).toInt());
    }
    QVariant val (sl);
    emit valueChanged (m_av->id(), m_isSystem, val);
}
