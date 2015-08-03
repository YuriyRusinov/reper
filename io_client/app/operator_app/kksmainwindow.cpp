#include "kksmainwindow.h"

#include "kksmasswidget.h"
#include "cmdjournalform.h"
#include "msgjournalform.h"
#include "kksstatusbar.h"
#include "kksabout.h"
#include "kksapplication.h"
#include "kksclient_name.h"
#include "kkssettings.h"
#include "defines.h"
#include "kksdatabase.h"

#include "KKSLoader.h"
#include "KKSJMonitor.h"

#include "KKSRubric.h"
#include <KKSTemplate.h>
#include <KKSCategory.h>
#include <KKSType.h>
#include <KKSObject.h>
#include <KKSObjectExemplar.h>
#include <KKSObjEditor.h>
#include <KKSCatEditor.h>
#include <KKSCategoryTemplateWidget.h>
#include <KKSTemplateEditor.h>
#include <KKSAttributesEditor.h>
#include <KKSObjEditorFactory.h>
#include <KKSCatEditorFactory.h>
#include <KKSTemplateEditorFactory.h>
#include <KKSAttributesFactory.h>
#include <KKSStuffFactory.h>
#include <KKSRubricFactory.h>

#include <kksincludeswidget.h>
#include <kkscmdjournalitem.h>
#include <kksmsgjournalitem.h>
#include <KKSJournalWidget.h>
#include <KKSDbgOutputWidget.h>
#include <kksstuffform.h>
#include <IndicatorForm.h>
#include <kkslifecycleform.h>
#include <kkssearchtemplateform.h>

#include "cmdjournalsettingsform.h"
#include "msgjournalsettingsform.h"
#include "kkscommandlineopts.h"

#include "ui_kksmainwindowdlg.h"

#include <QToolBar>
#include <QComboBox>
#include <QFile>
#include <QMessageBox>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFlags>

// openreport
//wrtembed
#include "reporthandler.h"       // The Report Window Child
#include "documentwindow.h"
// common
#include <dbtools.h>
#include <xsqlquery.h>
#include <data.h>
// images
//#include "../images/OpenReportsIcon.xpm" 
//#include "builtinSqlFunctions.h"


KKSMainWindow::KKSMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_masscreateW (0)
{
//    Q_INIT_RESOURCE(icon_set);

    ui = new Ui::KKSMainWindowDlg;
    ui->setupUi(this);

    m_mdiArea = new QMdiArea(this);
    #if QT_VERSION >= 0x040400
        m_mdiArea->setActivationOrder(QMdiArea::StackingOrder);
    #endif

    setCentralWidget(m_mdiArea);

    connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateMenus()));
    m_windowMapper = new QSignalMapper(this);
    connect(m_windowMapper, SIGNAL(mapped(QWidget *)),
            this, SLOT(setActiveSubWindow(QWidget *)));

    m_jmonitor = 0;
    //m_masscreateW = 0;

    KKSObjEditorFactory * oef = kksApp->oef();
    KKSCatEditorFactory * catf = kksApp->catf ();
    KKSTemplateEditorFactory * tf = kksApp->tf ();
    KKSRubricFactory * rf = kksApp->rf ();
    KKSAttributesFactory * af = kksApp->attrf();

    connect (oef, 
             SIGNAL(showReportEditor(qint64)), 
             this, 
             SLOT(slotCreateNewReportEditor(qint64)));
    
    connect (oef, 
             SIGNAL(showReportViewer(qint64)), 
             this, 
             SLOT(slotCreateNewReportViewer(qint64)));

    connect (rf, 
             SIGNAL(showReportEditor(qint64)), 
             this, 
             SLOT(slotCreateNewReportEditor(qint64)));
    
    connect (rf, 
             SIGNAL(showReportViewer(qint64)), 
             this, 
             SLOT(slotCreateNewReportViewer(qint64)));


    connect (oef, 
             SIGNAL(editorCreated(KKSObjEditor *)), 
             this, 
             SLOT(slotCreateNewObjEditor(KKSObjEditor*)));
    
    connect (oef,
             SIGNAL (editorSearchTemplate (QWidget *)),
             this,
             SLOT (slotSearchTemplateEdit(QWidget *)));

    connect (catf, 
             SIGNAL(categoryEditorCreated (KKSCatEditor *)), 
             this, 
             SLOT (slotCreateNewCatEditor (KKSCatEditor *)));

    connect (tf, 
             SIGNAL(templateEditorCreated (KKSTemplateEditor *)), 
             this, 
             SLOT (slotCreateNewTemplateEditor (KKSTemplateEditor *)));

    connect (rf, 
             SIGNAL (rubricEditorCreated (KKSIncludesWidget *)), 
             this, 
             SLOT (slotCreateNewRubricEditor (KKSIncludesWidget *)) );

    connect (rf, 
             SIGNAL (objEditorCreated (KKSObjEditor *)), 
             this, 
             SLOT (slotCreateNewObjEditor(KKSObjEditor*)));

    connect (af, 
             SIGNAL(mapCreated(QDockWidget *, QDockWidget *)), 
             this, 
             SLOT(addMapDockWidgets(QDockWidget *, QDockWidget *)));

    connect (af, 
             SIGNAL(mapAboutToDestroy(QDockWidget *, QDockWidget *)), 
             this, 
             SLOT(destroyMapDockWidgets(QDockWidget *, QDockWidget *)));

    init();
    
    showMaximized();
    
    if(kksApp->db()->connected()){
        connectToDb();//там только кнопки и док-виджеты все активируются в этом случае
        
        if(kksApp->commandLineOpts()->idObject > 0){ //открываем указанный ИО
            KKSList<const KKSFilterGroup *> filterGroups;
            slotCreateNewObjEditor(IO_IO_ID, kksApp->commandLineOpts()->idObject, filterGroups, QString());
        }
    }
}

KKSMainWindow::~KKSMainWindow()
{
    delete ui;

    if(m_jmonitor)
        delete m_jmonitor;
}

void KKSMainWindow::init()
{
    initIcons();
    initActions();
    initToolBars();
    initStatusBar();
    initReports();
    
    initConnections();
    setActionsEnabled(false);
    updateMenus();
}

void KKSMainWindow::closeEvent(QCloseEvent *event)
{
    m_mdiArea->closeAllSubWindows();
    if (activeKKSSubWindow()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void KKSMainWindow::initReports()
{
    m_reportHandler = new ReportHandler(this, "report handler");
    QAction * sepid = m_reportHandler->populateMenuBar(ui->menuReports, NULL);
    // setup the toolbar
    
    m_reportHandler->docToolBars(this);
    this->insertToolBar(tbOrg, m_reportHandler->getTbFiles());
    this->insertToolBar(tbOrg, m_reportHandler->getTbDatabase());
    this->insertToolBar(tbOrg, m_reportHandler->getTbEdit());
    this->insertToolBar(tbOrg, m_reportHandler->getTbOptions());
    this->insertToolBar(tbOrg, m_reportHandler->getTbFont());
    
    
    m_reportHandler->setParentWindow(m_mdiArea);

    /*connect(m_reportHandler, SIGNAL(dbOpenClosed()), this, SLOT(setCaption()));
    connect(m_reportHandler, SIGNAL(messageChanged(const QString &)),
            statusBar(), SLOT(message(const QString &)));
    connect(m_reportHandler, SIGNAL(messageCleared()),
            statusBar(), SLOT(clear()));
    */

    m_reportHandler->onSubWinChanged(NULL);
    connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(slotSubWindowActivated(QMdiSubWindow*)));
    connect(this, SIGNAL(reportSubWindowActivated(QMdiSubWindow*)), m_reportHandler, SLOT(onSubWinChanged(QMdiSubWindow*)));
    

}

void KKSMainWindow::slotCreateNewReportViewer(qint64 idReport)
{
    m_reportHandler->print(idReport, true);
}

void KKSMainWindow::slotCreateNewReportEditor(qint64 idReport)
{
    m_reportHandler->dbLoadDoc(idReport);
}


void KKSMainWindow::slotSubWindowActivated(QMdiSubWindow * w)
{
    if(!w){
        emit reportSubWindowActivated(w);
        return;
    }

    DocumentWindow * dw = qobject_cast<DocumentWindow*>(w->widget());
    if(dw)
        emit reportSubWindowActivated(w);
    else
        emit reportSubWindowActivated(NULL);


}


void KKSMainWindow::updateMenus()
{
    bool hasMdiChild = (activeKKSSubWindow() != 0);
    
    aCloseWindow->setEnabled(hasMdiChild);
    aCloseAllWindows->setEnabled(hasMdiChild);
    aTile->setEnabled(hasMdiChild);
    aCascade->setEnabled(hasMdiChild);
    aNextWindow->setEnabled(hasMdiChild);
    aPreviousWindow->setEnabled(hasMdiChild);
    aSaveIO->setEnabled(hasMdiChild);
    aSeparator->setVisible(hasMdiChild);

    QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
    if(windows.isEmpty()){
        aSeparator->setVisible(false);
        return;
    }

    QList<QAction *> aList = ui->aWindowMenu->actions();
    int aCount = aList.count();
    for(int j=9; j<aCount; j++){
        aList[j]->setChecked(false);
    }
    
    for (int i = 0; i < windows.size(); i++)
    {
        
        QIcon childIcon;
        QWidget *child = qobject_cast<KKSDialog*>(windows.at(i)->widget());

		if(!child){
			child = qobject_cast<KKSRecDialog*>(windows.at(i)->widget());
            if(!child){
                child = qobject_cast<DocumentWindow*>(windows.at(i)->widget());
                if(!child)
                    continue;
                else
                    childIcon = child->windowIcon();
            }
			else
				childIcon = ((KKSRecDialog *)child)->icon();
		}
		else{
		    childIcon = ((KKSDialog *)child)->icon();
		}

        WId winId = child->winId();
        QList<QAction *> aList = ui->aWindowMenu->actions();
        int aCount = aList.count();
        bool found = false;
        int j=9;
        for(j=9; j<aCount; j++){
            if((int)winId == aList[j]->data().toInt()){
                found = true;
                break;
            }
        }
        if(found){
            aList[j]->setChecked(child == activeKKSSubWindow());
            continue;
        }

        QString title = child->windowTitle();
        QString text;
        if(title.size() > 30){
            text = title.left(27);
            text = text + QString("...");
        }
        else
            text = title;

        QAction *action  = ui->aWindowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(child == activeKKSSubWindow());
        action->setData((int)winId);
        action->setIcon(childIcon);
        action->setText(text);
        tbSubWindows->addAction(action);
        connect(action, SIGNAL(triggered()), m_windowMapper, SLOT(map()));
        connect(child, SIGNAL(aboutToClose(QWidget *)), this, SLOT(closeSubWindow(QWidget *)));
        m_windowMapper->setMapping(action, windows.at(i));
    }


}

void KKSMainWindow::closeSubWindow(QWidget * t)
{
    if(!t)
        return;

    WId winId = t->winId();

    QList<QAction *> aList = ui->aWindowMenu->actions();
    int aCount = aList.count();
    bool found = false;
    int j=9;
    for(j=9; j<aCount; j++){
        if((int)winId == aList[j]->data().toInt()){
            found = true;
            break;
        }
    }
    if(!found)
        return;

    QAction * a = aList[j];
    delete a;
}

void KKSMainWindow::updateWindowMenu(bool bFirstTime)
{
    if(bFirstTime){
        ui->aWindowMenu->clear();
        ui->aWindowMenu->addAction(aCloseWindow);
        ui->aWindowMenu->addAction(aCloseAllWindows);
        ui->aWindowMenu->addSeparator();
        ui->aWindowMenu->addAction(aTile);
        ui->aWindowMenu->addAction(aCascade);
        ui->aWindowMenu->addSeparator();
        ui->aWindowMenu->addAction(aNextWindow);
        ui->aWindowMenu->addAction(aPreviousWindow);
        ui->aWindowMenu->addAction(aSeparator);
    }
/*
    QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
    aSeparator->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        
        KKSDialog *child = qobject_cast<KKSDialog*>(windows.at(i)->widget());

        if(!child)
            continue;

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->windowTitle());
        } else {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->windowTitle());
        }

        QAction *action  = ui->aWindowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeKKSSubWindow());
        connect(action, SIGNAL(triggered()), m_windowMapper, SLOT(map()));
        m_windowMapper->setMapping(action, windows.at(i));
    }
*/
}

void KKSMainWindow::setActionsEnabled(bool enabled)
{
    ui->aConnect->setEnabled(!enabled);
    
    ui->aDisconnect->setEnabled(enabled);
    ui->aChangeUser->setEnabled(enabled);
    ui->aConnInfo->setEnabled(enabled);

    ui->aCreateDoc->setEnabled(enabled);
    ui->aFindDoc->setEnabled(enabled);
    ui->aMyDocs->setEnabled (enabled);
    ui->aRubricControl->setEnabled (enabled);
    ui->aPrintDoc->setEnabled(enabled);

    ui->aCreateSearchQuery->setEnabled(enabled);
    ui->aShowSearchQueries->setEnabled(enabled);


    ui->aCreateMsg->setEnabled(enabled);
    ui->aCreateCmd->setEnabled(enabled);
    ui->aCreateTSDRecord->setEnabled(enabled);
    ui->aShowTSD->setEnabled(enabled);

    ui->aCreateCat->setEnabled(false);
    ui->aViewCat->setEnabled(false);
    ui->aViewAttrs->setEnabled(false);
    ui->aEditOSS->setEnabled(false);
    ui->aSysQualifiers->setEnabled(false);
    aGenerateMess->setEnabled(false);
    ui->actLCCreate->setEnabled(false);
    ui->actLCOpen->setEnabled(false);

    if(enabled){
        QString uName = kksApp->loader()->getDb()->getUser();
        if(uName == "admin"){
            ui->aCreateCat->setEnabled(enabled);
            ui->aViewCat->setEnabled(enabled);
            ui->aViewAttrs->setEnabled(enabled);
            ui->aEditOSS->setEnabled(enabled);
            ui->aSysQualifiers->setEnabled(enabled);
            this->aGenerateMess->setEnabled(enabled);
            ui->actLCCreate->setEnabled(enabled);
            ui->actLCOpen->setEnabled(enabled);
        }
    }
       
}

void KKSMainWindow::initActions()
{
    aSaveIO = new QAction(tr("Save"), this);
    aSaveIO->setIcon(QIcon(QPixmap(":/ddoc/accept.png")));
    aSaveIO->setShortcut(tr("F2"));
    aSaveIO->setStatusTip(tr("Save the active window"));
    connect(aSaveIO, SIGNAL(triggered()),
            this, SLOT(saveActiveSubWindow()));

    aCloseWindow = new QAction(tr("Cl&ose"), this);
    aCloseWindow->setShortcut(tr("Ctrl+F4"));
    aCloseWindow->setStatusTip(tr("Close the active window"));
    connect(aCloseWindow, SIGNAL(triggered()),
            m_mdiArea, SLOT(closeActiveSubWindow()));

    aCloseAllWindows = new QAction(tr("Close &All"), this);
    aCloseAllWindows->setStatusTip(tr("Close all the windows"));
    connect(aCloseAllWindows, SIGNAL(triggered()),
            m_mdiArea, SLOT(closeAllSubWindows()));

    aTile = new QAction(tr("&Tile"), this);
    aTile->setStatusTip(tr("Tile the windows"));
    connect(aTile, SIGNAL(triggered()), m_mdiArea, SLOT(tileSubWindows()));

    aCascade = new QAction(tr("&Cascade"), this);
    aCascade->setStatusTip(tr("Cascade the windows"));
    connect(aCascade, SIGNAL(triggered()), m_mdiArea, SLOT(cascadeSubWindows()));

    aNextWindow = new QAction(tr("Ne&xt"), this);
    aNextWindow->setStatusTip(tr("Move the focus to the next window"));
    connect(aNextWindow, SIGNAL(triggered()),
            m_mdiArea, SLOT(activateNextSubWindow()));

    aPreviousWindow = new QAction(tr("Pre&vious"), this);
    aPreviousWindow->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(aPreviousWindow, SIGNAL(triggered()),
            m_mdiArea, SLOT(activatePreviousSubWindow()));

    aSeparator = new QAction(this);
    aSeparator->setSeparator(true);
    
    aGenerateMess = new QAction (QIcon(":/ddoc/message_stream.png"), tr("Message streams"), this);
    aGenerateMess->setToolTip (tr("Generate message streams"));
    connect (aGenerateMess, SIGNAL(triggered()),
             this, SLOT (slotMess()));

    updateWindowMenu(true);

    connect(ui->aWindowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
}

void KKSMainWindow::initJournal()
{
    if(!m_jmonitor)
        return;

    m_journalW = m_jmonitor->journal();
    if (!m_journalW)
        return;
    m_journalW->setParent(this);
    addDockWidget(Qt::LeftDockWidgetArea, m_journalW);
}


void KKSMainWindow::initDebugWindow()
{
    m_debugWidget = kksApp->dbgWidget(true, true);

    if (!m_debugWidget)
        return;

    m_debugWidget->setParent(this);
    bool enabled = m_debugWidget->isVisible();
    addDockWidget(Qt::RightDockWidgetArea, m_debugWidget);
    if(!enabled)
        m_debugWidget->hide();
}

void KKSMainWindow::initToolBars()
{
    tbActions = new QToolBar(tr("Docs and Journals toolbar"), this);
    //tbActions->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbActions->setIconSize(QSize(32, 32));
    tbActions->addAction(ui->aCreateDoc);
    tbActions->addAction(ui->aFindDoc);
    tbActions->addAction(aSaveIO);
    m_masscreateW = new MassWidget ();
    tbActions->addWidget (m_masscreateW);
    tbActions->addAction(ui->aPrintDoc);
    
    tbActions->addSeparator();
    
    tbActions->addAction(ui->aMyDocs);
    tbActions->addAction(ui->aRubricControl);
    
    
    tbPost = new QToolBar(tr("Post toolbar"), this);
    //tbPost->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbPost->setIconSize(QSize(32, 32));
    tbPost->addAction(ui->aCreateMsg);
    tbPost->addAction(ui->aCreateCmd);
    tbPost->addSeparator();
    tbPost->addAction(ui->aCreateTSDRecord);
    tbPost->addAction(ui->aShowTSD);
    tbPost->addAction(aGenerateMess);

    tbQuery = new QToolBar(tr("Search query toolbar"), this);
    //tbQuery->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbQuery->setIconSize(QSize(32, 32));
    tbQuery->addAction(ui->aCreateSearchQuery);
    tbQuery->addAction(ui->aShowSearchQueries);

    tbAdmin = new QToolBar(tr("Admin toolbar"), this);
    //tbAdmin->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbAdmin->setIconSize(QSize(32, 32));
    tbAdmin->addAction(ui->aCreateCat);
    tbAdmin->addAction(ui->aViewCat);
    tbAdmin->addAction(ui->aViewAttrs);
    tbAdmin->addAction(ui->aEditOSS);
    tbAdmin->addAction(ui->aSysQualifiers);

    tbSubWindows = new QToolBar(tr("Windows"), this);
    tbSubWindows->setIconSize(QSize(32, 32));
    tbSubWindows->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //tbSubWindows->setVisible(false);

    tbOrg = new QToolBar (tr("Organization toolbar"), this);
    tbOrg->setIconSize (QSize (32, 32));
    QWidget * orgW = new QWidget (tbOrg);
    QHBoxLayout * hLay = new QHBoxLayout (orgW);
    QSpacerItem * spacer = new QSpacerItem(1, 32, QSizePolicy::Expanding);
    lModeName = new QLabel (m_modeName, orgW);
    lOrgLogo = new QLabel (m_orgSymbol, orgW);
    hLay->addItem(spacer);
    hLay->addWidget (lOrgLogo);
    hLay->addWidget (lModeName);
    tbOrg->addWidget (orgW);
    

    this->addToolBar(tbActions);
    this->addToolBar(tbPost);
    this->addToolBar(tbQuery);
    this->addToolBar(tbAdmin);
    
    //int tbArea = (Qt::RightToolBarArea | Qt::TopToolBarArea);
    this->addToolBar (/*(Qt::ToolBarArea)tbArea,*/ tbOrg);

    this->addToolBarBreak();

    this->addToolBar(tbSubWindows);

    QMenu * menuToolBars = new QMenu(tr("Toolbars"), this);
    ui->menuView->addMenu(menuToolBars);
    menuToolBars->addAction(tbActions->toggleViewAction());
    menuToolBars->addAction(tbPost->toggleViewAction());
    menuToolBars->addAction (tbQuery->toggleViewAction());
    menuToolBars->addAction (tbAdmin->toggleViewAction());
    menuToolBars->addAction (tbSubWindows->toggleViewAction());
}

void KKSMainWindow::initIcons()
{

    ui->aConnect->setIcon(QIcon(":/ddoc/connect.png"));
    ui->aChangeUser->setIcon(QIcon(":/ddoc/change_user.png"));
    ui->aDisconnect->setIcon(QIcon(":/ddoc/disconnect.png"));
    ui->aConnInfo->setIcon(QIcon(":/ddoc/connection_info.png"));
    //ui->aQuit->setIcon();

    //ui->aShowJournals->setIcon(QIcon(":/ddoc/show_journals.png"));
    //ui->aShowJournals->setIcon(QIcon(":/ddoc/show_dbg_win.png"));

    ui->aCreateDoc->setIcon(QIcon(":/ddoc/new_doc.png"));
    ui->aFindDoc->setIcon(QIcon(":/ddoc/find_doc.png"));
    ui->aMyDocs->setIcon(QIcon(":/ddoc/my_docs.png"));
    ui->aRubricControl->setIcon(QIcon(":/ddoc/rubricators.png"));
    ui->aPrintDoc->setIcon(QIcon(":/ddoc/print_doc.png"));

    ui->aCreateSearchQuery->setIcon(QIcon(":/ddoc/search.png"));
    ui->aShowSearchQueries->setIcon(QIcon(":/ddoc/show_search_queries.png"));

    ui->aCreateMsg->setIcon(QIcon(":/ddoc/create_msg.png"));
    ui->aCreateCmd->setIcon(QIcon(":/ddoc/create_cmd.png"));
    ui->aCreateTSDRecord->setIcon(QIcon(":/ddoc/create_tsd_record.png"));
    ui->aShowTSD->setIcon(QIcon(":/ddoc/show_tsd.png"));

    ui->aCreateCat->setIcon(QIcon(":/ddoc/category_add.png"));
    ui->aViewCat->setIcon(QIcon(":/ddoc/show_cat.png"));
    ui->aViewAttrs->setIcon(QIcon(":/ddoc/show_attrs.png"));
    ui->aEditOSS->setIcon(QIcon(":/ddoc/edit_oss.png"));
    ui->aSysQualifiers->setIcon(QIcon(":/ddoc/sys_qualifiers.png"));

    ui->aSettings->setIcon(QIcon(":/ddoc/settings.png"));
    ui->aAbout->setIcon(QIcon(":/ddoc/about.png"));
    ui->aCmdParamsHelp->setIcon(QIcon(":/ddoc/about.png"));

    
    /*
    KKSCommand::setPxIn(QPixmap(":/ico/myInput.xpm"));
    KKSCommand::setPxInAll(QPixmap(":/ico/allInput.xpm"));
    KKSCommand::setPxOut(QPixmap(":/ico/myOutput.xpm"));
    KKSCommand::setPxOutAll(QPixmap(":/ico/allOutput.xpm"));
    KKSCommand::setPxArchived(QPixmap(":/ico/archived.xpm"));
    */

    QIcon rubricIcon (":/ddoc/rubric.png");
    QIcon rubricItemIcon (":/ddoc/rubric_item.png");
    KKSRubric::setGeneralIcon(rubricIcon.pixmap (QSize(24,24)));
    KKSRubricItem::setGeneralIcon(rubricItemIcon.pixmap (QSize(24,24)));

    /*
    KKSMessage::setPxIn(QPixmap(":/ico/myInput.xpm"));
    KKSMessage::setPxOut(QPixmap(":/ico/myOutput.xpm"));
    */
    
}

void KKSMainWindow::initConnections()
{
    connect(ui->aConnect, SIGNAL(triggered()), this, SLOT(slotConnect()));
    connect(ui->aDisconnect, SIGNAL(triggered()), this, SLOT(slotDisconnect()));
    connect(ui->aChangeUser, SIGNAL(triggered()), this, SLOT(slotChangeUser()));
    connect(ui->aConnInfo, SIGNAL(triggered()), this, SLOT(slotConnInfo()));
    connect(ui->aQuit, SIGNAL(triggered()), this, SLOT(slotClose()));

    //connect(ui->aShowJournals, SIGNAL(triggered(bool)), this, SLOT(slotShowJournls(bool)));
    //connect(ui->aShowDbgWindow, SIGNAL(triggered(bool)), this, SLOT(slotShowDbgWin(bool)));

    connect(ui->aCreateDoc, SIGNAL(triggered()), this, SLOT(slotCreateDoc()));
    connect(ui->aFindDoc, SIGNAL(triggered()), this, SLOT(slotFindDoc()));
    connect (ui->aMyDocs, SIGNAL (triggered()), this, SLOT (slotMyDocs()) );
    connect (ui->aRubricControl, SIGNAL (triggered()), this, SLOT (slotRubricControl()) );
    connect(ui->aPrintDoc, SIGNAL(triggered()), this, SLOT(slotPrintDoc()));
    
    connect (ui->aCreateSearchQuery, SIGNAL (triggered()), this, SLOT (slotCreateSearchQuery()) );
    connect (ui->aShowSearchQueries, SIGNAL (triggered()), this, SLOT (slotShowSearchQueries()) );

    connect (ui->aCreateMsg, SIGNAL (triggered()), this, SLOT (slotCreateMsg()) );
    connect (ui->aCreateCmd, SIGNAL (triggered()), this, SLOT (slotCreateCmd()) );
    connect (ui->aCreateTSDRecord, SIGNAL (triggered()), this, SLOT (slotCreateTSDRecord()) );    
    connect(ui->aShowTSD, SIGNAL(triggered()), this, SLOT(slotShowTSD()));

    connect(ui->aCreateCat, SIGNAL(triggered()), this, SLOT(slotCreateCat()));
    connect(ui->aViewCat, SIGNAL(triggered()), this, SLOT(slotViewCat()));
    connect(ui->aViewAttrs, SIGNAL(triggered()), this, SLOT(slotViewAttrs()));
    connect(ui->aEditOSS, SIGNAL(triggered()), this, SLOT(slotEditOSS()));
    connect(ui->aSysQualifiers, SIGNAL(triggered()), this, SLOT(slotSysQualifiers()));
    connect(ui->actLCCreate, SIGNAL (triggered()), this, SLOT(slotCreateLC()) );
    connect(ui->actLCOpen, SIGNAL (triggered()), this, SLOT (slotOpenLC()) );

    connect(ui->aSettings, SIGNAL(triggered()), this, SLOT(slotSettings()));

    connect(ui->aAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
    connect(ui->aCmdParamsHelp, SIGNAL(triggered()), this, SLOT(slotCmdParamsHelp()));

}

void KKSMainWindow::initStatusBar()
{
    KKSStatusBar * statusBar = new KKSStatusBar(m_userName, m_dlName, this);

    QPixmap px;
    statusBar->setHellow(px, m_userName, m_dlName);
    
    statusBar->showStatusBar();

    setStatusBar(statusBar);
    this->statusBar()->hide();
}

bool KKSMainWindow::connectToDb()
{
    if(!kksApp->db()->connected()){
        int res = kksApp->GUIConnect(this);
        if(res == ERROR_CODE){
            setActionsEnabled(false);
        }
        else{
            setActionsEnabled(true);
            m_reportHandler->dbConnect(kksApp->db()->getName(), 
                                       kksApp->db()->getHost(), 
                                       kksApp->db()->getPort(), 
                                       kksApp->db()->getUser(), 
                                       kksApp->db()->getPass());
        }
    }
    else{
        setActionsEnabled(true);
    }

    if(kksApp->db1()->connected()){
        //создаем копии и передаем управление ими в класс KKSJMonitor. 
        //В деструкторе этого класса они будут удалены
        KKSCmdJSettings * settings = new KKSCmdJSettings(*(kksApp->getCmdJSettings()));
        KKSMsgJSettings * msgSettings = new KKSMsgJSettings(*(kksApp->getMsgJSettings()));
        m_jmonitor = new KKSJMonitor(settings, 
                                     msgSettings,
                                     kksApp->db1(), 
                                     kksApp->oef(),
                                     kksApp->eiof());
        

        connect(m_jmonitor, 
                SIGNAL(editorCreated(KKSObjEditor *)), 
                this, 
                SLOT(slotCreateNewObjEditor(KKSObjEditor*)));
        connect(m_jmonitor, SIGNAL(settingsChanged(KKSCmdJSettings *)), kksApp, SLOT(saveCmdJSettings(KKSCmdJSettings *)));
        connect(m_jmonitor, SIGNAL(msgSettingsChanged(KKSMsgJSettings *)), kksApp, SLOT(saveMsgJSettings(KKSMsgJSettings *)));
    }
    else
        return false;

    m_userName = kksApp->loader()->getUserName();
    m_dlName = kksApp->loader()->getDlName();
    m_maclabelName = kksApp->loader()->getCurrMacLabelName();
    
    m_modeName = kksApp->loader()->getOrgModeName ();
    lModeName->setText (m_modeName);
    m_orgSymbol = kksApp->loader()->getOrgLogo ();
    //QByteArray ba = m_orgSymbol.toLocal8Bit();
    QPixmap logo;
    //bool ok = logo.loadFromData(m_orgSymbol, "XPM");
    bool ok = logo.loadFromData(m_orgSymbol);
    if (ok){
        logo = logo.scaled(QSize(32, 32));
        lOrgLogo->setPixmap (logo);
    }

    m_jmonitor->initWorkModeInfo(lOrgLogo, lModeName);


    KKSStatusBar * bar = getStatusBar();
    if(!bar)
        return true;

    bar->updateHellow(m_userName, m_dlName);

    //initCmdJournal();
    //initMsgJournal();
    
    initJournal ();
    initDebugWindow();

    QAction * aJ = m_journalW->toggleViewAction();
    QAction * aD = m_debugWidget->toggleViewAction();
    aJ->setIcon(QIcon(":/ddoc/show_journals.png"));
    aD->setIcon(QIcon(":/ddoc/show_dbg_win.png"));
    ui->menuView->addSeparator();
    ui->menuView->addAction(aJ);
    ui->menuView->addAction(aD);

    if(kksApp->loader()->getLocalOrgId() == -1){
        qWarning() << tr("Local organization in current database does not set."
                                "\nMany operations such as creation of documnts and some others will not work correctly."
                                "\nPlease connect to your system administrator.");
        QMessageBox::warning(this, 
                             tr("Incorrect system configuration"), 
                             tr("Local organization in current database does not set."
                                "\nMany operations such as creation of documnts and some others will not work correctly."
                                "\nPlease connect to your system administrator."), 
                             QMessageBox::Ok, 
                             QMessageBox::NoButton);
    }

    return true;
}

bool KKSMainWindow::disconnectFromDb()
{
    int u = QMessageBox::question(this, 
                                  tr("Disconnection"), 
                                  tr("Disconnection will close all opened windows.\nProceed?"), 
                                  QMessageBox::Ok | QMessageBox::Cancel);
    if(u != QMessageBox::Ok)
        return false;

    if(m_jmonitor){
        delete m_jmonitor;
        m_jmonitor = NULL;
    }

    kksApp->db()->disconnect(false);
    kksApp->db1()->disconnect(false);
    setActionsEnabled(false);
    QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
    for (int i=0; i<windows.count(); i++)
    {
        QMdiSubWindow * w = windows[i];
        w->close();
    }
    
    m_userName = QString();
    m_dlName = QString();
	lModeName->setText(QString());
	lOrgLogo->setPixmap(QPixmap());

    KKSStatusBar * bar = getStatusBar();
    if(!bar)
        return true;

    bar->updateHellow(m_userName, m_dlName);

    return true;
}

KKSStatusBar * KKSMainWindow::getStatusBar()
{
    KKSStatusBar * bar = (KKSStatusBar *) statusBar();
    
    return bar;
}

QWidget * KKSMainWindow::activeKKSSubWindow()
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<QWidget *>(activeSubWindow->widget());
    return 0;
}

void KKSMainWindow::isActiveSubWindow(const KKSObjEditor * editor, bool * yes)
{
    if(!editor)
        return;
    if(!yes)
        return;

    QWidget * w = activeKKSSubWindow();
    if(!w)
        return;

    if(w == editor){
        *yes = true;
        return;
    }

    *yes = false;
}


KKSObjEditor * KKSMainWindow::activeObjEditor()
{
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<KKSObjEditor *>(activeSubWindow->widget());
    return 0;
}

KKSCatEditor * KKSMainWindow::activeCatEditor ()
{
    if (activeObjEditor())
        return 0;

    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<KKSCatEditor *>(activeSubWindow->widget());
    return 0;
}

KKSTemplateEditor *KKSMainWindow::activeTemplateEditor ()
{
    if (activeObjEditor() || activeCatEditor())
        return 0;

    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<KKSTemplateEditor *>(activeSubWindow->widget());
    return 0;
}

KKSIncludesWidget *KKSMainWindow::activeRubricEditor ()
{
    if (activeObjEditor() || activeCatEditor() || activeTemplateEditor ())
        return 0;

    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<KKSIncludesWidget *>(activeSubWindow->widget());
    return 0;
}

kkslifecycleform * KKSMainWindow::activeLCEditor ()
{
    if (activeObjEditor() || activeCatEditor() || activeTemplateEditor () || activeRubricEditor ())
        return 0;
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<kkslifecycleform *>(activeSubWindow->widget());
    return 0;
}

KKSSearchTemplateForm * KKSMainWindow::activeSTEditor ()
{
    if (activeObjEditor() || activeCatEditor() || activeTemplateEditor () || activeRubricEditor () || activeLCEditor ())
        return 0;
    
    if (QMdiSubWindow *activeSubWindow = m_mdiArea->activeSubWindow())
        return qobject_cast<KKSSearchTemplateForm *>(activeSubWindow->widget());
    return 0;
}
void KKSMainWindow::printActiveSubWindow()
{
    KKSObjEditor * editor = activeObjEditor();

    if (editor)
    {
        editor->print();
    }
    else{
        qCritical() << tr("Current version of DynamicDocs Operator does not print this window");
        QMessageBox::critical(this, tr("Error"), tr("Current version of DynamicDocs Operator does not print this window"), QMessageBox::Ok);
    }
}

void KKSMainWindow::saveActiveSubWindow()
{
    KKSObjEditor * editor = activeObjEditor();

    if (editor)
    {
        int num = m_masscreateW ? m_masscreateW->num () : 1;
        editor->save (num);
        //editor->save();
        return;
    }

    KKSCatEditor *cEditor = activeCatEditor ();
    if (cEditor)
    {
        cEditor->save ();
        return;
    }

    KKSTemplateEditor *tEditor = activeTemplateEditor ();
    if (tEditor)
    {
        tEditor->save ();
        return;
    }

    KKSIncludesWidget *rEditor = activeRubricEditor ();
    if (rEditor)
    {
        rEditor->save ();
        return;
    }
    
    kkslifecycleform * lcEditor = activeLCEditor ();
    if (lcEditor)
    {
        lcEditor->save ();
        return;
    }
    
    KKSSearchTemplateForm * stForm = activeSTEditor ();
    if (stForm)
    {
        stForm->saveSQLQuery();
        return;
    }
}

void KKSMainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;

    QMdiSubWindow * q_win = qobject_cast<QMdiSubWindow *>(window);
    if(!q_win)
        return;
    
    m_mdiArea->setActiveSubWindow(q_win);
}


void KKSMainWindow::slotShowJournls(bool yes)
{
    if(!yes)
        m_journalW->hide();
    else
        m_journalW->show();
}

void KKSMainWindow::slotShowDbgWin(bool yes)
{
    if(!yes)
        m_debugWidget->hide();
    else
        m_debugWidget->show();
}

void KKSMainWindow::slotCreateNewObjEditor(int idObject, 
                                           int idObjExemplar, 
                                           const KKSList<const KKSFilterGroup *> & filters,
                                           const QString & extraTitle)
{
    if(!kksApp->db()->connected())
        return;

    KKSCategory *c = NULL;
    KKSObject *wObj = kksApp->loader()->loadIO (idObjExemplar, true);
    //
    // Если экземпляр ИО также сам является информационным объектом, то задать категорию
    //
    if (wObj)
    {
        if (wObj->category() )
            c = wObj->category();//->tableCategory ();
    }

    KKSObjEditor *objEditor = kksApp->oef()->createObjEditor (idObject, 
                                                               idObjExemplar, 
                                                               filters, 
                                                               extraTitle,
                                                               c,
                                                               false, 
                                                               QString(),
                                                               false,
                                                               Qt::NonModal,
                                                               0);
    if(wObj)
        wObj->release ();

    if(!objEditor){
        qCritical() << tr("Insufficient privileges!");
        QMessageBox::critical(this, tr("Error"), tr("Insufficient privileges!"), QMessageBox::Ok);
        return;
    }

    slotCreateNewObjEditor(objEditor);
}

void KKSMainWindow::slotCreateNewObjEditor(KKSObjEditor * objEditor)
{
    if(!objEditor)
        return;

    QMdiSubWindow * m_objEditorW = m_mdiArea->addSubWindow (objEditor);
    m_objEditorW->setAttribute (Qt::WA_DeleteOnClose);
    connect (objEditor, SIGNAL (closeEditor()), m_objEditorW, SLOT (close()) );
    
    //помечаем, что редактор ИО является MDI-окном. В этом случае при определении активного окна необходимо будет слать сигнал isActiveSubWindow()
    objEditor->setAsSubWindow(true);
    connect(objEditor, SIGNAL(isActiveSubWindow(const KKSObjEditor *, bool *)), this, SLOT(isActiveSubWindow(const KKSObjEditor *, bool *)));


    objEditor->setWindowState (objEditor->windowState() | Qt::WindowMaximized | Qt::WindowActive);
    m_objEditorW->setWindowState (m_objEditorW->windowState() | Qt::WindowMaximized | Qt::WindowActive);
    objEditor->show();
    objEditor->setNumCopies (m_masscreateW->num());

    connect (this->m_masscreateW, SIGNAL (setNum(int)), objEditor, SLOT (setNumCopies (int)) );
}

void KKSMainWindow::slotSearchTemplateEdit (QWidget * stForm)
{
    if (!stForm)
        return;
    QMdiSubWindow * m_StFormW = m_mdiArea->addSubWindow (stForm);
    m_StFormW->setAttribute (Qt::WA_DeleteOnClose);
    stForm->show();
}

void KKSMainWindow::slotCreateNewObjEditor(int attrId, 
                                           const QString & value, 
                                           const QString & extraTitle)
{
    if(!kksApp->db()->connected())
        return;

    KKSList<const KKSFilter *> filters;

    KKSObject * io = kksApp->loader()->loadIO(IO_IO_ID, true);
    if(!io)
        return;

    KKSCategory * c = io->category()->tableCategory();
    if(!c){
        io->release();
        return;
    }

    const KKSFilter * f = c->createFilter(attrId, value, KKSFilter::foInSQL);
    
    io->release();

    if(!f)
        return;
    
    filters.append(f);
    f->release();

    KKSList<const KKSFilterGroup *> filterGroups;
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    slotCreateNewObjEditor(IO_IO_ID, IO_IO_ID, filterGroups, extraTitle);
}

void KKSMainWindow :: slotCreateNewCatEditor (KKSCatEditor *cEditor)
{
    if (!cEditor)
        return;

//    if (!cEditor->parent())
//        cEditor->setParent (this);
    m_mdiArea->addSubWindow(cEditor);
    cEditor->show();

}

void KKSMainWindow :: slotCreateNewRubricEditor (KKSIncludesWidget *rEditor)
{
    if (!rEditor)
        return;

    QMdiSubWindow * m_rubrEditorW = m_mdiArea->addSubWindow (rEditor);
    m_rubrEditorW->setAttribute (Qt::WA_DeleteOnClose);
    rEditor->showMaximized ();
    m_mdiArea->setActiveSubWindow (m_rubrEditorW);
}

void KKSMainWindow :: slotCreateNewTemplateEditor (KKSTemplateEditor *tEditor)
{
    if (!tEditor)
        return;

    m_mdiArea->addSubWindow (tEditor);
    tEditor->show ();
}

/*************************************
**************************************
**************************************
*************************************/

/*
void KKSMainWindow::slotJInCmd()
{
    if(!m_jmonitor)
        return;
    
    //QWidget * parent = ui->objDocWidget;
    //m_jmonitor->showCmdJournal(parent);

    QWidget * w = m_jmonitor->journal();//cmdJournal();
    //QMdiSubWindow * m = new QMdiSubWindow();
    //m->setWidget(w);
    m_mdiArea->addSubWindow(w);
    w->show();
}
*/

void KKSMainWindow::slotClose()
{
    close();
}

void KKSMainWindow::slotAbout()
{
    KKSAbout * f = new KKSAbout(this);
    f->exec();
    delete f;
}

void KKSMainWindow::slotCmdParamsHelp()
{
    KKSCoreApplication::showCommandLineParamsHelp(this);
}

void KKSMainWindow::slotSettings()
{
    KKSSettings * s = kksApp->getKKSSettings();
    if(s)
        s->editSettings(this);
}

void KKSMainWindow::slotConnect()
{
    connectToDb();
}

void KKSMainWindow::slotDisconnect()
{
    disconnectFromDb();
    kksApp->setAllowedUserName(QString::null);
}

void KKSMainWindow::slotChangeUser()
{
    if(!disconnectFromDb())
        return;
    
    kksApp->setAllowedUserName(QString::null);

    connectToDb();
}

void KKSMainWindow::slotConnInfo()
{
    kksApp->showConnectionInfo(this);
}

void KKSMainWindow :: slotCreateDoc()
{
    if(!kksApp->db()->connected())
        return;

    KKSCatEditorFactory * catf = kksApp->catf();
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;
    
    KKSObject * refCatTypeObj = kksApp->loader()->loadIO (IO_CAT_TYPE_ID, true);
    if (!refCatTypeObj || !refCatTypeObj->category() || !refCatTypeObj->category()->tableCategory())
        return;

    KKSFilter * filter = refCatTypeObj->category()->tableCategory()->createFilter(ATTR_ID, QString::number (10), KKSFilter::foNotEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();
    
    refCatTypeObj->release();

    /*
    KKSObject * refCatObj = kksApp->loader()->loadIO (IO_CAT_ID, true);
    if (!refCatObj || !refCatObj->category() || !refCatObj->category()->tableCategory())
        return;
    

    filter = refCatObj->category()->tableCategory()->createFilter(ATTR_IS_ARCHIVED, QString("FALSE"), KKSFilter::foEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();
    refCatObj->release ();
    */

////
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();


    KKSCategoryTemplateWidget* w = catf->viewCategories(filterGroups, 
                                                        true, 
                                                        Qt::ApplicationModal, 
                                                        this, 
                                                        Qt::Dialog);
    //w->show();
    int res = w->exec();
    if(res != QDialog::Accepted){
        return;
    }
    int idCategory = w->getCatID();
    int idTemplate = w->getTemplateID();

    if(idCategory <= 0)
        return;
    
    KKSTemplate * t = NULL;
    KKSCategory * c = NULL;
    if (idTemplate <= 0)
    {
        c = kksApp->loader()->loadCategory(idCategory);
        if (!c)
            return;

        t = new KKSTemplate(c->defTemplate());
    }
    else{
        t = kksApp->loader()->loadTemplate(idTemplate);
        if(!t)
            return;
        c = t->category ();
        if (!c)
            return;
        c->addRef ();
    }

    KKSObjEditorFactory *oef = kksApp->oef ();
    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   -1, 
                                                   KKSList<const KKSFilterGroup*>(), 
                                                   "",
                                                   c,
                                                   false,
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   this);
    c->release();
    t->release();
    slotCreateNewObjEditor(objEditor);
}

void KKSMainWindow::slotFindDoc()
{
    KKSList<const KKSFilter*> filters;
    KKSObject * o = kksApp->loader()->loadIO(IO_IO_ID, true);
    if(!o || !o->category())
        return;
    KKSCategory * c = o->category();//->tableCategory();
    if(!c)
        return;
    if (!c->tableCategory())
        return;

    c = c->tableCategory();

    KKSObjEditorFactory * oef = kksApp->oef();

    int idUser = kksApp->loader()->getUserId();
    KKSFilter * filter = c->createFilter(ATTR_AUTHOR, QString::number(idUser), KKSFilter::foEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();
    
    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   IO_IO_ID, 
                                                   filterGroups, 
                                                   "",
                                                   o->category(),
                                                   false,
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   0);
    slotCreateNewObjEditor(objEditor);
    
    o->release();
}

void KKSMainWindow :: slotMyDocs (void)
{
    if(!kksApp->db()->connected())
        return;

    KKSRubricFactory * rf = kksApp->rf ();
    KKSIncludesWidget * iW = rf->createRubricEditor (KKSRubricFactory::atMyDocsRubric, 
                                                     KKSRubricFactory::smRubricsWithIncludes, 
                                                     false,
                                                     this); //только "мои документы"
    if (!iW)
        return;
}

void KKSMainWindow :: slotRubricControl (void)
{
    if(!kksApp->db()->connected())
        return;

    KKSRubricFactory * rf = kksApp->rf ();
    KKSIncludesWidget * iW = rf->createRubricEditor (KKSRubricFactory::atOthers, 
                                                     KKSRubricFactory::smRubricsWithIncludes, 
                                                     false,
                                                     this);//общесистемный рубрикатор с категориями
    
    if (!iW)
        return;
}

void KKSMainWindow::slotPrintDoc()
{
    printActiveSubWindow();
}

void KKSMainWindow::slotCreateSearchQuery()
{
    KKSObjEditorFactory * oef = kksApp->oef();
    if(!oef)
        return;
    
    oef->createSearchTemplate();
}

void KKSMainWindow::slotShowSearchQueries()
{
    KKSObjEditorFactory * oef = kksApp->oef();
    if(!oef)
        return;
    
    oef->GUISearchTemplate();
}

void KKSMainWindow::slotCreateMsg()
{
    if(!m_jmonitor)
        return;

    m_jmonitor->createMsg();
}

void KKSMainWindow::slotCreateCmd()
{
    if(!m_jmonitor)
        return;

    m_jmonitor->createCmd();
}

void KKSMainWindow::slotCreateTSDRecord()
{
    KKSObjEditorFactory * oef =  kksApp->oef();
    QWidget *parent = this->centralWidget ();
    oef->insertReport (-1, parent, Qt::Dialog);
}

void KKSMainWindow::slotShowTSD()
{
    slotCreateNewObjEditor(IO_IO_ID, IO_TSD_ID, KKSList<const KKSFilterGroup*>(), "");
}

void KKSMainWindow::slotCreateCat()
{
    if(!kksApp->db()->connected())
        return;

    KKSCatEditorFactory * catf = kksApp->catf();

    KKSList<const KKSFilterGroup*> filterGroups;
    KKSCatEditor *catEditor = catf->createCategoryEditor (-1/*IO_CAT_ID*/, filterGroups, false, 3, false, Qt::NonModal, this, Qt::Dialog);
    if (!catEditor)
        return;
}

void KKSMainWindow::slotViewCat()
{
    if(!kksApp->db()->connected())
        return;

    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;
    KKSObject * refCatTypeObj = kksApp->loader()->loadIO (IO_CAT_TYPE_ID, true);
    if (!refCatTypeObj || !refCatTypeObj->category() || !refCatTypeObj->category()->tableCategory())
        return;

    KKSFilter * filter = refCatTypeObj->category()->tableCategory()->createFilter(ATTR_ID, QString::number (10), KKSFilter::foNotEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();
    
    refCatTypeObj->release();

    /*
    KKSObject * refCatObj = kksApp->loader()->loadIO (IO_CAT_ID, true);
    if (!refCatObj || !refCatObj->category() || !refCatObj->category()->tableCategory())
        return;
    

    filter = refCatObj->category()->tableCategory()->createFilter(ATTR_IS_ARCHIVED, QString("FALSE"), KKSFilter::foEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();
    refCatObj->release ();
    */

////
    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    //refCatTypeObj->release ();

    KKSCategoryTemplateWidget* ctw = kksApp->catf()->viewCategories (filterGroups, 
                                                                      false, 
                                                                      Qt::NonModal,
                                                                      this, 
                                                                      Qt::Dialog);
    m_mdiArea->addSubWindow (ctw);
    QSizePolicy ctwSP (QSizePolicy::Expanding, QSizePolicy::Expanding);
    ctw->setSizePolicy (ctwSP);
    ctw->showNormal ();
}

void KKSMainWindow::slotViewAttrs()
{
    if(!kksApp->db()->connected())
        return;

    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;
    KKSObject * refAttrObj = kksApp->loader()->loadIO (IO_ATTR_ID, true);
    if (!refAttrObj || !refAttrObj->category() || !refAttrObj->category()->tableCategory())
        return;

    KKSFilter * filter = refAttrObj->category()->tableCategory()->createFilter(ATTR_IS_SYSTEM, QString("true"), KKSFilter::foNotEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();

    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    refAttrObj->release ();

    KKSAttributesEditor * aEditor = kksApp->attrf()->viewAttributes (filterGroups,
                                                                      false,
                                                                      this->centralWidget (),
                                                                      Qt::Dialog);
    m_mdiArea->addSubWindow (aEditor);
    QSizePolicy attrSP (QSizePolicy::Expanding, QSizePolicy::Expanding);
    aEditor->setSizePolicy (attrSP);
    aEditor->showNormal ();}

void KKSMainWindow::slotEditOSS()
{
    KKSStuffFactory *stf = kksApp->sf ();
    KKSStuffForm * sForm = stf->createStuffEditorForm (4);
    QMdiSubWindow * m_UserPrivW = m_mdiArea->addSubWindow (sForm);
    m_UserPrivW->setAttribute (Qt::WA_DeleteOnClose);
    sForm->show ();
}

void KKSMainWindow::slotSysQualifiers()
{
    QString v = QString("select id from io_categories where id_io_category_type = 8");
    int attrId = 12;

    QString extraTitle = tr("System qualifiers");
    slotCreateNewObjEditor(attrId, v, extraTitle);
}

void KKSMainWindow::slotIndicatorEdit (IndicatorForm * iForm)
{
    QMdiSubWindow * m_IndW = m_mdiArea->addSubWindow (iForm);
    m_IndW->setAttribute (Qt::WA_DeleteOnClose);
    iForm->show ();
}

void KKSMainWindow::slotMess()
{
    qDebug () << __PRETTY_FUNCTION__;
    slotCreateNewObjEditor(IO_IO_ID, IO_MESSAGE_STREAM_ID, KKSList<const KKSFilterGroup*>(), "");
}

void KKSMainWindow::slotCreateLC()
{
    KKSCatEditorFactory * catF = kksApp->catf();
    kkslifecycleform * lcForm = catF->createLifeCycle ();
    QMdiSubWindow * m_LCW = m_mdiArea->addSubWindow (lcForm);
    m_LCW->setAttribute (Qt::WA_DeleteOnClose);
    lcForm->show();
    this->setActiveSubWindow (m_LCW);
}

void KKSMainWindow::slotOpenLC()
{
    KKSCatEditorFactory * catF = kksApp->catf();
    KKSObjEditor * lcEditor = qobject_cast<KKSObjEditor *>(catF->openLifeCycle());

    slotCreateNewObjEditor (lcEditor);
}

void KKSMainWindow::addMapDockWidgets(QDockWidget * legend, QDockWidget * layerOrder)
{
    legend->setParent(this);
    layerOrder->setParent(this);
    addDockWidget(Qt::LeftDockWidgetArea, legend);
    addDockWidget(Qt::LeftDockWidgetArea, layerOrder);
}

void KKSMainWindow::destroyMapDockWidgets(QDockWidget * legend, QDockWidget * layerOrder)
{
    if(legend){
        this->removeDockWidget(legend);
        legend->setParent(0);
        //delete legend;
    }
    
    if(layerOrder){
        this->removeDockWidget(layerOrder);
        layerOrder->setParent(0);
        //delete layerOrder;
    }
}
