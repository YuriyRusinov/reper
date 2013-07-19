#include <QCheckBox>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include <QTreeView>
#include <QGridLayout>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QLabel>
#include <QHBoxLayout>
#include <QSize>
#include <QHeaderView>
#include <QItemSelection>
#include <QMessageBox>
#include <QtDebug>

#include "KKSSortFilterProxyModel.h"
#include "KKSEventFilter.h"
#include "KKSAttribute.h"
#include "KKSAttrValue.h"
#include "KKSSyncWidget.h"

KKSSyncWidget :: KKSSyncWidget (QWidget * parent, bool accessOk, Qt::WFlags f)
    : QWidget (parent, f),
    chGlobal (new QCheckBox(tr ("Set document to global"), this)),
    lSyncType (new QLabel (tr ("Syncronization type :"))),
    lESyncType (new QLineEdit (this)),
    tbSyncType (new QToolButton (this)),
    actSyncType (new QAction (QIcon(":/ddoc/reload.png"), tr ("Syncronize"), this)),
    gbSyncOrg (new QGroupBox (tr ("Syncronization with"), this)),
    tbActions (new QToolBar (this)),
    actAdd (new QAction (QIcon(":/ddoc/add.png"), tr("&Add"), this)),
    actDel (new QAction (QIcon(":/ddoc/delete.png"), tr("&Delete"), this)),
    tvSync (new QTreeView (this)),
    gLay (new QGridLayout (this)),
    syncAttrVal (0),
    syncAttrType (0),
    sortModel (new KKSSortFilterProxyModel())
{
    //Q_INIT_RESOURCE (kksgui_icon_set);

    this->setupWidget ();
    tbSyncType->setDefaultAction (actSyncType);//setIconSize (QSize (16, 16));
    setCheckGlobal (false);
    tvSync->setModel (sortModel);
    tvSync->header()->setClickable (true);
    tvSync->header()->setSortIndicatorShown (true);
    tvSync->header()->setStretchLastSection (true);
    tvSync->header()->setSortIndicator (2, Qt::AscendingOrder);
    //tbSyncType->setIcon (QIcon(":/ddoc/reload.png"));

    connect (chGlobal, SIGNAL (stateChanged(int)), this, SLOT (setGlobal (int)) );
    connect (tbSyncType, SIGNAL (clicked()), this, SLOT (setSyncType()) );
    connect (actAdd, SIGNAL (triggered()), this, SLOT (addSyncOrg()) );
    connect (actDel, SIGNAL (triggered()), this, SLOT (delSyncOrg()) );

    m_accessOk = accessOk;
    if(!m_accessOk)
        chGlobal->setEnabled(false);
}

KKSSyncWidget :: ~KKSSyncWidget (void)
{
    if (syncAttrVal)
        syncAttrVal->release ();
    if (syncAttrType)
        syncAttrType->release ();
}

bool KKSSyncWidget :: isCheckGlobal (void) const
{
    return chGlobal->checkState() == Qt::Checked;
}

void KKSSyncWidget :: setCheckGlobal (bool global)
{
    if (global)
        chGlobal->setCheckState (Qt::Checked);
    else
        chGlobal->setCheckState (Qt::Unchecked);

    if(m_accessOk){
        lSyncType->setEnabled (global);
        lESyncType->setEnabled (global);
        tbSyncType->setEnabled (global);
        gbSyncOrg->setEnabled (global);
    }
    else{
        lSyncType->setEnabled (false);
        lESyncType->setEnabled (false);
        tbSyncType->setEnabled (false);
        gbSyncOrg->setEnabled (false);
    }
}

void KKSSyncWidget :: setSyncOrgModel (QAbstractItemModel * sMod)
{
    QAbstractItemModel * oldModel = sortModel->sourceModel ();
    sortModel->setSourceModel (sMod);
    if (oldModel && oldModel != sMod)
        delete oldModel;
}

void KKSSyncWidget :: setSyncAttrVal (KKSAttrValue * attrVal)
{
    if (syncAttrVal)
        syncAttrVal->release ();

    syncAttrVal = attrVal;

    if (syncAttrVal)
        syncAttrVal->addRef ();
}

KKSAttrValue * KKSSyncWidget :: getSyncAttrVal (void) const
{
    return syncAttrVal;
}

KKSAttrValue * KKSSyncWidget :: getSyncAttrType (void) const
{
    return syncAttrType;
}

void KKSSyncWidget :: setSyncAttrType (KKSAttrValue * sAType)
{
    if (syncAttrType)
        syncAttrType->release ();
    
    syncAttrType = sAType;

    if (syncAttrType)
        syncAttrType->addRef ();
}

void KKSSyncWidget :: setGlobal (int state)
{
    bool isGlob (state == Qt::Checked);
    setCheckGlobal (isGlob);
    emit setIsGlobal (isGlob);
}

void KKSSyncWidget :: setSyncType (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit setSyncType (lESyncType);
}

void KKSSyncWidget :: addSyncOrg (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit addSyncOrganization (syncAttrVal, sortModel->sourceModel());
}

void KKSSyncWidget :: delSyncOrg (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    QItemSelectionModel * selModel = tvSync->selectionModel ();
    QItemSelection sel = selModel->selection ();
    QModelIndexList sIndexes = sel.indexes ();
    if (sIndexes.isEmpty())
    {
        qWarning() << tr ("Select organization for remove");
        QMessageBox::warning (this, tr ("Delete organization"), tr ("Select organization for remove"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    QModelIndex wInd = sIndexes[0].sibling (sIndexes[0].row(), 0);
    wInd = sortModel->mapToSource (wInd);
    emit delSyncOrganization (syncAttrVal, wInd, sortModel->sourceModel ());
}

void KKSSyncWidget :: setupWidget (void)
{
    gLay->addWidget (chGlobal, 0, 0, 1, 2);
    QHBoxLayout * hSTLay = new QHBoxLayout ();
    hSTLay->addWidget (lSyncType);
    hSTLay->addWidget (lESyncType);
    lESyncType->setReadOnly (true);
    hSTLay->addWidget (tbSyncType);
    gLay->addLayout (hSTLay, 1, 0, 1, 2);
    gLay->addWidget (gbSyncOrg, 2, 0, 1, 2);

    QGridLayout *gbLay = new QGridLayout (gbSyncOrg);
    gbLay->addWidget (tbActions, 0, 0, 1, 1);
    QSize iconSize (24, 24);
    tbActions->setIconSize (iconSize);
//    tbActions->addAction (actSyncType);
    tbActions->addAction (actAdd);
    tbActions->addAction (actDel);
    gbLay->addWidget (tvSync);
}

void KKSSyncWidget :: setSyncType (const QString& syncName)
{
    lESyncType->setText (syncName);
}

KKSSortFilterProxyModel * KKSSyncWidget :: getSortModel (void) const
{
    return sortModel;
}
