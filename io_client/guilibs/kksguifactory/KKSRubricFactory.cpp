#include <QTreeView>
#include <QMessageBox>
#include <QItemDelegate>
#include <QAbstractItemModel>
#include <QAbstractProxyModel>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QtDebug>

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
#include <KKSRecDialog.h>
#include <KKSObject.h>
#include <KKSObjectExemplar.h>
#include "KKSAttrType.h"
#include <KKSCategory.h>
#include <KKSAttribute.h>
#include <KKSAttrView.h>
#include <KKSTemplate.h>
#include <KKSList.h>
#include <KKSFilter.h>
#include <defines.h>
#include <KKSSearchTemplate.h>
#include "kksstuffform.h"
#include "KKSAccessEntity.h"
#include <KKSEIOData.h>
#include <KKSEIODataModel.h>
#include <KKSRubricModel.h>
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
    qDebug () << __PRETTY_FUNCTION__;
}

//���������� ��������� �������
QString KKSRubricFactory::getEditorTitle(RubricMode mode) const
{
    QString title;

    switch (mode)
    {
        case atRootRubric:
        {
            title = tr("All rubrics");
            break;
        }
        case atMyDocsRubric:
        {
            title = tr("My Documents");
            break;
        }
        case atOthers:
        {
            title = tr("All rubrics with categories");
            break;
        }
        default:
        {
            return QString::null;
            break;
        }
    }

    return title;
}

//������� ������
KKSIncludesWidget * KKSRubricFactory :: createRubricEditor (RubricMode mode, 
                                                            RubricShowMode showMode,
                                                            bool bModal,
                                                            QWidget* parent,
                                                            Qt::WindowFlags flags)
{
    bool isDocs(mode == atMyDocsRubric);

    QString title = getEditorTitle(mode);
    if(title == QString::null)
        return 0;

    //���� � ��� ���������� ������ ��� "��� ���������", 
    //�� ��������� ������������� ����� ������� 
    //� ��� ������������� ������� �� ��� ������� ������������
    if(isDocs){
        KKSRubric * rootR = loader->loadRubricators(isDocs);
        if (!rootR) {
            rootR = new KKSRubric(-1, "root rubric for all tree");
            KKSRubric * rubrMyDocs = new KKSRubric(-1, tr("My Documents"));
            if (!rubrMyDocs)
                return 0;

            rootR->addRubric(rubrMyDocs);
            int res = ppf->updateRubricators(rootR, isDocs);
            if (res != OK_CODE) {
                qCritical() << tr("Cannot load My documents");
                QMessageBox::critical(parent, tr("Rubricator"), tr("Cannot load My documents"), QMessageBox::Ok);
                return 0;
            }

            ppf->createMyDocsRubricator(rubrMyDocs->id());
        }
        
        KKSIncludesWidget *iW = createRubricRecEditor (rootR, 
                                                       KKSIncludesWidget::rsMyDocs,
                                                       showMode,
                                                       parent,
                                                       flags);


        KKSEventFilter *ef = new KKSEventFilter(iW);
        iW->setWindowTitle(title);
        QTreeView *tv = iW->tvRubr();
        tv->viewport()->installEventFilter(ef);

        if(!bModal)//���� ���� ����������� �� ��������, �� ���� �������� ������, � ���, ��� ��� �������. ����� ��� ���������� �� ������ �������� ���� MDI-����������
            emit rubricEditorCreated(iW);    

        return iW;
    }
    
    KKSMap<qint64, KKSEIOData *> rubrInfo = getRubricatorsData();

    QAbstractItemModel * rModel = new KKSRubricModel(rubrInfo);
    //rubrIO->release();

    KKSIncludesWidget *iW = new KKSIncludesWidget(0, 
                                                  isDocs ? KKSIncludesWidget::rsMyDocs : KKSIncludesWidget::rsRubricator, 
                                                  parent);

    if(showMode == smRubricsOnly)
        iW->setForRubrics(true);
    else
        iW->setForRubrics(false);

    iW->setRubrModel(rModel);
    QTreeView *tv = iW->tvRubr();
    QAbstractItemModel * rubrMod = iW->rubrModel();
    
    if (rubrMod && mode == atOthers){//���� ��� ���� � ��������� �������� � ���� ������
        int ok = addCategoriesToRubricator(rubrMod);
        
        if(ok == ERROR_CODE){
            iW->setParent (0);
            delete iW;
        
            return 0;
        }
    }

    KKSEventFilter *ef = new KKSEventFilter(iW);
    iW->setWindowTitle(title);
    tv->viewport()->installEventFilter(ef);

    initConnections(iW);

    if(!bModal)//���� ���� ����������� �� ��������, �� ���� �������� ������, � ���, ��� ��� �������. ����� ��� ���������� �� ������ �������� ���� MDI-����������        
        emit rubricEditorCreated(iW);

    return iW;
}

int KKSRubricFactory::addCategoriesToRubricator(QAbstractItemModel * rubrMod) const
{
    if(!rubrMod)
        return ERROR_CODE;

    int nr = rubrMod->rowCount();
    bool isIns = rubrMod->insertRows(nr, 1);
    if (isIns)
    {
        QModelIndex wIndex = rubrMod->index(nr, 0);
        const KKSRubricBase * rOthers = new KKSRubricOthers(-1, tr("Groupped by categories"));
        // loader->loadCatRubricators ();//
        bool isRubrDataSet = rubrMod->setData(wIndex, QVariant::fromValue<const KKSRubricBase *>(rOthers), Qt::UserRole + 1);
        if (!isRubrDataSet)
        {
            return ERROR_CODE;
        }

        KKSObject * catIO = loader->loadIO(IO_CAT_ID, true);
        if (!catIO)
        {
            return ERROR_CODE;
        }

        KKSCategory * catCatIO = catIO->category();
        if (!catCatIO)
        {
            catIO->release();
        }

        catCatIO = catCatIO->tableCategory();
        if (!catCatIO)
        {
            catIO->release();
        }
        
        //������� ������ �� ��������� ��������� � ���� ������
        KKSList<const KKSFilterGroup *> rfilters;
        const KKSFilter * f = catCatIO->createFilter(1, "select id from cGetCategoriesForRubricator()", KKSFilter::foInSQL);
        KKSList<const KKSFilter *> cFilters;
        cFilters.append (f);
        f->release();
        KKSFilterGroup * fg = new KKSFilterGroup (true);
        fg->setFilters (cFilters);
        rfilters.append (fg);
        fg->release();

        //�������� ������ (��������� � ���� ������)
        KKSMap<qint64, KKSEIOData *> catInfo = loader->loadEIOList(catIO, rfilters);
        int nrc = rubrMod->rowCount (wIndex);
        
        for (KKSMap<qint64, KKSEIOData *>::const_iterator pc = catInfo.constBegin();
                pc != catInfo.constEnd();
                pc++)
        {
            rubrMod->insertRows (nrc, 1, wIndex);
            const KKSRubric * rCat = new KKSRubric(pc.key(), pc.value()->fieldValue("name"));
            (const_cast<KKSRubric *> (rCat))->setCategorized();
            (const_cast<KKSRubricBase *> (rOthers))->addNode(rCat);
            QModelIndex wcIndex = rubrMod->index (nrc, 0, wIndex);
            rubrMod->setData (wcIndex, QVariant::fromValue<const KKSRubricBase *>(rCat), Qt::UserRole+1);
            rCat->release();
            nrc++;
        }
        catIO->release();
    }

    return OK_CODE;
}

KKSIncludesWidget * KKSRubricFactory::createRubricRecEditor (KKSRubric * rootRubric,
                                                             KKSIncludesWidget::RubricatorSource rSource,
                                                             RubricShowMode showMode,
                                                             QWidget *parent,
                                                             Qt::WindowFlags flags)
{
    KKSIncludesWidget * iW = new KKSIncludesWidget (rootRubric, rSource, parent, flags);
    
    if(showMode == smRubricsOnly)
        iW->setForRubrics(true);
    else
        iW->setForRubrics(false);

    initConnections(iW);
    //emit rubricEditorCreated(iW);

    return iW;
}

void KKSRubricFactory::initConnections(KKSIncludesWidget * iW) const
{
    if(!iW)
        return;

    //���������� ������� � ��
    connect(iW, SIGNAL(saveRubric(KKSRubric *, bool)), this, SLOT(saveRubric(KKSRubric *, bool)));
    //���������� ������������� �� ��� ������ ����������� � �������
    connect(iW, SIGNAL(rubricItemRequested(const KKSRubric*, bool, QAbstractItemModel *)), this, SLOT(rubricItemUpload(const KKSRubric *, bool, QAbstractItemModel *)));
    //�������� ������ �� � �������, ��� ������� ������ ��������� (�� ���� ���������)
    connect(iW, SIGNAL(rubricItemCreationRequested(const KKSRubric *, QAbstractItemModel*, const QModelIndex&)), this, SLOT(rubricItemCreate(const KKSRubric *, QAbstractItemModel *, const QModelIndex&)));
    
    connect (iW, SIGNAL(signalShowReportViewer(qint64)), this, SIGNAL(showReportViewer(qint64)));
    connect (iW, SIGNAL(signalShowReportEditor(qint64)), this, SIGNAL(showReportEditor(qint64)));

    //�������� ��������� �� ��� ��������� ��
    connect(iW, SIGNAL(openRubricItemRequested(int)), this, SLOT(openRubricItem(int)));
    //�������� ��������� �� ��� �������� ������ �����������
    connect(iW, SIGNAL(openRubricItemRequested(int, int)), this, SLOT(openRubricItem(int, int)));

    connect(iW, SIGNAL(loadStuffModel(RubricForm *)), this, SLOT(loadRubricPrivilegies(RubricForm *)));
    connect(iW, SIGNAL(loadSearchtemplate(RubricForm *)), this, SLOT(loadSearchTemplate(RubricForm *)));
    connect(iW, SIGNAL(loadCategory(RubricForm *)), this, SLOT(loadCategory(RubricForm *)));
    connect(iW, SIGNAL(loadIO(RubricForm *)), this, SLOT(loadIO(RubricForm *)));
    
    
    connect(iW, SIGNAL(rubricAttachmentsView(QAbstractItemModel *, const KKSRubric *)), this, SLOT(viewAttachments(QAbstractItemModel *, const KKSRubric *)));

    connect(iW, SIGNAL(copyFromRubr(KKSRubric *, QAbstractItemModel *, const QModelIndex&)), this, SLOT(copyFromRubric(KKSRubric *, QAbstractItemModel *, const QModelIndex&)));
    connect(iW, SIGNAL(initAttachmentsModel(const KKSRubric *, bool)), this, SLOT(initRubricAttachments(const KKSRubric *, bool)));
    connect(iW, SIGNAL(appendRubricItemIntoModel(QAbstractItemModel *, const KKSRubricItem *)), this, SLOT(appendRubricItem(QAbstractItemModel *, const KKSRubricItem *)));
    connect(iW, SIGNAL(setSyncIO(const QList<int>&)), this, SLOT(setSyncSettings(const QList<int>&)));
    connect(iW, SIGNAL(putIOSIntoRubr(const QList<int>&, const KKSRubric*)), this, SLOT(putIntoRubr(const QList<int>&, const KKSRubric*)));
    connect(iW, SIGNAL(sendIOS(const QList<int>&)), this, SLOT(sendDocs(const QList<int>&)));
    connect(iW, SIGNAL(setAccessIOS(const QList<int>&)), this, SLOT(setAccessDocs(const QList<int>&)));
    connect(iW, SIGNAL(rubricRequested(KKSRubric *, int, QAbstractItemModel*, const QModelIndex&)), this, SLOT(loadRubric(KKSRubric *, int, QAbstractItemModel*, const QModelIndex&)));
    connect(iW, SIGNAL(rubricCategoryRequested(KKSRubric *)), this, SLOT(loadCategoryIntoRubric(KKSRubric *)));//������ ��������� � ���� �������

    connect(this, SIGNAL(rubricAttachments(QAbstractItemModel *, KKSIncludesWidget *)), iW, SLOT(slotInitAttachmentsModel(QAbstractItemModel *, KKSIncludesWidget *)));
}

void KKSRubricFactory::saveRubric(KKSRubric * rootR, bool isMyDocs)
{
    int res = ppf->updateRubricators(rootR, isMyDocs);

    KKSIncludesWidget *wParent = qobject_cast<KKSIncludesWidget *>(this->sender());
    
    if (res != OK_CODE) {
        qCritical() << tr("Cannot save rubricators");
        QMessageBox::critical(wParent, tr("Rubricators"), tr("Cannot save rubricators"), QMessageBox::Ok);
    } 
    else
        wParent->setSaved(true);
}

void KKSRubricFactory::rubricItemCreate(const KKSRubric * r, QAbstractItemModel * itemModel, const QModelIndex& parent)
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

    //����� ��� ������������, ����� ���������, �� ����� ������� � ��������� ������ ���� ������� �������� ��������� ��
    //� ������� �� ��� �������� �� �� ������������ ������ cioSaved, ������� ������� �������� ������ � �������������� ������ rubricItemCreated
    connect(objEditor, SIGNAL(closeEditor()), this, SLOT(objEditorClosed()));
    
    KKSObjectExemplar * pObjectRec = objEditor->getObjectEx();
    
    ioRubrs.insert(pObjectRec, const_cast<KKSRubric *> (r));
    ioModels.insert(pObjectRec, itemModel);
    ioParents.insert(pObjectRec, parent);
    includesW.insert(pObjectRec, editor);
    
    //oef->sendEditor(objEditor);

    //���� ������� ��� ����� �������� � ������ KKSMainWindow::slotCreateNewObjEditor
    //objEditor->setAttribute(Qt::WA_DeleteOnClose);

    bool mode(editor->windowModality() == Qt::NonModal);
    if (mode)
        emit objEditorCreated(objEditor);
}

//��������� ������������� �� ��� ������ ����������� � ������� (����� �� ��� ������ ����������� �� ������ ������������, ������������ � ��������� ��)
void KKSRubricFactory::rubricItemUpload(const KKSRubric *r, bool forRecords, QAbstractItemModel * itemModel) 
{
    KKSIncludesWidget *editor = qobject_cast<KKSIncludesWidget *>(this->sender());
    if (!editor || !r || !itemModel)
        return;

    KKSObject * o = loader->loadIO(IO_IO_ID, true);
    if (!o)
        return;
    KKSCategory * c = o->category()->tableCategory();
    if (!c) {
        o->release();
        return;
    }

    //���� ����� ���� � �������� � ������ ����������� � �� ����� �������������� ������
    //�� ��� ���� ���� ������� ������ ������ �� ����� ��
    if(forRecords && r->getIO())
    {
        KKSObject * o = r->getIO();
        recRubricItemUpload(r, itemModel, o, editor);
        return;
    }

    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;

    //��� ���������� ������ ����������� ��� ������� ���� ���������� ������������ ������� ����������
    if (forRecords) {
        KKSFilter * filterTab = c->createFilter(ATTR_TABLE_NAME, QString("is not null"), KKSFilter::foIsNotNull);
        filters.append(filterTab);
        filterTab->release();
        
        //����� ���� ��������� ��������� �����������, ��������� ��� �� ����������� �� q_base_table
        //������ ��������� ���������� ������� (������� report) ����������� �� q_base_table � ����� ����� 300
        //��� ���� �������� � ������
        filterTab = c->createFilter(ATTR_ID, QString("300"), KKSFilter::foGrEq);
        filters.append(filterTab);
        filterTab->release();
    }

    //���� ��� ������� ������ ���������, �� � ������ ������� �������, ��� ������ ������������ ������ �� ������ ���� ���������
    if(r->getCategory()){
        KKSFilter * filter = c->createFilter(ATTR_ID_IO_CATEGORY, QString::number(r->getCategory()->id()), KKSFilter::foEq);
        if (!filter) {
            o->release();
            return;
        }

        filters.append(filter);
        filter->release();
    }

    //���� �� ��������� � ������� ��
    //� ���� � ������� ��� ���� ������������� ��, �� ������� ������, ����������� �� �� ������
    //������ �������� ���� � ��� �� �� � ������� ������
    if (!forRecords && itemModel->rowCount() > 0)
    {
        int n = itemModel->rowCount();
        for (int i=0; i<n; i++)
        {
            QModelIndex wInd = itemModel->index (i, 0);
            qint64 idObject = wInd.data (Qt::UserRole).toLongLong();
            KKSFilter * f = c->createFilter(1, QString::number (idObject), KKSFilter::foNotEq);
            filters.append (f);
            f->release();
        }
    }

    if(filters.count() > 0){
        KKSFilterGroup * group = new KKSFilterGroup(true);
        group->setFilters(filters);
        filterGroups.append(group);
        group->release();
    }

    
    KKSRecDialog *objEditor = oef->createObjRecEditor(IO_IO_ID,
            IO_IO_ID,
            filterGroups,
            tr("Select reference IO"),
            c,
            true,
            false,
            Qt::WindowModal,
            editor,
            Qt::Dialog);

    //���� �������� ������ �����������, �� ���������� �� �������� �� ������ ������������, ������� �� ������ ����� ��������� ����� ����������� ����� ������������
    if (forRecords)
        objEditor->setSelectionMode(QAbstractItemView::SingleSelection);

    int res = objEditor->exec();

    int idObject = -1;
    QString name;

    if (res == QDialog::Accepted)
    {
        //���� ������������ � ��������� �� ����� �� ��, �� ������� �������� �� (���) � �������
        QList<qint64> idObjectList = objEditor->getIDList();
        int no = idObjectList.count();
        for (int i=0; i<no; i++)
        {
            int idObject = idObjectList[i];
            KKSObject * o = loader->loadIO(idObject, true);
            if (!o) {
                objEditor->setParent (0);
                delete objEditor;
                return;
            }
            
            if (forRecords)
            {
                recRubricItemUpload(r, itemModel, o, editor);

                o->release();
                objEditor->setParent(0);
                delete objEditor;

                return;
            }
            
            name = o->name();
            if (r->getCategory())
            {
                const KKSCategory * c = o->category();
                if (c->id() != r->getCategory()->id())
                {
                    qCritical() << tr("You are put document of category\n\"%1\"\nto rubric with category:\n\"%2\".\nThis is not allowed!")
                                                        .arg(c->name())
                                                        .arg(r->getCategory()->name());
                    QMessageBox::critical(editor, 
                                                    tr("Add document into rubric"), 
                                                    tr("You are put document of category\n\"%1\"\nto rubric with category:\n\"%2\".\nThis is not allowed!")
                                                        .arg(c->name())
                                                        .arg(r->getCategory()->name()));
                    objEditor->setParent (0);
                    delete objEditor;
                    o->release();
                    return;
                }
            }
            
            //����� �������� ������, ����� ����������� � ������� ��

            o->release();
            
            //������� �� �� �������� � �������, �������� ��� � �������
            KKSRubricItem * item = new KKSRubricItem(idObject, name, false);
            (const_cast<KKSRubric *> (r))->addItem(item);
            
            //��������� �� � ������ � ���� KKSEIOData
            this->appendRubricItem(itemModel, item);
            
            //�������� �������� �� ��� ������������� (��� �������� ����� ������)
            editor->setSaved(false);
        
        }//��� ������� ���������� �� (����� �����)
    }
    else
    {
        o->release();
        return;
    }

    //editor->slotAddRubricItem (idObject, name);
    const KKSRubricItem * equalItem = r->itemForId(idObject);
    if (equalItem) {
        qCritical() << tr("You cannot add one item to rubric twise");
        QMessageBox::critical(editor,
                tr("Error"),
                tr("You cannot add one item to rubric twise"),
                QMessageBox::Ok);
        return;
    }

    delete objEditor;
    o->release();
}

void KKSRubricFactory::recRubricItemUpload(const KKSRubric *r, QAbstractItemModel * itemModel, KKSObject * o, KKSIncludesWidget *editor)
{
    if(!itemModel || !editor)
        return;

    //���� ��������� ���, �� ���� ��� ������� �������� ��, � ������� ���������� ������ ���������� �����������
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;

    //filterGroups.clear();
    //filters.clear ();
/*
    if (itemModel->rowCount() > 0)
    {
        int n = itemModel->rowCount();
        for (int i=0; i<n; i++)
        {
            QModelIndex wInd = itemModel->index (i, 0);
            qint64 idRecord = wInd.data (Qt::UserRole).toLongLong();
            KKSFilter * f = c->createFilter(ATTR_ID, QString::number (idRecord), KKSFilter::foNotEq);
            filters.append (f);
            f->release();
        }
    }
*/
    const KKSCategory * c = o->category();
    if (!c)
    {
        //objEditor->setParent (0);
        //delete objEditor;
        //o->release();
        return;
    }

    c = c->tableCategory();
    
    if (itemModel->rowCount() > 0)
    {
        int n = itemModel->rowCount();
        for (int i=0; i<n; i++)
        {
            QModelIndex wInd = itemModel->index (i, 0);
            qint64 idRecord = wInd.data (Qt::UserRole).toLongLong();
            
            KKSFilter * f = c->createFilter(ATTR_ID, QString::number (idRecord), KKSFilter::foNotEq);
            filters.append (f);
            f->release ();
        }
    }

    if(filters.count() > 0){
        KKSFilterGroup * group = new KKSFilterGroup(true); //������ �
        group->setFilters(filters);
        filterGroups.append(group);
        group->release();
    }

    KKSRecDialog *recEditor = oef->createObjRecEditor(IO_IO_ID,
            o->id(),
            filterGroups,
            tr("Select record for rubric"),
            c,
            true,
            false,
            Qt::WindowModal,
            editor,
            Qt::Dialog);

    if (!recEditor || recEditor->exec() != QDialog::Accepted)
    {
        if (recEditor)
        {
            recEditor->setParent (0);
            delete recEditor;
        }

        return;
    }

    //���� ������������ ����� ����� �� �� - ��������� ������
    //��� ���� ������� ������� ���������� � ���
    //(����������� ��� ������ �� ��������� �������)
    QList<qint64> idRecList = recEditor->getIDList();
    KKSMap<qint64, KKSEIOData *> itemsData = loader->loadRecList (idRecList);

    for (KKSMap<qint64, KKSEIOData *>::const_iterator p = itemsData.constBegin();
            p != itemsData.constEnd();
            p++) 
    {
        //�������� � ������� ����
        QString rubrItemName = p.value()->sysFieldValue("rr_name");
        qint64 idRec = p.value()->sysFieldValue("id").toLongLong();
        KKSRubricItem * item = new KKSRubricItem(idRec, rubrItemName, false);
        (const_cast<KKSRubric *> (r))->addItem(item);
        item->release();

        //������ ������� ��� � ������
        int nr = itemModel->rowCount();
        bool isIns = itemModel->insertRows (nr, 1);
        QModelIndex rIndex = itemModel->index (nr, 0);
        
        KKSEIOData * itemInfo = p.value();
        
        bool isRubrSet = itemModel->setData (rIndex, QVariant::fromValue<KKSEIOData*>(itemInfo), Qt::UserRole+1);
        itemModel->setData (rIndex, KKSRubricItem::icon(), Qt::DecorationRole);
        itemModel->setData (rIndex, idRec, Qt::UserRole);
        
        //this->appendRubricItem(itemModel, item);
        editor->setSaved(false);
        editor->slotAddRubricItem((int)idRec, rubrItemName);
        //ioRec->release();
    }

    recEditor->setParent (0);
    delete recEditor;
}

void KKSRubricFactory::appendRecord(int idObject, const KKSRubric* r, QAbstractItemModel * attachModel, const QModelIndex& parent) {
    if (idObject <= 0 || !r || !attachModel)
        return;

    const KKSRubricItem * rItem(0);
    KKSObject * o = loader->loadIO(idObject, true);
    if (!o) {
        return;
    }
    QWidget * objEditor = qobject_cast<QWidget *>(this->sender());
    KKSList<const KKSFilterGroup *> recFilterGroups;
    KKSRecDialog * refObjEditor = oef->createObjRecEditor(IO_IO_ID,
            idObject,
            recFilterGroups,
            "",
            o->category()->tableCategory(),
            true,
            false,
            Qt::WindowModal,
            objEditor,
            Qt::Dialog);
    if (refObjEditor->exec() == QDialog::Accepted) {
        int idObjRec = refObjEditor->getID(); //recWidget->getID();
        KKSObjectExemplar * rec = loader->loadEIO(idObjRec, o);
        if (!rec) {
            o->release();
            return;
        }
        rItem = new KKSRubricItem(idObjRec, rec->name());
        (const_cast<KKSRubric *> (r))->addItem(rItem);
        int nr = attachModel->rowCount(parent);
        attachModel->insertRows(nr, 1, parent);
        bool isRubrSet = attachModel->setData(attachModel->index(nr, 0, parent), QVariant::fromValue<const KKSRubricBase *>(rItem), Qt::UserRole + 1);
        if (!isRubrSet) {
            o->release();
            rec->release();
            return;
        }
        rec->release();
    }
    qDebug() << __PRETTY_FUNCTION__ << r->items().count();
    o->release();

}

void KKSRubricFactory::openRubricItem(int idObject) 
{
    
    KKSIncludesWidget *editor = qobject_cast<KKSIncludesWidget *>(this->sender());
    if (!editor)
        return;

    KKSList<const KKSFilter*> filters;

    KKSObject * o = loader->loadIO(idObject, true);
    if (!o)
        return;
    KKSCategory * c = o->category(); //->tableCategory();
    if (!c) {
        o->release();
        return;
    }

    KKSList<const KKSFilterGroup *> filterGroups;
    bool mode(editor->windowModality() == Qt::NonModal);

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
        emit objEditorCreated(objEditor);

    o->release();
}

void KKSRubricFactory::openRubricItem(int idObject, int idRecord) 
{
    
    KKSIncludesWidget *editor = qobject_cast<KKSIncludesWidget *>(this->sender());
    if (!editor)
        return;

    KKSList<const KKSFilter*> filters;

    KKSObject * o = loader->loadIO(idObject, true);
    if (!o)
        return;
    
    KKSCategory * c = o->category(); //->tableCategory();
    if (!c) {
        o->release();
        return;
    }

    KKSList<const KKSFilterGroup *> filterGroups;
    bool mode(editor->windowModality() == Qt::NonModal);

    KKSObjEditor *objEditor = oef->createObjEditor(idObject,
            idRecord,
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
        emit objEditorCreated(objEditor);

    o->release();
}

void KKSRubricFactory::loadRubricPrivilegies(RubricForm * rForm) {
    qDebug() << __PRETTY_FUNCTION__;
    if (!rForm)
        return;

    KKSAccessEntity * acl = new KKSAccessEntity();
    KKSStuffForm * sForm = stf->createStuffEditorForm(acl, 1, rForm);
    acl->release();
    rForm->setStuffForm(sForm);
    //QAbstractItemModel * sModel = stf->getStuffModel();
    //rForm->setStuffModel (sModel);
}

void KKSRubricFactory::loadSearchTemplate(RubricForm * rForm)
{
    qDebug() << __PRETTY_FUNCTION__;
    if (!rForm)
        return;

    KKSSearchTemplate * st = oef->loadSearchTemplate();
    rForm->setSearchTemplate(st);
    if (st)
        st->release();
    //    QAbstractItemModel
}

void KKSRubricFactory::loadCategory(RubricForm * rForm)
{
    if (!rForm)
        return;

    KKSObject *refObj = loader->loadIO(IO_CAT_ID, true);
    if (!refObj)
        return;
    KKSCategory *c = refObj->category();
    if (!c) {
        refObj->release();
        return;
    }

    KKSList<const KKSFilterGroup*> filters;
    KKSCategory * ct = c->tableCategory();
    if (!ct) {
        refObj->release();
        return;
    }

    KKSList<const KKSFilter*> fl;
    
    //QString value = QString("select id from io_categories where id_io_category_type != 10 and not is_system");
    const KKSFilter * f = NULL;
    
    //� ������� ������� ������������ �� ����� ��������� ������ ������ ������������, ������� ������� ���������, �� �������� ������ ��� ��������� "����������"
    if(rForm->forRecords()){
        f = ct->createFilter(ATTR_ID_IO_CAT_TYPE, "1", KKSFilter::foEq);
        if (!f) {
            refObj->release();
            return;
        }
        fl.append(f);
        f->release();
    }
    else{
        //� ������� �������������� ����������� � �� �� ����� ��������� ������ ��, ������� ������� ���������, �� ������ ��������� ��������� ��� ��������� "����������� ���������"
        f = ct->createFilter(ATTR_ID_IO_CAT_TYPE, "10", KKSFilter::foNotEq);
        if (!f) {
            refObj->release();
            return;
        }
        fl.append(f);
        f->release();

        //����� �������� ��������� ���������
        f = ct->createFilter(ATTR_ID, "300", KKSFilter::foGr);
        if (!f) {
            refObj->release();
            return;
        }
        fl.append(f);
        f->release();
    }
    
    KKSFilterGroup * fg = new KKSFilterGroup(true);
    fg->setFilters(fl);
    filters.append(fg);
    fg->release();
    KKSRecDialog * recEditor = oef->createObjRecEditor(IO_IO_ID,
            refObj->id(),
            filters,
            "",
            c,
            true, false,
            Qt::ApplicationModal,
            rForm,
            Qt::Dialog);
    if (!recEditor) {
        refObj->release();
        return;
    }

    if (recEditor->exec() == QDialog::Accepted) {
        int idCat = recEditor->getID();
        KKSCategory * cat = loader->loadCategory(idCat);
        if (!cat) {
            refObj->release();
            return;
        }
        rForm->setCategory(cat);
        cat->release();
    }
    refObj->release();
}

//�� � ������� ����� ������ ������ ��� ������ � ������� ������������
void KKSRubricFactory::loadIO(RubricForm * rForm)
{
    if (!rForm)
        return;

    KKSObject *refObj = loader->loadIO(IO_IO_ID, true);
    if (!refObj)
        return;

    KKSCategory *c = refObj->category();
    if (!c) {
        refObj->release();
        return;
    }

    KKSList<const KKSFilterGroup*> filters;
    KKSCategory * ct = c->tableCategory();
    if (!ct) {
        refObj->release();
        return;
    }

    KKSList<const KKSFilter*> fl;

    //����� ������ ���������������� ����������� (��� ������� ��� �����)
    QString value = QString("300");
    const KKSFilter * f = ct->createFilter(ATTR_ID, value, KKSFilter::foGr);
    if (!f) {
        refObj->release();
        return;
    }
    fl.append(f);
    f->release();
    
    f = ct->createFilter(ATTR_TABLE_NAME, "", KKSFilter::foIsNotNull);
    if (!f) {
        refObj->release();
        return;
    }
    fl.append(f);
    f->release();

    KKSFilterGroup * fg = new KKSFilterGroup(true);
    fg->setFilters(fl);
    filters.append(fg);
    fg->release();

    KKSRecDialog * recEditor = oef->createObjRecEditor(IO_IO_ID,
            refObj->id(),
            filters,
            "",
            c,
            true, false,
            Qt::ApplicationModal,
            rForm,
            Qt::Dialog);

    if (!recEditor) {
        refObj->release();
        return;
    }

    if (recEditor->exec() == QDialog::Accepted) {
        int idObject = recEditor->getID();
        KKSObject * io = loader->loadIO(idObject);
        if (!io) {
            refObj->release();
            return;
        }
        rForm->setIO(io);
        io->release();
    }
    refObj->release();
}

void KKSRubricFactory::viewAttachments(QAbstractItemModel * attachModel, const KKSRubric * r) {
    if (!attachModel || !r || !r->getCategory())
        return;

    KKSList<const KKSRubricItem *> docList = r->items();
    KKSCategory * c = r->getCategory();
    QAbstractItemModel * objModel = attachModel;
    while (qobject_cast<QAbstractProxyModel *>(objModel))
        objModel = qobject_cast<QAbstractProxyModel *>(objModel)->sourceModel();
    if (objModel->rowCount() > 0) {
        int nr = objModel->rowCount();
        objModel->removeRows(0, nr);
    }

    if (objModel->columnCount() > 0) {
        int nc = objModel->columnCount();
        objModel->removeColumns(0, nc);
    }

    //QString aNameCode ("name");
    //KKSCategoryAttr * cAName = c->attribute (aNameCode);
    const KKSCategory * ct = c && c->tableCategory() ? c->tableCategory() : c;
    KKSMap<int, KKSCategoryAttr *> cAttrs = ct->attributes();
    int nCols = cAttrs.count() + 1;

    QList<int> bkColors = ct ? ct->searchAttributesByType(KKSAttrType::atRecordColor) : QList<int>();
    if (bkColors.isEmpty() && ct)
        bkColors += ct->searchAttributesByType(KKSAttrType::atRecordColorRef);
    if (!bkColors.isEmpty())
        nCols--;
    QList<int> fgColors = ct ? ct->searchAttributesByType(KKSAttrType::atRecordTextColor) : QList<int>();
    if (fgColors.isEmpty() && ct)
        fgColors += ct->searchAttributesByType(KKSAttrType::atRecordTextColorRef);

    if (!fgColors.isEmpty())
        nCols--;

    objModel->insertColumns(0, nCols);
    int nRows = docList.count();
    objModel->insertRows(0, nRows);
    int i = 1;
    objModel->setHeaderData(0, Qt::Horizontal, tr("Name"), Qt::DisplayRole);

    for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = cAttrs.constBegin(); \
            pa != cAttrs.constEnd(); pa++) {
        objModel->setHeaderData(i, Qt::Horizontal, pa.value()->title(), Qt::DisplayRole);
        if (pa.value()->type()->attrType() == KKSAttrType::atRecordColor ||
                pa.value()->type()->attrType() == KKSAttrType::atRecordColorRef ||
                pa.value()->type()->attrType() == KKSAttrType::atRecordTextColor ||
                pa.value()->type()->attrType() == KKSAttrType::atRecordTextColorRef)
            continue;
        i++;
    }

    for (int i = 0; i < nRows; i++) {
        int j = 1;
        int id_io = docList[i]->id();
        KKSObject * cio = loader->loadIO(id_io);
        if (!cio)
            continue;
        QModelIndex nIndex = objModel->index(i, 0);
        objModel->setData(nIndex, cio->name(), Qt::DisplayRole);
        objModel->setData(nIndex, cio->id(), Qt::UserRole);
        QVariant bkColVal;

        if (!bkColors.isEmpty()) {
            int idAttr = bkColors[0]; //������ ����� ������ �������, ������� ��������� �� ����
            KKSAttrView * vcol = new KKSAttrView(*(c->attribute(idAttr)));
            //QRgb rgb_col (0xFFFFFFFF);
            QVariant vc;
            KKSAttrValue * av = cio->attrValueId(vcol->id());
            if (av && !av->value().value().isEmpty() && vcol->type()->attrType() == KKSAttrType::atRecordColor) {
                vc = av->value().valueVariant(); //QColor::fromRgba(rgb_col);
                qDebug() << __PRETTY_FUNCTION__ << vc << vc.value<QColor > () << av->value().valueVariant();
                bkColVal = QVariant(QBrush(vc.value<QColor > ()));
            } else if (av && vcol->type()->attrType() == KKSAttrType::atRecordColorRef) {
                int idVal = av->value().value().toInt();
                KKSObject * objCRef = loader->loadIO(vcol->tableName(), true);

                //������� ��������� �� �������� � ��������� �������, 
                //������ ��������� ���������������� �� ��������� �������. ������� ��� �������� ����� ������������ � �������� �����
                KKSCategory * cRef = objCRef->category()->tableCategory();
                if (!cRef) {
                    objCRef->release();
                    continue;
                }

                //cRef �������� ��������� ����������, ������� � ��� ��� �������� ����� ���������� ����
                //������������� ������� ������ ���������
                KKSAttribute * a = cRef->attribute(vcol->columnName());
                if (!a) {
                    objCRef->release();
                    continue;
                }

                KKSObjectExemplar * colorC = loader->loadEIO(idVal, objCRef);
                KKSAttrValue * avR = colorC->attrValue(a->id());

                //rgb_col = av->value().value().toUInt();
                vc = avR->value().valueVariant(); //avR->value ().valueVariant ();//(QColor::fromRgba(rgb_col));//d->fields().value (vcol->code().toLower()));
                colorC->release();
                objCRef->release();
                //qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
                bkColVal = QVariant(QBrush(vc.value<QColor > ()));
            }
            objModel->setData(nIndex, bkColVal, Qt::BackgroundRole); //Qt::UserRole+1);
            vcol->release();
        }

        QVariant fgColVal;
        if (!fgColors.isEmpty()) {
            int idAttr = fgColors[0];
            KKSAttrView * vcol = new KKSAttrView(*(c->attribute(idAttr)));
            //QRgb rgb_colf (0x00000000);
            QVariant vc;
            KKSAttrValue * av = cio->attrValueId(vcol->id());
            if (av && !av->value().value().isEmpty() && vcol->type()->attrType() == KKSAttrType::atRecordTextColor) {
                vc = av->value().valueVariant(); //(QColor::fromRgba(rgb_colf));
                //qDebug() << __PRETTY_FUNCTION__ << vc << vc.value<QColor > ();
                fgColVal = QVariant(QBrush(vc.value<QColor > ()));
            } else if (vcol->type()->attrType() == KKSAttrType::atRecordTextColorRef) {
                int idVal = av->value().value().toInt();
                KKSObject * objCRef = loader->loadIO(vcol->tableName(), true);

                //������� ��������� �� �������� � ��������� �������, 
                //������ ��������� ���������������� �� ��������� �������. ������� ��� �������� ����� ������������ � �������� �����
                KKSCategory * cRef = objCRef->category()->tableCategory();
                if (!cRef) {
                    objCRef->release();
                    continue;
                }

                //cRef �������� ��������� ����������, ������� � ��� ��� �������� ����� ���������� ����
                //������������� ������� ������ ���������
                KKSAttribute * a = cRef->attribute(vcol->columnName());
                if (!a) {
                    objCRef->release();
                    continue;
                }

                KKSObjectExemplar * colorC = loader->loadEIO(idVal, objCRef);
                KKSAttrValue * avR = colorC->attrValue(a->id());

                //rgb_colf = av->value().value().toUInt();
                vc = avR->value().valueVariant(); //(QColor::fromRgba(rgb_colf));//d->fields().value (vcol->code().toLower()));
                colorC->release();
                objCRef->release();
                //qDebug () << __PRETTY_FUNCTION__ << vc << vc.value<QColor>();
                fgColVal = QVariant(QBrush(vc.value<QColor > ()));
            }
            objModel->setData(nIndex, fgColVal, Qt::ForegroundRole); //Qt::UserRole+2);
            vcol->release();
        }

        for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = cAttrs.constBegin(); \
             pa != cAttrs.constEnd(); pa++) {
            QModelIndex wIndex = objModel->index(i, j);
            if (pa.value()->type()->attrType() == KKSAttrType::atRecordColor ||
                    pa.value()->type()->attrType() == KKSAttrType::atRecordColorRef ||
                    pa.value()->type()->attrType() == KKSAttrType::atRecordTextColor ||
                    pa.value()->type()->attrType() == KKSAttrType::atRecordTextColorRef)
                continue;
            const KKSAttrValue * av = cio->attrValueId(pa.key()); //value()->id());
            qDebug() << __PRETTY_FUNCTION__ << pa.key() << pa.value()->code() << av;
            if (av) {
                if (av->attribute()->type()->attrType() == KKSAttrType::atJPG ||
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atJPG)
                        ) {
                    objModel->setData(wIndex, tr("<Image data %1>").arg(i), Qt::DisplayRole);
                } else if (av->attribute()->type()->attrType() == KKSAttrType::atSVG ||
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atSVG)
                        ) {
                    objModel->setData(wIndex, tr("<SVG data %1>").arg(i), Qt::DisplayRole);
                } else if (av->attribute()->type()->attrType() == KKSAttrType::atXMLDoc ||
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atXMLDoc)
                        ) {
                    objModel->setData(wIndex, tr("<XML document %1>").arg(i), Qt::DisplayRole);
                } else if (av->attribute()->type()->attrType() == KKSAttrType::atVideo ||
                        (av->attribute()->refType() && av->attribute()->refType()->attrType() == KKSAttrType::atVideo)
                        ) {
                    objModel->setData(wIndex, tr("<Video data %1>").arg(i), Qt::DisplayRole);
                } else if (av->attribute()->type()->attrType() == KKSAttrType::atList ||
                           av->attribute()->type()->attrType() == KKSAttrType::atSysChildCategoryRef ||
                           av->attribute()->type()->attrType() == KKSAttrType::atParent) {
                    bool ok;
                    int idVal = av->value().value().toInt(&ok);
                    if (!ok)
                        objModel->setData(wIndex, QVariant(), Qt::DisplayRole);

                    KKSObject *refObj = loader->loadIO(av->attribute()->tableName(), true);
                    if (!refObj)
                        continue;

                    //������� ��������� �� �������� � ��������� �������, 
                    //������ ��������� ���������������� �� ��������� �������. ������� ��� �������� ����� ������������ � �������� �����
                    KKSCategory * cRef = refObj->category()->tableCategory();
                    if (!cRef) {
                        refObj->release();
                        continue;
                    }

                    //cRef �������� ��������� ����������, ������� � ��� ��� �������� ����� ���������� ����
                    //������������� ������� ������ ���������
                    KKSAttribute * a = cRef->attribute(av->attribute()->columnName());
                    if (!a) {
                        refObj->release();
                        continue;
                    }

                    KKSObjectExemplar * oe = loader->loadEIO(idVal, refObj);
                    if (!oe) {
                        refObj->release();
                        continue;
                    }

                    KKSAttrValue * avr = oe->attrValue(a->id());
                    if (!avr) {
                        refObj->release();
                        continue;
                    }

                    objModel->setData(wIndex, avr->value().valueVariant(), Qt::DisplayRole);
                    oe->release();
                    refObj->release();
                } 
                else if (av->attribute()->type()->attrType() == KKSAttrType::atDateTime)
                    objModel->setData(wIndex, QDateTime::fromString(av->value().value(), Qt::ISODate).toString("dd.MM.yyyy hh:mm:ss"), Qt::DisplayRole);
                else if (av->attribute()->type()->attrType() == KKSAttrType::atDateTimeEx)
                    objModel->setData(wIndex, QDateTime::fromString(av->value().value(), Qt::ISODate).toString("dd.MM.yyyy hh:mm:ss.zzz"), Qt::DisplayRole);
                else if (av->attribute()->type()->attrType() == KKSAttrType::atTime)
                    objModel->setData(wIndex, QDateTime::fromString(av->value().value(), Qt::ISODate).toString("hh:mm:ss"), Qt::DisplayRole);
                else if (av->attribute()->type()->attrType() == KKSAttrType::atText ||
                        av->attribute()->type()->attrType() == KKSAttrType::atString) {
                    //������� ����. ������� \n \r
                    QString text;
                    text = av->value().value();
                    int index1 = text.indexOf('\n');
                    int index2 = text.indexOf('\r');
                    int index = -1;
                    if (index1 > -1 || index2 > -1) {
                        if (index1 == -1)
                            index = index2;
                        if (index2 == -1)
                            index = index1;
                        if (index1 > -1 && index2 > -1)
                            index = (index1 > index2) ? index2 : index1;
                    }
                    if (index > -1) {
                        text = text.left(index);
                        text += " ...";
                    }
                    objModel->setData(wIndex, text, Qt::DisplayRole);
                } else
                    objModel->setData(wIndex, av->value().value(), Qt::DisplayRole);
            }
            //qDebug () << __PRETTY_FUNCTION__ << wIndex << bkColVal << fgColVal;
            if (bkColVal.isValid())
                objModel->setData(wIndex, bkColVal, Qt::BackgroundRole);
            if (fgColVal.isValid())
                objModel->setData(wIndex, fgColVal, Qt::ForegroundRole);
            j++;
        }
        cio->release();
    }
}

void KKSRubricFactory::copyFromRubric(KKSRubric * rDest, QAbstractItemModel * aModel, const QModelIndex& wIndex) {
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
    if (!cat) {
        refIO->release();
        return;
    }

    KKSRecDialog * oEditor = oef->createObjRecEditor(IO_IO_ID, IO_RUBR_ID, filters, tr("Select source rubric"), cat, true, Qt::WindowModal);
    if (oEditor && oEditor->exec() == QDialog::Accepted) {
        int idRubr = oEditor->getID();
        KKSRubric * rSource = loader->loadRubric(idRubr, true);
        if (rSource)
            rDest->addRubric(rSource);
    } else
        if (oEditor)
        delete oEditor;
    refIO->release();
}

void KKSRubricFactory::initRubricAttachments(const KKSRubric * r, bool isRec)
{
    if (!r)
        return;

    KKSIncludesWidget * iW = qobject_cast<KKSIncludesWidget *>(this->sender());
    
    const KKSTemplate * t = isRec ? rubrRecTemlate() : 0;

    KKSMap<qint64, KKSEIOData *> rData = isRec ? loader->loadRecList (r) : KKSConverter::rubricEntityToData(loader, r);

    const KKSCategory * cat(0);
    KKSObject * refIO (0);

    //if (r->getCategory() && r->getCategory()->attributes().count() > 0)
    //    cat = r->getCategory();
    //else
    if (!isRec)
    {
        refIO = loader->loadIO(IO_IO_ID, true);
        cat = refIO->category()->tableCategory();
        t = new KKSTemplate(cat->defTemplate());
    }

    QAbstractItemModel * attachModel = new KKSEIODataModel(t, rData);

    if (!isRec)
    {
        int nr = attachModel->rowCount();
        //qDebug () << __PRETTY_FUNCTION__ << attachModel->rowCount() << attachModel->columnCount();
        for (int i = 0; i < nr; i++)
        {
            QModelIndex iconInd = attachModel->index(i, 0);
            //
            // ��������� ���������� ���������������, �� ���������� �������� ������ �� �������� �������� ������
            //
            int id = iconInd.data(Qt::UserRole).toInt();

            const KKSRubricItem * rItem = r->itemForId(id);
            QPixmap rIconP;
            if (rItem)
                rIconP.loadFromData(rItem->iconAsString().toUtf8()); //rData.value(id)->fields().value("r_icon").toUtf8());
            QIcon ioIcon(rIconP); //io->icon();
            if (!ioIcon.isNull()) {
                //qDebug () << __PRETTY_FUNCTION__ << QString::compare (rItem->iconAsString(), io->iconAsString());
                attachModel->setData(iconInd, ioIcon, Qt::DecorationRole);
            }
            else if (r->rubricType() == KKSRubricBase::btRubricAsCategory)
            {
                if (ioIcon.isNull())
                {
                    if (rItem)
                        attachModel->setData(iconInd, (rItem->getIcon().isNull() ? rItem->getDefaultIcon() : rItem->getIcon()), Qt::DecorationRole);
                    else
                        attachModel->setData(iconInd, KKSRubricItem::icon(), Qt::DecorationRole);
                }
                else
                    attachModel->setData(iconInd, ioIcon, Qt::DecorationRole);
            }
            else
            {
                if (rItem)
                    attachModel->setData(iconInd, (rItem->getIcon().isNull() ? rItem->getDefaultIcon() : rItem->getIcon()), Qt::DecorationRole); // rItem->getDefaultIcon()
                else
                    attachModel->setData(iconInd, KKSRubricItem::icon(), Qt::DecorationRole);
            }
            //io->release ();
        }

        refIO->release();
    }

    t->release();

    emit rubricAttachments(attachModel, iW);
}

/* ���� ��������� ������������ �� � ������� � ����������� ������������ � ���������� ������
 */
void KKSRubricFactory::appendRubricItem(QAbstractItemModel * attachModel, const KKSRubricItem * rItem)
{
    if (!attachModel || !rItem)
        return;

    KKSObject * refIO = loader->loadIO(IO_IO_ID, true);
    if (!refIO)
        return;

    KKSCategory * c = refIO->category();
    if (!c || !c->tableCategory()) {
        refIO->release();
        return;
    }

    //������� ������
    c = c->tableCategory();
    const KKSFilter * f = c->createFilter(ATTR_ID, QString::number(rItem->id()), KKSFilter::foEq);
    KKSFilterGroup * fg = new KKSFilterGroup(false);
    fg->addFilter(f);
    KKSList<const KKSFilterGroup *> filters;
    filters.append(fg);
    fg->release();
    f->release();
    
    //������� ������ � ���� KKSEIOData
    KKSMap<qint64, KKSEIOData *> itemData = loader->loadEIOList(refIO, filters);
    
    int cnt = attachModel->rowCount();
    if (!attachModel->insertRows(cnt, itemData.count()))
    {
        refIO->release();
        return;
    }

    int i = 0;
    for (KKSMap<qint64, KKSEIOData *>::const_iterator p = itemData.constBegin();
            p != itemData.constEnd();
            p++) 
    {
        QModelIndex wIndex = attachModel->index(cnt + i, 0);

        //KKSObject * io = loader->loadIO(p.key(), true);
        
        attachModel->setData(wIndex, QVariant::fromValue<KKSEIOData *>(p.value()), Qt::UserRole + 1);
        
        attachModel->setData(wIndex, p.key(), Qt::UserRole);
        
        QString icon;
        icon = p.value()->sysFieldValue("r_icon");

        if (!icon.isNull()){
            QPixmap px;
            bool isOk = px.loadFromData(icon.toUtf8(), "XPM");
            if(!isOk)
                attachModel->setData(wIndex, KKSRubricItem::icon(), Qt::DecorationRole);
            else
                attachModel->setData(wIndex, px, Qt::DecorationRole);
        }
        else
            attachModel->setData(wIndex, KKSRubricItem::icon(), Qt::DecorationRole);
        
        //io->release();
        
        i++;
    }

    refIO->release();
}

//���������� ��� �������� � �� ������ ��, ��������� � �������
//������������ ������ ��� �������� ������ ��
void KKSRubricFactory::rubricItemCreated(KKSObjectExemplar * rec)
{
    KKSRubric * r = ioRubrs.value(rec);
    QAbstractItemModel * attachModel = ioModels.value(rec);
    QModelIndex pInd = ioParents.value(rec);
    KKSIncludesWidget * iW = includesW.value(rec);

    if (!r || !attachModel)
        return;

    KKSObject * wObj = loader->loadIO(rec->id(), true);
    if (iW && wObj)
        iW->slotAddRubricItem(rec->id(), wObj->name());

    if (wObj)
        wObj->release();
}

void KKSRubricFactory::objEditorClosed()
{
    KKSObjEditor * objEditor = qobject_cast<KKSObjEditor *>(this->sender());
    if (!objEditor)
        return;

    KKSObjectExemplar * rec = objEditor->getObjectEx();
    if (!rec)
        return;

    int rubrres = ioRubrs.remove(rec);
    int modres = ioModels.remove(rec);
    int pres = ioParents.remove(rec);
    int wres = includesW.remove(rec);
    Q_UNUSED (rubrres);
    Q_UNUSED (modres);
    Q_UNUSED (pres);
    Q_UNUSED (wres);
    //qDebug() << __PRETTY_FUNCTION__ << rubrres << modres << pres << wres;
}

void KKSRubricFactory::setSyncSettings(const QList<int>& ioIDList) {
    if (ioIDList.isEmpty())
        return;

    KKSSyncDialog * syncD = new KKSSyncDialog();
    KKSSyncWidget * syncW = syncD->getSyncWidget();
    //int idUser = loader->getUserId();
    KKSObject * refObj = loader->loadIO(IO_IO_ID, true);
    KKSObjectExemplar * wObjE = loader->loadEIO(ioIDList.at(0), refObj);
    int attrId = ATTR_ID_SYNC_TYPE;
    KKSAttribute * attr = loader->loadAttribute(attrId);
    KKSAttrValue * av = wObjE->attrValue(attrId);
    if (av) {
        QMap<int, QString> refValues;
        QMap<int, QString> values = loader->loadAttributeValues(attr, refValues, false, true, attr->tableName());
        QMap<int, QString>::const_iterator pv = values.constFind(av->value().value().toInt());
        QString v_str = QString();
        if (pv != values.constEnd())
            v_str = pv.value();
        syncW->setSyncType(v_str);
    }
    attr->release();
    qint64 idObjE = wObjE->id();
    KKSCategory * c = wObjE->io()->category();
    if (!c)
        return;
    KKSCategory * ct = c->tableCategory();
    if (!ct)
        return;
    QMap<int, QStringList> syncOrgs = loader->getSyncOrg(idObjE);
    KKSCategoryAttr * syncAttr = ct->attribute(ATTR_IO_OBJECTS_ORGANIZATION);
    KKSCategoryAttr * syncAttrType = ct->attribute(ATTR_ID_SYNC_TYPE);
    KKSAttrValue * syncAttrVal(0); //= new KKSAttrValue (sVal, syncAttr);
    KKSValue val(QString("{}"), KKSAttrType::atCheckListEx);
    syncAttrVal = new KKSAttrValue(val, syncAttr);
    KKSAttrValue * syncAttrTypeV(0);
    KKSValue valt(QString(""), KKSAttrType::atList);
    syncAttrTypeV = new KKSAttrValue(valt, syncAttrType);
    syncW->setSyncAttrVal(syncAttrVal);
    syncW->setSyncAttrType(syncAttrVal);
    if (syncAttrVal)
        syncAttrVal->release();
    connect(syncD, SIGNAL(loadSyncRef(QString, QWidget *, qint64)),
            oef,
            SLOT(loadSyncType(QString, QWidget *, qint64))
            );
    connect(syncD, SIGNAL(loadAddSyncOrg(KKSAttrValue *, QAbstractItemModel *)),
            oef,
            SLOT(loadSyncAddAttrRef(KKSAttrValue *, QAbstractItemModel *))
            );
    connect(syncD, SIGNAL(loadDelSyncOrg(KKSAttrValue *, const QModelIndex&, QAbstractItemModel *)),
            oef,
            SLOT(loadSyncDelAttrRef(KKSAttrValue *, const QModelIndex&, QAbstractItemModel *))
            );
    if (syncD->exec() == QDialog::Accepted) {
        KKSAttrValue * syncVal = syncW->getSyncAttrVal();
        KKSAttrValue * syncType = syncW->getSyncAttrType();
        KKSObject * ioRef = loader->loadIO(IO_IO_ID, true);
        qDebug() << __PRETTY_FUNCTION__ << syncVal->value().value();
        for (int i = 0; i < ioIDList.count(); i++) {
            KKSObjectExemplar * rio = loader->loadEIO(ioIDList[i], ioRef);
            if (!rio)
                continue;
            KKSAttrValue * av = rio->attrValue(syncVal->attribute()->id());
            if (!av)
                rio->addAttrValue(syncVal);
            else {
                rio->removeAttrValue(av);
                rio->addAttrValue(syncVal);
            }
            KKSAttrValue * avT = rio->attrValue(syncType->attribute()->id());
            if (!avT)
                rio->addAttrValue(syncType);
            else {
                rio->removeAttrValue(avT);
                rio->addAttrValue(syncType);
            }
            KKSObject * wObj = loader->loadIO(ioIDList[i]);
            bool ok = KKSConverter::objectFromExemplar(loader, wObj, rio);
            wObj->setIsGlobal(syncW->isCheckGlobal());
            qDebug() << __PRETTY_FUNCTION__ << wObj->isGlobal();
            int ier = ppf->updateIO(wObj); //eiof->updateEIO (rio);
            wObj->release();
            qDebug() << __PRETTY_FUNCTION__ << ok << ier;
            rio->release();
        }
        ioRef->release();
    }
}

void KKSRubricFactory::putIntoRubr(const QList<int>& ioIDList, const KKSRubric * oldRubric) 
{
    if (ioIDList.isEmpty())
        return;
    
    KKSIncludesWidget * anotherW = this->createRubricEditor(atRootRubric, smRubricsOnly, true);

    KKSIncludesDialog * rubrDial = new KKSIncludesDialog(anotherW);
    anotherW->hideIncludes();

    QWidget * pWidget = qobject_cast<QWidget *>(this->sender());

    if (rubrDial->exec() == QDialog::Accepted) {
        const KKSRubric * r = anotherW->getSelectedRubric();
        if (!r) {
            qWarning() << tr("Select destignation");
            QMessageBox::warning(pWidget, tr("Copy documents"), tr("Select destignation"), QMessageBox::Ok);
            return;
        }
        if (oldRubric && (oldRubric == r || oldRubric->id() == r->id())) {
            qWarning() << tr("You put documents into same rubric");
            QMessageBox::warning(pWidget, tr("Copy documents"), tr("You put documents into same rubric"), QMessageBox::Ok);
            return;
        }
        for (int i = 0; i < ioIDList.count(); i++) {
            KKSObject * wObj = loader->loadIO(ioIDList[i], true);
            if (!wObj)
                continue;
            KKSRubricItem * rItem = new KKSRubricItem(ioIDList[i], wObj->name());
            (const_cast<KKSRubric *> (r))->addItem(rItem);
            //appendRubricItem (attachModel, rItem);
            rItem->release();
            wObj->release();
        }
        qDebug() << __PRETTY_FUNCTION__ << r->items().count();
        ppf->updateRubrics(const_cast<KKSRubric *> (r));
        //saveRubric (iW->rootRubric(), false);
        //initRubricAttachments (r);

    }
    delete rubrDial;
}

void KKSRubricFactory::sendDocs(const QList<int>& ioIDList) {
    if (ioIDList.isEmpty())
        return;
    oef->sendIOList(ioIDList);
}

void KKSRubricFactory::setAccessDocs(const QList<int>& ioIDList) {
    if (ioIDList.isEmpty())
        return;
    QWidget * parentW = qobject_cast<QWidget *>(sender());
    KKSAccessEntity * acl = new KKSAccessEntity();
    int idUser = loader->getUserId();
    KKSStuffForm * sForm = stf->createStuffEditorForm(acl, idUser, parentW, Qt::Dialog);
    KKSStuffDialog * sDial = new KKSStuffDialog(sForm);
    if (sDial->exec() == QDialog::Accepted) {
        KKSAccessEntity * acw = sForm->getAccessEntity();
        for (int i = 0; i < ioIDList.count(); i++) {
            KKSObject * wObj = loader->loadIO(ioIDList[i]);
            if (!wObj)
                continue;
            wObj->setAccessRules(acw);
            ppf->updateIO(wObj);
            wObj->release();
        }
    }
    sForm->setParent(0);
    delete sForm;
}

KKSRubric * KKSRubricFactory::loadRubric(KKSRubric * r, int idRubr, QAbstractItemModel * rubrMod, const QModelIndex& recIndex)
{
    //����� ������� ����������� ��� �������� ��������� � ��������. ������ �������� � ���
    KKSRubric * rw = NULL;
    if(r->rubricType() == KKSRubricBase::btRubric)
        rw = loader->loadRubric(idRubr);
    else
        rw = loader->loadRecRubric(idRubr);

    if (!rw)
        return r;



    KKSCategory * c = rw->getCategory();
    //    if (c)
    //        c->addRef();
    r->setCategory(c);
    //    c->release();

    KKSSearchTemplate * st = rw->getSearchTemplate();
    r->setSearchTemplate(st);

    KKSAccessEntity * acr = rw->getAccessRules();
    r->setAccessRules(acr);
    
    rubrMod->setData(recIndex, QVariant::fromValue<const KKSRubricBase *>(r), Qt::UserRole + 1);
    if (!rw->getIcon().isNull())
        rubrMod->setData (recIndex, rw->getIcon(), Qt::DecorationRole);

    rw->release();

    r->setInitialized(true);//��������� � �� � ���������� �� ����� ������ �������, ��� ������� ������ ������ �����
                            //���������� �� ��������, ��� �� ������� �������� ������ � ���������� � ��� ��������� ������������ �� �� � ������.
                            
    return r;
}

void KKSRubricFactory::loadCategoryIntoRubric(KKSRubric * r) {
    if (!r)
        return;
    int idCategory = r->id();
    KKSCategory * c = loader->loadCategory(idCategory, true);
    r->setCategory(c);
}

KKSTemplate * KKSRubricFactory::rubrRecTemlate (void)
{
    KKSCategory * ctRec = new KKSCategory;
    KKSTemplate * tRec = new KKSTemplate;// (ctRec->defTemplate());
    KKSAttrGroup * ag = new KKSAttrGroup;
    tRec->addGroup(ag);

    QStringList aNames;
    aNames << tr("IdObject")
           << tr("Qualifier Name")
           << tr("IdRecord")
           << tr("Record Name")
           << tr("State")
           << tr("UUID");
    
    QStringList aCodes;
    aCodes << QString("id_object")
           << QString("io_name")
           << QString("id")
           << QString("rr_name")
           << QString("state_name")
           << QString("uuid_t");
    
    QList<int> aTypes;
    aTypes << KKSAttrType::atInt
           << KKSAttrType::atString
           << KKSAttrType::atInt64
           << KKSAttrType::atString
           << KKSAttrType::atString
           << KKSAttrType::atString;

    QList<int> aWidths;
    aWidths << 70
            << 350
            << 50
            << 350
            << 150
            << 250;

    int na = aNames.count();
    for (int i=0; i<na; i++)
    {
        KKSAttribute * attr = new KKSAttribute;
        attr->setName(aNames[i]);
        attr->setTitle(aNames[i]);
        attr->setCode(aCodes[i]);
        attr->setDefWidth(aWidths[i]);
        KKSAttrType * aType = new KKSAttrType (aTypes[i]);
        attr->setType (aType);
        aType->release();
        KKSCategoryAttr * cAttrId = KKSCategoryAttr::create (attr, true, true);
        attr->release();
        ctRec->addAttribute(i+1, cAttrId);
        KKSAttrView *av = new KKSAttrView (*cAttrId);
        av->setVisible (true);
        cAttrId->release();
        av->setOrder (i);
        ag->addAttrView(i+1, av);
        av->release();
    }
    ag->release();
    tRec->setCategory (ctRec);
    ctRec->release();
    return tRec;
}

KKSMap<qint64, KKSEIOData *> KKSRubricFactory::getRubricatorsData() const
{
    KKSObject * rubrIO = loader->loadIO(IO_RUBR_ID, true);
    if (!rubrIO)
        return KKSMap<qint64, KKSEIOData *>();

    KKSCategory * cr = rubrIO->category();
    if (!cr)
    {
        rubrIO->release();
        return KKSMap<qint64, KKSEIOData *>();
    }

    cr = cr->tableCategory();
    if (!cr)
    {
        rubrIO->release();
        return KKSMap<qint64, KKSEIOData *>();
    }

    //������� ������ �� ��������� ������ ������ �� �����������
    KKSList<const KKSFilterGroup *> rfilters;
    const KKSFilter * f = cr->createFilter(1, QString("select id from getRubricatorsOnly(%1, false)").arg (loader->getUserId()), KKSFilter::foInSQL);
    KKSFilterGroup * fg = new KKSFilterGroup (true);
    fg->addFilter (f);
    f->release ();
    rfilters.append (fg);
    fg->release ();

    //�������� ���� ������ � ������ ������
    KKSMap<qint64, KKSEIOData *> rubrInfo = loader->loadEIOList(rubrIO, rfilters);

    return rubrInfo;
}

