#include <QPushButton>
#include <QGridLayout>
#include <QTabWidget>
#include <QTreeView>
#include <QTableView>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QSize>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include <QItemSelection>
#include <QGroupBox>
#include <QMessageBox>
#include <QtDebug>

#include <KKSTemplate.h>
#include <KKSCategory.h>
#include <defines.h>
#include "KKSTemplateEditor.h"
#include "KKSEventFilter.h"

KKSTemplateEditor :: KKSTemplateEditor (KKSTemplate *t, bool mode, QWidget *parent, Qt::WindowFlags f)
    : KKSDialog (parent, f),
    tRef (t),
    lEID (new QLineEdit (QString::number (t ? t->id() : 0))),
    lETitle (new QLineEdit (t->name())),
    lECat (new QLineEdit ((tRef ? tRef->category()->name() : QString()))),
    tvGroups (new QTreeView ()),
    pbAddGroup (new QPushButton (tr("Add"))),
    pbEditGroup (new QPushButton (tr("Edit"))),
    pbDelGroup (new QPushButton (tr("Delete"))),
    pbAddAttr (new QPushButton (tr("&Paste"))),
    pbEditAttr (new QPushButton (tr("Edit"))),
    pbDelAttr (new QPushButton (tr("Cu&t"))),
    pbUp (new QPushButton (tr("Up"))),
    pbDown (new QPushButton (tr("Down"))),
    tableAttrs (new QTableView()),
    pbOk (new QPushButton (tr("&OK"))),
    pbCancel (new QPushButton (tr("&Cancel"))),
    pbApply (new QPushButton (tr("A&pply"))),
    dbErr (false)
{
    if (t)
        tRef->addRef ();

    init_widgets (mode);
    tvGroups->setSelectionBehavior (QAbstractItemView::SelectRows);

    connect (pbAddGroup, SIGNAL (clicked()), this, SLOT (addTemplateGroup()) );
    connect (pbEditGroup, SIGNAL (clicked()), this, SLOT (editTemplateGroup()) );
    connect (pbDelGroup, SIGNAL (clicked()), this, SLOT (delTemplateGroup()) );

    connect (pbAddAttr, SIGNAL (clicked()), this, SLOT (addAttribute()) );
    connect (pbEditAttr, SIGNAL (clicked()), this, SLOT (editAttribute()) );
    connect (pbDelAttr, SIGNAL (clicked()), this, SLOT (delAttribute()) );

    pbUp->setEnabled (false);
    pbDown->setEnabled (false);
//    connect (pbUp, SIGNAL (clicked()), this, SLOT (moveUp()) );
//    connect (pbDown, SIGNAL (clicked()), this, SLOT (moveDown()) );

    connect (pbOk, SIGNAL (clicked()), this, SLOT (saveClose()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (pbApply, SIGNAL (clicked()), this, SLOT (save()) );
}

KKSTemplateEditor :: ~KKSTemplateEditor ()
{
    if (tRef)
        tRef->release ();
}

int KKSTemplateEditor :: getTemplateID (void) const
{
    if (tRef)
        return tRef->id ();
    return -1;
}

int KKSTemplateEditor :: getCategoryID (void) const
{
    if (tRef && tRef->category())
        return tRef->category()->id();
    return -1;
}

void KKSTemplateEditor :: save (void)
{
    if (lETitle->text().isEmpty())
        return;
    tRef->setName (lETitle->text());
    emit saveTemplate (tRef, this);
}

void KKSTemplateEditor :: saveClose (void)
{
    save ();
    if (!dbErr)
        accept ();
}

void KKSTemplateEditor :: init_widgets (bool mode)
{
    QGridLayout *gLayout = new QGridLayout ();
    this->setLayout (gLayout);
    QTabWidget * tabWidget = new QTabWidget ();
    gLayout->addWidget (tabWidget, 0, 0, 1, 1);

    this->init_par_widget (tabWidget);
    this->init_group_widget (tabWidget);
//    this->init_attr_widget (tabWidget);

    QHBoxLayout *hButtonsLay = new QHBoxLayout ();
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
    hButtonsLay->addWidget (pbApply);
    gLayout->addLayout (hButtonsLay, 1, 0, 1, 1);

    pbOk->setVisible (mode);
    pbCancel->setVisible (mode);
    pbApply->setVisible (mode);
}

void KKSTemplateEditor :: init_par_widget (QTabWidget * tWidget)
{
    QWidget *parWidget = new QWidget ();
    tWidget->addTab (parWidget, tr("Parameters"));
    QGridLayout *gPLay = new QGridLayout ();
    parWidget->setLayout (gPLay);

    QLabel *lID = new QLabel (QString("ID :"));
    gPLay->addWidget (lID, 0, 0, 1, 1, Qt::AlignRight);
    lEID->setReadOnly (true);
    gPLay->addWidget (lEID, 0, 1, 1, 1);

    QLabel *lTitle = new QLabel (tr("Title :"));
    gPLay->addWidget (lTitle, 1, 0, 1, 1, Qt::AlignRight);
    gPLay->addWidget (lETitle, 1, 1, 1, 1);

    QLabel *lCat = new QLabel (tr("Category :"));
    gPLay->addWidget (lCat, 2, 0, 1, 1, Qt::AlignRight);
    lECat->setReadOnly (true);
    gPLay->addWidget (lECat, 2, 1, 1, 1);
    lECat->adjustSize ();

    QSpacerItem *sPar = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gPLay->addItem (sPar, 3, 1, 1, 1);
}

void KKSTemplateEditor :: init_group_widget (QTabWidget *tWidget)
{
    QWidget *groupWidget = new QWidget ();
    tWidget->addTab (groupWidget, tr("Groups"));
    QGridLayout *gGLay = new QGridLayout ();
    groupWidget->setLayout (gGLay);
    KKSEventFilter *ef = new KKSEventFilter (groupWidget);
    tvGroups->viewport()->installEventFilter (ef);

    gGLay->addWidget (tvGroups, 0, 0, 5, 1);

    QGroupBox *gbAttrGroup = new QGroupBox (tr("Group of attributes"));
    gGLay->addWidget (gbAttrGroup, 0, 1, 1, 1);
    QGridLayout *gAttrGLay = new QGridLayout ();
    gbAttrGroup->setLayout (gAttrGLay);
    gAttrGLay->addWidget (pbAddGroup, 0, 0, 1, 1);
    gAttrGLay->addWidget (pbEditGroup, 1, 0, 1, 1);
    gAttrGLay->addWidget (pbDelGroup, 2, 0, 1, 1);

    QGroupBox *gbAttr = new QGroupBox (tr("Attributes"));
    gGLay->addWidget (gbAttr, 1, 1, 1, 1);
    QGridLayout *gAttrLay = new QGridLayout ();
    gbAttr->setLayout (gAttrLay);
    gAttrLay->addWidget (pbAddAttr, 0, 0, 1, 1);
    gAttrLay->addWidget (pbEditAttr, 1, 0, 1, 1);
    gAttrLay->addWidget (pbDelAttr, 2, 0, 1, 1);

    gGLay->addWidget (pbUp, 2, 1, 1, 1);
    gGLay->addWidget (pbDown, 3, 1, 1, 1);

    QSpacerItem *sGroup = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    gGLay->addItem (sGroup, 4, 1, 1, 1);
}

void KKSTemplateEditor :: init_attr_widget (QTabWidget *tWidget)
{
    QWidget *attrWidget = new QWidget ();
    tWidget->addTab (attrWidget, tr ("Attributes"));
    QGridLayout *gAttrsLay = new QGridLayout ();
    attrWidget->setLayout (gAttrsLay);

    gAttrsLay->addWidget (tableAttrs, 0, 0, 1, 1);
}

void KKSTemplateEditor :: addTemplateGroup (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();//selModel->currentIndex ();
/*    if (!gIndex.isValid () || gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
    {
        QMessageBox::warning (this, tr("Edit group of attributes"), tr("Please select group"));
        return;
    }
*/
    qDebug () << __PRETTY_FUNCTION__ << gIndex << gIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    while (gIndex.isValid() && gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
        gIndex = gIndex.parent ();
    qDebug () << __PRETTY_FUNCTION__ << gIndex << gIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    emit addAttrGroup (tRef, gIndex, this);
}

void KKSTemplateEditor :: editTemplateGroup (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();//selModel->currentIndex ();
    if (!gIndex.isValid () || gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
    {
        QMessageBox::warning (this, tr("Edit group of attributes"), tr("Please select group"));
        return;
    }

    emit editAttrGroup (gIndex.data (Qt::UserRole).toInt(), gIndex, tRef, this);
}

void KKSTemplateEditor :: delTemplateGroup (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();//selModel->currentIndex ();
    if (!gIndex.isValid () || gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
    {
        QMessageBox::warning (this, tr("Delete group of attributes"), tr("Please select group"));
        return;
    }

    if (gIndex.model()->rowCount() == 1)
    {
        //
        // Template contains only one group
        //
        QMessageBox::warning (this, tr("Delete group of attributes"), tr ("You cannot delete single group"));
        return;
    }

    if (gIndex.child (0, 0).isValid ())
    {
        QMessageBox::warning (this, tr("Delete group of attributes"), tr ("You cannot delete not empty group"));
        return;
    }

    int res = QMessageBox::question (this, tr("Delete group of attributes"), tr ("Do you really want to delete group %1 ?").arg (gIndex.data (Qt::DisplayRole).toString()), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
    if (res == QMessageBox::Yes)
        emit delAttrGroup (gIndex.data (Qt::UserRole).toInt(), gIndex, tRef, this);
}

void KKSTemplateEditor :: addAttribute (void)
{
    const KKSMap<int, KKSCategoryAttr *> avAttrs = tRef->availableAttrs();
    if (avAttrs.isEmpty())
    {
        QMessageBox::warning (this, tr("Add attributes into template"), tr("No available attributes"), QMessageBox::Ok);
        return;
    }

    QModelIndex gIndex = this->getSelectedIndex ();
    if (!gIndex.isValid () || gIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
    {
        QMessageBox::warning (this, tr("Add attribute"), tr("Please select target group"), QMessageBox::Ok);
        return;
    }
    emit addAttrIntoGroup (gIndex.data (Qt::UserRole).toInt(), gIndex, tRef, this);
}

void KKSTemplateEditor :: editAttribute (void)
{
    QModelIndex aIndex = this->getSelectedIndex();//selModel->currentIndex ();
    if (!aIndex.isValid () || aIndex.data (Qt::UserRole+USER_ENTITY).toInt() != 0)
        return;

    QModelIndex gIndex = aIndex.parent ();
    emit editAttrInGroup (aIndex.data (Qt::UserRole).toInt(), gIndex.data (Qt::UserRole).toInt(), aIndex, gIndex, tRef, this);
}

void KKSTemplateEditor :: delAttribute (void)
{
    QModelIndex aIndex = this->getSelectedIndex ();//selModel->currentIndex ();
    if (!aIndex.isValid () || aIndex.data (Qt::UserRole+USER_ENTITY).toInt() != 0 )
        return;

    QModelIndex gIndex = aIndex.parent ();
    emit delAttrFromGroup (aIndex.data (Qt::UserRole).toInt(), gIndex.data (Qt::UserRole).toInt(), aIndex, gIndex, tRef, this);
}

void KKSTemplateEditor :: moveUp (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();
    if (!gIndex.isValid())
        return;

    if (gIndex.parent().isValid())
    {
        //
        // Attribute
        //
        int idAttrGroup = gIndex.parent().data (Qt::UserRole).toInt();
        int idAttr = gIndex.data (Qt::UserRole).toInt ();
        KKSAttrGroup * aGroup = tRef->group (idAttrGroup);
        if (!aGroup)
            return;
        KKSAttrView * aView = aGroup->attrView (idAttr);
        if (!aView)
            return;

        if (gIndex.row() > 0)
        {
            QModelIndex gIndexP = gIndex.sibling (gIndex.row()-1, 0);//gIndex.column ());
            qDebug () << __PRETTY_FUNCTION__ << gIndex << gIndexP;
            if (!gIndexP.isValid())
                return;
            int idAttrP = gIndexP.data (Qt::UserRole).toInt();
            KKSAttrView * aViewPrev = aGroup->attrView (idAttrP);
            if (!aViewPrev)
                return;

            this->swapAttrs (aView, aViewPrev, gIndex, gIndexP);
        }
        else if (idAttrGroup != 0 && gIndex.parent().row() > 0)
        {
            QModelIndex gIndexGP = gIndex.parent().sibling (gIndex.parent().row()-1, 0);//gIndex.parent().column());
            qDebug () << __PRETTY_FUNCTION__ << gIndexGP;
            if (!gIndexGP.isValid())
                return;

            int idAttrGroupPrev = gIndexGP.data (Qt::UserRole).toInt ();
            emit delAttrFromGroup (idAttr, idAttrGroup, gIndex, gIndexGP, tRef, this);
            emit appendAttrIntoGroup (idAttr, idAttrGroupPrev, gIndex, tRef, this);
            this->tvGroups->selectionModel ()->clearSelection();//select (gIndex, QItemSelectionModel::Deselect);
            QModelIndex cIndex = gIndexGP.child (this->tvGroups->model()->rowCount(gIndexGP)-1, 0);
            QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
            QItemSelection newSel = QItemSelection (cIndex.sibling (cIndex.row(), 0), cIndex.sibling(cIndex.row(), cIndex.model()->columnCount (cIndex.parent())-1));
            this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
            this->tvGroups->selectionModel ()->setCurrentIndex (cIndex, QItemSelectionModel::Select);
            this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
        }
    }
    else if (gIndex.row() > 1)
    {
        //
        // Group
        //
        int idAttrGroup = gIndex.data (Qt::UserRole).toInt();
        KKSAttrGroup * aGroup = tRef->group (idAttrGroup);
        if (!aGroup)
            return;
        QModelIndex gIndexGP = gIndex.sibling (gIndex.row()-1, 0);//gIndex.column());
        if (!gIndexGP.isValid())
            return;
        int idAttrGroupPrev = gIndexGP.data (Qt::UserRole).toInt ();
        KKSAttrGroup *aGroupPrev = tRef->group (idAttrGroupPrev);
        if (!aGroupPrev)
            return;
        int worder = aGroup->order ();
        aGroup->setOrder (aGroupPrev->order());
        aGroupPrev->setOrder (worder);
        KKSMap <int, KKSAttrGroup *> tgr = tRef->groups();
        qSwap (tgr[gIndex.row()], tgr[gIndexGP.row()]);
        tRef->setGroups (tgr);
        emit updateAttrGroups (tRef, this);
        QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
        QItemSelection newSel = QItemSelection (gIndexGP.sibling (gIndexGP.row(), 0), gIndexGP.sibling(gIndexGP.row(), gIndexGP.model()->columnCount (gIndexGP.parent())-1));
        this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
        this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
        this->tvGroups->selectionModel ()->setCurrentIndex (gIndexGP, QItemSelectionModel::Select);
    }
}

void KKSTemplateEditor :: moveDown (void)
{
    QModelIndex gIndex = this->getSelectedIndex ();
    if (!gIndex.isValid())
        return;

    if (gIndex.parent().isValid())
    {
        //
        // Attribute
        //
        int idAttrGroup = gIndex.parent().data (Qt::UserRole).toInt();
        int idAttr = gIndex.data (Qt::UserRole).toInt ();
        KKSAttrGroup * aGroup = tRef->group (idAttrGroup);
        if (!aGroup)
            return;
        KKSAttrView * aView = aGroup->attrView (idAttr);
        if (!aView)
            return;

        if (gIndex.row() < tvGroups->model()->rowCount (gIndex.parent())-1)
        {
            QModelIndex gIndexP = gIndex.sibling (gIndex.row()+1, 0);//gIndex.column ());
            qDebug () << __PRETTY_FUNCTION__ << gIndex << gIndexP;
            if (!gIndexP.isValid())
                return;
            int idAttrP = gIndexP.data (Qt::UserRole).toInt();
            KKSAttrView * aViewPrev = aGroup->attrView (idAttrP);
            if (!aViewPrev)
                return;

            this->swapAttrs (aView, aViewPrev, gIndex, gIndexP);

        }
        else if (gIndex.parent().row() != 0 || 
                 (gIndex.parent().row() ==0 && 
                 this->tvGroups->model()->rowCount (gIndex.parent()) > 1))
                //gIndex.parent().row() < gIndex.parent().model()->rowCount()-1)
        {
            QModelIndex gIndexGP = gIndex.parent().sibling (gIndex.parent().row()+1, 0);//gIndex.parent().column());
            qDebug () << __PRETTY_FUNCTION__ << gIndexGP;
            if (!gIndexGP.isValid())
                return;

            int idAttrGroupPrev = gIndexGP.data (Qt::UserRole).toInt ();
            emit delAttrFromGroup (idAttr, idAttrGroup, gIndex, gIndexGP, tRef, this);
            emit pushAttrIntoGroup (idAttr, idAttrGroupPrev, tRef, this);
            this->tvGroups->selectionModel ()->clearSelection();//select (gIndex, QItemSelectionModel::Deselect);
            QModelIndex cIndex = gIndexGP.child (0, 0);
            QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
            QItemSelection newSel = QItemSelection (cIndex.sibling (cIndex.row(), 0), cIndex.sibling(cIndex.row(), cIndex.model()->columnCount (cIndex.parent())-1));
            this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
            this->tvGroups->selectionModel ()->setCurrentIndex (cIndex, QItemSelectionModel::Select);
            this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
        }
    }
    else if (gIndex.row() < gIndex.model()->rowCount() -1 )
    {
        //
        // Group
        //
        int idAttrGroup = gIndex.data (Qt::UserRole).toInt();
        KKSAttrGroup * aGroup = tRef->group (idAttrGroup);
        if (!aGroup)
            return;
        QModelIndex gIndexGN = gIndex.sibling (gIndex.row()+1, 0);//gIndex.column());
        if (!gIndexGN.isValid())
            return;
        int idAttrGroupNext = gIndexGN.data (Qt::UserRole).toInt ();
        KKSAttrGroup *aGroupNext = tRef->group (idAttrGroupNext);
        if (!aGroupNext)
            return;
        int worder = aGroup->order ();
        aGroup->setOrder (aGroupNext->order());
        aGroupNext->setOrder (worder);
        KKSMap <int, KKSAttrGroup *> tgr = tRef->groups();
        qSwap (tgr[gIndex.row()], tgr[gIndexGN.row()]);
        tRef->setGroups (tgr);
        emit updateAttrGroups (tRef, this);
        QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
        QItemSelection newSel = QItemSelection (gIndexGN.sibling (gIndexGN.row(), 0), gIndexGN.sibling(gIndexGN.row(), gIndexGN.model()->columnCount (gIndexGN.parent())-1));
        this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
        this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
        this->tvGroups->selectionModel ()->setCurrentIndex (gIndexGN, QItemSelectionModel::Select);
    }
}

QModelIndex KKSTemplateEditor :: getSelectedIndex (void) const
{
    if (!tRef)
        return QModelIndex();

    QItemSelectionModel *selModel = tvGroups->selectionModel ();
    if (!selModel)
        return QModelIndex ();

    QItemSelection sel = selModel->selection();
    if (sel.indexes().isEmpty())
        return QModelIndex ();
    QModelIndex wIndex = sel.indexes ().at (0);// (selModel->currentIndex ();
    return wIndex.sibling (wIndex.row(), 0);
}

void KKSTemplateEditor :: swapAttrs (KKSAttrView *aView, KKSAttrView *aViewPrev, const QModelIndex& gIndex, const QModelIndex& gIndexP)
{
    int worder = aView->order ();
    aView->setOrder (aViewPrev->order());
    aViewPrev->setOrder (worder);

    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 0), aView->title(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 0), aView->id(), Qt::UserRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 1), aView->defValue().valueVariant(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 1), aView->defValue().value(), Qt::UserRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 2), aView->isMandatory() ? tr("Yes") : tr ("No"), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 2), aView->isMandatory(), Qt::UserRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 3), aView->isReadOnly() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndexP.sibling(gIndexP.row(), 3), aView->isReadOnly(), Qt::UserRole);

    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 0), aViewPrev->title(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 0), aViewPrev->id(), Qt::UserRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 1), aViewPrev->defValue().valueVariant(), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 1), aViewPrev->defValue().value(), Qt::UserRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 2), aViewPrev->isMandatory() ? tr("Yes") : tr ("No"), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 2), aViewPrev->isMandatory(), Qt::UserRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 3), aViewPrev->isReadOnly() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
    this->tvGroups->model()->setData (gIndex.sibling(gIndex.row(), 3), aViewPrev->isReadOnly(), Qt::UserRole);

    QItemSelection oldSel = this->tvGroups->selectionModel ()->selection();
    QItemSelection newSel = QItemSelection (gIndexP.sibling (gIndexP.row(), 0), gIndexP.sibling(gIndexP.row(), gIndexP.model()->columnCount (gIndexP.parent())-1));
    this->tvGroups->selectionModel ()->select (newSel, QItemSelectionModel::Select);
    this->tvGroups->selectionModel ()->setCurrentIndex (gIndexP, QItemSelectionModel::Select);
    this->tvGroups->selectionModel ()->select (oldSel, QItemSelectionModel::Deselect);
}

void KKSTemplateEditor :: setModel (QAbstractItemModel *model)
{
    QAbstractItemModel *oldModel = this->tvGroups->model ();
    this->tvGroups->setModel (model);
    if (oldModel)
        delete oldModel;
}

void KKSTemplateEditor :: setItemDelegate (QAbstractItemDelegate *iDeleg)
{
    QAbstractItemDelegate *oldDeleg = this->tvGroups->itemDelegate ();
    this->tvGroups->setItemDelegate (iDeleg);
    if (oldDeleg)
        delete oldDeleg;
}

void KKSTemplateEditor :: saveErr (void)
{
    dbErr = true;
}