/***********************************************************************
 * Module:  KKSObjEditorFactory.cpp
 * Author:  sergey
 * Modified: 27 ноября 2008 г. 14:40:51
 * Purpose: Implementation of the class KKSObjEditorFactory
 * Comment: данный класс является фабрикой редакторов ИО
 ***********************************************************************/

#include <QObject>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QAbstractProxyModel>
#include <QStringList>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMessageBox>
#include <QScrollArea>
#include <QTabWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QItemDelegate>
#include <QModelIndex>
#include <QInputDialog>
#include <QTreeView>
#include <QIODevice>
#include <QTextStream>
#include <QTextCodec>
#include <QProgressDialog>
#include <QProgressBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QToolButton>
#include <QLabel>
#include <QFile>
#include <QPalette>
#include <QBrush>
#include <QColor>
#include <QKeySequence>
#include <QHeaderView>
#include <QInputDialog>
#include <QSqlError>
#include <QSvgRenderer>
#include <QPicture>
#include <QSpacerItem>
#include <QDateTimeEdit>
#include <QVector>
#include <QtDebug>

#include "KKSObjEditorFactory.h"
#include <KKSObjEditor.h>
#include <KKSRecDialog.h>
#include "KKSObject.h"
#include <KKSIndicator.h>
#include "KKSObjectExemplar.h"
#include "KKSMap.h"
#include "KKSLoader.h"
#include "KKSFileLoader.h"
#include "KKSConverter.h"
#include "KKSEIOData.h"
#include "KKSPPFactory.h"
#include "KKSTemplate.h"
#include "KKSRecWidget.h"
#include "KKSAttrView.h"
#include "KKSAttrGroup.h"
#include "KKSAttrType.h"
#include "KKSCategory.h"
#include "KKSAttributesFactory.h"
#include "KKSViewFactory.h"
#include "KKSEIOFactory.h"
#include "KKSStuffFactory.h"
#include "KKSRubricFactory.h"
#include "KKSIndFactory.h"
#include <KKSAttribute.h>
#include <KKSType.h>
#include <KKSItemDelegate.h>
#include <KKSMessageWidget.h>
#include <KKSEventFilter.h>
#include <KKSSortFilterProxyModel.h>
#include <KKSSearchTemplateFilterProxyModel.h>
#include <KKSAttrRefWidget.h>
#include <KKSPrivilege.h>
#include <KKSEdit.h>
#include <KKSPixmap.h>
#include <KKSColorWidget.h>
#include <rubricform.h>
#include "KKSPrerender.h"
#include "kksdatabase.h"
#include "defines.h"
#include "choosetemplateform.h"
#include "commandform.h"
#include "messageform.h"
#include "periodicreportform.h"
#include "kksfilterseditorform.h"
#include "kkssearchtemplateform.h"
#include "kksfilewidget.h"
#include "kksincludeswidget.h"
#include "kksxmlform.h"
#include "kksstatisticview.h"
#include "KKSStatElement.h"
#include "kksstuffform.h"
#include "KKSRubric.h"
#include "KKSSearchTemplate.h"
#include "KKSSearchTemplateType.h"
#include "KKSSearchTemplatesForm.h"
#include "KKSAddTable.h"
#include "KKSAttrType.h"
#include "KKSSyncWidget.h"
#include "KKSSyncDialog.h"
#include <KKSAccessEntity.h>
#include <KKSState.h>
#include <KKSEIODataModel.h>
#include <savesearchtemplateform.h>
#include <kkslifecycleform.h>
#include <KKSLifeCycle.h>
#include <kksstateform.h>
#include "defines.h"

/*
 Заголовочные ф-лы генератора отчетов
 Иванов В. И. 8.6.2010
*/

#include "kkssito.h"
#include "kksdatabase.h"

#include <openreports.h>
#include <xsqlquery.h>

#include <renderobjects.h>
#include <orprerender.h>
#include <orprintrender.h>
#include <previewdialog.h>
#include <parameter.h>

#include <QPrintDialog>
#include <QFile>

#include <QDomDocument>
#include <QSqlDatabase>
//!!!!!!


////////////////////////////////////////////////////////////////////////
// Name:       KKSObjEditorFactory::KKSObjEditorFactory()
// Purpose:    Implementation of KKSObjEditorFactory::KKSObjEditorFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSObjEditorFactory::KKSObjEditorFactory(KKSPPFactory * _ppf, 
                                         KKSLoader *l, 
                                         KKSFileLoader * fl,
                                         KKSAttributesFactory * awf, 
                                         KKSEIOFactory * _eiof) :
    KKSEntityFactory (0),
    ppf (_ppf),
    m_awf (awf),
    loader (l),
    fileLoader(fl),
    eiof (_eiof),
    m_sf (0),
    m_rf (0),
    m_indf (0)
{
    //Q_INIT_RESOURCE (kksfactory_icon_set);
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSObjEditorFactory::~KKSObjEditorFactory()
// Purpose:    Implementation of KKSObjEditorFactory::~KKSObjEditorFactory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSObjEditorFactory::~KKSObjEditorFactory()
{
}

//с точки зрения подсистемы разграничения доступа 
//такая реализация данного метода допустима,
//поскольку мы уже смогли создать экземпляр KKSObjEditorFactory
//а раз так, то в случае, если данный ЕИО является ИО, то право на чтение имеется
bool KKSObjEditorFactory :: isIO (int idObjEx) const
{
    KKSObject *io = loader->loadIO (idObjEx, true);
    if (!io)
        return false;
    io->release ();
    return true;
}

/*
метод отображает указанный экземпляр заданного ИО,
 при этом
 * idObject -- идентификатор редактируемого/создаваемого ИО
 * idObjE -- идентификатор экземпляра (копии) редактируемого/создаваемого ЭИО
 * filters -- применяемые фильтры, используются для отображения содержимого ИО-справочника
 * extraTitle -- дополнительная надпись, которая применяется в качестве заголовка окна редактора ЭИО (ИО)
 * wCat -- категория, к которой относится редактируемый/создаваемый ИО
 * mode -- флаг модальности, отвечающий за наличие/отсутствие кнопок OK, Cancel, Apply
 * tableName -- название таблицы справочника отображаемого ИО-справочника (если является стандартной, то игнорируется)
 * bShowExecButton -- наличие/отсутствие кнопки "Отправить адресату" в случае создания ответа на распоряжение,
 * в остальных случаях данная кнопка должна отсутствовать
 * windowModality -- модальность окна
 * parent -- виджет-родитель
 * f -- флаги создания окна
 Результат
 * виджет редактирования ИО/ЭИО
*/
KKSObjEditor* KKSObjEditorFactory :: createObjEditor (int idObject, //идентификатор ИО, которому принадлежит редактируемый (создаваемый ЭИО)
                                                      qint64 idObjE, //идентификатор редактируемого (создаваемого) ЭИО
                                                      const KKSList<const KKSFilterGroup *> & filters,//фильтры, которые применяются к подчиненной таблице редактиремого ЭИО, если последний является ИО (контейнерным)
                                                      const QString & extraTitle,//дополнительная надпись, которая применяется в качестве заголовка окна редактора ЭИО (ИО)
                                                      const KKSCategory* wCat,//категория, которой принадлежит редактируемый (создаваемый) ЭИО (ИО)
                                                      bool mode,//флаг, который отвечает за появление кнопок (OK, Cancel, Apply)
                                                      const QString& tableName,
                                                      bool bShowExecButton,//определяет наличие кнопки "Отправить адресату" при исполнении распоряжения. Во всех остальных случаях ее быть не должно
                                                      Qt::WindowModality windowModality, //флаг модальности
                                                      QWidget *parent, //родительское окно
                                                      Qt::WindowFlags f)//оконные флаги (Qt)
{
    if(idObject <= 0){
        qWarning() << tr("Current qualifier does not saved in database!\nPlease save it before creating new records in it.");
        QMessageBox::warning(parent, tr("Record creation does not allowed"), tr("Current qualifier does not saved in database!\nPlease save it before creating new records in it."));
        return NULL;
    }
    
    KKSObject *obj = 0;
    KKSObjectExemplar * wObjE = 0;
    const KKSTemplate * tSystem = 0;
    const KKSTemplate * tRecAttr (0);

    this->loadEntities (obj, wObjE, wCat, idObject, idObjE, tableName, tSystem, false, parent, false);
    this->loadRecEntities (obj, wObjE, wCat, idObject, idObjE, tableName, tRecAttr, false, parent);

    const KKSTemplate * ioTemplate = NULL;
    KKSObject * io = NULL;
    //
    // если ЭИО является ИО, то у него есть еще дополнительные параметры из таблицы attrs_values в БД
    // кроме того, необходимо открыть редактор в выбранном пользователем шаблоне, если шаблонов несколько
    //
    this->loadEIOasIO (ioTemplate, io, idObject, idObjE, wObjE, wCat);

    KKSTemplate * tRec = this->getRecordTemplate(io, wCat);

    QToolBar * tbOper = new QToolBar ();//objEditorWidget);
    KKSObjEditor * objEditorWidget = new KKSObjEditor (tSystem, 
                                                       ioTemplate, 
                                                       tRec, 
                                                       wCat, 
                                                       filters, 
                                                       extraTitle, 
                                                       mode, 
                                                       wObjE, 
                                                       io, 
                                                       (idObject == IO_IO_ID ? true:false), 
                                                       tbOper, 
                                                       parent, 
                                                       f);

    objEditorWidget->setShowExecButton(bShowExecButton);

    if (mode)
        objEditorWidget->setWindowModality (windowModality);

    QGridLayout *mainLayout = objEditorWidget->getLayout ();
    mainLayout->addWidget (tbOper, 0, 0, 1, 1);
    initActions (tbOper, objEditorWidget);

    int nCount = 0;
    int nWR = 1;

    QTabWidget * tabObj = new QTabWidget (objEditorWidget);
    objEditorWidget->setTabWidget (tabObj);
    if (idObject == IO_IO_ID)// && idObjE > 0 )
        this->setIONameSecret (objEditorWidget, wObjE, io, nWR, mainLayout);

    mainLayout->addWidget (tabObj, nWR, 0, 1, 1);
    QGridLayout *gTabLay = new QGridLayout ();
    tabObj->setLayout (gTabLay);
    QWidget * propertiesWidget = 0;

    QWidget *sysAttrTabWidget = new QWidget ();
    QWidget *sysRecAttrWidget = new QWidget ();

    QWidget *ioAttrWidget = 0;

    QGridLayout *gSysLay = new QGridLayout ();
    sysAttrTabWidget->setLayout (gSysLay);
    QGridLayout *gRecAttrLay = new QGridLayout ();
    sysRecAttrWidget->setLayout (gRecAttrLay);

    if (tRecAttr && !tRecAttr->attributes().isEmpty())
    {
        QScrollArea *scIndAttrs = new QScrollArea (sysRecAttrWidget);
        scIndAttrs->setWidgetResizable (true);
        QWidget *indWidget = new QWidget ();
        scIndAttrs->setWidget (indWidget);
        QGridLayout * gIndLay = new QGridLayout (indWidget);
        gRecAttrLay->addWidget (scIndAttrs, 0, 0, 1, 1);
        indWidget->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
        tabObj->addTab (sysRecAttrWidget, tr ("Record Attributes"));
        //int nInds = 
        setIndicators (tRecAttr, obj, indWidget, gIndLay, wCat, wObjE, tableName, objEditorWidget);
    }
//    qDebug () << __PRETTY_FUNCTION__ << nCount;
//    nCount = setAttrsToEditor (t, obj, scSysAttrs, sysAttrWidget, mainLayout, wObjE, objEditorWidget, true, nCount);
    
    QGridLayout *gIOLay = 0;
    if (idObject == IO_IO_ID)// && idObjE > 0 )
    {
        propertiesWidget = new QWidget ();
        tabObj->addTab (propertiesWidget, tr ("Properties"));
        QGridLayout * gPropLay = new QGridLayout (propertiesWidget);
        QTabWidget * tabProp = new QTabWidget (propertiesWidget);
        gPropLay->addWidget (tabProp, 0, 0, 1, 1);
        tabProp->addTab (sysAttrTabWidget, tr ("System attributes"));

        //
        //Статистика
        //
        //
        // Test of layouts
        //
        QWidget * statW = new QWidget ();
        QGridLayout *gStatLay = new QGridLayout (statW);
        //statW->setLayout (gStatLay);
        KKSList<KKSStatElement*> sl = loader->loadIOStatistic(io->id());
        KKSStatisticView * statView = new KKSStatisticView(sl);
        tabProp->addTab (statW, tr("History of changing"));
        gStatLay->addWidget (statView, 0, 0, 1, 1);

        //
        // Права доступа
        //
        QWidget * stuffW = 0;//new QWidget ();
//        QGridLayout * gStuffLay = new QGridLayout ();
//        stuffW->setLayout (gStuffLay);
        KKSAccessEntity * acl = io->getAccessRules();
        if (!acl)
        {
            acl = new KKSAccessEntity ();
            io->setAccessRules (acl);
        }
        //
        // Test layouts
        //
        KKSStuffForm *sForm = m_sf->createStuffEditorForm (acl, io->author(), stuffW);
        tabProp->addTab (sForm/*stuffW*/, tr ("User privilegies"));
        connect (sForm, SIGNAL (accessRulesChanged (KKSAccessEntity *)),
                 objEditorWidget,
                 SLOT (setAccessIO(KKSAccessEntity *))
                );

        if (ioTemplate && ioTemplate->attrsCount() > 0)
        {
            ioAttrWidget = new QWidget();
            gIOLay = new QGridLayout (ioAttrWidget);
            tabObj->insertTab (0, ioAttrWidget, tr ("Attributes"));
        }

        //
        // Рубрикатор
        //
        //
        // Test of layouts
        //
        this->putRubricator (io, objEditorWidget, tabObj);
        //
        // Синхронизация
        //
        if (idObjE != IO_ORG_ID)
            this->putSyncWidget (objEditorWidget, wObjE, idObjE, tabObj);
    }
    else
    {
        tabObj->addTab (sysAttrTabWidget, tr ("Attributes"));
    }

    if (wObjE->io()->id() >= 300)
        this->putRubricator(wObjE, objEditorWidget, tabObj);
    QScrollArea *scSysAttrs = new QScrollArea (sysAttrTabWidget);
    scSysAttrs->setWidgetResizable (true);
    QWidget *attrWidget = new QWidget ();
    QGridLayout * gAttrLay = new QGridLayout (attrWidget);
    attrWidget->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);

    scSysAttrs->setWidget (attrWidget);

    if (tabObj)
        gSysLay->addWidget (scSysAttrs, 0, 0, 1, 1);
    else
        mainLayout->addWidget (scSysAttrs, 1, 0, 1, 1);

    // objEditorWidget->clearAttributes ();
    QTabWidget * tabEnc = 0;
    bool isAttrCheckEx = false;
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = wCat->attributes().constBegin(); \
            pa != wCat->attributes().constEnd() && !isAttrCheckEx; \
            pa++)
         isAttrCheckEx = isAttrCheckEx || (pa.value()->type ()->attrType() == KKSAttrType::atCheckListEx);
    
    if (wCat && wCat->tableCategory () && !isAttrCheckEx)
    {
        const KKSCategory * tCat = wCat->tableCategory ();
        for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = tCat->attributes().constBegin(); \
                pa != tCat->attributes().constEnd() && !isAttrCheckEx; \
                pa++)
             isAttrCheckEx = isAttrCheckEx || (pa.value()->type ()->attrType() == KKSAttrType::atCheckListEx);
    }

    QGridLayout * gAttachLay = 0;
    QWidget * attachW = 0;
    if (io || isAttrCheckEx)
    {
        attachW = new QWidget ();
        tabObj->addTab (attachW, tr ("Tables"));
        tabObj->setCurrentIndex (tabObj->count ()-1);
        gAttachLay = new QGridLayout (attachW);
        tabEnc = new QTabWidget (attachW);

        objEditorWidget->setRecTab (tabEnc);
    }
    nCount = setAttributes (tSystem, obj, attrWidget, gAttrLay, wCat, wObjE, tableName, objEditorWidget);

    objEditorWidget->setSysAttrWidgets (qobject_cast<QWidget *>(scSysAttrs->parent()), scSysAttrs, attrWidget);
    
    if (ioAttrWidget)
        this->initIOAttrs(io, wObjE, wCat, objEditorWidget, ioAttrWidget, gIOLay);

    QVBoxLayout *buttonsLayout = new QVBoxLayout ();

    /*если то, что мы отображаем в форме является информационным объектом, т.е. idObject = IO_IO_ID
    то нам надо пристыковать подчиненную таблицу, если она имеется.
    Если то, что мы отображаем не является информационным объектом, а является чисто экземпляром ИО,
    то подчиненной таблицы быть не может
    Кроме того, если идет процесс создания нового ИО (ЭИО), а не редактирования, 
    то подчиненную таблицу также надо не отображать.

    Также для ИО надо отобразить виджет с прикрепленными файлами
    А также возможными вложениями (актуально для формуляров)
    */

    if (io)
    {
        //
        // добавляем подчиненную таблицу, если таковая имеется
        //
        //tabEnc = new QTabWidget (attachW);
        
        gAttachLay->addWidget (tabEnc, 0, 0, 2, 1);//, Qt::AlignTop);
#if 0
        QWidget * indWidget = new QWidget ();
        QGridLayout * gIndLay = new QGridLayout (indWidget);
        this->setIndicators(io, indWidget, gIndLay, objEditorWidget);
        int nInd = tabObj->addTab(indWidget, tr("Indicators"));
        qDebug () << __PRETTY_FUNCTION__ << nInd;
        objEditorWidget->setIndicatorNumber (nInd);
        

        KKSRecWidget * indicesW = m_indf->getIOIndices (io, objEditorWidget);
        connect (indicesW,
                 SIGNAL (addEntity (QAbstractItemModel *, const QModelIndex&)),
                 objEditorWidget,
                 SLOT (addIndex (QAbstractItemModel *, const QModelIndex&))
                );
        connect (indicesW,
                 SIGNAL (editEntity (QAbstractItemModel *, const QModelIndex&)),
                 objEditorWidget,
                 SLOT (editIndex (QAbstractItemModel *, const QModelIndex&))
                );
        connect (indicesW,
                 SIGNAL (delEntity (QAbstractItemModel *, const QModelIndex&)),
                 objEditorWidget,
                 SLOT (delIndex (QAbstractItemModel *, const QModelIndex&))
                );
        connect (indicesW,
                 SIGNAL (refreshMod (QAbstractItemModel *)),
                 objEditorWidget,
                 SLOT (refreshIndices (QAbstractItemModel *))
                );
        tabObj->addTab (indicesW, tr("Indices"));
#endif
        QToolButton * tbAddTable = new QToolButton (attachW);
        tbAddTable->setText ("...");// tr ("Add another table"),
        QSizePolicy tbSp (QSizePolicy::Minimum, QSizePolicy::Minimum);
        tbAddTable->setSizePolicy (tbSp);
        tbAddTable->setEnabled (!io->isSystem ());
        gAttachLay->addWidget (tbAddTable, 0, 1, 1, 1, Qt::AlignRight | Qt::AlignTop);
        bool isVisible = (io->category() && io->category()->tableCategory()) || (wCat && wCat->tableCategory());
        tbAddTable->setVisible (isVisible);
        connect (tbAddTable, SIGNAL (clicked()), objEditorWidget, SLOT (addAnotherTable()) );
        connect (objEditorWidget, SIGNAL (addAnotherTable (KKSObject *, KKSObjEditor *)), this, SLOT (addIOTable (KKSObject *, KKSObjEditor *)) );
        KKSRecWidget * recW (0);
        if (io->category() && io->category()->tableCategory())
        {
            KKSTemplate * tChild = NULL;
            tChild = new KKSTemplate (io->tableTemplate() != 0 ? *(io->tableTemplate()) : io->category()->tableCategory()->defTemplate());
            QWidget * copiesW = new QWidget ();
            QGridLayout *gRecLay = new QGridLayout (copiesW);
            recW = KKSViewFactory::createView (tChild, objEditorWidget, io, loader, filters, 0);//copiesW);
            KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (io->category()->tableCategory()->id());
            int nTemplC = lTempls.count();
            recW->actSetView->setEnabled ( nTemplC > 0 );
            objEditorWidget->setRecordsWidget (recW);
            tabEnc->insertTab (0, copiesW, io->category()->tableCategory()->name());
            gRecLay->addWidget (recW, 0, 0, 1, 1);
            tChild->release();
        }
        else if (wCat && wCat->tableCategory())
        {
            KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (wCat->tableCategory()->id());
            int nTemplC = lTempls.count();
            KKSTemplate *tChild = nTemplC > 0 ? lTempls[0] : new KKSTemplate (wCat->tableCategory()->defTemplate());

            QWidget * copiesW = new QWidget ();
            QGridLayout *gRecLay = new QGridLayout ();
            copiesW->setLayout (gRecLay);
            recW = KKSViewFactory::createView (tChild, objEditorWidget, io, loader, filters, 0);//copiesW);
            recW->actSetView->setEnabled ( nTemplC > 0 );
            objEditorWidget->setRecordsWidget (recW);
            tabEnc->insertTab (0, copiesW, wCat->tableCategory()->name() );
            gRecLay->addWidget (recW, 0, 0, 1, 1);
            if (nTemplC <= 0)
                tChild->release ();
        }
        else
            tabObj->setCurrentIndex (0);

        //
        // Дополнительные таблицы, если таковые имеются
        //
        KKSMap<int, KKSAddTable*> addTables = loader->loadIOTables (io);
        objEditorWidget->setAdditionalTables (addTables);
        KKSMap<int, KKSAddTable*>::const_iterator p;
        int it=1;
        for (p=addTables.constBegin(); p!=addTables.constEnd(); p++)
        {
            const KKSCategory * cat = p.value()->category ();
            QString tableName = p.value()->getTableName ();
            KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (cat->id());
            int nTemplC = lTempls.count();
            KKSTemplate *tChild = nTemplC > 0 ? lTempls[0] : new KKSTemplate (cat->defTemplate());

            QWidget * copiesW = new QWidget ();
            QGridLayout *gRecLay = new QGridLayout ();
            copiesW->setLayout (gRecLay);
            KKSRecWidget * recW = KKSViewFactory::createAdditionalView (tChild, objEditorWidget, io, cat, loader, tableName);
            //createView (tChild, objEditorWidget, io, loader, filters, 0);//copiesW);
            recW->actSetView->setEnabled ( nTemplC > 0 );
            objEditorWidget->addRecordsWidget (recW);
            objEditorWidget->addAdditionalCategory (cat);
            tabEnc->insertTab (it, copiesW, p.value()->getTitle());
            it++;
            gRecLay->addWidget (recW, 0, 0, 1, 1);
            if (nTemplC <= 0)
                tChild->release ();
        }

        //
        // прикрепленные файлы
        //
        QWidget * filesW = new QWidget ();
        QGridLayout *gFilesLay = new QGridLayout ();
        filesW->setLayout (gFilesLay);
        KKSList<KKSFileType*> fileTypes = loader->loadFileTypes();
        KKSFileWidget * W = new KKSFileWidget(io->files(), fileTypes, false);
        connect(W, SIGNAL(downloadFile(KKSFile*, QWidget *)), this, SLOT(slotDownloadFile(KKSFile*, QWidget*)));
        objEditorWidget->addFileWidget (W);
        tabObj->addTab (filesW, tr("Files"));
        gFilesLay->addWidget (W, 0, 0, 1, 1);
    }
    else if (!io && isAttrCheckEx && nCount > 0)
    {
        tabObj->setCurrentIndex (0);
        if (isAttrCheckEx)
            gAttachLay->addWidget (tabEnc, 0, 0, 1, 1);
    }
    else if (isAttrCheckEx)
        gAttachLay->addWidget (tabEnc, 0, 0, 1, 1);
    else
        tabObj->setCurrentIndex (0);

    if ((isAttrCheckEx && io) ||
            (tabEnc && tabEnc->count () > 0))
        tabEnc->setCurrentIndex (0);

    if (!io)
    {
        //
        // прикрепленные файлы (теперь они также доступны для записей справочников DynamicDocs)
        //
        QWidget * filesW = new QWidget ();
        QGridLayout *gFilesLay = new QGridLayout ();
        filesW->setLayout (gFilesLay);
        KKSList<KKSFileType*> fileTypes = loader->loadFileTypes();
        KKSList<KKSFile*> files = wObjE->files();
        KKSFileWidget * W = new KKSFileWidget(files, fileTypes, false);
        connect(W, SIGNAL(downloadFile(KKSFile*, QWidget *)), this, SLOT(slotDownloadFile(KKSFile*, QWidget*)));
        objEditorWidget->addFileWidget (W);
        tabObj->addTab (filesW, tr("Files"));
        gFilesLay->addWidget (W, 0, 0, 1, 1);

    }

    //системные параметры для записей пользовательских справочников
    if(!io && wObjE->io()->id() > _MAX_SYS_IO_ID_ ){
        this->putSystemParams(wObjE, objEditorWidget, tabObj, tabObj->count());
    }


    if (mode)
    {
        buttonsLayout->addStretch ();
        buttonsLayout->addWidget (objEditorWidget->pbOk);
        buttonsLayout->addWidget (objEditorWidget->pbCancel);
        buttonsLayout->addWidget (objEditorWidget->pbApply);
    }

    mainLayout->addLayout (buttonsLayout, 1, 1, 2, 1);
    setObjConnect (objEditorWidget);
    objEditorWidget->setObjChanged (idObjE<0);//false);

    if (tSystem)
        tSystem->release();

    if(obj)
        obj->release();
    
    if (wObjE)
        wObjE->release();

    return objEditorWidget;
}

/*
метод отображает указанный экземпляр заданного ИО с заданными параметрами
 при этом
 * idObject -- идентификатор редактируемого/создаваемого ИО
 * idObjE -- идентификатор экземпляра (копии) редактируемого/создаваемого ЭИО
 * filters -- применяемые фильтры, используются для отображения содержимого ИО-справочника
 * extraTitle -- дополнительная надпись, которая применяется в качестве заголовка окна редактора ЭИО (ИО)
 * wCat -- категория, к которой относится редактируемый/создаваемый ИО
 * io_aVals -- список априорно заданных параметров атрибутов ИО
 * mode -- флаг модальности, отвечающий за наличие/отсутствие кнопок OK, Cancel, Apply
 * tableName -- название таблицы справочника отображаемого ИО-справочника (если является стандартной, то игнорируется)
 * bShowExecButton -- наличие/отсутствие кнопки "Отправить адресату" в случае создания ответа на распоряжение,
 * в остальных случаях данная кнопка должна отсутствовать
 * windowModality -- модальность окна
 * parent -- виджет-родитель
 * flags -- сопутствующие флаги создания окна
 Результат
 * виджет редактирования ИО/ЭИО
*/

KKSObjEditor* KKSObjEditorFactory :: createObjEditorParam (int idObject,// идентификатор ИО, который будет содержать создаваемый (редактируемый) ЭИО (для ЭИО, которые являются ИО этот идентификатор должен быть равен IO_IO_ID)
                                                           qint64 idObjE, // идентификатор создаваемого (редактируемого) ЭИО. Если ЭИО создается, должно быть равно -1
                                                           const KKSList<const KKSFilterGroup *> & filters,// Применяется для ЭИО, которые являются контейнерными ИО. Содержит набор фильтров их таблицы
                                                           const QString & extraTitle,
                                                           const KKSCategory* wCat, // категория информационных объектов
                                                           const KKSMap<qint64, KKSAttrValue *>& io_aVals, // список априорно заданных параметров атрибутов ИО
                                                           const KKSMap<qint64, KKSAttrValue *>& aVals, // список априорно заданных параметров атрибутов ЭИО
                                                           bool mode, // наличие кнопок OK, Cancel, Apply
                                                           const QString& tableName, // название доп. таблицы, основная таблица соотвествует пустому значению
                                                           bool bShowExecButton, //определяет наличие кнопки "Отправить адресату" при исполнении распоряжения. Во всех остальных случаях ее быть не должно
                                                           Qt::WindowModality windowModality, // модальность окна
                                                           QWidget *parent,
                                                           Qt::WindowFlags flags
                                                          )
{
    KKSObject *obj = 0;
    KKSObjectExemplar * wObjE = 0;
    const KKSTemplate * tSystem = 0;

    this->loadEntities (obj, wObjE, wCat, idObject, idObjE, tableName, tSystem, false, parent, false);
    if (!wObjE)
        return 0;

    const KKSTemplate * ioTemplate = NULL;
    KKSObject * io = NULL;
    //
    // если ЭИО является ИО, то у него есть еще дополнительные параметры из таблицы attrs_values в БД
    // кроме того, необходимо открыть редактор в выбранном пользователем шаблоне, если шаблонов несколько
    //
    this->loadEIOasIO (ioTemplate, io, idObject, idObjE, wObjE, wCat);

    KKSTemplate * tRec = this->getRecordTemplate(io, wCat);

    QToolBar * tbOper = new QToolBar ();//objEditorWidget);
    KKSObjEditor * objEditorWidget = new KKSObjEditor (tSystem,
                                                       ioTemplate,
                                                       tRec,
                                                       wCat,
                                                       filters,
                                                       extraTitle,
                                                       mode,
                                                       wObjE,
                                                       io,
                                                       (idObject == IO_IO_ID ? true:false),
                                                       tbOper,
                                                       parent,
                                                       flags);

    objEditorWidget->setShowExecButton(bShowExecButton);

    if (mode)
        objEditorWidget->setWindowModality (windowModality);

    QGridLayout *mainLayout = objEditorWidget->getLayout ();
    mainLayout->addWidget (tbOper, 0, 0, 1, 1);
    initActions (tbOper, objEditorWidget);

    int nCount = 0;
    int nWR = 1;

    QTabWidget * tabObj = new QTabWidget (objEditorWidget);
    objEditorWidget->setTabWidget (tabObj);
    if (idObject == IO_IO_ID)// && idObjE > 0 )
        this->setIONameSecret (objEditorWidget, wObjE, io, nWR, mainLayout);

    mainLayout->addWidget (tabObj, nWR, 0, 1, 1);
    QGridLayout *gTabLay = new QGridLayout ();
    tabObj->setLayout (gTabLay);
    QWidget * propertiesWidget = 0;
    QWidget *sysAttrTabWidget = new QWidget ();
    QWidget *ioAttrWidget = 0;
    QGridLayout *gSysLay = new QGridLayout ();
    sysAttrTabWidget->setLayout (gSysLay);
    QGridLayout *gIOLay = 0;
    if (idObject == IO_IO_ID)// && idObjE > 0 )
    {
        propertiesWidget = new QWidget ();
        tabObj->addTab (propertiesWidget, tr ("Properties"));
        QGridLayout * gPropLay = new QGridLayout (propertiesWidget);
        QTabWidget * tabProp = new QTabWidget (propertiesWidget);
        gPropLay->addWidget (tabProp, 0, 0, 1, 1);
        tabProp->addTab (sysAttrTabWidget, tr ("System attributes"));

        //
        //Статистика
        //
        QWidget * statW = new QWidget ();
        QGridLayout *gStatLay = new QGridLayout (statW);
        //statW->setLayout (gStatLay);
        KKSList<KKSStatElement*> sl = loader->loadIOStatistic(io->id());
        KKSStatisticView * statView = new KKSStatisticView(sl);
        tabProp->addTab (statW, tr("History of changing"));
        gStatLay->addWidget (statView, 0, 0, 1, 1);

        //
        // Права доступа
        //
        QWidget * stuffW = new QWidget ();
        QGridLayout * gStuffLay = new QGridLayout ();
        stuffW->setLayout (gStuffLay);
        KKSAccessEntity * acl = io->getAccessRules();
        KKSStuffForm *sForm = m_sf->createStuffEditorForm (acl, io->author(), stuffW);
        tabProp->addTab (stuffW, tr ("User privilegies"));
        gStuffLay->addWidget (sForm, 0, 0, 1, 1);
/*        connect (sForm, SIGNAL (privilegiesChanged (KKSPrivilege *, KKSObject *)),
                 objEditorWidget,
                 SLOT (privChanged (KKSPrivilege *, KKSObject *))
                );*/
        connect (sForm, SIGNAL (accessRulesChanged (KKSAccessEntity *)),
                 objEditorWidget,
                 SLOT (setAccessIO(KKSAccessEntity *))
                );

        if (ioTemplate && ioTemplate->attrsCount() > 0)
        {
            ioAttrWidget = new QWidget();
            gIOLay = new QGridLayout (ioAttrWidget);
            tabObj->insertTab (0, ioAttrWidget, tr ("Attributes"));
        }

        //
        // Рубрикатор
        //
        this->putRubricator (io, objEditorWidget, tabObj);
        //
        // Синхронизация
        //
        if (idObjE != IO_ORG_ID)
            this->putSyncWidget (objEditorWidget, wObjE, idObjE, tabObj);
    }
    else
    {
        tabObj->addTab (sysAttrTabWidget, tr ("Attributes"));
    }

    QScrollArea *scSysAttrs = new QScrollArea (sysAttrTabWidget);
    scSysAttrs->setWidgetResizable (true);
    QWidget *attrWidget = new QWidget ();
    QGridLayout * gAttrLay = new QGridLayout (attrWidget);
    attrWidget->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);

    scSysAttrs->setWidget (attrWidget);

    if (tabObj)
        gSysLay->addWidget (scSysAttrs, 0, 0, 1, 1);
    else
        mainLayout->addWidget (scSysAttrs, 1, 0, 1, 1);

    // objEditorWidget->clearAttributes ();
    QTabWidget * tabEnc = 0;
    bool isAttrCheckEx = false;
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = wCat->attributes().constBegin(); \
            pa != wCat->attributes().constEnd() && !isAttrCheckEx; \
            pa++)
         isAttrCheckEx = isAttrCheckEx || (pa.value()->type ()->attrType() == KKSAttrType::atCheckListEx);

    if (wCat && wCat->tableCategory () && !isAttrCheckEx)
    {
        const KKSCategory * tCat = wCat->tableCategory ();
        for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = tCat->attributes().constBegin(); \
                pa != tCat->attributes().constEnd() && !isAttrCheckEx; \
                pa++)
             isAttrCheckEx = isAttrCheckEx || (pa.value()->type ()->attrType() == KKSAttrType::atCheckListEx);
    }

    QGridLayout * gAttachLay = 0;
    QWidget * attachW = 0;
    if (io || isAttrCheckEx)
    {
        attachW = new QWidget ();
        tabObj->addTab (attachW, tr ("Tables"));
        tabObj->setCurrentIndex (tabObj->count ()-1);
        gAttachLay = new QGridLayout (attachW);
        tabEnc = new QTabWidget (attachW);

        objEditorWidget->setRecTab (tabEnc);
    }
    if (io && !io_aVals.isEmpty())
        this->setPreliminaryAttrs(io, io_aVals);

    this->setPreliminaryAttrs (wObjE, aVals);
    nCount = setAttributes (tSystem, obj, attrWidget, gAttrLay, wCat, wObjE, tableName, objEditorWidget);

    objEditorWidget->setSysAttrWidgets (qobject_cast<QWidget *>(scSysAttrs->parent()), scSysAttrs, attrWidget);
//    qDebug () << __PRETTY_FUNCTION__ << nCount;
//    nCount = setAttrsToEditor (t, obj, scSysAttrs, sysAttrWidget, mainLayout, wObjE, objEditorWidget, true, nCount);

    if (ioAttrWidget)
        this->initIOAttrs(io, wObjE, wCat, objEditorWidget, ioAttrWidget, gIOLay);

    QVBoxLayout *buttonsLayout = new QVBoxLayout ();

    /*если то, что мы отображаем в форме является информационным объектом, т.е. idObject = IO_IO_ID
    то нам надо пристыковать подчиненную таблицу, если она имеется.
    Если то, что мы отображаем не является информационным объектом, а является чисто экземпляром ИО,
    то подчиненной таблицы быть не может
    Кроме того, если идет процесс создания нового ИО (ЭИО), а не редактирования,
    то подчиненную таблицу также надо не отображать.

    Также для ИО надо отобразить виджет с прикрепленными файлами
    А также возможными вложениями (актуально для формуляров)
    */

    if (io)
    {
        //
        // добавляем подчиненную таблицу, если таковая имеется
        //
        //tabEnc = new QTabWidget (attachW);
        gAttachLay->addWidget (tabEnc, 0, 0, 2, 1);//, Qt::AlignTop);
        QToolButton * tbAddTable = new QToolButton (attachW);
        tbAddTable->setText ("...");// tr ("Add another table"),
        QSizePolicy tbSp (QSizePolicy::Minimum, QSizePolicy::Minimum);
        tbAddTable->setSizePolicy (tbSp);
        tbAddTable->setEnabled (!io->isSystem ());
        gAttachLay->addWidget (tbAddTable, 0, 1, 1, 1, Qt::AlignRight | Qt::AlignTop);
        bool isVisible = (io->category() && io->category()->tableCategory()) || (wCat && wCat->tableCategory());
        tbAddTable->setVisible (isVisible);
        connect (tbAddTable, SIGNAL (clicked()), objEditorWidget, SLOT (addAnotherTable()) );
        connect (objEditorWidget, SIGNAL (addAnotherTable (KKSObject *, KKSObjEditor *)), this, SLOT (addIOTable (KKSObject *, KKSObjEditor *)) );
        if (io->category() && io->category()->tableCategory())
        {
            KKSTemplate * tChild = NULL;
            tChild = new KKSTemplate (io->tableTemplate() != 0 ? *(io->tableTemplate()) : io->category()->tableCategory()->defTemplate());
            QWidget * copiesW = new QWidget ();
            QGridLayout *gRecLay = new QGridLayout (copiesW);
            KKSRecWidget * recW = KKSViewFactory::createView (tChild, objEditorWidget, io, loader, filters, 0);//copiesW);
            KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (io->category()->tableCategory()->id());
            int nTemplC = lTempls.count();
            recW->actSetView->setEnabled ( nTemplC > 0 );
            objEditorWidget->setRecordsWidget (recW);
            tabEnc->insertTab (0, copiesW, io->category()->tableCategory()->name());
            gRecLay->addWidget (recW, 0, 0, 1, 1);
            tChild->release();
        }
        else if (wCat && wCat->tableCategory())
        {
            KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (wCat->tableCategory()->id());
            int nTemplC = lTempls.count();
            KKSTemplate *tChild = nTemplC > 0 ? lTempls[0] : new KKSTemplate (wCat->tableCategory()->defTemplate());

            QWidget * copiesW = new QWidget ();
            QGridLayout *gRecLay = new QGridLayout ();
            copiesW->setLayout (gRecLay);
            KKSRecWidget * recW = KKSViewFactory::createView (tChild, objEditorWidget, io, loader, filters, 0);//copiesW);
            recW->actSetView->setEnabled ( nTemplC > 0 );
            objEditorWidget->setRecordsWidget (recW);
            tabEnc->insertTab (0, copiesW, wCat->tableCategory()->name() );
            gRecLay->addWidget (recW, 0, 0, 1, 1);
            if (nTemplC <= 0)
                tChild->release ();
        }
        else
            tabObj->setCurrentIndex (0);

        //
        // Дополнительные таблицы, если таковые имеются
        //
        KKSMap<int, KKSAddTable*> addTables = loader->loadIOTables (io);
        objEditorWidget->setAdditionalTables (addTables);
        KKSMap<int, KKSAddTable*>::const_iterator p;
        int it=1;
        for (p=addTables.constBegin(); p!=addTables.constEnd(); p++)
        {
            const KKSCategory * cat = p.value()->category ();
            QString tableName = p.value()->getTableName ();
            KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (cat->id());
            int nTemplC = lTempls.count();
            KKSTemplate *tChild = nTemplC > 0 ? lTempls[0] : new KKSTemplate (cat->defTemplate());

            QWidget * copiesW = new QWidget ();
            QGridLayout *gRecLay = new QGridLayout ();
            copiesW->setLayout (gRecLay);
            KKSRecWidget * recW = KKSViewFactory::createAdditionalView (tChild, objEditorWidget, io, cat, loader, tableName);
            //createView (tChild, objEditorWidget, io, loader, filters, 0);//copiesW);
            recW->actSetView->setEnabled ( nTemplC > 0 );
            objEditorWidget->addRecordsWidget (recW);
            objEditorWidget->addAdditionalCategory (cat);
            tabEnc->insertTab (it, copiesW, p.value()->getTitle());
            it++;
            gRecLay->addWidget (recW, 0, 0, 1, 1);
            if (nTemplC <= 0)
                tChild->release ();
        }

        //
        // прикрепленные файлы
        //
        QWidget * filesW = new QWidget ();
        QGridLayout *gFilesLay = new QGridLayout ();
        filesW->setLayout (gFilesLay);
        KKSList<KKSFileType*> fileTypes = loader->loadFileTypes();
        KKSFileWidget * W = new KKSFileWidget(io->files(), fileTypes, false);
        connect(W, SIGNAL(downloadFile(KKSFile*, QWidget*)), this, SLOT(slotDownloadFile(KKSFile*, QWidget *)));
        objEditorWidget->addFileWidget (W);
        tabObj->addTab (filesW, tr("Files"));
        gFilesLay->addWidget (W, 0, 0, 1, 1);
    }
    else if (!io && isAttrCheckEx && nCount > 0)
    {
        tabObj->setCurrentIndex (0);
        if (isAttrCheckEx)
            gAttachLay->addWidget (tabEnc, 0, 0, 1, 1);
    }
    else if (isAttrCheckEx)
        gAttachLay->addWidget (tabEnc, 0, 0, 1, 1);
    else
        tabObj->setCurrentIndex (0);

    if ((isAttrCheckEx && io) ||
            (tabEnc && tabEnc->count () > 0))
        tabEnc->setCurrentIndex (0);

    if (mode)
    {
        buttonsLayout->addStretch ();
        buttonsLayout->addWidget (objEditorWidget->pbOk);
        buttonsLayout->addWidget (objEditorWidget->pbCancel);
        buttonsLayout->addWidget (objEditorWidget->pbApply);
    }

    mainLayout->addLayout (buttonsLayout, 1, 1, 2, 1);
    setObjConnect (objEditorWidget);
    objEditorWidget->setObjChanged (idObjE<0);//false);

    if (tSystem)
        tSystem->release();

    if(obj)
        obj->release();

    if (wObjE)
        wObjE->release();

    return objEditorWidget;
}

/* Метод устанавливает первоначальные значения атрибутов в ИО перед созданием/редактированием.
 * Параметры:
 * io -- Информационный объект
 * aVals -- набор значений атрибутов.
 */
void KKSObjEditorFactory :: setPreliminaryAttrs (KKSObject * io, const KKSMap<qint64, KKSAttrValue *>& aVals) const
{
    if (!io || aVals.isEmpty())
        return;

    for (KKSMap<qint64, KKSAttrValue*>::const_iterator pa = aVals.constBegin(); \
         pa != aVals.constEnd(); \
         pa++)
    {
        if (!io->category()->attributes().contains (pa.key()))
            continue;
        if (!io->attrValueId (pa.value()->attribute ()->id ()))
            io->addAttrValue (pa.value ());
        else
        {
            io->removeAttrValueId (pa.value()->attribute ()->id ());
            io->addAttrValue (pa.value ());
        }
    }
}
/* Метод устанавливает первоначальные значения атрибутов в ЭИО перед созданием/редактированием.
 * Параметры:
 * cio -- экземпляр информационного объекта
 * aVals -- набор значений атрибутов.
 */
void KKSObjEditorFactory :: setPreliminaryAttrs (KKSObjectExemplar * cio, const KKSMap<qint64, KKSAttrValue *>& aVals) const
{
    if (!cio || aVals.isEmpty())
        return;

    KKSObject * io = cio->io ();
    if (!io)
        return;

    KKSCategory * c = io->category ();
    if (!c)
        return;

    c = c->tableCategory ();
    if (!c)
        return;
    for (KKSMap<qint64, KKSAttrValue*>::const_iterator pa = aVals.constBegin(); \
         pa != aVals.constEnd(); \
         pa++)
    {
        if (!c->attributes().contains (pa.key()))
            continue;
        if (!cio->attrValue (pa.value()->attribute ()->id ()))
        {
            cio->addAttrValue (pa.value ());
            //int idAType = pa.value()->attribute()->type()->attrType();
            //qDebug () << __PRETTY_FUNCTION__ << idAType;
        }
        else
        {
            KKSAttrValue * av = cio->attrValue (pa.value()->attribute ()->id ());
            cio->removeAttrValue (av);
            cio->addAttrValue (pa.value ());
        }
    }
}
/* Метод устанавливает секретное название ИО перед созданием/редактированием.
 * Параметры:
 * editor -- виджет создания-редактирования
 * wObjE -- экземпляр справочника ИО
 * io -- информационный объект
 * nWR -- результирующий номер строки
 * mainLayout -- лейаут, куда помещается виджет с секретным именем.
 */
void KKSObjEditorFactory :: setIONameSecret (KKSObjEditor * editor, KKSObjectExemplar * wObjE, KKSObject * io, int& nWR, QGridLayout * mainLayout)
{
    QWidget * parentWProp = new QWidget (editor);
    QHBoxLayout * hIOLay = new QHBoxLayout (parentWProp);
    QLabel * lIOName = new QLabel (tr ("Name :"), parentWProp);
    KKSAttrValue * attr = wObjE->attrValue (ATTR_NAME);
    if (wObjE->id () < 0 || !attr )
    {
        QString s;
        KKSValue vName (s, KKSAttrType::atString);
        attr = new KKSAttrValue (vName, wObjE->io()->category()->tableCategory()->attribute (2));//name
        wObjE->addAttrValue (attr);
    }
    //const KKSValue pVal = attr->value();
    //const KKSCategoryAttr * pCategAttr = attr->attribute();
#ifdef Q_CC_MSVC
    QLineEdit * lEIOName = new KKSEdit (attr, KKSIndAttr::KKSIndAttrClass::iacTableAttr, io->name (), parentWProp);//QLineEdit (io->name (), parentWProp);
    connect (lEIOName, 
             SIGNAL (valueChanged(qint64, KKSIndAttr::KKSIndAttrClass, QVariant)), 
             editor, 
             SLOT (setValue (qint64, KKSIndAttr::KKSIndAttrClass, QVariant)) );
#else
    QLineEdit * lEIOName = new KKSEdit (attr, KKSIndAttr::iacTableAttr, io->name (), parentWProp);//QLineEdit (io->name (), parentWProp);
    connect (lEIOName, 
             SIGNAL (valueChanged(qint64, KKSIndAttr::KKSIndAttrClass, QVariant)), 
             editor, 
             SLOT (setValue (qint64, KKSIndAttr::KKSIndAttrClass, QVariant)) );
#endif
    lEIOName->setReadOnly (io->isSystem ());
    hIOLay->addWidget (lIOName);
    hIOLay->addWidget (lEIOName);
    QLabel * lIOMacLabel = new QLabel (tr ("Secrecy signature :"), parentWProp);
    hIOLay->addWidget (lIOMacLabel);
    
    KKSAttrRefWidget *aRefW = new KKSAttrRefWidget (parentWProp);
    attr = wObjE->attrValue (ATTR_ID_MACLABEL);
    int idMaclabel = -1;
    if(attr){
        idMaclabel = attr->value().value().toInt();
    }
    else
        idMaclabel = loader->getCurrMacLabelId();

    if (wObjE->id () < 0 || !attr )
    {
        KKSValue vName (QString::number (idMaclabel), KKSAttrType::atList);
        attr = new KKSAttrValue (vName, wObjE->io()->category()->tableCategory()->attribute (63)); //id_maclabel
        wObjE->addAttrValue (attr);
    }
    //const KKSValue pVal1 = attr->value ();
    //const KKSCategoryAttr * pCategAttr1 = attr->attribute ();
    
    QVariant cV = QString::number (idMaclabel);
    QMap<int, QString> refValues;
    QMap<int, QString> values = loader->loadAttributeValues (attr->attribute (), refValues);
    qDebug () << __PRETTY_FUNCTION__ << values << idMaclabel << cV;
    QString v_str = QString();
    QMap<int, QString>::const_iterator pv = values.constFind (cV.toInt());
    if (pv != values.constEnd())
        v_str = pv.value();

#ifdef Q_CC_MSVC
    QLineEdit * lEIOMacLabel = new KKSEdit (attr, KKSIndAttr::KKSIndAttrClass::iacTableAttr, v_str, aRefW);
#else
    QLineEdit * lEIOMacLabel = new KKSEdit (attr, KKSIndAttr::iacTableAttr, v_str, aRefW);
#endif
    QSizePolicy spMac (QSizePolicy::Minimum, QSizePolicy::Fixed);
    lEIOMacLabel->setReadOnly (true);
    lEIOMacLabel->setSizePolicy (spMac);
    hIOLay->addWidget (aRefW);//lEIOMacLabel);
    QToolButton * tbMac = new QToolButton (aRefW);//parentWPropparentWProp);
    tbMac->setText ("...");
    editor->addListAttrWidget (tbMac, aRefW, attr);
    connect (tbMac, SIGNAL (clicked()), editor, SLOT (setList()) );
#ifdef Q_CC_MSVC
    aRefW->setValue (attr->id(), KKSIndAttr::KKSIndAttrClass::iacTableAttr, cV);
#else
    aRefW->setValue (attr->id(), KKSIndAttr::iacTableAttr, cV);
#endif
    aRefW->setAttrWidget (lEIOMacLabel);
    connect (aRefW, 
             SIGNAL (valueChanged(qint64, KKSIndAttr::KKSIndAttrClass, QVariant)), 
             editor, 
             SLOT (setValue (qint64, KKSIndAttr::KKSIndAttrClass, QVariant)) );

    hIOLay->addWidget (tbMac);
    tbMac->setEnabled (!io->isSystem ());

    mainLayout->addWidget (parentWProp, nWR++, 0, 1, 1);
}

void KKSObjEditorFactory :: loadEntities (KKSObject *& obj,
                                          KKSObjectExemplar *& wObjE,
                                          const KKSCategory* wCat,
                                          int idObject,
                                          qint64 idObjE,
                                          const QString& tableName,
                                          const KKSTemplate *& tSystem,
                                          bool defTemplateOnly,
                                          QWidget * parent,
                                          bool simplify)
{
    obj = loader->loadIO (idObject, true);
    if(!obj)
    {
        qWarning() << "There is no object with id = " << idObject;
        return;
    }
    if (!obj->category() || !obj->category()->tableCategory())
    {
        qWarning() << "Corrupt table category of object with id = " << idObject;
        return;
    }

    wObjE = NULL;
    if (idObjE > 0)
    {
        wObjE = loader->loadEIO (idObjE, obj, wCat, tableName, simplify);
        if (!wObjE)
        {
            qWarning() << "There is no object exemplar with id = " << idObjE;
            return;
        }
    }
    else
       wObjE = new KKSObjectExemplar (-1, tr("New record"), obj);

    /*const KKSTemplate * */
    //tSystem = NULL;
    //
    //открываем системные атрибуты элемента справочника в выбранном пользователем шаблоне,
    //если шаблонов у категории несколько
    //
    bool withMand (idObjE<=0);
    if (!tSystem && !defTemplateOnly)
    {
        if (wCat && wCat->tableCategory () && idObject != IO_IO_ID)
            tSystem = getTemplate (wCat->tableCategory(), withMand, parent);
        else if (wCat && wCat->type()->id () == 10 && idObject != IO_IO_ID)
            tSystem = getTemplate (wCat, withMand, parent);
        else
            tSystem = getTemplate (obj->category()->tableCategory(), withMand, parent);
    }
    else if (!tSystem)
        tSystem = new KKSTemplate (obj->category()->tableCategory()->defTemplate());
    
    //qDebug () << __PRETTY_FUNCTION__ << obj->category()->tableCategory()->id () << (wCat && wCat->tableCategory() ? wCat->tableCategory()->id() : -1);

    //obj->release();
    return;
}

void KKSObjEditorFactory :: loadRecEntities (KKSObject * obj, 
                                             KKSObjectExemplar * wObjE, 
                                             const KKSCategory* wCat, 
                                             int idObject, 
                                             qint64 idObjE, 
                                             const QString& tableName, 
                                             const KKSTemplate *& tRecAttr, 
                                             bool defTemplateOnly, 
                                             QWidget * parent)
{
    Q_UNUSED(tableName);
    Q_UNUSED(wObjE);
    if(!obj || obj->id() != idObject)
    {
        qWarning() << "There is no object with id = " << idObject;
        return;
    }
    if (!obj->category() || (!obj->category()->recAttrCategory() && !wCat->recAttrCategory()))
        return;
        //qWarning() << "Corrupt table category of object with id = " << idObject;
/*
    if (idObjE > 0)
    {
        wObjE = loader->loadEIO (idObjE, obj, wCat, tableName);
        if (!wObjE)
        {
            qWarning() << "There is no object exemplar with id = " << idObjE;
            return;
        }
    }
    else
       wObjE = new KKSObjectExemplar (-1, tr("New record"), obj);
 */
    /*const KKSTemplate * */
    //tSystem = NULL;
    //
    //открываем системные атрибуты элемента справочника в выбранном пользователем шаблоне,
    //если шаблонов у категории несколько
    //
    bool withMand (idObjE<=0);
    if (!tRecAttr && !defTemplateOnly)
    {
        if (wCat && wCat->recAttrCategory () && idObject != IO_IO_ID)
            tRecAttr = getTemplate (wCat->recAttrCategory(), withMand, parent);
        else if (wCat && wCat->type()->id () == 10 && idObject != IO_IO_ID)
            tRecAttr = getTemplate (wCat, withMand, parent);
        else
            tRecAttr = getTemplate (obj->category()->recAttrCategory(), withMand, parent);
    }
    else if (!tRecAttr)
        tRecAttr = new KKSTemplate (obj->category()->recAttrCategory()->defTemplate());
    
    //qDebug () << __PRETTY_FUNCTION__ << obj->category()->tableCategory()->id () << (wCat && wCat->tableCategory() ? wCat->tableCategory()->id() : -1);

    //obj->release();
    return;
}

void KKSObjEditorFactory :: loadEIOasIO (const KKSTemplate *& ioTemplate, KKSObject *& io, int idObject, qint64 idObjE, KKSObjectExemplar * wObjE,  const KKSCategory* wCat)
{
    if (idObject == IO_IO_ID && idObjE > 0/*this->isIO (idObjE) && Юра был не прав!*/)
    {
        io = loader->loadIO (idObjE);
        if (!io)
        {
            qWarning() << __PRETTY_FUNCTION__ << "Object does not exist!";
            return;
        }
        if (io->attrTemplate())
        {
            ioTemplate = io->attrTemplate ();
            ioTemplate->addRef();
        }
        else
            ioTemplate = new KKSTemplate (io->category()->defTemplate ());

        for (int i=0; i< io->attrValues().count(); i++)
            if (io->attrValue(i))
                wObjE->addAttrValue (io->attrValue(i));
    }
    else if (idObject == IO_IO_ID && idObjE <= 0)
    {
        //
        // У вновь создаваемого ИО категория может быть задана, в данном случае для пользовательских параметров
        // используется шаблон по умолчанию для данной категории.
        //
        io = new KKSObject();
        //
        // Автором создаваемого ИО может быть только текущий пользователь.
        //
        int idAuthor = loader->getUserId();
        io->setAuthor (idAuthor);
        io->setIdMaclabel(loader->getCurrMacLabelId());
        io->setOwnerOrg(loader->getOrgId());
        //
        // Права доступа по умолчанию должны быть заданы явно
        //
        KKSPrivilege  * authPriv = KKSPrivilege::authPrivilege (idAuthor);
        KKSAccessEntity * acl = new KKSAccessEntity();
        acl->addPrivilege (idAuthor, authPriv);
        authPriv->release ();
        QList<int> bossList = loader->getBossList (loader->getUserId());
        for (int i=0; i<bossList.count(); i++)
        {
            //bool isBossAuth (bossList[i] == loader->getDlId());
            KKSPrivilege * ioBossPriv = KKSPrivilege::bossPrivilege (bossList[i]);//, true, true, isBossAuth, true, true);
            if (ioBossPriv)
            {
                acl->addBossPrivilege (bossList[i], ioBossPriv);
                acl->addPrivilege (bossList[i], ioBossPriv);
                ioBossPriv->release ();
            }
        }
        QList<int> unitList = loader->getUnitList (loader->getUserId());
        for (int i=0; i<unitList.count(); i++)
        {
            KKSPrivilege * ioUnitPriv = KKSPrivilege::unitPrivilege (unitList[i]);//, true, true, false, false, false);
            if (ioUnitPriv)
            {
                acl->addUnitPrivilege (unitList[i], ioUnitPriv);
                acl->addPrivilege (unitList[i], ioUnitPriv);
                ioUnitPriv->release ();
            }
        }
        KKSPrivilege * othersPriv = new KKSPrivilege (OTHERS_ROLE, false, false, false, false, false);
        acl->setOthersPrivilege (othersPriv);
        io->setAccessRules (acl);
        acl->release ();
        if (wCat)
        {
            io->setCategory(const_cast<KKSCategory*>(wCat));

            ioTemplate = new KKSTemplate (wCat->defTemplate());
            //если категория имеет рубрики, то они должны стать вложениями данного ИО
            if (wCat->rootRubric())
                io->setRootRubric (wCat->rootRubric()->deepCopy(true));
        }
        else
            ioTemplate = 0;
    }
}

/* Метод устанавливает шаблон отображения атрибутов на виджете.
 * Параметры:
 * io -- ИО,
 * wObjE -- соответствующий экземпляр справочника ИО
 * wCat -- категория ИО
 * editor -- виджет редактирования ИО
 * ioAttrWidget -- виджет с атрибутами
 * gIOLay -- соотв. лейаут
 */
void KKSObjEditorFactory :: initIOAttrs (KKSObject * io, KKSObjectExemplar * wObjE, const KKSCategory* wCat, KKSObjEditor * editor, QWidget * ioAttrWidget, QGridLayout * gIOLay)
{
    if (!io || !wObjE || !gIOLay || !ioAttrWidget)
        return;

    QPushButton *pbSetView = new QPushButton (tr("Set view"), ioAttrWidget);
    bool isCreate = (wObjE->id() <= 0);
    KKSList<KKSTemplate*> tcList;
    if (wCat)
        tcList = loader->loadCategoryTemplates (wCat->id(), isCreate);
    else
        tcList = loader->loadCategoryTemplates (io->category(), isCreate);
    bool isEn (tcList.count () > 0);
    if (isEn)
        io->setAttrTemplate (tcList[0]);
    gIOLay->addWidget (pbSetView, 1, 1, 1, 1);
    pbSetView->setEnabled (isEn);
    QSpacerItem *sPar = new QSpacerItem (20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gIOLay->addItem (sPar, 1, 0, 1, 1);

    QScrollArea * scIOAttrs = new QScrollArea (ioAttrWidget);
    scIOAttrs->setWidgetResizable(true);
    QWidget *ioAttrsW = new QWidget ();
    QGridLayout *gIOAttrLay = new QGridLayout (ioAttrsW);
    ioAttrsW->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
    scIOAttrs->setWidget (ioAttrsW);
    editor->setIOAttrWidgets (ioAttrWidget, scIOAttrs, ioAttrsW);

    ioAttrsW->setLayout (gIOAttrLay);
    gIOLay->addWidget (scIOAttrs, 0, 0, 1, 2);

    connect (pbSetView, SIGNAL (clicked()), editor, SLOT (setAttrView()));
    KKSTemplate *tio = 0;
    if (io->attrTemplate ())
    {
        tio = io->attrTemplate();
        tio->addRef ();
    }
    else if (wCat)
        tio = new KKSTemplate (wCat->defTemplate ());
    else
        tio = new KKSTemplate (io->category()->tableCategory()->defTemplate());

    setAttributes (tio, io, ioAttrsW, gIOAttrLay, (wCat ? wCat : io->category()->tableCategory()), editor);
    if (tio)
        tio->release ();

}

/*
 * Метод выводит список записей ИО-справочника в модальном режиме,
 * idObject -- идентификатор справочника ИО (как правило IO_IO_ID)
 * idObjE -- идентификатор ИО-справочника в ИО
 * filters -- применяемые фильтры
 * extraTitle -- дополнительная надпись, которая применяется в качестве заголовка окна редактора ЭИО (ИО)
 * wCat -- категория, к которой относится редактируемый/создаваемый ИО
 * mode -- флаг модальности, отвечающий за наличие/отсутствие кнопок OK, Cancel, Apply
 * windowModality -- модальность окна
 * parent -- виджет-родитель
 * f -- флаги создания окна
 Результат
 * виджет просмотра и выбора подходящей записи.
*/

KKSRecDialog* KKSObjEditorFactory :: createObjRecEditor (int idObject,// идентификатор ИО, который будет содержать создаваемый (редактируемый) ЭИО (для ЭИО, которые являются ИО этот идентификатор должен быть равен IO_IO_ID) 
                                                         qint64 idObjE, // идентификатор создаваемого (редактируемого) ЭИО. Если ЭИО создается, должно быть равно -1
                                                         const KKSList<const KKSFilterGroup *> & filters,// Применяется для ЭИО, которые являются контейнерными ИО. Содержит набор фильтров их таблицы
                                                         const QString & extraTitle,
                                                         const KKSCategory* wCat, // категория информационных объектов
                                                         bool mode, // наличие кнопок OK, Cancel, Apply
                                                         bool toolB,
                                                         Qt::WindowModality windowModality, // модальность окна
                                                         QWidget *parent, 
                                                         Qt::WindowFlags flags)
{
    if (idObjE == IO_SEARCH_ID)
    {
        KKSSearchTemplatesForm *stForm = new KKSSearchTemplatesForm (wCat, "io_objects", true, parent, flags);
        this->initSearchTemplateModel (stForm);
        return stForm;
    }
    KKSObject *obj = 0;
    KKSObjectExemplar * wObjE = 0;
    const KKSTemplate * tSystem = 0;//new KKSTemplate (obj->category()->tableCategory()->defTemplate());
    QString tableName = QString ();

    this->loadEntities (obj, wObjE, wCat, idObject, idObjE, tableName, tSystem, true, parent, false);
 //   tSystem = new KKSTemplate (obj->category()->tableCategory()->defTemplate());

    const KKSTemplate * ioTemplate = NULL;
    KKSObject * io = NULL;
    //
    // если ЭИО является ИО, то у него есть еще дополнительные параметры из таблицы attrs_values в БД
    // кроме того, необходимо открыть редактор в выбранном пользователем шаблоне, если шаблонов несколько
    //
    this->loadEIOasIO (ioTemplate, io, idObject, idObjE, wObjE, wCat);

    KKSTemplate * tRec = this->getRecordTemplate(io, wCat);

    KKSObjEditor * objEditorWidget = new KKSObjEditor (tSystem, ioTemplate, tRec, wCat, filters, extraTitle, mode, wObjE, io, (idObject == IO_IO_ID ? true:false), 0, parent, flags);

    if (mode)
        objEditorWidget->setWindowModality (windowModality);

    QGridLayout *mainLayout = objEditorWidget->getLayout ();
    //QVBoxLayout *buttonsLayout = new QVBoxLayout ();

    /*если то, что мы отображаем в форме является информационным объектом, т.е. idObject = IO_IO_ID
    то нам надо пристыковать подчиненную таблицу, если она имеется.
    Если то, что мы отображаем не является информационным объектом, а является чисто экземпляром ИО,
    то подчиненной таблицы быть не может
    Кроме того, если идет процесс создания нового ИО (ЭИО), а не редактирования, 
    то подчиненную таблицу также надо не отображать.

    Также для ИО надо отобразить виджет с прикрепленными файлами
    А также возможными вложениями (актуально для формуляров)
    */

    if (io)
    {
        //
        // добавляем подчиненную таблицу, если таковая имеется
        //
        KKSRecWidget * recW = 0;
        if (io->category() && io->category()->tableCategory())
        {
            KKSTemplate * tChild = NULL;
            tChild = new KKSTemplate (io->tableTemplate() != 0 ? *(io->tableTemplate()) : io->category()->tableCategory()->defTemplate());
            recW = KKSViewFactory::createView (tChild, objEditorWidget, io, loader, filters, 0);//copiesW);
            if (!recW)
                return objEditorWidget;
            KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (io->category()->tableCategory()->id());
            int nTemplC = lTempls.count();
            recW->actSetView->setEnabled ( nTemplC > 0 );
            objEditorWidget->setRecordsWidget (recW);
            mainLayout->addWidget (recW, 0, 0, 1, 1);
            tChild->release();
        }
        else if (wCat && wCat->tableCategory())
        {
            KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (wCat->tableCategory()->id());
            int nTemplC = lTempls.count();
            KKSTemplate *tChild = nTemplC > 0 ? lTempls[0] : new KKSTemplate (wCat->tableCategory()->defTemplate());

            recW = KKSViewFactory::createView (tChild, objEditorWidget, io, loader, filters, 0);//copiesW);
            if (!recW)
                return objEditorWidget;
            recW->actSetView->setEnabled ( nTemplC > 0 );
            objEditorWidget->setRecordsWidget (recW);
            mainLayout->addWidget (recW, 0, 0, 1, 1);
            if (nTemplC <= 0)
                tChild->release ();
        }
        if (toolB)
        {
            recW->showToolBar();
            recW->hideGroup (0);
            recW->hideGroup (2);
            recW->hideGroup (3);
        }
        else
            recW->hideToolBar();//hideAllButtons ();//gbSearch->setVisible (true);
//        recW->hideGroup (1);//gbEdit->setVisible (false);
//        recW->hideGroup (2);//gbImportExport->setVisible (false);
//        recW->tbSetView->setVisible (false);
        recW->pbOk->setVisible (mode || windowModality != Qt::NonModal);
        recW->pbCancel->setVisible (mode || windowModality != Qt::NonModal);
        connect (recW->pbOk, SIGNAL (clicked()), objEditorWidget, SLOT (accept()) );
        connect (recW->pbCancel, SIGNAL (clicked()), objEditorWidget, SLOT (reject()) );
        connect (recW->pbApply, SIGNAL (clicked()), objEditorWidget, SLOT (apply()) );


        io->release();
    }

    setObjConnect (objEditorWidget);
    objEditorWidget->setObjChanged (false);

    if (tSystem)
        tSystem->release();

    if(obj)
        obj->release();
    
    if (wObjE)
        wObjE->release();

    return objEditorWidget;
}

KKSTemplate * KKSObjEditorFactory :: getRecordTemplate (KKSObject * io, const KKSCategory* wCat)
{
    if (!io)
        return 0;
    //
    // добавляем шаблон записей, если таковые имеются
    //
    KKSTemplate * tRec = 0;
    if (io->category() && io->category()->tableCategory())
        tRec = new KKSTemplate (io->tableTemplate() != 0 ? *(io->tableTemplate()) : io->category()->tableCategory()->defTemplate());
    else if (wCat && wCat->tableCategory())
    {
        KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (wCat->tableCategory()->id());
        int nTemplC = lTempls.count();
        KKSTemplate *tChild = nTemplC > 0 ? lTempls[0] : new KKSTemplate (wCat->tableCategory()->defTemplate());
        tRec = new KKSTemplate (*tChild);
        if (nTemplC <= 0)
            tChild->release ();
    }
    return tRec;
}

void KKSObjEditorFactory :: setObjConnect (KKSObjEditor *editor)
{
    if (!editor)
        return;
    connect (editor, SIGNAL(includeRequested(KKSObjEditor*)), this, SLOT(slotIncludeRequested(KKSObjEditor*)));
    connect (editor, SIGNAL(includeRecRequested(KKSObjEditor*)), this, SLOT(slotIncludeRecRequested(KKSObjEditor*)));
    connect (editor, SIGNAL(openRubricItemRequested(int, KKSObjEditor*)), this, SLOT(slotOpenRubricItemRequested(int, KKSObjEditor*)));
    connect (editor, SIGNAL(openRubricItemRecRequested(int, KKSObjEditor*)), this, SLOT(slotOpenRubricItemRecRequested(int, KKSObjEditor*)));
    connect (editor, SIGNAL (updateAttributes (QWidget *, QScrollArea *, QWidget *, int, const KKSCategory *, KKSIndAttr::KKSIndAttrClass, KKSObjEditor*)), this, SLOT (regroupAttrs (QWidget *, QScrollArea *, QWidget *, int, const KKSCategory*, KKSIndAttr::KKSIndAttrClass, KKSObjEditor*)) );
    connect (editor, SIGNAL (saveObj(KKSObjEditor*, KKSObject*, KKSObjectExemplar*, int, QAbstractItemModel *)), this, SLOT (saveObject(KKSObjEditor*, KKSObject*, KKSObjectExemplar*, int, QAbstractItemModel *)) );
    connect (editor, SIGNAL (saveObjAsCommandResult(KKSObjEditor*, KKSObject*, KKSObjectExemplar*, int, QAbstractItemModel *)), this, SLOT (saveObjectAsCommandResult(KKSObjEditor*, KKSObject*, KKSObjectExemplar*, int, QAbstractItemModel *)) );
    connect (editor, SIGNAL (saveObjE(KKSObjEditor*, KKSObjectExemplar *, const KKSCategory*, QString, int, QAbstractItemModel *)), this, SLOT (saveObjectEx(KKSObjEditor*, KKSObjectExemplar *, const KKSCategory*, QString, int, QAbstractItemModel *)) );
    connect (editor, SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)), this, SLOT (createNewEditor(QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)) );
    connect (editor, SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex&)), this, SLOT (editExistOE (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex&)) );
    connect (editor, SIGNAL (delObjectEx (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex& )), this, SLOT (deleteOE (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)) );
    connect (editor, SIGNAL (filterObjectEx (KKSObjEditor*, int, const KKSCategory *, QString)), this, SLOT (filterEIO (KKSObjEditor*, int, const KKSCategory *, QString)) );
    connect (editor, SIGNAL (refreshObjectEx (KKSObjEditor*, int, const KKSCategory *, QString, QAbstractItemModel *)), this, SLOT (refreshEIO (KKSObjEditor*, int, const KKSCategory *, QString, QAbstractItemModel *)) );
    connect (editor, SIGNAL (filterObjectTemplateEx (KKSObjEditor*, int, const KKSCategory *, QString)), this, SLOT (filterTemplateEIO (KKSObjEditor*, int, const KKSCategory *, QString)) );
    connect (editor, SIGNAL (updateEIO (KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QItemSelection&, const QModelIndex& )), this, SLOT (updateEIOView (KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QItemSelection&, const QModelIndex& )) );
    connect (editor, SIGNAL (loadAttrRef (QString, QWidget*, int)), this, SLOT (loadAttributeReference (QString, QWidget *, int)) );
    connect (editor, SIGNAL (loadStateRef (KKSObjEditor*, QWidget *, KKSObjectExemplar *)), this, SLOT (setRecState(KKSObjEditor*, QWidget *, KKSObjectExemplar *)) );
    connect (editor, SIGNAL (setTemplate (KKSObjEditor*, KKSObject*)), this, SLOT (setEIOTemplates (KKSObjEditor*, KKSObject*)) );
    connect (editor, SIGNAL (importObjectEx (KKSObjEditor *, int, const KKSCategory *, QString, QAbstractItemModel *)), this, SLOT (importEIO (KKSObjEditor *, int, const KKSCategory *, QString, QAbstractItemModel *)) );
    connect (editor, SIGNAL (exportObjectEx (KKSObjEditor *, int, const KKSCategory *, QString, QAbstractItemModel *)), this, SLOT (exportEIO (KKSObjEditor *, int, const KKSCategory *, QString, QAbstractItemModel *)) );
    connect (editor, SIGNAL (prepareIO (KKSObject*, KKSObjectExemplar *, KKSObjEditor*)), this, SLOT (sendIO (KKSObject *, KKSObjectExemplar *, KKSObjEditor *)) );
    
    connect (editor, SIGNAL (editObjAttrRef (KKSObject *, const KKSAttrValue*, KKSIndAttr::KKSIndAttrClass, QAbstractItemModel *)), this, SLOT (loadObjAttrRef (KKSObject *, const KKSAttrValue*, KKSIndAttr::KKSIndAttrClass, QAbstractItemModel *)) );
    connect (editor, SIGNAL (editObjCAttrRef (KKSObjectExemplar *, const KKSAttrValue*, KKSIndAttr::KKSIndAttrClass, QAbstractItemModel *) ), this, SLOT (loadObjCAttrRef (KKSObjectExemplar *, const KKSAttrValue*, KKSIndAttr::KKSIndAttrClass, QAbstractItemModel *) ) );
    connect (editor, SIGNAL (delObjAttrRef (KKSObject *, const KKSAttrValue*, KKSIndAttr::KKSIndAttrClass, QAbstractItemModel *, const QModelIndex&)), this, SLOT (loadObjDelAttrRef (KKSObject *, const KKSAttrValue*, KKSIndAttr::KKSIndAttrClass, QAbstractItemModel *, const QModelIndex&)) );
    connect (editor, SIGNAL (delObjCAttrRef (KKSObjectExemplar *, const KKSAttrValue*, KKSIndAttr::KKSIndAttrClass, QAbstractItemModel *, const QModelIndex&)), this, SLOT (loadObjCDelAttrRef (KKSObjectExemplar *, const KKSAttrValue*, KKSIndAttr::KKSIndAttrClass, QAbstractItemModel *, const QModelIndex&)) );

    connect (editor, SIGNAL (openRefIO (QString)), this, SLOT (loadRefIO (QString)) );
    connect (editor, SIGNAL (openRefRec (QString, qint64)), this, SLOT (loadRefRec (QString, qint64)) );

    connect (editor, SIGNAL (printReport(KKSObject*)), this, SLOT (printReport(KKSObject*)));

    connect (editor, SIGNAL (addIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel*)), m_indf, SLOT(addIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel *)) );
    connect (editor, SIGNAL (editIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel*)), m_indf, SLOT(editIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel *)) );
    connect (editor, SIGNAL (delIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel*)), m_indf, SLOT(delIOIndex (KKSObject *, const QModelIndex&, QAbstractItemModel *)) );
    connect (editor, SIGNAL (refreshIOIndex (KKSObject *, QAbstractItemModel*)), m_indf, SLOT(refreshIndices (KKSObject *, QAbstractItemModel *)) );
    connect (editor, SIGNAL (viewIOIndicator (KKSObject *, int)), m_indf, SLOT (viewIOIndicator (KKSObject *, int)) );
    connect (editor, SIGNAL (editIOIndicator (KKSObject *, int, QWidget *)), m_indf, SLOT (editIOIndicator (KKSObject *, int, QWidget *)) );
    connect (editor, SIGNAL (addIOIndicator (KKSObject *, QWidget *)), m_indf, SLOT (addNewIOIndicator (KKSObject *, QWidget *)) );

    if ((editor->getObj () && editor->getObj()->id() < 0) ||
        (editor->getObjectEx() && editor->getObjectEx()->id() < 0))
    {
        connect (editor, SIGNAL(loadHistory(const KKSAttrValue *, qint64, qint64, bool)), this->m_awf, SLOT(loadIOAttrValueHistoryR(const KKSAttrValue *, qint64, qint64, bool)));
        connect (m_awf, SIGNAL(viewHistory(const KKSAttrValue *, const KKSList<KKSAttrValue *> &)), editor, SLOT(viewAHist(const KKSAttrValue *, const KKSList<KKSAttrValue *> &)));
    }
    
    connect (editor, SIGNAL (generateUUID (int, const KKSAttrValue *)), this, SLOT (genUUID (int, const KKSAttrValue *)) );
    connect (this, SIGNAL (setuuid (QString)), editor, SLOT (setIOUUID (QString)) );
    connect (this, SIGNAL (cioSaved(KKSObjectExemplar *)), editor, SLOT (recSaved(KKSObjectExemplar *)) );
}

/*
 * Метод выбирает подходящий пользовательский шаблон для создаваемого/редактируемого ЭИО.
 * c -- категория ИО
 * withAllMandatories -- флаг, указывающий должны ли отображаться все обязательные атрибуты
 * (необходимо при создании нового ЭИО/ИО
 * parent -- родительский виджет для формы выбора шаблонов.
 */
KKSTemplate * KKSObjEditorFactory :: getTemplate (const KKSCategory * c, bool withAllMandatories, QWidget * parent)
{
    if (!c)
        return 0;

    KKSTemplate * t = NULL;
    KKSTemplate * t0 = new KKSTemplate (c->defTemplate());

    if (!t0)
        return 0;

    KKSList<KKSTemplate*> tList;
    KKSCategory *c1 = const_cast <KKSCategory*>(c);
    c1->addRef ();
    KKSList<KKSTemplate*> tListDb = loader->loadCategoryTemplates (c1, withAllMandatories);
    c1->release ();
    if (tListDb.count() == 0)
        tList.append (t0);

    for (int i=0; i<tListDb.count(); i++)
        tList.append (tListDb[i]);

    if (tList.count() == 1)
    {
        t = tList.at(0);
        t->addRef();
    }
    else
    {
        ChooseTemplateForm * f = new ChooseTemplateForm(tList, parent);
        if (f->exec() == QDialog::Accepted)
        {
            t = f->getCurrentTemplate();
            t->addRef();
        }
        else
            t = new KKSTemplate(c->defTemplate());

        if (!parent)
            delete f;
    }

    return t;
}

/*
 * слот сохранения ЭИО в БД, параметры
 * editor -- виджет редактора ЭИО
 * wObjE -- сохраняемый ЭИО
 * cat -- категория таблицы, куда сохраняется ЭИО
 * tableName -- название таблицы, содержащей ЭИО
 * num -- количество копий сохраняемых ЭИО (по умолчанию 1)
 * recModel --  -- модель, в которую производится запись для отображения в таблице
 *  в случае успешного завершения.
 */

void KKSObjEditorFactory :: saveObjectEx (KKSObjEditor * editor, KKSObjectExemplar *wObjE, const KKSCategory* cat, QString tableName, int num, QAbstractItemModel * recModel)
{
    if (!editor->isObjChanged())
    {
        //
        // Все изменения уже сохранены
        //
        return;
    }
    bool isNew (wObjE->id() < 0);
    int nc = (isNew ? num : 1);
    bool transactionOk = true;

    if (wObjE->id() >=0 && num > 1)
    {
        qWarning() << tr ("Save IO copy. Only current copy will be updated");
        QMessageBox::warning (editor, tr ("Warning"), tr ("Save IO copy. Only current copy will be updated"), QMessageBox::Ok);
    }

    editor->clearAdditionalCopies ();
    KKSList<KKSObjectExemplar *> recList;
    for (int i=0; i<nc; i++)
    {
        int res;
        KKSObjectExemplar * wObjCopy = new KKSObjectExemplar (*wObjE);

        if (isNew)
        {
            wObjCopy->setId (-1);
            qDebug () << __PRETTY_FUNCTION__ << tableName << wObjCopy->io()->tableName();
            res = eiof->insertEIO (wObjCopy, cat, tableName, false, editor);
            if (res == OK_CODE)
            {
                editor->addAdditionalCopy (wObjCopy);
                recList.append (wObjCopy);
            }
            else
                transactionOk = false;

        }
        else
            res = eiof->updateEIO (wObjE, cat, tableName, editor);

        if (res != OK_CODE)
        {
            qCritical() << tr("An error was occured while inserting or updating EIO in database!");
            QMessageBox::critical (editor, 
                                   tr("Error"), 
                                   tr("An error was occured while inserting or updating EIO in database!"), 
                                   QMessageBox::Ok);
            transactionOk = false;
            break;
        }
        else
            recList.append(wObjE);
        if (isNew && i==0)
            wObjE->setId (wObjCopy->id());
        wObjCopy->release ();
    }
    if (!transactionOk)
        return;
    editor->setObjChanged (!transactionOk);
    Q_UNUSED (recModel);
/*    if (!recModel)
        return;
    int ier = 0;
    for (int i=0; i<recList.count() && ier >=0; i++)
        ier = writeRecIntoModel (recModel, recList[i]);*/
    emit cioSaved (wObjE);
}

int KKSObjEditorFactory :: writeRecIntoModel (QAbstractItemModel * recModel, KKSObjectExemplar * wObjE) const
{
    if (!recModel)
        return ERROR_CODE;
    const KKSCategoryAttr * cAttrP = recModel->data(QModelIndex(), Qt::UserRole+3).value<const KKSCategoryAttr *>();
    QModelIndex rIndex = KKSViewFactory::searchModelRowsIndex (recModel, wObjE->id(), QModelIndex(), Qt::UserRole);
    //qDebug () << __PRETTY_FUNCTION__ << (cAttrP ? cAttrP->id() : -1) << rIndex;
    int ier = OK_CODE;
    if (rIndex.isValid() && (!cAttrP || cAttrP->id() <=0))
    {
        ier = writeRecords (recModel, wObjE, rIndex);
        if (ier < 0)
            return ier;
    }
    else if (rIndex.isValid() && cAttrP && cAttrP->id() > 0)
    {
        int idP = wObjE->attrValue(cAttrP->id())->value().value().toInt();
        //qDebug () << __PRETTY_FUNCTION__ << idP << rIndex.parent().data (Qt::UserRole).toInt();
        if ((idP <= 0 && !rIndex.parent().isValid())
                || (idP > 0 && rIndex.parent().isValid() && rIndex.parent().data (Qt::UserRole).toInt() == idP))
        {
            ier = writeRecords (recModel, wObjE, rIndex);
            if (ier < 0)
                return ier;
        }
        else if (idP <= 0 && rIndex.parent().isValid())
        {
            int iRow = rIndex.row ();
            QModelIndex pIndex = rIndex.parent();
            recModel->removeRows(iRow, 1, pIndex);
            int nr = recModel->rowCount ();
            recModel->insertRows(nr, 1);
            rIndex = recModel->index (recModel->rowCount()-1, 0);
            ier = writeRecords (recModel, wObjE, rIndex);
            if (ier < 0)
                return ier;
        }
        else if (idP > 0 && rIndex.parent().isValid())
        {
            int iRow = rIndex.row ();
            QModelIndex pIndex = rIndex.parent();
            recModel->removeRows(iRow, 1, pIndex);
            pIndex = KKSViewFactory::searchModelRowsIndex(recModel, idP, QModelIndex(), Qt::UserRole);
            int nr = recModel->rowCount (pIndex);
            recModel->insertRows (nr, 1, pIndex);
            rIndex = recModel->index (nr, 0, pIndex);
            ier = writeRecords (recModel, wObjE, rIndex);
            if (ier < 0)
                return ier;
        }
        else if (idP > 0 && !rIndex.parent().isValid())
        {
            QModelIndex pIndex = KKSViewFactory::searchModelRowsIndex(recModel, idP, QModelIndex(), Qt::UserRole);
            int iRow = rIndex.row ();
            recModel->removeRows (iRow, 1);
            int nr = recModel->rowCount (pIndex);
            recModel->insertRows (nr, 1, pIndex);
            rIndex = recModel->index (nr, 0, pIndex);
            ier = writeRecords (recModel, wObjE, rIndex);
            if (ier < 0)
                return ier;
        }
    }
    else if (!rIndex.isValid() && (!cAttrP || cAttrP->id() <=0))
    {
        int nr = recModel->rowCount();
        recModel->insertRows (nr, 1);
        rIndex = recModel->index (nr, 0);
        qDebug () << __PRETTY_FUNCTION__ << rIndex;
        ier = writeRecords (recModel, wObjE, rIndex);
        if (ier < 0)
            return ier;
    }
    else if (!rIndex.isValid () && cAttrP && cAttrP->id() > 0)
    {
        int idP = wObjE->attrValue(cAttrP->id())->value().value().toInt();
        QModelIndex pIndex = KKSViewFactory::searchModelRowsIndex(recModel, idP, QModelIndex(), Qt::UserRole);
        int nr = recModel->rowCount (pIndex);
        bool isInserted = recModel->insertRows (nr, 1, pIndex);
        rIndex = recModel->index (nr, 0, pIndex);
        qDebug () << __PRETTY_FUNCTION__ << isInserted << rIndex;
        ier = writeRecords (recModel, wObjE, rIndex);
        if (ier < 0)
            return ier;
    }
    return OK_CODE;
}

int KKSObjEditorFactory :: writeRecords (QAbstractItemModel * recModel, KKSObjectExemplar * wObjE, const QModelIndex& rIndex) const
{
    if (!recModel || !rIndex.isValid())
        return ERROR_CODE;
    if (!recModel->setData (rIndex, wObjE->id(), Qt::UserRole))
        return ERROR_CODE;
    KKSCategory * c = wObjE->io()->category()->tableCategory ();
    KKSList<const KKSFilterGroup *> filters;
    KKSList<const KKSFilter *> fl;
    const KKSFilter * f = c->createFilter(1, QString::number (wObjE->id()), KKSFilter::foEq);
    if(!f){
        qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
        return ERROR_CODE;
    }
    fl.append(f);
    f->release();

    KKSFilterGroup * fg = new KKSFilterGroup(true);
    fg->setFilters(fl);
    filters.append(fg);
    fg->release();
    KKSMap<qint64, KKSEIOData *> recs = loader->loadEIOList(wObjE->io(), filters);
    KKSMap<qint64, KKSEIOData *>::const_iterator p = recs.constFind (wObjE->id());
    if (p == recs.constEnd())
        return ERROR_CODE;
    KKSEIOData * d = p.value ();
    if (!recModel->setData (rIndex, QVariant::fromValue<KKSEIOData>(*d), Qt::UserRole+1))
        return ERROR_CODE;

    return wObjE->id ();
}
/*
 * слот сохранения ИО в БД в ответ на команду или распоряжение, параметры
 * editor -- виджет редактора ЭИО
 * wObj -- сохраняемый ИО,
 * pObjectEx -- соответствующий ЭИО справочника ИО
 * num -- количество копий сохраняемых ЭИО (по умолчанию 1).
 * recModel -- модель, в которую производится запись в случае успешного завершения
 */
void KKSObjEditorFactory :: saveObjectAsCommandResult (KKSObjEditor* editor, 
                                                       KKSObject* wObj, 
                                                       KKSObjectExemplar* pObjectEx, 
                                                       int num,
                                                       QAbstractItemModel * recModel)
{
    if(!editor)
        return;
    
    //editor->setObjChanged(true);
    
    saveObject(editor, wObj, pObjectEx, num, recModel);
    if(editor->isObjChanged())//Значит на предыдущем шаге произошла ошибка
        return; 

    int idJournal = editor->idJournal();
    if(idJournal <= 0)
        return;

    if(wObj->id() <= 0){
        qWarning("Update IO as Command Result: ID < 0!");
        return;
    }

    int ok = updateInControlJournal(idJournal, wObj->id());
    if (ok != OK_CODE)
    {
        qCritical() << tr("An error was orrured while updating data in control journal!");
        QMessageBox::critical(editor,
                              tr("Error"),
                              tr("An error was orrured while updating data in control journal!"),
                              QMessageBox::Ok);
    }
}

/*
 * слот сохранения ИО в БД, параметры
 * editor -- виджет редактора ЭИО
 * wObj -- сохраняемый ИО,
 * pObjectEx -- соответствующий ЭИО справочника ИО
 * num -- количество копий сохраняемых ЭИО (по умолчанию 1)
 * recModel -- модель, в которую производится запись для отображения в таблице
 *  в случае успешного завершения.
 */
void KKSObjEditorFactory :: saveObject (KKSObjEditor* editor, 
                                        KKSObject* wObj, 
                                        KKSObjectExemplar* pObjectEx, 
                                        int num,
                                        QAbstractItemModel * recModel
)
{
    Q_UNUSED (recModel);
    if (!editor->isObjChanged())
    {
        //
        // Все изменения уже сохранены
        //
        return;
    }

    bool transactionOk = true;

    int nc = (pObjectEx->id() < 0 ? num : 1);
    if (pObjectEx->id() >=0 && num > 1)
    {
        qWarning() << tr ("Save IO copy. Only existing copy will be updated");
        QMessageBox::warning (editor, tr ("Warning"), tr ("Save IO copy. Only existing copy will be updated"), QMessageBox::Ok);
    }
    qDebug () << __PRETTY_FUNCTION__ << num << nc;
    bool isNew (pObjectEx->id() < 0);

    editor->clearAdditionalCopies ();
    KKSList<KKSObjectExemplar *> recList;
    for (int i=0; i<nc; i++)
    {
        int res;

        bool ok = true;

        KKSObjectExemplar * wObjCopy = 0;
        KKSObject * io = 0;

        ok = KKSConverter::objectFromExemplar (loader, wObj, pObjectEx);
        KKSAttrValue * avRec = pObjectEx->attrValue (ATTR_RECORD_FILL_COLOR);
        if (avRec)
            qDebug () << __PRETTY_FUNCTION__ << avRec->value().valueVariant() << avRec->value().valueForInsert() << avRec->value().valueVariant().value<QColor>();
        qDebug () << __PRETTY_FUNCTION__ << wObj->recordFillColor();
        if (i > 0)
        {
            wObjCopy = new KKSObjectExemplar (*pObjectEx);
            io = new KKSObject (*wObj);
            ok = KKSConverter::objectFromExemplar (loader, io, wObjCopy);
            if (isNew)
            {
                io->setId (-1);
                wObjCopy->setId (-1);
            }
        }

        if (!ok)
        {
            qCritical() << tr("An error was occured while inserting or updating IO in database!");
            QMessageBox::critical(editor, 
                                  tr("Error"), 
                                  tr("An error was occured while inserting or updating IO in database!"), 
                                  QMessageBox::Ok);
            
            transactionOk = false;
            break;
        }

        if ((io && io->id () <= 0 && i>0) || (wObj->id () <=0 && i == 0))
        {
            int idJournal = editor->idJournal();
            bool draft = editor->asDraft();
            //в журнал можно записать только одну копию!
            if(idJournal > 0 && nc > 1){
                qCritical() <<     tr("In control journal you can insert ONLY one copy of IO!");
                QMessageBox::critical(editor,
                                      tr("Error"),
                                      tr("In control journal you can insert ONLY one copy of IO!"),
                                      QMessageBox::Ok);
                transactionOk = false;
                break;
            }
            
            //создание нового ИО
            //операция INSERT всегда разрешена
            res = (i == 0 ? ppf->insertIO (wObj, true, editor) : ppf->insertIO (io, true, editor));
            
            if (wObj->id () > 0 && i==0){
                pObjectEx->setId (wObj->id());
                KKSValue v(wObj->tableName(), KKSAttrType::atString);
                pObjectEx->attrValue(ATTR_TABLE_NAME)->setValue(v);
                //v.setValue(wObj->code(), KKSAttrType::atString);
                //pObjectEx->attrValue("code")->setValue(v);
            }
            if (res == OK_CODE)
            {
                editor->addAdditionalCopy (i==0 ? pObjectEx : wObjCopy);
                recList.append (i==0 ? pObjectEx : wObjCopy);
            }
            else
            {
                qCritical() <<  tr ("Cannot insert IO to DB");
                QMessageBox::critical(editor, tr ("Insert document"), tr ("Cannot insert IO to DB"), QMessageBox::Ok, QMessageBox::Ok);
                return;
            }

            if (idJournal > 0 && ((io && io->id() > 0) || (wObj && wObj->id() > 0)))
            {
                int ok = insertInControlJournal(idJournal, io ? io->id() : wObj->id(), draft);
                if (ok != OK_CODE)
                {
                    qCritical() << tr("An error was orrured while inserting data in control journal!");
                    QMessageBox::critical(editor,
                                          tr("Error"),
                                          tr("An error was orrured while inserting data in control journal!"),
                                          QMessageBox::Ok);
                    
                    transactionOk = false;
                    break;
                }
            }
        }
        else //изменение существующего ИО
             //данная операция разрешена только для автора или ПОЛЬЗОВАТЕЛЯ, который на это уполномочен
            res = ppf->updateIO (wObj, editor);

        if (res != OK_CODE)
        {
            if(res == ERROR_CODE){
                qCritical() << tr("An error was occured while inserting or updating IO in database!");
                QMessageBox::critical(editor, 
                                      tr("Error"), 
                                      tr("An error was occured while inserting or updating IO in database!"), 
                                      QMessageBox::Ok);
            }
            else if(res == PRIVILEGE_ERROR){
                qCritical() << tr("You cannot update this IO because of insufficient privileges!");
                QMessageBox::critical(editor, 
                                      tr("Error"), 
                                      tr("You cannot update this IO because of insufficient privileges!"), 
                                      QMessageBox::Ok);
            }
            else if(res == PRIVILEGE_ERROR1){
                qWarning() << tr("User privileges was updated, but IO attributes stay unchanged because of only owner organization can modify the IO");
                QMessageBox::warning( editor, 
                                      tr("Warning"), 
                                      tr("User privileges was updated, but IO attributes stay unchanged because of only owner organization can modify the IO"), 
                                      QMessageBox::Ok);
            }

            transactionOk = false;
            break;
        }

        if (wObjCopy)
            wObjCopy->release ();

        if (io)
           io->release ();
    }
    if (transactionOk)
    {
        editor->setObjChanged (false);
        emit cioSaved (pObjectEx);
    }
    else
        editor->setObjChanged (true);
/*
    if (!recModel || !transactionOk)
        return;
    int ier = 0;
    for (int i=0; i<recList.count() && ier >=0; i++)
        ier = writeRecIntoModel (recModel, recList[i]);
*/
}

void KKSObjEditorFactory :: loadObject (KKSObjEditor * editor, int idObjectE, int idObject)
{
    KKSObject *wObj = loader->loadIO (idObject, true);
    if(!wObj){
        qCritical() << tr("You cannot load the IO becase of insufficient privileges!");
        QMessageBox::critical(editor, 
                              tr("Error"), 
                              tr("You cannot load the IO becase of insufficient privileges!"), 
                              QMessageBox::Ok);
        return;
    }

    KKSObjectExemplar *wObjEx = loader->loadEIO (idObjectE, wObj);
    editor->setObjectEx (wObjEx);
}

void KKSObjEditorFactory :: setParams (KKSPPFactory *_ppf, 
                                       KKSLoader *l, 
                                       KKSFileLoader * fl,
                                       KKSAttributesFactory * awf, 
                                       KKSEIOFactory * _eiof,
                                       KKSStuffFactory * _sf)
{
    ppf = _ppf;
    loader = l;
    fileLoader = fl;
    m_awf = awf;
    eiof = _eiof;
    m_sf = _sf;
}

void KKSObjEditorFactory :: setStuffFactory (KKSStuffFactory * _sf)
{
    m_sf = _sf;
}

/*
 * Слот выбора шаблона отображения атрибутов/таблицы
 * editor -- редактор-отправитель сигнала
 * idObject -- идентификатор ИО справочника
 * cat -- категория атрибутов или таблицы
 * tableName -- название таблицы
 */
void KKSObjEditorFactory::filterEIO(KKSObjEditor * editor, int idObject, const KKSCategory * cat, QString tableName)
{
    if(!editor)
        return;

    KKSObject * o = loader->loadIO(idObject, true);
    if(!o)
        return;

    KKSCategory * c = 0;
    if (cat)
        c = new KKSCategory (*cat);
    else
        c = new KKSCategory (*(o->category()->tableCategory()));
    if(!c){
        o->release();
        return;
    }

    bool forIO = false;
    KKSMap<int, KKSAttribute*> attrs;
    if(idObject == IO_IO_ID){
        forIO = true;
        attrs = loader->loadIOUsedAttrs();

    }
/*
    KKSFiltersEditorForm * f = new KKSFiltersEditorForm(c, o->tableName(), attrs, forIO, editor);
*/
    QString tName = o->tableName();
    KKSSearchTemplate * st = new KKSSearchTemplate;
    KKSSearchTemplateForm * filterForm = GUISearchTemplateInit (st, c, true, tName, true, editor);
    connect (filterForm, SIGNAL (saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory *)), this, SLOT (saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory *)) );
    //connect (filterForm, SIGNAL (loadAttributeRefValues (const QString &, const KKSAttribute *, QComboBox *)), this, SLOT (loadAttributeFilters (const QString &, const KKSAttribute *, QComboBox *)) );
    connect (filterForm, SIGNAL (loadAttributeRefValues (const QString &, const KKSAttribute *, QAbstractItemModel *)), this, SLOT (loadAttributeFilters (const QString &, const KKSAttribute *, QAbstractItemModel *)) );
//    connect (f, SIGNAL (loadAttributeRefValues (const QString &, const KKSAttribute *, QComboBox *)), this, SLOT (loadAttributeFilters (const QString &, const KKSAttribute *, QComboBox *)) );
//    connect (f, SIGNAL (loadAttributeRefValues (const QString &, const KKSAttribute *, QAbstractItemModel *)), this, SLOT (loadAttributeFilters (const QString &, const KKSAttribute *, QAbstractItemModel *)) );
//    connect (f, SIGNAL (saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory *)), this, SLOT (saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory *)) );
    connect (filterForm, SIGNAL (loadSearchCriteria (KKSSearchTemplate *, const KKSCategory*, QString)), this, SLOT ( GUIloadDBSearchTemplate(KKSSearchTemplate *, const KKSCategory*, QString)) );
    if(filterForm->exec() == QDialog::Accepted){
        const KKSTemplate * t = new KKSTemplate (c->defTemplate());
        KKSList<const KKSFilterGroup *> filters;
        filters = filterForm->filters();
        
        //!!TODO
        //необходимо добавить поиск в найденном. Т.е. добавлять уже примененные фильтры
        
        //если используем фильтры для справочника ИО, то из него надо исключить системные справочники и журналы
        //они доступны только администратору или из специальных точек доступа

        if(forIO && loader->getUserId() != ADMIN_ROLE)
        {
            KKSList<const KKSFilter *> fl;
            QString value = "select id from io_categories where id_io_category_type <> 2";//исключаем журналы
            const KKSFilter * f = c->createFilter(ATTR_ID_IO_CATEGORY, value, KKSFilter::foInSQL);
            if(!f){
                qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
                return;
            }
            
            fl.append(f);
            f->release();

            value = "select id from io_categories where id_io_category_type <> 8";//исключаем системные справочники
            f = c->createFilter(ATTR_ID_IO_CATEGORY, value, KKSFilter::foInSQL);
            if(!f){
                qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
                return;
            }
            
            fl.append(f);
            f->release();

            value = "select id from io_categories where id_io_category_type <> 9";//исключаем системный справочник категорий
            f = c->createFilter(ATTR_ID_IO_CATEGORY, value, KKSFilter::foInSQL);
            if(!f){
                qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
                return;
            }
            fl.append(f);
            f->release();

            KKSFilterGroup * fg = new KKSFilterGroup(true);
            fg->setFilters(fl);
            filters.append(fg);
            fg->release();
        }
        else
        {
            
            //for (int i=0; i<editor->filters().count(); i++)
            //{
            //    KKSFilterGroup * fg = new KKSFilterGroup (*editor->filters().at(i));
            //    filters.append (fg);// += editor->filters();
            //    fg->release ();
            //}
        }

        QTreeView * tv=0;
        int i = editor->tabEnclosures ? editor->tabEnclosures->currentIndex () : 0;
        if (i==0)
            tv = editor->getRecordsWidget()->getView();
        else if (i <= editor->addRecWidgets.count())
            tv = editor->addRecWidgets[i-1]->getView();
        KKSViewFactory::loadEIOEx (editor,
                                   o, 
                                   loader, 
                                   t, 
                                   tv /*View()*/, 
                                   filters,
                                   true,
                                   c,
                                   tableName);
        editor->setFilters (filters);
        t->release();
        
    }
    filterForm->setParent (0);
    delete filterForm;

    c->release ();
    o->release();
}

/* слот обновления списка записей в таблице (основной или дополнительной).
 * editor -- редактор-отправитель сигнала ИО (ЭИО)
 * idObject -- идентификатор ИО-справочника
 * cat -- категория соответствующей таблицы
 * tableName -- название таблицы
 * sourceMod -- обновляемая модель
 */
void KKSObjEditorFactory :: refreshEIO (KKSObjEditor * editor, int idObject, const KKSCategory * cat, QString tableName, QAbstractItemModel * sourceMod)
{
    if(!editor || !sourceMod)
        return;

    KKSObject * o = loader->loadIO(idObject, true);
    if(!o)
        return;
    
    KKSCategory * c = 0;
    if (cat)
        c = new KKSCategory (*cat);
    else
        c = new KKSCategory (*(o->category()->tableCategory()));
    if(!c){
        o->release();
        return;
    }
    
    KKSList<const KKSFilterGroup *> filters = editor->filters();
    KKSMap<qint64, KKSEIOData *> recList = loader->loadEIOList(c, tableName, filters, o->id() <= _MAX_SYS_IO_ID_ ? true : false);
    
    QMap<QString, QVariant> val;
    for (KKSMap<qint64, KKSEIOData *>::const_iterator p=recList.constBegin();
            p != recList.constEnd();
            p++)
    {
        QString idStr = QString::number (p.key());
        QVariant v = QVariant::fromValue (*p.value());
        val.insert (idStr, v);
    }
    
    const KKSTemplate * t = sourceMod->data(sourceMod->index(0,0), Qt::UserRole+2).value<const KKSTemplate *>();
    if (!t)
    {
        c->release ();
        o->release ();
        return;
    }
    t->addRef();
    //sourceMod->setData (QModelIndex(), val, Qt::UserRole+1);
    recList.clear ();
    
    QTreeView * tv=0;
    int i = editor->tabEnclosures ? editor->tabEnclosures->currentIndex () : 0;
    if (i==0)
        tv = editor->getRecordsWidget()->getView();
    else if (i <= editor->addRecWidgets.count())
        tv = editor->addRecWidgets[i-1]->getView();
    KKSViewFactory::loadEIOEx (editor,
                                o, 
                                loader, 
                                t, 
                                tv /*View()*/, 
                                filters,
                                true,
                                c,
                                tableName);
    t->release ();
    c->release ();
    o->release ();
}

/* слот поиска ЭИО в таблице (основной или дополнительной).
 * editor -- редактор-отправитель сигнала ИО (ЭИО)
 * idObject -- идентификатор ИО-справочника
 * cat -- категория соответствующей таблицы
 * tableName -- название таблицы
 */
void KKSObjEditorFactory :: filterTemplateEIO (KKSObjEditor * editor, 
                                               int idObject, 
                                               const KKSCategory * cat, 
                                               QString tableName)
{
    KKSObject * o = loader->loadIO(idObject, true);
    if(!o)
        return;

    KKSCategory * c = 0;
    if (cat)
        c = new KKSCategory (*cat);
    else
        c = new KKSCategory (*(o->category()->tableCategory()));
    if(!c){
        o->release();
        return;
    }

    bool forIO = false;
    KKSMap<int, KKSAttribute*> attrs;
    if(idObject == IO_IO_ID){
        forIO = true;
        attrs = loader->loadIOUsedAttrs();

    }
    KKSSearchTemplate * searchT = 0;
    KKSList<KKSSearchTemplate *> stList = loader->loadSearchTemplates ();

    KKSSearchTemplatesForm *stForm = new KKSSearchTemplatesForm (c, tableName, true, editor);
    initSearchTemplateModel (stForm);
    QItemSelectionModel * selTModel = stForm->selectionModel ();
    QAbstractProxyModel * sortTModel = qobject_cast<QAbstractProxyModel *>(stForm->dataModel());

    if (selTModel && stForm->exec () == QDialog::Accepted)
    {
        if (selTModel->selection().indexes ().isEmpty())
            return;
        QModelIndex stProxyIndex = selTModel->selection().indexes ().at (0);
        QModelIndex stIndex = sortTModel->mapToSource (stProxyIndex);
        if (stIndex.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
            return;
        int idSearchTemplate = stIndex.data (Qt::UserRole).toInt();
        searchT = loader->loadSearchTemplate (idSearchTemplate);
        if (!searchT)
        {
            stForm->setParent (0);
            delete stForm;
            c->release ();
            o->release ();
        }
        bool isApp = loader->isApplicable(searchT,c->id());
        if (!isApp)
        {
            QMessageBox::warning (stForm, tr("Search by templates"), tr("Search template %1 cannot been applied onto this reference").arg(searchT->name()), QMessageBox::Ok);
            stForm->setParent (0);
            delete stForm;
            c->release ();
            o->release ();
            return;
        }
        const KKSTemplate * t = new KKSTemplate (c->defTemplate());
        if (stForm->getSearchInRes() == Qt::Unchecked)
            editor->clearFilters();
        KKSList< const KKSFilterGroup*> filters = editor->filters();
        KKSFilterGroup * stGroup = searchT->getMainGroup ();
        filters.append (stGroup);
        if(forIO && loader->getUserId() != ADMIN_ROLE){
            filterSysIO (c, filters);
        }

        QTreeView * tv=0;
        int i = editor->tabEnclosures ? editor->tabEnclosures->currentIndex () : 0;
        if (i==0)
            tv = editor->getRecordsWidget()->getView();
        else if (i <= editor->addRecWidgets.count())
            tv = editor->addRecWidgets[i-1]->getView();
        KKSViewFactory::loadEIOEx (editor,
                                   o,
                                   loader,
                                   t,
                                   tv /*View()*/,
                                   filters,
                                   false,
                                   c,
                                   tableName);
        editor->setFilters (filters);
        t->release();
        //filters.append (*stGroup);
    }

    stForm->setParent (0);
    delete stForm;
    c->release ();
    o->release();
    return;
}

/*
 * слот создания редактора нового ЭИО, являющегося элементом справочника idObject.
 * Параметры:
 * editor -- виджет-родитель, отвечающий за просмотр-редактирование ИО-справочника
 * idObject -- идентификатор ИО-справочника,
 * с -- категория таблицы ИО-справочника,
 * tableName -- название таблицы
 * nTab -- номер вкладки в редакторе, где произошел вызов
 * isModal -- флаг модальности
 * recModel -- модель записей
 * flags -- сопутствующие флаги
 */

KKSObjEditor* KKSObjEditorFactory :: createNewEditor (QWidget * editor, int idObject, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * recModel, Qt::WindowFlags flags)
{
    KKSObjEditor * oEditor = qobject_cast <KKSObjEditor *>(editor);
    if (qobject_cast <KKSObjEditor *>(editor))
        qobject_cast <KKSObjEditor *>(editor)->clearW ();
/*    KKSObject *wObj = loader->loadIO (idObject);
    const KKSCategory *c = t ? t->category() : 0;
    if (!c && wObj->category ())
        c = wObj->category()->tableCategory ();
*/
    KKSObjEditor * newObjEditor = this->createObjEditor (idObject, 
                                                         -1, 
                                                         //editor->m_filters,
                                                         KKSList<const KKSFilterGroup*>(), 
                                                         "",
                                                         c,
                                                         isModal,
                                                         tableName,
                                                         false,
                                                         oEditor ? oEditor->windowModality () : Qt::NonModal,
                                                         0,
                                                         flags);

    if (!newObjEditor)
    {
        qCritical() << tr("Cannot create new object editor! Corrupt data!");
        QMessageBox::critical(editor, 
                              tr("Error"), 
                              tr("Cannot create new object editor! Corrupt data!"), 
                              QMessageBox::Ok);
        return 0;
    }
//    connect(newObjEditor, 
//            SIGNAL(eioChanged(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )), 
//            editor, 
//            SLOT(updateEIOEx(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )));
    newObjEditor->setRecordsModel (recModel);

    //cSelection = oEditor ? oEditor->recWidget->tv->selectionModel()->selection() : QItemSelection();

    qDebug () << __PRETTY_FUNCTION__ << tableName;
    newObjEditor->setCurrentTable (tableName);
    newObjEditor->setParentTab (nTab);
    
    if (qobject_cast <KKSObjEditor *>(editor))
        connect(newObjEditor, 
                SIGNAL(eioChanged(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )), 
                editor, 
                SLOT(updateEIOEx(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )));
    
    newObjEditor->setAttribute (Qt::WA_DeleteOnClose);
    //newObjEditor->showNormal ();
//    newObjEditor->setObjChanged (false);
    qDebug () << __PRETTY_FUNCTION__ << newObjEditor->isObjChanged();
    emit editorCreated(newObjEditor);
    return newObjEditor;
}

/*
 * слот создания редактора нового ИО/ЭИО с заданными параметрами, являющегося элементом справочника idObject.
 * Параметры:
 * editor -- виджет-родитель, отвечающий за просмотр-редактирование ИО-справочника
 * idObject -- идентификатор ИО-справочника,
 * с -- категория таблицы ИО-справочника,
 * tableName -- название таблицы
 * nTab -- номер вкладки в редакторе, где произошел вызов
 * isModal -- флаг модальности
 * ioAvals -- параметры создаваемого ИО
 * aVals  -- параметры создаваемого ЭИО
 * recModel -- модель для записи
 * flags -- сопутствующие флаги
 */
KKSObjEditor* KKSObjEditorFactory :: createNewEditorParam (QWidget * editor, 
                                                           int idObject, 
                                                           const KKSCategory * c, 
                                                           QString tableName, 
                                                           int nTab, 
                                                           bool isModal, 
                                                           const KKSMap<qint64, KKSAttrValue *>& ioAvals, 
                                                           const KKSMap<qint64, KKSAttrValue *>& aVals, 
                                                           QAbstractItemModel * recModel,
                                                           Qt::WindowFlags flags)
{
    if (ioAvals.isEmpty () && aVals.isEmpty ())
    {
        return this->createNewEditor (editor, idObject, c, tableName, nTab, isModal, recModel);
    }
    KKSObjEditor * oEditor = qobject_cast <KKSObjEditor *>(editor);
    if (qobject_cast <KKSObjEditor *>(editor))
        qobject_cast <KKSObjEditor *>(editor)->clearW ();
    bool isAvalsEmpty (aVals.isEmpty ());
    qDebug () << __PRETTY_FUNCTION__ << isAvalsEmpty;
/*    KKSObject *wObj = loader->loadIO (idObject);
    const KKSCategory *c = t ? t->category() : 0;
    if (!c && wObj->category ())
        c = wObj->category()->tableCategory ();
*/
    KKSObjEditor * newObjEditor = this->createObjEditorParam (idObject,
                                                              -1,
                                                              KKSList<const KKSFilterGroup*>(),
                                                              "",
                                                              c,
                                                              ioAvals,
                                                              aVals,
                                                              isModal,
                                                              tableName,
                                                              false,
                                                              oEditor ? oEditor->windowModality () : Qt::NonModal,
                                                              0,
                                                              flags);

    if (!newObjEditor)
    {
        qCritical() << tr("Cannot create new object editor! Corrupt data!");
        QMessageBox::critical(editor,
                              tr("Error"),
                              tr("Cannot create new object editor! Corrupt data!"),
                              QMessageBox::Ok);
        return 0;
    }

    if (qobject_cast <KKSObjEditor *>(editor))
        connect(newObjEditor, 
                SIGNAL(eioChanged(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )), 
                editor, 
                SLOT(updateEIOEx(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )));
    
    newObjEditor->setAttribute (Qt::WA_DeleteOnClose);
    newObjEditor->setRecordsModel (recModel);
    //newObjEditor->showNormal ();
//    newObjEditor->setObjChanged (false);
    qDebug () << __PRETTY_FUNCTION__ << newObjEditor->isObjChanged();
    emit editorCreated(newObjEditor);
    return newObjEditor;
}

/*
 * слот создания редактора существующего ИО/ЭИО, являющегося элементом справочника idObject.
 * Параметры:
 * editor -- виджет-родитель, отвечающий за просмотр-редактирование ИО-справочника
 * idObject -- идентификатор ИО-справочника,
 * idObjEx -- идентификатор экземпляра ИО.
 * с0 -- категория таблицы ИО-справочника,
 * tableName -- название таблицы
 * nTab -- номер вкладки в редакторе, где произошел вызов
 * isModal -- флаг модальности
 */
void KKSObjEditorFactory :: editExistOE (QWidget * editor, int idObject, qint64 idObjEx, const KKSCategory * c0, QString tableName, int nTab, bool isModal, QAbstractItemModel * recModel, const QModelIndex& recIndex)
{
    //Q_UNUSED (recIndex);
    const KKSCategory *c = 0;//t ? t->category() : 0;
    KKSObject * io = NULL;

    if(idObject == IO_IO_ID)
    {
        io = loader->loadIO(idObjEx, false);
        if(!io)
            return;
        c = io->category();//->tableCategory();
    }
    else
    {
        io = loader->loadIO (idObject, true);
        if (!io)
            return;

        c = c0 ? c0 : io->category()->tableCategory();
    }

    if (!c){
        if(io)
            io->release();
        return;
    }

    c->addRef ();
    if (qobject_cast <KKSObjEditor *>(editor))
        qobject_cast <KKSObjEditor *>(editor)->clearW ();
    KKSObjEditor * newObjEditor = this->createObjEditor (idObject, 
                                                         idObjEx, 
                                                         KKSList<const KKSFilterGroup*>(), 
                                                         "",
                                                         c,
                                                         isModal,
                                                         tableName,
                                                         false,
                                                         editor ? editor->windowModality () : Qt::NonModal,
                                                         editor ? (editor->windowModality() == Qt::NonModal ? NULL : editor) : NULL,
                                                         editor->windowFlags());
    c->release ();
    io->release();

    if(!newObjEditor)
    {
        qCritical() << tr("Cannot create new object editor! Corrupt data!");
        QMessageBox::critical(editor, 
                              tr("Error"), 
                              tr("Cannot create new object editor! Corrupt data!"), 
                              QMessageBox::Ok);
        return;
    }

    //cSelection = qobject_cast <KKSObjEditor *>(editor) && qobject_cast <KKSObjEditor *>(editor)->recWidget ? qobject_cast <KKSObjEditor *>(editor)->recWidget->tv->selectionModel()->selection() : QItemSelection();
    //qDebug () << __PRETTY_FUNCTION__ << cSelection << nTab;

    connect (newObjEditor, 
             SIGNAL(eioChanged(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )), 
             editor, 
             SLOT(updateEIOEx(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )));

    newObjEditor->setRecordsModel (recModel);
    newObjEditor->setCurrentTable (tableName);
    newObjEditor->setParentTab (nTab);
    newObjEditor->setAttribute (Qt::WA_DeleteOnClose);
    newObjEditor->setRecordsIndex(recIndex);
    //qDebug () << __PRETTY_FUNCTION__ << newObjEditor->isObjChanged() << nTab;
    newObjEditor->setObjChanged (false);
    if(editor && editor->windowModality() != Qt::NonModal){
        newObjEditor->exec();
    }
    else
        emit editorCreated(newObjEditor);
}

/* Метод удаляет ЭИО из соответствующего ИО-справочника.
 * Параметры:
 * editor -- родительский редактор.
 * idObject -- идентификатор ИО-справочника
 * idObjEx -- идентификатор удаляемого ЭИО
 * tableName -- название таблицы
 * drow -- номер удаляемой строки (для сохранения выделения)
 */
int KKSObjEditorFactory :: deleteOE (QWidget * editor, int idObject, qint64 idObjEx, QString tableName, QAbstractItemModel * recModel, const QModelIndex& pRecIndex)
{
    if (!editor)
        return ERROR_CODE;

    if (qobject_cast <KKSObjEditor *>(editor))
        qobject_cast <KKSObjEditor *>(editor)->clearW ();
    
    KKSObject *wObj = loader->loadIO (idObject, false);//ибо потом используются шаблоны ИО
    if(!wObj)
        return ERROR_CODE;

    KKSObjectExemplar *wObjEx = loader->loadEIO (idObjEx, wObj);
    if(!wObjEx)
        return ERROR_CODE;

    int resCode;
    //cSelection = qobject_cast <KKSObjEditor *>(editor) && qobject_cast <KKSObjEditor *>(editor)->recWidget ? qobject_cast <KKSObjEditor *>(editor)->recWidget->tv->selectionModel()->selection() : QItemSelection();
    //int row = cSelection.isEmpty() ? -1 : cSelection.indexes().at(0).row();

    if (idObject == IO_IO_ID)
    {
        KKSObject *wObjCh = loader->loadIO (idObjEx, true);
        if(wObjCh){
            resCode = ppf->deleteIO (wObjCh);
            wObjCh->release ();
        }
        else{
            resCode = PRIVILEGE_ERROR; //недостаточно прав доступа
        }
    }
    else
        resCode = eiof->deleteEIO (wObjEx, tableName);

    if (resCode != OK_CODE)
    {
        if(resCode == ERROR_CODE){    
            qCritical() << tr("An error was occured while deleting record!");
            QMessageBox::critical(editor, 
                                  tr("Error"), 
                                  tr("An error was occured while deleting record!"), 
                                  QMessageBox::Ok );
        }
        else if(resCode == PRIVILEGE_ERROR){
            qCritical() << tr("You cannot delete this IO because of insufficient privileges!");
            QMessageBox::critical(editor, 
                                  tr("Error"), 
                                  tr("You cannot delete this IO because of insufficient privileges!"), 
                                  QMessageBox::Ok);
        }

        wObjEx->release();
        return resCode;
    }
    wObjEx->release();

    if (recModel && pRecIndex.isValid())
    {
        QModelIndex pIndex = pRecIndex.parent();
        int ir = pRecIndex.row();
        recModel->removeRows (ir, 1, pIndex);
    }
    wObj->release();
    return OK_CODE;
}

/* Слот обновляет вид таблицы ИО-справочника.
 * Параметры:
 * wObj -- ИО-справочник
 * idObjEx -- список экземпляров
 * recModel -- обновляемая модель
 * cSelection -- выделенные индексы
 */
void KKSObjEditorFactory :: updateEIOView (KKSObject * wObj, const KKSMap<qint64, KKSObjectExemplar *>& idObjEx, QAbstractItemModel * recModel, const QItemSelection& cSelection, const QModelIndex& recIndex)
{
    Q_UNUSED (cSelection);
    KKSObjEditor * editor = qobject_cast<KKSObjEditor *>(this->sender());
    if (!editor || !recModel)
        return;

    //editor->clearW ();
    if(!wObj)
        return;

    qDebug () << __PRETTY_FUNCTION__ <<  wObj->id() << idObjEx.keys();
    if (wObj->category() && wObj->category()->tableCategory())
    {
        QModelIndex cInd = QModelIndex();//KKSViewFactory::searchModelRowsIndex(recModel,);
        const KKSTemplate * t = recModel->data(cInd, Qt::UserRole+2).value<const KKSTemplate *>();
        if (!t)
        {
            wObj->release ();
            return;
        }
        else
            t->addRef ();

        KKSViewFactory::updateEIOEx (loader, wObj, idObjEx, t, recModel);

        t->release ();

    }
    if (recIndex.isValid())
    {
        KKSEIOData * d = loader->loadEIOInfo (wObj->id(), recIndex.data(Qt::UserRole).toInt());
        recModel->setData (recIndex, QVariant::fromValue<KKSEIOData*>(d), Qt::UserRole+1);
        QIcon tIcon;
        if (d)
        {
            QString strIcon = d->sysFieldValue("r_icon");
            if (!strIcon.isEmpty())
            {
                QPixmap pIcon;
                pIcon.loadFromData (strIcon.toUtf8());
                tIcon = QIcon (pIcon);
            }
            else
                tIcon = QIcon(":/ddoc/rubric_item.png");
            const KKSCategoryAttr * cAttrB = recModel->data(QModelIndex(), Qt::UserRole+4).value<const KKSCategoryAttr*>();
            const KKSCategoryAttr * cAttrF = recModel->data(QModelIndex(), Qt::UserRole+5).value<const KKSCategoryAttr*>();
            if (cAttrB)
            {
                bool ok;
                quint64 vl = d->fields().value (cAttrB->code(false)).toLongLong (&ok);
                recModel->setData (recIndex, vl, Qt::BackgroundRole);
            }
            if (cAttrF)
            {
                bool ok;
                quint64 vl = d->fields().value (cAttrF->code(false)).toLongLong (&ok);
                recModel->setData (recIndex, vl, Qt::ForegroundRole);
            }
        }
        recModel->setData (recIndex.sibling(recIndex.row(), 0), tIcon, Qt::DecorationRole);
        d->release ();
    }

}

/* Метод производит первоначальную установку атрибутов и их значений в соответствии с шаблоном.
 * Параметры :
 * t -- шаблон просмотра ЭИО
 * obj -- ИО, содержащий редактируемый/создаваемый ЭИО
 * attrWidget -- родительский виджет
 * gAttrLayout -- соответствующий лейаут
 * c -- категория ЭИО
 * wObjE -- создаваемый/редактируемый ЭИО
 * tableName -- название целевой таблицы
 * editor -- редактор ЭИО.
 */
int KKSObjEditorFactory :: setAttributes (const KKSTemplate *t, 
                                          KKSObject *obj,
                                          QWidget *attrWidget,
                                          QGridLayout *gAttrLayout,
                                          const KKSCategory *c,
                                          KKSObjectExemplar *wObjE,
                                          const QString& tableName,
                                          KKSObjEditor *editor
                                          )
{
    if (!t || !obj || !wObjE || (!c && !t->category()) || !attrWidget || !gAttrLayout || !editor)
        return 0;

    KKSList<KKSAttrValue*> avs = wObjE->attrValues();
    //int cnt = avs.count();
    //for(int i=0; i<cnt; i++)
    
    //Системные атрибуты, а также атрибуты записей справочников не поддерживают темпоральность
    //поэтому можно однозначно по ИД атрибута идентифицировать его значение

    for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = t->category()->attributes().constBegin(); pa != t->category()->attributes().constEnd(); pa++)
    {
        
        KKSCategoryAttr * ca = pa.value();
        KKSAttrValue *av1 = wObjE->attrValue (ca->id());
        KKSAttrValue *av = NULL;
        if(av1)
            av = new KKSAttrValue(*av1);
        else if (t->attributes().contains(pa.key())
                && t->attributes().value(pa.key()))
        {
            KKSValue v = t->attributes().value(pa.key())->defValue().isNull() ? ca->defValue() : t->attributes().value(pa.key())->defValue();
            qDebug () << __PRETTY_FUNCTION__ << pa.key() << v.value();
            av = new KKSAttrValue (v, ca);
        }
        else
            av = new KKSAttrValue(ca->defValue(), ca);

        KKSValue v;
        if (av1)
            v = av1->value();
        else if (t->attributes().value(pa.key(), 0))
            v = t->attributes().value(pa.key())->defValue().isNull() ? ca->defValue() : t->attributes().value(pa.key())->defValue();
        else if (!ca->defValue().isNull())
            v = ca->defValue();
        else
            v = KKSValue();
        
        if (wObjE && wObjE->id()<0 && c && ((av && av->attribute()->tableName () == "io_categories") || ca->id () == ATTR_ID_IO_CATEGORY))
        {
            v = KKSValue(QString::number (c->id()), KKSAttrType::atList);
        }
        else if (wObjE && wObjE->id()<0 && wObjE->io()->id() == IO_IO_ID && ca->id () == ATTR_AUTHOR && wObjE->id() <= 0)
        {
            v = KKSValue(QString::number (loader->getUserId()), KKSAttrType::atList);
        }

        av->setValue(v);

        editor->setSysAttrValue(av);

        av->release();

        
        /*QVariant val;
        KKSAttrValue *av = wObjE->attrValue (pa.value()->id());//pa.key());
        val = av ? av->value().valueVariant () : QVariant();
        if (c && ((av && av->attribute()->tableName () == "io_categories") || pa.value()->id () == ATTR_ID_IO_CATEGORY))
            val = QString::number (c->id());
        else if (wObjE->io()->id() == IO_IO_ID && pa.value()->id () == ATTR_AUTHOR && wObjE->id() <= 0)
            val = QString::number (loader->getUserId());
        //qDebug () << __PRETTY_FUNCTION__ << pa.value()->code() << val << val.isNull();
        
        editor->setSysAttribute(pa.value(), val);//pa.value()->defValue().valueVariant());
        */

    }


    int nc = 0;
    KKSMap<int, KKSAttrGroup *> attrGroups = t->groups();
    int nattrg = attrGroups.count();
    bool isWid = false;
    Q_UNUSED (isWid);
    if (nattrg == 0)
        return 0;

    KKSMap<int, KKSAttrGroup *>::const_iterator pg = attrGroups.constBegin();
    bool isGrouped (nattrg>=2 ||
                   (nattrg == 1 && pg.value() && !pg.value()->childGroups().isEmpty()));
    
    for (; pg != attrGroups.constEnd(); pg++)
    {
        KKSAttrGroup * aGroup = pg.value();//attrGroups[i];
        if (!aGroup)
            continue;
        KKSMap<int, KKSAttrView*> attrs = aGroup->attrViews();
        KKSList<KKSAttrView *> attrs_list;
        for (KKSMap<int, KKSAttrView*>::const_iterator pa = attrs.constBegin(); pa != attrs.constEnd(); pa++)
            attrs_list.append (pa.value());
        qSort (attrs_list.begin(), attrs_list.end(), compareAttrViews);

        //
        // проверка на то, что группа может содержать только атрибуты с типом KKSAttrType::atCheckListEx
        //
        bool isAtCheckListEx = !aGroup->childGroups().isEmpty();//false;
        for (int ii=0; ii<attrs_list.count() && !isAtCheckListEx; ii++)
            isAtCheckListEx = isAtCheckListEx || (attrs_list[ii]->type()->attrType() != KKSAttrType::atCheckListEx);

        if (!isAtCheckListEx && isGrouped)
            continue;

        QGroupBox *gbAttr = 0;
        QGridLayout *gbLay = 0;
        if (isGrouped)
        {
            gbAttr = new QGroupBox (aGroup->name(), attrWidget);
            gAttrLayout->addWidget (gbAttr, nc, 0, 1, 1);
            isWid = true;
            gbLay = new QGridLayout ();
            gbLay->setVerticalSpacing (10);
            //gbLay->setAlignment (Qt::AlignJustify);//Qt::AlignRight | Qt::AlignVCenter);
            gbLay->setContentsMargins (5, 5, 5, 5);
            gbAttr->setLayout (gbLay);
            gbAttr->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
            nc++;
        }

        putAttrsGroupsOnWidget (obj, wObjE, editor, nc, c, tableName, aGroup, gbLay, gAttrLayout, isGrouped);
    }

//    QVBoxLayout *vLay = new QVBoxLayout ();
//    vLay->addStretch (10);
//    gSysLayout->addLayout (vLay, nc, 0, 1, 2);

    return nc;
}

/* Метод производит первоначальную установку пользовательских атрибутов и их значений в соответствии с шаблоном.
 * Параметры :
 * t -- шаблон просмотра ЭИО
 * obj -- редактируемый/создаваемый ИО
 * attrWidget -- родительский виджет
 * gAttrLayout -- соответствующий лейаут
 * c -- категория ИО
 * editor -- редактор ИО
 * updateView -- флаг обновления существующего расклада или первоначальный расклад.
 */

int KKSObjEditorFactory :: setAttributes (const KKSTemplate *t, 
                                          KKSObject *obj,
                                          QWidget *attrWidget,
                                          QGridLayout *gAttrLayout,
                                          const KKSCategory *c,
                                          KKSObjEditor *editor,
                                          bool updateView)
{
    if (!t || !obj || !c || !attrWidget || !gAttrLayout || !editor)
        return 0;

    /*
    KKSList<KKSAttrValue*> avs = obj->attrValues();
    int cnt = avs.count();
    for(int i=0; i<cnt; i++)
    //for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = c->attributes().constBegin(); pa != c->attributes().constEnd(); pa++)
    {
        KKSAttrValue *av1 = avs.at(i);
        KKSAttrValue *av = new KKSAttrValue(*av1);
        KKSValue v = av ? av->value() : KKSValue();
        
        av->setValue(v);

        editor->setSysAttrValue(av);

        av->release();
    }*/

    for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = c->attributes().constBegin(); pa != c->attributes().constEnd(); pa++)
    {
        KKSValue value;
        KKSList<KKSAttrValue*> avs = obj->attrValuesByIdAttr (pa.value()->id());
        int cnt = avs.count();

        if(cnt == 0){
            KKSAttrValue * attrValue;
            attrValue = new KKSAttrValue(pa.value()->defValue(), pa.value());
            editor->setIOAttrValue(attrValue);
        }
        
        for(int i=0; i<cnt; i++){
            KKSAttrValue * av = avs[i];

            value = av ? av->value() : pa.value()->defValue();

            KKSAttrValue * attrValue;
            if(av)
                attrValue = new KKSAttrValue(*av);
            else
                attrValue = new KKSAttrValue(value, pa.value());

            editor->setIOAttrValue(attrValue);
        }
        
    }

    int nc = 0;
    KKSMap<int, KKSAttrGroup *> attrGroups = t->groups();
    int nattrg = attrGroups.count();
    qDebug () <<  __PRETTY_FUNCTION__ << t->groups().keys() << attrGroups.keys() << nattrg;
    bool isWid = false;
    Q_UNUSED (isWid);

    if (nattrg == 0)
        return 0;

    //qDebug () << __PRETTY_FUNCTION__ << nattrg << isIOGrouped << attrGroups.keys() << t->groups().keys() << c->id() << t->id();
    //isIOGrouped = (attrGroups.count()>=2 || (attrGroups.count() == 1 && attrGroups[0] && !attrGroups[0]->childGroups().isEmpty()));
    KKSMap<int, KKSAttrGroup *>::const_iterator pg = attrGroups.constBegin();
    bool isIOGrouped = ( nattrg >=2 || (nattrg == 1 && pg.value() && !pg.value()->childGroups().isEmpty()));
    qDebug () << __PRETTY_FUNCTION__ << attrGroups.count() << isIOGrouped << (attrGroups.count()>=2) << t->id();
    for (; pg != attrGroups.constEnd(); pg++)
    {
        KKSAttrGroup * aGroup = pg.value();//attrGroups[i];
        if (!aGroup)
            continue;
        KKSMap<int, KKSAttrView*> attrs = aGroup->attrViews();
        QList<KKSAttrView *> attrs_list = attrs.values();
        qSort (attrs_list.begin(), attrs_list.end(), compareAttrViews);

        //
        // проверка на то, что группа может содержать только атрибуты с типом KKSAttrType::atCheckListEx
        //
        bool isAtCheckListEx = false;
        for (int ii=0; ii<attrs_list.count() && !isAtCheckListEx; ii++)
            isAtCheckListEx = isAtCheckListEx || (attrs_list[ii]->type()->attrType() != KKSAttrType::atCheckListEx);

        if (!isAtCheckListEx && isIOGrouped)
            continue;

        QGroupBox *gbAttr = 0;
        QGridLayout *gbLay = 0;
        if (isIOGrouped)// && isAtCheckListEx)
        {
            gbAttr = new QGroupBox (aGroup->name(), attrWidget);
            gAttrLayout->addWidget (gbAttr, nc, 0, 1, 1);
            isWid = true;
            gbLay = new QGridLayout ();
            gbLay->setVerticalSpacing (10);
            gbLay->setAlignment (Qt::AlignRight | Qt::AlignVCenter);
            gbLay->setContentsMargins (5, 5, 5, 5);
            gbAttr->setLayout (gbLay);
            gbAttr->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            nc++;
        }
        for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = c->attributes().constBegin(); pa != c->attributes().constEnd(); pa++)
        {
            KKSAttrValue *av = obj->attrValueId (pa.value()->id());//key());
            qDebug () << __PRETTY_FUNCTION__ << av << (av ? av->value().value() : 0) << pa.value()->id() << obj->attrValueId (pa.value()->id());
        }

        putAttrsGroupsOnWidget (obj, editor, nc, c, aGroup, gbLay, gAttrLayout, isIOGrouped, updateView);
    }
//    QVBoxLayout *vLay = new QVBoxLayout ();
//    vLay->addStretch (10);
//    gSysLayout->addLayout (vLay, nc, 0, 1, 2);

    return nc;
}

/* Метод устанавливает индикаторы в лейаут.
 * Параметры :
 * obj -- редактируемый ИО,
 * indWidget -- родительский виджет,
 * gIndLay -- его лейаут
 * editor -- редактор ИО
 * updateView -- не используется
 */
/*
void KKSObjEditorFactory :: setIndicators (KKSObject * obj,
                                           QWidget * indWidget,
                                           QGridLayout * gIndLay,
                                           KKSObjEditor * editor,
                                           bool updateView
                                          )
{
    if (!obj || !indWidget || !gIndLay || !editor)
        return;

    KKSMap<int, KKSIndicatorValue*> indMap;
    for (int i=0; i<obj->indicatorValues().count(); i++)
        //KKSList<int, KKSIndicator*>::const_iterator pa = obj->().constBegin(); pa != c->attributes().constEnd(); pa++)
    {
        QVariant val;
        KKSIndicatorValue *av = obj->indicatorValues().at (i);
        qDebug () << __PRETTY_FUNCTION__ << av << (av ? av->value().value() : 0);
        //<< pa.value()->id() << obj->attrValueId (pa.value()->id());
        val = av ? av->value().valueVariant () : QVariant();//obj->indicatorValues().at(i)->defValue().valueVariant();
        editor->setIndValue (av);//->indicator());
        qDebug () << __PRETTY_FUNCTION__ << val;
        if (av)
            indMap.insert (av->id(), av);
    }
    int nc(1);
    QToolBar * tbIndActions = new QToolBar (indWidget);
    QAction * actSearch = new QAction (QIcon(":/ddoc/search.png"), tr ("Search"), indWidget);
    tbIndActions->addAction (actSearch);
    tbIndActions->addSeparator ();
    QAction * actAdd = new QAction (QIcon(":/ddoc/add.png"), tr ("Add Indicator value"), indWidget);
    connect (actAdd, SIGNAL (triggered()), editor, SLOT (addIndicator()) );
    QAction * actRefresh = new QAction (QIcon(":/ddoc/refreshIcon.png"), tr ("&Refresh"), indWidget);
    tbIndActions->addAction (actAdd);
    tbIndActions->addSeparator ();
    tbIndActions->addAction (actRefresh);
    gIndLay->addWidget (tbIndActions, 0, 0, 1, 4, Qt::AlignLeft | Qt::AlignTop);
    qDebug () << __PRETTY_FUNCTION__ << gIndLay->rowCount();
    QGridLayout * gbLay (0);
    putIndicatorsGroupsOnToWidget (obj, editor, nc, indMap, gbLay, gIndLay, false, updateView);
    QSpacerItem * sp = new QSpacerItem (20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);
    int nr = gIndLay->rowCount();
    qDebug () << __PRETTY_FUNCTION__ << gIndLay->rowCount();
    gIndLay->addItem (sp, nr, 0, 1, 4);//, Qt::AlignLeft | Qt::AlignBottom);
    qDebug () << __PRETTY_FUNCTION__ << gIndLay->rowCount();
}
 */

int KKSObjEditorFactory :: setIndicators (const KKSTemplate *t,
                                          KKSObject * obj,
                                          QWidget * indWidget,
                                          QGridLayout * gIndLay,
                                          const KKSCategory *c,
                                          KKSObjectExemplar *wObjE,
                                          const QString& tableName,
                                          KKSObjEditor * editor,
                                          bool updateView
                                          )
{
    Q_UNUSED(updateView);

    if (!t || !obj || !wObjE || (!c && !t->category()) || !indWidget || !gIndLay || !editor)
        return 0;

    KKSList<KKSAttrValue*> avs = wObjE->indValues();
    //int cnt = avs.count();
    //for(int i=0; i<cnt; i++)
    
    //Системные атрибуты, а также атрибуты записей справочников не поддерживают темпоральность
    //поэтому можно однозначно по ИД атрибута идентифицировать его значение

    for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = t->category()->attributes().constBegin(); pa != t->category()->attributes().constEnd(); pa++)
    {
        
        KKSCategoryAttr * ca = pa.value();
        KKSAttrValue *av1 = wObjE->indValue (ca->id());
        KKSAttrValue *av = NULL;
        if(av1)
            av = new KKSAttrValue(*av1);
        else
            av = new KKSAttrValue(ca->defValue(), ca);

        KKSValue v = av1 ? av1->value() : KKSValue();
        
        if (c && ((av && av->attribute()->tableName () == "io_categories") || ca->id () == ATTR_ID_IO_CATEGORY))
        {
            v = KKSValue(QString::number (c->id()), KKSAttrType::atList);
        }
        else if (wObjE->io()->id() == IO_IO_ID && ca->id () == ATTR_AUTHOR && wObjE->id() <= 0)
        {
            v = KKSValue(QString::number (loader->getUserId()), KKSAttrType::atList);
        }

        av->setValue(v);

        editor->setIndValue(av);

        av->release();
    }
    int nc = 0;
    KKSMap<int, KKSAttrGroup *> attrGroups = t->groups();
    int nattrg = attrGroups.count();
    bool isWid = false;
    Q_UNUSED (isWid);
    if (nattrg == 0)
        return 0;

    KKSMap<int, KKSAttrGroup *>::const_iterator pg = attrGroups.constBegin();
    bool isGrouped (nattrg>=2 ||
                   (nattrg == 1 && pg.value() && !pg.value()->childGroups().isEmpty()));
    
    for (; pg != attrGroups.constEnd(); pg++)
    {
        KKSAttrGroup * aGroup = pg.value();//attrGroups[i];
        if (!aGroup)
            continue;
        KKSMap<int, KKSAttrView*> attrs = aGroup->attrViews();
        KKSList<KKSAttrView *> attrs_list;
        for (KKSMap<int, KKSAttrView*>::const_iterator pa = attrs.constBegin(); pa != attrs.constEnd(); pa++)
            attrs_list.append (pa.value());
        qSort (attrs_list.begin(), attrs_list.end(), compareAttrViews);

        //
        // проверка на то, что группа может содержать только атрибуты с типом KKSAttrType::atCheckListEx
        //
        bool isAtCheckListEx = !aGroup->childGroups().isEmpty();//false;
        for (int ii=0; ii<attrs_list.count() && !isAtCheckListEx; ii++)
            isAtCheckListEx = isAtCheckListEx || (attrs_list[ii]->type()->attrType() != KKSAttrType::atCheckListEx);

        if (!isAtCheckListEx && isGrouped)
            continue;

        QGroupBox *gbAttr = 0;
        QGridLayout *gbLay = 0;
        if (isGrouped)
        {
            gbAttr = new QGroupBox (aGroup->name(), indWidget);
            gIndLay->addWidget (gbAttr, nc, 0, 1, 1);
            isWid = true;
            gbLay = new QGridLayout ();
            gbLay->setVerticalSpacing (10);
            //gbLay->setAlignment (Qt::AlignJustify);//Qt::AlignRight | Qt::AlignVCenter);
            gbLay->setContentsMargins (5, 5, 5, 5);
            gbAttr->setLayout (gbLay);
            gbAttr->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
            nc++;
        }

        putRecAttrsGroupsOnWidget (obj, wObjE, editor, nc, c, tableName, aGroup, gbLay, gIndLay, isGrouped);
    }

//    QVBoxLayout *vLay = new QVBoxLayout ();
//    vLay->addStretch (10);
//    gSysLayout->addLayout (vLay, nc, 0, 1, 2);

    return nc;
}

/* Метод загружает значение атрибута-ссылки в его виджет.
 * Параметры:
 * tableName -- название таблицы-справочника
 * awAttr -- виджет, содержащий искомое значение
 * attrId -- идентификатор атрибута
 */
void KKSObjEditorFactory :: loadAttributeReference (QString tableName, QWidget *awAttr, int attrId)
{
    KKSAttrRefWidget *aRefW = qobject_cast<KKSAttrRefWidget *>(awAttr);
    KKSObjEditor *editor = (qobject_cast<KKSObjEditor*>(this->sender()));
    QWidget * wEditor = qobject_cast<QWidget *>(this->sender());
//    connect (this, SIGNAL (editorCreatedModal (KKSObjEditor *)), editor, SLOT (childWidget(KKSObjEditor *)) );
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
    {
        qDebug () << __PRETTY_FUNCTION__ << tableName;
        return;
    }
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    //qDebug () << __PRETTY_FUNCTION__ << editor << refObj->id();

    //aRefW->

    KKSList<const KKSFilterGroup*> filters;
    if ((qobject_cast<CommandForm *>(this->sender()) && refObj->id () == IO_CAT_ID) ||
        (qobject_cast<MessageForm *>(this->sender()) && refObj->id () == IO_IO_ID))
    {
        QString value = QString ("select id from %1 where not is_system").arg (tableName);
        KKSCategory * ct = c->tableCategory ();
        if (!ct)
        {
            refObj->release();
            return;
        }
        KKSList<const KKSFilter *> fl;
        const KKSFilter * f = ct->createFilter (ATTR_ID, value, KKSFilter::foInSQL);
        if (!f)
        {
            refObj->release();
            return;
        }
        fl.append (f);
        f->release ();
        KKSFilterGroup * fg = new KKSFilterGroup(true);
        fg->setFilters(fl);
        filters.append(fg);
        fg->release();
    }
    else{ //все остальные случаи (обычные атрибуты)
        KKSAttribute * a = loader->loadAttribute(attrId);
        if(!a){
            refObj->release();
            return;
        }

        KKSSearchTemplate * st = NULL;
        st = a->searchTemplate();
        if(st){
            KKSFilterGroup * fg = st->getMainGroup();
            if(fg){
                filters.append(fg);
            }
        }

    }

    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         refObj->id (), 
                                                         //editor->m_filters, 
                                                         filters,//KKSList<const KKSFilterGroup*>(), 
                                                         "",
                                                         c,
                                                         true,
                                                         false,
                                                         wEditor ? wEditor->windowModality () : Qt::ApplicationModal,
                                                         wEditor, 
                                                         Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    emit editorCreatedModal (recEditor);
    
    if (editor && aRefW && recEditor->exec () == QDialog::Accepted)
    {
        aRefW->setValue (aRefW->getIdAttrValue(), aRefW->isSysAttr(), recEditor->getID());
        
        KKSAttrValue * av = NULL;

        KKSMap<qint64, KKSAttrValue *> sysAttrValues = editor->getSysAttrValues();
        av = sysAttrValues.value(aRefW->getIdAttrValue(), NULL);
        if(!av){
            KKSMap<qint64, KKSAttrValue *> ioAttrValues = editor->getIOAttrValues();
            av = ioAttrValues.value(aRefW->getIdAttrValue(), NULL);
            if(!av){
                KKSMap<qint64, KKSAttrValue *> recAttrValues = editor->getRecAttrValues();
                av = recAttrValues.value(aRefW->getIdAttrValue(), NULL);
            }
        }

        if(!av){
            //пройдемся по всем составным атрибутам ИО (ЭИО), если такие присутствуют
            KKSMap<qint64, KKSAttrValue*> aaList = editor->getSysAttrValues();
            KKSMap<qint64, KKSAttrValue*>::const_iterator i;
            for (i=aaList.constBegin (); i != aaList.constEnd(); i++){
                const KKSAttrValue *avC = i.value();            
                if(avC->attribute()->type()->attrType() != KKSAttrType::atComplex)
                    continue;
                av = avC->attrsValues().value(aRefW->getIdAttrValue(), NULL);
                if(av)
                    break;
            }

            if(!av){
                aaList = editor->getIOAttrValues();
                for (i=aaList.constBegin (); i != aaList.constEnd(); i++){
                    const KKSAttrValue *avC = i.value();            
                    if(avC->attribute()->type()->attrType() != KKSAttrType::atComplex)
                        continue;
                    av = avC->attrsValues().value(aRefW->getIdAttrValue(), NULL);
                    if(av)
                        break;
                }

                if(!av){
                    aaList = editor->getRecAttrValues();
                    for (i=aaList.constBegin (); i != aaList.constEnd(); i++){
                        const KKSAttrValue *avC = i.value();            
                        if(avC->attribute()->type()->attrType() != KKSAttrType::atComplex)
                            continue;
                        av = avC->attrsValues().value(aRefW->getIdAttrValue(), NULL);
                        if(av)
                            break;
                    }
                }
            }
        }
        
        if(!av){
            qCritical() << tr("Cannot find selected attribute!");
            QMessageBox::critical(editor, tr("Error"), tr("Cannot find selected attribute!"), QMessageBox::Ok);
            refObj->release();
            return;
        }
        
        /*KKSList<KKSAttrValue *> avs = editor->getObjectEx()->io()->attrValues();
        int cnt = avs.count();
        bool bFound = false;
        for(int i=0; i<cnt; i++){
            if(avs.at(i)->id() == aRefW->getIdAttrValue()){
                bFound = true;
                av = avs.at(i);
            }
        }*/

       
        /*
        //KKSMap<int, KKSCategoryAttr*>::const_iterator pa = editor->getObjectEx()->io()->category()->tableCategory()->attributes().constFind (aRefW->getIdAttr());
        //KKSCategoryAttr* pcAttr = 0;
        if (pa == editor->getObjectEx()->io()->category()->tableCategory()->attributes().constEnd())
        {
            const KKSCategory *tc = 0;
            if (editor->getObj () && editor->getObj ()->category() && editor->getObj ()->category()->tableCategory ())
                tc = editor->getObj ()->category()->tableCategory ();
            else //if (editor->getObj ())
                tc = editor->getCategory();
            if (tc)
            {
                pa = tc->attributes().constFind (aRefW->getIdAttr());
                if (pa != tc->attributes().constEnd())
                    pcAttr = pa.value();
            }
        }
        else
            pcAttr = pa.value ();

        */



        QMap<int, QString> values;
        QMap<int, QString> refColumnValues;
        switch (av->attribute()->type()->attrType())
        {
            case KKSAttrType::atList: 
            case KKSAttrType::atRecordColorRef: 
            case KKSAttrType::atRecordTextColorRef: 
            default:
                values = loader->loadAttributeValues (av->attribute(),
                                                      refColumnValues, 
                                                      false, 
                                                      true, 
                                                      QString::null, 
                                                      KKSList<const KKSFilterGroup*>());
                break;
            case KKSAttrType::atParent: 
                values = loader->loadAttributeValues (av->attribute(),
                                                      refColumnValues,
                                                      true, 
                                                      true, 
                                                      tableName.isEmpty() ? av->attribute()->tableName() : tableName,
                                                      KKSList<const KKSFilterGroup*>()); 
                break;
        }
        
        QString v_str = QString();
        QMap<int, QString>::const_iterator pv = values.constFind (recEditor->getID());
        if (pv != values.constEnd())
            v_str = pv.value();

        if(!refColumnValues.isEmpty())
        {
            int id = recEditor->getID();
            if(id <= 0)
                return;
            QString s = refColumnValues.constFind (id).value();
            //реальным значением атрибута (которое в дальнейшем пойдет в соответствующий INSERT или UPDATE)
            //должен быть внешний ключ. Поэтому мы сюда всегда записываем именно его, 
            //а в m_awf (т.е. в визуальный элемент) записываем значение из values
            aRefW->setValue (aRefW->getIdAttrValue(), aRefW->isSysAttr(), s);
        }


        QToolButton *tbRef=0;
        QCheckBox *ch=0; 
        QLabel *lTitle=0;
        qDebug () << __PRETTY_FUNCTION__ << values << v_str;
        QVariant vc;
        if (av->attribute()->type()->attrType() == KKSAttrType::atList || 
            av->attribute()->type()->attrType() == KKSAttrType::atParent )
        {
            vc = QVariant (v_str);
            KKSValue v = av->value();
            v.setColumnValue (v_str);
            av->setValue (v);
                
        }
        else if (av->attribute()->type()->attrType() == KKSAttrType::atRecordColorRef || 
                 av->attribute()->type()->attrType() == KKSAttrType::atRecordTextColorRef)
        {
            vc = QColor::fromRgba (v_str.toUInt());
        }
        
        m_awf->setValue (aRefW->getAttrWidget(), av, av->attribute()->refType(), aRefW->isSysAttr(), vc, true, tableName, c->tableCategory()->id(), tbRef, ch, editor, lTitle, true);
    }
    
    else if (qobject_cast<QLineEdit *> (awAttr) && 
             qobject_cast<CommandForm *> (this->sender()) && 
             recEditor->exec () == QDialog::Accepted)
    {
        KKSAttribute * attr = loader->loadAttribute (attrId);
        if (!attr){
            refObj->release();
            return;
        }

        int idAttr = attr->id ();
        attr->release ();

        CommandForm * cForm = qobject_cast<CommandForm *> (this->sender());
        QLineEdit * lE = qobject_cast<QLineEdit *> (awAttr);
        KKSMap<int, KKSCategoryAttr*>::const_iterator pa = cForm->getCommand()->io()->category()->tableCategory()->attributes().constFind (idAttr);
        KKSObjEditor * oRecEditor = qobject_cast<KKSObjEditor *>(recEditor);
        switch (oRecEditor->getObjectEx()->id())
        {
            case IO_CAT_ID:
                {
                    KKSCategory * cCat = loader->loadCategory (recEditor->getID ());
                    cForm->setCategory (cCat);
                    if (pa != cForm->getCommand()->io()->category()->tableCategory()->attributes().constEnd())
                    {
                        //KKSList<KKSAttrValue *> avals = cForm->attrValues();
                        KKSObjectExemplar * command = cForm->getCommand();
                        command->attrValue (attrId)->setValue (KKSValue (QString::number (cCat->id()), KKSAttrType::atList));
                        //cForm->setValues (avals);
                    }
                    if (cCat)
                        cCat->release ();
                    break;
                }
            case IO_POS_ID:
                {
                    if (pa != cForm->getCommand()->io()->category()->tableCategory()->attributes().constEnd())
                    {
                        int iattr;
                        int c_pos = loader->getDlId ();
                        if (attrId == ATTR_ID_DL_TO)
                            iattr = 5;
                        else
                            iattr = 6;
                        Q_UNUSED (iattr);
                        KKSObjectExemplar * command = cForm->getCommand();
                        command->attrValue (attrId)->setValue (KKSValue (QString::number (recEditor->getID ()), KKSAttrType::atList));
                        //KKSList<KKSAttrValue *> avals = cForm->attrValues();
                        //avals[iattr]->setValue (KKSValue (QString::number (recEditor->getID ()), KKSAttrType::atList));
                        //cForm->setValues (avals);
                        
                        QMap<int, QString> values;
                        QMap<int, QString> refValues;
                        if( pa.value()->type()->attrType() == KKSAttrType::atList){
                            values = loader->loadAttributeValues (pa.value(), refValues);
                        }
                        else{
                            values = loader->loadAttributeValues (pa.value(), 
                                                                  refValues,
                                                                  false, 
                                                                  true, 
                                                                  tableName.isEmpty() ? pa.value()->tableName() : tableName);
                        }
                        
                        QString v_str = QString();
                        QMap<int, QString>::const_iterator pv = values.constFind (recEditor->getID());
                        if (pv != values.constEnd())
                            v_str = pv.value();

                        if (attrId == ATTR_ID_DL_EXECUTOR)
                        {
                            if (!loader->isLocalDl (v_str.toInt()) && !loader->isPublicDl (c_pos))
                            {
                                qCritical() << tr ("Cannot send commands to remote positions");
                                QMessageBox::critical(cForm, tr ("Set executor"), tr ("Cannot send commands to remote positions"), QMessageBox::Ok);
                                return;
                            }
                        }
                        lE->setText (v_str);//pa.value ()->name());
                    }
                    break;
                }
            default: 
                {
                    KKSObjectExemplar * command = cForm->getCommand();

                    command->attrValue (attrId)->setValue (KKSValue (QString::number (recEditor->getID ()), KKSAttrType::atList));
                    
                    QMap<int, QString> values;
                    QMap<int, QString> refValues;

                    if( pa.value()->type()->attrType() == KKSAttrType::atList){ 
                        values = loader->loadAttributeValues (pa.value(), refValues);
                    }
                    else{
                        values = loader->loadAttributeValues (pa.value(), 
                                                              refValues,
                                                              false, 
                                                              true, 
                                                              tableName.isEmpty() ? pa.value()->tableName() : tableName);
                    }
                    
                    QString v_str = QString();
                    QMap<int, QString>::const_iterator pv = values.constFind (recEditor->getID());
                    if (pv != values.constEnd())
                    {
                        v_str = pv.value();
                        lE->setText (v_str);
                    }
                    break;
                }
        }
    }
    
    else if (qobject_cast<QLineEdit *> (awAttr) && 
             qobject_cast<MessageForm *> (this->sender()) && 
             recEditor->exec () == QDialog::Accepted)
    {
        KKSAttribute * attr = loader->loadAttribute (attrId);
        if (!attr){
            refObj->release();
            return;
        }

        int idAttr = attr->id ();
        attr->release ();
        MessageForm * messForm = qobject_cast<MessageForm *> (this->sender());
        QLineEdit * lE = qobject_cast<QLineEdit *> (awAttr);
        KKSMap<int, KKSCategoryAttr*>::const_iterator pa = messForm->getMessage()->io()->category()->tableCategory()->attributes().constFind (idAttr);
        if (pa != messForm->getMessage()->io()->category()->tableCategory()->attributes().constEnd())
        {
            int iattr (-1);
            int c_pos = loader->getDlId ();
            if (attrId == ATTR_ID_IO_OBJECT)
                iattr = 5;
            else if (attrId == ATTR_ID_DL_RECEIVER)
                iattr = 7;
            else
                return;
            KKSList<KKSAttrValue *> avals = messForm->attrValues ();
            avals[iattr]->setValue (KKSValue (QString::number (recEditor->getID ()), KKSAttrType::atList));
            messForm->setValues (avals);
            
            QMap<int, QString> values;
            QMap<int, QString> refValues;

            if(pa.value()->type()->attrType() == KKSAttrType::atList){
                values = loader->loadAttributeValues (pa.value(), refValues);
            }
            else{
                values = loader->loadAttributeValues (pa.value(), 
                                                      refValues, 
                                                      false, 
                                                      true, 
                                                      tableName.isEmpty() ? pa.value()->tableName() : tableName);
            }
            QString v_str = QString();
            QMap<int, QString>::const_iterator pv = values.constFind (recEditor->getID());
            if (pv != values.constEnd())
                v_str = pv.value();
            if (attrId == ATTR_ID_DL_RECEIVER)
            {
                if (!loader->isLocalDl (v_str.toInt()) && !loader->isPublicDl (c_pos))
                {
                    qCritical() << tr ("Cannot send messages to remote positions");
                    QMessageBox::critical (messForm, tr ("Set receiver"), tr ("Cannot send messages to remote positions"), QMessageBox::Ok);
                    refObj->release();
                    return;
                }
            }
            lE->setText (v_str);
        }
    }
    else if (qobject_cast<QLineEdit *> (awAttr) && qobject_cast<PeriodicReportForm *> (this->sender()) && recEditor->exec () == QDialog::Accepted)
    {
        KKSAttribute * attr = loader->loadAttribute (attrId);
        if (!attr)
            return;
        int idAttr = attr->id ();
        attr->release ();
        PeriodicReportForm * pForm = qobject_cast<PeriodicReportForm *> (this->sender());
        QLineEdit * lE = qobject_cast<QLineEdit *> (awAttr);
        KKSMap<int, KKSCategoryAttr*>::const_iterator pa = pForm->getReport()->io()->category()->tableCategory()->attributes().constFind (idAttr);
        if (pa != pForm->getReport()->io()->category()->tableCategory()->attributes().constEnd())
        {
            int iattr;
            if (attrId == ATTR_ID_IO_CATEGORY)
                iattr = 1;
            else if (attrId == ATTR_ID_DL_TO)
                iattr = 3;
            else if (attrId == ATTR_ID_DL_EXECUTOR)
                iattr = 4;
            else if (attrId == ATTR_ID_DL_CONTROLLER)
                iattr = 5;
            else if (attrId == ATTR_ID_JR_STATE)
                iattr = 10;
            else{
                refObj->release();
                return;
            }

            KKSList<KKSAttrValue *> avals = pForm->attrValues ();
            avals[iattr]->setValue (KKSValue (QString::number (recEditor->getID ()), KKSAttrType::atList));
            pForm->setValues (avals);
            
            QMap<int, QString> values;
            QMap<int, QString> refValues;
            if(pa.value()->type()->attrType() == KKSAttrType::atList){
                values = loader->loadAttributeValues (pa.value(), refValues);
            }
            else{
                values = loader->loadAttributeValues (pa.value(), 
                                                      refValues, 
                                                      false, 
                                                      true, 
                                                      tableName.isEmpty() ? pa.value()->tableName() : tableName);
            }
            
            QString v_str = QString();
            QMap<int, QString>::const_iterator pv = values.constFind (recEditor->getID());
            if (pv != values.constEnd())
                v_str = pv.value();
            lE->setText (v_str);//pa.value ()->name());
        }
    }
    else if (qobject_cast<QLineEdit *> (awAttr) && qobject_cast<KKSObjEditor *>(this->sender()) && recEditor->exec () == QDialog::Accepted)
    {
        int idVal = recEditor->getID();
        qDebug () << __PRETTY_FUNCTION__ << idVal;
        QLineEdit * lE = qobject_cast<QLineEdit *> (awAttr);
        KKSAttribute * attr = loader->loadAttribute (attrId);
        KKSObjEditor * editor = qobject_cast<KKSObjEditor *>(this->sender());
        if (!attr){
            refObj->release();
            return;
        }

        //int idAttr = attr->id ();
        QMap<int, QString> refValues;
        QMap<int, QString> values = loader->loadAttributeValues (attr, refValues, false, true, attr->tableName());
        QMap<int, QString>::const_iterator pv = values.constFind (idVal);
        QString v_str;
        if (pv != values.constEnd())
            v_str = pv.value();
        lE->setText (v_str);
        KKSObjectExemplar * pObjEx = editor->getObjectEx();
        KKSAttrValue * av = pObjEx->attrValue (attrId);
        KKSValue val (QString::number (idVal), KKSAttrType::atList);
        if (!av)
        {
            KKSCategoryAttr * cAttr = KKSCategoryAttr::create (attr, true, false);
            av = new KKSAttrValue (val, cAttr);
            pObjEx->addAttrValue (av);
            cAttr->release ();
        }
        else
            av->setValue (val);
        
        editor->setSysAttrValue(av);
        attr->release ();
    }

    recEditor->setParent (0);
    delete recEditor;
    refObj->release();
}

void KKSObjEditorFactory :: setRecState (KKSObjEditor * editor, QWidget * stateW, KKSObjectExemplar * wObjE)
{
    KKSObject *refObj = loader->loadIO (IO_STATE_ID, true);
    if (!refObj)
    {
        return;
    }
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    //qDebug () << __PRETTY_FUNCTION__ << editor << refObj->id();

    //aRefW->

    KKSList<const KKSFilterGroup*> filters;
    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         refObj->id (), 
                                                         //editor->m_filters, 
                                                         filters,//KKSList<const KKSFilterGroup*>(), 
                                                         "",
                                                         c,
                                                         true,
                                                         false,
                                                         Qt::WindowModal,
                                                         editor, 
                                                         Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    emit editorCreatedModal (recEditor);
    
    if (editor && stateW && recEditor->exec () == QDialog::Accepted)
    {
        int idState = recEditor->getID();
        KKSState * st = loader->loadState(idState);
        wObjE->setState (st);
        if (st && qobject_cast<QLineEdit *>(stateW))
            (qobject_cast<QLineEdit *>(stateW))->setText(st->name());
        if (st)
            st->release();
        editor->attrValueChanged();
    }
    recEditor->setParent(0);
    delete recEditor;
    refObj->release ();
}

void KKSObjEditorFactory :: loadExecReference (QAbstractItemModel *exModel)
{
    if (!exModel || exModel->columnCount () == 0)
        return;
    QString tableName = QString("mail_lists");
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
    {
        qDebug () << __PRETTY_FUNCTION__ << tableName;
        return;
    }
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    QWidget * wEditor = qobject_cast<QWidget *>(this->sender());
    KKSList<const KKSFilterGroup*> filters;

    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         refObj->id (), 
                                                         filters,
                                                         "",
                                                         c,
                                                         true,
                                                         false,
                                                         wEditor ? wEditor->windowModality () : Qt::ApplicationModal,
                                                         wEditor, 
                                                         Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    if (recEditor->exec () == QDialog::Accepted)
    {
        QList<int> oldPos;
        for (int i=0; i<exModel->rowCount(); i++)
        {
            QModelIndex wIndex = exModel->index (i, 0);
            oldPos.append (wIndex.data (Qt::UserRole).toInt());
        }
        int id_mail_list = recEditor->getID ();
        qDebug () << __PRETTY_FUNCTION__ << id_mail_list;
        KKSObjectExemplar * cMailList = loader->loadEIO (id_mail_list, refObj);
        KKSAttrValue * aVal = cMailList->attrValue (ATTR_MAIL_LISTS_POSITION);
        qDebug () << __PRETTY_FUNCTION__ << (aVal ? aVal->value ().valueVariant() : QString());
        QStringList posVals = aVal->value ().valueVariant().toStringList ();
        QMap<int, QString> allPos = loader->getDls ();
        QList<int> posIdList;
        for (int i=0; i<posVals.count(); i++)
        {
            bool ok;
            int idPos = posVals[i].toInt( &ok );
            if (!oldPos.contains (idPos))
                posIdList.append (idPos);
        }

        int nr = exModel->rowCount();
        int nc = exModel->columnCount ();
        if (nc == 0)
            exModel->insertColumns (0, 1);

        exModel->insertRows (nr, posIdList.count());
        for (int i=0; i<posIdList.count(); i++)
        {
            QModelIndex wIndex = exModel->index (nr+i, 0);
            QString posName = allPos.constFind (posIdList[i]) != allPos.constEnd() ? allPos.constFind (posIdList[i]).value () : QString();
            exModel->setData (wIndex, posIdList[i], Qt::UserRole);
            exModel->setData (wIndex, posName, Qt::DisplayRole);
            exModel->setData (wIndex, true, Qt::CheckStateRole);
        }
    }
    refObj->release();
}

void KKSObjEditorFactory :: addExecReference (QString tableName, QAbstractItemModel *exModel, int idAttr)
{
    Q_UNUSED (idAttr);

    if (!exModel || exModel->columnCount () == 0 )
        return;
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
    {
        qDebug () << __PRETTY_FUNCTION__ << tableName;
        return;
    }
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    QWidget * wEditor = qobject_cast<QWidget *>(this->sender());
    KKSList<const KKSFilterGroup*> filters;

    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         refObj->id (), 
                                                         filters,
                                                         "",
                                                         c,
                                                         true, false,
                                                         wEditor ? wEditor->windowModality () : Qt::ApplicationModal,
                                                         wEditor, 
                                                         Qt::Dialog);
    if (!recEditor || !recEditor->getRecordsWidget()){
        refObj->release();
        return;
    }

    QTreeView * tv = recEditor->getRecordsWidget()->getView();
    if (!tv || !tv->selectionModel()){
        refObj->release();
        return;
    }

    tv->setSelectionMode (QAbstractItemView::ExtendedSelection);
    tv->setSelectionBehavior (QAbstractItemView::SelectRows);
    if (recEditor->exec () == QDialog::Accepted)
    {
        QList<int> oldPos;
        for (int i=0; i<exModel->rowCount(); i++)
        {
            QModelIndex wIndex = exModel->index (i, 0);
            oldPos.append (wIndex.data (Qt::UserRole).toInt());
        }
        QModelIndexList selIndexes = recEditor->getRecordsWidget()->getSourceIndexes();
        if (selIndexes.isEmpty())
        {
            qWarning() << tr ("Select executor");
            QMessageBox::warning (recEditor, tr ("Add executor"), tr ("Select executor"), QMessageBox::Ok, QMessageBox::Ok);
            refObj->release();
            return;
        }

        QMap<int, QString> allPos = loader->getDls ();
        QList<int> posIdList;
        for (int i=0; i<selIndexes.count(); i++)
        {
            if (selIndexes[i].column () == 0 && !oldPos.contains (selIndexes[i].data (Qt::UserRole).toInt()))
                posIdList.append (selIndexes[i].data (Qt::UserRole).toInt());
        }

        int nr = exModel->rowCount();
        int nc = exModel->columnCount ();
        if (nc == 0)
            exModel->insertColumns (0, 1);

        exModel->insertRows (nr, posIdList.count());
        for (int i=0; i<posIdList.count(); i++)
        {
            QModelIndex wIndex = exModel->index (nr+i, 0);
            QString posName = allPos.constFind (posIdList[i]) != allPos.constEnd() ? allPos.constFind (posIdList[i]).value () : QString();
            exModel->setData (wIndex, posIdList[i], Qt::UserRole);
            exModel->setData (wIndex, posName, Qt::DisplayRole);
            exModel->setData (wIndex, true, Qt::CheckStateRole);
        }
    }
    refObj->release();
}

/* слот осуществляет скачивание файла из БД в локальную файловую систему.
 */
void KKSObjEditorFactory :: slotDownloadFile(KKSFile * f, QWidget * parent)
{
    if(!f)
        return;

    int res = fileLoader->rGetFile(f->id(), f->localUrl(), -1, parent);

    if(res != OK_CODE){
        f->setLocalUrl(QString::null);
    }
}

/* Метод осуществляет установку шаблонов для пользовательских атрибутов.
 * Параметры:
 * editor -- виджет редактора ИО.
 * wObj -- редактируемый ИО.
 */
void KKSObjEditorFactory :: setEIOTemplates (KKSObjEditor* editor, KKSObject* wObj)
{
    if (!wObj || !editor)
        return;
    KKSCategory *c = 0;
    if (wObj->category() && wObj->category()->tableCategory ())
        c = wObj->category()->tableCategory ();
    if (!c)
        return;

    KKSList<KKSTemplate*> tListDb = loader->loadCategoryTemplates(c);

    KKSList<KKSTemplate*> tList;
    for (int i=0; i<tListDb.count(); i++)
    {
        KKSTemplate *tdb = tListDb[i];
        if (tdb)
        {
            tdb->addRef ();
            tList.append (tdb);
            tdb->release ();
        }
    }
    KKSTemplate * tRef = 0;

    ChooseTemplateForm * f = new ChooseTemplateForm (tList, editor);
    if (f->exec() == QDialog::Accepted)
    {
        tRef = f->getCurrentTemplate();
        qDebug () << __PRETTY_FUNCTION__ << tRef->id ();
        tRef->addRef();
        if (f->isSave ())
        {
            wObj->setTableTemplate (tRef);
            //так ни в коем случае нельзя делать!!!
            //ppf->updateIO (wObj);
            //надо просто обновить информацию о пользовательских шаблонах
            int ok = ppf->updateUserTemplates(wObj);
            if(ok != OK_CODE){
                qCritical() << tr("Cannot update templates!");
                QMessageBox::critical(editor, tr("Error"), tr("Cannot update templates!"), QMessageBox::Ok);
                f->setParent (0);
                delete f;
                return;
            }
        }
    }
//    else
//        tRef = new KKSTemplate(c->defTemplate());

    f->setParent (0);
    delete f;

    if (!tRef)
        return;
    editor->setRecTemplate (tRef);
    KKSList<const KKSFilterGroup*> filters = editor->m_filters;
    KKSViewFactory::loadEIOEx (editor, wObj, loader, tRef, editor->getRecordsWidget()->getView(), filters, true);
    tRef->release ();
    editor->update ();
}

int KKSObjEditorFactory :: insertInControlJournal(int idJournal, int idObject, bool draft)
{
    return ppf->setAsExecutedCommand(idObject, idJournal, draft);
}

int KKSObjEditorFactory :: updateInControlJournal(int idJournal, int idObject)
{
    int ok = ppf->setAsCommandResult(idObject, idJournal);
    if(ok <= 0){ //not set as executed (в таблице tsd_control_journal не зановилась информация о черновике исполняемого ИО)
        ok = ppf->setAsExecutedCommand(idObject, idJournal, false);
    }
    return ok;
}

void KKSObjEditorFactory :: slotIncludeRequested(KKSObjEditor * editor)
{
    if(!editor)
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
    if(!filter){
        o->release();
        return;
    }

    filters.append(filter);
    filter->release();
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();
    
    KKSObjEditor *objEditor = createObjEditor(IO_IO_ID, 
                                              IO_IO_ID, 
                                              filterGroups, 
                                              "",
                                              c,
                                              true,
                                              QString(),
                                              false,
                                              editor->windowModality(),
                                              editor,
                                              Qt::Dialog);
    int res = objEditor->exec();
    
    int idObject = -1;
    QString name;

    if(res == QDialog::Accepted){
        idObject = objEditor->getID();
        KKSObject * o = loader->loadIO(idObject, true);
        if(!o){
            delete objEditor;
            return;
        }
        name = o->name();
        o->release();
    }
    
    delete objEditor;
    
    editor->slotIncludeSelected(idObject, name);
    o->release();
}

void KKSObjEditorFactory :: slotIncludeRecRequested(KKSObjEditor * editor)
{
    if(!editor)
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
    if(!filter){
        o->release();
        return;
    }
    KKSFilter * filterRef = c->createFilter(ATTR_ID_IO_TYPE, QString::number (2), KKSFilter::foEq);
    if (!filterRef)
    {
        filter->release ();
        o->release ();
        return;
    }

    filters.append (filter);
    filters.append (filterRef);
    filterRef->release ();
    filter->release();
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();
    
    KKSRecDialog *objEditor = createObjRecEditor(IO_IO_ID, 
                                                 IO_IO_ID, 
                                                 filterGroups, 
                                                 "",
                                                 c,
                                                 true,
                                                 false,
                                                 editor->windowModality(),
                                                 editor,
                                                 Qt::Dialog);
    int res = objEditor->exec();
    
    int idObject = -1;
    QString name;
    int idObjRec (-1);

    if(res == QDialog::Accepted){
        idObject = objEditor->getID();
        KKSObject * o = loader->loadIO(idObject, true);
        if(!o){
            delete objEditor;
            return;
        }
        /*name = o->name();
        o->release();*/
        KKSList<const KKSFilterGroup *> recFilterGroups;
        KKSRecDialog * refObjEditor = createObjRecEditor (IO_IO_ID,
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
            idObjRec = refObjEditor->getID();
            KKSObjectExemplar * rec = loader->loadEIO(idObjRec, o);
            if (rec)
            {
                name = rec->name ();
                rec->release ();
            }
        }
        o->release();
                
    }
    else
    {
        o->release();
        return;
    }
    
    delete objEditor;
    
    if (idObjRec > 0)
        editor->slotIncludeRecSelected(idObjRec, idObject, name);//slotIncludeSelected(idObject, name);
    o->release();
}

void KKSObjEditorFactory :: slotOpenRubricItemRequested(int idObject, KKSObjEditor * editor)
{
    KKSObject * o = loader->loadIO(idObject, true);
    if(!o)
        return;
    KKSCategory * c = o->category();
    if(!c){
        o->release();
        return;
    }

    KKSObjEditor * newObjEditor = this->createObjEditor (IO_IO_ID, 
                                                         idObject, 
                                                         KKSList<const KKSFilterGroup*>(), 
                                                         "",
                                                         c,
                                                         false,
                                                         QString(),
                                                         false,
                                                         editor->windowModality (),
                                                         NULL);
    o->release();

    if(!newObjEditor)
    {
        qCritical() << tr("Cannot create new object editor! Corrupt data!");
        QMessageBox::critical(editor, 
                              tr("Error"), 
                              tr("Cannot create new object editor! Corrupt data!"), 
                              QMessageBox::Ok);
        return;
    }

    connect(newObjEditor, 
            SIGNAL(eioChanged(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )), 
            editor, 
            SLOT(updateEIOEx(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )));
    
    newObjEditor->setAttribute (Qt::WA_DeleteOnClose);

    emit editorCreated(newObjEditor);
}

void KKSObjEditorFactory :: slotOpenRubricItemRecRequested(int idObjectE, KKSObjEditor * editor)
{
    int idObject = loader->getRefIO(idObjectE);
    KKSObject * o = loader->loadIO(idObject, true);
    if(!o)
        return;
    KKSCategory * c = o->category();
    if(!c){
        o->release();
        return;
    }
/*    c = c->tableCategory ();
    if(!c){
        o->release();
        return;
    }*/

    KKSObjEditor * newObjEditor = this->createObjEditor (idObject, 
                                                         idObjectE, 
                                                         KKSList<const KKSFilterGroup*>(), 
                                                         "",
                                                         c,
                                                         false,
                                                         QString(),
                                                         false,
                                                         editor->windowModality (),
                                                         NULL);
    o->release();

    if(!newObjEditor)
    {
        qCritical() << tr("Cannot create new object editor! Corrupt data!");
        QMessageBox::critical(editor, 
                              tr("Error"), 
                              tr("Cannot create new object editor! Corrupt data!"), 
                              QMessageBox::Ok);
        return;
    }

    connect(newObjEditor, 
            SIGNAL(eioChanged(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )), 
            editor, 
            SLOT(updateEIOEx(KKSObject *, const KKSMap<qint64, KKSObjectExemplar *>&, QAbstractItemModel *, const QModelIndex& )));
    
    newObjEditor->setAttribute (Qt::WA_DeleteOnClose);

    emit editorCreated(newObjEditor);
}

/* Слот выбирает подходящий пользовательский шаблон и перегруппировывает атрибуты.
 */
void KKSObjEditorFactory :: regroupAttrs (QWidget *wIOAttr, QScrollArea *scIOattr, QWidget *ioAttrs, int idObj, const KKSCategory *c, KKSIndAttr::KKSIndAttrClass isSystem, KKSObjEditor *editor)
{
    KKSObject *wObj = idObj > 0 ? loader->loadIO (idObj, false) : editor->getObj();
    if (!wObj || !editor )//|| (!c && !isSystem))
        return;

    KKSTemplate * tDef;
    KKSList<KKSTemplate*> tListDb;
    KKSObject *io = 0;
#ifdef Q_CC_MSVC
    if (isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
    if (isSystem == KKSIndAttr::iacTableAttr)
#endif
    {
        io = loader->loadIO (IO_IO_ID, false);
        if (!io)
        {
            wObj->release ();
            return;
        }
        /* wObj->category()->tableCategory() */
        tListDb = loader->loadCategoryTemplates ((c ? c->id() : io->category()->tableCategory()->id()));
        tDef = new KKSTemplate (c ? c->defTemplate() : io->category()->tableCategory()->defTemplate());
    }
    else
    {
        tListDb = loader->loadCategoryTemplates(c->id());
        tDef = new KKSTemplate (c->defTemplate());
    }

    KKSList<KKSTemplate*> tList;
    tList.append (tDef);
    tDef->release ();
    for (int i=0; i<tListDb.count(); i++)
    {
        KKSTemplate *tdb = tListDb[i];
        if (tdb)
        {
            tdb->addRef ();
            tList.append (tdb);
            tdb->release ();
        }
    }

    if (tList.count() == 0)
        return;

    KKSTemplate * tRef = 0;

    ChooseTemplateForm * f = new ChooseTemplateForm (tList, editor);
    if (f->exec() == QDialog::Accepted)
    {
        tRef = f->getCurrentTemplate();
        tRef->addRef();
        if (f->isSave ())
        {
#ifdef Q_CC_MSVC
            if (isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
            if (isSystem == KKSIndAttr::iacTableAttr)
#endif
                io->setAttrTemplate (tRef);
            else
                wObj->setAttrTemplate (tRef);

            //так ни в коем случае нельзя делать!!!
            //int res = ppf->updateIO (isSystem ? io : wObj);
            //надо просто обновить информацию о пользовательских шаблонах
#ifdef Q_CC_MSVC
            int ok = ppf->updateUserTemplates(isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr ? io : wObj);
#else
            int ok = ppf->updateUserTemplates(isSystem == KKSIndAttr::iacTableAttr ? io : wObj);
#endif
            if(ok != OK_CODE){
                qCritical() << tr("Cannot update templates!");
                QMessageBox::critical(editor, tr("Error"), tr("Cannot update templates!"), QMessageBox::Ok);
                f->setParent (0);
                delete f;
                return;
            }
        }
    }

    f->setParent (0);
    delete f;

    if (!tRef)
        return;

#ifdef Q_CC_MSVC
    if (isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
    if (isSystem == KKSIndAttr::iacTableAttr)
#endif
        editor->setSysTemplate (tRef);
    else
        editor->setIoTemplate (tRef);

    if (!wIOAttr)
        wIOAttr = new QWidget ();

    if (ioAttrs)
    {
#ifdef Q_CC_MSVC
        if (isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
        if (isSystem == KKSIndAttr::iacTableAttr)
#endif
            editor->setSysAttrWidgets (wIOAttr, scIOattr, 0);
        else
            editor->setIOAttrWidgets (wIOAttr, scIOattr, 0);
    }

    QGridLayout *gIOLay = qobject_cast<QGridLayout *>(wIOAttr->layout());
    if (!gIOLay)
    {
        gIOLay = new QGridLayout ();
        wIOAttr->setLayout (gIOLay);
        gIOLay->addWidget (scIOattr, 0, 0, 1, 2);
    }
    
    QScrollArea * scIOAttrs = scIOattr;//new QScrollArea (wIOAttr);
    scIOAttrs->setWidgetResizable(true);
    QWidget *ioAttrsW = new QWidget ();
    QGridLayout *gIOAttrLay = new QGridLayout (ioAttrsW);
    ioAttrsW->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Fixed);
#ifdef Q_CC_MSVC
    if (isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
    if (isSystem == KKSIndAttr::iacTableAttr)
#endif
    {
        editor->setSysAttrWidgets (wIOAttr, scIOAttrs, ioAttrsW);
        editor->clearSysOpts ();
    }
    else
    {
        editor->setIOAttrWidgets (wIOAttr, scIOAttrs, ioAttrsW);
        editor->clearOptionals ();
    }
    
    ioAttrsW->setLayout (gIOAttrLay);
#ifdef Q_CC_MSVC
    if (isSystem == KKSIndAttr::KKSIndAttrClass::iacTableAttr)
#else
    if (isSystem == KKSIndAttr::iacTableAttr)
#endif
    {
        KKSObjectExemplar *wObjE = editor->getObjectEx();//KKSConverter::objToExemplar (loader, wObj);// loader->loadEIO (idObj, io);
        if (!wObjE)
        {
            wObj->release ();
            if (io)
                io->release ();
            return;
        }
        wObjE->addRef ();
        setAttributes (tRef, wObj, ioAttrsW, gIOAttrLay, 0, wObjE, QString(), editor);
        wObjE->release ();
    }
    else
        setAttributes (tRef, wObj, ioAttrsW, gIOAttrLay, c, editor, true);

    editor->setObjChanged (false);
    if (idObj > 0)
        wObj->release ();
    if (io)
        io->release ();
}

/* Метод осуществляет импорт данных в справочник.
 * Параметры:
 * editor -- редактор ИО
 * idObject -- идентификатор ИО-справочника
 * cat -- главная категория справочника, в который импортируем записи
 * tableName -- название таблицы.
 */
void KKSObjEditorFactory :: importEIO (KKSObjEditor * editor, int idObject, const KKSCategory * cat, QString tableName, QAbstractItemModel * recModel)
{
    Q_UNUSED (recModel);
    KKSCategory *c0 = 0;//t ? t->category() : 0;

    KKSObject *io = loader->loadIO (idObject, true);
    if (!io)
        return;

    if (cat && cat->tableCategory())
        c0 = new KKSCategory (*(cat->tableCategory()));
    else if (cat && cat->type()->id() == 10)
        c0 = new KKSCategory (*(cat));
    else
        c0 = new KKSCategory (*(io->category()->tableCategory()));

    KKSAttribute *attrId = loader->loadAttribute (1);
    if (!attrId){
        io->release();
        return;
    }

    KKSCategoryAttr *cAttrId = KKSCategoryAttr::create (attrId, true, true);
    if (!c0->attributes().contains (cAttrId->id()))
        c0->addAttribute (cAttrId->id(), cAttrId);

    if (!c0){
        io->release();
        return;
    }

//    c0->addRef ();
    KKSXMLForm * xmlForm = new KKSXMLForm (io, tr("Import copies into IO %1").arg (io->name()), false, editor);
    if (!xmlForm)
    {
        attrId->release ();
        cAttrId->release ();
        c0->release ();
    }
    connect (xmlForm, SIGNAL (loadAttributes (KKSCategory *, const QStringList&)), this, SLOT (importCatAttrs (KKSCategory *, const QStringList&)) );
    connect (xmlForm, SIGNAL (loadCSV (QIODevice *, QString, QString, QString, QAbstractItemModel *, KKSXMLForm *)), this, SLOT (importCSV (QIODevice *, QString, QString, QString, QAbstractItemModel *, KKSXMLForm *)) );

    if (xmlForm->exec () == QDialog::Accepted)
    {
        xmlForm->hide ();
        KKSCategory * c1 = xmlForm->getCategory ();
        //
        // категория справочника, полученная из xml-файла
        //
        if (!c1 || !c1->tableCategory() ||
            xmlForm->getCSVFile().isEmpty() || 
            xmlForm->getXMLFile().isEmpty() || 
            xmlForm->getCharset().isEmpty() || 
            xmlForm->getFieldDelimiter().isEmpty()
           )
        {
            qCritical() << tr("Invalid data");
            QMessageBox::critical(editor, tr ("Error"), tr("Invalid data"), QMessageBox::Ok);
            io->release();
            return;
        }

        if (!c1->tableCategory()->attributes().contains (cAttrId->id()))
            c1->tableCategory()->addAttribute (cAttrId->id(), cAttrId);
        
        if (c0->attributes().count() != c1->tableCategory()->attributes().count())//категория, в которую импортируем, не соответствует той, которая получена из xml-файла
        {
            qCritical() << tr("Inconsistent categories");
            QMessageBox::critical(editor, tr ("Error"), tr("Inconsistent categories"), QMessageBox::Ok);
            io->release();
            return;
        }

        KKSMap<int, KKSCategoryAttr*>::const_iterator pc0 = c0->attributes ().constBegin();//подчиненная
        KKSMap<int, KKSCategoryAttr*>::const_iterator pc  = c1->tableCategory()->attributes ().constBegin();//подчиненная
        for (; pc0 != c0->attributes().constEnd(); pc0++)
        {
            if (pc0.key() != pc.key() )
            {
                qCritical() << tr("Inconsistent categories");
                QMessageBox::critical(editor, tr ("Error"), tr("Inconsistent categories"), QMessageBox::Ok);
                io->release();
                return;
            }
            pc++;
        }

        QStringList attrCodes;
        for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = c0->attributes().constBegin(); \
             pa != c0->attributes().constEnd(); \
             pa++)
        {
            qDebug () << __PRETTY_FUNCTION__ << pa.key() << pa.value()->code (true);
            attrCodes += pa.value()->code (false);
        }
        importCopies (io, attrCodes,/*xmlForm->getAttrCodeList (),*/ xmlForm->getCopies(), editor, c0, tableName);
    }

    if (attrId)
        attrId->release ();

    if (cAttrId)
        cAttrId->release ();

    xmlForm->setParent (0);
    delete xmlForm;
    c0->release ();
    io->release();
}

void KKSObjEditorFactory :: importCatAttrs (KKSCategory *c, const QStringList& catAttrs)
{
    if (!c || catAttrs.isEmpty())
        return;

/*    
    for (int i=0; i<catAttrs.count(); i++)
    {
        QString code = catAttrs[i];
        KKSAttribute *attr = loader->loadAttribute (code);
        if (!attr)
            continue;
        KKSCategoryAttr *cAttr = KKSCategoryAttr::create (attr, false, false);
        if (!cAttr)
            continue;
        c->addAttribute (cAttr->id(), cAttr);
        cAttr->release ();
        attr->release ();
    }
 */
}

/* Импорт данных из файла и сохранение их в БД.
 */
void KKSObjEditorFactory :: importCSV (QIODevice *csvDev, QString codeName, QString fDelim, QString tDelim, QAbstractItemModel *dataModel, KKSXMLForm *xmlForm)
{
    if (!csvDev)
        return;
    
    if (!csvDev->isOpen() && !csvDev->open (QIODevice::ReadOnly))
        return;

    if (codeName.isEmpty () || fDelim.isEmpty() || !dataModel)
        return;

    QTextStream csvFile (csvDev);
    QTextCodec *csvCodec = QTextCodec::codecForName (codeName.toAscii());
    if (!csvCodec)
        return;
    csvFile.setCodec (csvCodec);
    if (dataModel->rowCount () > 0)
        dataModel->removeRows (0, dataModel->rowCount());

    xmlForm->clearCopies ();
    QStringList csvStrings;
    while (!csvFile.atEnd())
    {
        QString fstr = csvFile.readLine ();
        csvStrings.append (fstr);
    }
    csvDev->close ();

    dataModel->insertRows (0, csvStrings.count());
    for (int i0=0; i0<csvStrings.count(); i0++)
    {
        QString fstr = csvStrings[i0];
        QStringList lineData;
        QString escFDelim = QString("%1").arg (QRegExp::escape(fDelim));
        // (?!(\\{(.)*(%1)*(.)*\\}))
        //escFDelim += QString("(({(.)*}))*");//[\^(\{%1*\})]").arg (QRegExp::escape (fDelim));
        QRegExp fRegExp(escFDelim);
        lineData = fstr.split (fRegExp);
        //qDebug () << __PRETTY_FUNCTION__ << lineData.size() << escFDelim << fstr;
        //for (int ii=0; ii<lineData.size(); ii++)
        //    qDebug () << __PRETTY_FUNCTION__ << lineData[ii];// << escFDelim << fRegExp;
        if (fstr.contains (tDelim))
        {
            for (int i=0; i<lineData.count(); )
            {
                if (lineData[i].startsWith ("{"))
                {
                    lineData[i] = lineData[i].mid(1);
                    while (!lineData[i].endsWith("}"))
                    {
                        lineData[i] += (i < lineData.count()-1 ? QString (",%1").arg (lineData[i+1]) : tDelim);
                        //qDebug () << __PRETTY_FUNCTION__ << lineData[i];
                        if (i < lineData.count()-1)
                            lineData.removeAt (i + 1);
                    }
                    lineData[i] = lineData[i].mid (1, lineData[i].count()-2);
                    //qDebug () << __PRETTY_FUNCTION__ << lineData[i];
                }
                while (lineData[i].startsWith (tDelim) && (!lineData[i].endsWith (tDelim) ||
                                                            lineData[i].endsWith (QString("\\%1").arg (tDelim)) ))
                {
                    lineData[i] += fDelim + (i < lineData.count()-1 ? lineData[i+1] : tDelim);
                    if (i < lineData.count()-1)
                        lineData.removeAt (i + 1);
                }
                //qDebug () << __PRETTY_FUNCTION__ << lineData[i];
                i++;
            }
        }

        if (lineData.count() - dataModel->columnCount() > 0)
        {
            //qDebug () << __PRETTY_FUNCTION__ << lineData.count() << dataModel->columnCount() << lineData;
            if (dataModel->rowCount())
                dataModel->removeRows (0, dataModel->rowCount());
            qCritical() << tr ("Inconsistence data on row %1").arg (i0);
            QMessageBox::critical(xmlForm, tr("Import data"), tr ("Inconsistence data on row %1").arg (i0));
            return;
        }

        int row = i0;
        QStringList oeStr;
        for (int i=0; i<lineData.count(); i++)
        {
            QModelIndex wIndex = dataModel->index (row, i);
            if (!wIndex.isValid())
                continue;

            if (lineData[i].startsWith (tDelim))
            {
                oeStr.append (lineData[i].mid (1, lineData[i].count()-2));
                dataModel->setData (wIndex, lineData[i].mid (1, lineData[i].count()-2), Qt::DisplayRole);
            }
            else
            {
                oeStr.append (lineData[i]);
                dataModel->setData (wIndex, lineData[i], Qt::DisplayRole);
            }
        }
        //qDebug () << __PRETTY_FUNCTION__ << oeStr;
        xmlForm->addCopy (oeStr);
    }
}

/* Метод осуществляет импорт экземпляров справочника из матрицы строк в БД.
 * Параметры :
 * io -- целевой информационный объект-справочник
 * attrCodeList -- список кодов атрибутов
 * oesList -- список импортируемых записей должен соответствовать категории ИО
 * oEditor -- редактор Ио-справочника
 * cat -- категория справочника должна соответствовать категории таблиц справочника ИО
 * tableName -- название целевой таблицы
 */
void KKSObjEditorFactory :: importCopies (KKSObject *io, 
                                          const QStringList& attrCodeList, 
                                          const QList<QStringList>& oesList, 
                                          KKSObjEditor *oEditor, 
                                          const KKSCategory *cat, 
                                          const QString& tableName)
{
    if (!io || !oEditor)
        return;
/*    Q_UNUSED (attrCodeList);
    Q_UNUSED (oesList);
    Q_UNUSED (cat);
    Q_UNUSED (tableName);
 */

    KKSList<KKSObjectExemplar *> oeList;
    int n = oesList.count();
    int m = attrCodeList.count();
    QMap<QString, qint64> oldUids;
    QStringList uidsSorted;
    for (int i=0; i<n; i++)
    {
        oldUids.insert(oesList[i][0], 0);
        uidsSorted.append (oesList[i][0]);
    }
    //qDebug () << __PRETTY_FUNCTION__ << oldUids;
    
    QProgressDialog *pImportD = new QProgressDialog ();//tr("Import Copies"), tr("Abort import"), 0, n*m);
    QLabel *lImport = new QLabel (tr("Generate Records"), pImportD);
    pImportD->setLabel (lImport);
    QPushButton *pbCancelButton = new QPushButton (tr("Abort generate"), 0);//pImportD);
    pImportD->setCancelButton (pbCancelButton);
    QProgressBar *pBar = new QProgressBar (pImportD);
    pImportD->setBar (pBar);

    pBar->setMinimum (0);
    pBar->setMaximum (n*m);

    pImportD->setWindowTitle (tr("Import copies into %1").arg (oEditor->windowTitle()));
    pImportD->setWindowModality (Qt::WindowModal);
    connect (pbCancelButton, SIGNAL (clicked()), pImportD, SLOT (cancel()) );

    int v = 0;
    pImportD->setMinimumDuration (0);
    pImportD->show ();
    //qDebug () << __PRETTY_FUNCTION__ << pImportD->isVisible () << pImportD->labelText();// << pbCancelButton->isVisible () << pBar->isVisible ();
    
    KKSList<KKSAttribute *> attrList;
    KKSList<const KKSCategoryAttr*> cAttrList;
    //KKSList<KKSObject*> refObjList;
    
    for (int j=0; j<m; j++){
        KKSAttribute *attr = loader->loadAttribute (attrCodeList[j], io->tableName());
        if(!attr)
            continue;

        attrList.append(attr);

        const KKSCategoryAttr *cAttr = cat->attribute(attr->id());
        if(!cAttr)
            cAttr = new KKSCategoryAttr();

        cAttrList.append(cAttr);

        cAttr->release();

        if(attr)
            attr->release();
    }
    
    int m1 = attrList.count();

    QMap<QString, qint64> refIds;
    for (int i=0; i<n; i++)
    {
        KKSObjectExemplar *oe = new KKSObjectExemplar ();
        pImportD->setValue (v);
        //pBar->setValue (v);
        if (pImportD->wasCanceled())
        {
            pImportD->setParent (0);
            delete pImportD;
            return;
        }
        if (!oe)
            continue;
        
        oe->setIo (io);
        
        for (int j=0; j<m1; j++)
        {
            pImportD->setValue (v++);
            if (pImportD->wasCanceled())
            {
                pImportD->setParent (0);
                delete pImportD;
                return;
            }
            
            if (!attrList[j])
                continue;

            const KKSCategoryAttr *cAttr = cAttrList[j];//здесь NULL никогда не вернется
            if (!cAttr || cAttr->id() <= 0)
                return;

            KKSAttrType :: KKSAttrTypes iType = cAttr->type()->attrType();

            KKSValue val;
            if (iType == KKSAttrType::atList || 
                iType == KKSAttrType::atParent ||
                iType == KKSAttrType::atRecordColorRef ||
                iType == KKSAttrType::atRecordTextColorRef)
            {
                
                QString av_str (oesList[i][j+1]);//(iType == KKSAttrType::atParent ? 0 : j+1)]);
                //qDebug () << __PRETTY_FUNCTION__ << av_str;
                qint64 rid (-1);
                if (refIds.contains(av_str))
                    rid = refIds.value (av_str);
                else
                {
                    QString tName = (iType == KKSAttrType::atParent ? io->tableName() : cAttr->tableName());
                    rid = loader->getIdByUID (tName, av_str);
                    //if (iType == KKSAttrType::atParent)
                    //    qDebug () << __PRETTY_FUNCTION__ << tName << rid << av_str;
                }
                if (rid > 0 && iType != KKSAttrType::atParent)
                    val = KKSValue(QString::number (rid), cAttr->refType()->attrType());
                else if (iType == KKSAttrType::atParent && !av_str.isEmpty())
                    val = KKSValue (av_str, cAttr->refType()->attrType());
                else
                    val = KKSValue(QString (), cAttr->refType()->attrType());
            }
            else if (iType == KKSAttrType::atCheckList ||
                        iType == KKSAttrType::atCheckListEx)
            {
                QString av_str (oesList[i][j+1].mid(oesList[i][j+1].indexOf("{")+1, oesList[i][j+1].lastIndexOf("}")-1));
                QStringList rValues = av_str.split (",");
                for (int ii=0; ii<rValues.count(); ii++)
                {
                    if (rValues[ii].startsWith("\""))
                        rValues[ii] = rValues[ii].mid (1);
                    if (rValues[ii].endsWith("\""))
                        rValues[ii] = rValues[ii].mid (0, rValues[ii].size()-1);
                }

                QString vArr ("{");
                for (int ii=0; ii<rValues.count(); ii++){
                    QString a = rValues[ii];

                    a.replace(QString("\\\n"), QChar('\n'), Qt::CaseInsensitive);
                    a.replace(QString("\\\'"), QChar('\''), Qt::CaseInsensitive);
                    a.replace(QString("\\\""), QChar('\"'), Qt::CaseInsensitive);
                    a.replace(QString("\\,"), QChar(','), Qt::CaseInsensitive);
                
                    vArr += QString("'%1'%2").arg (rValues[ii])
                                           .arg (ii<rValues.count()-1 ? QString (",") : QString());
                }

                vArr += QString ("}");
                qDebug () << __PRETTY_FUNCTION__ << vArr;
                

                val = KKSValue (vArr, iType);
                //KSA rattr->release ();
                //KSA refObj->release ();
            }
            else if (iType == KKSAttrType::atJPG ||
                     iType == KKSAttrType::atSVG ||
                     iType == KKSAttrType::atVideo ||
                     iType == KKSAttrType::atXMLDoc)
            {
                val = KKSValue (QString("''"), iType);
            }
            else{
                QString av_str = oesList[i][j+1];
                
                av_str.replace(QString("\\\n"), QChar('\n'), Qt::CaseInsensitive);
                av_str.replace(QString("\\\'"), QChar('\''), Qt::CaseInsensitive);
                av_str.replace(QString("\\\""), QChar('\"'), Qt::CaseInsensitive);
                val = KKSValue (av_str, iType);
            }
            //if (iType == KKSAttrType :: atSVG)
                //qDebug () << __PRETTY_FUNCTION__ << cAttr->code (true) << val.value() << iType << val.isNull();
            
            KKSAttrValue *av = new KKSAttrValue (val, const_cast<KKSCategoryAttr*>(cAttr));
            if (av)
            {
                oe->addAttrValue (av);
                av->release ();
            }
            
        }

        oeList.append (oe);
        oe->release ();
    
    } //outer for

   
    pImportD->hide ();

    pImportD->setMaximum (0);
    pImportD->setCancelButton (0);
    lImport->setText (tr ("Write copies into database. Please wait..."));
    pImportD->setWindowTitle (tr("Save into DB"));
    pImportD->show ();
    QString tabName = tableName.isEmpty() ? io->tableName() : tableName;
    int res = eiof->insertEIOList (oeList, uidsSorted, oldUids, cat, tabName, pImportD, true);
    pImportD->hide ();
    if (res == ERROR_CODE)
    {
        qCritical() << tr("Cannot insert into table");
        QMessageBox::critical (oEditor, tr("Import copies"), tr("Cannot insert into table"), QMessageBox::Ok);
        return;
    }
//    pImportD->setValue (v++);

    const KKSTemplate * tChild = oEditor->recTemplate ();
    //tChild = new KKSTemplate (io->tableTemplate() != 0 ? *(io->tableTemplate()) : io->category()->tableCategory()->defTemplate());
    if (!tChild)
        return;
    KKSList<const KKSFilterGroup *> filters = oEditor->filters ();
    lImport->setText (tr ("Create view. Please wait..."));
    pImportD->setWindowTitle (tr("Update IO"));
    pImportD->show ();
    pImportD->setMinimumDuration (10000);
    int i = oEditor->tabEnclosures->currentIndex ();
    QTreeView * tv=0;
    if (i==0)
        tv = oEditor->getRecordsWidget()->getView();
    else if (i <= oEditor->addRecWidgets.count())
        tv = oEditor->addRecWidgets[i-1]->getView();

    KKSViewFactory::loadEIOEx (oEditor, io, loader, tChild, tv, filters, false, cat, tabName, pImportD);
    tChild->release ();
    pImportD->setParent (0);
    delete pImportD;
}

int KKSObjEditorFactory :: searchParents (const KKSList<KKSObjectExemplar *>& oeList, const KKSCategoryAttr *cAttr, const KKSValue& refVal)
{
    int pKey (-1);
    if (!cAttr || !cAttr->type() || cAttr->type()->attrType() != KKSAttrType::atParent || refVal.value().isEmpty())
        return pKey;
    for (int i=0; i<oeList.count() && pKey<0; i++)
    {
        KKSObjectExemplar * oe = oeList[i];
        if (!oe)
            continue;
        KKSAttrValue * av(0);// = oe->
        for (int ii=0; ii<oe->attrValues().count() && !av; ii++)
        {
            if (oe->attrValueIndex(ii) && oe->attrValueIndex(ii)->attribute() && cAttr->columnName(false) == oe->attrValueIndex(ii)->attribute()->code(false))
            {
                av = oe->attrValueIndex(ii);
                av->addRef();
            }
        }
        if (av && QString::compare (av->value().value(), refVal.value(), Qt::CaseInsensitive) == 0)
            pKey = i+1;//oe->id();
        if (av)
            av->release ();
    }
    return pKey;
}

/* Метод осуществляет экспорт содержимого ИО-справочника.
 * Параметры:
 * editor -- редактор ИО-справочника,
 * idObject -- идентификатор ИО
 * cat -- категория справочника, содержащего таблицу
 * tableName -- название таблицы
 */
void KKSObjEditorFactory :: exportEIO (KKSObjEditor * editor, int idObject, const KKSCategory * cat, QString tableName, QAbstractItemModel * recModel)
{
    KKSObject *io = loader->loadIO (idObject, true);
    if (!io)
        return;


    const KKSCategory * cMain = 0; //потом надо не забыть очистить под нее память
    //данная категория в дальнейшем передается в метод loadEIOList, в который должна передаваться подчиненная категория.
    //в настоящий момент времени cat - категория главная. поэтому используем ее подчиненную
    const KKSCategory * cChild = 0;
    
    if (cat && cat->tableCategory())
        cMain = new KKSCategory (*cat);
    else if (cat && cat->type()->id() == 10)//подчиненная категория
        return; //требуется в данном методе как информация о главной, так и о подчиненной
    else{
        if(!io->category()->tableCategory()) //если это даже не справочник то экспортировать нечего вообще
            return;

        cMain = new KKSCategory (*(io->category()));
    }

    if(!cMain || !cMain->tableCategory()){
        if(cMain)
            cMain->release();
        return;
    }

    cChild = cMain->tableCategory();

/*    int i = editor->tabEnclosures->currentIndex ();
    QTreeView * tv=0;
    if (i==0)
        tv = editor->recWidget->getView();
    else if (i <= editor->addRecWidgets.count())
        tv = editor->addRecWidgets[i-1]->getView();
 */

    KKSList<KKSObjectExemplar*> oeList;
    QList<int> idOe;
    this->getModelIds (recModel, QModelIndex(), idOe);
/*    for (int i=0; i<idOe.count(); i++)
    {
        int id = idOe[i];//wIndex.data (Qt::UserRole).toInt();
        KKSObjectExemplar * oe = loader->loadEIO (id, io, c0, tableName);
        if (!oe)
        {
            QMessageBox::warning (editor, tr ("Export IO"), tr ("Cannot load copy %1").arg (id), QMessageBox::Ok);
            io->release();
            return;
        }
        oeList.append (oe);
        oe->release ();
    }
 */
    Q_UNUSED (idOe);

    KKSList<KKSEIOData *> objEx = loader->loadEIOList1 (cChild, tableName, editor->filters(), io->id() <= _MAX_SYS_IO_ID_ ? true : false);//сюда передаем табличную (подчиненную) категорию
    KKSXMLForm *xmlForm = new KKSXMLForm (io, tr("Export IO %1").arg (io->name()), true, editor);
    if (!xmlForm)
    {
        if (io)
            io->release ();
        if(cMain)
            cMain->release();

        return;
    }
    /*int res = xmlForm->initExportData (objEx, c0);//io->category()->tableCategory());
    if (res == ERROR_CODE)
    {
        if (io)
            io->release ();
        xmlForm->setParent (0);
        delete xmlForm;
        return;
    }*/
    xmlForm->setExportModel (recModel);

    if (xmlForm->exec () == QDialog::Accepted)
    {
        QString csvFileName = xmlForm->getCSVFile ();
        QString xmlFileName = xmlForm->getXMLFile ();
        QString charSet = xmlForm->getCharset ();
        QString fDelim = xmlForm->getFieldDelimiter ();
        QString tDelim = xmlForm->getTextDelimiter ();
        QFile *fXml = new QFile (xmlFileName);
        fXml->open (QIODevice::WriteOnly);
        int res = this->exportHeader (fXml, cMain, charSet, fDelim, tDelim, editor);//Сюда передаем главную категорию
        fXml->close ();
        delete fXml;
        if (res == ERROR_CODE)
        {
            if (io)
                io->release ();

            if(cMain)
                cMain->release();
            xmlForm->setParent (0);
            delete xmlForm;

            return;
        }
        QFile fCSV (csvFileName);
        //res = this->exportCopies (&fCSV, cChild, oeList, charSet, fDelim, tDelim, editor, tableName);
        res = this->exportCopies (&fCSV, cChild, objEx, charSet, fDelim, tDelim, editor);//сюда передаем табличную (подчиненную) категорию
        if (res == ERROR_CODE)
        {
            if (io)
                io->release ();
            if(cMain)
                cMain->release();
            xmlForm->setParent (0);
            delete xmlForm;

            return;
        }
    }

    xmlForm->setParent (0);
    delete xmlForm;

    if (io)
        io->release ();

    if(cMain)
        cMain->release();
}

void KKSObjEditorFactory :: getModelIds (QAbstractItemModel * mod, const QModelIndex& wIndex, QList<int>& ids) const
{
    if (!mod)
        return;
    for (int i=0; i<mod->rowCount (wIndex); i++)
    {
        QModelIndex ind = mod->index(i, 0, wIndex);
        int id = mod->data (ind, Qt::UserRole).toInt();
        ids.append (id);
        getModelIds (mod, ind, ids);
    }
}

/* Вспомогательный метод экспорта колонок таблицы в xml-файл.
 */
int KKSObjEditorFactory :: exportHeader (QIODevice *xmlDev, // XML-файл, содержащий всю информацию
                                          const KKSCategory *c, // категория справочника, содержащего таблицу (io->category())
                                          QString codeName, // кодировка выходных данных
                                          QString fDelim, // разделитель полей
                                          QString tDelim, // разделитель текста
                                          KKSObjEditor *oEditor
                                          )
{
    if (!xmlDev)
    {
        qCritical() <<  tr ("Invalid device");
        QMessageBox::critical(oEditor, tr("XML file"), tr ("Invalid device"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    if (!c)
    {
        qCritical() << tr ("Invalid category");
        QMessageBox::critical(oEditor, tr("XML export"), tr ("Invalid category"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    bool isXmlOpen = xmlDev->isOpen ();
    if (!xmlDev->isOpen() && !xmlDev->open (QIODevice::WriteOnly))
    {
        qCritical() << tr("Cannot open output header file");
        QMessageBox::critical(oEditor, tr ("File open error"), tr("Cannot open output header file"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    QXmlStreamWriter * xmlWriter = new QXmlStreamWriter (xmlDev);
    if (!xmlWriter)
    {
        qCritical() << tr ("Cannot set xml writer");
        QMessageBox::critical(oEditor, tr ("Xml writer"), tr ("Cannot set xml writer"), QMessageBox::Ok);
        return ERROR_CODE;
    }
/*
#ifdef Q_OS_WIN32
    QTextCodec * xmlCodec = QTextCodec::codecForName ("Windows-1251");
    xmlWriter->setCodec (xmlCodec);
#endif
*/
    xmlWriter->writeStartDocument ();
    QString dtd = QString ("\n<!DOCTYPE Categories [");
    dtd += QString("\n <!ENTITY Charset '%1'> \n <!ENTITY field_delimiter '%2'> \n <!ENTITY text_delimiter '%3'> \n ").arg (codeName).arg (fDelim).arg (tDelim);
    dtd += QString("\n <!ELEMENT category (cname, ccode, ctype, cdescription, cis_main, id_child?, attributes)>");
    dtd += QString("\n <!ATTLIST category id ID #REQUIRED>");
    dtd += QString("\n <!ELEMENT cname (#PCDATA)>");
    dtd += QString("\n <!ELEMENT ccode (#PCDATA)>");
    dtd += QString("\n <!ELEMENT ctype (#PCDATA)>");
    dtd += QString("\n <!ELEMENT cdescription (#PCDATA)>");
    dtd += QString("\n <!ELEMENT cis_main (#PCDATA)>");
    dtd += QString("\n <!ELEMENT id_child (#PCDATA)>");
    dtd += QString("\n <!ELEMENT attributes (#PCDATA)>");
    dtd += QString("\n <!ATTLIST attributes number CDATA \"0\">\n");
    dtd += QString("\n <!ELEMENT attribute (code, name, type, mandatory, read_only, def_width, table_name?, column_name?, ref_column_name?)> \n <!ELEMENT code (#PCDATA)> \n <!ELEMENT name (#PCDATA)> \n <!ELEMENT type (#PCDATA)> \n <!ELEMENT mandatory (#PCDATA)> \n <!ELEMENT read_only (#PCDATA)>\n <!ELEMENT table_name (#PCDATA)> \n <!ELEMENT column_name (#PCDATA)> \n <!ELEMENT ref_column_name (#PCDATA)> \n");
    dtd += QString ("]>\n");
    xmlWriter->writeDTD (dtd);
    QString namespaceUri;

    xmlWriter->writeStartElement (namespaceUri, QString("Categories"));
    xmlWriter->writeCharacters (QString("\n"));
    int ierc = exportCategory (xmlWriter, c);
    const KKSCategory * ct = c->tableCategory();
    if (ierc < 0)
        return ERROR_CODE;
    int ierct = exportCategory (xmlWriter, ct);
    if (ierct < 0)
        return ERROR_CODE;

    xmlWriter->writeEndElement ();
    //
    // Categories
    //
    xmlWriter->writeEndDocument ();
    xmlWriter->setDevice (0);
    if (!isXmlOpen)
        xmlDev->close ();

//    delete xmlWriter;

    return OK_CODE;
}

int KKSObjEditorFactory :: exportCategory (QXmlStreamWriter * xmlWriter, const KKSCategory *c)
{
    if (!xmlWriter || !c)
        return ERROR_CODE;

    QString namespaceUri;
    QString catName = QString ("category");
    xmlWriter->writeStartElement(namespaceUri, catName);
    QString cid ("id");
    xmlWriter->writeAttribute(cid, QString::number (c->id()));
    xmlWriter->writeCharacters ("\n    ");
    xmlWriter->writeStartElement (QString("cname"));
    xmlWriter->writeCharacters (c->name());
    xmlWriter->writeEndElement ();
    xmlWriter->writeCharacters ("\n    ");
    xmlWriter->writeStartElement (QString("ccode"));
    xmlWriter->writeCharacters (c->code());
    xmlWriter->writeEndElement ();
    xmlWriter->writeCharacters ("\n    ");
    xmlWriter->writeStartElement (QString("ctype"));
    xmlWriter->writeCharacters (QString::number (c->type()->id()));
    xmlWriter->writeEndElement ();
    xmlWriter->writeCharacters ("\n    ");
    xmlWriter->writeStartElement (QString("cdescription"));
    xmlWriter->writeCharacters (c->desc());
    xmlWriter->writeEndElement ();
    xmlWriter->writeCharacters ("\n    ");
    xmlWriter->writeStartElement (QString("cis_main"));
    xmlWriter->writeCharacters (c->isMain() ? QString("true") : QString ("false"));
    xmlWriter->writeEndElement ();
    xmlWriter->writeCharacters ("\n    ");
    if (c->tableCategory())
    {
        xmlWriter->writeStartElement (QString("id_child"));
        xmlWriter->writeCharacters (QString::number (c->tableCategory()->id()));
        xmlWriter->writeEndElement ();
        xmlWriter->writeCharacters ("\n    ");
    }
    QString attrsName = QString ("attributes");
    xmlWriter->writeStartElement (attrsName);
    xmlWriter->writeAttribute (QString ("number"), QString::number (c->attributes().count()));
    xmlWriter->writeCharacters ("\n    ");

    KKSMap<int, KKSCategoryAttr*>::const_iterator pc;
    int i=0;
    for (pc = c->attributes().constBegin(); pc != c->attributes().constEnd(); pc++)
    {
        QString fieldName = QString ("attribute");
        QString acode = QString ("code");
        QString atitle = QString ("name");
        QString atype = QString ("type");
        QString aMandatory = QString ("mandatory");
        QString aReadOnly = QString ("read_only");
        QString attrId = QString ("id");
        QString attrTableName = QString ("table_name");
        QString attrColumnName = QString ("column_name");
        QString attrRefColumnName = QString ("ref_column_name");
        QString attrDefW = QString ("def_width");

        xmlWriter->writeCharacters ("    ");
        xmlWriter->writeStartElement (fieldName);
        xmlWriter->writeAttribute (attrId, QString::number(pc.key()));
        xmlWriter->writeCharacters ("\n            ");

        xmlWriter->writeStartElement (acode);
        xmlWriter->writeCharacters (pc.value()->code());
        xmlWriter->writeEndElement ();
        xmlWriter->writeCharacters ("\n            ");

        xmlWriter->writeStartElement (atitle);
        xmlWriter->writeCharacters (pc.value()->title());
        xmlWriter->writeEndElement ();
        xmlWriter->writeCharacters ("\n            ");

        xmlWriter->writeStartElement (atype);
        xmlWriter->writeCharacters (QString::number (pc.value()->type()->attrType()));
        xmlWriter->writeEndElement ();
        xmlWriter->writeCharacters ("\n            ");
        
        xmlWriter->writeStartElement (aMandatory);
        xmlWriter->writeCharacters(pc.value()->isMandatory() ? QString("true") : QString("false"));
        xmlWriter->writeEndElement ();
        xmlWriter->writeCharacters ("\n            ");

        xmlWriter->writeStartElement (aReadOnly);
        xmlWriter->writeCharacters(pc.value()->isReadOnly() ? QString("true") : QString("false"));
        xmlWriter->writeEndElement ();
        xmlWriter->writeCharacters ("\n            ");
        
        xmlWriter->writeStartElement (attrDefW);
        xmlWriter->writeCharacters (QString::number (pc.value()->defWidth()));
        xmlWriter->writeEndElement ();
        
        if (pc.value()->type()->attrType() == KKSAttrType::atCheckList ||
            pc.value()->type()->attrType() == KKSAttrType::atCheckListEx ||
            pc.value()->type()->attrType() == KKSAttrType::atList ||
            pc.value()->type()->attrType() == KKSAttrType::atParent ||
            pc.value()->type()->attrType() == KKSAttrType::atRecordColorRef ||
            pc.value()->type()->attrType() == KKSAttrType::atRecordTextColorRef)
        {
            xmlWriter->writeCharacters ("\n            ");
            xmlWriter->writeStartElement (attrTableName);
            xmlWriter->writeCharacters (pc.value()->tableName());
            xmlWriter->writeEndElement ();
            xmlWriter->writeCharacters ("\n            ");
            xmlWriter->writeStartElement (attrColumnName);
            xmlWriter->writeCharacters (pc.value()->columnName());
            xmlWriter->writeEndElement ();
            xmlWriter->writeCharacters ("\n            ");
            xmlWriter->writeStartElement (attrRefColumnName);
            xmlWriter->writeCharacters (pc.value()->refColumnName(false));
            xmlWriter->writeEndElement ();
            xmlWriter->writeCharacters ("\n        ");
        }
        else
            xmlWriter->writeCharacters ("\n        ");
        
        xmlWriter->writeEndElement ();
        //
        // Field
        //
        i++;
        //if (i != c->attributes().count())
        xmlWriter->writeCharacters ("\n    ");
        //else
        //    xmlWriter->writeCharacters ("\n    ");
    }

    xmlWriter->writeEndElement ();
    xmlWriter->writeCharacters ("\n");
    xmlWriter->writeEndElement ();
    xmlWriter->writeCharacters ("\n");
    return OK_CODE;
}

/* Вывод ЭИО в файл. Параметры
 * csvDev -- устройство вывода
 * c -- табличная категория
 * oeList -- список ЭИО
 * codeName -- кодировка
 * fDelim -- разделитель полей
 * tDelim -- разделитель текста
 * oEditor -- родительский редактор
 */
int KKSObjEditorFactory :: exportCopies (QIODevice *csvDev, // целевой CSV файл
                                         const KKSCategory *c,//подчиненная категория
                                         const KKSList<KKSObjectExemplar *>& oeList,
                                         QString codeName, // кодировка выходных данных
                                         QString fDelim, // разделитель полей
                                         QString tDelim, // разделитель текста
                                         KKSObjEditor *oEditor, // родительский редактор ИО и ЭИО
                                         QString tableName
                                         )
{
    Q_UNUSED (tableName);
    if (!csvDev)
    {
        qCritical() << tr ("Invalid device");
        QMessageBox::critical(oEditor, tr("CSV file"), tr ("Invalid device"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    if (!c)
    {
        qCritical() << tr ("Invalid category");
        QMessageBox::critical (oEditor, tr("CSV export"), tr ("Invalid category"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    if (!csvDev->isOpen() && !csvDev->open (QIODevice::WriteOnly))
    {
        qCritical() << tr("Cannot open output file");
        QMessageBox::critical (oEditor, tr ("File open error"), tr("Cannot open output file"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    QTextStream csvFile (csvDev);
    QTextCodec *csvCodec = QTextCodec::codecForName (codeName.toAscii());
    if (!csvCodec)
    {
        qCritical() << tr ("Invalid encoding %1").arg (codeName);
        QMessageBox::critical(oEditor, tr ("CSV export"), tr ("Invalid encoding %1").arg (codeName), QMessageBox::Ok);
        return ERROR_CODE;
    }
    csvFile.setCodec (csvCodec);

    KKSTemplate *t = new KKSTemplate (c->defTemplate());
    if (!t)
        return ERROR_CODE;

    QList<KKSAttrView*> attrs_list = t->sortedAttrs();
    t->release ();
    for (int i=0; i<oeList.count(); i++)
    {
        if (!oeList[i])
            continue;

        QString fstr;
        QTextStream oeStream (&fstr, QIODevice::WriteOnly);
        //
        // TODO: вывод unique_id
        //
        KKSMap<int, KKSCategoryAttr*>::const_iterator pc;
        int j=0;
        for (pc = c->attributes().constBegin(); pc != c->attributes().constEnd(); pc++)
        {
            switch (pc.key())
            {
                case 1: oeStream << oeList[i]->id (); break;
                case 2: 
                       {
                           QString av_str = oeList[i]->name();
                           av_str.replace (QChar('\n'), QString("\\n"), Qt::CaseInsensitive);
                           av_str.replace (QChar('\''), QString("\\'"), Qt::CaseInsensitive);
                           av_str.replace (tDelim, QString("\\%1").arg (tDelim), Qt::CaseInsensitive);
                           oeStream << tDelim << av_str << tDelim; 
                       }
                    break;
                case 3: 
                       {
                           QString av_str = oeList[i]->desc();
                           av_str.replace (QChar('\n'), QString("\\n"), Qt::CaseInsensitive);
                           av_str.replace (QChar('\''), QString("\\'"), Qt::CaseInsensitive);
                           av_str.replace (tDelim, QString("\\%1").arg (tDelim), Qt::CaseInsensitive);
                           oeStream << tDelim << av_str << tDelim; 
                       }
                    break;
                case 6: 
                       {
                           QString av_str = oeList[i]->code();
                           av_str.replace (QChar('\n'), QString("\\n"), Qt::CaseInsensitive);
                           av_str.replace (QChar('\''), QString("\\'"), Qt::CaseInsensitive);
                           av_str.replace (tDelim, QString("\\%1").arg (tDelim), Qt::CaseInsensitive);
                           oeStream << tDelim << av_str << tDelim; 
                       }
                    break;
                default:
                        {
                            KKSAttrValue * av = oeList[i]->attrValueIndex (j);//pc.key());
                            if (pc.value()->type()->attrType() == KKSAttrType::atDouble || 
                                pc.value()->type()->attrType() == KKSAttrType::atInt || 
                                pc.value()->type()->attrType() == KKSAttrType::atBool
                                )
                            {
                                qDebug () << __PRETTY_FUNCTION__ << (av ? av->value().value () : QString());
                                oeStream << (av ? av->value().value() : QString());
                            }
                            else if (pc.value()->type()->attrType() == KKSAttrType::atList || 
                                     pc.value()->type()->attrType() == KKSAttrType::atParent ||
                                     pc.value()->type()->attrType() == KKSAttrType::atRecordColorRef ||
                                     pc.value()->type()->attrType() == KKSAttrType::atRecordTextColorRef)
                            {
                                KKSAttrValue * av = oeList[i]->attrValueIndex (j);//pc.key());
                                if (!av)
                                {
                                    oeStream << QString();
                                    continue;
                                }
                                if (pc.value()->type()->attrType() != KKSAttrType::atParent)
                                {

                                }
                                else
                                {
                                }
                                QString av_str (av->value().columnValue());
                                if (!av_str.isEmpty ())
                                {
                                    av_str.replace (QChar('\n'), QString("\\n"), Qt::CaseInsensitive);
                                    av_str.replace (QChar('\''), QString("\\'"), Qt::CaseInsensitive);
                                    av_str.replace (tDelim, QString("\\%1").arg (tDelim), Qt::CaseInsensitive);
                                }
                                oeStream << tDelim << av_str << tDelim;

                            }
                            else if (pc.value()->type()->attrType() == KKSAttrType::atCheckList ||
                                     pc.value()->type()->attrType() == KKSAttrType::atCheckListEx)
                            {
                                KKSAttrValue * av = oeList[i]->attrValueIndex (j);//pc.key());
                                if (!av)
                                {
                                    oeStream << QString();
                                    continue;
                                }
                                QMap<int, QString> values;
                                QMap<int, QString> refColumnValues;
                                QString tName = av->attribute()->tableName ();
                                KKSObject * refObj = loader->loadIO (tName, true);
                                if (!refObj)
                                    continue;

                                KKSCategory * cRef = refObj->category();
                                if (!cRef)
                                {
                                    refObj->release ();
                                    continue;
                                }
                                bool isXml = false;
                                cRef = cRef->tableCategory();
                                if (cRef)
                                    isXml = isXml || cRef->isAttrTypeContains(KKSAttrType::atXMLDoc) || cRef->isAttrTypeContains (KKSAttrType::atSVG);
                                refObj->release ();

                                values = loader->loadAttributeValues (av->attribute(), 
                                                                      refColumnValues,
                                                                      isXml, 
                                                                      !isXml, 
                                                                      QString::null, 
                                                                      KKSList<const KKSFilterGroup*>());

                                QVariant cV = av->value().valueVariant();
                                QList<int> iVals;
                                QStringList sVals;
                                if (av->attribute()->refColumnName().isEmpty() || 
                                    av->attribute()->refColumnName() == "id")
                                {
                                    //bool ok = false;
                                    sVals = cV.toStringList();//toInt(&ok);
                                    for (int ii=0; ii<sVals.count(); ii++)
                                        iVals.append (sVals[ii].toInt());
                                }
                                else{
                                    sVals = cV.toStringList();
                                    for (int ii=0; ii<sVals.count(); ii++)
                                    {
                                        int id = refColumnValues.key(sVals[ii]);
                                        if (id > 0)
                                            iVals.append (id);
                                    }
                                }
                                qDebug () << __PRETTY_FUNCTION__ << iVals << cV;

                                oeStream << QString("{");
                                for (int ii=0; ii<iVals.count(); ii++)
                                {
                                    QMap<int, QString>::const_iterator pv = values.constFind (iVals[ii]);
                                    if (pv != values.constEnd())
                                    {
                                        QString av_str (pv.value());
                                        if (!av_str.isEmpty ())
                                        {
                                            av_str.replace (QChar('\n'), QString("\\n"), Qt::CaseInsensitive);
                                            av_str.replace (QChar('\''), QString("\\'"), Qt::CaseInsensitive);
                                            av_str.replace (tDelim, QString("\\%1").arg (tDelim), Qt::CaseInsensitive);
                                        }
                                        oeStream << tDelim << av_str << tDelim;
                                    }
                                    else
                                        oeStream << tDelim << QString() << tDelim;
                                    if (ii<iVals.count()-1)
                                        oeStream << fDelim;
                                }
                                oeStream << QString("}");
                            }
                            else
                            {
                                QString av_str = (av ? av->value().value() : QString());
                                if (!av_str.isEmpty ())
                                {
                                    av_str.replace (QChar('\n'), QString("\\n"), Qt::CaseInsensitive);
                                    av_str.replace (QChar('\''), QString("\\'"), Qt::CaseInsensitive);
                                    av_str.replace (tDelim, QString("\\%1").arg (tDelim), Qt::CaseInsensitive);
                                }
                                qDebug () << __PRETTY_FUNCTION__ << tDelim << av_str << tDelim;
                                oeStream << tDelim << av_str << tDelim;
                            }
                        }
            }
            j++;
            if (j < c->attributes().count())
                oeStream << fDelim;
        }
        csvFile << fstr << '\n';
    }

    csvDev->close ();
    return OK_CODE;
}

/* Вывод ЭИО в файл. Параметры
 * csvDev -- устройство вывода
 * c -- табличная категория
 * oeData -- список данных ЭИО
 * codeName -- кодировка
 * fDelim -- разделитель полей
 * tDelim -- разделитель текста
 * oEditor -- родительский редактор
 */
int KKSObjEditorFactory :: exportCopies (QIODevice *csvDev, // целевой CSV файл
                                         const KKSCategory *c, //подчиненная категория
                                         const KKSList<KKSEIOData *>& oeData,
                                         QString codeName, // кодировка выходных данных
                                         QString fDelim, // разделитель полей
                                         QString tDelim, // разделитель текста
                                         KKSObjEditor *oEditor // родительский редактор ИО и ЭИО
                                        )
{
    if (!csvDev)
    {
        qCritical() << tr ("Invalid device");
        QMessageBox::critical(oEditor, tr("CSV file"), tr ("Invalid device"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    if (!c)
    {
        qCritical() << tr ("Invalid category");
        QMessageBox::critical (oEditor, tr("CSV export"), tr ("Invalid category"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    if (!csvDev->isOpen() && !csvDev->open (QIODevice::WriteOnly))
    {
        qCritical() << tr("Cannot open output file");
        QMessageBox::critical (oEditor, tr ("File open error"), tr("Cannot open output file"), QMessageBox::Ok);
        return ERROR_CODE;
    }

    QTextStream csvFile (csvDev);
    QTextCodec *csvCodec = QTextCodec::codecForName (codeName.toAscii());
    if (!csvCodec)
    {
        qCritical() << tr ("Invalid encoding %1").arg (codeName);
        QMessageBox::critical (oEditor, tr ("CSV export"), tr ("Invalid encoding %1").arg (codeName), QMessageBox::Ok);
        return ERROR_CODE;
    }
    csvFile.setCodec (csvCodec);

    KKSTemplate *t = new KKSTemplate (c->defTemplate());
    if (!t)
        return ERROR_CODE;

    QList<KKSAttrView*> attrs_list = t->sortedAttrs();
    t->release ();

    int noe = oeData.size();
    for (int i = 0; i < noe; i++)
    {
        KKSEIOData * d = oeData[i];
        QString fstr;
        QTextStream oeStream (&fstr, QIODevice::WriteOnly);
        oeStream << tDelim << d->sysFieldValue("unique_id") << tDelim;

        for (int ii=0; ii<attrs_list.count(); ii++)
        {
            KKSAttrView * v = attrs_list [ii];
            if (v->id() == 1)
            {
                oeStream << fDelim << d->sysFieldValue("id").toInt() << (attrs_list.count() > 1 ? fDelim : QString());
                continue;
            }
            QString attrCode = v->code();
            QString attrValue = d->fields().value(attrCode);
            KKSAttrType::KKSAttrTypes aType = v->type()->attrType();
            switch (aType)
            {
                case KKSAttrType::atDouble:
                case KKSAttrType::atInt:
                {
                    oeStream << attrValue;
                    break;
                }
                case KKSAttrType::atList:
                case KKSAttrType::atParent:
                case KKSAttrType::atRecordColorRef:
                case KKSAttrType::atRecordTextColorRef:
                {
                    qDebug () << __PRETTY_FUNCTION__ << d->fields() << d->sysFields();
                    oeStream << tDelim << d->sysFields().value (attrCode+"_uid") << tDelim;
                    break;
                }
                default:
                    oeStream << tDelim << attrValue << tDelim;
            }

            if (ii < attrs_list.count()-1)
                oeStream << fDelim;
        }
        csvFile << fstr << "\n";
    }

    csvDev->close ();
    return OK_CODE;
}

/* Инициализация действий редактора ИО(ЭИО).
 * Параметры:
 * tBar -- тулбар с набором действий
 * editor -- родительский редактор
 */
void KKSObjEditorFactory :: initActions (QToolBar *tBar, KKSObjEditor *editor)
{
    tBar->setIconSize (QSize (24, 24));
    tBar->setToolButtonStyle (Qt::ToolButtonTextBesideIcon);
    QAction *aSaveIO = new QAction (QIcon(":/ddoc/io_save.png"), tr ("&Save"), editor);
    QKeySequence kSave (QKeySequence::Save);
    aSaveIO->setShortcut (kSave);
    tBar->addAction (aSaveIO);
    connect (aSaveIO, SIGNAL (triggered()), editor, SLOT (apply()) );

    QAction *aSaveCloseIO = new QAction (QIcon(":/ddoc/io_save_close.png"), tr ("Sa&ve and close"), editor);
    QKeySequence kSaveClose (Qt::ALT+Qt::Key_V);
    aSaveCloseIO->setShortcut (kSaveClose);
    tBar->addAction (aSaveCloseIO);
    connect (aSaveCloseIO, SIGNAL (triggered()), editor, SLOT (accept()) );

    QAction *aCloseIO = new QAction (QIcon(":/ddoc/io_close.png"), tr ("&Close"), editor);
    tBar->addAction (aCloseIO);
    connect (aCloseIO, SIGNAL (triggered()), editor, SLOT (close()) );

    tBar->addSeparator ();

    QAction *aViewAttrs = new QAction (QIcon(":/ddoc/apply_template.png"), tr ("Set V&iew"), editor);
    tBar->addAction (aViewAttrs);
    QList<QKeySequence> kvList;
    QKeySequence kView (Qt::Key_F3);
    QKeySequence kView1 (Qt::ALT+Qt::Key_I);
    kvList.append (kView1);
    kvList.append (kView);
    aViewAttrs->setShortcuts (kvList);
    connect (aViewAttrs, SIGNAL (triggered()), editor, SLOT (setAttrView()) );

    if (editor->getObj() )
    {
        QAction *aSendIO = new QAction (QIcon(":/ddoc/send_as_mail.png"), tr ("Send"), editor);
        QKeySequence kSend (Qt::CTRL+Qt::Key_Return);
        tBar->addAction (aSendIO);
        aSendIO->setShortcut (kSend);
        connect (aSendIO, SIGNAL (triggered()), editor, SLOT (sendIO()) );
    }

    tBar->addSeparator ();

    if (editor->showExecButton())
    {
/*        QWidget * wExec = new QWidget (tBar, Qt::Tool);
        QToolButton * tbExecW = new QToolButton (wExec);//, Qt::Tool);
        tbExecW->setIconSize (QSize (24, 24));
        tbExecW->setToolButtonStyle (Qt::ToolButtonTextBesideIcon);
        QHBoxLayout *hExecLay = new QHBoxLayout ();
        wExec->setLayout (hExecLay);
        hExecLay->addStretch ();*/
        QAction *aExecCommand = new QAction (QIcon(":/ddoc/exec_cmd.png"), tr ("Execute command"), editor);
        //tbExecW->setDefaultAction (aExecCommand);
        //hExecLay->addWidget (tbExecW);
        tBar->addAction (aExecCommand);//addWidget (wExec);
        connect (aExecCommand, SIGNAL (triggered()), editor, SLOT (saveAsCommandResult()) );
    }
}

/* Метод осуществляет отправку сообщения должностному лицу(ам)
 */
void KKSObjEditorFactory :: sendIO (KKSObject *wObj, KKSObjectExemplar *wObjE, KKSObjEditor *editor)
{
    //if (wObj && wObjE)
    //    qDebug () << __PRETTY_FUNCTION__ << wObj->id() << wObjE->id ();

    int idObj = -1;
    int idAuthor = -1;
    if (wObj && wObj->id() != IO_IO_ID){ //нажали на кнопку в самом редакторе отправляемого ИО
        idObj = wObj->id();
        idAuthor = wObj->author();
    }
    else if (wObjE && wObjE->id () != IO_IO_ID){
        idObj = wObjE->id ();
        idAuthor = -1;//мы не знаем здесь автора (отсылка из справочника ИО)
    }
    else if (editor && editor->getRecordsWidget()){//здесь автор тоже неизвестен
        idObj = editor->getID();
        idAuthor = -1;
    }
    else
        return;

    int idUser = loader->getUserId();
    
    if(idUser != idAuthor && loader->getPrivilege(idUser, idObj, 5, true) == false){
        qCritical() << tr("You does not have permissions for that operation!");
        QMessageBox::critical(editor, tr("Access error!"), tr("You does not have permissions for that operation!"), QMessageBox::Ok);
        return;
    }


    KKSRecWidget *rwDls = new KKSRecWidget (false);
    QTreeView *tvPos = rwDls->getView();//new QTreeView ();
    tvPos->setSelectionMode (QAbstractItemView::ExtendedSelection);
    rwDls->hideToolBar (); 
//    rwDls->hideAllButtons ();
    KKSObject * ioDls = loader->loadIO (IO_POS_ID, true);
    if (!ioDls)
    {
        delete tvPos;
        delete rwDls;
        return;
    }

    KKSTemplate *tDls = new KKSTemplate (ioDls->category()->tableCategory()->defTemplate());
    if (!tDls)
    {
        ioDls->release ();
        delete tvPos;
        delete rwDls;
        return;
    }
    KKSList<const KKSFilterGroup *>  filterGroups;
    tvPos->header()->setClickable (true);
    tvPos->header()->setSortIndicatorShown (true);
    tvPos->header()->setSortIndicator (0, Qt::AscendingOrder);
    tvPos->setSortingEnabled (true);
    KKSItemDelegate *itemDelegD = new KKSItemDelegate ();
    tvPos->setItemDelegate (itemDelegD);
    KKSViewFactory::loadEIOEx (0, ioDls, loader, tDls, tvPos, filterGroups, true, 0, QString(), 0, true);

    KKSRecWidget *rwIOs = new KKSRecWidget (false);
    QTreeView *tvIOs = rwIOs->getView();//new QTreeView ();
    tvIOs->setSelectionMode (QAbstractItemView::SingleSelection);
    tvIOs->header()->setClickable (true);
    tvIOs->header()->setSortIndicatorShown (true);
    tvIOs->header()->setSortIndicator (0, Qt::AscendingOrder);
    tvIOs->setSortingEnabled (true);
    KKSItemDelegate *itemDelegIO = new KKSItemDelegate ();
    tvIOs->setItemDelegate (itemDelegIO);
    KKSObject * io = loader->loadIO (IO_IO_ID, true);
    if (!io)
    {
        tDls->release ();
        ioDls->release ();
        delete rwIOs;
        delete rwDls;
    }

    KKSCategory * c = io->category()->tableCategory();
    KKSTemplate *tio = c ? new KKSTemplate (c->defTemplate()) : 0;
    if (!tio || !c)
    {
        if (c)
            c->release ();
        io->release ();
        tDls->release ();
        ioDls->release ();
        delete rwIOs;
        delete rwDls;
    }

    if (wObj || wObjE)
    {
        KKSList<const KKSFilter *> filters;
        KKSFilter * filter = c->createFilter (ATTR_ID, QString::number (idObj)/*(wObj ? wObj->id() : wObjE->id()))*/, KKSFilter::foEq);
        filters.append (filter);
        filter->release ();
        
        KKSFilterGroup * fg = new KKSFilterGroup(true);
        fg->setFilters(filters);
        filterGroups.append (fg);
        fg->release();
    }

    KKSViewFactory::loadEIOEx (0, io, loader, tio, tvIOs, filterGroups, true);

    rwIOs->hideAllButtons ();
    rwIOs->showEditGroup ();
    KKSMessageWidget * messDial = new KKSMessageWidget (rwDls, rwIOs, editor);
    connect (messDial, SIGNAL (addDocument()), this, SLOT (addSendIO()) );

    if (messDial->exec() == QDialog::Accepted)
    {
        QList<int> idDls = messDial->getDlsList ();
        QList<int> idIOs = messDial->getAttachments ();
        QString messText = messDial->message ();
        qDebug () << __PRETTY_FUNCTION__ << idDls << idIOs << messText;
        for (int i=0; i<idDls.count(); i++)
        {
            if (!idIOs.isEmpty())
            {
                for (int j=0; j<idIOs.count(); j++)
                {
                    QString ioSql = QString ("select * from msginsertout (%1, %2, %3, 1)")
                                            .arg (idIOs[j])
                                            .arg (QString ("'%1'").arg (messText))
                                            .arg (idDls[i]);
                    KKSResult *res = loader->getDb()->execute (ioSql);
                    if (!res || res->getRowCount () != 1 || res->getCellAsInt(0, 0) <= 0)
                    {
                        qCritical() << tr ("Cannot send message to %1").arg (idDls[i]);
                        QMessageBox::critical(messDial, tr("Send message"), tr ("Cannot send message to %1").arg (idDls[i]), QMessageBox::Ok);
                        continue;
                    }
                    if (res)
                        delete res;
                }
            }
            else
            {
                QString messSql = QString ("select * from msginsertout (NULL::integer, %1, %2, 1)")
                                          .arg (QString ("'%1'").arg (messText))
                                          .arg (idDls[i]);
                KKSResult * res = loader->getDb()->execute (messSql);
                if (!res || res->getRowCount () != 1 || res->getCellAsInt(0, 0) <= 0)
                {
                    qCritical() << tr ("Cannot send message to %1").arg (idDls[i]);
                    QMessageBox::critical (messDial, tr("Send message"), tr ("Cannot send message to %1").arg (idDls[i]), QMessageBox::Ok);
                    continue;
                }
                if (res)
                    delete res;
            }
        }
    }
    delete messDial;

    tio->release ();
    io->release ();
    tDls->release ();
    ioDls->release ();
}

/* Метод осуществляет рассылку документов должностному (ым) лицу (ам).
 */
void KKSObjEditorFactory :: sendIOList (const QList<int>& idIOList)
{
    int idUser = loader->getUserId();

    KKSRecWidget *rwDls = new KKSRecWidget (false);
    QTreeView *tvPos = rwDls->getView();//new QTreeView ();
    tvPos->setSelectionMode (QAbstractItemView::ExtendedSelection);
    rwDls->hideToolBar (); 
//    rwDls->hideAllButtons ();
    KKSObject * ioDls = loader->loadIO (IO_POS_ID, true);
    if (!ioDls)
    {
        delete tvPos;
        delete rwDls;
        return;
    }

    KKSTemplate *tDls = new KKSTemplate (ioDls->category()->tableCategory()->defTemplate());
    if (!tDls)
    {
        ioDls->release ();
        delete tvPos;
        delete rwDls;
        return;
    }
    KKSList<const KKSFilterGroup *>  filterGroups;
    tvPos->header()->setClickable (true);
    tvPos->header()->setSortIndicatorShown (true);
    tvPos->header()->setSortIndicator (0, Qt::AscendingOrder);
    tvPos->setSortingEnabled (true);
    KKSItemDelegate *itemDelegD = new KKSItemDelegate ();
    tvPos->setItemDelegate (itemDelegD);
    KKSViewFactory::loadEIOEx (0, ioDls, loader, tDls, tvPos, filterGroups, true, 0, QString(), 0, true);

    KKSRecWidget *rwIOs = new KKSRecWidget (false);
    QTreeView *tvIOs = rwIOs->getView();//new QTreeView ();
    tvIOs->setSelectionMode (QAbstractItemView::SingleSelection);
    tvIOs->header()->setClickable (true);
    tvIOs->header()->setSortIndicatorShown (true);
    tvIOs->header()->setSortIndicator (0, Qt::AscendingOrder);
    tvIOs->setSortingEnabled (true);
    KKSItemDelegate *itemDelegIO = new KKSItemDelegate ();
    tvIOs->setItemDelegate (itemDelegIO);
    for (int i=0; i<idIOList.count(); i++)
    {
        KKSObject * wObj = loader->loadIO (idIOList[i]);
        if (!wObj)
            continue;
        int idObj (idIOList[i]);
        int idAuthor (-1);
        if (wObj && wObj->id() != IO_IO_ID)
        {
            idAuthor = wObj->author();
        }
        if (idUser != idAuthor && !loader->getPrivilege(idUser, idObj, 5, true))
        {
            qCritical() << tr("You does not have permissions for send document %1 !").arg (idObj);
            QMessageBox::critical(0, tr("Access error!"), tr("You does not have permissions for send document %1 !").arg (idObj), QMessageBox::Ok);
            return;
        }
        wObj->release ();
    }
    KKSObject * io = loader->loadIO (IO_IO_ID, true);
    if (!io)
    {
        tDls->release ();
        ioDls->release ();
        delete rwIOs;
        delete rwDls;
    }

    KKSCategory * c = io->category()->tableCategory();
    KKSTemplate *tio = c ? new KKSTemplate (c->defTemplate()) : 0;
    if (!tio || !c)
    {
        if (c)
            c->release ();
        io->release ();
        tDls->release ();
        ioDls->release ();
        delete rwIOs;
        delete rwDls;
    }

    KKSList<const KKSFilter *> filters;
    for (int i=0; i<idIOList.count(); i++)
    {
        KKSFilter * filter = c->createFilter (ATTR_ID, QString::number (idIOList[i]), KKSFilter::foEq);
        filters.append (filter);
        filter->release ();
    }

    KKSFilterGroup * fg = new KKSFilterGroup(false);
    fg->setFilters(filters);
    filterGroups.append (fg);
    fg->release();

    KKSViewFactory::loadEIOEx (0, io, loader, tio, tvIOs, filterGroups, true);

    rwIOs->hideAllButtons ();
    rwIOs->showEditGroup ();
    KKSMessageWidget * messDial = new KKSMessageWidget (rwDls, rwIOs, 0);
    connect (messDial, SIGNAL (addDocument()), this, SLOT (addSendIO()) );

    if (messDial->exec() == QDialog::Accepted)
    {
        QList<int> idDls = messDial->getDlsList ();
        QList<int> idIOs = messDial->getAttachments ();
        QString messText = messDial->message ();
        qDebug () << __PRETTY_FUNCTION__ << idDls << idIOs << messText;
        for (int i=0; i<idDls.count(); i++)
        {
            if (!idIOs.isEmpty())
            {
                for (int j=0; j<idIOs.count(); j++)
                {
                    QString ioSql = QString ("select * from msginsertout (%1, %2, %3, 1)")
                                            .arg (idIOs[j])
                                            .arg (QString ("'%1'").arg (messText))
                                            .arg (idDls[i]);
                    KKSResult *res = loader->getDb()->execute (ioSql);
                    if (!res || res->getRowCount () != 1 || res->getCellAsInt(0, 0) <= 0)
                    {
                        qCritical() << tr ("Cannot send message to %1").arg (idDls[i]);
                        QMessageBox::critical (messDial, tr("Send message"), tr ("Cannot send message to %1").arg (idDls[i]), QMessageBox::Ok);
                        continue;
                    }
                    if (res)
                        delete res;
                }
            }
            else
            {
                QString messSql = QString ("select * from msginsertout (NULL::integer, %1, %2, 1)")
                                          .arg (QString ("'%1'").arg (messText))
                                          .arg (idDls[i]);
                KKSResult * res = loader->getDb()->execute (messSql);
                if (!res || res->getRowCount () != 1 || res->getCellAsInt(0, 0) <= 0)
                {
                    qCritical() << tr ("Cannot send message to %1").arg (idDls[i]);
                    QMessageBox::critical (messDial, tr("Send message"), tr ("Cannot send message to %1").arg (idDls[i]), QMessageBox::Ok);
                    continue;
                }
                if (res)
                    delete res;
            }
        }
    }
    delete messDial;
}

/* Метод добавляет новые ИО к сообщению(ям).
 */
void KKSObjEditorFactory :: addSendIO (void)
{
    if (qobject_cast<KKSMessageWidget *>(this->sender()) == 0)
        return;

    KKSMessageWidget *messDial = qobject_cast<KKSMessageWidget *>(this->sender());
    QList<int> idIOs = messDial->getAttachments ();

    KKSObject * io = loader->loadIO (IO_IO_ID, true);
    if (!io)
        return;

    KKSCategory *c = io->category ()->tableCategory ();
    if (!c)
    {
        io->release();
        return;
    }
    KKSList<const KKSFilterGroup *>  filterGroup;

    KKSFilterGroup * fg = new KKSFilterGroup (true);
    KKSList<const KKSFilter *> filters;

    for (int i=0; i<idIOs.count(); i++)
    {
        KKSFilter * f = c->createFilter (ATTR_ID, QString::number (idIOs[i]), KKSFilter::foNotEq);
        filters.append (f);
    }
   
    fg->setFilters (filters);
    filterGroup.append (fg);

    fg->release ();

    KKSRecDialog * editor = this->createObjRecEditor (IO_IO_ID,
                                                      IO_IO_ID,
                                                      filterGroup,
                                                      "",
                                                      io->category(),
                                                      true, false,
                                                      Qt::WindowModal,
                                                      messDial,
                                                      Qt::Dialog);

    if (qobject_cast<KKSObjEditor *>(editor) && editor->exec() == QDialog::Accepted && editor->getID () > 0)
    {
        qDebug () << __PRETTY_FUNCTION__ << editor->getID();
        QAbstractItemModel *sAttModel = messDial->getAttachmentsModel ();
        sAttModel->insertRows (sAttModel->rowCount(), 1);
        QModelIndex cInd = sAttModel->index (sAttModel->rowCount()-1, 0);
        KKSTemplate * t = new KKSTemplate (c->defTemplate());
        if (t)
        {
            KKSObjEditor * oEditor = qobject_cast<KKSObjEditor *>(editor);
            KKSMap<qint64, KKSObjectExemplar *> recs;
            recs.insert(editor->getID(), oEditor->getObjectEx());
            KKSViewFactory::updateEIOEx (loader, io, recs, t, sAttModel);
            t->release ();
        }
    }
    if (editor)
    {
        editor->setParent (0);
        delete editor;
    }
    io->release ();
}

/* Метод загружает новый периодический доклад.
 * Параметры :
 * idObjE -- идентификатор ЭИО в ТСД.
 * parent, f -- флаги QT
 */
void KKSObjEditorFactory :: insertReport (qint64 idObjE, QWidget *parent, Qt::WindowFlags f)
{
    bool isCreate (idObjE <= 0);

    KKSObject * prIO = loader->loadIO (IO_TSD_ID, true);
    if (!prIO)
        return;
    QString title = (isCreate ? QString ("New report") : QString());
    KKSObjectExemplar * reportEIO = 0;
    if (isCreate)
        reportEIO = new KKSObjectExemplar (idObjE, title, prIO);
    else
        reportEIO = loader->loadEIO (idObjE, prIO);

    if (!reportEIO)
    {
        prIO->release ();
        return;
    }
    KKSCategory * tc0 = reportEIO->io()->category()->tableCategory();
    QStringList aVals;
    aVals << QString::number (-1) // id
          << QString::number (-1) // category is unknown
          << QDateTime::currentDateTime().toString(Qt::ISODate) // insert date
          << QString::number (-1) // адресат
          << QString::number (-1) // отв. исполнитель
          << QString::number (-1) // контролер
          << QDateTime::currentDateTime().toString(Qt::ISODate) // deadline
          << QString() // period
          << QString ("") // reason
          << QString ("false") // archived
          << QString::number (1);

    KKSMap<int, KKSCategoryAttr*>::const_iterator psa;// = tc0->attributes().constFind (idSenderAttr);
    int i=0;
    for (psa = tc0->attributes().constBegin(); \
         psa != tc0->attributes().constEnd(); \
         psa++)
    {
        reportEIO->addAttrValue (aVals[i], psa.value());
        i++;
    }
    {
        KKSList<KKSAttrValue *>::const_iterator pcAttrs = reportEIO->attrValues().constBegin ();
        Q_UNUSED (pcAttrs);
        //for (; pcAttrs != reportEIO->attrValues().constEnd(); pcAttrs++)
        //    qDebug () << __PRETTY_FUNCTION__ << (*pcAttrs)->attribute()->code () << (*pcAttrs)->value().value ();
    }
    PeriodicReportForm * pForm = new PeriodicReportForm (isCreate, parent, f);
    pForm->setReport (reportEIO);
    
    reportEIO->release ();
    prIO->release ();
    
    connect (pForm, SIGNAL (loadReference(QString, QWidget *, int)), this, SLOT (loadAttributeReference (QString, QWidget *, int)) );

    if (pForm->getReport() && pForm->exec () == QDialog::Accepted)
    {
        KKSObjectExemplar *rEIO = pForm->getReport ();
        int res = eiof->insertTSDRecord(rEIO);
        qDebug () << __PRETTY_FUNCTION__ << res;
		if(res <= 0){
			qCritical() << tr("Adressee, executor and controller must be all either public or local!");
            QMessageBox::critical(parent, tr("Error"), tr("Adressee, executor and controller must be all either public or local!"), QMessageBox::Ok);
		}
    }

    pForm->setParent (0);
    delete pForm;
    return;
}

void KKSObjEditorFactory :: loadAttributeSingleFilters (const QString & tableName, const KKSAttribute * attr, QAbstractItemModel * mod)
{
    if (tableName.isEmpty() || !attr || !mod)
        return;

    //QString tableName = attr->tableName ();
    //if (tableName.isEmpty())
    //    return;

    KKSObject * refObj = loader->loadIO (tableName, true);
    if (!refObj)
        return;

    KKSList<const KKSFilterGroup *> filters;
    KKSCategory * c = refObj->category();
    if (c)
        c = c->tableCategory ();

    if (!c)
    {
        refObj->release ();
        return;
    }

    if (loader->getUserId() != ADMIN_ROLE && refObj->id () == IO_CAT_ID)
    {
        KKSList<const KKSFilter *> fl;
        QString value = "select id from io_categories where not is_system";
        //
        // исключаем системные категории
        //
        const KKSFilter * f = c->createFilter(ATTR_ID, value, KKSFilter::foInSQL);
        if(!f){
            qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
            refObj->release ();
            return;
        }
        
        fl.append(f);
        f->release();

        value = "select id from io_categories where id_io_category_type != 10";
        //
        // исключаем системные подчиненные категории
        //
        f = c->createFilter(ATTR_ID, value, KKSFilter::foInSQL);
        if(!f){
            qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
            refObj->release ();
            return;
        }
        
        fl.append(f);
        f->release();

        KKSFilterGroup * fg = new KKSFilterGroup(true);
        fg->setFilters(fl);
        filters.append(fg);
        fg->release();
    }
    else if (loader->getUserId() != ADMIN_ROLE && refObj->id () == IO_USERS_ID)
    {
        KKSList<const KKSFilter *> fl;
        QString value = "select id from users where id > 2";
        //
        // исключаем системных пользователей
        //
        const KKSFilter * f = c->createFilter(ATTR_ID, value, KKSFilter::foInSQL);
        if(!f){
            qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
            refObj->release ();
            return;
        }
        
        fl.append(f);
        f->release();

        KKSFilterGroup * fg = new KKSFilterGroup(true);
        fg->setFilters(fl);
        filters.append(fg);
        fg->release();
    }

    KKSMap<qint64, KKSEIOData *> refRec = loader->loadEIOList (refObj, filters);
    KKSMap<qint64, KKSEIOData *>::const_iterator p;
    
    //если атрибут типа ссылка ссылается на поле, отличное от id (например, в случае справочников "погруженных" в среду DynamicDocs из внешней схемы БД)
    //В качестве userData в комбобоксе надо использовать именно его

    mod->insertRows (0, refRec.count());
    int i (0);
    for (p = refRec.constBegin(); p != refRec.constEnd(); p++)
    {
        QModelIndex wIndex = mod->index (i, 0);
        QString title = p.value()->fields().value (attr->columnName());
        if (title.isEmpty() && refObj->id () == IO_USERS_ID)
            title = p.value()->fields().value ("fio");
//        else{
//            title = p.value()->fields
//        }
        
        QString refColName = attr->refColumnName();
        if(refColName.isEmpty())
            refColName = "id";
        QString key = p.value()->fields().value(refColName);
        if (attr->refType()->id() == KKSAttrType::atJPG)
        {
            QPixmap pjpg;
            pjpg.loadFromData (p.value()->fields().value(attr->columnName()).toLocal8Bit(), "XPM");
            QSize iconSize (24, 24);
            QPixmap icon (pjpg.scaled (iconSize, Qt::KeepAspectRatio));
            mod->setData (wIndex, icon, Qt::DecorationRole);
        }
        else if (attr->refType()->id() == KKSAttrType::atSVG)
        {
            QPixmap pic;
            pic.loadFromData (p.value()->fields().value(attr->columnName()).toLocal8Bit(), "SVG");
            QSize iconSize (24, 24);
            QPixmap icon (pic.scaled (iconSize, Qt::KeepAspectRatio));
            mod->setData (wIndex, icon, Qt::DecorationRole);
        }
        else
            mod->setData (wIndex, title, Qt::DisplayRole);
        mod->setData (wIndex, p.key(), Qt::UserRole);
        i++;
        //cbList->addItem (title, p.key());
    }

    refObj->release ();
}
 

void KKSObjEditorFactory :: loadAttributeFilters (const QString & tableName, const KKSAttribute * attr, QAbstractItemModel * mod)
{
    if (tableName.isEmpty() || !attr || !mod)
        return;

    
    if (attr->columnName().isEmpty())
    {
        qWarning ("loadAttributeValues was invoked with corrupt KKSAttribute "
                 "(type is atList or atParent or atCheckList or atCheckListEx but tableName or columnName are empty)!");
        return;
    }
    

    bool isXml (attr->refType()->id() == KKSAttrType::atSVG ||
                attr->refType()->id() == KKSAttrType::atXMLDoc);
    
    QMap<int, QString> refValues;
    QMap<int, QString> values = loader->loadAttributeValues (attr, refValues, isXml, !isXml);
    qDebug () << __PRETTY_FUNCTION__ << attr->tableName() << attr->columnName () << attr->id() << values;

    if (mod->rowCount())
    {
        int nr = mod->rowCount();
        mod->removeRows (0, nr);
    }

    if (mod->columnCount() > 1)
    {
        int nc = mod->columnCount();
        mod->removeColumns (1, nc-1);
    }

    if (!mod->columnCount())
        mod->insertColumns (0, 1);

    if (attr->type()->attrType() == KKSAttrType::atList ||
        attr->type()->attrType() == KKSAttrType::atParent)
    {
        loadAttributeSingleFilters (tableName, attr, mod);
        return;
    }
    mod->insertRows (0, values.count());
    int i=0;
    for (QMap<int, QString>::const_iterator p = values.constBegin(); p != values.constEnd(); p++)
    {
        QModelIndex wIndex = mod->index (i, 0);
        if (attr->type()->attrType() == KKSAttrType::atCheckList ||
             attr->type()->attrType() == KKSAttrType::atCheckListEx )
            mod->setData (wIndex, Qt::Unchecked, Qt::CheckStateRole);
        if (attr->refType()->id() == KKSAttrType::atJPG)
        {
            QPixmap pjpg;
            pjpg.loadFromData (p.value().toLocal8Bit(), "XPM");
            QSize iconSize (24, 24);
            QPixmap icon (pjpg.scaled (iconSize, Qt::KeepAspectRatio));
            mod->setData (wIndex, icon, Qt::DecorationRole);
        }
        else if (attr->refType()->id() == KKSAttrType::atSVG)
        {
            QPixmap pic;
            pic.loadFromData (p.value().toLocal8Bit(), "SVG");
            QSize iconSize (24, 24);
            QPixmap icon (pic.scaled (iconSize, Qt::KeepAspectRatio));
            mod->setData (wIndex, icon, Qt::DecorationRole);
        }
        else
            mod->setData (wIndex, p.value(), Qt::DisplayRole);
        mod->setData (wIndex, p.key(), Qt::UserRole);
        i++;
    }
}

/* Метод сохраняет критерии поиска в БД.
 * Параметры:
 * group -- группа критериев поиска.
 */
void KKSObjEditorFactory :: saveSearchCriteria (KKSSearchTemplate * st, KKSFilterGroup * group, const KKSCategory * c)
{
    if (!group)
        return;
    KKSList<KKSSearchTemplate *> stList = loader->loadSearchTemplates ();
    bool isContains = false;
    int numc (-1);
    Q_UNUSED (numc);
    Q_UNUSED (isContains);
    Q_UNUSED (c);
    QString stName;
    if (!st)
        st = new KKSSearchTemplate (-1, group, stName, loader->getUserId());
    KKSSearchTemplateForm * stForm = qobject_cast<KKSSearchTemplateForm *>(this->sender());
    if (stForm)
    {
        int idCat = stForm->getIdCat();
        KKSCategory * cat = loader->loadCategory (idCat);
        if (cat)
        {
            st->setCategory (cat->id(), cat->name());
            cat->release ();
        }

        int idSearchTemplateType = stForm->getIdType ();
        KKSMap<int, KKSSearchTemplateType *> sTypes = loader->loadSearchTemplateTypes ();
        KKSSearchTemplateType * sType = sTypes.value (idSearchTemplateType);
        st->setType (sType);
        int idSearchTemplate (-1);
        if (st->id() < 0)
        {
            idSearchTemplate = ppf->insertSearchTemplate(st);
            st->setId (idSearchTemplate);
        }
        else
            idSearchTemplate = ppf->updateSearchTemplate(st);
        if (idSearchTemplate < 0)
        {
            QMessageBox::warning(stForm,tr("Save search template"), tr("Cannot save search template, Error!"), QMessageBox::Ok);
            return;
        }
    }
/*    SaveSearchTemplateForm * stForm = GUISearchTemplate (st);
    if (st && stForm->exec () == QDialog::Accepted)
    {
        KKSSearchTemplate * stt = stForm->getSearchTemplate ();
//        if(c)
//            st->setCategory(c->id(), c->name());
        int idCat = stForm->getIdCat ();
        KKSCategory * cat = loader->loadCategory (idCat);
        if (cat)
        {
            st->setCategory (cat->id(), cat->name());
            cat->release ();
        }

        int idSearchTemplateType = stForm->getIdType ();
        KKSMap<int, KKSSearchTemplateType *> sTypes = loader->loadSearchTemplateTypes ();
        KKSSearchTemplateType * sType = sTypes.value (idSearchTemplateType);
        stt->setType (sType);

        int idSearchTemplate = ppf->insertSearchTemplate (stt);

        stt->setId (idSearchTemplate);
        if (qobject_cast<KKSFiltersEditorForm *>(this->sender ()))
        {
            KKSFiltersEditorForm * feForm = qobject_cast<KKSFiltersEditorForm *>(this->sender ());
            feForm->setSearchTemplate (stt);
        }
        st->release ();
    }
    delete stForm;
 */
}

SaveSearchTemplateForm * KKSObjEditorFactory :: GUISearchTemplate (KKSSearchTemplate * st, bool mode, QWidget *parent, Qt::WindowFlags f) const
{
    QAbstractItemModel * searchTModel = new QStandardItemModel (0, 1);
    searchTModel->setHeaderData (0, Qt::Horizontal, tr ("Search group"), Qt::DisplayRole);
    //searchTModel->setHeaderData (1, Qt::Horizontal, tr ("Author"), Qt::DisplayRole);
    //searchTModel->setHeaderData (2, Qt::Horizontal, tr ("Creation date/time"), Qt::DisplayRole);
    //searchTModel->setHeaderData (3, Qt::Horizontal, tr ("Category"), Qt::DisplayRole);
    //searchTModel->setHeaderData (4, Qt::Horizontal, tr ("Type"), Qt::DisplayRole);
    KKSViewFactory::getSearchTemplates (loader, searchTModel, QModelIndex(), false);
    SaveSearchTemplateForm * stForm = new SaveSearchTemplateForm (st, mode, parent, f);
    QModelIndex stCInd = KKSViewFactory::searchModelRowsIndex(searchTModel, st->type()->id(), QModelIndex(), Qt::UserRole);
    stForm->setTypesModel (searchTModel);
    stForm->selectType (stCInd);
/*    KKSList<const KKSFilterGroup *> filters;
    KKSObject * refCatObj = loader->loadIO (IO_CAT_ID, true);
    if (!refCatObj)
        return 0;
    KKSCategory * cat = refCatObj->category();
    if (!cat || !cat->tableCategory())
    {
        refCatObj->release();
        return 0;
    }
    cat = cat->tableCategory();
    KKSFilter * cMainFilter = cat->createFilter (ATTR_IS_MAIN, QString("false"), KKSFilter::foEq);
    if (!cMainFilter)
    {
        refCatObj->release();
        return 0;
    }

    KKSFilter * cArchFilter = cat->createFilter (ATTR_IS_ARCHIVED, QString("FALSE"), KKSFilter::foEq);
    if (!cArchFilter)
    {
        refCatObj->release();
        return 0;
    }
    KKSList<const KKSFilterGroup *> cFilterGroups;
    KKSList<const KKSFilter*> catFilters;
    
    catFilters.append (cMainFilter);
    catFilters.append (cArchFilter);
    
    KKSFilterGroup * cGroup = new KKSFilterGroup (true);
    
    cGroup->setFilters (catFilters);
    cFilterGroups.append (cGroup);
    cGroup->release ();
*/
    QModelIndex catInd;
    QAbstractItemModel * catModel = initSearchCatsModel (st, catInd);
 //KKSViewFactory::initCategoriesModel (loader, filters, cFilterGroups);
//    QModelIndex catInd = KKSViewFactory::searchModelRowsIndexMultiType(catModel, st->idCategory(), 1);
    qDebug () << __PRETTY_FUNCTION__ << catInd;
    stForm->setCategoryModel (catModel);
    stForm->selectCategory (catInd);
    connect (stForm, SIGNAL (categoryChanged(KKSSearchTemplate *, int)), this, SLOT (searchTemplateCategoryChanged(KKSSearchTemplate *, int)));
    return stForm;
}

QAbstractItemModel * KKSObjEditorFactory :: initSearchCatsModel (KKSSearchTemplate * st, QModelIndex & catInd) const
{
    if (!st)
        return 0;
    KKSList<const KKSFilterGroup *> filters;
    KKSObject * refCatTypeObj = loader->loadIO(IO_CAT_TYPE_ID, true);
    if (!refCatTypeObj)
        return 0;
    KKSCategory * catType = refCatTypeObj->category();
    if (!catType || !catType->tableCategory())
    {
        refCatTypeObj->release();
        return 0;
    }
    catType = catType->tableCategory();
    KKSFilter * ctSysFilter = catType->createFilter(1, QString("select id from io_category_types where id not in (8,9)"), KKSFilter::foInSQL);
    KKSList<const KKSFilter*> catTypeFilters;
    catTypeFilters.append (ctSysFilter);
    ctSysFilter->release();
    KKSFilterGroup * ctGroup = new KKSFilterGroup (true);
    ctGroup->setFilters(catTypeFilters);
    filters.append (ctGroup);
    ctGroup->release();
    refCatTypeObj->release();
    KKSObject * refCatObj = loader->loadIO (IO_CAT_ID, true);
    if (!refCatObj)
        return 0;
    KKSCategory * cat = refCatObj->category();
    if (!cat || !cat->tableCategory())
    {
        refCatObj->release();
        return 0;
    }
    cat = cat->tableCategory();
    KKSFilter * cSysFilter = cat->createFilter (1, QString("select id from io_categories where id_io_category_type not in (8,9)"), KKSFilter::foInSQL);
    if (!cSysFilter)
    {
        refCatObj->release();
        return 0;
    }

    KKSList<const KKSFilterGroup *> cFilterGroups;
    KKSList<const KKSFilter*> catFilters;
    
    catFilters.append (cSysFilter);
    cSysFilter->release();
    refCatObj->release ();
    
    KKSFilterGroup * cGroup = new KKSFilterGroup (true);
    
    cGroup->setFilters (catFilters);
    cFilterGroups.append (cGroup);
    cGroup->release ();

    QAbstractItemModel * catModel = KKSViewFactory::initCategoriesModel (loader, filters, cFilterGroups);
    catInd = KKSViewFactory::searchModelRowsIndexMultiType(catModel, st->idCategory(), 1);
    return catModel;
}

void KKSObjEditorFactory :: searchTemplateCategoryChanged (KKSSearchTemplate * st, int idCategory)
{
    if (!st || st->id() < 0)
        return;
    bool isApp = loader->isApplicable (st, idCategory);
    if (!isApp)
    {
        QWidget * pWidget = qobject_cast<QWidget *>(this->sender());
        
        KKSCategory * c = loader->loadCategory(idCategory, true);
        qCritical() << tr("Category %1 is not applicable into search template %2").arg (c->name()).arg(st->id());
        QMessageBox::critical (pWidget, tr("Set category into search template"), tr("Category %1 is not applicable into search template %2").arg (c->name()).arg(st->name()), QMessageBox::Ok);
        c->release ();
        return;
    }
}

/* Метод загружает критерий поиска из БД.
 */
void KKSObjEditorFactory :: loadSearchCriteria ()//QAbstractItemModel * mod)
{
//    if (!mod)
//        return;

    KKSSearchTemplate * searchT = loadSearchTemplate ();
    QWidget * parent = qobject_cast<QWidget *>(this->sender());
    if (!searchT)
    {
        qCritical() << tr ("Cannot load search template from database");
        QMessageBox::critical (parent, tr ("Select template"), tr ("Cannot load search template from database"), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    const KKSFilterGroup * mainGroup = searchT->getMainGroup ();
    KKSList<const KKSFilterGroup *> fGroups;
    fGroups.append (mainGroup);
    KKSFiltersEditorForm * feForm = (qobject_cast<KKSFiltersEditorForm *>(this->sender()));
    if (feForm)
        feForm->setFilters (fGroups);
}

void KKSObjEditorFactory :: GUIloadDBSearchTemplate (KKSSearchTemplate *st, const KKSCategory* c, QString tableName)
{
    KKSSearchTemplatesForm * stForm = new KKSSearchTemplatesForm (c, tableName, true);
    this->initSearchTemplateModel(stForm);
    stForm->hideActions(0, 11);
    if (stForm && stForm->exec() == QDialog::Accepted)
    {
        int idSearchTemplate = stForm->getIdSearchTemplate();
        if (idSearchTemplate < 0)
            return;
        KKSSearchTemplate * searchT = loader->loadSearchTemplate(idSearchTemplate);
        KKSFilterGroup * mainGroup = searchT->getMainGroup ();
        st->setMainGroup(mainGroup);
        KKSList<const KKSFilterGroup *> fGroups;
        fGroups.append (mainGroup);
        KKSSearchTemplateForm * feForm = (qobject_cast<KKSSearchTemplateForm *>(this->sender()));
        if (feForm)
            feForm->setFilters (fGroups);
    }
}
/* Метод загружает шаблоны поиска из БД и осуществляет выбор из формы.
 */
KKSSearchTemplate * KKSObjEditorFactory :: loadSearchTemplate (void)
{
    KKSSearchTemplate * searchT = 0;
    QWidget * parent = qobject_cast<QWidget *>(this->sender());
    KKSSearchTemplatesForm *stForm = GUISearchTemplate (true, parent);
    QAbstractItemModel * searchTModel = stForm->dataModel();
    while (qobject_cast<QAbstractProxyModel *>(searchTModel))
        searchTModel = (qobject_cast<QAbstractProxyModel *>(searchTModel))->sourceModel();

    QItemSelectionModel * selTModel = stForm->selectionModel ();
    QAbstractProxyModel * sortTModel = qobject_cast<QAbstractProxyModel *>(stForm->dataModel());

    if (selTModel && stForm->exec () == QDialog::Accepted)
    {
        if (selTModel->selection().indexes ().isEmpty())
            return 0;
        QModelIndex stProxyIndex = selTModel->selection().indexes ().at (0);
        QModelIndex stIndex = sortTModel->mapToSource (stProxyIndex);

        QVariant v = searchTModel->data (stIndex, Qt::UserRole+USER_ENTITY);
        if (v.toInt() == 0)
            return 0;
        int idSearchTemplate = stIndex.data (Qt::UserRole).toInt();
        searchT = loader->loadSearchTemplate (idSearchTemplate);
    }

    stForm->setParent (0);
    delete stForm;
    return searchT;
}

KKSSearchTemplatesForm * KKSObjEditorFactory :: GUISearchTemplate (bool mode, QWidget * parent, Qt::WindowFlags flags)
{
    KKSObject * io = loader->loadIO (IO_IO_ID, true);
    if (!io)
        return NULL;

    KKSCategory *c = io->category ()->tableCategory ();
    if (!c)
    {
        io->release();
        return NULL;
    }
    io->release();

    KKSSearchTemplatesForm *stForm = new KKSSearchTemplatesForm (c, "io_objects", mode, parent, flags);
    this->initSearchTemplateModel (stForm);
    if (!mode)
    {
        stForm->setResultsVisible(false);
        emit editorSearchTemplate (stForm);
    }
    return stForm;
}

void KKSObjEditorFactory :: initSearchTemplateModel (KKSSearchTemplatesForm *stForm) const
{
    if (!stForm)
        return;

    connect (stForm, SIGNAL (addNewEmptySearchTemplate (const QModelIndex&, 
                                                        QAbstractItemModel *, 
                                                        const KKSCategory *, 
                                                        const QString &)), 
             this, SLOT (addNewSearchTempl (const QModelIndex&, 
                                            QAbstractItemModel *, 
                                            const KKSCategory *,
                                            const QString &)) );

    connect (stForm, SIGNAL (addNewCopySearchTemplate (const QModelIndex&, QAbstractItemModel *)), this, SLOT (addCopySearchTempl (const QModelIndex&, QAbstractItemModel *)) );
    connect (stForm, SIGNAL (updateSearchTemplate (const QModelIndex&, QAbstractItemModel *)), this, SLOT (updateSearchTempl (const QModelIndex&, QAbstractItemModel *)) );
    connect (stForm, SIGNAL (deleteSearchTemplate (const QModelIndex&, QAbstractItemModel *)), this,  SLOT (deleleSearchTempl (const QModelIndex&, QAbstractItemModel *)) );
    
    connect (stForm, SIGNAL (addSearchTemplateType (const QModelIndex&, QAbstractItemModel * )), this, SLOT (addSearchTemplateType (const QModelIndex&, QAbstractItemModel *)) );
    connect (stForm, SIGNAL (updateSearchTemplateType (const QModelIndex&, QAbstractItemModel * )), this, SLOT (editSearchTemplateType (const QModelIndex&, QAbstractItemModel *)) );
    connect (stForm, SIGNAL (delSearchTemplateType (const QModelIndex&, QAbstractItemModel * )), this, SLOT (delSearchTemplateType (const QModelIndex&, QAbstractItemModel *)) );
    
    connect (stForm, SIGNAL (refSearchTemplates (QAbstractItemModel *)), this, SLOT (refreshSearchTemplates (QAbstractItemModel *)) );
    connect (stForm, SIGNAL (applySearchTemplate (int)), this, SLOT (applySearchTemplate(int)) );

    QSortFilterProxyModel * sortTModel = new KKSSearchTemplateFilterProxyModel;//QSortFilterProxyModel;
    sortTModel->setFilterCaseSensitivity (Qt::CaseInsensitive);
    int ncount (0);
    QStandardItemModel * searchTModel = new QStandardItemModel (ncount, 5);
    searchTModel->setHeaderData (0, Qt::Horizontal, tr ("Search criteria"), Qt::DisplayRole);
    searchTModel->setHeaderData (1, Qt::Horizontal, tr ("Author"), Qt::DisplayRole);
    searchTModel->setHeaderData (2, Qt::Horizontal, tr ("Creation date/time"), Qt::DisplayRole);
    searchTModel->setHeaderData (3, Qt::Horizontal, tr ("Category"), Qt::DisplayRole);
    searchTModel->setHeaderData (4, Qt::Horizontal, tr ("Type"), Qt::DisplayRole);
    KKSViewFactory::getSearchTemplates (loader, searchTModel);
    sortTModel->setSourceModel (searchTModel);
    sortTModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    sortTModel->sort(0);
    stForm->setDataModel (sortTModel);
}

void KKSObjEditorFactory::refreshSearchTemplates (QAbstractItemModel * searchMod)
{
    if (!searchMod)
        return;
    if (searchMod->rowCount())
    {
        int nr = searchMod->rowCount();
        searchMod->removeRows(0, nr);
    }
    KKSViewFactory::getSearchTemplates (loader, searchMod);
    
}

void KKSObjEditorFactory::applySearchTemplate (int idSearchTemplate)
{
    KKSSearchTemplate * searchT = loader->loadSearchTemplate(idSearchTemplate);
    if (!searchT)
    {
        QWidget * pWidget = qobject_cast<QWidget *>(sender());
        QMessageBox::warning (pWidget, tr("Apply search template"), tr("Cannot load search template %1 from DB").arg (idSearchTemplate), QMessageBox::Ok);
        return;
    }
    int idCat = searchT->idCategory();
    const KKSObject * refObj = loader->loadIO (IO_IO_ID);
    const KKSCategory * c = refObj->category();
    //loader->loadCategory(idCat, true);
    if (!c || !c->tableCategory())
    {
        refObj->release();
        searchT->release();
        return;
    }
    const KKSCategory * ct = c->tableCategory ();
    KKSFilter * f = ct->createFilter(1, QString("select io.id from io_objects io inner join io_categories c on (io.id_io_category=c.id and c.id_child=%1)").arg (idCat), KKSFilter::foInSQL);
    KKSFilterGroup * fg = new KKSFilterGroup (true);
    fg->addFilter (f);
    f->release ();
    KKSList<const KKSFilterGroup *> filters;
    filters.append (fg);
    fg->release ();
    KKSMap<qint64, KKSEIOData *> rList = loader->loadEIOList(refObj, filters);
    qint64 idObject (-1);
    if (!rList.count())
    {
        QWidget * pWidget = qobject_cast<QWidget *>(sender());
        QMessageBox::warning (pWidget, tr("Apply search template"), tr("No convenient references for search"), QMessageBox::Ok);
        refObj->release();
        searchT->release();
        return;
    }
    else if (rList.count() > 1)
    {
        KKSRecDialog * recEditor = this->createObjRecEditor(IO_IO_ID,IO_IO_ID,filters,tr("Select reference"), ct, true, false, Qt::WindowModal, 0, Qt::Dialog);
        if (recEditor && recEditor->exec() == QDialog::Accepted)
        {
            idObject = recEditor->getID();
            delete recEditor;
        }
        else
        {
            refObj->release();
            searchT->release();
            delete recEditor;
            return;
        }
    }
    else
        idObject = rList.begin().key();
    QString extraTitle;
    KKSList<const KKSFilterGroup *> refFilters;
    KKSObject * o = loader->loadIO (idObject);
    if (!o)
    {
        refObj->release();
        searchT->release();
        return;
    }
    const KKSCategory * wCat = o->category();
    if (!wCat || !wCat->tableCategory())
    {
        o->release();
        refObj->release();
        searchT->release();
        return;
    }
    wCat = wCat->tableCategory();
    const KKSTemplate * t = new KKSTemplate (wCat->defTemplate());
    KKSFilterGroup * stGroup = searchT->getMainGroup ();
    refFilters.append (stGroup);
    KKSObjEditor * editor = createObjEditor (IO_IO_ID, 
                                             idObject, 
                                             refFilters, 
                                             extraTitle,
                                             wCat,
                                             false, 
                                             QString(),
                                             false,
                                             Qt::NonModal,
                                             0);
    QTreeView * tv=0;
    tv = editor->getRecordsWidget()->getView();
    QString tableName = o->tableName();
    //qDebug () << __PRETTY_FUNCTION__ << tableName;
    if (idObject == IO_IO_ID && loader->getUserId() != ADMIN_ROLE)
        filterSysIO (wCat, refFilters);
    KKSViewFactory::loadEIOEx (editor,
                               o,
                               loader,
                               t,
                               tv,
                               refFilters,
                               false,
                               wCat,
                               tableName);
    editor->setFilters (refFilters);
    o->release();
    t->release();
    emit editorCreated (editor);

    refObj->release();
    searchT->release ();
}

void KKSObjEditorFactory::filterSysIO (const KKSCategory * ct, KKSList<const KKSFilterGroup *> & filters)
{
    KKSList<const KKSFilter *> fl;
    QString value = "select id from io_categories where id_io_category_type <> 2";//исключаем журналы
    const KKSFilter * f = ct->createFilter(ATTR_ID_IO_CATEGORY, value, KKSFilter::foInSQL);
    if(!f){
        qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
        return;
    }

    fl.append(f);
    f->release();

    value = "select id from io_categories where id_io_category_type <> 8";//исключаем системные справочники
    f = ct->createFilter(ATTR_ID_IO_CATEGORY, value, KKSFilter::foInSQL);
    if(!f){
        qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
        return;
    }

    fl.append(f);
    f->release();

    value = "select id from io_categories where id_io_category_type <> 9";//исключаем системный справочник категорий
    f = ct->createFilter(ATTR_ID_IO_CATEGORY, value, KKSFilter::foInSQL);
    if(!f){
        qWarning() << __PRETTY_FUNCTION__ << "Cannon create system filter!!";
        return;
    }
    fl.append(f);
    f->release();

    KKSFilterGroup * fg = new KKSFilterGroup(true);
    fg->setFilters(fl);
    filters.append(fg);
    fg->release();
}

void KKSObjEditorFactory::createSearchTemplate()
{
    QModelIndex parent= QModelIndex();
    addNewSearchTempl(parent, NULL, NULL, "io_objects");
}

/* Метод создает новый шаблон поиска и добавляет соответствующую запись в модель searchMod.
 */
void KKSObjEditorFactory :: addNewSearchTempl (const QModelIndex& parent, 
                                               QAbstractItemModel * searchMod, 
                                               const KKSCategory * cat,
                                               const QString & tableName)
{
    QWidget * pWidget = qobject_cast <QWidget *>(this->sender());
    QModelIndex pIndex = QModelIndex();
    if (parent.isValid() && parent.data (Qt::UserRole+USER_ENTITY).toInt() != 0)
        pIndex = parent.parent();
    else if (parent.isValid() && parent.data (Qt::UserRole+USER_ENTITY).toInt() == 0)
        pIndex = parent;

    bool isMod = pWidget ? pWidget->isModal() : false;
    qDebug () << __PRETTY_FUNCTION__ << pIndex << isMod;
    int idSearchType = pIndex.isValid() ? pIndex.data (Qt::UserRole).toInt () : 1;
    QString stName;
    KKSSearchTemplate * st = new KKSSearchTemplate (-1, 0, stName, loader->getUserId());
    if (!st)
        return;
    KKSMap<int, KKSSearchTemplateType*> sTemplates = loader->loadSearchTemplateTypes ();
    KKSSearchTemplateType * stt = sTemplates.value (idSearchType);
    st->setType (stt);
    KKSSearchTemplateForm * filterForm = GUISearchTemplateInit (st, cat, false, tableName, isMod, pWidget);
    connect (filterForm, SIGNAL (saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory *)), this, SLOT (saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory *)) );
    //connect (filterForm, SIGNAL (loadAttributeRefValues (const QString &, const KKSAttribute *, QComboBox *)), this, SLOT (loadAttributeFilters (const QString &, const KKSAttribute *, QComboBox *)) );
    connect (filterForm, SIGNAL (loadAttributeRefValues (const QString &, const KKSAttribute *, QAbstractItemModel *)), this, SLOT (loadAttributeFilters (const QString &, const KKSAttribute *, QAbstractItemModel *)) );

    if (!isMod)
    {
        emit editorSearchTemplate (filterForm);
        return;
    }

    if (filterForm->exec () == QDialog::Accepted)
    {
        int res = filterForm->searchT()->id ();
        if (res > 0)
        {
            KKSSearchTemplate * stdb = loader->loadSearchTemplate (res);
            stName = stdb->name ();
            stdb->release ();
            if(searchMod){
                int nr = searchMod->rowCount(pIndex);
                searchMod->insertRows (nr, 1, pIndex);
                QModelIndex wIndex = searchMod->index (nr, 0, pIndex);
                if (searchMod->columnCount(pIndex) >= 1)
                {
                    searchMod->setData (wIndex, stName, Qt::DisplayRole);
                    searchMod->setData (wIndex, res, Qt::UserRole);
                }
                else
                {
                    searchMod->setData (wIndex, res, Qt::DisplayRole);
                    searchMod->setData (wIndex, res, Qt::UserRole);
                    
                    QModelIndex w1Index = searchMod->index (nr, 1, pIndex);
                    searchMod->setData (w1Index, stName, Qt::DisplayRole);
                    searchMod->setData (w1Index, res, Qt::UserRole);
                }
                searchMod->setData (wIndex, QIcon(":/ddoc/rubric_item.png").pixmap(24, 24), Qt::DecorationRole);
                searchMod->setData (wIndex, 1, Qt::UserRole+USER_ENTITY);
                QSize searchRow = searchMod->data (wIndex, Qt::SizeHintRole).toSize();
                searchRow.rheight() = 24;
                searchMod->setData (wIndex, searchRow, Qt::SizeHintRole);
            }
        }
    }

    filterForm->setParent (0);
    delete filterForm;

    st->release ();
}

KKSSearchTemplateForm * KKSObjEditorFactory :: GUISearchTemplateInit (KKSSearchTemplate * st, const KKSCategory * cat, bool forIO, const QString& tableName, bool mode, QWidget *parent, Qt::WindowFlags flags) const
{
    if (!st)
        return 0;
    const KKSCategory * c = cat;

    if(!c){
        KKSObject * o = loader->loadIO (IO_IO_ID, true);
        if (!o)
        {
            return 0;
        }

        c = o->category()->tableCategory();
        if (!c)
        {
            o->release();
            return 0;
        }
        o->release();
    }

    st->setCategory(c->id(), c->name());

    KKSMap<int, KKSAttribute *> attrsIO;
    if(!c || (c && c->id() == IO_TABLE_CATEGORY_ID))
        attrsIO = loader->loadIOUsedAttrs ();//атрибуты информационных объектов грузим только если обрабатываем справоник ИО
    
    //KKSFiltersEditorForm *filterForm = new KKSFiltersEditorForm (c, tableName, attrsIO, false, st, pWidget);
    KKSSearchTemplateForm * filterForm = new KKSSearchTemplateForm (c, tableName, attrsIO, forIO, st, mode, parent, flags);
    QAbstractItemModel * searchTypesMod = new QStandardItemModel (0, 1);
    searchTypesMod->setHeaderData(0, Qt::Horizontal, tr("Search template type name"), Qt::DisplayRole);
    KKSViewFactory::getSearchTemplates(loader, searchTypesMod, QModelIndex(), false);
    filterForm->setSearchTemplateModel (searchTypesMod);
    int idSearchType = st ? st->type()->id() : 1;
    QModelIndex tIndex = KKSViewFactory::searchModelIndex(searchTypesMod, idSearchType, QModelIndex(), Qt::UserRole);
    filterForm->selectTypeInd (tIndex);
    QModelIndex catInd;
    QAbstractItemModel * catModel = initSearchCatsModel (st, catInd);
    filterForm->setCatModel (catModel);
    filterForm->setCatInd (catInd);
    connect (filterForm, SIGNAL (catSearchTemplateChanged(KKSSearchTemplate *, int)), this, SLOT (searchTemplateCategoryChanged(KKSSearchTemplate *, int)));
    return filterForm;
}

/* Метод копирует существующий шаблон поиска с идентификатором в wIndex и добавляет соответствующую запись в модель searchMod.
 */
void KKSObjEditorFactory :: addCopySearchTempl (const QModelIndex& wIndex, QAbstractItemModel * searchMod)
{
    qDebug () << __PRETTY_FUNCTION__ << wIndex << searchMod;
    if (!wIndex.isValid () || !searchMod)
        return;
    int idSearchTemplate = wIndex.data (Qt::UserRole).toInt ();
    KKSSearchTemplate * st = loader->loadSearchTemplate (idSearchTemplate);
    if (!st)
    {
        qCritical() << tr ("Cannot load original search template.");
        QMessageBox::critical (qobject_cast<QWidget *>(this->sender()), tr ("Search templates"), tr ("Cannot load original search template."), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    st->setId (-1);
    QString stName = st->name ();
    stName += tr (" copy");
    st->setAuthor (loader->getUserId(), loader->getUserName());
    st->setName (stName);
    int res = ppf->insertSearchTemplate (st);
    if (res < 0)
    {
        qCritical() << tr ("Cannot insert copy of search template into db.");
        QMessageBox::critical(qobject_cast<QWidget *>(this->sender()), tr ("Search templates"), tr ("Cannot insert copy of search template into db."), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    int nr = searchMod->rowCount (wIndex.parent());
    searchMod->insertRows (nr, 1, wIndex.parent());
    QModelIndex stIndex = searchMod->index (nr, 0, wIndex.parent());
    searchMod->setData (stIndex, stName, Qt::DisplayRole);
    searchMod->setData (stIndex, res, Qt::UserRole);
    searchMod->setData (stIndex, st->idAuthor (), Qt::UserRole+1);
    st->release ();
}

/* Метод обновляет существующий шаблон поиска с идентификатором в wIndex и изменяет соответствующую запись в модель searchMod.
 */
void KKSObjEditorFactory :: updateSearchTempl (const QModelIndex& wIndex, QAbstractItemModel * searchMod)
{
    qDebug () << __PRETTY_FUNCTION__ << wIndex << searchMod;
    if (!wIndex.isValid () || !searchMod)
        return;
    QModelIndex pIndex = wIndex.sibling(wIndex.row(), 0).parent();
    int idSearchTemplate = wIndex.data (Qt::UserRole).toInt ();
    QWidget * pWidget = qobject_cast<QWidget *>(this->sender());
    KKSSearchTemplate * st = loader->loadSearchTemplate (idSearchTemplate);
    if (!st)
    {
        qCritical() << tr ("Cannot load original search template.");
        QMessageBox::critical(pWidget, tr ("Search templates"), tr ("Cannot load original search template."), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    bool isMod = pWidget ? pWidget->isModal() : false;
    int idCat = st->idCategory();
    KKSCategory * c = loader->loadCategory(idCat, true);
    QString tableName;
    KKSMap<int, KKSAttribute *> attrsIO;
    if(!c || (c && c->id() == IO_TABLE_CATEGORY_ID))
        attrsIO = loader->loadIOUsedAttrs ();//атрибуты информационных объектов грузим только если обрабатываем справоник ИО
    KKSSearchTemplateForm * filterForm = GUISearchTemplateInit (st, c, false, tableName, isMod, pWidget);
    //new KKSSearchTemplateForm (c, tableName, attrsIO, false, st, isMod, pWidget);
    connect (filterForm, SIGNAL (saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory *)), this, SLOT (saveSearchCriteria (KKSSearchTemplate *, KKSFilterGroup *, const KKSCategory *)) );
    //connect (filterForm, SIGNAL (loadAttributeRefValues (const QString &, const KKSAttribute *, QComboBox *)), this, SLOT (loadAttributeFilters (const QString &, const KKSAttribute *, QComboBox *)) );
    connect (filterForm, SIGNAL (loadAttributeRefValues (const QString &, const KKSAttribute *, QAbstractItemModel *)), this, SLOT (loadAttributeFilters (const QString &, const KKSAttribute *, QAbstractItemModel *)) );

    if (!isMod)
    {
        emit editorSearchTemplate (filterForm);
        return;
    }

    if (filterForm->exec () == QDialog::Accepted)
    {
        int res = filterForm->searchT()->id ();
        if (res > 0)
        {
            KKSSearchTemplate * stdb = loader->loadSearchTemplate (res);
            QString stName = stdb->name ();
            stdb->release ();
            if(searchMod){
                int nr = searchMod->rowCount(pIndex);
                searchMod->insertRows (nr, 1, pIndex);
                QModelIndex wIndex = searchMod->index (nr, 0, pIndex);
                searchMod->setData(wIndex, QIcon(":/ddoc/rubric_item.png").pixmap(24, 24), Qt::DecorationRole);
                if (searchMod->columnCount(pIndex) >= 1)
                {
                    searchMod->setData (wIndex, stName, Qt::DisplayRole);
                    searchMod->setData (wIndex, res, Qt::UserRole);
                }
                else
                {
                    searchMod->setData (wIndex, res, Qt::DisplayRole);
                    searchMod->setData (wIndex, res, Qt::UserRole);
                    
                    wIndex = searchMod->index (nr, 1, pIndex);
                    searchMod->setData (wIndex, stName, Qt::DisplayRole);
                    searchMod->setData (wIndex, res, Qt::UserRole);
                }
            }
        }
    }

    filterForm->setParent (0);
    delete filterForm;

    st->release ();
}

/* Метод удаляет существующий шаблон поиска с идентификатором в wIndex и соответствующую запись в модель searchMod.
 */
void KKSObjEditorFactory :: deleleSearchTempl (const QModelIndex& wIndex, QAbstractItemModel * searchMod)
{
    qDebug () << __PRETTY_FUNCTION__ << wIndex << searchMod;
    if (!wIndex.isValid () || !searchMod)
        return;
    int idSearchTemplate = wIndex.data (Qt::UserRole).toInt ();
    QWidget * pWidget = qobject_cast<QWidget *>(this->sender());
    KKSSearchTemplate * st = loader->loadSearchTemplate (idSearchTemplate);
    if (!st)
    {
        qCritical() << tr ("Cannot load original search template.");
        QMessageBox::critical (pWidget, tr ("Search templates"), tr ("Cannot load original search template."), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    int idUser = loader->getUserId ();
    int idAuthor = st->idAuthor ();
    if (idUser != 1 && idAuthor != idUser)
    {
        
        qWarning() << tr ("Only admin and author can delete search template %1").arg (st->name ());
        QMessageBox::warning (pWidget, 
							  tr ("Search templates"), 
							  tr ("Only admin and author can delete search template %1").arg (st->name ()), 
							  QMessageBox::Ok, 
							  QMessageBox::NoButton);
        st->release ();
		return;
    }
    st->release ();

    int res = ppf->deleteSearchTemplate (idSearchTemplate);
    if (res < 0)
    {
        qCritical() << tr ("Cannot delete search template.");
        QMessageBox::critical (pWidget, tr ("Search templates"), tr ("Cannot delete search template."), QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    searchMod->removeRows (wIndex.row(), 1);
}

/* Слот создает новый тип шаблонов поиска и добавляет его к существующему типу в качестве дочернего.
 * Параметры :
 * parent -- индекс родительского типа
 * sourceMod -- модель типов и шаблонов.
 */
void KKSObjEditorFactory :: addSearchTemplateType (const QModelIndex& parent, QAbstractItemModel * searchMod)
{
    if (!searchMod)
        return;
    KKSList<const KKSFilterGroup *> filters;
    KKSObject * refObj = loader->loadIO (IO_SEARCH_TYPE_ID);
    if (!refObj)
        return;
    
    const KKSCategory * c = refObj->category();
    if (!c)
    {
        refObj->release ();
        return;
    }
    
    c = c->tableCategory();
    KKSMap<qint64, KKSAttrValue *> io_aVals;
    KKSMap<qint64, KKSAttrValue *> aVals;
    if (parent.isValid())
    {
        int idp = parent.data (Qt::UserRole).toInt();
        KKSAttribute * pAttr = loader->loadAttribute (ATTR_ID_PARENT);
        KKSValue val (QString::number (idp), KKSAttrType::atParent);
        if (pAttr)
        {
            KKSCategoryAttr * cAttr = KKSCategoryAttr::create (pAttr, false, true);
            if (cAttr)
            {
                KKSAttrValue * aVal = new KKSAttrValue ( val, cAttr);
                if (aVal)
                {
                    aVals.insert (ATTR_ID_PARENT, aVal);
                    aVal->release ();
                }
                cAttr->release ();
            }
            pAttr->release ();
        }
    }
    KKSObjEditor * editor = createObjEditorParam (IO_SEARCH_TYPE_ID, 
                                                  -1,
                                                  filters,
                                                  tr("Add search template type"),
                                                  c,
                                                  io_aVals,
                                                  aVals,
                                                  true,
                                                  QString("search_template_types"),
                                                  false,
                                                  Qt::WindowModal);
    editor->tbActions->setVisible (false);
    if (editor && editor->exec() == QDialog::Accepted)
    {
        KKSObjectExemplar * pObj = editor->getObjectEx();
        KKSMap<int, KKSSearchTemplateType *> stt = loader->loadSearchTemplateTypes();
        KKSMap<int, KKSSearchTemplateType *>::const_iterator p = stt.constFind (pObj->id());
        KKSSearchTemplatesForm * stForm = qobject_cast<KKSSearchTemplatesForm *> (this->sender());
        QSortFilterProxyModel * sortMod = stForm ? qobject_cast<QSortFilterProxyModel *>(stForm->dataModel()) : 0;
        int nr = searchMod->rowCount (parent);
        searchMod->insertRows (nr, 1, parent);
        QModelIndex wIndex = searchMod->index (nr, 0, parent);
        searchMod->setData (wIndex, p.value()->name(), Qt::DisplayRole);
        searchMod->setData (wIndex, p.value()->id (), Qt::UserRole);
        searchMod->setData (wIndex, 0, Qt::UserRole+USER_ENTITY);
        searchMod->setData (wIndex, QIcon(":/ddoc/rubric.png").pixmap(24, 24), Qt::DecorationRole);
        if (sortMod)
        {
            int logIndex = stForm->getIndicatorSection();
            Qt::SortOrder order = stForm->getIndicatorOrder();
            sortMod->sort(logIndex, order);
        }
    }
    
    refObj->release ();
}

/* Слот редактирует тип шаблонов поиска.
 * Параметры :
 * wIndex -- индекс редактируемого типа
 * sourceMod -- модель типов и шаблонов.
 */
void KKSObjEditorFactory :: editSearchTemplateType (const QModelIndex& wIndex, QAbstractItemModel * searchMod)
{
    if (!searchMod)
        return;
    KKSList<const KKSFilterGroup *> filters;
    KKSObject * refObj = loader->loadIO (IO_SEARCH_TYPE_ID);
    if (!refObj)
        return;
    
    const KKSCategory * c = refObj->category();
    if (!c)
    {
        refObj->release ();
        return;
    }
    
    c = c->tableCategory();
    KKSMap<qint64, KKSAttrValue *> io_aVals;
    KKSMap<qint64, KKSAttrValue *> aVals;
    QModelIndex parent = wIndex.parent();
    if (parent.isValid())
    {
        int idp = parent.data (Qt::UserRole).toInt();
        KKSAttribute * pAttr = loader->loadAttribute (ATTR_ID_PARENT);
        KKSValue val (QString::number (idp), KKSAttrType::atParent);
        if (pAttr)
        {
            KKSCategoryAttr * cAttr = KKSCategoryAttr::create (pAttr, false, true);
            if (cAttr)
            {
                KKSAttrValue * aVal = new KKSAttrValue ( val, cAttr);
                if (aVal)
                {
                    aVals.insert (ATTR_ID_PARENT, aVal);
                    aVal->release ();
                }
                cAttr->release ();
            }
            pAttr->release ();
        }
    }
    KKSObjEditor * editor = createObjEditorParam (IO_SEARCH_TYPE_ID, 
                                                  wIndex.data (Qt::UserRole).toInt(),
                                                  filters,
                                                  tr("Edit search template type"),
                                                  c,
                                                  io_aVals,
                                                  aVals,
                                                  true,
                                                  QString("search_template_types"),
                                                  false,
                                                  Qt::WindowModal);
    editor->tbActions->setVisible (false);
    if (editor && editor->exec() == QDialog::Accepted)
    {
        KKSObjectExemplar * pObj = editor->getObjectEx();
        KKSMap<int, KKSSearchTemplateType *> stt = loader->loadSearchTemplateTypes();
        KKSMap<int, KKSSearchTemplateType *>::const_iterator p = stt.constFind (pObj->id());
        searchMod->setData (wIndex, p.value()->name(), Qt::DisplayRole);
        searchMod->setData (wIndex, p.value()->id (), Qt::UserRole);
        searchMod->setData (wIndex, 0, Qt::UserRole+USER_ENTITY);
    }
    
    refObj->release ();
}

/* Слот удаляет тип шаблонов поиска.
 * Параметры :
 * wIndex -- индекс удаляемого типа
 * sourceMod -- модель типов и шаблонов.
 */
void KKSObjEditorFactory :: delSearchTemplateType (const QModelIndex& wIndex, QAbstractItemModel * searchMod)
{
    if (!searchMod || !wIndex.isValid())
        return;
    KKSObject * refObj = loader->loadIO (IO_SEARCH_TYPE_ID);
    if (!refObj)
        return;
    KKSObjectExemplar * eio = loader->loadEIO (wIndex.data (Qt::UserRole).toInt(), refObj);
    if (!eio)
    {
        refObj->release ();
        return;
    }
    int ier = eiof->deleteEIO (eio);
    if (ier < 0)
    {
        eio->release ();
        refObj->release ();
        return;
    }
    QModelIndex parent = wIndex.parent ();
    searchMod->removeRows (wIndex.row(), 1, parent);
}

/* Метод добавляет к ИО wObj дополнительную таблицу и соответствующую вкладку на editor.
 */
void KKSObjEditorFactory :: addIOTable (KKSObject * wObj, KKSObjEditor * editor)
{
    if (!wObj || !editor)
        return;

    qDebug () << __PRETTY_FUNCTION__;
    QTabWidget * tabEnc = editor->getRecTab ();

    if (!tabEnc)
        return;

    bool ok;
    QString titleName = QInputDialog::getText (editor, tr ("Add additional table"), tr ("Set reference title"), QLineEdit::Normal, QString(), &ok);
    QString tableName = QString ("eio_table_%1_%2").arg (wObj->id()).arg (tabEnc->count());
    qDebug () << __PRETTY_FUNCTION__ << tableName << ok;
    if (!ok || tableName.isEmpty())
        return;

    KKSObject * o = loader->loadIO (IO_CAT_ID, true);
    if (!o)
        return;

    KKSCategory * c = o->category();
    if (!c)
    {
        o->release ();
        return;
    }

    KKSCategory *ct = c->tableCategory ();
    if (!ct)
    {
        o->release ();
        c->release ();
        return;
    }

    KKSList<const KKSFilterGroup *> filters;
    KKSList<const KKSFilter *> fl;
    QString value = "select id from io_categories where id_io_category_type=10 and id > 45";
    //
    // только справочники
    //
    const KKSFilter * f = ct->createFilter(ATTR_ID, value, KKSFilter::foInSQL);
    if (!f)
    {
        c->release ();
        o->release ();
        return;
    }
    fl.append (f);
    f->release ();
    KKSFilterGroup * fg = new KKSFilterGroup(true);
    fg->setFilters(fl);
    filters.append(fg);
    fg->release();
    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         IO_CAT_ID, 
                                                         filters,
                                                         "",
                                                         c,//o->category (),
                                                         true, false,
                                                         Qt::ApplicationModal,
                                                         editor, 
                                                         Qt::Dialog);
    if (!recEditor)
    {
        o->release ();
        return;
    }

    if (recEditor->exec () == QDialog::Accepted)
    {
        int idCat = recEditor->getID ();
        qDebug () << __PRETTY_FUNCTION__ << idCat;
        //QWidget * rw = new QWidget (editor);
        //KKSRecWidget * rw = new KKSRecWidget (tabEnc);
        KKSCategory * wCat = loader->loadCategory (idCat);
        if (!wCat)
        {
            o->release ();
            return;
        }
        int resT = ppf->insertAddTable (wObj, tableName, wCat, titleName, -1, QString());
        if (resT < 0)
        {
            wCat->release ();
            o->release ();
            return;
        }
        KKSList<KKSTemplate*> lTempls = loader->loadCategoryTemplates (wCat->id());
        int nTemplC = lTempls.count();
        KKSTemplate *tChild = nTemplC > 0 ? lTempls[0] : new KKSTemplate (wCat->defTemplate());

        QWidget * copiesW = new QWidget (editor);
        QGridLayout * gRecLay = new QGridLayout (copiesW);
        KKSRecWidget * recW = KKSViewFactory::createAdditionalView (tChild, editor, wObj, wCat, loader, tableName);//, filters, 0);//copiesW);
        recW->actSetView->setEnabled ( nTemplC > 0 );
        gRecLay->addWidget (recW, 0, 0, 1, 1);
        editor->addRecordsWidget (recW);
        editor->addAdditionalCategory (wCat);
        tabEnc->addTab (copiesW, titleName);
        KKSAddTable *aT = new KKSAddTable (wObj->id(), resT, tableName, wCat, titleName, -1, QString());
        if (!aT)
        {
            wCat->release ();
            o->release ();
            return;
        }
        editor->addAdditionalTable (aT);
        int nCats = editor->addCatsCount ();
        tabEnc->insertTab (nCats, copiesW, titleName);
        aT->release ();

        wCat->release ();
    }

    o->release ();
}

/* Метод добавляет новое значение пользовательского атрибута-ссылки на набор элементов справочника.
 * Параметры:
 * wObj -- ИО, содержащий атрибут
 * attr -- атрибут-ссылка
 * isSystem -- не используется
 * sMod -- модель значений атрибутов
 */
void KKSObjEditorFactory :: loadObjAttrRef (KKSObject * wObj, const KKSAttrValue* avE, KKSIndAttr::KKSIndAttrClass isSystem, QAbstractItemModel * sMod)
{
    if (!wObj || !avE || !sMod)
        return;
    //qDebug () << __PRETTY_FUNCTION__ << wObj->id() << attr->id() << attr->type()->id() << isSystem;
    Q_UNUSED (isSystem);
    
    QString tableName = avE->attribute()->tableName ();
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
        return;
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    KKSObjEditor * wEditor = qobject_cast<KKSObjEditor *>(this->sender());
    KKSList<const KKSFilterGroup*> filters;
    KKSCategory * ct = c->tableCategory ();
    if (!ct){
        refObj->release();
        return;
    }

    QStringList sl;
    for (int i=0; i<sMod->rowCount(); i++)
        sl << QString::number (sMod->data (sMod->index (i, 0), Qt::UserRole).toInt());

    QString value = sl.isEmpty() ? QString ("select id from %1").arg (tableName) : QString ("select id from %1 where id not in (%2)").arg (tableName).arg (sl.join (","));
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

    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         refObj->id (), 
                                                         //editor->m_filters, 
                                                         filters, 
                                                         "",
                                                         c,
                                                         true, false,
                                                         wEditor ? wEditor->windowModality () : Qt::ApplicationModal,
                                                         wEditor, 
                                                         Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    if (recEditor->exec () == QDialog::Accepted)
    {
        QStringList sl;
        KKSAttrValue * av = wObj->attrValueId (avE->attribute()->id());
        
        if (!av /*&& wObj->id() <= 0*/) //новый ИО создаем
        {
            QString s;
            KKSValue val (s, KKSAttrType::atCheckListEx);
            KKSCategory * c = wObj->category ();
            if (!c){
                refObj->release();
                return;
            }

            KKSCategoryAttr * cAttr = c->attribute (avE->attribute()->id());
            if (!cAttr){
                refObj->release();
                return;
            }

            av = new KKSAttrValue (val, cAttr);
            wObj->addAttrValue (av);
            wEditor->setObjChanged (true);
        }
        
        if (av)
            sl = av->value().value().split (",");
        
        if (!sl.contains (QString::number (recEditor->getID())))
        {
            sl += QString::number (recEditor->getID());
            int n = sMod->rowCount();
            sMod->insertRows (n, 1);
            QModelIndex wIndex = sMod->index (n, 0);
            sMod->setData (wIndex, recEditor->getID(), Qt::UserRole);
            updateAttrModel (wIndex, sMod, recEditor->getID(), tableName, refObj);
            KKSValue val (sl.join (","), KKSAttrType::atCheckListEx);
            av->setValue (val);
            const_cast<KKSAttrValue *>(avE)->setValue (val);
            wEditor->setObjChanged (true);
        }
        //qDebug () << __PRETTY_FUNCTION__ << sl;
    }

    refObj->release ();
}

/* Метод добавляет новое значение системного атрибута-ссылки на набор элементов справочника.
 * Параметры:
 * wObjE -- ЭИО, содержащий атрибут
 * attr -- атрибут-ссылка
 * isSystem -- не используется
 * sMod -- модель значений атрибутов
 */
void KKSObjEditorFactory :: loadObjCAttrRef (KKSObjectExemplar * wObjE, const KKSAttrValue* avE, KKSIndAttr::KKSIndAttrClass isSystem, QAbstractItemModel * sMod)
{
    if (!wObjE || !avE || !sMod)
        return;
    
    qDebug () << __PRETTY_FUNCTION__ << wObjE->id() << avE->attribute()->id() << avE->attribute()->type()->id() << isSystem;
    
    QString tableName = avE->attribute()->tableName ();
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
        return;
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    KKSObjEditor * wEditor = qobject_cast<KKSObjEditor *>(this->sender());
    KKSList<const KKSFilterGroup*> filters;
    KKSCategory * ct = c->tableCategory ();
    if (!ct){
        refObj->release();
        return;
    }

    QStringList sl;
    for (int i=0; i<sMod->rowCount(); i++)
        sl << QString::number (sMod->data (sMod->index (i, 0), Qt::UserRole).toInt());

    QString value = sl.isEmpty() ? QString ("select id from %1").arg (tableName) : QString ("select id from %1 where id not in (%2)").arg (tableName).arg (sl.join (","));
    
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

    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         refObj->id (), 
                                                         //editor->m_filters, 
                                                         filters, 
                                                         "",
                                                         c,
                                                         true, false,
                                                         wEditor ? wEditor->windowModality () : Qt::ApplicationModal,
                                                         wEditor, 
                                                         Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    if (recEditor->exec () == QDialog::Accepted)
    {
        QStringList sl;
        KKSAttrValue * av = wObjE->attrValue (avE->attribute()->id());
        if (!av && wObjE->id() <= 0)
        {
            QString s;
            KKSValue val (s, KKSAttrType::atCheckListEx);
            KKSObject * io = wObjE->io();
            if (!io){
                refObj->release();
                return;
            }

            KKSCategory * c = io->category ();
            if (!c){
                refObj->release();
                return;
            }

            c = c->tableCategory();
            if (!c){
                refObj->release();
                return;
            }

            KKSCategoryAttr * cAttr = c->attribute (avE->attribute()->id());
            if (!cAttr){
                refObj->release();
                return;
            }

            av = new KKSAttrValue (val, cAttr);
            wObjE->addAttrValue (av);
            wEditor->setObjChanged (true);
        }
        
        if (!av)
        {
            qCritical() << tr("Cannot set value");
            QMessageBox::critical (wEditor, avE->attribute()->title(), tr("Cannot set value"), QMessageBox::Ok, QMessageBox::NoButton);
        }
        else
        {
            sl = av->value().value().split (",");
            if (!sl.contains (QString::number (recEditor->getID())))
            {
                sl += QString::number (recEditor->getID());
                int n = sMod->rowCount();
                sMod->insertRows (n, 1);
                QModelIndex wIndex = sMod->index (n, 0);
                sMod->setData (wIndex, recEditor->getID(), Qt::UserRole);
                updateAttrModel (wIndex, sMod, recEditor->getID(), tableName, refObj);
                KKSValue val (sl.join (","), KKSAttrType::atCheckListEx);
                //qDebug () << __PRETTY_FUNCTION__ << sl << av->attribute()->id();
                av->setValue (val);
                const_cast<KKSAttrValue *>(avE)->setValue (val);
                wEditor->setObjChanged (true);
            }
        }
    }
/*    KKSList<KKSAttrValue *> avals = wObjE->attrValues();
    for (int i=0; i< avals.size(); i++)
        if (avals[i]->attribute()->type()->attrType() == KKSAttrType::atCheckListEx)
            qDebug () << __PRETTY_FUNCTION__ << avals[i]->attribute()->id() << avals[i]->attribute()->id() << avals[i]->value().value();
*/
    refObj->release ();
}

/* слот загружает возможные виды синхронизации из таблицы tableName в виджет attrW для атрибута с attrId
 */
void KKSObjEditorFactory :: loadSyncType (QString tableName, QWidget * attrW, qint64 attrId)
{
//    KKSAttrRefWidget *aRefW = qobject_cast<KKSAttrRefWidget *>(attrW);
    QWidget * wEditor = qobject_cast<QWidget *>(this->sender());
//    connect (this, SIGNAL (editorCreatedModal (KKSObjEditor *)), editor, SLOT (childWidget(KKSObjEditor *)) );
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
    {
        qDebug () << __PRETTY_FUNCTION__ << tableName;
        return;
    }
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    //qDebug () << __PRETTY_FUNCTION__ << editor << refObj->id();

    //aRefW->

    KKSList<const KKSFilterGroup*> filters;
    KKSAttribute * a = loader->loadAttribute(attrId);
    if(!a){
        refObj->release();
        return;
    }

    KKSSearchTemplate * st = NULL;
    st = a->searchTemplate();
    if(st){
        KKSFilterGroup * fg = st->getMainGroup();
        if(fg){
            filters.append(fg);
        }
    }

    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         refObj->id (), 
                                                         //editor->m_filters, 
                                                         filters,//KKSList<const KKSFilterGroup*>(), 
                                                         "",
                                                         c,
                                                         true,
                                                         false,
                                                         wEditor ? wEditor->windowModality () : Qt::ApplicationModal,
                                                         wEditor, 
                                                         Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    if (recEditor->exec () == QDialog::Accepted)
    {
        int idVal = recEditor->getID();
        qDebug () << __PRETTY_FUNCTION__ << idVal;
        QLineEdit * lE = qobject_cast<QLineEdit *> (attrW);
        KKSAttribute * attr = loader->loadAttribute (attrId);
        KKSSyncDialog * sDial = qobject_cast<KKSSyncDialog *>(this->sender());
        //KKSObjEditor * editor = qobject_cast<KKSObjEditor *>(this->sender());
        if (!attr){
            refObj->release();
            return;
        }

        //int idAttr = attr->id ();
        QMap<int, QString> refValues;
        QMap<int, QString> values = loader->loadAttributeValues (attr, refValues, false, true, attr->tableName());
        QMap<int, QString>::const_iterator pv = values.constFind (idVal);
        QString v_str;
        if (pv != values.constEnd())
            v_str = pv.value();
        lE->setText (v_str);
        KKSAttrValue * av(0);// = sDial->getSyncWidget();//pObjEx->attrValue (attrId);
        KKSValue val (QString::number (idVal), KKSAttrType::atList);
        KKSCategoryAttr * cAttr = KKSCategoryAttr::create (attr, true, false);
        av = new KKSAttrValue (val, cAttr);
        sDial->getSyncWidget()->setSyncAttrType (av);//setSyncAttrVal (av);
        cAttr->release ();
        av->release();
        
        attr->release ();
    }
    
    refObj->release ();
}

void KKSObjEditorFactory :: loadSyncAddAttrRef (KKSAttrValue * avE, QAbstractItemModel * sMod)
{
    if (!avE || !sMod)
        return;

    QString tableName = avE->attribute()->tableName ();
    qDebug () << __PRETTY_FUNCTION__ << tableName;
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
        return;
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    KKSSyncDialog * wEditor = qobject_cast<KKSSyncDialog *>(this->sender());
    KKSList<const KKSFilterGroup*> filters;
    KKSCategory * ct = c->tableCategory ();
    if (!ct){
        refObj->release();
        return;
    }

    QStringList sl;
    for (int i=0; i<sMod->rowCount(); i++)
        sl << QString::number (sMod->data (sMod->index (i, 0), Qt::UserRole).toInt());

    QString value = sl.isEmpty() ? QString ("select id from %1").arg (tableName) : QString ("select id from %1 where id not in (%2)").arg (tableName).arg (sl.join (","));
    
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

    KKSRecDialog * recEditor = this->createObjRecEditor (IO_IO_ID, 
                                                         refObj->id (), 
                                                         //editor->m_filters, 
                                                         filters, 
                                                         "",
                                                         c,
                                                         true, false,
                                                         wEditor ? wEditor->windowModality () : Qt::ApplicationModal,
                                                         wEditor, 
                                                         Qt::Dialog);
    if (!recEditor){
        refObj->release();
        return;
    }

    if (recEditor->exec () == QDialog::Accepted)
    {
        QStringList sl;
        KKSAttrValue * av = wEditor->getSyncWidget()->getSyncAttrVal();//wObjE->attrValue (avE->attribute()->id());
        if (!av)
        {
            QString s;
            KKSValue val (s, KKSAttrType::atCheckListEx);
            KKSObject * io = loader->loadIO (IO_IO_ID);
            if (!io){
                refObj->release();
                return;
            }

            KKSCategory * c = io->category ();
            if (!c){
                refObj->release();
                return;
            }

            c = c->tableCategory();
            if (!c){
                refObj->release();
                return;
            }

            KKSCategoryAttr * cAttr = c->attribute (av->attribute()->id());
            if (!cAttr){
                refObj->release();
                return;
            }

            av = new KKSAttrValue (val, cAttr);
        }
        
        if (!av)
        {
            qCritical() << tr("Cannot set value");
            QMessageBox::critical (wEditor, av->attribute()->title(), tr("Cannot set value"), QMessageBox::Ok, QMessageBox::NoButton);
        }
        else
        {
            sl = av->value().value().split (",");
            if (!sl.contains (QString::number (recEditor->getID())))
            {
                sl += QString::number (recEditor->getID());
                int n = sMod->rowCount();
                sMod->insertRows (n, 1);
                QModelIndex wIndex = sMod->index (n, 0);
                sMod->setData (wIndex, recEditor->getID(), Qt::UserRole);
                updateAttrModel (wIndex, sMod, recEditor->getID(), tableName, refObj);
                QString s = QString ("{%1}").arg (sl.join (","));
                KKSValue val (s, KKSAttrType::atCheckListEx);
                //qDebug () << __PRETTY_FUNCTION__ << sl << av->attribute()->id();
                av->setValue (val);
                const_cast<KKSAttrValue *>(avE)->setValue (val);
//                wEditor->setObjChanged (true);
            }
        }
    }
}

void KKSObjEditorFactory :: loadSyncDelAttrRef (KKSAttrValue * avE, const QModelIndex& oInd, QAbstractItemModel * sMod)
{
    if (!avE || !sMod || !oInd.isValid())
        return;
    
    QString tableName = avE->attribute()->tableName ();
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
        return;
    
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    KKSSyncDialog * wEditor = qobject_cast<KKSSyncDialog *>(this->sender());

    int res = QMessageBox::question (wEditor, 
                                     tr("Delete value from %1").arg (avE->attribute()->title()), 
                                     tr("Do you really want to delete ?"), 
                                     QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);

    if (res == QMessageBox::Yes)
    {
        QStringList sl;
        KKSAttrValue * av = wEditor->getSyncWidget()->getSyncAttrVal();// wObjE->attrValue (avE->attribute()->id());
        qDebug () << __PRETTY_FUNCTION__ << (av ? av->value().value() : QString());
        if (av)
            sl = av->value().value().split (",");
        if (sl.contains (QString::number (oInd.data (Qt::UserRole).toInt())))
        {
            int ipos = sl.indexOf (QString::number (oInd.data (Qt::UserRole).toInt()));
            sl.removeAt (ipos);
            qDebug () << __PRETTY_FUNCTION__ << sl;
            int row = oInd.row();
            QModelIndex par = oInd.parent();
            sMod->removeRows (row, 1, par);
            int nr = sMod->rowCount (par);
            KKSValue val (sl.join (","), KKSAttrType::atCheckListEx);
            av->setValue (val);
            const_cast<KKSAttrValue *>(avE)->setValue (val);
            for (int i=0; i<nr; i++)
                sMod->setData (sMod->index (i, 0, par), sl[i].toInt(), Qt::UserRole);
        }
    }
    
    refObj->release();
}

/* Метод обновляет модель значений атрибута типа многие-ко-многим.
 */
void KKSObjEditorFactory :: updateAttrModel (const QModelIndex & wIndex, QAbstractItemModel * sMod, int id, QString tableName, KKSObject * refIO)
{
    if (!wIndex.isValid() || !sMod || id <=0 || tableName.isEmpty() || !refIO)
        return;
    int row = wIndex.row ();
    KKSList<const KKSFilterGroup*> filters;
    QString value = QString ("select id from %1 where id=%2").arg (tableName).arg(id);
    KKSCategory * c = refIO->category();
    if (!c)
        return;
    KKSCategory * ct = c->tableCategory ();
    if (!ct)
        return;
    const KKSFilter * f = ct->createFilter (ATTR_ID, value, KKSFilter::foInSQL);
    KKSList <const KKSFilter *> fl;
    fl.append (f);
    f->release ();
    KKSFilterGroup * fg = new KKSFilterGroup (true);
    fg->setFilters (fl);
    filters.append (fg);
    fg->release ();
    
    KKSMap<qint64, KKSEIOData *> eioList = loader->loadEIOList (refIO, filters);
    
    KKSMap<int, KKSCategoryAttr *> attrs = ct->attributes ();
    int ii=0;
    
    for (KKSMap<qint64, KKSEIOData *>::const_iterator pv = eioList.constBegin(); pv!=eioList.constEnd(); pv++)
    {
        int ic=0;
        
        KKSEIOData * eData = pv.value();
        if (qobject_cast<KKSEIODataModel *>(sMod))
        {
            sMod->setData (wIndex, QVariant::fromValue (eData), Qt::UserRole+1);
            continue;
        }
        for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = attrs.constBegin(); \
                                                            pa != attrs.constEnd(); \
                                                            pa++)
        {
            QString fValue = eData->fieldValue(pa.value()->code());

            
            QModelIndex saInd = sMod->index (row, ic);
            /*
            KKSAttrValue * av = wObjC->attrValue (pa.value()->id());
            QVariant val = av ? av->value().valueVariant () : QVariant();
            if (av->attribute()->type()->attrType() == KKSAttrType::atJPG)
                val = QObject::tr("<Image data %1>").arg (ii);
            else if (av->attribute()->type()->attrType() == KKSAttrType::atSVG)
                val = QObject::tr("<SVG data %1>").arg (ii);
            else if (av->attribute()->type()->attrType() == KKSAttrType::atXMLDoc)
                val = QObject::tr("<XML document %1>").arg (ii);
            else if (av->attribute()->type()->attrType() == KKSAttrType::atVideo)
                val = QObject::tr("<Video data %1>").arg (ii);
            else if (av->attribute()->type()->attrType() == KKSAttrType::atList ||
                        av->attribute()->type()->attrType() == KKSAttrType::atParent)
            {
                QVariant tVal(val);
                QMap<int, QString> refColumnValues;
                QMap<int, QString> avals = loader->loadAttributeValues(av->attribute(), refColumnValues, false, true, av->attribute()->tableName());
                QString cV = avals.value (tVal.toInt());
                val = cV;
            }
            */

            sMod->setData (saInd, pv.key(), Qt::UserRole);
            sMod->setData (saInd, fValue, Qt::DisplayRole);
            //sMod->setData (saInd, val, Qt::DisplayRole);
            ic++;
        }
        //wObjC->release ();
        ii++;
    }
}

/* Метод удаляет значение пользовательского атрибута-ссылки на набор элементов справочника.
 * Параметры:
 * wObj -- ИО, содержащий атрибут
 * attr -- атрибут-ссылка
 * isSystem -- не используется
 * sMod -- модель значений атрибутов
 */
void KKSObjEditorFactory :: loadObjDelAttrRef (KKSObject * wObj, const KKSAttrValue* avE, KKSIndAttr::KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd)
{
    Q_UNUSED (isSystem);

    if (!wObj || !avE || !sourceModel || !wInd.isValid())
        return;
    
    QString tableName = avE->attribute()->tableName ();
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
        return;
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    QWidget * wEditor = qobject_cast<QWidget *>(this->sender());

    int res = QMessageBox::question (wEditor, 
                                     tr("Delete value from %1").arg (avE->attribute()->title()),
                                     tr("Do you really want to delete ?"), 
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, 
                                     QMessageBox::Cancel);

    if (res == QMessageBox::Yes)
    {
        QStringList sl;
        KKSAttrValue * av = wObj->attrValueId (avE->attribute()->id());
        qDebug () << __PRETTY_FUNCTION__ << (av ? av->value().value() : QString());
        if (av)
            sl = av->value().value().split (",");
        if (sl.contains (QString::number (wInd.data (Qt::UserRole).toInt())))
        {
            int ipos = sl.indexOf (QString::number (wInd.data (Qt::UserRole).toInt()));
            sl.removeAt (ipos);
            qDebug () << __PRETTY_FUNCTION__ << sl;
            int row = wInd.row();
            QModelIndex par = wInd.parent();
            sourceModel->removeRows (row, 1, par);
            int nr = sourceModel->rowCount (par);
            KKSValue val (sl.join (","), KKSAttrType::atCheckListEx);
            av->setValue (val);
            const_cast<KKSAttrValue *>(avE)->setValue (val);
            for (int i=0; i<nr; i++)
                sourceModel->setData (sourceModel->index (i, 0, par), sl[i].toInt(), Qt::UserRole);
        }
    }

    refObj->release();
}

void KKSObjEditorFactory :: loadObjCDelAttrRef (KKSObjectExemplar * wObjE, const KKSAttrValue* avE, KKSIndAttr::KKSIndAttrClass isSystem, QAbstractItemModel * sourceModel, const QModelIndex& wInd)
{
    if (!wObjE || !avE || !sourceModel || !wInd.isValid())
        return;
    
    Q_UNUSED (isSystem);
    
    QString tableName = avE->attribute()->tableName ();
    KKSObject *refObj = loader->loadIO (tableName, true);
    if (!refObj)
        return;
    
    KKSCategory *c = refObj->category ();
    if (!c){
        refObj->release();
        return;
    }

    QWidget * wEditor = qobject_cast<QWidget *>(this->sender());

    int res = QMessageBox::question (wEditor, 
                                     tr("Delete value from %1").arg (avE->attribute()->title()), 
                                     tr("Do you really want to delete ?"), 
                                     QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);

    if (res == QMessageBox::Yes)
    {
        QStringList sl;
        KKSAttrValue * av = wObjE->attrValue (avE->attribute()->id());
        qDebug () << __PRETTY_FUNCTION__ << (av ? av->value().value() : QString());
        if (av)
            sl = av->value().value().split (",");
        if (sl.contains (QString::number (wInd.data (Qt::UserRole).toInt())))
        {
            int ipos = sl.indexOf (QString::number (wInd.data (Qt::UserRole).toInt()));
            sl.removeAt (ipos);
            qDebug () << __PRETTY_FUNCTION__ << sl;
            int row = wInd.row();
            QModelIndex par = wInd.parent();
            sourceModel->removeRows (row, 1, par);
            int nr = sourceModel->rowCount (par);
            KKSValue val (sl.join (","), KKSAttrType::atCheckListEx);
            av->setValue (val);
            const_cast<KKSAttrValue *>(avE)->setValue (val);
            for (int i=0; i<nr; i++)
                sourceModel->setData (sourceModel->index (i, 0, par), sl[i].toLongLong(), Qt::UserRole);
        }
    }
    
    refObj->release();
}

void KKSObjEditorFactory :: loadRefIO (QString tableName)
{
    KKSObjEditor * editor = qobject_cast<KKSObjEditor *>(this->sender());
    KKSObject * wObj = loader->loadIO (tableName, true);
    if (!wObj)
        return;
    int idObj = wObj->id ();
    bool isModal = editor->isModal ();
    QString s;
    this->editExistOE (editor, IO_IO_ID, idObj, wObj->category(), s, 0, isModal);
    wObj->release ();
}

void KKSObjEditorFactory :: loadRefRec (QString tableName, qint64 id)
{
    KKSObjEditor * editor = qobject_cast<KKSObjEditor *>(this->sender());
    KKSObject * wObj = loader->loadIO (tableName, true);
    if (!wObj)
        return;
    
    int idObj = wObj->id ();
    
    bool isModal = editor->isModal ();
    QString s;
    
    //this->editExistOE (editor, idObj, id, wObj->category()->tableCategory(), s, 0, isModal);
    this->editExistOE (editor, idObj, id, wObj->category(), s, 0, isModal);
    
    wObj->release ();
}

void KKSObjEditorFactory :: setRubrFactory (KKSRubricFactory * _rf)
{
    m_rf = _rf;
}

void KKSObjEditorFactory :: printReport(KKSObject* io)
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QPSQL7");
    KKSDatabase * k_db = loader->getDb();
    db.setHostName(k_db->getHost());
    db.setDatabaseName(k_db->getName());
    db.setUserName(k_db->getUser());
    qDebug () << __PRETTY_FUNCTION__ << k_db->getPort();//.toInt();
    db.setPort(5432);//k_db->getPort().toInt());
    db.setPassword(k_db->getPass());
    if (!db.open())
    {
        qCritical() << __PRETTY_FUNCTION__ << db.lastError ();
        qCritical() << tr("Current version of KKS SITO Operator does not print this window");
        QMessageBox::critical(0, tr("Error"), 
        		tr("Current version of KKS SITO Operator does not print this window"), QMessageBox::Ok);
        return;
    }
    QSqlQuery query(db);
    query.prepare("select report_source from report where id =:id;");
    query.bindValue(":id", io->attrValueId(ATTR_SHB)->value().valueVariant());
    query.exec();
//    qDebug() << __PRETTY_FUNCTION__ << query;

    QDomDocument _doc;
    QString sql = QString ("select report_source from report where id = %1;").arg (io->attrValueId(ATTR_SHB)->value().valueVariant().toInt());
    KKSResult * res = k_db->execute (sql);
    if (!res)
    {
        qCritical() << tr ("Cannot read report source from database");
        QMessageBox::critical(qobject_cast<QWidget *>(this->sender()), tr ("Report"),
                              tr ("Cannot read report source from database"),
                              QMessageBox::Ok,
                              QMessageBox::NoButton);
        return;
    }
/*
    query.first();
    _doc.setContent(query.value(0).toString());
    query.clear();
*/
    qDebug () << __PRETTY_FUNCTION__ << res->getCellAsString (0, 0);
    _doc.setContent (res->getCellAsString (0, 0));
    delete res;

    ORPreRender * pre = new ORPreRender;//new KKSPrerender;
    if (!pre)
        return;
    pre->setDom (_doc);
/*
    QString tblnm=io->tableName();//"eio_table_82";
    KKSMap<QString, KKSCategory*> tables = io->getAdditionalTables ( );
//   KKSMap<QString, KKSCategory *>::const_iterator p = tables.constBegin();
//	qDebug()<<"Additional tables";
//
//	tblnm = p.key();
//	qDebug()<<"Additional tables end";
//
    KKSMap<QString, KKSCategory*>::const_iterator i = tables.begin();
    while (i != tables.end() )
    {
         tblnm = i.key();
         break;
     }

    //query.prepare("select e.name, t. code as a_type, e.p_value from "+tblnm+" as e left join a_types as t on e.id_a_type=t.id;");
    sql = QString ("select e.name, t. code as a_type, e.p_value from %1  as e left join a_types as t on e.id_a_type=t.id;").arg (tblnm);
    ParameterList prm;
    res = k_db->execute (sql);
    if (res)//query.exec())
    {
//        QMessageBox::critical(this, tr("Error"), 
//        		tr("Current version of KKS SITO Operator does not print this window"), QMessageBox::Ok);

        //while (query.next())
        for (int i=0; i<res->getRowCount(); i++)
            prm.append (res->getCellAsString (i, 0), res->getCellAsInt (i, 2));
//                prm.append(query.value(0).toString(), query.value(2).toInt());
        pre.setParamList(prm);
        delete res;
    }
*/
    ORODocument * doc = pre->generate ();//((KKSPrerender *)pre)->generate();

    if (doc)
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setNumCopies( 1 );
        PreviewDialog preview (doc, &printer);
        if (preview.exec() == QDialog::Rejected) 
            return;

        ORPrintRender render;
        render.setupPrinter(doc, &printer);

        QPrintDialog pd(&printer);
        pd.setMinMax(1, doc->pages());
        if(pd.exec() == QDialog::Accepted)
        {
          render.setPrinter(&printer);
          render.render(doc);
        }
        delete doc;
    }

    delete pre;

}

int KKSObjEditorFactory :: putAttrsGroupsOnWidget ( KKSObject * obj,
                                                    KKSObjectExemplar *wObjE,
                                                    KKSObjEditor * editor,
                                                    int& nc,
                                                    const KKSCategory *c,
                                                    QString tableName,
                                                    KKSAttrGroup * aGroup,
                                                    QGridLayout *gbLay,
                                                    QGridLayout * gAttrLayout,
                                                    bool isGrouped)
{
    if (!aGroup)
        return 0;

    KKSMap<int, KKSAttrView*> attrs = aGroup->attrViews();
    KKSList<KKSAttrView *> attrs_list;
    for (KKSMap<int, KKSAttrView*>::const_iterator pa = attrs.constBegin(); pa != attrs.constEnd(); pa++)
        attrs_list.append (pa.value());
    qSort (attrs_list.begin(), attrs_list.end(), compareAttrViews);

    //
    // проверка на то, что группа может содержать только атрибуты с типом KKSAttrType::atCheckListEx
    //
    bool isAtCheckListEx = !aGroup->childGroups().isEmpty();//false;
    for (int ii=0; ii<attrs_list.count() && !isAtCheckListEx; ii++)
        isAtCheckListEx = isAtCheckListEx || (attrs_list[ii]->type()->attrType() != KKSAttrType::atCheckListEx);

    if (!isAtCheckListEx && isGrouped)
        return 0;

    int n_str=0;
    for (int ii=0; ii<attrs_list.count() && isAtCheckListEx; ii++)
    {
        KKSAttrView * a = attrs_list[ii];
        if (!a->isVisible() || (obj->id () == IO_IO_ID && (a->id () == ATTR_NAME || a->id () == ATTR_MACLABEL)) )
            continue;

        //int id = a->id();
        
        KKSAttrValue * av = NULL;
        //bool isAvalsSet = false;
        
        KKSMap<qint64, KKSAttrValue*> sysAttrValue = editor->getSysAttrValues();
        for (KKSMap<qint64, KKSAttrValue*>::const_iterator pa = sysAttrValue.constBegin(); pa != sysAttrValue.constEnd(); pa++)
        {
            KKSAttrValue * av1 = pa.value();
            if(av1->attribute()->id() == a->id()){
                av = av1;
                break;
            }
        }

        if (av)
            av->addRef ();


        //for (int iii=0; iii<wObjE->attrValues().count() && !isAvalsSet; iii++)
        //    isAvalsSet = (wObjE->attrValueIndex (iii)->attribute()->id() == a->id());
        
        //if (wObjE->id() <= 0 && !isAvalsSet)
        //    av = new KKSAttrValue (a->defValue(), a);
        //else
        //{
            //av = wObjE->attrValue (id);

            //if (av)
            //    av->addRef ();
            //else
            //{
            //    KKSValue v;
            //    if (!a->defValue().value().isEmpty())
            //        v = a->defValue ();
            //    av = new KKSAttrValue(v, a);
            //}
        //}

        if (a->type()->attrType() != KKSAttrType::atCheckListEx)
            n_str++;

        if (av && av->id() < 0)
            qDebug () << __PRETTY_FUNCTION__ << av->id();
        if (isGrouped)
            m_awf->putAttrWidget (av, 
                                  editor, 
                                  gbLay, 
                                  n_str,
#ifdef Q_CC_MSVC
                                  KKSIndAttr::KKSIndAttrClass::iacTableAttr,
#else
                                  KKSIndAttr::iacTableAttr,
#endif
                                  (tableName.isEmpty () ? obj->tableName() : tableName), 
                                  (c ? c->id():-1));
        else
        {
            gAttrLayout->setVerticalSpacing (10);
            m_awf->putAttrWidget (av, 
                                  editor, 
                                  gAttrLayout, 
                                  nc, 
#ifdef Q_CC_MSVC
                                  KKSIndAttr::KKSIndAttrClass::iacTableAttr,
#else
                                  KKSIndAttr::iacTableAttr,
#endif
                                  obj->tableName(), 
                                  (c ? c->id():-1));

            if (a->type()->attrType() != KKSAttrType::atCheckListEx)
                nc++;
        }

        av->release ();
    }

    KKSMap<int, KKSAttrGroup *> subGroups = aGroup->childGroups ();
    for (KKSMap<int, KKSAttrGroup *>::const_iterator pg = subGroups.constBegin(); \
            pg != subGroups.constEnd(); \
            pg++)
    {
        QGroupBox * gb = new QGroupBox ();
        gb->setTitle (pg.value()->name());
        QGridLayout * gLay = new QGridLayout (gb);
        gLay->setContentsMargins (0, 0, 0, 0);
        int ngr = putAttrsGroupsOnWidget (obj, wObjE, editor, nc, c, tableName, pg.value(), gLay, gAttrLayout, true);
        n_str++;
        qDebug () << __PRETTY_FUNCTION__ << ngr << gbLay->columnCount ();
        gbLay->addWidget (gb, n_str, 0, 1, 3);//, Qt::AlignHCenter);
    }

    return n_str;
}

int KKSObjEditorFactory :: putRecAttrsGroupsOnWidget ( KKSObject * obj,
                                                       KKSObjectExemplar *wObjE,
                                                       KKSObjEditor * editor,
                                                       int& nc,
                                                       const KKSCategory *c,
                                                       QString tableName,
                                                       KKSAttrGroup * aGroup,
                                                       QGridLayout *gbLay,
                                                       QGridLayout * gAttrLayout,
                                                       bool isGrouped)
{
    if (!aGroup)
        return 0;

    KKSMap<int, KKSAttrView*> attrs = aGroup->attrViews();
    KKSList<KKSAttrView *> attrs_list;
    for (KKSMap<int, KKSAttrView*>::const_iterator pa = attrs.constBegin(); pa != attrs.constEnd(); pa++)
        attrs_list.append (pa.value());
    qSort (attrs_list.begin(), attrs_list.end(), compareAttrViews);

    //
    // проверка на то, что группа может содержать только атрибуты с типом KKSAttrType::atCheckListEx
    //
    bool isAtCheckListEx = !aGroup->childGroups().isEmpty();//false;
    for (int ii=0; ii<attrs_list.count() && !isAtCheckListEx; ii++)
        isAtCheckListEx = isAtCheckListEx || (attrs_list[ii]->type()->attrType() != KKSAttrType::atCheckListEx);

    if (!isAtCheckListEx && isGrouped)
        return 0;

    int n_str=0;
    for (int ii=0; ii<attrs_list.count() && isAtCheckListEx; ii++)
    {
        KKSAttrView * a = attrs_list[ii];
        if (!a->isVisible() || (obj->id () == IO_IO_ID && (a->id () == ATTR_NAME || a->id () == ATTR_MACLABEL)) )
            continue;

        //int id = a->id();
        
        KKSAttrValue * av = NULL;
        //bool isAvalsSet = false;
        
        KKSMap<qint64, KKSAttrValue*> recAttrValue = editor->getRecAttrValues();
        for (KKSMap<qint64, KKSAttrValue*>::const_iterator pa = recAttrValue.constBegin(); pa != recAttrValue.constEnd(); pa++)
        {
            KKSAttrValue * av1 = pa.value();
            if(av1->attribute()->id() == a->id()){
                av = av1;
                break;
            }
        }

        if (av)
            av->addRef ();


        //for (int iii=0; iii<wObjE->attrValues().count() && !isAvalsSet; iii++)
        //    isAvalsSet = (wObjE->attrValueIndex (iii)->attribute()->id() == a->id());
        
        //if (wObjE->id() <= 0 && !isAvalsSet)
        //    av = new KKSAttrValue (a->defValue(), a);
        //else
        //{
            //av = wObjE->attrValue (id);

            //if (av)
            //    av->addRef ();
            //else
            //{
            //    KKSValue v;
            //    if (!a->defValue().value().isEmpty())
            //        v = a->defValue ();
            //    av = new KKSAttrValue(v, a);
            //}
        //}

        if (a->type()->attrType() != KKSAttrType::atCheckListEx)
            n_str++;

        if (av && av->id() < 0)
            qDebug () << __PRETTY_FUNCTION__ << av->id();
        if (isGrouped)
            m_awf->putAttrWidget (av, 
                                  editor, 
                                  gbLay, 
                                  n_str, 
#ifdef Q_CC_MSVC
                                  KKSIndAttr::KKSIndAttrClass::iacEIOUserAttr, 
#else
                                  KKSIndAttr::iacEIOUserAttr, 
#endif
                                  (tableName.isEmpty () ? obj->tableName() : tableName), 
                                  (c ? c->id():-1));
        else
        {
            gAttrLayout->setVerticalSpacing (10);
            m_awf->putAttrWidget (av, 
                                  editor, 
                                  gAttrLayout, 
                                  nc, 
#ifdef Q_CC_MSVC
                                  KKSIndAttr::KKSIndAttrClass::iacEIOUserAttr, 
#else
                                  KKSIndAttr::iacEIOUserAttr, 
#endif
                                  obj->tableName(), 
                                  (c ? c->id():-1));

            if (a->type()->attrType() != KKSAttrType::atCheckListEx)
                nc++;
        }

        av->release ();
    }

    KKSMap<int, KKSAttrGroup *> subGroups = aGroup->childGroups ();
    for (KKSMap<int, KKSAttrGroup *>::const_iterator pg = subGroups.constBegin(); \
            pg != subGroups.constEnd(); \
            pg++)
    {
        QGroupBox * gb = new QGroupBox ();
        gb->setTitle (pg.value()->name());
        QGridLayout * gLay = new QGridLayout (gb);
        gLay->setContentsMargins (0, 0, 0, 0);
        int ngr = putRecAttrsGroupsOnWidget (obj, wObjE, editor, nc, c, tableName, pg.value(), gLay, gAttrLayout, true);
        n_str++;
        qDebug () << __PRETTY_FUNCTION__ << ngr << gbLay->columnCount ();
        gbLay->addWidget (gb, n_str, 0, 1, 3);//, Qt::AlignHCenter);
    }

    return n_str;
}

void KKSObjEditorFactory :: putAttrsGroupsOnWidget (KKSObject * obj, 
                                                    KKSObjEditor * editor, 
                                                    int& nc, 
                                                    const KKSCategory *c, 
                                                    KKSAttrGroup * aGroup, 
                                                    QGridLayout *gbLay, 
                                                    QGridLayout * gAttrLayout, 
                                                    bool isGrouped, 
                                                    bool updateView)
{
    if (!aGroup)
        return;

    KKSMap<int, KKSAttrView*> attrs = aGroup->attrViews();
    QList<KKSAttrView *> attrs_list = attrs.values();
    qSort (attrs_list.begin(), attrs_list.end(), compareAttrViews);

    //
    // проверка на то, что группа может содержать только атрибуты с типом KKSAttrType::atCheckListEx
    //
    bool isAtCheckListEx = false;
    for (int ii=0; ii<attrs_list.count() && !isAtCheckListEx; ii++)
        isAtCheckListEx = isAtCheckListEx || (attrs_list[ii]->type()->attrType() != KKSAttrType::atCheckListEx);

    if (!isAtCheckListEx && isGrouped)
        return;

    int n_str=0;
    for (KKSMap<int, KKSCategoryAttr*>::const_iterator pa = c->attributes().constBegin(); pa != c->attributes().constEnd(); pa++)
    {
        KKSAttrValue *av = obj->attrValueId (pa.value()->id());//key());
        qDebug () << __PRETTY_FUNCTION__ << av << (av ? av->value().value() : 0) << pa.value()->id() << obj->attrValueId (pa.value()->id());
    }
    for (int ii=0; ii<attrs_list.count(); ii++)
    {
        KKSAttrView * a = attrs_list[ii];
        //qDebug () << __PRETTY_FUNCTION__ << (a->type()->attrType() == KKSAttrType::atCheckListEx) << updateView << a->isVisible();
        if (!a->isVisible() || (a->type()->attrType() == KKSAttrType::atCheckListEx && updateView))
            continue;

        //QString code = a->code();
        //int id = a->id();
        
        KKSAttrValue * av = NULL;
        //bool isAvalsSet = false;
        
        KKSMap<qint64, KKSAttrValue*> ioAttrValue = editor->getIOAttrValues();
        for (KKSMap<qint64, KKSAttrValue*>::const_iterator pa = ioAttrValue.constBegin(); pa != ioAttrValue.constEnd(); pa++)
        {
            KKSAttrValue * av1 = pa.value();
            if(av1->attribute()->id() == a->id()){
                av = av1;
                break;
            }
        }

        if (av)
            av->addRef ();
        
        //for (int iii=0; iii<obj->attrValues().count() && !isAvalsSet; iii++)
        //    isAvalsSet = (obj->attrValue (iii)->attribute()->id() == a->id());
        //if (obj->id() <= 0 && !isAvalsSet)
        //    av = new KKSAttrValue (a->defValue(), a);
        //else
        //{
        //    av = obj->attrValueId (id);

        //    if (av)
        //        av->addRef ();
        //    else
        //    {
        //        KKSValue v;
        //        if (!a->defValue().value().isEmpty())
        //            v = a->defValue ();
        //        av = new KKSAttrValue(v, a);
        //    }
        //}
        if (av->id() < 0)
            qDebug () << __PRETTY_FUNCTION__ << av->id();        
        if (a->type()->attrType() != KKSAttrType::atCheckListEx)
            n_str++;

        if (isGrouped)
            m_awf->putAttrWidget (av, 
                                  editor, 
                                  gbLay, 
                                  n_str, 
#ifdef Q_CC_MSVC
                                  KKSIndAttr::KKSIndAttrClass::iacIOUserAttr, 
#else
                                  KKSIndAttr::iacIOUserAttr, 
#endif
                                  obj->tableName(), 
                                  (c ? c->id():-1));
        else
        {
            gAttrLayout->setVerticalSpacing (10);
            m_awf->putAttrWidget (av, 
                                  editor, 
                                  gAttrLayout, 
                                  nc, 
#ifdef Q_CC_MSVC
                                  KKSIndAttr::KKSIndAttrClass::iacIOUserAttr, 
#else
                                  KKSIndAttr::iacIOUserAttr, 
#endif
                                  obj->tableName(), 
                                  (c ? c->id():-1));

            nc++;
        }
        av->release ();
    }

    KKSMap<int, KKSAttrGroup *> subGroups = aGroup->childGroups ();
    for (KKSMap<int, KKSAttrGroup *>::const_iterator pg = subGroups.constBegin(); \
            pg != subGroups.constEnd(); \
            pg++)
    {
        n_str++;
        QGroupBox * gb = new QGroupBox ();
        gb->setTitle (pg.value()->name());
        qDebug () << __PRETTY_FUNCTION__ << gbLay->columnCount ();
        gbLay->addWidget (gb, n_str, 0, 1, 3);
        QGridLayout * gLay = new QGridLayout ();
        gLay->setContentsMargins (0, 0, 0, 0);
        gb->setLayout (gLay);
        putAttrsGroupsOnWidget (obj, editor, nc, c, pg.value(), gLay, gAttrLayout, true, updateView);
    }
}

void KKSObjEditorFactory :: putIndicatorsGroupsOnToWidget (KKSObject * obj, KKSObjEditor * editor, int& nc, const KKSMap<int, KKSIndicatorValue*>& indMap, QGridLayout *gbLay, QGridLayout * gIndLayout, bool isGrouped, bool updateView)
{
    if (!obj || !editor || !gIndLayout || indMap.isEmpty())
        return;

    Q_UNUSED (updateView);
    int n_str (nc);
    for (KKSMap<int, KKSIndicatorValue*>::const_iterator p=indMap.constBegin();
         p!=indMap.constEnd();
         ++p)
    {
        if (isGrouped)
            m_indf->putIndWidget(p.value(), editor, gbLay, n_str);
        else
        {
            gIndLayout->setVerticalSpacing (10);
            m_indf->putIndWidget (p.value(), editor, gIndLayout, n_str);
        }
        n_str++;
    }
//    QSpacerItem * indLay = new QSpacerItem (20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);
//    gIndLayout->addItem (indLay, n_str, 0, 1, 4);
    nc = n_str;
}

void KKSObjEditorFactory :: putSystemParams (KKSObjectExemplar * recio,
                                             KKSObjEditor * editor,
                                             QTabWidget * tabObj,
                                             int pageNum)
{
    QWidget * pSysWidget = new QWidget (editor);
    QScrollArea * scSysArea = new QScrollArea (pSysWidget);
    QGridLayout * gLaySys = new QGridLayout (pSysWidget);
    gLaySys->addWidget (scSysArea, 0, 0, 1, 1);
    QWidget * pSysPW = new QWidget ();
    scSysArea->setWidget (pSysPW);
    QGridLayout * gLay = new QGridLayout (scSysArea);

    QGroupBox * gb1 = new QGroupBox(tr("Name:"), pSysPW);
    QGroupBox * gb2 = new QGroupBox(tr("Visualization:"), pSysPW);
    QGroupBox * gb3 = new QGroupBox(tr("Identification:"), pSysPW);
    QList<QGroupBox *> gbList;
    gbList.append(gb1);
    gbList.append(gb2);
    gbList.append(gb3);
    
    QGridLayout * gbLay1 = new QGridLayout(gb1);
    QGridLayout * gbLay2 = new QGridLayout(gb2);
    QGridLayout * gbLay3 = new QGridLayout(gb3);
    QList<QGridLayout *> gbLayList;
    gbLayList.append(gbLay1);
    gbLayList.append(gbLay2);
    gbLayList.append(gbLay3);

    gLay->addWidget(gb1, 0, 0, 1, 1);
    gLay->addWidget(gb2, 1, 0, 1, 1);
    gLay->addWidget(gb3, 2, 0, 1, 1);


    QStringList lSysNames;
    lSysNames << tr("ID*: ")
              << tr("Name: ")
              << tr("State*: ")
              << tr("Icon: ")
              << tr("Fill color: ")
              << tr("Text color: ")
              << tr("Unique ID (UUID_OSSP)*: ")
              << tr("DynamicDocs UID*: ")
              << tr("Last update*: ");

    int n = lSysNames.count ();
    for (int i=0; i<n; i++)
    {
        QGroupBox *currentGroupBox = NULL;
        QGridLayout * currentLayout = NULL;

        int index = 0;
        if(i <= 2){
            currentGroupBox = gbList.at(0);
            currentLayout = gbLayList.at(0);
            index = i;
        }
        else if(i <= 5){
            currentGroupBox = gbList.at(1);
            currentLayout = gbLayList.at(1);
            index = i-3;
        }
        else{
            currentGroupBox = gbList.at(2);
            currentLayout = gbLayList.at(2);
            index = i-5;
        }

        QLabel * l = new QLabel (lSysNames[i], currentGroupBox);

        currentLayout->addWidget(l, index, 0, 1, 1);//, Qt::AlignRight | Qt::AlignVCenter);

        QHBoxLayout * edLay = new QHBoxLayout;
        QWidget * lE = 0;

        if(i == 8){
            lE = new QDateTimeEdit (currentGroupBox);
            edLay->addWidget (lE);
            QFont f  = l->font();
            f.setBold(true);
            l->setFont(f);
        }
        else if(i == 3){
            KKSAttribute * attr = new KKSAttribute();
            attr->setId(ATTR_R_ICON);
            KKSCategoryAttr * ca = new KKSCategoryAttr (*attr);
            KKSAttrValue * av = new KKSAttrValue;//recio->attrValue(ATTR_RECORD_FILL_COLOR);
            av->setAttribute(ca);
            av->setId(attr->id());
            attr->release();
            ca->release();
            lE = new KKSPixmap(av, KKSIndAttr::iacEIOSysAttr, recio->iconAsString(), currentGroupBox);
            edLay->addWidget(lE, Qt::AlignCenter | Qt::AlignVCenter);
            l->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
            ((KKSPixmap *)lE)->setAlignment( Qt::AlignCenter | Qt::AlignVCenter);
            lE->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

            QToolButton *tbRef = new QToolButton ();
            tbRef->setMinimumHeight (20);
            tbRef->setText ("...");
            QObject::connect (tbRef, SIGNAL(pressed()), lE, SLOT(openFile()));
            edLay->addWidget (tbRef);
            lE->setMinimumHeight (20);
            QObject::connect (lE, 
                              SIGNAL (valueChanged(qint64, KKSIndAttr::KKSIndAttrClass, QVariant)), 
                              editor, 
                              SLOT (setValue (qint64, KKSIndAttr::KKSIndAttrClass, QVariant)) );
        }
        else if(i == 4 || i == 5)
        {
            QColor rgb_color;
            KKSAttribute * attr = new KKSAttribute();
            attr->setId(i==4 ? ATTR_RECORD_FILL_COLOR : ATTR_RECORD_TEXT_COLOR);
            KKSCategoryAttr * ca = new KKSCategoryAttr (*attr);
            KKSAttrValue * av = new KKSAttrValue;//recio->attrValue(ATTR_RECORD_FILL_COLOR);
            av->setAttribute(ca);
            av->setId(attr->id());
            attr->release();
            ca->release();
            if(i == 4){
                rgb_color = recio->recordFillColor();
                if (rgb_color.isValid())
                    av->setValue (KKSValue(QString::number(rgb_color.rgba()), KKSAttrType::atRecordColor));
                lE = new KKSColorWidget(av, KKSIndAttr::iacEIOSysAttr, rgb_color, KKSAttrType::atRecordColor, currentGroupBox);
            }
            else{
                rgb_color = recio->recordTextColor();
                if (rgb_color.isValid())
                    av->setValue (KKSValue(QString::number(rgb_color.rgba()), KKSAttrType::atRecordTextColor));
                lE = new KKSColorWidget(av, KKSIndAttr::iacEIOSysAttr, rgb_color, KKSAttrType::atRecordTextColor, currentGroupBox);

            }
            av->release();
            QObject::connect (lE, 
                              SIGNAL (valueChanged(qint64, KKSIndAttr::KKSIndAttrClass, QVariant)), 
                              editor, 
                              SLOT (setValue (qint64, KKSIndAttr::KKSIndAttrClass, QVariant)) );

            QSizePolicy hPwt (QSizePolicy::Expanding, QSizePolicy::Fixed);//Expanding);
            lE->setSizePolicy (hPwt);
            lE->setMinimumHeight (20);
            edLay->addWidget (lE, Qt::AlignCenter);

        }
        else{
            lE = new QLineEdit (currentGroupBox);
            edLay->addWidget (lE);
        }
        
        if (qobject_cast<QLineEdit *>(lE))
            (qobject_cast<QLineEdit *>(lE))->setReadOnly(true);
        else if (qobject_cast<QDateTimeEdit *>(lE))
            (qobject_cast<QDateTimeEdit *>(lE))->setReadOnly(true);
                
        
        switch (i)
        {
            case 0: {
                        (qobject_cast<QLineEdit *>(lE))->setText (QString::number(recio->id())); 
                        QFont f = l->font();
                        f.setBold(true);
                        l->setFont(f);
                        break;
                    }
            case 1: (qobject_cast<QLineEdit *>(lE))->setText (recio->name()); break;
            case 2:
            {
                QToolButton * tbState = new QToolButton (currentGroupBox);
                tbState->setText(tr("..."));
                edLay->addWidget (tbState);
                const KKSState * st = recio->state();
                if(st)
                    (qobject_cast<QLineEdit *>(lE))->setText (st->name());
                else
                    (qobject_cast<QLineEdit *>(lE))->setText (tr("Active"));

                QFont f = l->font();
                f.setBold(true);
                l->setFont(f);
                editor->setStateWidget (lE);
                connect (tbState, SIGNAL (clicked()), editor, SLOT (setStateList()) );
                break;
            }
            //case 3: (qobject_cast<QLineEdit *>(lE))->setText (recio->iconAsString()); break;
            //case 4: (qobject_cast<QLineEdit *>(lE))->setText (QString::number((int)(recio->recordFillColor().rgba()))); break;
            //case 5: (qobject_cast<QLineEdit *>(lE))->setText (QString::number((int)(recio->recordTextColor().rgba()))); break;
            case 6: 
            {
                (qobject_cast<QLineEdit *>(lE))->setText (recio->uuid()); 
                QFont f = l->font();
                f.setBold(true);
                l->setFont(f);
                break;
            }
            case 7: 
            {
                (qobject_cast<QLineEdit *>(lE))->setText (recio->uniqueId()); 
                QFont f = l->font();
                f.setBold(true);
                l->setFont(f);
                break;
            }
            case 8: 
            {
                (qobject_cast<QDateTimeEdit *>(lE))->setDateTime (recio->lastUpdate()); 
                QFont f = l->font();
                f.setBold(true);
                l->setFont(f);
                break;
            }
            default: break;
        }

        currentLayout->addLayout(edLay, index, 1, 1, 1);//, Qt::AlignJustify | Qt::AlignVCenter);
    }

    gLay->setRowStretch(n, 1);
    tabObj->insertTab(pageNum, pSysWidget, tr("System parameters"));
}

void KKSObjEditorFactory :: putRubricator (KKSObject * obj, KKSObjEditor * editor, QTabWidget * tabObj)
{
    if (!obj || !editor || ! tabObj)
        return;
    //
    // Рубрикатор
    //
    //QWidget * includesW = new QWidget ();
    QSizePolicy iwSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);// MinimumExpanding
    //includesW->setSizePolicy (iwSizePolicy);
    //qDebug () << __PRETTY_FUNCTION__ << includesW->sizePolicy ();
    //QGridLayout *gIncludesLay = new QGridLayout (includesW);
    KKSIncludesWidget * iW = new KKSIncludesWidget (obj->rootRubric(), true, false, false, false);//includesW);
//    qDebug () << __FUNCTION__ << iW->sizePolicy().horizontalPolicy() << iW->sizePolicy().verticalPolicy();
    //iW->setSizePolicy (iwSizePolicy);
    if (iW && m_rf)
    {
        connect (iW, SIGNAL (saveRubric (KKSRubric *, bool)), m_rf, SLOT (saveRubric (KKSRubric *, bool)) );
        connect (iW, SIGNAL (rubricItemRequested (const KKSRubric*, bool, QAbstractItemModel *)), m_rf, SLOT (rubricItemUpload(const KKSRubric*, bool, QAbstractItemModel *)) );
        connect (iW, SIGNAL (rubricItemCreationRequested (const KKSRubric *, QAbstractItemModel*, const QModelIndex&)), m_rf, SLOT (rubricItemCreate(const KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
        connect (iW, SIGNAL (openRubricItemRequested (int)), m_rf, SLOT (openRubricItem (int)) );
        connect (iW, SIGNAL (loadStuffModel (RubricForm *)), m_rf, SLOT (loadRubricPrivilegies(RubricForm *)) );
        connect (iW, SIGNAL (loadSearchtemplate (RubricForm *)), m_rf, SLOT (loadSearchTemplate (RubricForm *)) );
        connect (iW, SIGNAL (loadCategory (RubricForm *)), m_rf, SLOT (loadCategory (RubricForm *)) );
        connect (iW, SIGNAL (rubricAttachmentsView (QAbstractItemModel *, const KKSRubric *)), m_rf, SLOT (viewAttachments (QAbstractItemModel *, const KKSRubric *)) );
        connect (iW, SIGNAL (copyFromRubr(KKSRubric *, QAbstractItemModel *, const QModelIndex&)), m_rf, SLOT (copyFromRubric (KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
        connect (iW, SIGNAL (initAttachmentsModel (const KKSRubric *, bool)), m_rf, SLOT (initRubricAttachments (const KKSRubric *, bool)) );
        connect (iW, SIGNAL (appendRubricItemIntoModel (QAbstractItemModel *, const KKSRubricItem * )), m_rf, SLOT (appendRubricItem (QAbstractItemModel *, const KKSRubricItem *)) );

        connect (m_rf, SIGNAL (rubricAttachments (QAbstractItemModel *)), iW, SLOT (slotInitAttachmentsModel (QAbstractItemModel *)) );
    }
    QTreeView *tv = iW->tvRubr();
    KKSEventFilter *ef = new KKSEventFilter (iW);
    tv->viewport()->installEventFilter (ef);

    //connect(this, SIGNAL(includeSelected(int, QString)), objEditorWidget, SLOT(slotIncludeSelected(int, QString)));
    editor->addIncludesWidget (iW);
    tabObj->addTab (iW/*includesW*/, tr("Assotiated IO"));
    //gIncludesLay->addWidget (iW, 0, 0, 1, 1);
}

void KKSObjEditorFactory :: putRubricator (KKSObjectExemplar * eio, KKSObjEditor * editor, QTabWidget * tabObj)
{
    if (!eio || !editor || ! tabObj)
        return;
    //
    // Рубрикатор
    //
    //QWidget * includesW = new QWidget ();
    QSizePolicy iwSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    //includesW->setSizePolicy (iwSizePolicy);
    //qDebug () << __PRETTY_FUNCTION__ << includesW->sizePolicy ();
    //QGridLayout *gIncludesLay = new QGridLayout (includesW);
    KKSIncludesWidget * iW = new KKSIncludesWidget (eio->rootRubric(), true, false, false, true);// includesW);
    iW->setSizePolicy (iwSizePolicy);
    if (iW && m_rf)
    {
        connect (iW, SIGNAL (saveRubric (KKSRubric *, bool)), m_rf, SLOT (saveRubric (KKSRubric *, bool)) );
        connect (iW, SIGNAL (rubricItemRequested (const KKSRubric*, bool, QAbstractItemModel *)), m_rf, SLOT (rubricItemUpload(const KKSRubric*, bool, QAbstractItemModel *)) );
        connect (iW, SIGNAL (rubricItemCreationRequested (const KKSRubric *, QAbstractItemModel*, const QModelIndex&)), m_rf, SLOT (rubricItemCreate(const KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
        //connect (iW, SIGNAL (openRubricItemRequested (int)), m_rf, SLOT (openRubricItem (int)) );
        connect (iW, SIGNAL (loadStuffModel (RubricForm *)), m_rf, SLOT (loadRubricPrivilegies(RubricForm *)) );
        connect (iW, SIGNAL (loadSearchtemplate (RubricForm *)), m_rf, SLOT (loadSearchTemplate (RubricForm *)) );
        connect (iW, SIGNAL (loadCategory (RubricForm *)), m_rf, SLOT (loadCategory (RubricForm *)) );
        connect (iW, SIGNAL (rubricAttachmentsView (QAbstractItemModel *, const KKSRubric *)), m_rf, SLOT (viewAttachments (QAbstractItemModel *, const KKSRubric *)) );
        connect (iW, SIGNAL (copyFromRubr(KKSRubric *, QAbstractItemModel *, const QModelIndex&)), m_rf, SLOT (copyFromRubric (KKSRubric *, QAbstractItemModel *, const QModelIndex&)) );
        connect (iW, SIGNAL (initAttachmentsModel (const KKSRubric *, bool)), m_rf, SLOT (initRubricAttachments (const KKSRubric *, bool)) );
        connect (iW, SIGNAL (appendRubricItemIntoModel (QAbstractItemModel *, const KKSRubricItem * )), m_rf, SLOT (appendRubricItem (QAbstractItemModel *, const KKSRubricItem *)) );
        //connect (iW, SIGNAL (appendRubricRecord (int, const KKSRubric *, QAbstractItemModel *, const QModelIndex& )), m_rf, SLOT (appendRecord (int, const KKSRubric *, QAbstractItemModel *, const QModelIndex& )) );

        connect (m_rf, SIGNAL (rubricAttachments (QAbstractItemModel *)), iW, SLOT (slotInitAttachmentsModel (QAbstractItemModel *)) );
//        connect (eiW, SIGNAL (loadStuffModel(RubricForm *)), m_rf, SLOT (loadRubricPrivilegies(RubricForm *)) );
//        connect (eiW, SIGNAL (loadSearchtemplate (RubricForm *)), m_rf, SLOT (loadSearchTemplate (RubricForm *)) );
//        connect (eiW, SIGNAL (loadCategory (RubricForm *)), m_rf, SLOT (loadCategory (RubricForm *)) );
//        connect (eiW, SIGNAL (rubricAttachmentsView (QAbstractItemModel *, const KKSRubric *)), m_rf, SLOT (viewAttachments (QAbstractItemModel *, const KKSRubric *)) );
    }
    QTreeView *tv = iW->tvRubr();
    KKSEventFilter *ef = new KKSEventFilter (iW);
    tv->viewport()->installEventFilter (ef);

    //connect(this, SIGNAL(includeSelected(int, QString)), objEditorWidget, SLOT(slotIncludeSelected(int, QString)));
    editor->addIncludesRecWidget (iW);
    tabObj->addTab (iW/*includesW*/, tr("Assotiated Records"));
    //gIncludesLay->addWidget (iW, 0, 0, 1, 1);
}

void KKSObjEditorFactory :: putSyncWidget (KKSObjEditor * editor, KKSObjectExemplar * wObjE, qint64 idObjE, QTabWidget * tabObj)
{
    int idUser = loader->getUserId();
    
    bool enabled = true;
    if(idObjE > -1){//т.е. если ИО открывается не на создание, а на редактирование
        if(loader->getPrivilege(idUser, idObjE, 5, true) == false){
            enabled = false;
        }
    }

    KKSCategory * c = wObjE->io()->category();
    if (!c)
        return;
    KKSCategory * ct = c->tableCategory ();
    if (!ct)
        return;

    KKSSyncWidget * syncW = new KKSSyncWidget (editor, enabled);
    if (idObjE > 0)
    {
        KKSAttrValue * avGlob = wObjE->attrValue (ATTR_IS_GLOBAL);
        bool isG = avGlob->value().valueVariant().toBool ();
        syncW->setCheckGlobal (isG);
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
    }
    else
    {
        /*KKSCategory * c = wObjE->io()->category();
        if (!c)
            return;
        KKSCategory * ct = c->tableCategory ();
        if (!ct)
            return;
            */
        KKSCategoryAttr * globAttr = ct->attribute (ATTR_IS_GLOBAL);//is_global

        KKSValue val (QString("true"), KKSAttrType::atBool);
        KKSAttrValue * av = new KKSAttrValue (val, globAttr);
        if (!av)
            return;
        wObjE->addAttrValue (av);
        editor->setSysAttrValue(av);
        av->release ();
    }
    QMap<int, QStringList> syncOrgs = loader->getSyncOrg (idObjE);
    KKSSortFilterProxyModel * sortMod = syncW->getSortModel ();
    QList<int> attrsIds;
    attrsIds << (ATTR_ID) << (ATTR_SHORT_NAME) << (ATTR_NAME);
    for (int i=0; i<3; i++)
    {
        KKSAttribute * attr = loader->loadAttribute (attrsIds[i]);
        KKSCategoryAttr * cAttr = KKSCategoryAttr::create (attr, true, true);
        KKSAttrView * av = new KKSAttrView (*cAttr);
        sortMod->addAttrView (av);
        av->release ();
        cAttr->release ();
        attr->release ();
    }
    QAbstractItemModel * syncOrgModel = new QStandardItemModel (syncOrgs.count(), 3);
    syncOrgModel->setHeaderData (0, Qt::Horizontal, tr ("ID"), Qt::DisplayRole);
    syncOrgModel->setHeaderData (1, Qt::Horizontal, tr ("Short name"), Qt::DisplayRole);
    syncOrgModel->setHeaderData (2, Qt::Horizontal, tr ("Name"), Qt::DisplayRole);
    int i=0;
    for (QMap<int, QStringList>::const_iterator po = syncOrgs.constBegin(); \
         po != syncOrgs.constEnd(); \
         ++po)
    {
        QModelIndex wIndex = syncOrgModel->index (i, 0);
        syncOrgModel->setData (wIndex, po.key(), Qt::UserRole);
        syncOrgModel->setData (wIndex, po.key(), Qt::DisplayRole);
        wIndex = syncOrgModel->index (i, 1);
        syncOrgModel->setData (wIndex, po.value().at (0), Qt::DisplayRole);

        wIndex = syncOrgModel->index (i, 2);
        syncOrgModel->setData (wIndex, po.value().at (1), Qt::DisplayRole);
        ++i;
    }
    syncW->setSyncOrgModel (syncOrgModel);
    
    //KKSAttribute * syncAttr = loader->loadAttribute (ATTR_IO_OBJECTS_ORGANIZATION);
    KKSCategoryAttr * syncAttr = ct->attribute (ATTR_IO_OBJECTS_ORGANIZATION); 
    KKSAttrValue * syncAttrVal (0);//= new KKSAttrValue (sVal, syncAttr);
    if (idObjE < 0)
    {
        KKSValue val (QString ("{}"), KKSAttrType::atCheckListEx);
        syncAttrVal = new KKSAttrValue (val, syncAttr);
        wObjE->addAttrValue (syncAttrVal);
        editor->setSysAttrValue(syncAttrVal);
    }
    else
    {
        KKSAttrValue * av = wObjE->attrValue (ATTR_IO_OBJECTS_ORGANIZATION);
        syncAttrVal = av;
        if (av)
            syncAttrVal->addRef ();
        editor->setSysAttrValue(syncAttrVal);
    }
    syncW->setSyncAttrVal (syncAttrVal);
    if (syncAttrVal)
        syncAttrVal->release ();
    connect (syncW, SIGNAL (setIsGlobal (bool)), editor, SLOT (setIOGlobal (bool)) );
    connect (syncW, SIGNAL (setSyncType (QLineEdit *)), editor, SLOT (setSyncType (QLineEdit *)) );
    connect (syncW, SIGNAL (addSyncOrganization (KKSAttrValue *, QAbstractItemModel *)), editor, SLOT (addSyncOrg (KKSAttrValue *, QAbstractItemModel *)) );
    connect (syncW, SIGNAL (delSyncOrganization (KKSAttrValue *, const QModelIndex&, QAbstractItemModel *)), editor, SLOT (delSyncOrg (KKSAttrValue *, const QModelIndex&, QAbstractItemModel *)) );
    if (idObjE < 0)
    {
        syncW->setCheckGlobal(true);
        KKSValue valSyncType (QString::number(1), KKSAttrType::atList);
        KKSCategoryAttr * syncTypeAttr = ct->attribute (ATTR_ID_SYNC_TYPE);
        KKSAttrValue * syncTypeAttrVal = new KKSAttrValue (valSyncType, syncTypeAttr);
        QMap<int, QString> refColumnValues;
        QMap<int, QString> syncVals = loader->loadAttributeValues (syncTypeAttr, refColumnValues);
        syncW->setSyncAttrVal(syncTypeAttrVal);
        wObjE->addAttrValue (syncTypeAttrVal);
        syncW->setSyncType (syncVals.value(1));
    }
    tabObj->addTab (syncW, tr ("Syncronization"));
}

void KKSObjEditorFactory :: setIndicesFactory (KKSIndFactory * _indf)
{
    m_indf = _indf;
}

void KKSObjEditorFactory :: addAttrSearchTemplate (void)
{
    QAction * act = qobject_cast<QAction *>(this->sender());
    KKSRecWidget * rw = qobject_cast<KKSRecWidget *> (act->parentWidget());
    if (!rw)
        return;
    QAbstractItemModel * sModel = rw->getSourceModel ();
    QModelIndex parent = QModelIndex ();
    addNewSearchTempl (parent, sModel, NULL, "io_objects");
}

void KKSObjEditorFactory :: editAttrSearchTemplate (void)
{
    QAction * act = qobject_cast<QAction *>(this->sender());
    KKSRecWidget * rw = qobject_cast<KKSRecWidget *> (act->parentWidget());
    if (!rw)
        return;
    QAbstractItemModel * sModel = rw->getSourceModel ();
    QModelIndex wIndex = rw->getSourceIndex();
    wIndex = wIndex.sibling (wIndex.row(), 0);
    updateSearchTempl (wIndex, sModel);
}

void KKSObjEditorFactory :: delAttrSearchTemplate (void)
{
    QAction * act = qobject_cast<QAction *>(this->sender());
    KKSRecWidget * rw = qobject_cast<KKSRecWidget *> (act->parentWidget());
    if (!rw)
        return;
    QAbstractItemModel * sModel = rw->getSourceModel ();
    QModelIndex wIndex = rw->getSourceIndex();
    wIndex = wIndex.sibling (wIndex.row(), 0);
    deleleSearchTempl (wIndex, sModel);
}

void KKSObjEditorFactory :: genUUID (int idAttr, const KKSAttrValue * av)
{
    if (idAttr < 0 || !av)
        return;
    KKSDatabase * db = loader->getDb();
    QString sql = QString("select generateUUID()::varchar;");
    KKSResult * res = db->execute (sql);
    if (!res || res->getRowCount () != 1)
    {
        if (res)
            delete res;
        return;
    }
    QString uuid = res->getCellAsString (0, 0);
    delete res;
    emit setuuid (uuid);
}

void KKSObjEditorFactory :: sendEditor (KKSObjEditor * editor)
{
    if (!editor)
        return;
    emit editorCreated (editor);
}

void KKSObjEditorFactory :: loadLifeCycleState (KKSLifeCycleEx * lc, QLineEdit * le, int lcSt, const QVector<qint64>& st)
{
    if (!lc)
        return;

    KKSObject * refObj = loader->loadIO (IO_STATE_ID);
    if (!refObj)
        return;

    const KKSCategory * wCat = refObj->category();
    if (!wCat)
    {
        refObj->release();
        return;
    }
    wCat = wCat->tableCategory();
    KKSList<const KKSFilterGroup *> filters;
    QString values = QString();
    int nrst = st.size();
    for (int i=0; i<nrst; i++)
    {
        values += QString("%1%2").arg(st[i]).arg(i<nrst-1 ? QString(",") : QString());
    }
    const KKSFilter * f = wCat->createFilter(1, QString("select id from %1 where id in(%2) and not is_system").arg(refObj->tableName()).arg(values), KKSFilter::foInSQL);
    KKSList<const KKSFilter *> stFilters;
    stFilters.append(f);
    f->release();
    KKSFilterGroup * fg = new KKSFilterGroup (true);
    fg->setFilters(stFilters);
    filters.append(fg);
    fg->release();
    KKSRecDialog * wEditor = this->createObjRecEditor (IO_IO_ID,
                                                       IO_STATE_ID,
                                                       filters,
                                                       tr("Set state"),
                                                       wCat,
                                                       true,
                                                       false,
                                                       Qt::ApplicationModal
                                                       );
    
    if (!wEditor)
    {
        refObj->release();
        return;
    }
    wEditor->disconnect(SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)), this, SLOT (createNewEditor(QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)));//disconnect (lcEditor);
    connect (wEditor, SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)), this, SLOT (addNewState(QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)) );
    wEditor->disconnect(SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& )), this, SLOT (editExistOE (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex&)));
    connect (wEditor, SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& )), this, SLOT (editState (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex&)) );
    //wEditor->disconnect(SIGNAL (delObjectEx (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)), objf, SLOT (deleteOE (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)));
    if (wEditor->exec() == QDialog::Accepted)
    {
        int idState = wEditor->getID ();
        KKSState * st = loader->loadState (idState);
        switch (lcSt)
        {
            case kkslifecycleform::lcStart: lc->setStartState (st); break;
            case kkslifecycleform::lcAttrChanged: lc->setAutoStateAttr(st); break;
            case kkslifecycleform::lcIndChanged: lc->setAutoStateInd(st); break;
            default:break;
        }
 //       
        if (le && st)
            le->setText (st->name());
        if (st)
            st->release();
    }
    delete wEditor;
    refObj->release();
}

void KKSObjEditorFactory :: addNewState (QWidget* editor, int idObject, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * stateModel)
{
    Q_UNUSED (tableName);
    Q_UNUSED (nTab);
    Q_UNUSED (isModal);
    Q_UNUSED (c);
    if (idObject != IO_STATE_ID)
        return;
    KKSState * st = new KKSState ();
    if (!st)
        return;
    kksstateform * stForm = new kksstateform (st);
    if (stForm && stForm->exec() == QDialog::Accepted)
    {
        KKSState * stn = stForm->getState();
        int ier = ppf->insertState (stn);
        if (ier < 0)
        {
            qCritical() << tr("Cannot insert new state into Db, error !");
            QMessageBox::critical(editor, tr("Add state"), tr("Cannot insert new state into Db, error !"), QMessageBox::Ok);
            st->release();
            delete stForm;
            return;
        }
        KKSEIOData * stInfo = loader->loadEIOInfo (idObject, stn->id());
        int nr = stateModel->rowCount();
        stateModel->insertRows (nr, 1);
        QModelIndex recIndex = stateModel->index (nr, 0);
        stateModel->setData (recIndex, QVariant::fromValue<KKSEIOData *>(stInfo), Qt::UserRole+1);
        stateModel->setData (recIndex, stn->id(), Qt::UserRole);
    }
    st->release();
    delete stForm;
}

void KKSObjEditorFactory :: editState (QWidget * editor, int idObject, qint64 idObjEx, const KKSCategory * c, QString tableName, int nTab, bool isModal, QAbstractItemModel * recModel, const QModelIndex& recIndex)
{
    Q_UNUSED (tableName);
    Q_UNUSED (nTab);
    Q_UNUSED (isModal);
    Q_UNUSED (c);
    if (idObject != IO_STATE_ID)
        return;
    KKSState * st = loader->loadState (idObjEx);
    if (!st)
        return;
    kksstateform * stForm = new kksstateform (st);
    if (stForm && stForm->exec() == QDialog::Accepted)
    {
        KKSState * stn = stForm->getState();
        int ier = ppf->updateState (stn);
        if (ier < 0)
        {
            qCritical() << tr("Cannot update state in Db, error !");
            QMessageBox::critical(editor, tr("Edit state"), tr("Cannot update state in Db, error !"), QMessageBox::Ok);
            st->release();
            delete stForm;
            return;
        }
        KKSEIOData * stInfo = loader->loadEIOInfo (idObject, stn->id());
        recModel->setData (recIndex, QVariant::fromValue<KKSEIOData *>(stInfo), Qt::UserRole+1);
    }
    st->release();
    delete stForm;
}

void KKSObjEditorFactory :: addLifeCycleState (KKSLifeCycleEx * lc, QAbstractItemModel * stateMod)
{
    
    if (!lc)
        return;

    KKSObject * refObj = loader->loadIO (IO_STATE_ID);
    if (!refObj)
        return;

    const KKSCategory * wCat = refObj->category();
    if (!wCat)
    {
        refObj->release();
        return;
    }
    wCat = wCat->tableCategory();
    KKSList<const KKSFilterGroup *> filters;
    const KKSFilter * f = wCat->createFilter(1, QString("select id from %1 where not is_system").arg(refObj->tableName()), KKSFilter::foInSQL);
    KKSList<const KKSFilter *> stFilters;
    stFilters.append(f);
    f->release();
    KKSFilterGroup * fg = new KKSFilterGroup (true);
    fg->setFilters(stFilters);
    filters.append(fg);
    fg->release();
    KKSRecDialog * wEditor = this->createObjRecEditor (IO_IO_ID,
                                                       IO_STATE_ID,
                                                       filters,
                                                       tr("Set state"),
                                                       wCat,
                                                       false,
                                                       true,
                                                       Qt::ApplicationModal
                                                       );
    
    if (!wEditor)
    {
        refObj->release();
        return;
    }
    wEditor->disconnect(SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)), this, SLOT (createNewEditor(QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)));//disconnect (lcEditor);
    connect (wEditor, SIGNAL (newObjectEx (QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)), this, SLOT (addNewState(QWidget*, int, const KKSCategory *, QString, int, bool, QAbstractItemModel *)) );
    wEditor->disconnect(SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& )), this, SLOT (editExistOE (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex&)));
    connect (wEditor, SIGNAL (editObjectEx (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex& )), this, SLOT (editState (QWidget*, int, qint64, const KKSCategory *, QString, int, bool, QAbstractItemModel *, const QModelIndex&)) );
    //wEditor->disconnect(SIGNAL (delObjectEx (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)), objf, SLOT (deleteOE (QWidget*, int, qint64, QString, QAbstractItemModel *, const QModelIndex&)));
    if (wEditor->exec() == QDialog::Accepted)
    {
        int idState = wEditor->getID ();
        KKSState * st = loader->loadState (idState);
        if (!st)
        {
            qCritical() << tr("Cannot add state into life cycle %1, Db error").arg(lc->name());
            QMessageBox::critical(wEditor, tr("Add state into life cycle"), tr("Cannot add state into life cycle %1, Db error").arg(lc->name()), QMessageBox::Ok);
            delete wEditor;
            refObj->release ();
            return;
        }
        KKSMap<int, KKSState *> stList = lc->states();
        if (stList.constFind (idState) != stList.constEnd())
        {
            delete wEditor;
            refObj->release ();
            st->release();
            return;
        }
        lc->addState (st);
        int nr = stateMod->rowCount();
        stateMod->insertRows (nr, 1);
        QModelIndex recIndex = stateMod->index (nr, 0);
        stateMod->setData (recIndex, idState, Qt::UserRole);
        stateMod->setData (recIndex, st->name(), Qt::DisplayRole);
        st->release();
    }
    delete wEditor;
    refObj->release();
}
