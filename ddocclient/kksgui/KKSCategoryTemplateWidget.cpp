#include <QGridLayout>
#include <QVBoxLayout>
#include <QEventLoop>
#include <QPushButton>
#include <QTreeView>
#include <QPointer>
#include <QDialog>
#include <QMessageBox>
#include <QItemSelectionModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QKeySequence>
#include <QGroupBox>
#include <QMap>
#include <QResizeEvent>
#include <QtDebug>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QIcon>

#include "defines.h"
#include <KKSCategory.h>
#include <KKSType.h>
#include "KKSItemDelegate.h"
#include "KKSEventFilter.h"
#include "KKSCategoryTemplateWidget.h"

KKSCategoryTemplateWidget :: KKSCategoryTemplateWidget (bool mode, const QList<int>& fTypes, bool asAdmin, QWidget *parent, Qt::WindowFlags f)
    : KKSDialog (parent, f),
    catTemplLayout (new QGridLayout()),
    tvCatTemplate (new QTreeView()),
    pbOk (new QPushButton (tr("&OK"))),
    pbCancel (new QPushButton (tr("&Cancel"))),
    tbCatTempl (new QToolBar (tr ("Category and templates editor"), this)),
    actAddC (new QAction (QIcon(":/ddoc/category_add.png"), tr("Add empty category"), this)),
    //actAddCopyC (new QAction (QIcon (":/ddoc/add_copy.png"), tr ("Add copy of selected category"), this)),
    actAddConBase (new QAction (QIcon (":/ddoc/category_copy.png"), tr ("Add new category on exists"), this)),
    actAddT (new QAction (QIcon(":/ddoc/add.png"), tr("Add template"), this)),
    actEditC (new QAction (QIcon (":/ddoc/edit.png"), tr("Edit selected category"), this)),
    actEditT (new QAction (QIcon (":/ddoc/edit.png"), tr("Edit template"), this)),
    actDelC (new QAction (QIcon (":/ddoc/delete.png"), tr("Delete selected category"), this)),
    actDelT (new QAction (QIcon (":/ddoc/delete.png"), tr("Delete template"), this)),
    m_asAdmin(asAdmin),
    forbiddenTypes (fTypes)
{
    this->init_widgets ();

    sModel = tvCatTemplate->selectionModel ();
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (this);
    tvCatTemplate->setItemDelegate (itemDeleg);
    KKSEventFilter *eFilter = new KKSEventFilter (this);
    tvCatTemplate->viewport()->installEventFilter (eFilter);

    if (sModel)
        connect (sModel, SIGNAL (currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT (currIndexChanged (const QModelIndex&, const QModelIndex&)) );

    pCatInd.clear ();

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );

    pbOk->setVisible (mode);
    pbCancel->setVisible (mode);
    pbOk->setDefault (true);
    pbOk->setAutoDefault (true);
    QKeySequence cancel_key (Qt::Key_Escape);
    pbCancel->setShortcut (cancel_key);

    connect (actAddC, SIGNAL (triggered()), this, SLOT (addCat()) );
    //connect (actAddCopyC, SIGNAL (triggered()), this, SLOT (addCopyCat()) );
    connect (actAddConBase, SIGNAL (triggered()), this, SLOT (addCloneCat()) );
    connect (actAddT, SIGNAL (triggered()), this, SLOT (addTemplate()) );
    connect (actEditC, SIGNAL (triggered()), this, SLOT (editCat()) );
    connect (actEditT, SIGNAL (triggered()), this, SLOT (editTemplate()) );
    connect (actDelC, SIGNAL (triggered()), this, SLOT (delCat()) );
    connect (actDelT, SIGNAL (triggered()), this, SLOT (delTemplate()) );
}

KKSCategoryTemplateWidget :: ~KKSCategoryTemplateWidget (void)
{
    pCatInd.clear ();
}

int KKSCategoryTemplateWidget :: getCatID (void) const
{
    return getCurrentCategoryId ();
}

int KKSCategoryTemplateWidget :: getTemplateID (void) const
{
    return getCurrentTemplateId ();
}

void KKSCategoryTemplateWidget :: accept (void)
{
    this->apply ();
    KKSDialog::accept ();
}

void KKSCategoryTemplateWidget :: apply (void)
{
}

void KKSCategoryTemplateWidget :: addCat (void)
{
    int idParentC = -1;
    QModelIndex wIndex = tvCatTemplate->selectionModel()->currentIndex();
    if (wIndex.isValid())
        idParentC = this->getCurrentCategoryId ();
    int idCatType = -1;
    while (wIndex.parent().isValid() && (idCatType < 0 || idCatType==10))
    {
        wIndex = wIndex.parent();
        if (wIndex.data(Qt::UserRole+USER_ENTITY).toInt() == 1)
            idCatType = wIndex.data (Qt::UserRole).toInt();
    }
    idCatType = wIndex.data (Qt::UserRole).toInt();
    
    //Разрешаем создавать только несистемные категории
    
//    for (int i=1; i<=7; i++)
//        avTypes << i;
//    avTypes << 11 << 12;
//    forbiddenTypes << 8 << 9 << 10;
    
    if (!forbiddenTypes.contains (idCatType))
        emit addNewCategory (this, idCatType, false);
    else
    {
        QMessageBox::warning (this, tr ("Add category"), tr ("This is system type of category.\nCategory cannot been created."), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
}

void KKSCategoryTemplateWidget :: addCopyCat (void)
{
    int idCat = this->getCurrentCategoryId ();
    QModelIndex wIndex = tvCatTemplate->selectionModel()->currentIndex ();
    if (idCat > 0)
    {
        if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() > 0)
            pCatInd.insert (idCat, wIndex);
        emit addCopyCategory(this, idCat, false);
    }
    else
        QMessageBox::warning (this, tr("Warning"), tr("Please select category"), QMessageBox::Ok);
}

void KKSCategoryTemplateWidget :: addCloneCat (void)
{
    int idCat = this->getCurrentCategoryId ();
    QModelIndex wIndex = tvCatTemplate->selectionModel()->currentIndex ();
    if (idCat > 0)
    {
        if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() > 0)
            pCatInd.insert (idCat, wIndex);
        emit addNewCategoryE(this, idCat, false);
    }
    else
        QMessageBox::warning (this, tr("Warning"), tr("Please select category"), QMessageBox::Ok);
}

void KKSCategoryTemplateWidget :: addTemplate (void)
{
    int idCat = this->getCurrentCategoryId ();
    QModelIndex cIndex = this->getMainCatIndex ();
    if (idCat > 0)
    {
        emit addNewTemplate (this, idCat);
        pCatInd.insert (idCat, cIndex);
    }
    else
        QMessageBox::warning (this, tr("Warning"), tr("You have to select category"), QMessageBox::Ok);
}

void KKSCategoryTemplateWidget :: editCat (void)
{
    int idCat = this->getCurrentCategoryId ();
    QModelIndex wIndex = tvCatTemplate->selectionModel()->currentIndex ();
    if (idCat > 0)
    {
        if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() > 0)
            pCatInd.insert (idCat, wIndex);
        emit editCategory (this, idCat, false);
    }
    else
        QMessageBox::warning (this, tr("Warning"), tr("Please select category"), QMessageBox::Ok);
}

void KKSCategoryTemplateWidget :: editTemplate (void)
{
    int idT = this->getCurrentTemplateId ();
    if (idT > 0)
        emit editTempl (this, idT);
    else
    {
        if (tvCatTemplate->selectionModel()->currentIndex ().isValid() &&
            tvCatTemplate->selectionModel()->currentIndex ().parent().isValid() &&
            tvCatTemplate->selectionModel()->currentIndex ().data (Qt::UserRole+USER_ENTITY).toInt() == 0)
            QMessageBox::warning (this, tr("Warning"), tr("Base template cannot be edited"), QMessageBox::Ok);
        else
            QMessageBox::warning (this, tr("Warning"), tr("Please select template"), QMessageBox::Ok);
    }
}

void KKSCategoryTemplateWidget :: delCat (void)
{
    int idC = this->getCurrentCategoryId ();
    if (idC <= 0)
    {
        QMessageBox::warning (this, tr ("Delete category"), tr ("Select category for delete"), QMessageBox::Ok);
        return;
    }

    QMessageBox::StandardButton cres=QMessageBox::question (this, tr("Delete category"), tr("Do you really want to delete category %1 and all its items ?").arg (idC), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

    if (cres == QMessageBox::Yes)
        emit deleteCat (this, idC);
}

void KKSCategoryTemplateWidget :: delTemplate (void)
{
    int idT = this->getCurrentTemplateId ();
    if (idT <= 0)
    {
        if (tvCatTemplate->selectionModel()->currentIndex ().isValid() &&
            tvCatTemplate->selectionModel()->currentIndex ().parent().isValid() &&
            tvCatTemplate->selectionModel()->currentIndex ().data (Qt::UserRole+USER_ENTITY).toInt() == 0)
            QMessageBox::warning (this, tr("Warning"), tr("Base template cannot be deleted"), QMessageBox::Ok);
        return;
    }

    QMessageBox::StandardButton tres=QMessageBox::question (this, tr("Delete template"), tr("Do you really want to delete template %1 and all its items ?").arg (idT), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);

    if (tres == QMessageBox::Yes)
    {
        QModelIndex cIndex = this->getMainCatIndex ();
        emit deleteT (this, idT);
        pCatInd.insert (cIndex.data (Qt::UserRole).toInt (), cIndex);
    }
}

void KKSCategoryTemplateWidget :: init_widgets (void)
{
    this->setWindowTitle (tr("View categories and templates"));
    if (!catTemplLayout)
        return;

    this->setLayout (catTemplLayout);
    catTemplLayout->addWidget (tbCatTempl, 0, 0, 1, 1);
    tbCatTempl->addAction (actAddC);
    //tbCatTempl->addAction (actAddCopyC);
    tbCatTempl->addAction (actAddConBase);
    tbCatTempl->addAction (actEditC);
    tbCatTempl->addAction (actDelC);
    //tbCatTempl->addSeparator ();
    tbCatTempl->addAction (actAddT);
    tbCatTempl->addAction (actEditT);
    tbCatTempl->addAction (actDelT);
    actAddT->setVisible (false);
    actEditT->setVisible (false);
    actDelT->setVisible (false);
    catTemplLayout->addWidget (tvCatTemplate, 1, 0, 1, 1);
    tvCatTemplate->setSelectionMode (QAbstractItemView::SingleSelection);
    QVBoxLayout *vButtonsGroupLayout = new QVBoxLayout ();

    vButtonsGroupLayout->addStretch ();
    vButtonsGroupLayout->addWidget (pbOk);
    vButtonsGroupLayout->addWidget (pbCancel);

    catTemplLayout->addLayout (vButtonsGroupLayout, 0, 1, 2, 1);
    tbCatTempl->setVisible (m_asAdmin);
}

int KKSCategoryTemplateWidget :: getCurrentCategoryId (void) const
{
    int idCat = -1;
    if (tvCatTemplate->selectionModel () &&
            tvCatTemplate->selectionModel()->currentIndex ().isValid())
    {
        QModelIndex wIndex = tvCatTemplate->selectionModel()->currentIndex ();
        if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
        {
            //
            // Template item is selected
            //
            if (wIndex.parent().parent().isValid() && wIndex.parent().data (Qt::UserRole+USER_ENTITY).toInt()<=0)
                idCat = wIndex.parent().parent().data (Qt::UserRole).toInt();
            else
                idCat = wIndex.parent().data (Qt::UserRole).toInt();
        }
        else if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 1)
        {
            //
            // Category item is selected
            //
            idCat = wIndex.parent().parent().isValid() ? wIndex.parent().data (Qt::UserRole).toInt () : wIndex.data (Qt::UserRole).toInt ();
        }
        else if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() < 0)
        {
            //
            // word "Templates"
            //
            idCat = wIndex.parent().data (Qt::UserRole).toInt();
        }
    }
    return idCat;
}

int KKSCategoryTemplateWidget :: getCurrentTemplateId (void) const
{
    int idTemplate = -1;
    if (tvCatTemplate->selectionModel () &&
            tvCatTemplate->selectionModel()->currentIndex ().isValid() &&
            tvCatTemplate->selectionModel()->currentIndex ().parent().isValid())
    {
        QModelIndex wIndex = tvCatTemplate->selectionModel()->currentIndex ();
        if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
        {
            //
            // Template item is selected
            //
            idTemplate = wIndex.data (Qt::UserRole).toInt();
        }
    }
    return idTemplate;
}

void KKSCategoryTemplateWidget :: currIndexChanged (const QModelIndex& current, const QModelIndex& previous)
{
    qDebug () << __PRETTY_FUNCTION__ << current << previous;
    QAbstractItemModel * tModel = tvCatTemplate->model ();
    QModelIndex tIndex (current);
    while (tIndex.parent().isValid())
        tIndex = tIndex.parent();
    bool isActionsDisabled (forbiddenTypes.contains (tIndex.data(Qt::UserRole).toInt()));
    actAddC->setDisabled (isActionsDisabled);
    //actAddCopyC->setDisabled (isActionsDisabled);
    actEditC->setDisabled (isActionsDisabled);
    actDelC->setDisabled (isActionsDisabled);
    if (tModel &&
            current.isValid () && 
            current.parent().isValid() && 
            !current.parent().parent().isValid() &&
            tModel->rowCount (current) == 0)
    {
        int idCat = current.data (Qt::UserRole).toInt ();
        emit uploadTemplatesIntoCategory (this, idCat, current);
    }
    Q_UNUSED (previous);
}

void KKSCategoryTemplateWidget :: uploadModel (QAbstractItemModel *mod)
{
    QAbstractItemModel * oldModel = tvCatTemplate->model ();
    tvCatTemplate->setModel (mod);
    if (oldModel)
        delete oldModel;

    if (!mod)// && sModel)
        disconnect (SIGNAL (currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT (currIndexChanged (const QModelIndex&, const QModelIndex&)) );

    sModel = tvCatTemplate->selectionModel ();

    if (sModel)
        connect (sModel, SIGNAL (currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT (currIndexChanged (const QModelIndex&, const QModelIndex&)) );
}

void KKSCategoryTemplateWidget :: addCatRec (KKSCategory * c)
{
    if (!c)
        return;

    if (c->type()->id() == 10)
    {
        //
        // Подчиненная категория
        //
        return;
    }
    QAbstractItemModel * cModel = tvCatTemplate->model ();
    int nRows = cModel->rowCount();
    QModelIndex wIndex; 
    if (pCatInd.contains (c->id()))
        wIndex = pCatInd.value (c->id());
    else
    {
        QModelIndex parent = QModelIndex();
        for (int i=0; i < nRows && !parent.isValid(); i++)
        {
            QModelIndex pInd = cModel->index (i, 0);
            if (c->type()->id() == pInd.data (Qt::UserRole).toInt())
                parent = pInd;
        }
        int nCRows = cModel->rowCount (parent);
        if (!cModel->insertRows (nCRows, 1, parent) || (cModel->columnCount (parent) == 0 && !cModel->insertColumns (0, 1, parent)))
            return;
        wIndex = cModel->index (nCRows, 0, parent);
    }

    cModel->setData (wIndex, c->id(), Qt::UserRole);
    cModel->setData (wIndex, c->name(), Qt::DisplayRole);
    cModel->setData (wIndex, 1, Qt::UserRole+USER_ENTITY);
    this->tvCatTemplate->update();
    //c->release ();
}

void KKSCategoryTemplateWidget :: uploadCatT (KKSTemplate *t)
{
    QModelIndex cIndex = pCatInd.value (t->category()->id());
    qDebug () << __PRETTY_FUNCTION__ << t->id () << cIndex;
    if (cIndex.isValid())
        emit uploadTemplatesIntoCategory (this, t->category()->id(), cIndex);
}

QModelIndex KKSCategoryTemplateWidget :: getMainCatIndex (void) const
{
    QModelIndex catIndex;
    if (tvCatTemplate->selectionModel () &&
            tvCatTemplate->selectionModel()->currentIndex ().isValid())
    {
        QModelIndex wIndex = tvCatTemplate->selectionModel()->currentIndex ();
        if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
        {
            //
            // Template item is selected
            //
            if (wIndex.parent().parent().isValid() && wIndex.parent().data (Qt::UserRole+USER_ENTITY).toInt()<=0)
                catIndex = wIndex.parent().parent();//.data (Qt::UserRole).toInt();
            else
                catIndex = wIndex.parent();//.data (Qt::UserRole).toInt();
        }
        else if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() > 0)
        {
            //
            // Category item is selected
            //
            catIndex = wIndex;
        }
        else if (wIndex.data (Qt::UserRole+USER_ENTITY).toInt() < 0)
        {
            //
            // word "Templates"
            //
            catIndex = wIndex.parent();//.data (Qt::UserRole).toInt();
        }
    }
    return catIndex;
}

QAbstractItemModel * KKSCategoryTemplateWidget :: getModel (void) const
{
    return this->tvCatTemplate->model ();
}

QModelIndex KKSCategoryTemplateWidget :: getSelectedIndex (void) const
{
    return this->tvCatTemplate->selectionModel()->currentIndex ();
}

QSize KKSCategoryTemplateWidget :: sizeHint (void) const
{
    return QSize (640, 600);
}
