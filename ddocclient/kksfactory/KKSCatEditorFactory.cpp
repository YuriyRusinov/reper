/***********************************************************************
 * Module:  KKSCatEditorFactory.h
 * Author:  yuriy
 * Modified: 13 марта 2009 г. 16:15:50
 * Purpose: Declaration of the class KKSCatEditorFactory
 * Comment: данный класс является фабрикой редакторов категорий
 ***********************************************************************/

#include <QPushButton>
//#include <QToolButton>
#include <QAction>
#include <QModelIndex>
#include <QMessageBox>
#include <QComboBox>
#include <QLineEdit>
#include <QItemSelectionModel>
#include <QAbstractProxyModel>
#include <QItemSelection>
#include <QTreeView>
#include <QAction>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QtDebug>

#include "KKSObjEditorFactory.h"
#include "KKSPPFactory.h"
#include "KKSEIOFactory.h"
#include "KKSViewFactory.h"
#include "KKSLoader.h"
#include "KKSStuffFactory.h"
#include <KKSCategoryTemplateWidget.h>
#include "KKSTemplateEditorFactory.h"
#include <KKSCatEditor.h>
#include <KKSObjEditor.h>
#include <KKSCategory.h>
#include <KKSCategoryAttr.h>
#include <KKSRecWidget.h>
#include <KKSEIOData.h>
#include <KKSType.h>
#include <KKSObject.h>
#include <KKSAttributesEditor.h>
#include <KKSLifeCycle.h>
#include "KKSCatEditorFactory.h"
#include "KKSAttributesFactory.h"
#include "KKSConverter.h"
#include <KKSObjectExemplar.h>
#include <KKSAccessEntity.h>
#include <kksattreditor.h>
#include <kksfilterseditorform.h>
#include <KKSRubric.h>
#include <kksincludeswidget.h>
#include <KKSCatAttrsModel.h>
#include <KKSItemDelegate.h>
#include <defines.h>

////////////////////////////////////////////////////////////////////////
// Name:       KKSCatEditorFactory::KKSCatEditorFactory()
// Purpose:    Implementation of KKSCatEditorFactory::KKSCatEditorFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSCatEditorFactory :: KKSCatEditorFactory (KKSLoader *l, KKSObjEditorFactory * _objf, KKSPPFactory *_ppf, KKSEIOFactory *_eiof, KKSAttributesFactory * _attrf, KKSStuffFactory * _sf, QObject *parent)
    : KKSEntityFactory (parent),
    loader (l),
    objf (_objf),
    ppf (_ppf),
    eiof (_eiof),
    tf (0),
    attrf (_attrf),
    stuffF (_sf)
{
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSCatEditorFactory::~KKSCatEditorFactory()
// Purpose:    Implementation of KKSCatEditorFactory::~KKSCatEditorFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSCatEditorFactory :: ~KKSCatEditorFactory (void)
{
    tf = 0;
}

////////////////////////////////////////////////////////////////////////////////////
// Name:       KKSCatEditorFactory::~KKSCatEditorFactory()
// Purpose:    Association of KKSCatEditorFactory::
//                ~setTemplateEditorFactory (KKSTemplateEditorFactory *_tf)
// Return:     
///////////////////////////////////////////////////////////////////////////////////

void KKSCatEditorFactory :: setTemplateEditorFactory (KKSTemplateEditorFactory *_tf)
{
    tf = _tf;
}

KKSTemplateEditorFactory * KKSCatEditorFactory :: getTemplateEditorFactory (void) const
{
    return tf;
}

KKSCategoryTemplateWidget* KKSCatEditorFactory :: viewCategories (const KKSList<const KKSFilterGroup *> & filters,
                                                                  bool mode,
                                                                  Qt::WindowModality windowModality,
                                                                  QWidget *parent,
                                                                  Qt::WindowFlags f)
{
    bool asAdmin (loader->getUserId() == ADMIN_ROLE);

    KKSCategoryTemplateWidget *ctWidget = new KKSCategoryTemplateWidget (mode, asAdmin, parent, f);
    if (!ctWidget)
        return 0;
    
    KKSViewFactory::loadCategories (ctWidget, loader, filters);

    connect (ctWidget, SIGNAL (uploadTemplatesIntoCategory (KKSCategoryTemplateWidget *, int, const QModelIndex&)),
             this,
             SLOT (loadCatTemplates (KKSCategoryTemplateWidget *, int, const QModelIndex&))
             );

    connect (ctWidget, SIGNAL (addNewCategory (QWidget *, int, bool)),
             this,
             SLOT (addCategory (QWidget *, int, bool))
             );

    connect (ctWidget, SIGNAL (addCopyCategory (QWidget *, int, bool)),
             this,
             SLOT (addCopyCategory (QWidget *, int, bool))
             );

    connect (ctWidget, SIGNAL (editCategory(QWidget*, int, bool)),
             this,
             SLOT (editCategory (QWidget*, int, bool))
             );

    connect (ctWidget, SIGNAL (deleteCat (QWidget *, int)),
             this,
             SLOT (delCategory(QWidget *, int ))
             );

    connect (ctWidget, SIGNAL (addNewTemplate (QWidget *, int)),
             tf,
             SLOT (addTemplate (QWidget *, int))
             );

    connect (ctWidget, SIGNAL (editTempl (QWidget *, int)),
             tf,
             SLOT (editTemplate (QWidget *, int))
             );

    connect (ctWidget, SIGNAL (deleteT (QWidget *, int)),
             tf,
             SLOT (delTemplate (QWidget *, int))
             );

    connect (this, SIGNAL (categoryAdded (KKSCategory *)),
             ctWidget,
             SLOT (addCatRec (KKSCategory *))
             );

    connect (tf, SIGNAL (templateAdded (KKSTemplate *)),
             ctWidget,
             SLOT (uploadCatT (KKSTemplate *))
             );

    ctWidget->setWindowModality (windowModality);
    return ctWidget;
}

void KKSCatEditorFactory :: loadCatTemplates (KKSCategoryTemplateWidget *ctw, int idCat, const QModelIndex& ind)
{
    KKSViewFactory::loadCategoryTemplates (idCat, loader, ctw->getModel(), ind);
}

void KKSCatEditorFactory :: addCategory (QWidget *ctw, int idCatType, bool isChild)
{
    KKSList<const KKSFilterGroup *> filterGroups;
    bool mode = (ctw->windowModality() != Qt::NonModal);
    QWidget *parent = 0;//(isChild ? (KKSCatEditor *)this->sender() : 0);
    Qt::WindowModality cwModal = ctw->windowModality();
    if (isChild && qobject_cast<KKSCatEditor *>(this->sender()))
    {
        //
        // Вызов пришел от редактора категорий
        //
        parent = qobject_cast<KKSCatEditor *>(this->sender());
        mode = true;
        cwModal = Qt::WindowModal;
    }
    KKSCatEditor * catEditor = this->createCategoryEditor (-1, filterGroups, isChild, idCatType, mode, cwModal, parent, Qt::Dialog);
    if (!catEditor)
        return;

    catEditor->setWindowTitle (tr("Creation new category"));
    if (cwModal != Qt::NonModal)
        (qobject_cast<QWidget *>(catEditor))->setAttribute (Qt::WA_DeleteOnClose);

    catEditor->show ();
}

void KKSCatEditorFactory :: addCopyCategory (QWidget *ctw, int idCat, bool isChild)
{
    Q_UNUSED (isChild);
    KKSCategory *c = loader->loadCategory (idCat);
    if (!c)
    {
        QMessageBox::critical(ctw, tr("Error!"), tr("Cannot load the category!"), QMessageBox::Ok);
        return;
    }

    if (c->tableCategory())
    {
        KKSCategory * ct = c->tableCategory();
        ct->setId (-1);
        ct->setName(ct->name() + tr(" (copy)"));
        ct->setCode (QString());
        int res = ppf->insertCategory(ct);
        if (res == ERROR_CODE)
        {
            QMessageBox::critical(ctw, tr("Error!"), tr("Cannot create copy of the category"), QMessageBox::Ok);
            c->release();
            return;
        }

        emit categoryAdded(ct);
    }
    c->setId(-1);
    c->setName(c->name() + tr(" (copy)"));
    c->setCode(QString::null);

    int res = ppf->insertCategory(c);
    if(res == ERROR_CODE){
        QMessageBox::critical(ctw, tr("Error!"), tr("Cannot create copy of the category"), QMessageBox::Ok);
        c->release();
        return;
    }

    emit categoryAdded(c);
}

void KKSCatEditorFactory :: editCategory (QWidget* ctw, int idCat, bool isChild)
{
    KKSList<const KKSFilterGroup*> filterGroups;
    bool mode = (ctw->windowModality() != Qt::NonModal);
    QWidget *parent = 0;//(isChild ? (KKSCatEditor *)this->sender() : 0);
    Qt::WindowModality cwModal = ctw->windowModality();
    if (isChild && qobject_cast<KKSCatEditor *>(this->sender()))
    {
        //
        // Вызов пришел от редактора категорий
        //
        parent = qobject_cast<KKSCatEditor *>(this->sender());
        mode = true;
        cwModal = Qt::WindowModal;
    }
    KKSCategory *c = loader->loadCategory (idCat);
    KKSCatEditor *catEditor = this->createCategoryEditor (idCat, filterGroups, isChild, c->type()->id(), mode, cwModal, parent, Qt::Dialog);//, ctw, Qt::Dialog);
    if (c)
    {
        catEditor->setWindowTitle (tr("Edit category %1").arg(c->name()));
        c->release ();
    }

    if (!catEditor)
    {
        QMessageBox::warning (ctw, tr ("Category edition"), tr ("Cannot create category editor id = %1").arg (idCat), QMessageBox::Ok);
        return;
    }
    if (cwModal != Qt::NonModal)
        (qobject_cast<QWidget *>(catEditor))->setAttribute (Qt::WA_DeleteOnClose);

    if (mode)
        catEditor->show ();
}

void KKSCatEditorFactory :: delCategory (QWidget * ctw, int idCat)
{
    QModelIndex sIndex = QModelIndex ();
    QModelIndex pIndex = QModelIndex ();
    if (qobject_cast<KKSCategoryTemplateWidget *>(ctw))
        sIndex = (qobject_cast<KKSCategoryTemplateWidget *>(ctw))->getSelectedIndex ();
    int row = -1;
    if (sIndex.parent().isValid() && sIndex.parent().data(Qt::UserRole+USER_ENTITY).toInt () != 2)
    {
        pIndex = sIndex.parent().parent();
        row = sIndex.parent().row();
    }
    else
    {
        pIndex = sIndex.parent();
        row = sIndex.row ();
    }
    qDebug  () << __PRETTY_FUNCTION__ << pIndex << sIndex;

    KKSCategory *cat = loader->loadCategory (idCat);
    if (!cat)
        return;

    int res = ppf->deleteCategory (cat);
    if (res != OK_CODE)
    {
        QMessageBox::warning (ctw, tr ("Category remove"), tr ("Cannot remove category %1").arg (cat->name()), QMessageBox::Ok);
        cat->release ();
        return;
    }
    if (cat->tableCategory())
    {
        int tres = ppf->deleteCategory (cat->tableCategory());
        if (tres != OK_CODE)
        {
            QMessageBox::warning (ctw, tr ("Table Category remove"), tr ("Cannot remove category %1").arg (cat->tableCategory()->name()), QMessageBox::Ok);
            cat->release ();
            return;
        }
    }
    if (qobject_cast<KKSCategoryTemplateWidget *>(ctw))
        (qobject_cast<KKSCategoryTemplateWidget *>(ctw))->getModel()->removeRows (row, 1, pIndex);
    cat->release ();
}

KKSCatEditor* KKSCatEditorFactory :: createCategoryEditor (int idCategory, // идентификатор категории
                                                           const KKSList<const KKSFilterGroup *> & filters,
                                                           bool isChildCat,
                                                           int idCatType,
                                                           bool mode,
                                                           Qt::WindowModality windowModality,
                                                           QWidget *parent,
                                                           Qt::WindowFlags f)
{
    Q_UNUSED (filters);
    KKSCategory *cat (0);
    KKSCategory * tableCat (0);//= new KKSCategory(-1, QString(), cTableT);
    if (idCategory <= 0 && isChildCat)
    {
        cat = new KKSCategory ();
        KKSType *cType = new KKSType (10, tr ("Child category"));
        cType->setAsQualifier(false);
        if (!cType)
            return 0;
        cat->setType (cType);
        cType->release ();
    }
    else if (isChildCat && parent && qobject_cast<KKSCatEditor *>(parent))
    {
        KKSCategory * c = loader->loadCategory (idCategory);
        if (!c)
            return 0;
        cat = new KKSCategory (*c);
        cat->setId (-1);
        cat->setCode (QString());
        c->release ();
    }
    else if (idCategory > 0)
        cat = loader->loadCategory (idCategory);
    else
    {
        cat = new KKSCategory ();
        KKSType * ct = loader->loadType (idCatType);
        cat->setType (ct);
        //if (cat && ct && (idCatType == 1 || idCatType == 2 || idCatType == 12))// категории данных типов являются табличными
        if (cat && ct && ct->isQualifier())// категории данных типов являются табличными
        {
            KKSType * cTableT = loader->loadType (10);
            //
            // загрузка типа "подчиненная категория"
            //
            tableCat = new KKSCategory (-1, QString(), cTableT);
            cat->setTableCategory (tableCat);
            if (cTableT)
                cTableT->release ();
            if (tableCat)
                tableCat->release ();
        }
        
        if (ct)
            ct->release ();
        
        KKSAccessEntity * acl = new KKSAccessEntity ();
        cat->setAccessRules (acl);
        if (cat->tableCategory())
            cat->tableCategory()->setAccessRules (acl);
        acl->release ();
    }

    if (!cat)
        return 0;

    if (isChildCat && qobject_cast<KKSCatEditor *>(parent))
    {
        KKSCatEditor *pEditor = qobject_cast<KKSCatEditor *>(parent);
        pEditor->category()->setTableCategory (cat);
    }

    KKSRecWidget *rAttrCw = getAttrsWidget (cat, mode, 0);
    //cEditor->setAttrs (rw);
    KKSType * cTableT = loader->loadType (10);
    KKSRecWidget * rAttrTCw = getAttrsWidget (cat->tableCategory(), mode, 0);
    QAction * actCopyFrom = new QAction (QIcon (":/ddoc/add_copy.png"), tr("Copy attributes from"), rAttrTCw);
    rAttrTCw->addToolBarAction (actCopyFrom);
    KKSMap<int, KKSType*> cTypes = loader->loadAvailableTypes();

    KKSRecWidget *rwt = getTemplateWidget (cat, false, 0);//KKSViewFactory :: createCategoryTemplates (cat->id (), loader, 0);
//    cEditor->setTemplates (rwt);
    KKSRecWidget *rwtT = getTemplateWidget (cat->tableCategory(), false, 0);

    KKSCatEditor *cEditor = new KKSCatEditor (cat, rAttrCw, rAttrTCw, rwt, rwtT, cTypes, idCatType, mode, parent, f);
    if (!cEditor)
        return 0;
    connect (actCopyFrom, SIGNAL (triggered()), cEditor, SLOT (copyAttributesFrom()));

    cEditor->setTableType (cTableT);

    KKSObject *attrTypesIO = loader->loadIO (IO_ATTR_TYPE_ID, true);
    if (!attrTypesIO)
    {
        QMessageBox::warning (parent, QObject::tr ("Attributes types"), QObject::tr ("Cannot load attribute types"), QMessageBox::Ok);
        return 0;
    }

    KKSMap<int, KKSAttrType*> availAttrTypes;
    KKSMap<int, KKSEIOData *> attrTypesList = loader->loadEIOList (attrTypesIO);
    KKSMap<int, KKSEIOData *>::const_iterator pAttrs;
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

    cEditor->setAttrTypes (availAttrTypes);
    if (!isChildCat)
    {
        KKSStuffForm *sForm = stuffF->createStuffEditorForm (cat->getAccessRules(), 1, cEditor);
        cEditor->setAccessWidget (sForm);
    }

    connect (cEditor, SIGNAL (addAttrsIntoCat (KKSCategory *, QAbstractItemModel *, KKSCatEditor *)), this, SLOT (addAttributeIntoCategory (KKSCategory *, QAbstractItemModel *, KKSCatEditor *)) );
    connect (cEditor, SIGNAL (copyAttrsFromAnotherCat (KKSCategory *, QAbstractItemModel *, KKSCatEditor *)), this, SLOT (copyAttributesIntoCategory (KKSCategory *, QAbstractItemModel *, KKSCatEditor *)) );
    connect (cEditor, SIGNAL (setAttribute (int, KKSCategory*, QAbstractItemModel *, KKSCatEditor *)), this, SLOT (loadCatAttribute (int, KKSCategory*, QAbstractItemModel *, KKSCatEditor *)));
    connect (cEditor, SIGNAL (setAttribute (KKSCategoryAttr *, KKSCategory*, QAbstractItemModel *, KKSCatEditor *)), this, SLOT (loadCatAttribute (KKSCategoryAttr *, KKSCategory*, QAbstractItemModel *, KKSCatEditor *)) );
    connect (cEditor, SIGNAL (delAttrFromCategory (int, KKSCategory *, QAbstractItemModel *, KKSCatEditor *)), this, SLOT (delCatAttribute(int, KKSCategory *, QAbstractItemModel *, KKSCatEditor *)) );
    
    connect (cEditor, SIGNAL (saveCategory (KKSCategory *, int, int, KKSCatEditor *)), this, SLOT (saveCategory (KKSCategory *, int, int, KKSCatEditor *)) );
    connect (cEditor, SIGNAL (addChildCat (QWidget *, int, bool)), this, SLOT (addCategory (QWidget *, int, bool)) );
    connect (cEditor, SIGNAL (editChildCat (QWidget *, int, bool)), this, SLOT (editCategory (QWidget *, int, bool)) );
    
    connect (cEditor, SIGNAL (addNewCategoryTemplate (QWidget *, int, QAbstractItemModel *)), tf, SLOT (addTemplate (QWidget *, int, QAbstractItemModel *)) );
    connect (cEditor, SIGNAL (editCategoryTemplate (QWidget *, int, QAbstractItemModel *, const QModelIndex& )), tf, SLOT (editTemplate (QWidget *, int, QAbstractItemModel *, const QModelIndex&)) );
    connect (cEditor, SIGNAL (delCategoryTemplate (QWidget *, int, QAbstractItemModel *, const QModelIndex& )), tf, SLOT (delTemplate (QWidget *, int, QAbstractItemModel *, const QModelIndex&)) );
    
    connect (this, SIGNAL (categoryDbError ()), cEditor, SLOT (catDbError()) );

    cEditor->setWindowModality (windowModality);
    if (windowModality != Qt::NonModal)
    {
        cEditor->show ();
        emit categoryEditorCreatedModal (cEditor);
    }
    else
        emit categoryEditorCreated (cEditor);
//    typeRef->release ();
//    cat->release ();
    return cEditor;
}

KKSRecWidget * KKSCatEditorFactory :: getAttrsWidget (const KKSCategory *cat, bool mode, QWidget *parent) const
{
    //Q_UNUSED (mode);
    KKSRecWidget * rw (0);

    if (cat)//cat->type()->id() != 10)
        rw = KKSViewFactory :: createCategAttrsView (cat, parent);
    else
    {
        QTreeView * tvTableAttrs = new QTreeView ();
        rw = new KKSRecWidget (tvTableAttrs, mode);
        rw->hideGroup (0);//gbSearch->setVisible (false);
        rw->hideGroup (2);//tbSetView->setVisible (false);
        rw->hideGroup (3);//gbImportExport->setVisible (false);
        QAbstractItemModel * acModel = new KKSCatAttrsModel (0, 4);//QStandardItemModel (0, 4);
        acModel->setHeaderData (0, Qt::Horizontal, QObject::tr ("Name"));
        acModel->setHeaderData (1, Qt::Horizontal, QObject::tr ("Default value"));
        acModel->setHeaderData (2, Qt::Horizontal, QObject::tr ("Mandatory"));
        acModel->setHeaderData (3, Qt::Horizontal, QObject::tr ("Read only"));
        KKSViewFactory::updateAttrModel (0, acModel);
        tvTableAttrs->setModel (acModel);
        KKSItemDelegate *itemDeleg = new KKSItemDelegate (rw);
        tvTableAttrs->setItemDelegate (itemDeleg);
    }
    return rw;
}

KKSRecWidget * KKSCatEditorFactory :: getTemplateWidget (const KKSCategory * cat, bool mode, QWidget *parent) const
{
    KKSRecWidget * rtw (0);
    if (cat)
        rtw = KKSViewFactory :: createCategoryTemplates (cat->id (), loader, parent);
    else
    {
        QTreeView * tvTableTempl = new QTreeView ();
        rtw = new KKSRecWidget (tvTableTempl, mode);
        tvTableTempl->header()->setClickable (true);
        tvTableTempl->header()->setSortIndicatorShown (true);
        tvTableTempl->header()->setSortIndicator (0, Qt::AscendingOrder);
        tvTableTempl->setSortingEnabled (true);
        rtw->hideGroup (0);//gbSearch->setVisible (false);
        rtw->hideGroup (3);//tbSetView->setVisible (false);
        rtw->hideGroup (2);//gbImportExport->setVisible (false);
        KKSItemDelegate *itemDeleg = new KKSItemDelegate (rtw);
        tvTableTempl->setItemDelegate (itemDeleg);
        QSortFilterProxyModel *sortTemplModel = new QSortFilterProxyModel (rtw);
        if (!sortTemplModel)
            return rtw;

        sortTemplModel->setSortCaseSensitivity (Qt::CaseInsensitive);
        sortTemplModel->setDynamicSortFilter (true);
        tvTableTempl->setModel (sortTemplModel);
        QStandardItemModel * tModel = new QStandardItemModel (0, 1);
        sortTemplModel->setSourceModel (tModel);
    }
    return rtw;
}

void KKSCatEditorFactory :: loadCatAttribute (int id, KKSCategory *c, QAbstractItemModel * acModel, KKSCatEditor *editor)
{
    if (!editor || !c)
        return;
    KKSAttribute *attr = loader->loadAttribute (id);
    if (!attr)
        return;
    bool is_bad;
    KKSCategoryAttr *cAttr = KKSCategoryAttr::create (attr, false, false, QString(), &is_bad);
    if (is_bad)
    {
        if (cAttr)
            cAttr->release ();
        return;
    }
    loadCatAttribute (cAttr, c, acModel, editor);
    cAttr->release ();
}

void KKSCatEditorFactory :: addAttributeIntoCategory (KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *editor)
{
    if (!c || !attrModel)
        return;
    KKSList<const KKSFilterGroup *> filters;
    KKSAttributesEditor *aEditor = attrf->viewAttributes (filters, true, editor, Qt::Dialog);
    //KKSViewFactory::createAttrView (loader, filters, true, editor, Qt::Dialog);
    if (!aEditor)
        return;
    aEditor->setWindowModality (Qt::WindowModal);
    if (aEditor->exec() == QDialog::Accepted)
    {
        QList<int> idAttrsList = aEditor->getAttributesId();
        for (int i=0; i<idAttrsList.size(); i++)
        {
            KKSAttribute * attr = loader->loadAttribute (idAttrsList[i]);
            if (!attr)
                continue;
            bool is_bad;
            KKSCategoryAttr *cAttr = KKSCategoryAttr::create (attr, false, false, QString(), &is_bad);
            if (is_bad || !cAttr)
            {
                if (cAttr)
                    cAttr->release ();
                return;
            }
            if (!c->attributes().contains (cAttr->id()))
                c->addAttribute(cAttr->id(), cAttr);
            cAttr->release ();
            attr->release();
        }
        KKSViewFactory::updateAttrModel (c, attrModel);
    }
    editor->resize(editor->size() + QSize(1, 1));
    editor->update ();
    editor->resize(editor->size() - QSize(1, 1));
    editor->update ();
}

void KKSCatEditorFactory :: copyAttributesIntoCategory (KKSCategory *c, QAbstractItemModel * attrModel, KKSCatEditor *cEditor)
{
    if (!c || !attrModel)
        return;

    KKSObject * ioCat = loader->loadIO (IO_CAT_ID, true);
    if (!ioCat)
        return;
    KKSList<const KKSFilterGroup *> filters;
    KKSObjEditor * oEditor = objf->createObjRecEditor (IO_IO_ID,
                                                       IO_CAT_ID,
                                                       filters,
                                                       tr ("Select source category"),
                                                       ioCat->category(),
                                                       true, false,
                                                       Qt::WindowModal,
                                                       cEditor,
                                                       Qt::Dialog
                                                       );
    if (oEditor->exec() == QDialog::Accepted)
    {
        int idCat = oEditor->getID();
//        qDebug () << __PRETTY_FUNCTION__ << idCat;
        KKSCategory * cSourceCat = loader->loadCategory (idCat);
        if (!cSourceCat)
        {
            QMessageBox::warning (cEditor, tr ("Category editor"), tr ("Cannot load source category"), QMessageBox::Ok);
            ioCat->release ();
            return;
        }
        KKSMap<int, KKSCategoryAttr*>::const_iterator pa = cSourceCat->attributes().constBegin();
        for (; pa != cSourceCat->attributes().constEnd(); pa++)
        {
            if (!c->attributes().contains (pa.key()))
                c->addAttribute (pa.key(), pa.value());
        }

        cSourceCat->release ();
        KKSViewFactory::updateAttrModel (c, attrModel);
       
    }
    ioCat->release ();
}

void KKSCatEditorFactory :: loadCatAttribute (KKSCategoryAttr *cAttr, KKSCategory *c, QAbstractItemModel * acModel, KKSCatEditor *editor)
{
    if (!cAttr || !c || !acModel || !editor)
        return;

    KKSMap<int, KKSCategoryAttr *> existsAttrs = c->attributes();
    int idattr = -1;
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator p = existsAttrs.constBegin(); p!= existsAttrs.constEnd() && idattr < 0; p++)
    {
        qDebug () << __PRETTY_FUNCTION__ << p.value()->id() << cAttr->id () << cAttr->defValue().value() << c->id();
        if (p.value()->id() == cAttr->id())
            idattr = p.key ();
    }

    int res;
    if (idattr < 0)
        res = c->addAttribute (cAttr->id(), cAttr);
    else
    {
        cAttr->addRef();
        res = c->replaceAttribute (cAttr->id(), cAttr);
    }

    if (res != OK_CODE)
        return;

    KKSViewFactory::updateAttrModel (c, acModel);
    editor->update ();
}

void KKSCatEditorFactory :: delCatAttribute (int id, KKSCategory *c, QAbstractItemModel *attrModel, KKSCatEditor *editor)
{
    if (!editor || !c || id<0 || !c->attribute(id) || !attrModel)
        return;

    c->removeAttribute (id);
//     = editor->recWidget->getModel ();
    KKSViewFactory::updateAttrModel (c, attrModel);
}

void KKSCatEditorFactory :: saveCategory (KKSCategory *cat, int idTableCat, int idType, KKSCatEditor *cEditor)
{
    if (!cat)
        return;

    Q_UNUSED (idTableCat);
    Q_UNUSED (idType);
/*
    KKSCategory *tCat = loader->loadCategory (idTableCat);
    if (tCat)
    {
        cat->setTableCategory (tCat);
        cEditor->category()->setTableCategory (tCat);
    }
    
    KKSType *cType = loader->loadType (idType);
    if (!cType)
        return;
    cat->setType (cType);
    cEditor->category()->setType (cType);
    cType->release ();

    KKSLifeCycle * ls = KKSLifeCycle :: defLifeCycle();
    if (!ls)
        return;
    cat->setLifeCycle (ls);
    ls->release ();
*/
    int res;
    if (cat->id() <= 0)
    {
        res = ppf->insertCategory (cat);

        if (qobject_cast<KKSCatEditor *>(cEditor->parent()))
        {
            KKSCatEditor *pEditor = qobject_cast<KKSCatEditor *>(cEditor->parent());
            pEditor->updateChildCat (cat->id(), cat->name());
        }
    }
    else
        res = ppf->updateCategory (cat);

    if (res == ERROR_CODE)
    {
        emit categoryDbError ();
        QMessageBox::warning (cEditor, tr ("Category"), tr("Category is not saved"));
        return;
    }
    //if (qobject_cast<KKSCatEditor *>(cEditor->parent()) == 0)
    emit categoryAdded (cat);
    cEditor->lEID->setText (QString::number (cat->id()));
    cEditor->setWindowTitle (tr("Edit category %1").arg (cat->name()));
    //cat->release ();
}

KKSList<const KKSFilterGroup *> KKSCatEditorFactory :: viewMainCategories (void)
{
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;
    KKSObject * refCatObj = loader->loadIO (IO_CAT_ID, true);
    if (!refCatObj || !refCatObj->category() || !refCatObj->category()->tableCategory())
        return filterGroups;

    //KKSFilter * filter = refCatObj->category()->tableCategory()->createFilter("is_main", QString("true"), KKSFilter::foEq);
    KKSFilter * filter = refCatObj->category()->tableCategory()->createFilter(17, QString("true"), KKSFilter::foEq);
    if(!filter)
        return filterGroups;

    filters.append(filter);
    filter->release();

    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    refCatObj->release ();
    return filterGroups;
}

/*
void KKSCatEditorFactory :: loadAttrsRefs (KKSAttribute * attr, KKSAttrEditor * aEditor)
{
    QString val_ref = QString("select id from io_categories where id_io_category_type in (1,2,8,9)"); // ,2,8,9
    QString attrCode = "id_io_category";

    KKSList<const KKSFilterGroup *> filterGroupsRefs = KKSViewFactory::AttrRefsFilters (loader, attrCode, val_ref);
    QMap<int, QString> io_refs = KKSViewFactory::loadAttrRefs (loader, filterGroupsRefs);
    qDebug () << __PRETTY_FUNCTION__ << io_refs;

    int ind = -1;
    int i = 0;
    int idRef = -1;
    for (QMap<int, QString>::const_iterator pio = io_refs.constBegin(); pio != io_refs.constEnd() && ind < 0; pio++)
    {
        KKSObject *io = loader->loadIO (pio.key());
        if(!io)
            continue;

        if (io->tableName() == attr->tableName())
        {
            qDebug () << __PRETTY_FUNCTION__ << attr->tableName() << io->id() << i << pio.value() << pio.key();
            ind = i;
            idRef = pio.key();//io->id ();
            aEditor->setCurrentRef (pio.value());//ind);
        }
        i++;
        io->release ();
    }

    if (ind >= 0)
        loadAttrsRefFields (attr, idRef, aEditor);
}

void KKSCatEditorFactory :: loadAttrsRefFields (KKSAttribute * attr, int idRef, KKSAttrEditor * aEditor)
{
    if (!attr || !aEditor)
        return;

    KKSObject *io = loader->loadIO (idRef);
    if (!io)
        return;

    KKSCategory *c = io->category()->tableCategory ();
    if (!c)
    {
        io->release ();
        return;
    }

    QMap<QString, QString> fields;
    for (KKSMap<int, KKSCategoryAttr*>::const_iterator pc = c->attributes().constBegin(); pc != c->attributes().constEnd(); pc++)
        fields.insert (pc.value()->code(), pc.value()->code ());//name ());

    qDebug () << __PRETTY_FUNCTION__ << fields;
    aEditor->setIO (io);
    aEditor->uploadReferenceFields (fields);
    io->release ();
}

void KKSCatEditorFactory :: findAttributes (int idAttrs, KKSAttributesEditor * attrsEditor)
{
    if (idAttrs < 0 || !attrsEditor)
        return;

    KKSObject * o = loader->loadIO (idAttrs);
    if(!o)
        return;

    KKSCategory * c = o->category()->tableCategory();
    if(!c){
        o->release();
        return;
    }

    bool forIO = false;
    KKSMap<int, KKSAttribute*> attrs;

    KKSFiltersEditorForm * f = new KKSFiltersEditorForm (c, attrs, forIO, attrsEditor);
    if (f->exec() == QDialog::Accepted)
    {
        KKSList<KKSTemplate*> tListDb = loader->loadCategoryTemplates (c);
        const KKSTemplate * t = (tListDb.empty() ? new KKSTemplate (c->defTemplate()) : tListDb[0]);
        KKSList<const KKSFilterGroup *> filters;
        filters = f->filters();
        
        //если используем фильтры для справочника ИО, то из него надо исключить системные справочники и журналы
        //они доступны только администратору или из специальных точек доступа
        KKSViewFactory::loadEIOEx (attrsEditor, o, loader, t, attrsEditor->getView (), filters);
        if (tListDb.empty())
            t->release();
    }
    delete f;

    o->release();

}
*/
