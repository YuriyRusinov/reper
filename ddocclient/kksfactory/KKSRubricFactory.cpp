#include <QTreeView>
#include <QMessageBox>
#include <QItemDelegate>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QtDebug>

#include <kksincludeswidget.h>
#include <KKSIncludesDialog.h>
#include <rubricform.h>
#include "KKSLoader.h"
#include "KKSConverter.h"
#include <KKSRubric.h>
#include <KKSEventFilter.h>
#include "KKSPPFactory.h"
#include "KKSEIOFactory.h"
#include "KKSObjEditorFactory.h"
#include "KKSStuffFactory.h"
#include <KKSObjEditor.h>
#include <KKSObject.h>
#include <KKSObjectExemplar.h>
#include "KKSAttrType.h"
#include <KKSCategory.h>
#include <defines.h>
#include <KKSSearchTemplate.h>
#include "kksstuffform.h"
#include "KKSAccessEntity.h"
#include <KKSEIOData.h>
#include <KKSEIODataModel.h>
#include <KKSSyncDialog.h>
#include <KKSSyncWidget.h>
#include <KKSStuffDialog.h>
#include <KKSSortFilterProxyModel.h>

#include "KKSRubricFactory.h"

KKSRubricFactory :: KKSRubricFactory (KKSLoader *l, KKSPPFactory *_ppf, KKSEIOFactory *_eiof, KKSObjEditorFactory *_oef, KKSStuffFactory * _stf, QObject* parent)
    : KKSEntityFactory(parent),
    loader (l),
    ppf (_ppf),
    eiof (_eiof),
    oef (_oef),
    stf (_stf)
{
    if (oef)
        connect (oef, SIGNAL (cioSaved(KKSObjectExemplar *)), this, SLOT (rubricItemCreated(KKSObjectExemplar *)) );
}

KKSRubricFactory :: ~KKSRubricFactory (void)
{
}

KKSIncludesWidget * KKSRubricFactory :: createRubricEditor (int mode, const KKSList<const KKSFilterGroup *> & filters, bool withCategories, QWidget* parent)
{
    Q_UNUSED (filters);
    bool isDocs (mode==atMyDocsRubric);

    KKSRubric * rootR = loader->loadRubricators ( isDocs );
    if (!rootR){
        rootR = new KKSRubric(-1, "root rubric for all tree");
        KKSRubric * rubrMyDocs = new KKSRubric (-1, tr("My Documents"));
        if (!rubrMyDocs)
            return 0;
        rootR->addRubric (rubrMyDocs);
        int res = ppf->updateRubricators (rootR, isDocs);
        if (res != OK_CODE)
        {
            QMessageBox::warning (parent, tr("Rubricator"), tr ("Cannot load My documents"), QMessageBox::Ok);
            return 0;
        }

        ppf->createMyDocsRubricator (rubrMyDocs->id());
    }

    QString title;
    switch (mode)
    {
        case atRootRubric:
            {
                title = tr ("All rubrics");
                break;
            }
        case atMyDocsRubric:
            {
                title = tr ("My Documents");
                break;
            }
        case atOthers:
            {
                title = tr ("Others");
                break;
            }
        default:
            {
                rootR->release();
                return 0;
                break;
            }
    }

    KKSIncludesWidget *iW = new KKSIncludesWidget (rootR, false, isDocs, false, false, parent);
    QTreeView *tv = iW->tvRubr();
    QAbstractItemModel * rubrMod = iW->rubrModel();
    if (rubrMod && withCategories)
    {
        int nr = rubrMod->rowCount();
        bool isIns = rubrMod->insertRows (nr, 1);
        if (isIns)
        {
            QModelIndex wIndex = rubrMod->index (nr, 0);
            const KKSRubricBase * rOthers = loader->loadCatRubricators ();//new KKSRubricOthers (-1, tr("Others"));
            bool isRubrDataSet = rubrMod->setData (wIndex, QVariant::fromValue<const KKSRubricBase *>(rOthers), Qt::UserRole+1);
            Q_UNUSED (isRubrDataSet);
//            bool isDataSet = rubrMod->setData (wIndex, tr("Others"), Qt::DisplayRole);
//            bool isTypeSet = rubrMod->setData (wIndex, atOthers, Qt::UserRole);
//            bool isIconSet = rubrMod->setData (wIndex, KKSRubric::icon().scaled(24, 24), Qt::DecorationRole);
//            int nrc = rubrMod->rowCount (wIndex);
//            const KKSRubric * cRubric = loader->loadCatRubricators ();
//            rubrMod->insertRows (nrc, 1, wIndex);
//            rubrMod->setData (rubrMod->index (nrc, 0, wIndex), QVariant::fromValue<const KKSRubricBase *>(cRubric), Qt::UserRole+1);
//            if (isRubrDataSet && isDataSet && isTypeSet && isIconSet)
//                iW->addRubricIntoModel (cRubric, wIndex);
        }
    }
    KKSEventFilter *ef = new KKSEventFilter (iW);
    iW->setWindowTitle (title);
    tv->viewport()->installEventFilter (ef);
    if (rootR)
        rootR->release ();

    //QAbstractItemDelegate *iDeleg = tv->itemDelegate ();
    //qDebug () << __PRETTY_FUNCTION__ << iDeleg;

    connect (iW, SIGNAL (saveRubric (KKSRubric *, bool)), this, SLOT (saveRubric (KKSRubric *, bool)) );
    connect (iW, SIGNAL (rubricItemRequested (const KKSRubric*, bool, QAbstractItemModel *)), this, SLOT (rubricItemUpload(const KKSRubric *,bool, QAbstractItemModel *)) );
    connect (iW, SIGNAL (rubricItemCreationRequested (const KKSRubric *, QAbstractItemModel*, const QModelIndex&)), this, SLOT (rubricItemCreate(const KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
    connect (iW, SIGNAL (openRubricItemRequested (int)), this, SLOT (openRubricItem (int)) );
    connect (iW, SIGNAL (loadStuffModel (RubricForm *)), this, SLOT (loadRubricPrivilegies(RubricForm *)) );
    connect (iW, SIGNAL (loadSearchtemplate (RubricForm *)), this, SLOT (loadSearchTemplate (RubricForm *)) );
    connect (iW, SIGNAL (loadCategory (RubricForm *)), this, SLOT (loadCategory (RubricForm *)) );
    connect (iW, SIGNAL (rubricAttachmentsView (QAbstractItemModel *, const KKSRubric *)), this, SLOT (viewAttachments (QAbstractItemModel *, const KKSRubric *)) );
    connect (iW, SIGNAL (copyFromRubr(KKSRubric *, QAbstractItemModel *, const QModelIndex&)), this, SLOT (copyFromRubric (KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
    connect (iW, SIGNAL (initAttachmentsModel (const KKSRubric *)), this, SLOT (initRubricAttachments (const KKSRubric *)) );
    connect (iW, SIGNAL (appendRubricItemIntoModel (QAbstractItemModel *, const KKSRubricItem * )), this, SLOT (appendRubricItem (QAbstractItemModel *, const KKSRubricItem *)) );
    connect (iW, SIGNAL (setSyncIO(const QList<int>&)), this, SLOT (setSyncSettings (const QList<int>&)) );
    connect (iW, SIGNAL (putIOSIntoRubr (const QList<int>&, const KKSRubric*)), this, SLOT (putIntoRubr (const QList<int>&, const KKSRubric*)) );
    connect (iW, SIGNAL (sendIOS (const QList<int>&)), this, SLOT (sendDocs (const QList<int>&)) );
    connect (iW, SIGNAL (setAccessIOS (const QList<int>&)), this, SLOT (setAccessDocs (const QList<int>&)) );
    
    connect (this, SIGNAL (rubricAttachments (QAbstractItemModel *)), iW, SLOT (slotInitAttachmentsModel (QAbstractItemModel *)) );
    emit rubricEditorCreated (iW);

    return iW;
}

KKSIncludesWidget * KKSRubricFactory :: createModalRubricEditor (int mode, const KKSList<const KKSFilterGroup *> & filters, bool withCategories, QWidget* parent)
{
    Q_UNUSED (filters);
    bool isDocs (mode==atMyDocsRubric);

    KKSRubric * rootR = loader->loadRubricators ( isDocs );
    if (!rootR){
        rootR = new KKSRubric(-1, "root rubric for all tree");
        KKSRubric * rubrMyDocs = new KKSRubric (-1, tr("My Documents"));
        if (!rubrMyDocs)
            return 0;
        rootR->addRubric (rubrMyDocs);
        int res = ppf->updateRubricators (rootR, isDocs);
        if (res != OK_CODE)
        {
            QMessageBox::warning (parent, tr("Rubricator"), tr ("Cannot load My documents"), QMessageBox::Ok);
            return 0;
        }

        ppf->createMyDocsRubricator (rubrMyDocs->id());
    }

    QString title;
    switch (mode)
    {
        case atRootRubric:
            {
                title = tr ("All rubrics");
                break;
            }
        case atMyDocsRubric:
            {
                title = tr ("My Documents");
                break;
            }
        case atOthers:
            {
                title = tr ("Others");
                break;
            }
        default:
            {
                rootR->release();
                return 0;
                break;
            }
    }

    KKSIncludesWidget *iW = new KKSIncludesWidget (rootR, false, isDocs, false, false, parent);
    iW->setForRubrics (true);
    QTreeView *tv = iW->tvRubr();
    QAbstractItemModel * rubrMod = iW->rubrModel();
    if (rubrMod && withCategories)
    {
        int nr = rubrMod->rowCount();
        bool isIns = rubrMod->insertRows (nr, 1);
        if (isIns)
        {
            QModelIndex wIndex = rubrMod->index (nr, 0);
            const KKSRubricBase * rOthers = loader->loadCatRubricators ();//new KKSRubricOthers (-1, tr("Others"));
            bool isRubrDataSet = rubrMod->setData (wIndex, QVariant::fromValue<const KKSRubricBase *>(rOthers), Qt::UserRole+1);
            Q_UNUSED (isRubrDataSet);
        }
    }
    KKSEventFilter *ef = new KKSEventFilter (iW);
    iW->setWindowTitle (title);
    tv->viewport()->installEventFilter (ef);
    if (rootR)
        rootR->release ();

    //QAbstractItemDelegate *iDeleg = tv->itemDelegate ();
    //qDebug () << __PRETTY_FUNCTION__ << iDeleg;
/*
    connect (iW, SIGNAL (saveRubric (KKSRubric *, bool)), this, SLOT (saveRubric (KKSRubric *, bool)) );
    connect (iW, SIGNAL (rubricItemRequested (bool)), this, SLOT (rubricItemUpload(bool)) );
    connect (iW, SIGNAL (rubricItemCreationRequested (const KKSRubric *, QAbstractItemModel*, const QModelIndex&)), this, SLOT (rubricItemCreate(const KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
    connect (iW, SIGNAL (openRubricItemRequested (int)), this, SLOT (openRubricItem (int)) );
    connect (iW, SIGNAL (loadStuffModel (RubricForm *)), this, SLOT (loadRubricPrivilegies(RubricForm *)) );
    connect (iW, SIGNAL (loadSearchtemplate (RubricForm *)), this, SLOT (loadSearchTemplate (RubricForm *)) );
    connect (iW, SIGNAL (loadCategory (RubricForm *)), this, SLOT (loadCategory (RubricForm *)) );
    connect (iW, SIGNAL (rubricAttachmentsView (QAbstractItemModel *, const KKSRubric *)), this, SLOT (viewAttachments (QAbstractItemModel *, const KKSRubric *)) );
    connect (iW, SIGNAL (copyFromRubr(KKSRubric *, QAbstractItemModel *, const QModelIndex&)), this, SLOT (copyFromRubric (KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
    connect (iW, SIGNAL (initAttachmentsModel (const KKSRubric *)), this, SLOT (initRubricAttachments (const KKSRubric *)) );
    connect (iW, SIGNAL (appendRubricItemIntoModel (QAbstractItemModel *, const KKSRubricItem * )), this, SLOT (appendRubricItem (QAbstractItemModel *, const KKSRubricItem *)) );
    connect (iW, SIGNAL (setSyncIO(const QList<int>&)), this, SLOT (setSyncSettings (const QList<int>&)) );
    connect (iW, SIGNAL (putIOSIntoRubr (const QList<int>&, const KKSRubric*)), this, SLOT (putIntoRubr (const QList<int>&, const KKSRubric*)) );
    connect (iW, SIGNAL (sendIOS (const QList<int>&)), this, SLOT (sendDocs (const QList<int>&)) );
    connect (iW, SIGNAL (setAccessIOS (const QList<int>&)), this, SLOT (setAccessDocs (const QList<int>&)) );
    
    connect (this, SIGNAL (rubricAttachments (QAbstractItemModel *)), iW, SLOT (slotInitAttachmentsModel (QAbstractItemModel *)) );
*/
    return iW;
}

void KKSRubricFactory :: saveRubric (KKSRubric * rootR, bool isMyDocs)
{
    int res = ppf->updateRubricators (rootR, isMyDocs);

    KKSIncludesWidget *wParent = qobject_cast<KKSIncludesWidget *>(this->sender ());
    if (res != OK_CODE)
    {
        QMessageBox::warning (wParent, tr ("Rubricators"), tr ("Cannot save rubricators"), QMessageBox::Ok);
    }
    else
        wParent->setSaved (true);
}

void KKSRubricFactory :: rubricItemCreate (const KKSRubric * r, QAbstractItemModel * itemModel, const QModelIndex& parent)
{
    KKSIncludesWidget *editor = qobject_cast<KKSIncludesWidget *>(this->sender());
    if (!editor)
        return;

    if (!r)
        return;

    KKSCategory * c = r->getCategory();
    if (!c)
        return;

    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   -1, 
                                                   KKSList<const KKSFilterGroup*>(), 
                                                   "",
                                                   c,
                                                   false,
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   editor,
                                                   Qt::Dialog);

    //int res = objEditor->exec();
    //Q_UNUSED (res);
    connect (objEditor, SIGNAL (closeEditor()), this, SLOT (objEditorClosed ()) );
    KKSObjectExemplar * pObjectRec = objEditor->getObjectEx();
    ioRubrs.insert(pObjectRec, const_cast<KKSRubric *>(r));
    ioModels.insert(pObjectRec, itemModel);
    ioParents.insert(pObjectRec, parent);
    includesW.insert(pObjectRec, editor);
    oef->sendEditor (objEditor);
/*
    int idObject = -1;
    QString name;


    KKSObject * o = objEditor->getObj();
    if(!o){
        delete objEditor;
        return;
    }
	
    idObject = o->id();
    name = o->name();

    if(idObject < 0)
        return;

    editor->slotAddRubricItem (idObject, name);
*/
    objEditor->setAttribute(Qt::WA_DeleteOnClose);

}

void KKSRubricFactory :: rubricItemUpload (const KKSRubric *r, bool forRecords, QAbstractItemModel * itemModel)
{
    KKSIncludesWidget *editor = qobject_cast<KKSIncludesWidget *>(this->sender());
    if(!editor || !r || !itemModel)
        return;

    KKSList<const KKSFilter*> filters;
    
    KKSObject * o = loader->loadIO(IO_IO_ID, true);
    if(!o)
        return;
    KKSCategory * c = o->category()->tableCategory();
    if(!c){
        o->release();
        return;
    }

    //KKSObjEditorFactory * oef = kksSito->oef();

    int idUser = loader->getUserId();
    KKSFilter * filter = c->createFilter(ATTR_AUTHOR, QString::number(idUser), KKSFilter::foEq);
                        //: c->createFilter(ATTR_ID, QString::number(idUser), KKSFilter::foEq); // 
    if(!filter){
        o->release();
        return;
    }
    filters.append(filter);
    filter->release();
    if (forRecords)
    {
        KKSFilter * filterTab = c->createFilter (8, QString ("is not null"), KKSFilter::foIsNotNull);
        filters.append (filterTab);
        filterTab->release ();
    }
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   IO_IO_ID, 
                                                   filterGroups, 
                                                   "",
                                                   c,
                                                   true,
                                                   QString(),
                                                   false,
                                                   Qt::WindowModal,
                                                   editor,
                                                   Qt::Dialog);
    int res = objEditor->exec();
    
    int idObject = -1;
    QString name;

    if(res == QDialog::Accepted)
    {
        idObject = objEditor->getID();
        KKSObject * o = loader->loadIO(idObject, true);
        if(!o){
            delete objEditor;
            return;
        }
        name = o->name();
        if (r->getCategory())
        {
            const KKSCategory * c = o->category();
            if (c->id() != r->getCategory()->id())
            {
                int res = QMessageBox::question(editor,tr("Add document into rubric"), tr("You are put document of category %1 to rubric with category %2.\n Do you want to proceed ?").arg (c->id()).arg(r->getCategory()->id()), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if (res != QMessageBox::Yes)
                {
                    o->release ();
                    return;
                }
            }
        }
        o->release();
    }
    else
    {
        o->release ();
        return;
    }

    //editor->slotAddRubricItem (idObject, name);
    const KKSRubricItem * equalItem = r->itemForId(idObject);
    if(equalItem){
        QMessageBox::critical(editor, 
                             tr("Error"), 
                             tr("You cannot add one item to rubric twise"), 
                             QMessageBox::Ok);
        return;
    }

    KKSRubricItem * item = new KKSRubricItem(idObject, name, false);
    (const_cast<KKSRubric *>(r))->addItem(item);
    this->appendRubricItem(itemModel, item);
    editor->setSaved(false);
    
    delete objEditor;
    o->release();
}


void KKSRubricFactory :: appendRecord (int idObject, const KKSRubric* r, QAbstractItemModel * attachModel, const QModelIndex& parent)
{
    if (idObject <=0 || !r || !attachModel)
        return;
    
    const KKSRubricItem * rItem (0);
    KKSObject * o = loader->loadIO(idObject, true);
    if(!o){
        return;
    }
    QWidget * objEditor = qobject_cast<QWidget *>(this->sender());
    KKSList<const KKSFilterGroup *> recFilterGroups;
    KKSObjEditor * refObjEditor = oef->createObjRecEditor (IO_IO_ID,
                                                           idObject,
                                                           recFilterGroups,
                                                           "",
                                                           o->category()->tableCategory(),
                                                           true,
                                                           false,
                                                           Qt::WindowModal,
                                                           objEditor,
                                                           Qt::Dialog);
    if (refObjEditor->exec() == QDialog::Accepted)
    {
        int idObjRec = refObjEditor->getID();//recWidget->getID();
        KKSObjectExemplar * rec = loader->loadEIO(idObjRec, o);
        if (!rec)
        {
            o->release ();
            return;
        }
        rItem = new KKSRubricItem (idObjRec, rec->name());
        (const_cast<KKSRubric *>(r))->addItem (rItem);
        int nr = attachModel->rowCount (parent);
        attachModel->insertRows (nr, 1, parent);
        bool isRubrSet = attachModel->setData (attachModel->index (nr, 0, parent), QVariant::fromValue<const KKSRubricBase *>(rItem), Qt::UserRole+1);
        if (!isRubrSet)
        {
            o->release ();
            rec->release ();
            return;
        }
        rec->release ();
    }
    qDebug () << __PRETTY_FUNCTION__ << r->items().count();
    o->release();
    
}
void KKSRubricFactory :: openRubricItem (int idObject)
{
    KKSIncludesWidget *editor = qobject_cast<KKSIncludesWidget *>(this->sender());
    if(!editor)
        return;

    KKSList<const KKSFilter*> filters;
    
    KKSObject * o = loader->loadIO (idObject, true);
    if(!o)
        return;
    KKSCategory * c = o->category();//->tableCategory();
    if(!c){
        o->release();
        return;
    }

    KKSList<const KKSFilterGroup *> filterGroups;
    bool mode (editor->windowModality () == Qt::NonModal);

    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   idObject, 
                                                   filterGroups, 
                                                   "",
                                                   c,
                                                   !mode,
                                                   QString(),
                                                   false,
                                                   editor->windowModality(),
                                                   0,
                                                   Qt::Dialog);

    if (mode)
        emit objEditorCreated (objEditor);

    o->release();
}

void KKSRubricFactory :: loadRubricPrivilegies (RubricForm * rForm)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!rForm)
        return;

    KKSAccessEntity * acl = new KKSAccessEntity();
    KKSStuffForm * sForm = stf->createStuffEditorForm (acl, 1, rForm);
    acl->release ();
    rForm->setStuffForm (sForm);
    //QAbstractItemModel * sModel = stf->getStuffModel();
    //rForm->setStuffModel (sModel);
}

void KKSRubricFactory :: loadSearchTemplate (RubricForm * rForm)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!rForm)
        return;

    KKSSearchTemplate * st = oef->loadSearchTemplate ();
    rForm->setSearchTemplate (st);
    if (st)
        st->release ();
//    QAbstractItemModel
}

void KKSRubricFactory :: loadCategory (RubricForm * rForm)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!rForm)
        return;

    KKSObject *refObj = loader->loadIO (IO_CAT_ID, true);
    if (!refObj)
        return;
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    KKSList<const KKSFilterGroup*> filters;
    KKSCategory * ct = c->tableCategory ();
    if (!ct){
        refObj->release();
        return;
    }

    QString value = QString ("select id from io_categories where id_io_category_type != 10 and not is_system");
    const KKSFilter * f = ct->createFilter (ATTR_ID, value, KKSFilter::foInSQL);
    if (!f){
        refObj->release();
        return;
    }

    KKSList<const KKSFilter*> fl;
    fl.append (f);
    f->release ();
    KKSFilterGroup * fg = new KKSFilterGroup (true);
    fg->setFilters (fl);
    filters.append (fg);
    fg->release ();
    KKSObjEditor * recEditor = oef->createObjRecEditor (IO_IO_ID, 
                                                        refObj->id (), 
                                                        filters, 
                                                        "",
                                                        c,
                                                        true, false,
                                                        Qt::ApplicationModal,
                                                        rForm, 
                                                        Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    if (recEditor->exec () == QDialog::Accepted)
    {
        int idCat = recEditor->getID();
        KKSCategory * cat = loader->loadCategory (idCat);
        if (!cat)
        {
            refObj->release ();
            return;
        }
        rForm->setCategory (cat);
        cat->release ();
    }
    refObj->release ();
}

void KKSRubricFactory :: viewAttachments (QAbstractItemModel * attachModel, const KKSRubric * r)
{
    if (!attachModel || !r || !r->getCategory())
        return;

    KKSList<const KKSRubricItem *> docList = r->items();
    KKSCategory * c = r->getCategory();
    QAbstractItemModel * objModel = attachModel;
    while (qobject_cast<QAbstractProxyModel *>(objModel))
        objModel = qobject_cast<QAbstractProxyModel *>(objModel)->sourceModel ();
    if (objModel->rowCount () > 0)
    {
        int nr = objModel->rowCount ();
        objModel->removeRows (0, nr);
    }

    if (objModel->columnCount () > 0)
    {
        int nc = objModel->columnCount ();
        objModel->removeColumns (0, nc);
    }

    //QString aNameCode ("name");
    //KKSCategoryAttr * cAName = c->attribute (aNameCode);
    const KKSCategory * ct = c && c->tableCategory() ? c->tableCategory() : c;
    KKSMap<int, KKSCategoryAttr *> cAttrs = ct->attributes();
    int nCols = cAttrs.count ()+1;

    QList<int> bkColors = ct ? ct->searchAttributesByType (KKSAttrType::atRecordColor) : QList<int>();
    if (bkColors.isEmpty() && ct)
        bkColors += ct->searchAttributesByType (KKSAttrType::atRecordColorRef);
    if (!bkColors.isEmpty())
        nCols--;
    QList<int> fgColors = ct ? ct->searchAttributesByType (KKSAttrType::atRecordTextColor) : QList<int>();
    if (fgColors.isEmpty() && ct)
        fgColors += ct->searchAttributesByType (KKSAttrType::atRecordTextColorRef);

    if (!fgColors.isEmpty())
        nCols--;
    
    objModel->insertColumns (0, nCols);
    int nRows = docList.count();
    objModel->insertRows (0, nRows);
    int i=1;
    objModel->setHeaderData (0, Qt::Horizontal, tr ("Name"), Qt::DisplayRole);
    
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa=cAttrs.constBegin(); \
            pa != cAttrs.constEnd(); pa++)
    {
        objModel->setHeaderData (i, Qt::Horizontal, pa.value()->title(), Qt::DisplayRole);
        if (pa.value()->type()->attrType () == KKSAttrType::atRecordColor ||
            pa.value()->type()->attrType () == KKSAttrType::atRecordColorRef ||
            pa.value()->type()->attrType () == KKSAttrType::atRecordTextColor ||
            pa.value()->type()->attrType () == KKSAttrType::atRecordTextColorRef )
            continue;
        i++;
    }
    
    for (int i=0; i<nRows; i++)
    {
        int j=1;
        int id_io = docList[i]->id();
        KKSObject * cio = loader->loadIO (id_io);
        if (!cio)
            continue;
        QModelIndex nIndex = objModel->index (i, 0);
        objModel->setData (nIndex, cio->name(), Qt::DisplayRole);
        objModel->setData (nIndex, cio->id(), Qt::UserRole);
        QVariant bkColVal;

        if (!bkColors.isEmpty())
        {
            int idAttr = bkColors[0];//всегда берем первый атрибут, который ссылаетс€ на цвет
            KKSAttrView * vcol = new KKSAttrView (*(c->attribute(idAttr)));
            //QRgb rgb_col (0xFFFFFFFF);
            QVariant vc;
            KKSAttrValue * av = cio->attrValueId (vcol->id());
            if (av && !av->value ().value().isEmpty() && vcol->type()->attrType() == KKSAttrType::atRecordColor)
            {
                vc = av->value ().valueVariant ();//QColor::fromRgba(rgb_col);
                qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>() << av->value().valueVariant();
                bkColVal = QVariant (QBrush (vc.value<QColor>()));
            }
            else if (av && vcol->type()->attrType() == KKSAttrType::atRecordColorRef)
            {
                int idVal = av->value ().value().toInt ();
                KKSObject * objCRef = loader->loadIO (vcol->tableName(), true);
                
                //атрибут ссылаетс€ на значение в некоторой таблице, 
                //значит категори€ соответствующего »ќ описывает таблицу. ѕоэтому код атрибута можно использовать в качестве ключа
                KKSCategory * cRef = objCRef->category()->tableCategory();
                if(!cRef){
                    objCRef->release();
                    continue;
                }
                
                //cRef €вл€етс€ табличной категорией, поэтому в ней все атрибуты имеют уникальные коды
                //использование данного вызова допустимо
                KKSAttribute * a = cRef->attribute(vcol->columnName());
                if(!a){
                    objCRef->release();
                    continue;
                }

                KKSObjectExemplar * colorC = loader->loadEIO (idVal, objCRef);
                KKSAttrValue * avR = colorC->attrValue (a->id());
                
                //rgb_col = av->value().value().toUInt();
                vc = avR->value().valueVariant();//avR->value ().valueVariant ();//(QColor::fromRgba(rgb_col));//d->fields().value (vcol->code().toLower()));
                colorC->release ();
                objCRef->release ();
                //qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
                bkColVal = QVariant (QBrush (vc.value<QColor>()));
            }
            objModel->setData (nIndex, bkColVal, Qt::BackgroundRole);//Qt::UserRole+1);
            vcol->release ();
        }

        QVariant fgColVal;
        if (!fgColors.isEmpty())
        {
            int idAttr = fgColors[0];
            KKSAttrView * vcol = new KKSAttrView (*(c->attribute(idAttr)));
            //QRgb rgb_colf (0x00000000);
            QVariant vc;
            KKSAttrValue * av = cio->attrValueId (vcol->id());
            if (av && !av->value ().value().isEmpty() && vcol->type()->attrType() == KKSAttrType::atRecordTextColor)
            {
                vc = av->value().valueVariant ();//(QColor::fromRgba(rgb_colf));
                qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
                fgColVal = QVariant (QBrush (vc.value<QColor>()));
            }
            else if (vcol->type()->attrType() == KKSAttrType::atRecordTextColorRef)
            {
                int idVal = av->value ().value().toInt ();
                KKSObject * objCRef = loader->loadIO (vcol->tableName(), true);

                //атрибут ссылаетс€ на значение в некоторой таблице, 
                //значит категори€ соответствующего »ќ описывает таблицу. ѕоэтому код атрибута можно использовать в качестве ключа
                KKSCategory * cRef = objCRef->category()->tableCategory();
                if(!cRef){
                    objCRef->release();
                    continue;
                }
                
                //cRef €вл€етс€ табличной категорией, поэтому в ней все атрибуты имеют уникальные коды
                //использование данного вызова допустимо
                KKSAttribute * a = cRef->attribute(vcol->columnName());
                if(!a){
                    objCRef->release();
                    continue;
                }

                KKSObjectExemplar * colorC = loader->loadEIO (idVal, objCRef);
                KKSAttrValue * avR = colorC->attrValue (a->id());

                //rgb_colf = av->value().value().toUInt();
                vc = avR->value().valueVariant ();//(QColor::fromRgba(rgb_colf));//d->fields().value (vcol->code().toLower()));
                colorC->release ();
                objCRef->release ();
                //qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
                fgColVal = QVariant (QBrush (vc.value<QColor>()));
            }
            objModel->setData (nIndex, fgColVal, Qt::ForegroundRole);//Qt::UserRole+2);
            vcol->release ();
        }

        for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa=cAttrs.constBegin(); \
             pa != cAttrs.constEnd(); pa++)
        {
            QModelIndex wIndex = objModel->index (i, j);
            if (pa.value()->type()->attrType () == KKSAttrType::atRecordColor ||
                pa.value()->type()->attrType () == KKSAttrType::atRecordColorRef ||
                pa.value()->type()->attrType () == KKSAttrType::atRecordTextColor ||
                pa.value()->type()->attrType () == KKSAttrType::atRecordTextColorRef )
                continue;
            const KKSAttrValue * av = cio->attrValueId (pa.key());//value()->id());
            qDebug () << __PRETTY_FUNCTION__ << pa.key() << pa.value()->code() << av;
            if (av)
            {
                if( av->attribute()->type()->attrType() == KKSAttrType::atJPG || 
                   (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atJPG)
                   )
                {
                    objModel->setData (wIndex, tr("<Image data %1>").arg (i), Qt::DisplayRole);
                }
                else if( av->attribute()->type()->attrType() == KKSAttrType::atSVG || 
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atSVG)
                       )
                {
                    objModel->setData (wIndex, tr("<SVG data %1>").arg (i), Qt::DisplayRole);
                }
                else if( av->attribute()->type()->attrType() == KKSAttrType::atXMLDoc || 
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atXMLDoc)
                       )
                {
                    objModel->setData (wIndex, tr("<XML document %1>").arg (i), Qt::DisplayRole);
                }
                else if( av->attribute()->type()->attrType() == KKSAttrType::atVideo || 
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atVideo)
                       )
                {
                    objModel->setData (wIndex, tr("<Video data %1>").arg (i), Qt::DisplayRole);
                }
                else if (av->attribute()->type()->attrType() == KKSAttrType::atList ||
                         av->attribute()->type()->attrType() == KKSAttrType::atParent)
                {
                    bool ok;
                    int idVal = av->value().value().toInt(&ok);
                    if (!ok)
                        objModel->setData (wIndex, QVariant(), Qt::DisplayRole);

                    KKSObject *refObj = loader->loadIO (av->attribute()->tableName(), true);
                    if (!refObj)
                        continue;

                    //атрибут ссылаетс€ на значение в некоторой таблице, 
                    //значит категори€ соответствующего »ќ описывает таблицу. ѕоэтому код атрибута можно использовать в качестве ключа
                    KKSCategory * cRef = refObj->category()->tableCategory();
                    if(!cRef){
                        refObj->release();
                        continue;
                    }
                    
                    //cRef €вл€етс€ табличной категорией, поэтому в ней все атрибуты имеют уникальные коды
                    //использование данного вызова допустимо
                    KKSAttribute * a = cRef->attribute(av->attribute()->columnName());
                    if(!a){
                        refObj->release();
                        continue;
                    }

                    KKSObjectExemplar * oe = loader->loadEIO (idVal, refObj);
                    if (!oe){
                        refObj->release();
                        continue;
                    }

                    KKSAttrValue * avr = oe->attrValue (a->id());
                    if (!avr){
                        refObj->release();
                        continue;
                    }

                    objModel->setData (wIndex, avr->value().valueVariant(), Qt::DisplayRole);
                    oe->release ();
                    refObj->release ();
                }
                else if (av->attribute()->type()->attrType() == KKSAttrType::atDateTime)
                    objModel->setData (wIndex, QDateTime::fromString(av->value().value(), Qt::ISODate).toString("dd.MM.yyyy hh:mm:ss"), Qt::DisplayRole);
                else if (av->attribute()->type()->attrType() == KKSAttrType::atTime)
                    objModel->setData (wIndex, QDateTime::fromString(av->value().value(), Qt::ISODate).toString("hh:mm:ss"), Qt::DisplayRole);
                else if (av->attribute()->type()->attrType() == KKSAttrType::atText ||
                         av->attribute()->type()->attrType() == KKSAttrType::atString)
                {
                    //убираем спец. символы \n \r
                    QString text;
                    text = av->value().value();
                    int index1 = text.indexOf('\n');
                    int index2 = text.indexOf('\r');
                    int index = -1;
                    if(index1 > -1 || index2 > -1){
                        if(index1 == -1)
                            index = index2;
                        if(index2 == -1)
                            index = index1;
                        if(index1 > -1 && index2 > -1)
                            index = (index1 > index2) ? index2 : index1;
                    }
                    if(index > -1){
                        text = text.left(index);
                        text += " ...";
                    }
                    objModel->setData (wIndex, text, Qt::DisplayRole);
                }
                else
                    objModel->setData (wIndex, av->value().value(), Qt::DisplayRole);
            }
            //qDebug () << __PRETTY_FUNCTION__ << wIndex << bkColVal << fgColVal;
            if (bkColVal.isValid ())
                objModel->setData (wIndex, bkColVal, Qt::BackgroundRole);
            if (fgColVal.isValid ())
                objModel->setData (wIndex, fgColVal, Qt::ForegroundRole);
            j++;
        }
        cio->release ();
    }
}

void KKSRubricFactory :: copyFromRubric (KKSRubric * rDest, QAbstractItemModel * aModel, const QModelIndex& wIndex)
{
    //QWidget * parent = qobject_cast<QWidget *>(this->sender());

    Q_UNUSED(wIndex);
    Q_UNUSED(aModel);
    KKSList<const KKSFilterGroup *> filters;
    KKSObject * refIO = loader->loadIO(IO_RUBR_ID, true);
    if (!refIO)
        return;
    KKSCategory * cat = refIO->category();
    if (cat)
        cat = cat->tableCategory();
    if (!cat){
        refIO->release();
        return;
    }

    KKSObjEditor * oEditor = oef->createObjRecEditor(IO_IO_ID, IO_RUBR_ID, filters, tr("Select source rubric"), cat, true, Qt::WindowModal);
    if (oEditor && oEditor->exec() == QDialog::Accepted)
    {
        int idRubr = oEditor->getID();
        KKSRubric * rSource = loader->loadRubric(idRubr);
        if (rSource)
            rDest->addRubric (rSource);
    }
    else
        if (oEditor)
            delete oEditor;
    refIO->release();
}

void KKSRubricFactory :: initRubricAttachments (const KKSRubric * r)
{
    if (!r)
        return;

    qDebug () << __PRETTY_FUNCTION__ << r->name() << r->items().count();
    KKSMap<qint64, KKSEIOData *> rData = KKSConverter::rubricEntityToData(loader, r);
    KKSObject * refIO = loader->loadIO(IO_IO_ID);
    const KKSCategory * cat (0);
    
    //if (r->getCategory() && r->getCategory()->attributes().count() > 0)
    //    cat = r->getCategory();
    //else
    cat = refIO->category()->tableCategory();
    const KKSTemplate * t = new KKSTemplate (cat->defTemplate ());
    QAbstractItemModel * attachModel = new KKSEIODataModel (t, rData);//new QStandardItemModel (0, 0);
    int nr = attachModel->rowCount();
    //qDebug () << __PRETTY_FUNCTION__ << attachModel->rowCount() << attachModel->columnCount();
    for (int i=0; i<nr; i++)
    {
        QModelIndex iconInd = attachModel->index (i, 0);
        //
        // поскольку справочник неиерархический, то достаточно пройтись только по индексам верхнего уровн€
        //
        int id = iconInd.data (Qt::UserRole).toInt();
        //KKSObject * io = loader->loadIO (id);
        //if (!io)
        //    continue;
        const KKSRubricItem * rItem = r->itemForId (id);
//        QIcon ioIcon;// = io->icon();
//        qDebug () << __PRETTY_FUNCTION__ << rData.value(id)->fields().keys();
        QPixmap rIconP;
        if (rItem)
            rIconP.loadFromData (rItem->iconAsString().toUtf8());//rData.value(id)->fields().value("r_icon").toUtf8());
        QIcon ioIcon (rIconP);//io->icon();
        if (!ioIcon.isNull())
        {
            //qDebug () << __PRETTY_FUNCTION__ << QString::compare (rItem->iconAsString(), io->iconAsString());
            attachModel->setData (iconInd, ioIcon, Qt::DecorationRole);
        }
        else if (r->rubricType() == KKSRubricBase::atRubricCategory)
        {
            if (ioIcon.isNull()){
                if(rItem)
                    attachModel->setData (iconInd, (rItem->getIcon().isNull() ? rItem->getDefaultIcon() : rItem->getIcon()), Qt::DecorationRole);
                else
                    attachModel->setData (iconInd, KKSRubricItem::icon(), Qt::DecorationRole);
            }
            else
                attachModel->setData (iconInd, ioIcon, Qt::DecorationRole);
        }
        else{
            if(rItem)
                attachModel->setData (iconInd, (rItem->getIcon().isNull() ? rItem->getDefaultIcon() : rItem->getIcon()), Qt::DecorationRole); // rItem->getDefaultIcon()
            else
                attachModel->setData (iconInd, KKSRubricItem::icon(), Qt::DecorationRole);
        }
        //io->release ();
    }
    t->release ();
    refIO->release ();
    
    emit rubricAttachments (attachModel);
}

/* —лот добавл€ет существующий »ќ в рубрику с последующим отображением в визуальной модели
 */
void KKSRubricFactory :: appendRubricItem (QAbstractItemModel * attachModel, const KKSRubricItem * rItem)
{
    if (!attachModel || !rItem)
        return;

    KKSObject * refIO = loader->loadIO(IO_IO_ID);
    if (!refIO)
        return;

    KKSCategory * c = refIO->category();
    if (!c || !c->tableCategory())
    {
        refIO->release ();
        return;
    }
    c = c->tableCategory();
    const KKSFilter * f = c->createFilter(1, QString::number(rItem->id()), KKSFilter::foEq);
    KKSFilterGroup * fg = new KKSFilterGroup (false);
    fg->addFilter (f);
    KKSList<const KKSFilterGroup *> filters;
    filters.append (fg);
    fg->release ();
    f->release ();
    KKSMap<qint64, KKSEIOData *> itemData = loader->loadEIOList (refIO, filters);
    int cnt = attachModel->rowCount();
    if (!attachModel->insertRows (cnt, itemData.count()))
    {
        refIO->release ();
        return;
    }
    int i=0;
    for (KKSMap<qint64, KKSEIOData *>::const_iterator p = itemData.constBegin();
         p != itemData.constEnd();
         p++)
    {
        QModelIndex wIndex = attachModel->index (cnt+i, 0);
        KKSObject * io = loader->loadIO (p.key());
        attachModel->setData (wIndex, QVariant::fromValue<KKSEIOData *>(p.value()), Qt::UserRole+1);
        if (io && !io->icon().isNull())
            attachModel->setData (wIndex, io->icon(), Qt::DecorationRole);
        else
            attachModel->setData (wIndex, KKSRubricItem::icon(), Qt::DecorationRole);
        i++;
    }
    refIO->release ();
}

void KKSRubricFactory :: rubricItemCreated(KKSObjectExemplar * rec)
{
    KKSRubric * r = ioRubrs.value (rec);
    QAbstractItemModel * attachModel = ioModels.value (rec);
    QModelIndex pInd = ioParents.value (rec);
    KKSIncludesWidget * iW = includesW.value (rec);
    
    if (!r || !attachModel)
        return;
    
    KKSObject * wObj = loader->loadIO (rec->id());
    if (iW && wObj)
        iW->slotAddRubricItem(rec->id(), wObj->name());
    
    if (wObj)
        wObj->release ();
}

void KKSRubricFactory :: objEditorClosed ()
{
    KKSObjEditor * objEditor = qobject_cast<KKSObjEditor *>(this->sender());
    if (!objEditor)
        return;
    
    KKSObjectExemplar * rec = objEditor->getObjectEx();
    if (!rec)
        return;
    
    int rubrres = ioRubrs.remove (rec);
    int modres = ioModels.remove (rec);
    int pres = ioParents.remove (rec);
    int wres = includesW.remove (rec);
    qDebug () << __PRETTY_FUNCTION__ << rubrres << modres << pres << wres;
}

void KKSRubricFactory :: setSyncSettings (const QList<int>& ioIDList)
{
    if (ioIDList.isEmpty())
        return;

    KKSSyncDialog * syncD = new KKSSyncDialog ();
    KKSSyncWidget * syncW = syncD->getSyncWidget ();
    int idUser = loader->getUserId();
    KKSObject * refObj = loader->loadIO (IO_IO_ID);
    KKSObjectExemplar * wObjE = loader->loadEIO (ioIDList.at(0), refObj);
    int attrId = ATTR_ID_SYNC_TYPE;
    KKSAttribute * attr = loader->loadAttribute (attrId);
    KKSAttrValue * av = wObjE->attrValue (attrId);
    if (av)
    {
        QMap<int, QString> refValues;
        QMap<int, QString> values = loader->loadAttributeValues (attr, refValues, false, true, attr->tableName());
        QMap<int, QString>::const_iterator pv = values.constFind (av->value().value().toInt());
        QString v_str = QString();
        if (pv != values.constEnd())
            v_str = pv.value();
        syncW->setSyncType (v_str);
    }
    attr->release ();
    qint64 idObjE = wObjE->id();
    KKSCategory * c = wObjE->io()->category();
    if (!c)
        return;
    KKSCategory * ct = c->tableCategory ();
    if (!ct)
        return;
    QMap<int, QStringList> syncOrgs = loader->getSyncOrg (idObjE);
    KKSCategoryAttr * syncAttr = ct->attribute (ATTR_IO_OBJECTS_ORGANIZATION);
    KKSCategoryAttr * syncAttrType = ct->attribute (ATTR_ID_SYNC_TYPE);
    KKSAttrValue * syncAttrVal (0);//= new KKSAttrValue (sVal, syncAttr);
    KKSValue val (QString ("{}"), KKSAttrType::atCheckListEx);
    syncAttrVal = new KKSAttrValue (val, syncAttr);
    KKSAttrValue * syncAttrTypeV (0);
    KKSValue valt (QString (""), KKSAttrType::atList);
    syncAttrTypeV = new KKSAttrValue (valt, syncAttrType);
    syncW->setSyncAttrVal (syncAttrVal);
    syncW->setSyncAttrType (syncAttrVal);
    if (syncAttrVal)
        syncAttrVal->release ();
    connect (syncD, SIGNAL (loadSyncRef (QString, QWidget *, qint64)),
             oef, 
             SLOT (loadSyncType (QString, QWidget *, qint64))
            );
    connect (syncD, SIGNAL (loadAddSyncOrg (KKSAttrValue *, QAbstractItemModel *)), 
             oef,
             SLOT (loadSyncAddAttrRef (KKSAttrValue *, QAbstractItemModel *)) 
            );
    connect (syncD, SIGNAL (loadDelSyncOrg (KKSAttrValue *, const QModelIndex&, QAbstractItemModel *)),
             oef,
             SLOT (loadSyncDelAttrRef (KKSAttrValue *, const QModelIndex&, QAbstractItemModel *))
            );
    if (syncD->exec() == QDialog::Accepted)
    {
        KKSAttrValue * syncVal = syncW->getSyncAttrVal();
        KKSAttrValue * syncType = syncW->getSyncAttrType ();
        KKSObject * ioRef = loader->loadIO (IO_IO_ID);
        qDebug () << __PRETTY_FUNCTION__ << syncVal->value().value();
        for (int i=0; i<ioIDList.count(); i++)
        {
            KKSObjectExemplar * rio = loader->loadEIO (ioIDList[i], ioRef);
            if (!rio)
                continue;
            KKSAttrValue * av = rio->attrValue(syncVal->attribute()->id());
            if (!av)
                rio->addAttrValue (syncVal);
            else
            {
                rio->removeAttrValue (av);
                rio->addAttrValue (syncVal);
            }
            KKSAttrValue * avT = rio->attrValue (syncType->attribute()->id());
            if (!avT)
                rio->addAttrValue (syncType);
            else
            {
                rio->removeAttrValue (avT);
                rio->addAttrValue (syncType);
            }
            KKSObject * wObj = loader->loadIO (ioIDList[i]);
            bool ok = KKSConverter::objectFromExemplar(loader, wObj, rio);
            wObj->setIsGlobal (syncW->isCheckGlobal());
            qDebug () << __PRETTY_FUNCTION__ << wObj->isGlobal();
            int ier = ppf->updateIO (wObj);//eiof->updateEIO (rio);
            wObj->release ();
            qDebug () << __PRETTY_FUNCTION__ << ok << ier;
            rio->release ();
        }
        ioRef->release ();
    }
}

void KKSRubricFactory :: putIntoRubr (const QList<int>& ioIDList, const KKSRubric * oldRubric)
{
    if (ioIDList.isEmpty())
        return;
    KKSList<const KKSFilterGroup*> filters;
    KKSIncludesWidget * anotherW = this->createModalRubricEditor(atRootRubric, filters, false);
    KKSIncludesDialog * rubrDial = new KKSIncludesDialog (anotherW);
    anotherW->hideIncludes();
    QWidget * pWidget = qobject_cast<QWidget *>(this->sender());
    KKSIncludesWidget * iW = qobject_cast<KKSIncludesWidget *>(this->sender());
    QAbstractItemModel * attachModel = iW->rubrItemsModel();
    if (rubrDial->exec() == QDialog::Accepted)
    {
        const KKSRubric * r = anotherW->getSelectedRubric ();
        if (!r)
        {
            QMessageBox::warning (pWidget, tr("Copy documents"), tr ("Select destignation"), QMessageBox::Ok);
            return;
        }
        if (oldRubric && (oldRubric == r || oldRubric->id() == r->id() ) )
        {
            QMessageBox::warning (pWidget, tr("Copy documents"), tr ("You put documents into same rubric"), QMessageBox::Ok);
            return;
        }
        for (int i=0; i<ioIDList.count(); i++)
        {
            KKSObject * wObj = loader->loadIO (ioIDList[i]);
            if (!wObj)
                continue;
            KKSRubricItem * rItem = new KKSRubricItem (ioIDList[i], wObj->name());
            (const_cast<KKSRubric *>(r))->addItem (rItem);
            //appendRubricItem (attachModel, rItem);
            rItem->release ();
            wObj->release ();
        }
        qDebug () << __PRETTY_FUNCTION__ << r->items().count();
        ppf->updateRubrics (const_cast<KKSRubric *>(r));
        //saveRubric (iW->rootRubric(), false);
        //initRubricAttachments (r);
        
    }
    delete rubrDial;
}

void KKSRubricFactory :: sendDocs (const QList<int>& ioIDList)
{
    if (ioIDList.isEmpty())
        return;
    oef->sendIOList (ioIDList);
}

void KKSRubricFactory :: setAccessDocs (const QList<int>& ioIDList)
{
    if (ioIDList.isEmpty())
        return;
    QWidget * parentW = qobject_cast<QWidget *>(sender());
    KKSAccessEntity * acl = new KKSAccessEntity ();
    int idUser = loader->getUserId();
    KKSStuffForm * sForm = stf->createStuffEditorForm (acl, idUser, parentW, Qt::Dialog);
    KKSStuffDialog * sDial = new KKSStuffDialog (sForm);
    if (sDial->exec() == QDialog::Accepted)
    {
        KKSAccessEntity * acw = sForm->getAccessEntity();
        for (int i=0; i<ioIDList.count(); i++)
        {
            KKSObject * wObj = loader->loadIO (ioIDList[i]);
            if (!wObj)
                continue;
            wObj->setAccessRules(acw);
            ppf->updateIO (wObj);
            wObj->release ();
        }
    }
    sForm->setParent (0);
    delete sForm;
}