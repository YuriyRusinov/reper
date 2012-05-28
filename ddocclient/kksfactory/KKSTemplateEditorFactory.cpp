/***********************************************************************
 * Module:  KKSTemplateEditorFactory.h
 * Author:  yuriy
 * Modified: 13 марта 2009 г. 16:15:50
 * Purpose: Declaration of the class KKSTemplateEditorFactory
 * Comment: данный класс является фабрикой редакторов шаблонов
 ***********************************************************************/

#include <QObject>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QAbstractProxyModel>
#include <QMessageBox>
#include <QTreeView>
#include <QInputDialog>
#include <QtDebug>

#include "KKSPPFactory.h"
#include "KKSViewFactory.h"
#include "KKSLoader.h"
#include "KKSEIOData.h"
#include <KKSCategory.h>
#include <KKSCategoryTemplateWidget.h>
#include "KKSTemplateEditorFactory.h"
#include <KKSTemplateEditor.h>
#include "KKSCatEditorFactory.h"
#include <KKSCatEditor.h>
#include <KKSAttributesEditor.h>
#include <KKSItemDelegate.h>
#include <kkscatattreditor.h>
#include <defines.h>
#include <KKSObject.h>

KKSTemplateEditorFactory :: KKSTemplateEditorFactory (KKSLoader *l, KKSPPFactory *_ppf, QObject *parent)
    : KKSEntityFactory (parent),
    loader (l),
    ppf (_ppf),
    catf (0)
{
}

KKSTemplateEditorFactory :: ~KKSTemplateEditorFactory (void)
{
    catf = 0;
}

void KKSTemplateEditorFactory :: setCatEditorFactory (KKSCatEditorFactory *cf)
{
    catf = cf;
}

KKSCatEditorFactory *KKSTemplateEditorFactory :: getCatEditorFactory (void) const
{
    return catf;
}

KKSTemplateEditor *KKSTemplateEditorFactory :: createTemplateEditor (KKSTemplate *t,
                                                                     bool mode,
                                                                     Qt::WindowModality windowModality,
                                                                     QWidget *parent,
                                                                     Qt::WindowFlags f)
{
    if (!t)
        return 0;

    KKSCategory *c = t->category ();
    if (!c)
        return 0;

    KKSTemplateEditor *tEditor = new KKSTemplateEditor (t, mode, parent, f);
    if (!tEditor)
        return 0;

    QStandardItemModel *tGroupModel = new QStandardItemModel (0, 3);
    tGroupModel->setHeaderData (0, Qt::Horizontal, tr ("Attribute groups"));
    tGroupModel->setHeaderData (1, Qt::Horizontal, tr ("Default value"));
    tGroupModel->setHeaderData (2, Qt::Horizontal, tr ("Read only"));
    tEditor->setModel (tGroupModel);
    KKSItemDelegate *itemDeleg = new KKSItemDelegate (tEditor);
    tEditor->setItemDelegate (itemDeleg);
    KKSViewFactory::initTemplateGroups (t, tGroupModel);
    tEditor->setWindowModality (windowModality);
    tEditor->setWindowTitle (tr("New template for %1").arg (c->name()));

    this->initTemplateConnections (tEditor);
    if (mode)
        emit templateEditorCreatedModal (tEditor);
    else
        emit templateEditorCreated (tEditor);

    return tEditor;
}

void KKSTemplateEditorFactory :: initTemplateConnections (KKSTemplateEditor *tEditor)
{
    connect (tEditor, SIGNAL (saveTemplate (KKSTemplate *, KKSTemplateEditor *)),
             this,
             SLOT (saveTemplate (KKSTemplate *, KKSTemplateEditor *))
             );

    connect (tEditor, SIGNAL (addAttrGroup (KKSTemplate *, const QModelIndex&, KKSTemplateEditor *)),
            this,
            SLOT (addAGroupToTemplate (KKSTemplate *, const QModelIndex&, KKSTemplateEditor *))
            );

    connect (tEditor, SIGNAL (editAttrGroup (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *)),
            this,
            SLOT (editAGroupInTemplate (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *))
            );

    connect (tEditor, SIGNAL (delAttrGroup (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *)),
            this,
            SLOT (delAGroupInTemplate (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *))
            );

    connect (tEditor, SIGNAL (addAttrIntoGroup (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *)),
            this,
            SLOT (addAttrToTemplate (int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *))
            );

    connect (tEditor, SIGNAL (appendAttrIntoGroup (int, int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *)),
            this,
            SLOT (appendAttrToTemplate (int, int, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *))
            );
/*
    connect (tEditor, SIGNAL (pushAttrIntoGroup (int, int, KKSTemplate *, KKSTemplateEditor *)),
            this,
            SLOT (pushAttrToTemplate (int, int, KKSTemplate *, KKSTemplateEditor *))
            );
*/
    connect (tEditor, SIGNAL (editAttrInGroup (int, int, const QModelIndex&, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *)),
            this,
            SLOT (editAttrInTemplate (int, int, const QModelIndex&, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *))
            );

    connect (tEditor, SIGNAL (delAttrFromGroup (int, int, const QModelIndex&, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *)),
            this,
            SLOT (deleteAttrFromTemplate (int, int, const QModelIndex&, const QModelIndex&, KKSTemplate *, KKSTemplateEditor *))
            );

    connect (tEditor, SIGNAL (updateAttrGroups (KKSTemplate *, KKSTemplateEditor *)),
            this,
            SLOT (updateTemplate (KKSTemplate *, KKSTemplateEditor *))
            );
    
    connect (this, SIGNAL (templateDbError()), tEditor, SLOT (saveErr()) );
    connect (this, SIGNAL (saveSuccess()), tEditor, SLOT (dropErr()) );
}

KKSTemplate * KKSTemplateEditorFactory :: initTemplate (int idTemplate, QString name, KKSCategory *c)
{
    if (!c && idTemplate <= 0)
        return 0;

    KKSTemplate *t = (idTemplate <= 0 ? new KKSTemplate (idTemplate, name, c) : loader->loadTemplate (idTemplate));
    if (!t)
        return 0;

    qDebug () << __PRETTY_FUNCTION__ << t->groups().count();
    if (idTemplate > 0)
        return t;

    KKSAttrGroup *tAttrG = new KKSAttrGroup ();
    if (!tAttrG)
        return 0;

    QString tgrName = tr("Group1");

    tAttrG->setName (tgrName);

    tAttrG->setOrder (0);
    KKSMap<int, KKSCategoryAttr *> cAttrs = c->attributes ();
    KKSMap<int, KKSCategoryAttr *>::const_iterator pa;
    int i=0;
    for (pa=cAttrs.constBegin(); pa != cAttrs.constEnd(); pa++)
    {
        KKSAttrView *va = new KKSAttrView (*(pa.value()));
        if (!va)
            continue;
        va->setOrder (i);
        tAttrG->addAttrView (pa.key(), va);
        va->release ();
        i++;
    }
    t->addGroup (tAttrG);
    tAttrG->release ();
    return t;
}

void KKSTemplateEditorFactory :: addTemplate (QWidget *ctw, int idCat, QAbstractItemModel *templMod)
{
    KKSCategory *c = loader->loadCategory (idCat);
    if (!c)
        return;

    int id = -1;
    QString name = QString();
    KKSTemplate * t = this->initTemplate (id, name, c);
    QWidget *parent = ctw;
    bool mode = (ctw->windowModality () != Qt::NonModal);
    Qt::WindowModality cwModal = ctw->windowModality();
    bool isCat (qobject_cast<KKSCatEditor *>(this->sender()));
    if (isCat)
    {
        //
        // Вызов пришел от редактора категорий
        //
        parent = qobject_cast<KKSCatEditor *>(this->sender());
        mode = true;
        cwModal = Qt::WindowModal;
    }
    KKSTemplateEditor *tEditor = this->createTemplateEditor (t, mode, cwModal, parent, Qt::Dialog);
    if (!tEditor)
    {
        if (t)
            t->release ();

        if (c)
            c->release ();

        QMessageBox::warning (ctw, tr ("Template editor"), tr ("Cannot create template editor"), QMessageBox::Ok);
    }

    if (t)
        t->release ();

    if (c)
        c->release ();

    if (isCat && mode && tEditor->exec () == QDialog::Accepted)
    {
        QModelIndex tIndex = KKSViewFactory::searchModelIndex (templMod, t->id(), QModelIndex(), Qt::UserRole);
        if (!tIndex.isValid())
        {
            templMod->insertRows (templMod->rowCount(), 1);
            tIndex = templMod->index (templMod->rowCount()-1, 0);
        }
        KKSTemplate * t = loader->loadTemplate (tEditor->getTemplateID());
        templMod->setData (tIndex, t->id(), Qt::UserRole);
        templMod->setData (tIndex, t->name(), Qt::DisplayRole);
    }
    else if (!isCat && mode)
        tEditor->show ();
}

void KKSTemplateEditorFactory :: editTemplate (QWidget *ctw, int idTempl, QAbstractItemModel * templMod, const QModelIndex& tIndex)
{
    KKSTemplate * t = this->initTemplate (idTempl);
    QWidget *parent = ctw;
    bool mode = (ctw->windowModality () != Qt::NonModal);
    Qt::WindowModality cwModal = ctw->windowModality();
    if (qobject_cast<KKSCatEditor *>(this->sender()))
    {
        //
        // Вызов пришел от редактора категорий
        //
        parent = qobject_cast<KKSCatEditor *>(this->sender());
        mode = true;
        cwModal = Qt::WindowModal;
    }
    KKSTemplateEditor *tEditor = this->createTemplateEditor (t, mode, cwModal, parent, Qt::Dialog);
    if (!tEditor)
    {
        if (t)
            t->release ();

        QMessageBox::warning (ctw, tr ("Template editor"), tr ("Cannot create template editor"), QMessageBox::Ok);
        return;
    }

    if (t)
        t->release ();

    if (qobject_cast<KKSCatEditor *>(this->sender()) && mode && tEditor->exec() == QDialog::Accepted)
    {
        KKSTemplate * t = loader->loadTemplate (tEditor->getTemplateID());
        templMod->setData (tIndex, t->id(), Qt::UserRole);
        templMod->setData (tIndex, t->name(), Qt::DisplayRole);
    }
    else if (!qobject_cast<KKSCatEditor *>(this->sender()) && mode)
        tEditor->show ();
}

void KKSTemplateEditorFactory :: delTemplate (QWidget *ctw, int idTempl, QAbstractItemModel * templMod, const QModelIndex& tIndex)
{
    KKSTemplate *t = loader->loadTemplate (idTempl);
    if (!t)
        return;

    int res = ppf->deleteTemplate (t);
    if (res == ERROR_CODE)
    {
        QMessageBox::warning (ctw, tr ("Template editor"), tr ("Cannot remove selected template"), QMessageBox::Ok);
        return;
    }
    if (qobject_cast<KKSCatEditor *>(this->sender()))
    {
        //
        // Вызов пришел от редактора категорий
        //
        int trow (tIndex.row());
        QModelIndex pInd (tIndex.parent());
        templMod->removeRows (trow, 1, pInd);
/*        KKSCatEditor *cEditor = qobject_cast<KKSCatEditor *>(this->sender());
        QAbstractItemModel * sortTemplModel = 0;
        QAbstractItemModel * tModel = 0;
        QModelIndex wIndex = QModelIndex ();
        sortTemplModel = cEditor->getTemplateModel ();
        tModel = sortTemplModel;
        if (qobject_cast<QAbstractProxyModel *>(sortTemplModel))
            tModel = (qobject_cast<QAbstractProxyModel *>(sortTemplModel))->sourceModel ();
        wIndex = qobject_cast<QAbstractProxyModel *>(sortTemplModel) ? qobject_cast<QAbstractProxyModel *>(sortTemplModel)->mapToSource (cEditor->getSelectedTemplateIndex ()) : cEditor->getSelectedTemplateIndex ();
        tModel->removeRows (wIndex.row(), 1);*/
    }
//    QModelIndex tInd = ctw->tvCatTemplate->selectionModel()->currentIndex ();
//    ctw->tvCatTemplate->model()->removeRows (tInd.row(), 1, tInd.parent());
    //emit templateDeleted (t);

    t->release ();
}

void KKSTemplateEditorFactory :: addAGroupToTemplate (KKSTemplate *t, const QModelIndex& parent, KKSTemplateEditor *tEditor)
{
    if (!t || !tEditor || !tEditor->tvGroups || !tEditor->tvGroups->model ())
        return;

    QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
    KKSAttrGroup *tAttrG = new KKSAttrGroup ();
    if (!tAttrG)
        return;

    KKSAttrGroup * pAttrG = 0;
    if (parent.isValid())
    {
        qDebug () << __PRETTY_FUNCTION__ << parent;
        //KKSMap<int, KKSAttrgroup*>::const_iterator pg = t->
        pAttrG = t->searchGroupById (parent.data(Qt::UserRole).toInt());//getParentGroup (parent, t);
        if (!pAttrG)
            return;
        tAttrG->setParent (pAttrG);
    }


    bool ok;
    QString tgrName = QInputDialog::getText (tEditor, tr("Set title to new group"), tr("Name:"), QLineEdit::Normal, tr("Group%1").arg(t->groups().count()+1), &ok);
    if (!ok)
        return;
    if (ok && !tgrName.isEmpty())
        tAttrG->setName (tgrName);

    int ires;
    QList<int> idList = t->groupKeys();//pAttrG->childGroups().keys();
//    KKSMap<int, KKSAttrGroup*>::const_iterator pMin = pAttrG->childGroups().constBegin();
    int id = -1;
    while (idList.contains (id))
        id--;
    if (parent.isValid())
    {
        tAttrG->setOrder (pAttrG->childGroups().count());
        tAttrG->setId (id);
        ires=pAttrG->addChildGroup (id, tAttrG);
        qDebug () << __PRETTY_FUNCTION__ << pAttrG->childGroups().keys();
    }
    else
    {
        tAttrG->setOrder (t->groups().count());
/*        QList<int> idList = t->groups().keys();
        KKSMap<int, KKSAttrGroup*>::const_iterator pMin = t->groups().constBegin();
        int id = -1;//idList.isEmpty() ? 0 : idList[idList.size()-1]+1;
        if (pMin != t->groups().constEnd() && pMin.key() <= 0)
            id = pMin.key()-1;
*/
        tAttrG->setId (id);
        t->addGroup (tAttrG);
    }

    qDebug () << __PRETTY_FUNCTION__ << t->groups().keys() << id << idList << tAttrG->id();
    KKSViewFactory::insertTemplateGroup (tAttrG, parent, tGroupsModel->rowCount (parent)/*t->groups().count()-1*/, tGroupsModel);
    tAttrG->release ();
}

void KKSTemplateEditorFactory :: editAGroupInTemplate (int idAttrGroup, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor)
{
    qDebug () << __PRETTY_FUNCTION__ << idAttrGroup;
    if (!t || !tEditor || !tEditor->tvGroups ||  !tEditor->tvGroups->model () /*|| idAttrGroup < 0 || idAttrGroup >= t->groups().count()*/)
        return;

    QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
    KKSAttrGroup *tAttrG = t->searchGroupById (idAttrGroup);
    if (!tAttrG)
        return;

    bool ok;
    QString tgrName = QInputDialog::getText (tEditor, tr("Set group title"), tr("Name:"), QLineEdit::Normal, tAttrG->name(), &ok);
    if (ok && !tgrName.isEmpty())
        tAttrG->setName (tgrName);
    KKSViewFactory::updateTemplateGroup (tAttrG, gIndex, tGroupsModel);// tAttrG->order(), 
}

void KKSTemplateEditorFactory :: delAGroupInTemplate (int idAttrGroup, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor)
{
     qDebug () << __PRETTY_FUNCTION__ << idAttrGroup;
     if (!t || !tEditor || !tEditor->tvGroups ||  !tEditor->tvGroups->model () /*|| idAttrGroup < 0 || idAttrGroup >= t->groups().count()*/)
        return;

    QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
    if (gIndex.parent().isValid())
    {
        KKSAttrGroup * pGroup = t->searchGroupById (gIndex.parent().data(Qt::UserRole).toInt());
        pGroup->removeChildGroup (idAttrGroup);
    }
    else
        t->removeGroup (idAttrGroup);

    tGroupsModel->removeRows (gIndex.row(), 1, gIndex.parent());//(idAttrGroup, 1);
}

void KKSTemplateEditorFactory :: addAttrToTemplate (int idAttrGroup, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor)
{
    if (!t || !tEditor || !tEditor->tvGroups || !tEditor->tvGroups->model () || !gIndex.isValid())
        return;

    const KKSMap<int, KKSCategoryAttr *> avAttrs = t->availableAttrs();
    if (avAttrs.isEmpty())
    {
        QMessageBox::warning (tEditor, tr("Add attributes into template"), tr("No available attributes"), QMessageBox::Ok);
        return;
    }
    KKSAttributesEditor *aEditor = KKSViewFactory::createAvailAttrView (avAttrs, tEditor, Qt::Dialog);

    if (aEditor->exec () != QDialog::Accepted)
        return;

    int idAttr = aEditor->getAttrId ();
    if (idAttr < 0)
        return;

    this->appendAttrToTemplate (idAttr, idAttrGroup, gIndex, t, tEditor);
}

void KKSTemplateEditorFactory :: appendAttrToTemplate (int idAttr, int idAttrGroup, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor)
{
    if (!t || !tEditor || !tEditor->tvGroups || !tEditor->tvGroups->model ())
        return;

    const KKSCategoryAttr *cAttr = t->category()->attribute (idAttr);
    KKSAttrView *va = new KKSAttrView (*cAttr);
    if (!va)
        return;

    qDebug () << __PRETTY_FUNCTION__ << t->groups().keys() << gIndex << gIndex.parent() << gIndex.data (Qt::UserRole).toInt();
    KKSAttrGroup *gAttr = t->searchGroupById (idAttrGroup);//group (idAttrGroup);
    if (!gAttr)
        return;

    if (gAttr->parent())
        qDebug () << __PRETTY_FUNCTION__ << gAttr->parent()->id();
    va->setOrder (gAttr->attrViews().count());

    gAttr->addAttrView (va->id(), va);
    gAttr->addRef ();
    if (gAttr->parent())
    {
        qDebug () << __PRETTY_FUNCTION__ << gAttr->parent()->id();
        KKSAttrGroup * gAttrP = gAttr->parent();
        gAttrP->removeChildGroup (idAttrGroup);
        qDebug () <<  __PRETTY_FUNCTION__ << gAttr->id();
        gAttrP->addChildGroup (idAttrGroup, gAttr);
    }
    else
    {
        t->removeGroup (idAttrGroup, true);
        t->addGroup (gAttr);
    }
    QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
    int nr = tGroupsModel->rowCount (gIndex);
    int iLastAttr = 0;
    for (int i=0; i<nr; i++)
    {
        QModelIndex wIndex = tGroupsModel->index (i, 0, gIndex);
        if (wIndex.isValid() && wIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
            iLastAttr = i;
    }
    qDebug () << __PRETTY_FUNCTION__ << iLastAttr;
    if (tGroupsModel->rowCount (gIndex) != 0 && tGroupsModel->index (0, 0, gIndex).data (Qt::UserRole+USER_ENTITY).toInt ()==0)
        iLastAttr++;
    bool is_inserted = tGroupsModel->insertRows (iLastAttr, 1, gIndex);
    QModelIndex aNewIndex = tGroupsModel->index (iLastAttr, 0, gIndex);
    qDebug () << __PRETTY_FUNCTION__ << iLastAttr << is_inserted;
    tGroupsModel->setData (aNewIndex, va->title(), Qt::DisplayRole);
    tGroupsModel->setData (aNewIndex, va->id(), Qt::UserRole);
    tGroupsModel->setData (aNewIndex, 0, Qt::UserRole+USER_ENTITY);

    aNewIndex = aNewIndex.sibling (aNewIndex.row(), 1);
    tGroupsModel->setData (aNewIndex, va->defValue().valueVariant(), Qt::DisplayRole);
    tGroupsModel->setData (aNewIndex, va->defValue().value(), Qt::UserRole);

    aNewIndex = aNewIndex.sibling (aNewIndex.row(), 2);
    tGroupsModel->setData (aNewIndex, va->isReadOnly() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
    tGroupsModel->setData (aNewIndex, va->isReadOnly(), Qt::UserRole);
    //KKSViewFactory::updateTemplateGroup (gAttr, gIndex, iLastAttr, tGroupsModel);

    if (va)
        va->release ();
}

/*
void KKSTemplateEditorFactory :: pushAttrToTemplate (int idAttr, int idAttrGroup, KKSTemplate *t, KKSTemplateEditor *tEditor)
{
    if (!t || !tEditor || !tEditor->tvGroups || !tEditor->tvGroups->model ())
        return;

    const KKSCategoryAttr *cAttr = t->category()->attribute (idAttr);
    KKSAttrView *va = new KKSAttrView (*cAttr);
    if (!va)
        return;

    KKSAttrGroup *gAttr = t->group (idAttrGroup);
    if (!gAttr)
        return;

    KKSMap<int, KKSAttrView *> aViews = gAttr->attrViews();
    if (aViews.count() == 0)
    {
        va->setOrder (0);
        gAttr->addAttrView (va->id(), va);
        QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
//        KKSViewFactory::updateTemplateGroup (gAttr, QModelIndex(), idAttrGroup, tGroupsModel);
        va->release ();
        return;
    }
    KKSMap<int, KKSAttrView *>::const_iterator pa;
//    KKSAttrView *aVFirst = aViews.constBegin().value();
//    int order = aVFirst->order();
    va->setOrder (0);
    for (pa=aViews.constBegin(); pa != aViews.constEnd(); pa++)
    {
        int order = pa.value()->order();
        pa.value()->setOrder (order+1);
    }

    gAttr->addAttrView (va->id(), va);
    QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
    //KKSViewFactory::updateTemplateGroup (gAttr, QModelIndex(), idAttrGroup, tGroupsModel);
    if (va)
        va->release ();
}
*/
void KKSTemplateEditorFactory :: editAttrInTemplate (int idAttr, int idAttrGroup, const QModelIndex& aIndex, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor)
{
    if (!t || !tEditor || !tEditor->tvGroups || !tEditor->tvGroups->model () || !aIndex.isValid() || !gIndex.isValid())
        return;

    KKSAttrGroup *gAttr = t->searchGroupById (idAttrGroup);
    if (!gAttr)
        return;

    //KKSMap<int, KKSAttrGroup *> gList = t->groups ();

    KKSCategoryAttr * cav = gAttr->attrView (idAttr);
    KKSAttrView * av = gAttr->attrView (idAttr);
    if (!av)
        return;
    cav->setDefValue (av->defValue());
    qDebug () << __PRETTY_FUNCTION__ << idAttr << cav->defValue().value();

    KKSObject *attrTypesIO = loader->loadIO (IO_ATTR_TYPE_ID, true);
    if (!attrTypesIO)
    {
        QMessageBox::warning (tEditor, QObject::tr ("Attributes types"), QObject::tr ("Cannot load attribute types"), QMessageBox::Ok);
        return;
    }

    KKSMap<int, KKSAttrType*> availAttrTypes;
    KKSMap<qint64, KKSEIOData *> attrTypesList = loader->loadEIOList (attrTypesIO);
    KKSMap<qint64, KKSEIOData *>::const_iterator pAttrs;
    for (pAttrs = attrTypesList.constBegin(); pAttrs != attrTypesList.constEnd(); pAttrs++)
    {
        KKSAttrType *aType = new KKSAttrType ();
        if (!aType)
            continue;
        aType->setId (pAttrs.key());
        aType->setName (pAttrs.value()->fields ().value ("name"));
        aType->setCode (pAttrs.value()->fields ().value ("code"));
        availAttrTypes.insert (pAttrs.key(), aType);
        if (aType)
            aType->release ();
    }

    KKSCatAttrEditor *aEditor = new KKSCatAttrEditor (cav, availAttrTypes, true, tEditor);
    QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
    if (aEditor->exec() == QDialog::Accepted)
    {
        av->setDefValue (aEditor->getCatAttribute()->defValue());
        gAttr->replaceAttrView (av->id(), av);
        tGroupsModel->setData (aIndex, av->title(), Qt::DisplayRole);
        tGroupsModel->setData (aIndex, av->id(), Qt::UserRole);
        tGroupsModel->setData (aIndex, 0, Qt::UserRole+USER_ENTITY);

        QModelIndex aNewIndex = aIndex.sibling (aIndex.row(), 1);
        tGroupsModel->setData (aNewIndex, av->defValue().valueVariant(), Qt::DisplayRole);
        tGroupsModel->setData (aNewIndex, av->defValue().value(), Qt::UserRole);

        aNewIndex = aIndex.sibling (aIndex.row(), 2);
        tGroupsModel->setData (aNewIndex, av->isReadOnly() ? QObject::tr("Yes") : QObject::tr ("No"), Qt::DisplayRole);
        tGroupsModel->setData (aNewIndex, av->isReadOnly(), Qt::UserRole);
    }

    attrTypesIO->release();
}

void KKSTemplateEditorFactory :: deleteAttrFromTemplate (int idAttr, int idAttrGroup, const QModelIndex& aIndex, const QModelIndex& gIndex, KKSTemplate *t, KKSTemplateEditor *tEditor)
{
     if (!t || !tEditor || !tEditor->tvGroups ||  !tEditor->tvGroups->model () || !aIndex.isValid() || !gIndex.isValid())
         return;

    QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
    KKSAttrGroup *tAttrG = t->searchGroupById (idAttrGroup);
    if (!tAttrG)
        return;

    KKSMap<int, KKSAttrView *> aViews = tAttrG->attrViews();
    KKSMap<int, KKSAttrView *>::const_iterator pa = aViews.constFind (idAttr);
    if (pa == aViews.constEnd())
        return;
//    QList <KKSAttrView *> avList = aViews.values();
//    qSort (avList.begin(), avList.end(), compareAttrViews);

//    for (int i=avList.indexOf (pa.value())+1; i<avList.count(); i++)
//        avList[i]->setOrder (avList[i]->order()-1);

    KKSMap<int, KKSAttrView *>::const_iterator pAttr = pa;
    pAttr++;
    for (; pAttr != aViews.constEnd(); pAttr++)
        pAttr.value()->setOrder (pAttr.value()->order()-1);
    aViews.remove (idAttr);
    tAttrG->setAttrViews (aViews);
    //removeAttrView (idAttr);
    tGroupsModel->removeRows (aIndex.row(), 1, gIndex);
    qDebug () << __PRETTY_FUNCTION__ << t->searchGroupById (idAttrGroup)->attrViews().count() << tAttrG->attrViews().count();
//    KKSViewFactory::updateTemplateGroup (tAttrG, gIndex, tGroupsModel);//tAttrG->order(), 
}

void KKSTemplateEditorFactory :: saveTemplate (KKSTemplate *t, KKSTemplateEditor *tEditor)
{
    if (!t)
        return;

    int res;
    KKSCatEditor *cEditor = 0;
    QAbstractItemModel * sortTemplModel = 0;
    QAbstractItemModel * tModel = 0;
    QModelIndex wIndex = QModelIndex ();
    if (qobject_cast<KKSCatEditor *>(tEditor->parent()))
    {
        cEditor = qobject_cast<KKSCatEditor *>(tEditor->parent());
        sortTemplModel = cEditor->getTemplateModel ();
        tModel = sortTemplModel;
        while (qobject_cast<QAbstractProxyModel *>(tModel))
            tModel = (qobject_cast<QAbstractProxyModel *>(tModel))->sourceModel ();
    }

    qDebug () << __PRETTY_FUNCTION__ << t->groups().count();
    for (KKSMap<int, KKSAttrGroup *>::const_iterator pg=t->groups().constBegin(); \
         pg != t->groups().constEnd(); \
         pg++)
    {
        qDebug () << __PRETTY_FUNCTION__ << pg.value()->id() << pg.value()->attrViews ().count() << pg.value()->attrViews ().keys();
        KKSAttrGroup *g = pg.value();
        for (KKSMap<int, KKSAttrView *>::const_iterator pa=g->attrViews().constBegin(); \
                pa != g->attrViews().constEnd(); \
                pa++)
        {
            KKSAttrView * av = pa.value ();
            if (!av)
                continue;
            qDebug () << __PRETTY_FUNCTION__ << av->defValue().valueForInsert() << av->defValue().valueVariant();
        }
    }
    if (t->id() < 0)
    {
        res = ppf->insertTemplate (t);
        if (res != ERROR_CODE && cEditor)
        {
            tModel->insertRows (0, 1);
            wIndex = tModel->index (0, 0);
        }
    }
    else
    {
        res = ppf->updateTemplate (t);
        if (res != ERROR_CODE && cEditor)
            wIndex = qobject_cast<QAbstractProxyModel *>(sortTemplModel) ? qobject_cast<QAbstractProxyModel *>(sortTemplModel)->mapToSource (cEditor->getSelectedTemplateIndex ()) : cEditor->getSelectedTemplateIndex ();
    }

    qDebug () << __PRETTY_FUNCTION__ << wIndex << t->name() << t->id();
    if (res == ERROR_CODE)
    {
        emit templateDbError();
        QMessageBox::warning (tEditor, tr("Save template"), tr("Cannot save template"));
        return;
    }
    else if (cEditor && wIndex.isValid ())
    {
        tModel->setData (wIndex, t->name(), Qt::DisplayRole);
        tModel->setData (wIndex, t->id(), Qt::UserRole);
        tModel->setData (wIndex, 0, Qt::UserRole+USER_ENTITY);
        cEditor->getTemplView()->update ();
    }

    if (tEditor)
    {
        tEditor->ID = t->id();
        tEditor->lEID->setText (QString::number (t->id()));
    }

    emit templateAdded (t);
    emit saveSuccess ();
}

void KKSTemplateEditorFactory :: updateTemplate (KKSTemplate *t, KKSTemplateEditor *tEditor)
{
    if (!t || !tEditor || !tEditor->tvGroups || !tEditor->tvGroups->model ())
        return;

    QAbstractItemModel *tGroupsModel = tEditor->tvGroups->model ();
    KKSViewFactory::initTemplateGroups (t, tGroupsModel);
}
