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
#include <KKSRecDialog.h>
#include <KKSCategory.h>
#include <KKSAttribute.h>
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
#include <kkslifecycleform.h>
#include <KKSAttrModel.h>
#include <defines.h>
#include <KKSSortFilterProxyModel.h>

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

    QList<int> fTypes = loader->getForbiddenTypes ();
    KKSCategoryTemplateWidget *ctWidget = new KKSCategoryTemplateWidget (mode, fTypes, asAdmin, parent, f);
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

    connect (ctWidget, SIGNAL (addNewCategoryE (QWidget *, int, bool)),
             this,
             SLOT (addCloneCategory (QWidget *, int, bool))
             );
    
    connect (ctWidget, SIGNAL (editCategory(QWidget*, int, bool)),
             this,
             SLOT (editCategory (QWidget*, int, bool))
             );

    connect (ctWidget, SIGNAL (deleteCat (QWidget *, int)),
             this,
             SLOT (delCategory(QWidget *, int ))
             );

/*    
    connect (ctWidget, SIGNAL (addNewTemplate (QWidget *, int)),
             tf,
             SLOT (addTemplate (QWidget *, int))
             );

    connect (ctWidget, SIGNAL (editTempl (QWidget *, int)),
             tf,
             SLOT (editTemplate (QWidget *, int))
             );

    connect (ctWidget, SIGNAL (deleteT (QWidget *, int, QAbstractItemModel*, const QModelIndex&)),
             tf,
             SLOT (delTemplate (QWidget *, int, QAbstractItemModel*, const QModelIndex&))
             );
*/
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
        qCritical() << tr("Cannot load the category!");
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
            qCritical() << tr("Cannot create copy of the category");
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
        qCritical() << tr("Cannot create copy of the category");
        QMessageBox::critical(ctw, tr("Error!"), tr("Cannot create copy of the category"), QMessageBox::Ok);
        c->release();
        return;
    }

    emit categoryAdded(c);
}

void KKSCatEditorFactory :: addCloneCategory (QWidget *ctw, int idCat, bool isChild)
{
    Q_UNUSED (isChild);
    KKSCategory *c = loader->loadCategory (idCat);
    if (!c)
    {
        qCritical() << tr("Cannot load the category!");
        QMessageBox::critical(ctw, tr("Error!"), tr("Cannot load the category!"), QMessageBox::Ok);
        return;
    }

    if (c->tableCategory())
    {
        KKSCategory * ct = c->tableCategory();
        ct->setId (-1);
        ct->setName(ct->name() + tr(" (copy)"));
        ct->setCode (QString());
        //int res = ppf->insertCategory(ct);
        /*if (res == ERROR_CODE)
        {
            QMessageBox::critical(ctw, tr("Error!"), tr("Cannot create copy of the category"), QMessageBox::Ok);
            c->release();
            return;
        }*/

    }
    c->setId(-1);
    c->setName(c->name() + tr(" (copy)"));
    c->setCode(QString::null);
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
    int idCatType = c->type()->id();
    KKSCatEditor * catEditor = this->createCategoryEditor (c, filterGroups, isChild, idCatType, mode, cwModal, parent, Qt::Dialog);
    if (!catEditor)
        return;

    catEditor->setWindowTitle (tr("Creation new category on %1").arg (c->name()));
    if (cwModal != Qt::NonModal)
        (qobject_cast<QWidget *>(catEditor))->setAttribute (Qt::WA_DeleteOnClose);

    catEditor->show ();
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
        qCritical() << tr ("Cannot create category editor id = %1").arg (idCat);
        QMessageBox::critical(ctw, tr ("Category edition"), tr ("Cannot create category editor id = %1").arg (idCat), QMessageBox::Ok);
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
        qCritical() << tr ("Cannot remove category %1").arg (cat->name());
        QMessageBox::critical (ctw, tr ("Category remove"), tr ("Cannot remove category %1").arg (cat->name()), QMessageBox::Ok);
        cat->release ();
        return;
    }
    if (cat->tableCategory())
    {
        int tres = ppf->deleteCategory (cat->tableCategory());
        if (tres != OK_CODE)
        {
            qCritical() << tr ("Cannot remove category %1").arg (cat->tableCategory()->name());
            QMessageBox::critical(ctw, tr ("Table Category remove"), tr ("Cannot remove category %1").arg (cat->tableCategory()->name()), QMessageBox::Ok);
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
    KKSCategory * indCat (0);
    Q_UNUSED (indCat);
    
    if (idCategory <= 0 && isChildCat)
    {
        cat = new KKSCategory ();
        KKSType *cType = new KKSType (10, tr ("Child category"));
        cType->setAsQualifier(false);
        if (!cType)
            return 0;
        cat->setType (cType);
        cType->release ();
		cat->setMain(false);
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
    {
        cat = loader->loadCategory (idCategory);
    }
    else
    {
        cat = new KKSCategory ();
        KKSType * ct = loader->loadType (idCatType);
        cat->setType (ct);
        //if (cat && ct && (idCatType == 1 || idCatType == 2 || idCatType == 12))// категории данных типов являются табличными
        KKSType * cTableT = loader->loadType (10);
        //indCat = new KKSCategory (-1, QString(), cTableT);
        //cat->setRecAttrCategory (indCat);
        //if (indCat)
        //    indCat->release ();
        if (cat && ct && ct->isQualifier())// категории данных типов являются табличными
        {
            //
            // загрузка типа "подчиненная категория"
            //
            tableCat = new KKSCategory (-1, QString(), cTableT);
			tableCat->setMain(false);
            cat->setTableCategory (tableCat);
            if (tableCat)
                tableCat->release ();
        }
        if (cTableT)
            cTableT->release ();
        
        if (ct)
            ct->release ();
        
        KKSAccessEntity * acl = new KKSAccessEntity ();
        cat->setAccessRules (acl);
        if (cat->tableCategory())
            cat->tableCategory()->setAccessRules (acl);
        if (cat->recAttrCategory())
            cat->recAttrCategory()->setAccessRules (acl);
        acl->release ();
    }

    if (!cat)
        return 0;

    KKSCatEditor * cEditor = createCategoryEditor (cat, filters, isChildCat, idCatType, mode, windowModality, parent, f);
    return cEditor;
}

KKSCatEditor* KKSCatEditorFactory :: createCategoryEditor (KKSCategory *cat, // категория
                                                           const KKSList<const KKSFilterGroup *> & filters,
                                                           bool isChildCat, // создается или редактируется дочерняя категория
                                                           int idCatType, // тип категории по умолчанию
                                                           bool mode,
                                                           Qt::WindowModality windowModality,
                                                           QWidget *parent,
                                                           Qt::WindowFlags f)
{
    Q_UNUSED (filters);
    if (!cat)
        return 0;

    if (isChildCat && qobject_cast<KKSCatEditor *>(parent))
    {
        KKSCatEditor *pEditor = qobject_cast<KKSCatEditor *>(parent);
        pEditor->category()->setTableCategory (cat);
    }

    //загружаем атрибуты категории в целом
    KKSRecWidget *rAttrCw = getAttrsWidget (cat, mode, 0);
    //cEditor->setAttrs (rw);
    KKSType * cTableT = loader->loadType (10);
    bool isIOs = loader->isIOCreated(cat);
    
    //загружаем атрибуты таблицы
    KKSRecWidget * rAttrTCw = getAttrsWidget (cat->tableCategory(), mode, 0);
    rAttrTCw->setToolBarActionEnabled(rAttrTCw->actAdd, !isIOs);
    //rAttrTCw->setToolBarActionEnabled(rAttrTCw->actEdit, !isIOs);
    rAttrTCw->setToolBarActionEnabled(rAttrTCw->actDel, !isIOs);
    if (isIOs)
        rAttrTCw->setStatusTip(tr("There are documents created by category %1, operations with attributes are unavailable").arg (cat->name()));
    
    //загружаем показатели записей таблицы
    KKSRecWidget * recAttrCw = getAttrsWidget (cat->recAttrCategory(), mode, 0);
    
    bool isIOSync = loader->isIOSynced(cat);
    recAttrCw->setToolBarActionEnabled(recAttrCw->actAdd, !isIOSync);
    recAttrCw->setToolBarActionEnabled(recAttrCw->actDel, !isIOSync);
    QAction * actCopyFrom = new QAction (QIcon (":/ddoc/add_copy.png"), tr("Copy attributes from"), rAttrTCw);
    rAttrTCw->addToolBarAction (actCopyFrom);
    KKSMap<int, KKSType*> cTypes = loader->loadAvailableTypes();

    KKSRecWidget *rwt = getTemplateWidget (cat, false, 0);//KKSViewFactory :: createCategoryTemplates (cat->id (), loader, 0);
//    cEditor->setTemplates (rwt);
    KKSRecWidget *rwtT = getTemplateWidget (cat->tableCategory(), false, 0);
    KKSRecWidget *rawtT = getTemplateWidget (cat->recAttrCategory(), false, 0);

    KKSCatEditor *cEditor = new KKSCatEditor (cat, rAttrCw, rAttrTCw, recAttrCw, rwt, rwtT, rawtT, cTypes, idCatType, mode, parent, f);
    if (!cEditor)
        return 0;
    connect (actCopyFrom, SIGNAL (triggered()), cEditor, SLOT (copyAttributesFrom()));

    cEditor->setTableType (cTableT);

    KKSObject *attrTypesIO = loader->loadIO (IO_ATTR_TYPE_ID, true);
    if (!attrTypesIO)
    {
        qCritical() << QObject::tr ("Cannot load attribute types");
        QMessageBox::critical (parent, QObject::tr ("Attributes types"), QObject::tr ("Cannot load attribute types"), QMessageBox::Ok);
        return 0;
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

    cEditor->setAttrTypes (availAttrTypes);
    if (!isChildCat)
    {
        KKSStuffForm *sForm = stuffF->createStuffEditorForm (cat->getAccessRules(), 1, cEditor);
        cEditor->setAccessWidget (sForm);
    }

    connect (cEditor, SIGNAL (addAttrsIntoCat (KKSCategory *, QTreeView *, KKSCatEditor *)), this, SLOT (addAttributeIntoCategory (KKSCategory *, QTreeView *, KKSCatEditor *)) );
    connect (cEditor, SIGNAL (copyAttrsFromAnotherCat (KKSCategory *, QAbstractItemModel *, KKSCatEditor *)), this, SLOT (copyAttributesIntoCategory (KKSCategory *, QAbstractItemModel *, KKSCatEditor *)) );
    connect (cEditor, SIGNAL (setAttribute (int, KKSCategory*, QTreeView *, KKSCatEditor *)), this, SLOT (loadCatAttribute (int, KKSCategory*, QTreeView *, KKSCatEditor *)));
    connect (cEditor, SIGNAL (setAttribute (KKSCategoryAttr *, KKSCategory*, QTreeView *, KKSCatEditor *)), this, SLOT (loadCatAttribute (KKSCategoryAttr *, KKSCategory*, QTreeView *, KKSCatEditor *)) );
    connect (cEditor, SIGNAL (delAttrFromCategory (int, KKSCategory *, QAbstractItemModel *, KKSCatEditor *)), this, SLOT (delCatAttribute(int, KKSCategory *, QAbstractItemModel *, KKSCatEditor *)) );
    
    connect (cEditor, SIGNAL (saveCategory (KKSCategory *, int, int, KKSCatEditor *)), this, SLOT (saveCategory (KKSCategory *, int, int, KKSCatEditor *)) );
    connect (cEditor, SIGNAL (addChildCat (QWidget *, int, bool)), this, SLOT (addCategory (QWidget *, int, bool)) );
    connect (cEditor, SIGNAL (editChildCat (QWidget *, int, bool)), this, SLOT (editCategory (QWidget *, int, bool)) );
    
    connect (cEditor, SIGNAL (addNewCategoryTemplate (QWidget *, KKSCategory *, QAbstractItemModel *)), tf, SLOT (addTemplate (QWidget *, KKSCategory *, QAbstractItemModel *)) );
    connect (cEditor, SIGNAL (editCategoryTemplate (QWidget *, KKSTemplate *, QAbstractItemModel *, const QModelIndex& )), tf, SLOT (editCatTemplate (QWidget *, KKSTemplate *, QAbstractItemModel *, const QModelIndex&)) );
    connect (cEditor, SIGNAL (delCategoryTemplate (QWidget *, int, QAbstractItemModel *, const QModelIndex& )), tf, SLOT (delTemplate (QWidget *, int, QAbstractItemModel *, const QModelIndex&)) );
    
    connect (this, SIGNAL (categoryDbError ()), cEditor, SLOT (catDbError()) );
    connect (this, SIGNAL (categoryAdded(KKSCategory *)), cEditor, SLOT (catDbOk(KKSCategory *)) );
    
    connect (cEditor, SIGNAL (refreshTemplates (KKSCategory *, QAbstractItemModel *)), this, SLOT (refreshCategoryTemplates (KKSCategory *, QAbstractItemModel *)) );
    connect (cEditor, SIGNAL (lifeCycle (KKSCategory *, QLineEdit *)), this, SLOT (setLifeCycleIntoCategory(KKSCategory *, QLineEdit *)) );

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
    Q_UNUSED (mode);
    KKSRecWidget * rw (0);

    if (cat)//cat->type()->id() != 10)
        rw = KKSViewFactory :: createCategAttrsView (loader, cat, parent);
    else
    {
        KKSType * cTableT = KKSType::createType10(); //child type
        cat = new KKSCategory (-1, QString(), cTableT);
		(const_cast<KKSCategory*>(cat))->setMain(false);
        //cat->setName("Indicators of " + pCategory->name());
        KKSAccessEntity * acl = new KKSAccessEntity ();
        const_cast<KKSCategory *>(cat)->setAccessRules (acl);
        
        rw = new KKSRecWidget (false); //mode);
        QTreeView * tvTableAttrs = rw->getView();//new QTreeView ();
        tvTableAttrs->header()->setClickable (true);
        tvTableAttrs->header()->setSortIndicatorShown (true);
        tvTableAttrs->header()->setSortIndicator (5, Qt::AscendingOrder);
        tvTableAttrs->setSortingEnabled (true);
        
        rw->hideActionGroup (_ID_FILTER_GROUP);
        rw->hideActionGroup (_ID_IMPORT_GROUP);
        rw->hideActionGroup (_ID_VIEW_GROUP);
        rw->hideActionGroup (_ID_REPORT_GROUP);


        QAbstractItemModel * acModel = new KKSAttrModel (cat);//QStandardItemModel (0, 4);
        acModel->setHeaderData (0, Qt::Horizontal, QObject::tr ("ID"));
        acModel->setHeaderData (1, Qt::Horizontal, QObject::tr ("Name"));
        acModel->setHeaderData (2, Qt::Horizontal, QObject::tr ("Default value"));
        acModel->setHeaderData (3, Qt::Horizontal, QObject::tr ("Mandatory"));
        acModel->setHeaderData (4, Qt::Horizontal, QObject::tr ("Read only"));
        acModel->setHeaderData (5, Qt::Horizontal, QObject::tr ("Order"));
        acModel->setHeaderData (6, Qt::Horizontal, QObject::tr ("Directives"));

        KKSViewFactory::updateAttrModel (0, acModel);
        //tvTableAttrs->setModel (acModel);
        KKSItemDelegate *itemDeleg = new KKSItemDelegate (rw);
        tvTableAttrs->setItemDelegate (itemDeleg);

        //делаем список с атрибутами в категории сортируемым
        KKSSortFilterProxyModel *sortModel = 0;
        sortModel = new KKSSortFilterProxyModel (tvTableAttrs);//ksa было editor
        tvTableAttrs->setModel (sortModel);
        sortModel->setDynamicSortFilter (true);
        sortModel->setSourceModel (acModel);

        //добавим в KKSSortFilterProxyModel набор атрибутов, которые задаются справочником атрибутов в категории
        //это надо для правильной фильтрации по соответствующим колонкам в таблице (с учетом типа колонок(атрибутов))
        KKSMap<int, KKSCategoryAttr *> acList = loader->loadCategoryAttrs(ATTRS_CAT_TABLE_CATEGORY_ID);
        if(acList.count() > 0){
            KKSAttrView * av = 0;
            KKSCategoryAttr * ac = 0;
            
            ac = acList.value(1); //id
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(2);//name
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(304);//def_value
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(128); //is_mandatory
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(305); //is_read_only
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(404); //order
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();

            ac = acList.value(405); //directives
            if(ac)
                av = new KKSAttrView(*ac);
            else
                av = new KKSAttrView();
            sortModel->addAttrView(av);
            av->release();
        }


        int sortCol = tvTableAttrs->header()->sortIndicatorSection ();
        Qt::SortOrder sOrder = tvTableAttrs->header()->sortIndicatorOrder ();
        sortModel->sort (sortCol, sOrder);

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
        rtw = new KKSRecWidget (mode);
        QTreeView * tvTableTempl = rtw->getView();
        tvTableTempl->header()->setClickable (true);
        tvTableTempl->header()->setSortIndicatorShown (true);
        tvTableTempl->header()->setSortIndicator (0, Qt::AscendingOrder);
        tvTableTempl->setSortingEnabled (true);
        
        rtw->hideActionGroup (_ID_FILTER_GROUP);
        rtw->hideActionGroup (_ID_VIEW_GROUP);
        rtw->hideActionGroup (_ID_IMPORT_GROUP);
        rtw->hideActionGroup (_ID_REPORT_GROUP);
        
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

void KKSCatEditorFactory :: loadCatAttribute (int id, KKSCategory *c, QTreeView * tv, KKSCatEditor *editor)
{
    if (!editor || !c)
        return;

    KKSSortFilterProxyModel *sortModel = qobject_cast <KKSSortFilterProxyModel *>(tv->model());
    if (!sortModel)
        return;

    QAbstractItemModel * acModel = sortModel->sourceModel();
    if(!acModel)
        return;

    KKSAttribute *attr = loader->loadAttribute (id);
    if (!attr)
        return;
    bool is_bad;
    
    int nextOrder  = acModel->rowCount();
    //если атрибут это ИД, то всегжа делаем его первым по порядку
    if(id == ATTR_ID)
        nextOrder = -1;

    KKSCategoryAttr *cAttr = KKSCategoryAttr::create (attr, false, false, QString(), nextOrder+1, QString::null, &is_bad);
    if (is_bad)
    {
        if (cAttr)
            cAttr->release ();
        return;
    }
    loadCatAttribute (cAttr, c, tv, editor);
    cAttr->release ();
}

void KKSCatEditorFactory :: addAttributeIntoCategory (KKSCategory *c, QTreeView * tv, KKSCatEditor *editor)
{
    if (!c || !tv)
        return;

    KKSList<const KKSFilterGroup *> filters;
    KKSAttributesEditor *aEditor = attrf->viewAttributes (filters, true, editor, Qt::Dialog);
    if (!aEditor)
        return;

    KKSSortFilterProxyModel *sortModel = qobject_cast <KKSSortFilterProxyModel *>(tv->model());
    if (!sortModel)
        return;

    QAbstractItemModel * attrModel = sortModel->sourceModel();
    if(!attrModel)
        return;

    sortModel->setDynamicSortFilter(false);

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
            int nextOrder  = attrModel->rowCount();
            KKSCategoryAttr *cAttr = KKSCategoryAttr::create (attr, false, false, QString(), nextOrder+1, QString::null, &is_bad);
            if (is_bad || !cAttr)
            {
                if (cAttr)
                    cAttr->release ();
                sortModel->setDynamicSortFilter(true);
                return;
            }
            if (!c->attributes().contains (cAttr->id()))
            {
                attrModel->insertRows(attrModel->rowCount(), 1);
                c->addAttribute(cAttr->id(), cAttr);
            }

            cAttr->release ();
            attr->release();
        }

        bool isCat = attrModel->setData(QModelIndex(), QVariant::fromValue<KKSCategory *>(c), Qt::UserRole+2);
        if (!isCat)
        {
            sortModel->setDynamicSortFilter(true);
            return;
        }
    }

    int sortCol = tv->header()->sortIndicatorSection ();
    Qt::SortOrder sOrder = tv->header()->sortIndicatorOrder ();
    sortModel->sort (sortCol, sOrder);
    
    sortModel->setDynamicSortFilter(true);

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
    KKSRecDialog * oEditor = objf->createObjRecEditor (IO_IO_ID,
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
            qCritical() << tr ("Cannot load source category");
            QMessageBox::critical (cEditor, tr ("Category editor"), tr ("Cannot load source category"), QMessageBox::Ok);
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

void KKSCatEditorFactory :: loadCatAttribute (KKSCategoryAttr *cAttr, KKSCategory *c, QTreeView * tv, KKSCatEditor *editor)
{
    if (!cAttr || !c || !tv || !editor)
        return;

    KKSSortFilterProxyModel *sortModel = qobject_cast <KKSSortFilterProxyModel *>(tv->model());
    if (!sortModel)
        return;

    QAbstractItemModel * acModel = sortModel->sourceModel();
    if(!acModel)
        return;
    
    KKSMap<int, KKSCategoryAttr *> existsAttrs = c->attributes();
    int idattr = -1;
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator p = existsAttrs.constBegin(); p!= existsAttrs.constEnd() && idattr < 0; p++)
    {
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

    sortModel->setDynamicSortFilter(false);
    KKSViewFactory::updateAttrModel (c, acModel);
    editor->update ();

    int sortCol = tv->header()->sortIndicatorSection ();
    Qt::SortOrder sOrder = tv->header()->sortIndicatorOrder ();
    sortModel->sort (sortCol, sOrder);

    sortModel->setDynamicSortFilter(true);
}

void KKSCatEditorFactory :: delCatAttribute (int id, KKSCategory *c, QAbstractItemModel *attrModel, KKSCatEditor *editor)
{
    if (!editor || !c || id<0 || !c->attribute(id) || !attrModel)
        return;

    c->removeAttribute (id);
//     = editor->recWidget->getModel ();
    KKSViewFactory::updateAttrModel (c, attrModel);
    KKSMap<int, KKSTemplate *> cTempls = c->getTemplates();
    for (KKSMap<int, KKSTemplate *>::const_iterator p = cTempls.constBegin();
            p != cTempls.constEnd();
            p++)
    {
        KKSTemplate * t = p.value();
        if (t)
            t->removeAttribute (id);
    }
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
    else{
        res = ppf->updateCategoryEx (cat);
    }

    if (res == ERROR_CODE)
    {
        emit categoryDbError ();
        qCritical() << tr("Category is not saved");
        QMessageBox::critical(cEditor, tr ("Category editor"), tr("Category %1 is not saved").arg (cat->name()));
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
    KKSFilter * filter = refCatObj->category()->tableCategory()->createFilter(ATTR_IS_MAIN, QString("true"), KKSFilter::foEq);
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

void KKSCatEditorFactory :: refreshCategoryTemplates (KKSCategory * c, QAbstractItemModel * templModel)
{
    if (!c || !templModel)
        return;

    KKSList<KKSTemplate *> lTempls = loader->loadCategoryTemplates (c->id());
    KKSViewFactory::loadCategoryDbTemplates (lTempls, templModel, QModelIndex());
}

void KKSCatEditorFactory :: setLifeCycleIntoCategory (KKSCategory * c, QLineEdit * lE)
{
    if (!c)
        return;
    qDebug () << __PRETTY_FUNCTION__ << c->id() << c->name();
    KKSObject * lcIO = loader->loadIO(IO_LIFE_CYCLE_ID, true);
    const KKSCategory * wCat = lcIO ? lcIO->category() : 0;
    if (wCat)
        wCat = wCat->tableCategory();
    if (lcIO)
        lcIO->release();
    KKSRecDialog * lcEditor = this->objf->createObjRecEditor(IO_IO_ID,
                                                             IO_LIFE_CYCLE_ID,
                                                             KKSList<const KKSFilterGroup *>(),
                                                             tr("Set life cycle onto category %1").arg (c->name()),
                                                             wCat,
                                                             true,
                                                             true,
                                                             Qt::ApplicationModal);
    
    lcEditor->disconnect(SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)), objf, SLOT (createNewEditor(QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)));//disconnect (lcEditor);
    lcEditor->disconnect(SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& )), objf, SLOT (editExistOE (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex&)));
    lcEditor->disconnect(SIGNAL (delObjectEx (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)), objf, SLOT (deleteOE (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)));
    connect (lcEditor,
             SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)),
             this,
             SLOT (addLifeCycle (QWidget *, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *))
            );
    connect (lcEditor,
             SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& )),
             this,
             SLOT (editLifeCycle (QWidget *, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& ))
            );
    connect (lcEditor,
             SIGNAL (delObjectEx (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)),
             this,
             SLOT (delLifeCycle (QWidget *, int, qint64, QString, QAbstractItemModel *, const QModelIndex&))
            );
    
    if (!lcEditor)
        return;
    if (lcEditor->exec() == QDialog::Accepted)
    {
        int lcId = lcEditor->getID();
        KKSLifeCycleEx * lc = loader->loadLifeCycle(lcId);
        if (!lc)
        {
            lcEditor->setParent(0);
            delete lcEditor;
        }
        c->setLifeCycle (lc);
        if (lE)
            lE->setText(lc->name());
        if (lc)
            lc->release ();
    }
    lcEditor->setParent(0);
    delete lcEditor;
}

void KKSCatEditorFactory :: addLifeCycle (QWidget * editor, int idObject, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * sRecMod)
{
    Q_UNUSED (c);
    Q_UNUSED (tableName);
    Q_UNUSED (nTab);
    Q_UNUSED (isModal);
    qDebug () << __PRETTY_FUNCTION__;
    KKSLifeCycleEx * lc = new KKSLifeCycleEx (-1, QString(), QString());
    
    kkslifecycleform * lcForm = new kkslifecycleform (lc);//, editor);
    connect (lcForm,
             SIGNAL (loadState (KKSLifeCycleEx *, QLineEdit *, int, const QVector<qint64>& )),
             this->objf,
             SLOT (loadLifeCycleState (KKSLifeCycleEx *, QLineEdit *, int, const QVector<qint64>& ))
            );
    connect (lcForm,
             SIGNAL (addLCState (KKSLifeCycleEx *, QAbstractItemModel *)),
             objf,
             SLOT (addLifeCycleState(KKSLifeCycleEx *, QAbstractItemModel *))
            );
    if (!lc || !lcForm || lcForm->exec() != QDialog::Accepted)
    {
        if (lc)
            lc->release ();
        if (lcForm)
            delete lcForm;
        return;
    }
    KKSLifeCycleEx * lcf = lcForm->getLC();
    int ier = this->ppf->insertLifeCycle (lcf);
    if (ier < 0)
    {
        qCritical() << tr ("Cannot add new life cycle. Database error");
        QMessageBox::critical(editor, tr("Add new life cycle"), tr ("Cannot add new life cycle. Database error"), QMessageBox::Ok);
        lcForm->setParent (0);
        delete lcForm;
        lc->release ();
        return;
    }
    int nr = sRecMod->rowCount();
    sRecMod->insertRows(nr, 1);
    QModelIndex recIndex = sRecMod->index (nr, 0);
    KKSEIOData * lcInfo = this->loader->loadEIOInfo (idObject, lcf->id());
    sRecMod->setData (recIndex, QVariant::fromValue<KKSEIOData *>(lcInfo), Qt::UserRole+1);
    sRecMod->setData (recIndex, lcf->id(), Qt::UserRole);
    sRecMod->setData (recIndex, QIcon(":/ddoc/rubric_item.png"), Qt::DecorationRole);
    
    lcForm->setParent (0);
    delete lcForm;
    lc->release ();
}

void KKSCatEditorFactory :: editLifeCycle (QWidget * editor, int idObject, qint64 idObjE, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * sRecMod, const QModelIndex& recIndex)
{
    Q_UNUSED (c);
    Q_UNUSED (tableName);
    Q_UNUSED (nTab);
    Q_UNUSED (isModal);
    if (idObject != IO_LIFE_CYCLE_ID)
        return;
    qDebug () << __PRETTY_FUNCTION__;
    KKSLifeCycleEx * lc = loader->loadLifeCycle(idObjE);
    
    kkslifecycleform * lcForm = new kkslifecycleform (lc);//, editor);
    connect (lcForm,
             SIGNAL (loadState (KKSLifeCycleEx *, QLineEdit *, int, const QVector<qint64>& )),
             this->objf,
             SLOT (loadLifeCycleState (KKSLifeCycleEx *, QLineEdit *, int, const QVector<qint64>& ))
            );
    connect (lcForm,
             SIGNAL (addLCState (KKSLifeCycleEx *, QAbstractItemModel *)),
             objf,
             SLOT (addLifeCycleState(KKSLifeCycleEx *, QAbstractItemModel *))
            );
    if (!lc || !lcForm || lcForm->exec() != QDialog::Accepted)
    {
        if (lc)
            lc->release ();
        if (lcForm)
            delete lcForm;
        return;
    }
    KKSLifeCycleEx * lcf = lcForm->getLC();
    int ier = this->ppf->updateLifeCycle (lcf);
    if (ier < 0)
    {
        qCritical() <<  tr ("Cannot update life cycle. Database error");
        QMessageBox::critical (editor, tr("Edit life cycle"), tr ("Cannot update life cycle. Database error"), QMessageBox::Ok);
        lcForm->setParent (0);
        delete lcForm;
        lc->release ();
        return;
    }
    KKSEIOData * lcInfo = this->loader->loadEIOInfo (idObject, lcf->id());
    sRecMod->setData (recIndex, QVariant::fromValue<KKSEIOData *>(lcInfo), Qt::UserRole+1);
    sRecMod->setData (recIndex, QIcon(":/ddoc/rubric_item.png"), Qt::DecorationRole);
    lcForm->setParent (0);
    delete lcForm;
    lc->release ();
}

void KKSCatEditorFactory :: delLifeCycle (QWidget * editor, int idObject, qint64 idObjE, QString tableName, QAbstractItemModel * recModel, const QModelIndex& recIndex)
{
    Q_UNUSED (tableName);
    if (idObject != IO_LIFE_CYCLE_ID)
        return;
    qDebug () << __PRETTY_FUNCTION__;
    int ier = ppf->deleteLifeCycle(idObjE);
    if (ier < 0)
    {
        qCritical() << tr ("Cannot delete life cycle. Database error");
        QMessageBox::critical(editor, tr("Remove life cycle"), tr ("Cannot delete life cycle. Database error"), QMessageBox::Ok);
        return;
    }
    int row = recIndex.row();
    QModelIndex pInd = recIndex.parent();
    recModel->removeRows (row, 1, pInd);
}

KKSRecDialog * KKSCatEditorFactory :: openLifeCycle ()
{
    KKSObject * lcIO = loader->loadIO(IO_LIFE_CYCLE_ID, true);
    if (!lcIO)
        return 0;
    const KKSCategory * wCat = lcIO->category();
    if (wCat)
        wCat = wCat->tableCategory();
    KKSRecDialog * lcEditor = this->objf->createObjRecEditor(IO_IO_ID,
                                                             IO_LIFE_CYCLE_ID,
                                                             KKSList<const KKSFilterGroup *>(),
                                                             tr("View life cycles"),
                                                             wCat,
                                                             false,
                                                             true,
                                                             Qt::NonModal);
    
    lcEditor->disconnect(SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)), objf, SLOT (createNewEditor(QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)));//disconnect (lcEditor);
    lcEditor->disconnect(SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& )), objf, SLOT (editExistOE (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex&)));
    lcEditor->disconnect(SIGNAL (delObjectEx (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)), objf, SLOT (deleteOE (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)));
    connect (lcEditor,
             SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)),
             this,
             SLOT (addLifeCycle (QWidget *, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *))
            );
    connect (lcEditor,
             SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& )),
             this,
             SLOT (editLifeCycle (QWidget *, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& ))
            );
    connect (lcEditor,
             SIGNAL (delObjectEx (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)),
             this,
             SLOT (delLifeCycle (QWidget *, int, qint64, QString, QAbstractItemModel *, const QModelIndex&))
            );
    
    lcIO->release();
    return lcEditor;
}

kkslifecycleform * KKSCatEditorFactory :: createLifeCycle ()
{
    KKSLifeCycleEx * lc = new KKSLifeCycleEx (-1, QString(), QString());
    
    kkslifecycleform * lcForm = new kkslifecycleform (lc, false);//, editor);
    connect (lcForm,
             SIGNAL (loadState (KKSLifeCycleEx *, QLineEdit *, int, const QVector<qint64>& )),
             this->objf,
             SLOT (loadLifeCycleState (KKSLifeCycleEx *, QLineEdit *, int, const QVector<qint64>& ))
            );
    connect (lcForm,
             SIGNAL (addLCState (KKSLifeCycleEx *, QAbstractItemModel *)),
             objf,
             SLOT (addLifeCycleState(KKSLifeCycleEx *, QAbstractItemModel *))
            );
    connect (lcForm,
             SIGNAL (saveLifeCycle(KKSLifeCycleEx *)),
             this,
             SLOT (saveLifeCycleToDb (KKSLifeCycleEx *))
            );
    emit lifeCycleEditorCreated (lcForm);
    return lcForm;
}

void KKSCatEditorFactory :: saveLifeCycleToDb (KKSLifeCycleEx * lc)
{
    if (!lc)
        return;
    kkslifecycleform * parentWidget = qobject_cast<kkslifecycleform *>(this->sender());
    bool isIns (lc->id() <= 0);
    int ier = (lc->id() <= 0 ? ppf->insertLifeCycle (lc) : ppf->updateLifeCycle (lc));
    if (ier < 0)
    {
        qCritical() << tr ("Error in life cycle save");
        QMessageBox::critical(parentWidget, tr ("Save life cycle"), tr ("Error in life cycle save"), QMessageBox::Ok);
        return;
    }
    if (isIns && parentWidget)
        parentWidget->updateLC (lc);
        
}