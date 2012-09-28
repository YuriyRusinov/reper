#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QTreeView>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QHeaderView>
#include <QGroupBox>
#include <QAbstractItemDelegate>
#include <QSize>
#include <QtDebug>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QContextMenuEvent>
#include <QMenu>
#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>

#include "KKSSortFilterProxyModel.h"
#include <KKSEIOData.h>
#include <KKSAttribute.h>
#include "KKSRecProxyModel.h"
#include "KKSTreeItem.h"
#include "KKSEIODataModel.h"
#include "KKSRecWidget.h"

KKSRecWidget :: KKSRecWidget (QTreeView *tView, bool mode, QWidget *parent, Qt::WindowFlags f)
    : QWidget (parent, f),
    tv (tView),
    tBActions (new QToolBar(this)),
    pMenu (new QMenu(this)),
    pGroupBy (new QMenu (tr("Group &By ..."), this)),
    filterLE (new QLineEdit (this)),
    gbFilter (new QGroupBox (this)),
    actFilter (new QAction (QIcon(":/ddoc/search.png"), tr("&Create search query"), this)),
    actSearchT (new QAction (QIcon(":/ddoc/search_template.png"), tr("Search by template"), this)),
    actAdd (new QAction (QIcon(":/ddoc/add.png"), tr("&Add"), this)),
    actEdit (new QAction (QIcon(":/ddoc/edit.png"), tr("&Edit"), this)),
    actDel (new QAction (QIcon(":/ddoc/delete.png"), tr("&Delete"), this)),
    actImport (new QAction (QIcon(":/ddoc/import_qualifier.png"), tr("&Import"), this)),
    actExport (new QAction (QIcon(":/ddoc/export_qualifier.png"), tr("E&xport"), this)),
    actGroupBy (new QAction (QIcon(), tr("&Group by..."), this)),
    actViewAll (new QAction (QIcon(":/ddoc/view_all.png"), tr ("View &All"), this)),
    actViewOnlyFromHere (new QAction (QIcon(":/ddoc/view_current.png"), tr ("View only from &here"), this)),
    actViewCurentRec (new QAction (QIcon(":/ddoc/view_current_rec.png"), tr ("View current &record"), this)),
    actHideRec (new QAction (QIcon(":/ddoc/hide.png"), tr ("&Hide record"), this)),
    actRefresh (new QAction (QIcon(":/ddoc/refreshIcon.png"), tr ("&Refresh"), this)),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this)),
    pbApply (new QPushButton (tr("A&pply"), this)),
    actSetView (new QAction (QIcon(":/ddoc/apply_template.png"), tr("Apply template"), this))
{
    tView->setParent (this);
    tView->setSelectionBehavior (QAbstractItemView::SelectRows);
    //Q_INIT_RESOURCE (kksgui_icon_set);
    this->init_widgets (mode);
    hideFilter ();

    connect (actAdd, SIGNAL(triggered()), this, SLOT (addRec()) );
    connect (actEdit, SIGNAL(triggered()), this, SLOT (editRec()) );
    connect (actDel, SIGNAL(triggered()), this, SLOT (delRec()) );
    connect (actRefresh, SIGNAL(triggered()), this, SLOT (refreshRec()) );
    connect (actHideRec, SIGNAL (triggered()), this, SLOT (hideRecord()) );
    connect (actViewAll, SIGNAL (triggered()), this, SLOT (viewAllRecs()) );
    connect (actViewOnlyFromHere, SIGNAL (triggered()), this, SLOT (viewRecsFromHere()) );

    connect (tView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(tvDoubleClicked(const QModelIndex &)));
    connect (filterLE, SIGNAL (textEdited ( const QString &)), this, SLOT (filterRecs (const QString&)) );
}
/*
KKSRecWidget :: KKSRecWidget (const QString& filterTitle, const QString& addTitle, const QString& editTitle, const QString& delTitle, const QString& importTitle, const QString& exportTitle, QTreeView *tView, bool mode, QWidget *parent, Qt::WindowFlags f)
    : QWidget (parent, f),
    tv (tView),
    tBActions (new QToolBar(this)),
//    tbFilter (new QToolButton (this)),//(tr("&Filter"))),
    actFilter (new QAction (QIcon(), tr("&Filter"), this)),
//    tbAdd (new QToolButton (this)),//(tr("&Add"))),
    actAdd (new QAction (QIcon(), tr("&Add"), this)),
//    tbEdit (new QToolButton (this)),//(tr("&Edit"))),
    actEdit (new QAction (QIcon(), tr("&Edit"), this)),
//    tbDel (new QToolButton (this)),//(tr("&Delete"))),
    actDel (new QAction (QIcon(), tr("&Delete"), this)),
//    tbImport (new QToolButton (this)),//(tr("&Import"))),
    actImport (new QAction (QIcon(), tr("&Import"), this)),
//    tbExport (new QToolButton (this)),//(tr("E&xport"))),
    actExport (new QAction (QIcon(), tr("E&xport"), this)),
    pbOk (new QPushButton (tr("&OK"))),
    pbCancel (new QPushButton (tr("&Cancel"))),
    pbApply (new QPushButton (tr("A&pply"))),
//    tbSetView (new QToolButton (this)),//(tr("Set View")))
    actSetView (new QAction (QIcon(), tr("Set View"), this))
{
    this->init_widgets (mode);
}
*/

KKSRecWidget :: ~KKSRecWidget (void)
{
}

QSize KKSRecWidget::sizeHint() const
{
    QWidget * parent = parentWidget();
    if(!parent)
        return QSize(500, 500);

    QString parentName = QString(parent->metaObject()->className());
    if(parentName == QString("QMdiSubWindow")){
        QMdiSubWindow * w = qobject_cast<QMdiSubWindow *>(parent);
        if(!w)
            return QSize(500, 500);
        QMdiArea *a = w->mdiArea();
        if(!a)
            return QSize(500, 500);
        return a->size();
    }

    return parent->size();
}

void KKSRecWidget :: hideFilter (void)
{
    gbFilter->setVisible (false);
}

void KKSRecWidget :: showFilter (void)
{
    gbFilter->setVisible (true);
}

qint64 KKSRecWidget :: getID (void) const
{
    if (!tv || !tv->model() || !tv->selectionModel())
        return -1;

    QItemSelectionModel *selModel = tv->selectionModel ();
    int row = selModel->currentIndex ().row();

    QModelIndex wIndex = selModel->currentIndex ().sibling(row, 0);

    if (!wIndex.isValid())
        return -1;

    return wIndex.data (Qt::UserRole).toLongLong ();
}

void KKSRecWidget :: tvDoubleClicked(const QModelIndex & index)
{
    if (!index.isValid())
        return;

    int id = index.data (Qt::UserRole).toInt ();
    Q_UNUSED (id);

    emit entityDoubleClicked();//id будем получать через getID()
}

void KKSRecWidget :: setEIOModel (QAbstractItemModel *model)
{
    QAbstractItemModel *oldModel = tv->model ();
    tv->setModel (model);
    if (oldModel && oldModel != model)
        delete oldModel;
}

void KKSRecWidget :: init_widgets (bool mode)
{
    gLay = new QGridLayout (this);
    if (!tv)
        return;
    gLay->addWidget (tBActions, 0, 0, 1, 1);
    gLay->addWidget (tv, 2, 0, 1, 1);

    QVBoxLayout * vButtonsLayout = new QVBoxLayout();
    vButtonsLayout->addStretch ();
    vButtonsLayout->addWidget (pbOk);
    vButtonsLayout->addWidget (pbCancel);
    vButtonsLayout->addWidget (pbApply);

    gLay->addLayout (vButtonsLayout, 2, 1, 1, 1);

    QSize iconSize (24, 24);
    tBActions->setIconSize (iconSize);
    QMenu * searchMenu = new QMenu (this);
    QAction * actSearchMenu = pMenu->addMenu (searchMenu);
    actSearchMenu->setText (tr("Search ..."));
    searchMenu->addAction (actFilter);
    tBActions->addAction (actFilter);
    searchMenu->addAction (actSearchT);
    tBActions->addAction (actSearchT);

    actFilterSep = tBActions->addSeparator ();
    pMenu->addSeparator ();

    tBActions->addAction (actAdd);
    pMenu->addAction (actAdd);
    tBActions->addAction (actEdit);
    pMenu->addAction (actEdit);
    tBActions->addAction (actDel);
    pMenu->addAction (actDel);
    actEditSep = tBActions->addSeparator ();
    pMenu->addSeparator ();

    tBActions->addAction (actImport);
    tBActions->addAction (actExport);
    //pMenu->addAction (actImport);
    //pMenu->addAction (actExport);
    actImportExportSep = tBActions->addSeparator ();
    //pMenu->addSeparator ();

    tBActions->addAction (actSetView);
    pMenu->addAction (actSetView);
    pMenu->addAction (actGroupBy);
    actGroupBy->setMenu (pGroupBy);
    QAction * actFilterRec = new QAction (QIcon(), tr("&Filter ..."), this);
    pMenu->addAction (actFilterRec);
    pFilter = new QMenu (this);
    actFilterRec->setMenu (pFilter);
    pFilter->setEnabled (false);
    actFilterRec->setEnabled (false);
//    QHBoxLayout * hFilterLay = new QHBoxLayout ();
//    hFilterLay->addWidget (gbFilter);
//    hFilterLay->addStretch ();
//    gLay->addLayout (hFilterLay, 1, 0, 1, 1);
    QGridLayout * gFLay = new QGridLayout (gbFilter);
    QLabel * lFilter = new QLabel (tr("Filter"), gbFilter);
    gFLay->addWidget (lFilter, 0, 0, 1, 1);
    gFLay->addWidget (filterLE, 0, 1, 1, 1);
    pFilter->addAction (actViewAll);
    pFilter->addSeparator ();
    pFilter->addAction (actViewOnlyFromHere);
    pFilter->addAction (actViewCurentRec);
    pFilter->addAction (actHideRec);
    tBActions->addAction (actRefresh);
    tBActions->addSeparator ();
    tBActions->addWidget (gbFilter);
    pMenu->addAction (actRefresh);
    pbOk->setVisible (mode);
    pbCancel->setVisible (mode);
    pbApply->setVisible (mode);
}

void KKSRecWidget :: filterRecs (const QString& text)
{
    QSortFilterProxyModel * sortMod = qobject_cast<QSortFilterProxyModel *>(getModel ());
    if (!sortMod)
        return;
    sortMod->setFilterRegExp (text);
    sortMod->setFilterKeyColumn (-1);
}

void KKSRecWidget :: hideAllButtons (void)
{
    for (int i=0; i<=3; i++)
        hideGroup (i);
}

void KKSRecWidget :: showEditGroup (void)
{
    showGroup (1);
}

void KKSRecWidget :: hideToolBar (void)
{
    tBActions->setVisible (false);
}

void KKSRecWidget :: showToolBar (void)
{
    tBActions->setVisible (true);
}

void KKSRecWidget :: resizeSections (const QList<int>& hAttrWidths)
{
    if (!tv)
        return;

    QAbstractItemModel * model = tv->model();
    if(!model)
        return;

    int nCols = qMin (model->columnCount (), hAttrWidths.count());
//    qDebug () << __PRETTY_FUNCTION__ << nCols << model->columnCount();
    for (int i=0; i<nCols; i++)
        tv->header ()->resizeSection (i, hAttrWidths[i]);
}

QAbstractItemModel *KKSRecWidget :: getModel (void) const
{
    return this->tv->model ();
}

QAbstractItemModel * KKSRecWidget :: getSourceModel (void) const
{
    QAbstractItemModel *sModel = getModel ();
    while (qobject_cast<QAbstractProxyModel *>(sModel))
        sModel = (qobject_cast<QAbstractProxyModel *>(sModel))->sourceModel ();

    return sModel;
}

QModelIndex KKSRecWidget :: getCurrentIndex (void) const
{
    return this->tv->selectionModel()->currentIndex ();;
}

QModelIndex KKSRecWidget :: getSourceIndex (void) const
{
    QModelIndex wIndex = this->getCurrentIndex();
    QAbstractItemModel *mod = this->getModel ();
    while (qobject_cast<QAbstractProxyModel *>(mod))
    {
        wIndex = (qobject_cast<QAbstractProxyModel *>(mod))->mapToSource (wIndex);
        mod = (qobject_cast<QAbstractProxyModel *>(mod))->sourceModel ();
    }

    return wIndex;
}

QModelIndexList KKSRecWidget :: getSourceIndexes (void) const
{
    QModelIndexList selIndexes = this->tv->selectionModel()->selectedIndexes ();
    QModelIndexList selSourceIndexes;
    for (int i=0; i<selIndexes.count(); i++)
    {
        QModelIndex wIndex = selIndexes[i];
        QAbstractItemModel *mod = this->getModel ();
        while (qobject_cast<QAbstractProxyModel *>(mod))
        {
            wIndex = (qobject_cast<QAbstractProxyModel *>(mod))->mapToSource (wIndex);
            mod = (qobject_cast<QAbstractProxyModel *>(mod))->sourceModel ();
        }
        selSourceIndexes.append (wIndex);
    }
    return selSourceIndexes;
}

QItemSelectionModel *KKSRecWidget :: getSelectionModel (void) const
{
    return this->tv->selectionModel ();
}

QTreeView * KKSRecWidget :: getView (void) const
{
    return this->tv;
}

void KKSRecWidget :: hideGroup (int num_gr)
{
    switch (num_gr)
    {
        case 0: {
                    actFilter->setVisible (false);
                    actFilterSep->setVisible (false);
                    QList<QAction*> acts = tBActions->actions();
                    acts.at (1)->setVisible (false);
                    break;
                }
        case 1: 
                {
                    actAdd->setVisible (false);
                    actEdit->setVisible (false);
                    actDel->setVisible (false);
                    actEditSep->setVisible (false);
                    break;
                }
        case 2: {
                    actImport->setVisible (false);
                    actExport->setVisible (false);
                    actImportExportSep->setVisible (false);
                    break;
                }
        case 3: actSetView->setVisible (false); break;
        default: break;
    }

    return;
}

void KKSRecWidget :: showGroup (int num_gr)
{
    switch (num_gr)
    {
        case 0:
                {
                    actFilter->setVisible (true);
                    actFilterSep->setVisible (false);
                    break;
                }
        case 1: 
                {
                    actAdd->setVisible (true);
                    actEdit->setVisible (true);
                    actDel->setVisible (true);
                    actEditSep->setVisible (true);
                    break;
                }
        case 2: {
                    actImport->setVisible (true);
                    actExport->setVisible (true);
                    actImportExportSep->setVisible (true);
                    break;
                }
        case 3: actSetView->setVisible (true); break;
        default: break;
    }

    return;
}

QAbstractItemDelegate * KKSRecWidget :: getItemDelegate () const
{
    return tv->itemDelegate();
}

void KKSRecWidget :: setItemDelegate (QAbstractItemDelegate * deleg)
{
    QAbstractItemDelegate * oldDeleg = tv->itemDelegate();
    tv->setItemDelegate (deleg);
    if (oldDeleg && oldDeleg != deleg)
    {
        oldDeleg->setParent (0);
        delete oldDeleg;
    }
}

void KKSRecWidget :: addToolBarAction (QAction * act)
{
    this->tBActions->addAction (act);
}

QAction * KKSRecWidget :: addToolBarSeparator (void)
{
    QAction * as = this->tBActions->addSeparator ();
    return as;
}

void KKSRecWidget :: insertToolBarAction (QAction * before, QAction * action)
{
    tBActions->insertAction (before, action);
}

void KKSRecWidget :: removeToolBarAction (QAction * action)
{
    tBActions->removeAction (action);
}

void KKSRecWidget :: addRec (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QAbstractItemModel * sourceMod = getSourceModel ();
    if (!sourceMod)
        return;
    QModelIndex pIndex = getSourceIndex ();
    emit addEntity (sourceMod, pIndex);
}

void KKSRecWidget :: editRec (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QAbstractItemModel * sourceMod = getSourceModel ();
    if (!sourceMod)
        return;
    QModelIndex wIndex = getSourceIndex ();
    emit editEntity (sourceMod, wIndex);
}

void KKSRecWidget :: delRec (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QAbstractItemModel * sourceMod = getSourceModel ();
    if (!sourceMod)
        return;
    QModelIndex wIndex = getSourceIndex ();
    emit delEntity (sourceMod, wIndex);
}

void KKSRecWidget :: refreshRec (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QAbstractItemModel * sourceMod = getSourceModel ();
    if (!sourceMod)
        return;
    emit refreshMod (sourceMod);
}

void KKSRecWidget :: setRecContextMenuPolicy (Qt::ContextMenuPolicy policy)
{
    tv->setContextMenuPolicy (policy);
}

void KKSRecWidget :: contextMenuEvent (QContextMenuEvent * event)
{
    //qDebug () << __PRETTY_FUNCTION__ << event->pos() << event->globalPos() << childAt (event->pos()) << tv;
    if (this->tBActions->isVisible() && this->childAt(event->pos()) == tv->viewport())
    {
        pMenu->exec (event->globalPos());
        event->accept();
    }
    else
        QWidget::contextMenuEvent (event);
}

void KKSRecWidget :: hideRecord (void)
{
    QItemSelection sel = tv->selectionModel()->selection();
    if (sel.indexes().isEmpty())
        return;
    qDebug () << __PRETTY_FUNCTION__;
    QModelIndex wIndex = getSourceIndex ();
    QAbstractItemModel * sModel = getSourceModel ();
    KKSEIOData * d = wIndex.data (Qt::UserRole+1).value<KKSEIOData *>();
    if (!d)
        return;
    d->setVisible (false);
    sModel->setData (wIndex, QVariant::fromValue<KKSEIOData *>(d), Qt::UserRole+1);
    KKSTreeItem * rItem = qobject_cast<KKSEIODataModel *>(sModel)->getItem (QModelIndex());
    qobject_cast<KKSEIODataModel *>(sModel)->setupData(rItem);
/* 
    setVisibleFrom (wIndex, false);
    tv->update ();
    QHeaderView * h = tv->header();
    int sortSection = h->sortIndicatorSection();
    Qt::SortOrder sOrder = h->sortIndicatorOrder();
    QSortFilterProxyModel * mod = qobject_cast<QSortFilterProxyModel *> (tv->model ());
    if (mod)
        mod->sort (sortSection, sOrder);
    QAbstractProxyModel * sortSourceModel = qobject_cast<QAbstractProxyModel *>(getModel());
    if (!sortSourceModel)
        return;
    KKSRecProxyModel * prModel = qobject_cast<KKSRecProxyModel *>(sortSourceModel->sourceModel());
    if (prModel)
        prModel->setVisibleItems (true);*/
}

void KKSRecWidget :: viewAllRecs (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    setVisibleFrom ();
    QAbstractItemModel * sModel = getSourceModel ();
    KKSTreeItem * rItem = qobject_cast<KKSEIODataModel *>(sModel)->getItem (QModelIndex());
    qobject_cast<KKSEIODataModel *>(sModel)->setupData(rItem);
    /*QAbstractProxyModel * sortSourceModel = qobject_cast<QAbstractProxyModel *>(getModel());
    if (!sortSourceModel)
        return;
    KKSRecProxyModel * prModel = qobject_cast<KKSRecProxyModel *>(sortSourceModel->sourceModel());
    if (prModel)
        prModel->setVisibleItems (false);*/
}

void KKSRecWidget :: viewRecsFromHere (void)
{
//    setVisibleFrom (QModelIndex(), false);
    QItemSelection sel = tv->selectionModel()->selection();
    if (sel.indexes().isEmpty())
        return;
    qDebug () << __PRETTY_FUNCTION__;
    QModelIndex wIndex = getSourceIndex ();
    QAbstractItemModel * sModel = getSourceModel ();
    KKSEIOData * d = wIndex.data (Qt::UserRole+1).value<KKSEIOData *>();
    if (!d)
        return;
    d->setVisible (true);
    sModel->setData (wIndex, QVariant::fromValue<KKSEIOData *>(d), Qt::UserRole+1);

    for (int i=0; i<sModel->rowCount(wIndex.parent()); i++)
    {
        bool isV (i == wIndex.row());
        QModelIndex wpInd = sModel->index (i, 0, wIndex.parent());
        setVisibleFrom (wpInd, isV);
    }
    KKSTreeItem * rItem = qobject_cast<KKSEIODataModel *>(sModel)->getItem (QModelIndex());
    qobject_cast<KKSEIODataModel *>(sModel)->setupData(rItem);
/*    QAbstractProxyModel * sortSourceModel = qobject_cast<QAbstractProxyModel *>(getModel());
    if (!sortSourceModel)
        return;
    KKSRecProxyModel * prModel = qobject_cast<KKSRecProxyModel *>(sortSourceModel->sourceModel());
    if (prModel)
        prModel->setVisibleItems (true);*/
    //setVisibleFrom (wIndex, false);
    //setVisibleFrom (wIndex, true);
}

void KKSRecWidget :: setVisibleFrom (const QModelIndex & sParent, bool v)
{
    QAbstractItemModel * sModel = getSourceModel ();
    if (!sModel)
        return;
    KKSEIOData * dp = sParent.isValid() ? sModel->data (sParent, Qt::UserRole+1).value<KKSEIOData *>() : 0;
    if (dp)
        dp->setVisible (v);
    int nr = sModel->rowCount (sParent);
    for (int i=0; i<nr; i++)
    {
        QModelIndex wIndex = sModel->index (i, 0, sParent);
        if (!wIndex.isValid ())
            continue;
        KKSEIOData * d = sModel->data(wIndex, Qt::UserRole+1).value<KKSEIOData *>();
        if (d)
        {
            d->setVisible (v);
            sModel->setData (wIndex, QVariant::fromValue<KKSEIOData *>(d), Qt::UserRole+1);
        }
        if (sModel->hasChildren (wIndex))
            setVisibleFrom (wIndex, v);
    }
}

void KKSRecWidget :: clearGroupMenu (void)
{
    pGroupBy->clear ();
}

QAction * KKSRecWidget :: addGroupAttribute (KKSAttribute * attr)
{
    if (!attr)
        return 0;
    QAction * aGroupAttr = pGroupBy->addAction (attr->title());
    return aGroupAttr;
}

void KKSRecWidget :: enableGroupMenu (bool enable)
{
    this->pGroupBy->setEnabled (enable);
}

void KKSRecWidget :: enableFilterMenu (bool enable)
{
    this->pFilter->setEnabled (enable);
}
