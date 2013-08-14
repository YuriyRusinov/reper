#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QRegExp>
#include <QMessageBox>
#include <QHeaderView>
#include <QAction>
#include <QToolBar>
#include <QLineEdit>
#include <QAbstractItemDelegate>
#include <QMessageBox>
#include <QLabel>
#include <QtDebug>

#include "KKSSearchTemplatesForm.h"
#include "KKSHIntervalW.h"
#include "defines.h"
#include "KKSCategory.h"
#include <KKSEventFilter.h>
#include <KKSItemDelegate.h>

KKSSearchTemplatesForm :: KKSSearchTemplatesForm (const KKSCategory * c1, const QString & tableName, bool mode, QWidget * parent, Qt::WFlags f)
    : QDialog (parent, f),
    idUser (-1),
    c(c1),
    m_tableName(tableName),
    searchView (new QTreeView (this)),
    tbActions (new QToolBar (this)),
    actAddNew (new QAction (tr("&Add empty template"), this)),
    actAddCopy (new QAction (tr("Add copy template"), this)),
    actEdit (new QAction (tr("&Edit template"), this)),
    actDel (new QAction (tr("&Delete template"), this)),
    actAddNewType (new QAction (tr("Add new search template type"), this)),
    actEditType (new QAction (tr("Edit search template type"), this)),
    actDelType (new QAction (tr("Delete search template type"), this)),
    lEFilter (new QLineEdit (this)),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this))
{
    if(c)
        c->addRef();

    this->init ();

    QItemSelectionModel * selModel = searchView->selectionModel ();
    QHeaderView * hv = searchView->header();

    QAbstractItemDelegate * sDeleg = new KKSItemDelegate (this);
    searchView->setItemDelegate (sDeleg);
    
    KKSEventFilter * ef = new KKSEventFilter (this);
    searchView->viewport()->installEventFilter (ef);
    hv->setClickable (true);
    hv->setSortIndicatorShown (true);
    hv->setSortIndicator (0, Qt::AscendingOrder);
    hv->setStretchLastSection (true);
    searchView->setSortingEnabled (true);
    connect (selModel, SIGNAL (selectionChanged (const QItemSelection &, const QItemSelection &)), \
             this, \
             SLOT (searchTemplatesSelectionChanged (const QItemSelection &, const QItemSelection &)) );

    connect (actAddNew, SIGNAL (triggered()), this, SLOT (addEmptySearchTemplate()) );
    connect (actAddCopy, SIGNAL (triggered()), this, SLOT (addCopySearchTemplate()) );
    connect (actEdit, SIGNAL (triggered()), this, SLOT (editSearchTemplate()) );
    connect (actDel, SIGNAL (triggered()), this, SLOT (delSearchTemplate()) );
    
    connect (actAddNewType, SIGNAL (triggered()), this, SLOT (addSearchTemplateType()) );
    connect (actEditType, SIGNAL (triggered()), this, SLOT (editSearchTemplateType()) );
    connect (actDelType, SIGNAL (triggered()), this, SLOT (delSearchTemplateType()) );

    connect (lEFilter, SIGNAL (textEdited(const QString&)), this, SLOT (setFilterSt (const QString&)) );

    pbOk->setVisible (mode);
    pbCancel->setVisible (mode);
    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSSearchTemplatesForm :: ~KKSSearchTemplatesForm (void)
{
    if(c)
        c->release();
}

int KKSSearchTemplatesForm :: getIdSearchTemplate (void) const
{
    QItemSelectionModel * selModel = searchView->selectionModel ();
    QModelIndex wIndex = selModel->currentIndex ();
    if (!wIndex.isValid () || wIndex.data(Qt::UserRole+USER_ENTITY).toInt() != 1)
        return -1;
    else
        return wIndex.data (Qt::UserRole).toInt ();
}

int KKSSearchTemplatesForm :: getIdSearchTemplateType (void) const
{
    QItemSelectionModel * selModel = searchView->selectionModel ();
    QModelIndex wIndex = selModel->currentIndex ();
    if (!wIndex.isValid ())
        return -1;
    else if  (wIndex.data(Qt::UserRole+USER_ENTITY).toInt() == 1)
        return wIndex.parent().data (Qt::UserRole).toInt ();
    else
        return wIndex.data (Qt::UserRole).toInt ();
    
}

QAbstractItemModel * KKSSearchTemplatesForm :: dataModel (void)
{
    if (searchView)
        return searchView->model ();
    else
        return 0;
}

void KKSSearchTemplatesForm :: setDataModel (QAbstractItemModel * mod)
{
    if (!searchView)
        return;
    QAbstractItemModel * oldModel = searchView->model ();
    searchView->setModel (mod);
    searchView->setColumnWidth (0, 250);
    searchView->setColumnWidth (1, 150);
    searchView->setColumnWidth (2, 110);
    searchView->setColumnWidth (3, 200);
    searchView->setColumnWidth (4, 200);

    if (oldModel && oldModel != mod)
        delete oldModel;
}

void KKSSearchTemplatesForm :: setUserId (int iduser)
{
    idUser = iduser;
}

void KKSSearchTemplatesForm :: addEmptySearchTemplate (void)
{
    QAbstractItemModel * mod = searchView->model ();
    QModelIndex wIndex = getCurrentIndex ();
    if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
        emit addNewEmptySearchTemplate (wIndex, mod, c, m_tableName);
    else
        emit addNewEmptySearchTemplate (QModelIndex(), mod, c, m_tableName);
}

void KKSSearchTemplatesForm :: addCopySearchTemplate (void)
{
    QModelIndex wIndex = getCurrentIndex ();
    if (!wIndex.isValid())
    {
        qWarning() << tr ("Select search template for copy");
        QMessageBox::warning (this, tr ("Search templates"), tr ("Select search template for copy"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QAbstractItemModel * mod = searchView->model ();
    emit addNewCopySearchTemplate (wIndex, mod);
}

void KKSSearchTemplatesForm :: editSearchTemplate (void)
{
    QModelIndex wIndex = getCurrentIndex ();
    if (!wIndex.isValid())
    {
        qWarning() << tr ("Select search template for edit");
        QMessageBox::warning (this, tr ("Search templates"), tr ("Select search template for edit"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QAbstractItemModel * mod = searchView->model ();
    emit updateSearchTemplate (wIndex, mod);
}

void KKSSearchTemplatesForm :: delSearchTemplate (void)
{
    int res = QMessageBox::question (this, tr ("Search templates"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (res != QMessageBox::Yes)
        return;
    QModelIndex wIndex = getCurrentIndex ();
    QAbstractItemModel * mod = searchView->model ();
    emit deleteSearchTemplate (wIndex, mod);
}

void KKSSearchTemplatesForm :: searchTemplatesSelectionChanged (const QItemSelection & selected, const QItemSelection& deselected)
{
    qDebug () << __PRETTY_FUNCTION__ << selected << deselected;
}

void KKSSearchTemplatesForm :: init (void)
{
    this->setWindowTitle (tr("Search templates"));
    QGridLayout * gLayout = new QGridLayout (this);
    gLayout->addWidget (tbActions, 0, 0, 1, 1);
    actAddNewType->setToolTip (tr("Add new search template type"));
    actAddNewType->setIcon (QIcon (":/ddoc/folder_add.png"));
    actEditType->setToolTip (tr("Edit search template type"));
    actEditType->setIcon (QIcon (":/ddoc/folder_edit.png"));
    actDelType->setToolTip (tr("Delete search template type"));
    actDelType->setIcon (QIcon (":/ddoc/folder_del.png"));

    actAddNew->setToolTip (tr("Add new search template"));
    actAddNew->setIcon (QIcon (":/ddoc/add.png"));
    actAddCopy->setToolTip (tr("Copy selected search template"));
    actAddCopy->setIcon (QIcon (":/ddoc/add_copy.png"));
    actEdit->setToolTip (tr ("Edit search template"));
    actEdit->setIcon (QIcon (":/ddoc/edit.png"));
    actDel->setToolTip (tr("Delete search template"));
    actDel->setIcon (QIcon (":/ddoc/delete.png"));

    tbActions->addAction (actAddNewType);
    tbActions->addAction (actEditType);
    tbActions->addAction (actDelType);
    tbActions->addSeparator ();

    tbActions->addAction (actAddNew);
    tbActions->addAction (actAddCopy);
    tbActions->addAction (actEdit);
    tbActions->addAction (actDel);
    tbActions->addSeparator ();
    QLabel * lFilter = new QLabel (tr("Filter :"));
    tbActions->addWidget (lFilter);
    tbActions->addWidget (lEFilter);
    gLayout->addWidget (searchView, 1, 0, 1, 1);
    QSizePolicy twSizePol (QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    twSizePol.setHorizontalStretch (5);
    searchView->setSizePolicy (twSizePol);

    QSizePolicy pbSizePol (QSizePolicy::MinimumExpanding, QSizePolicy::Fixed, QSizePolicy::PushButton);
    pbSizePol.setHorizontalStretch (0);

    QHBoxLayout * hButtonsLay = new QHBoxLayout ();
    hButtonsLay->addStretch (1);
    //QSpacerItem *sp = new QSpacerItem (40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    //gLayout->addItem (sp, 2, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop);

    hButtonsLay->addWidget (pbOk);//, 2, 1, 1, 1);
    hButtonsLay->addWidget (pbCancel);//, 2, 2, 1, 1);
    gLayout->addLayout (hButtonsLay, 2, 0, 1, 1);

    pbOk->setSizePolicy (pbSizePol);
    pbCancel->setSizePolicy (pbSizePol);

    QStandardItemModel *searchModel = new QStandardItemModel (0, 1);
    searchModel->setHeaderData (0, Qt::Horizontal, tr ("Search templates"));
    searchView->setModel (searchModel);
    QSize rs = this->sizeHint();
    this->resize (rs.width()*2, rs.height()*2);
}

void KKSSearchTemplatesForm :: hideActions (int aBegin, int aEnd)
{
    int ia0 = qMax (aBegin, 0);
    int ia1 = qMin (aEnd, tbActions->actions().size());
    for (int i=ia0; i<ia1; i++)
    {
        QAction * a = tbActions->actions().at(i);
        if (a)
            a->setVisible (false);
    }
}

void KKSSearchTemplatesForm :: showActions (int aBegin, int aEnd)
{
    int ia0 = qMax (aBegin, 0);
    int ia1 = qMin (aEnd, tbActions->actions().size());
    for (int i=ia0; i<ia1; i++)
    {
        QAction * a = tbActions->actions().at(i);
        if (a)
            a->setVisible (true);
    }
}

QModelIndex KKSSearchTemplatesForm :: getCurrentIndex (void) const
{
    QItemSelectionModel * selModel = searchView->selectionModel ();
    QItemSelection selected = selModel->selection ();
    if (selected.indexes().isEmpty ())
        return QModelIndex ();
    QModelIndex wIndex = selected.indexes().at(0);
    wIndex = wIndex.sibling (wIndex.row(), 0);
    return wIndex;
}

QItemSelectionModel * KKSSearchTemplatesForm :: selectionModel (void) const
{
    return searchView->selectionModel ();
}

void KKSSearchTemplatesForm :: addSearchTemplateType (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QModelIndex pIndex = getCurrentIndex ();
    QAbstractItemModel * mod = dataModel ();
    if (pIndex.data (Qt::UserRole+USER_ENTITY) == 1)
        pIndex = pIndex.parent();
    emit addSearchTemplateType (pIndex, mod);
}

void KKSSearchTemplatesForm :: editSearchTemplateType (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QModelIndex wIndex = getCurrentIndex ();
    QAbstractItemModel * mod = dataModel ();
    if (wIndex.data (Qt::UserRole+USER_ENTITY) == 1)
        return;
    emit updateSearchTemplateType (wIndex, mod);
}

void KKSSearchTemplatesForm :: delSearchTemplateType (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QModelIndex wIndex = getCurrentIndex ();
    QAbstractItemModel * mod = dataModel ();
    if (wIndex.data (Qt::UserRole+USER_ENTITY) == 1)
        return;
    if (mod->rowCount (wIndex) > 0)
    {
        qWarning() << tr ("Cannot delete type %1, it is not empty").arg (wIndex.data(Qt::DisplayRole).toString());
        QMessageBox::warning (this, tr ("Delete search template type"), tr ("Cannot delete type %1, it is not empty").arg (wIndex.data(Qt::DisplayRole).toString(), QMessageBox::Ok));
        return;
    }
    int res = QMessageBox::question (this, tr ("Delete search template type"), tr("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (res == QMessageBox::Yes)
        emit delSearchTemplateType (wIndex, mod);
}

void KKSSearchTemplatesForm :: setFilterSt (const QString& text)
{
    QSortFilterProxyModel * sortTMod = qobject_cast<QSortFilterProxyModel *>(searchView->model());
    if (!sortTMod)
        return;
    sortTMod->setFilterRegExp (text);
//    sortTMod->setFilterKeyColumn (-1);
}
