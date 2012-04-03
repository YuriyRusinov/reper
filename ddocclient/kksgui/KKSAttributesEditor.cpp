#include <QDialog>
#include <QMessageBox>
#include <QGridLayout>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
//#include <QPushButton>
//#include <QToolButton>
#include <QAction>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QtDebug>

#include <KKSAttribute.h>
#include <KKSCategoryAttr.h>
#include <KKSAttrType.h>

#include "KKSRecWidget.h"
#include "ui/kksattreditor.h"
#include "KKSAttributesEditor.h"
#include "defines.h"

KKSAttributesEditor :: KKSAttributesEditor (const KKSMap<int, KKSAttrType*>& aTypes, 
                                            const KKSMap<int, KKSAGroup *>& aGroups, 
                                            const QMap<int, QString>& io_refs, 
                                            QWidget *parent, 
                                            Qt::WindowFlags f)
    : KKSDialog (parent, f),
    recW (0),
    attrTypes (aTypes),
    attrsGroups (aGroups),
    ioRefs (io_refs),
    aGroupAdd (new QAction (QIcon(":/ddoc/add.png"), tr("Add group of attributes"), this)),
    aGroupEdit (new QAction (QIcon(":/ddoc/edit.png"), tr("Edit group of attributes"), this)),
    aGroupDel (new QAction (QIcon(":/ddoc/delete.png"), tr ("Delete group of attributes"), this))
{
    this->setWindowTitle (tr("Select attribute"));

    connect (aGroupAdd, SIGNAL (triggered()), this, SLOT (addAGroup()) );
    connect (aGroupEdit, SIGNAL (triggered()), this, SLOT (editAGroup()) );
    connect (aGroupDel, SIGNAL (triggered()), this, SLOT (delAGroup()) );
}

KKSAttributesEditor :: KKSAttributesEditor (QWidget *parent, Qt::WindowFlags f)
    : KKSDialog (parent, f),
    recW (0),
    attrTypes (KKSMap<int, KKSAttrType*>()),
    attrsGroups (KKSMap<int, KKSAGroup*>()),
    ioRefs (QMap<int, QString>()),
    aGroupAdd (new QAction (QIcon(":/ddoc/add.png"), tr("Add group of attributes"), this)),
    aGroupEdit (new QAction (QIcon(":/ddoc/edit.png"), tr("Edit group of attributes"), this)),
    aGroupDel (new QAction (QIcon(":/ddoc/delete.png"), tr ("Delete group of attributes"), this))
{
    this->setWindowTitle (tr("Select attribute"));
    connect (aGroupAdd, SIGNAL (triggered()), this, SLOT (addAGroup()) );
    connect (aGroupEdit, SIGNAL (triggered()), this, SLOT (editAGroup()) );
    connect (aGroupDel, SIGNAL (triggered()), this, SLOT (delAGroup()) );
}

KKSAttributesEditor :: ~KKSAttributesEditor (void)
{
}

int KKSAttributesEditor :: getAttrId (void) const
{
    QModelIndex aInd = recW ? recW->getSourceIndex() : QModelIndex();
    if (!aInd.isValid())
        return -1;
    aInd = aInd.sibling (aInd.row(), 0);
    int idEntity = aInd.data (Qt::UserRole+USER_ENTITY).toInt();
    if (idEntity > 0)
        return aInd.data (Qt::UserRole).toInt();

    return -1;
}

QList<int> KKSAttributesEditor :: getAttributesId (void) const
{
    QList<int> attrsId;
    if (recW && !recW->getSourceIndexes().isEmpty())
    {
        QModelIndexList indexes = recW->getSourceIndexes();
        for (int i=0; i<indexes.count(); i++)
        {
            int attrId = indexes[i].sibling (indexes[i].row(), 0).data (Qt::UserRole).toInt();
            int attrEId = indexes[i].sibling (indexes[i].row(), 0).data (Qt::UserRole+USER_ENTITY).toInt();
            if (!attrsId.contains (attrId) && attrEId == 1)
                attrsId.append (attrId);
        }
    }

    return attrsId;
}

void KKSAttributesEditor :: addAttribute (void)
{
    KKSAttribute *attribute = new KKSAttribute ();
    if (!attribute)
        return;

    KKSAttrEditor *aEditor = new KKSAttrEditor (attribute, attrTypes, attrsGroups, ioRefs, this);
    connect (aEditor, 
             SIGNAL (getReferenceFields (KKSAttribute *, int, KKSAttrEditor *)),
             this,
             SLOT (getRefFields (KKSAttribute *, int, KKSAttrEditor *)));

    connect (aEditor, 
             SIGNAL (getSearchTemplate (KKSAttrEditor *)),
             this,
             SLOT (slotGetSearchTemplate (KKSAttrEditor *)) );
    
    connect (aEditor, 
             SIGNAL(showAttrsWidget(KKSAttribute *, KKSAttrEditor *)), 
             this, 
             SIGNAL(showAttrsWidget(KKSAttribute *, KKSAttrEditor *)) ); //передаем все в KKSAttributesFactory

    connect (aEditor, 
             SIGNAL(addAttribute(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)), 
             this, 
             SIGNAL(addAttribute(KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)) );//передаем все в KKSAttributesFactory

    connect (aEditor, 
             SIGNAL(editAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)), 
             this, 
             SIGNAL(editAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)) );//передаем все в KKSAttributesFactory

    connect (aEditor, 
             SIGNAL(delAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)), 
             this, 
             SIGNAL(delAttribute(int, KKSAttribute *, QAbstractItemModel*, KKSAttrEditor *)) );//передаем все в KKSAttributesFactory


    QModelIndex pIndex = recW->getSourceIndex();
    int idEType = pIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    while (pIndex.isValid() && idEType != 0)
    {
        pIndex = pIndex.parent();
        idEType = pIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    }
    if (!pIndex.isValid())
    {
        QMessageBox::warning (this, tr ("Add attribute"), tr("Select parent group"), QMessageBox::Ok);
        delete aEditor;
        attribute->release ();
        return;
    }
    aEditor->setGroupId(pIndex.data(Qt::UserRole).toInt());
    if (aEditor->exec () == QDialog::Accepted)
    {
        int idType = aEditor->getTypeID ();
        int idAGroup = aEditor->getGroupId ();
        emit insertAttr (aEditor->getAttribute(), idType, recW->getSourceModel(), idAGroup, this);
    }

    if (aEditor)
    {
        aEditor->disconnect ();
        delete aEditor;
    }

    attribute->release ();
}

void KKSAttributesEditor :: editAttribute (void)
{
    int idAttr = this->getAttrId ();
    if (idAttr < 0)
        return;

    QModelIndex pIndex = recW->getSourceIndex();
    pIndex = pIndex.sibling(pIndex.row(), 0);
    QAbstractItemModel * aModel = recW->getSourceModel ();
    if (!pIndex.isValid())
    {
        QMessageBox::warning (this, tr ("Edit attribute"), tr("Select parent group"), QMessageBox::Ok);
        return;
    }
    emit updateAttr (idAttr, aModel, pIndex, this);
}

void KKSAttributesEditor :: delAttribute (void)
{
    int idAttr = this->getAttrId ();
    if (idAttr < 0)
        return;

    QModelIndex pIndex = recW->getSourceIndex();
    if (!pIndex.isValid())
    {
        QMessageBox::warning (this, tr ("Delete attribute"), tr("Select parent group"), QMessageBox::Ok);
        return;
    }
    if (QMessageBox::question (this, tr("Delete attribute"), tr("Do you really want to delete attribute %1 ?").arg (idAttr), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes) == QMessageBox::Yes)
        emit deleteAttr (idAttr, recW->getSourceModel(), pIndex, this);
}

void KKSAttributesEditor :: uploadAttribute (KKSAttribute *attr)
{
    Q_UNUSED (attr);
/*
    //
    // Действия после загрузки атрибута из базы
    //
    bool is_bad;
    KKSCategoryAttr *cAttr = KKSCategoryAttr::create (attr, true, false, QString(), &is_bad);

    if (is_bad)
    {
        QMessageBox::warning (this, tr("Load attribute"), tr("Cannot transform attribute into category"));
        return;
    }
    if (!cAttr)
        return;

    qDebug () << __PRETTY_FUNCTION__ << ioRefs;
    QModelIndex pIndex = recW->getSourceIndex();
    int idEType = pIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    while (pIndex.isValid() && idEType != 0)
    {
        pIndex = pIndex.parent();
        idEType = pIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    }
    if (!pIndex.isValid())
    {
        QMessageBox::warning (this, tr ("Edit attribute"), tr("Select parent group"), QMessageBox::Ok);
        cAttr->release ();
        return;
    }
    KKSAttrEditor *aEditor = new KKSAttrEditor (cAttr, attrTypes, ioRefs, this);
    connect (aEditor, 
             SIGNAL (getSearchTemplate (KKSAttrEditor *)),
             this,
             SLOT (slotGetSearchTemplate (KKSAttrEditor *)) );


    cAttr->release ();
    if (aEditor->exec () == QDialog::Accepted)
        emit updateAttr (aEditor->getAttribute(), recW->getSourceModel(), pIndex, this);
 */
}

void KKSAttributesEditor :: init_widgets (void)
{
    if (!recW)
        return;
    QGridLayout *gLay = new QGridLayout ();
    this->setLayout (gLay);
    gLay->addWidget (recW, 0, 0, 1, 1);
}

void KKSAttributesEditor :: setRecordsWidget (KKSRecWidget * rw)
{
    if (recW)
    {
        disconnect (recW, 0, this, 0);
        delete recW;
    }

    recW = rw;
    if (recW)
    {
        recW->setParent (this);
        recW->disconnect ();
        connect (recW->actFilter, SIGNAL (triggered()), this, SLOT (searchAttrs()) );
        recW->actAdd->setText (tr("Add attribute"));
        connect (recW->actAdd, SIGNAL (triggered()), this, SLOT (addAttribute()) );
        recW->actEdit->setText (tr("Edit attribute"));
        connect (recW->actEdit, SIGNAL (triggered()), this, SLOT (editAttribute()) );
        recW->actDel->setText (tr("Delete attribute"));
        connect (recW->actDel, SIGNAL (triggered()), this, SLOT (delAttribute()) );
        recW->insertToolBarAction (recW->actAdd, aGroupAdd);
        recW->insertToolBarAction (recW->actAdd, aGroupEdit);
        recW->insertToolBarAction (recW->actAdd, aGroupDel);
        QAction * gaSep = new QAction (this);
        gaSep->setSeparator (true);
        recW->insertToolBarAction (recW->actAdd, gaSep);
    }
    this->init_widgets ();
}

QAbstractItemModel * KKSAttributesEditor :: getModel (void) const
{
    return recW->getModel ();
}

QModelIndex KKSAttributesEditor :: getCurrentIndex (void) const
{
    return recW->getCurrentIndex ();
}

KKSMap<int, KKSAttrType *> KKSAttributesEditor :: getTypes (void) const
{
    return attrTypes;
}

QItemSelectionModel * KKSAttributesEditor :: getSelectionModel (void) const
{
    return recW->getSelectionModel ();
}

QMap<int, QString> KKSAttributesEditor :: getReferences (void) const
{
    return ioRefs;
}

void KKSAttributesEditor :: getRefFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor)
{
    emit getFieldsOfReference (attr, idRef, aEditor);
}

void KKSAttributesEditor :: slotGetSearchTemplate (KKSAttrEditor * aEditor)
{
    emit getSearchTemplate (aEditor);
}

void KKSAttributesEditor :: searchAttrs (void)
{
    int idAttrRef = IO_ATTR_ID;
    QAbstractItemModel * aModel = recW->getSourceModel ();
    emit findInAttributes (idAttrRef, aModel, this);
}

QTreeView * KKSAttributesEditor :: getView (void) const
{
    return recW->getView ();
}

void KKSAttributesEditor :: setAttr (const QModelIndex & index)
{
    qDebug () << __PRETTY_FUNCTION__ << index;
    QModelIndex aIndex = index.sibling (index.row(), 0);
    int idEntity = aIndex.data (Qt::UserRole+USER_ENTITY).toInt();
    if (idEntity)
        editAttribute ();
    else
        editAGroup ();
    //if (index.isValid())
    //    accept ();
}

void KKSAttributesEditor :: mousePressEvent (QMouseEvent *event)
{
    qDebug () << __PRETTY_FUNCTION__ << event << event->pos();
    QWidget::mousePressEvent (event);
}

void KKSAttributesEditor :: dragEnterEvent (QDragEnterEvent *event)
{
    qDebug () << __PRETTY_FUNCTION__ << event << event->pos();
    QWidget::dragEnterEvent (event);
}

KKSMap<int, KKSAGroup *> KKSAttributesEditor :: getAvailableGroups (void) const
{
    return attrsGroups;
}

void KKSAttributesEditor :: addAGroup (void)
{
    QAbstractItemModel * aModel = recW->getSourceModel ();
    emit insertAttrGroup (aModel, this);
}

void KKSAttributesEditor :: editAGroup (void)
{
    QAbstractItemModel * aModel = recW->getSourceModel ();
    QModelIndex wIndex = recW->getSourceIndex ();
    wIndex = wIndex.sibling (wIndex.row(), 0);
    if (!wIndex.isValid())
    {
        QMessageBox::warning(this, tr("Edit attributes group"), tr ("Select group of attributes."), QMessageBox::Ok);
        return;
    }
    if (wIndex.data(Qt::UserRole+USER_ENTITY).toInt() > 0)
        wIndex = wIndex.parent();
    int idAttrGroup = wIndex.data(Qt::UserRole).toInt();
    emit updateAttrGroup (idAttrGroup, aModel, wIndex, this);
}

void KKSAttributesEditor :: delAGroup (void)
{
    QAbstractItemModel * aModel = recW->getSourceModel ();
    QModelIndex wIndex = recW->getSourceIndex ();
    if (!wIndex.isValid())
    {
        QMessageBox::warning(this, tr("Delete attributes group"), tr ("Select group of attributes."), QMessageBox::Ok);
        return;
    }
    if (wIndex.data(Qt::UserRole+USER_ENTITY).toInt() > 0 || aModel->hasChildren(wIndex))
    {
        QMessageBox::warning(this, tr("Delete group"), tr ("You cannot delete non empty group."), QMessageBox::Ok);
        return;
    }
    int idAttrGroup = wIndex.data(Qt::UserRole).toInt();
    if (QMessageBox::question(this, tr ("Delete group"), tr ("Do you really want to delete ?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        emit deleteAttrGroup (idAttrGroup, aModel, wIndex, this);
}
