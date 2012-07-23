#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QTreeView>
#include <QGridLayout>
#include <QVBoxLayout>
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

#include "KKSSortFilterProxyModel.h"
#include "KKSEIOData.h"
#include "KKSRecWidget.h"

KKSRecWidget :: KKSRecWidget (QTreeView *tView, bool mode, QWidget *parent, Qt::WindowFlags f)
    : QWidget (parent, f),
    tv (tView),
    tBActions (new QToolBar(this)),
    pMenu (new QMenu(this)),
    actFilter (new QAction (QIcon(":/ddoc/search.png"), tr("&Filter"), this)),
    actSearchT (new QAction (QIcon(":/ddoc/search_template.png"), tr("Search by template"), this)),
    actAdd (new QAction (QIcon(":/ddoc/add.png"), tr("&Add"), this)),
    actEdit (new QAction (QIcon(":/ddoc/edit.png"), tr("&Edit"), this)),
    actDel (new QAction (QIcon(":/ddoc/delete.png"), tr("&Delete"), this)),
    actImport (new QAction (QIcon(":/ddoc/import_qualifier.png"), tr("&Import"), this)),
    actExport (new QAction (QIcon(":/ddoc/export_qualifier.png"), tr("E&xport"), this)),
    actGroupBy (new QAction (QIcon(), tr("&Group by"), this)),
    actHideRec (new QAction (QIcon(":/ddoc/hide.png"), tr ("&Hide record"), this)),
    actRefresh (new QAction (QIcon(":/ddoc/refreshIcon.png"), tr ("&Refresh"), this)),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this)),
    pbApply (new QPushButton (tr("A&pply"), this)),
    actSetView (new QAction (QIcon(":/ddoc/apply_template.png"), tr("Apply template"), this))
{
    tView->setParent (this);
    //Q_INIT_RESOURCE (kksgui_icon_set);
    this->init_widgets (mode);

    connect (actAdd, SIGNAL(triggered()), this, SLOT (addRec()) );
    connect (actEdit, SIGNAL(triggered()), this, SLOT (editRec()) );
    connect (actDel, SIGNAL(triggered()), this, SLOT (delRec()) );
    connect (actRefresh, SIGNAL(triggered()), this, SLOT (refreshRec()) );
    connect (actHideRec, SIGNAL (triggered()), this, SLOT (hideRecord()) );

    connect (tView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(tvDoubleClicked(const QModelIndex &)));
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
    gLay->addWidget (tv, 1, 0, 1, 1);

    QVBoxLayout * vButtonsLayout = new QVBoxLayout();
    vButtonsLayout->addStretch ();
    vButtonsLayout->addWidget (pbOk);
    vButtonsLayout->addWidget (pbCancel);
    vButtonsLayout->addWidget (pbApply);

    gLay->addLayout (vButtonsLayout, 1, 1, 1, 1);

    QSize iconSize (24, 24);
    tBActions->setIconSize (iconSize);
    pMenu->addAction (actFilter);
    tBActions->addAction (actFilter);
    pMenu->addAction (actSearchT);
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
    pMenu->addAction (actImport);
    pMenu->addAction (actExport);
    actImportExportSep = tBActions->addSeparator ();
    pMenu->addSeparator ();

    tBActions->addAction (actSetView);
    pMenu->addAction (actSetView);
    pMenu->addAction (actGroupBy);
    pMenu->addAction (actHideRec);
    tBActions->addAction (actRefresh);
    pMenu->addAction (actRefresh);
    pbOk->setVisible (mode);
    pbCancel->setVisible (mode);
    pbApply->setVisible (mode);
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
    KKSEIOData * d = wIndex.data (Qt::UserRole+1).value<KKSEIOData *>();
    if (!d)
        return;
    d->setVisible (false);
}