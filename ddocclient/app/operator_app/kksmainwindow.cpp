#include "kksmainwindow.h"
#include "kksmasswidget.h"
#include "cmdjournalform.h"
#include "msgjournalform.h"
#include "kksstatusbar.h"
#include "kksabout.h"
#include "kkssito.h"
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
#include <KKSIndFactory.h>

#include <kksincludeswidget.h>
#include <kkscmdjournalitem.h>
#include <kksmsgjournalitem.h>
#include <KKSJournalWidget.h>
#include <kksstuffform.h>
#include <IndicatorForm.h>

#include "cmdjournalsettingsform.h"
#include "msgjournalsettingsform.h"

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

    KKSObjEditorFactory * oef = kksSito->oef();
    KKSCatEditorFactory * catf = kksSito->catf ();
    KKSTemplateEditorFactory * tf = kksSito->tf ();
    KKSRubricFactory * rf = kksSito->rf ();
    KKSIndFactory * indf = kksSito->indf ();
    connect (oef, 
             SIGNAL(editorCreated(KKSObjEditor *)), 
             this, 
             SLOT(slotCreateNewObjEditor(KKSObjEditor*)));

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

    connect (indf,
             SIGNAL (viewIndicatorWidget (IndicatorForm *)),
             this,
             SLOT (slotIndicatorEdit (IndicatorForm *))
            );

    init();
    
    showMaximized();
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
    
    initConnections();
    setActionsEnabled(false);
    updateMenus();
}

void KKSMainWindow::closeEvent(QCloseEvent *event)
{
    m_mdiArea->closeAllSubWindows();
    if (activeObjEditor()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void KKSMainWindow::updateMenus()
{
    bool hasMdiChild = (activeObjEditor() != 0
                        || activeCatEditor() != 0
                        || activeTemplateEditor() != 0
                        || activeRubricEditor() != 0);
    
    aCloseWindow->setEnabled(hasMdiChild);
    aCloseAllWindows->setEnabled(hasMdiChild);
    aTile->setEnabled(hasMdiChild);
    aCascade->setEnabled(hasMdiChild);
    aNextWindow->setEnabled(hasMdiChild);
    aPreviousWindow->setEnabled(hasMdiChild);
    aSaveIO->setEnabled(hasMdiChild);
    aSeparator->setVisible(hasMdiChild);
}

void KKSMainWindow::updateWindowMenu()
{
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

    QList<QMdiSubWindow *> windows = m_mdiArea->subWindowList();
    aSeparator->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        
        KKSObjEditor *child = qobject_cast<KKSObjEditor*>(windows.at(i)->widget());

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
        action ->setChecked(child == activeObjEditor());
        connect(action, SIGNAL(triggered()), m_windowMapper, SLOT(map()));
        m_windowMapper->setMapping(action, windows.at(i));
    }
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

    if(enabled){
        QString uName = kksSito->loader()->getDb()->getUser();
        if(uName == "admin"){
            ui->aCreateCat->setEnabled(enabled);
            ui->aViewCat->setEnabled(enabled);
            ui->aViewAttrs->setEnabled(enabled);
            ui->aEditOSS->setEnabled(enabled);
            ui->aSysQualifiers->setEnabled(enabled);
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

    updateWindowMenu();

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


void KKSMainWindow::initToolBars()
{
    QToolBar * tbActions = new QToolBar(tr("Docs and Journals toolbar"), this);
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
    
    
    QToolBar * tbPost = new QToolBar(tr("Post toolbar"), this);
    //tbPost->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbPost->setIconSize(QSize(32, 32));
    tbPost->addAction(ui->aCreateMsg);
    tbPost->addAction(ui->aCreateCmd);
    tbPost->addSeparator();
    tbPost->addAction(ui->aCreateTSDRecord);
    tbPost->addAction(ui->aShowTSD);

    QToolBar * tbQuery = new QToolBar(tr("Search query toolbar"), this);
    //tbQuery->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbQuery->setIconSize(QSize(32, 32));
    tbQuery->addAction(ui->aCreateSearchQuery);
    tbQuery->addAction(ui->aShowSearchQueries);

    QToolBar * tbAdmin = new QToolBar(tr("Admin toolbar"), this);
    //tbAdmin->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    tbAdmin->setIconSize(QSize(32, 32));
    tbAdmin->addAction(ui->aCreateCat);
    tbAdmin->addAction(ui->aViewCat);
    tbAdmin->addAction(ui->aViewAttrs);
    tbAdmin->addAction(ui->aEditOSS);
    tbAdmin->addAction(ui->aSysQualifiers);

    QToolBar * tbOrg = new QToolBar (tr("Organization toolbar"), this);
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

    QMenu * menuToolBars = new QMenu(tr("Toolbars"), this);
    ui->menuView->addMenu(menuToolBars);
    menuToolBars->addAction(tbActions->toggleViewAction());
    menuToolBars->addAction(tbPost->toggleViewAction());
    menuToolBars->addAction (tbQuery->toggleViewAction());
    menuToolBars->addAction (tbAdmin->toggleViewAction());
}

void KKSMainWindow::initIcons()
{

    ui->aConnect->setIcon(QIcon(":/ddoc/connect.png"));
    ui->aChangeUser->setIcon(QIcon(":/ddoc/change_user.png"));
    ui->aDisconnect->setIcon(QIcon(":/ddoc/disconnect.png"));
    ui->aConnInfo->setIcon(QIcon(":/ddoc/connection_info.png"));
    //ui->aQuit->setIcon();

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

    
    /*
    KKSCommand::setPxIn(QPixmap(":/ico/myInput.xpm"));
    KKSCommand::setPxInAll(QPixmap(":/ico/allInput.xpm"));
    KKSCommand::setPxOut(QPixmap(":/ico/myOutput.xpm"));
    KKSCommand::setPxOutAll(QPixmap(":/ico/allOutput.xpm"));
    KKSCommand::setPxArchived(QPixmap(":/ico/archived.xpm"));
    */

    QIcon rubricIcon (":/ddoc/rubric.png");
    QIcon rubricItemIcon (":/ddoc/rubric_item.png");
    KKSRubric::setIcon(rubricIcon.pixmap (QSize(24,24)));
    KKSRubricItem::setIcon(rubricItemIcon.pixmap (QSize(24,24)));

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

    connect(ui->aSettings, SIGNAL(triggered()), this, SLOT(slotSettings()));

    connect(ui->aAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));
}

void KKSMainWindow::initStatusBar()
{
    KKSStatusBar * statusBar = new KKSStatusBar(m_userName, m_dlName, this);

    QPixmap px;
    statusBar->setHellow(px, m_userName, m_dlName);
    
    statusBar->showStatusBar();

    setStatusBar(statusBar);
}



bool KKSMainWindow::connectToDb()
{
    int res = kksSito->GUIConnect(this);
    if(res == ERROR_CODE){
        setActionsEnabled(false);
    }
    else
        setActionsEnabled(true);
    
    if(kksSito->db1()->connected()){
        //создаем копии и передаем управление ими в класс KKSJMonitor. 
        //В деструкторе этого класса они будут удалены
        KKSCmdJSettings * settings = new KKSCmdJSettings(*(kksSito->getCmdJSettings()));
        KKSMsgJSettings * msgSettings = new KKSMsgJSettings(*(kksSito->getMsgJSettings()));
        m_jmonitor = new KKSJMonitor(settings, 
                                     msgSettings,
                                     kksSito->db1(), 
                                     kksSito->oef(),
                                     kksSito->eiof());
        

        connect(m_jmonitor, 
                SIGNAL(editorCreated(KKSObjEditor *)), 
                this, 
                SLOT(slotCreateNewObjEditor(KKSObjEditor*)));
        connect(m_jmonitor, SIGNAL(settingsChanged(KKSCmdJSettings *)), kksSito, SLOT(saveCmdJSettings(KKSCmdJSettings *)));
        connect(m_jmonitor, SIGNAL(msgSettingsChanged(KKSMsgJSettings *)), kksSito, SLOT(saveMsgJSettings(KKSMsgJSettings *)));
    }
    else
        return false;

    m_userName = kksSito->loader()->getUserName();
    m_dlName = kksSito->loader()->getDlName();
    m_maclabelName = kksSito->loader()->getCurrMacLabelName();
    
    m_modeName = kksSito->loader()->getOrgModeName ();
    lModeName->setText (m_modeName);
    m_orgSymbol = kksSito->loader()->getOrgLogo ();
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

    if(kksSito->loader()->getLocalOrgId() == -1){
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

    kksSito->db()->disconnect();
    kksSito->db1()->disconnect();
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

void KKSMainWindow::printActiveSubWindow()
{
    KKSObjEditor * editor = activeObjEditor();

    if (editor)
    {
        editor->print();
    }
    else{
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
}

void KKSMainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    m_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void KKSMainWindow::slotCreateNewObjEditor(int idObject, 
                                           int idObjExemplar, 
                                           const KKSList<const KKSFilterGroup *> & filters,
                                           const QString & extraTitle)
{
    if(!kksSito->db()->connected())
        return;

    KKSCategory *c = NULL;
    KKSObject *wObj = kksSito->loader()->loadIO (idObjExemplar, true);
    //
    // Если экземпляр ИО также сам является информационным объектом, то задать категорию
    //
    if (wObj)
    {
        if (wObj->category() )
            c = wObj->category();//->tableCategory ();
    }

    KKSObjEditor *objEditor = kksSito->oef()->createObjEditor (idObject, 
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
//    int idObj = objEditor->getObj () ? objEditor->getObj()->id() : -1;
//    int idObjC = objEditor->getObjectEx() ? objEditor->getObjectEx()->id() : -1;
//   if (idObj == IO_IO_ID && idObjC == IO_IO_ID)
//    {
    objEditor->setWindowState (objEditor->windowState() | Qt::WindowMaximized | Qt::WindowActive);
    m_objEditorW->setWindowState (m_objEditorW->windowState() | Qt::WindowMaximized | Qt::WindowActive);
    objEditor->show();//Maximized();
    //    qDebug () << __PRETTY_FUNCTION__ << objEditor->size () << m_objEditorW->size ();
//    }
//    else
//        qDebug () << __PRETTY_FUNCTION__ << objEditor->size () << m_objEditorW->size ();
/*    objEditor->show();
    QSize sobj (objEditor->layout ()->minimumSize ());

    objEditor->resize (objEditor->width(),
                       sobj.height() +
                           objEditor->geometry().y() -
                           objEditor->y());

    m_objEditorW->resize (m_objEditorW->layout()->sizeHint().width(),
                          m_objEditorW->layout()->minimumSize().height() +
                              m_objEditorW->geometry().y() -
                              m_objEditorW->y() +
                              34);*/

}


void KKSMainWindow::slotCreateNewObjEditor(int attrId, 
                                           const QString & value, 
                                           const QString & extraTitle)
{
    if(!kksSito->db()->connected())
        return;

    KKSList<const KKSFilter *> filters;

    KKSObject * io = kksSito->loader()->loadIO(IO_IO_ID, true);
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

void KKSMainWindow::slotSettings()
{
    KKSSettings * s = kksSito->getKKSSettings();
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
}

void KKSMainWindow::slotChangeUser()
{
    if(!disconnectFromDb())
        return;
    
    connectToDb();
}

void KKSMainWindow::slotConnInfo()
{
    kksSito->showConnectionInfo(this);
}

void KKSMainWindow :: slotCreateDoc()
{
    if(!kksSito->db()->connected())
        return;

    KKSCatEditorFactory * catf = kksSito->catf();
    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;
    KKSObject * refCatTypeObj = kksSito->loader()->loadIO (IO_CAT_TYPE_ID, true);
    if (!refCatTypeObj || !refCatTypeObj->category() || !refCatTypeObj->category()->tableCategory())
        return;

    KKSFilter * filter = refCatTypeObj->category()->tableCategory()->createFilter(1, QString::number (10), KKSFilter::foNotEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();

    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    refCatTypeObj->release ();

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
        c = kksSito->loader()->loadCategory(idCategory);
        if (!c)
            return;

        t = new KKSTemplate(c->defTemplate());
    }
    else{
        t = kksSito->loader()->loadTemplate(idTemplate);
        if(!t)
            return;
        c = t->category ();
        if (!c)
            return;
        c->addRef ();
    }

    KKSObjEditorFactory *oef = kksSito->oef ();
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
    KKSObject * o = kksSito->loader()->loadIO(IO_IO_ID, true);
    if(!o || !o->category())
        return;
    KKSCategory * c = o->category();//->tableCategory();
    if(!c)
        return;
    if (!c->tableCategory())
        return;

    c = c->tableCategory();

    KKSObjEditorFactory * oef = kksSito->oef();

    int idUser = kksSito->loader()->getUserId();
    KKSFilter * filter = c->createFilter(13, QString::number(idUser), KKSFilter::foEq);
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
    if(!kksSito->db()->connected())
        return;

    KKSRubricFactory * rf = kksSito->rf ();
    KKSList<const KKSFilterGroup*> filterGroups;
    KKSIncludesWidget * iW = rf->createRubricEditor (1, filterGroups, this);
    if (!iW)
        return;
}

void KKSMainWindow :: slotRubricControl (void)
{
    if(!kksSito->db()->connected())
        return;

    KKSRubricFactory * rf = kksSito->rf ();
    KKSList<const KKSFilterGroup*> filterGroups;
    KKSIncludesWidget * iW = rf->createRubricEditor (0, filterGroups, this);
    
    if (!iW)
        return;
}

void KKSMainWindow::slotPrintDoc()
{
    printActiveSubWindow();
}

void KKSMainWindow::slotCreateSearchQuery()
{
    KKSObjEditorFactory * oef = kksSito->oef();
    if(!oef)
        return;
    
    oef->createSearchTemplate();
}

void KKSMainWindow::slotShowSearchQueries()
{
    KKSObjEditorFactory * oef = kksSito->oef();
    if(!oef)
        return;
    
    oef->loadSearchTemplate();
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
    KKSObjEditorFactory * oef =  kksSito->oef();
    QWidget *parent = this->centralWidget ();
    oef->insertReport (-1, parent, Qt::Dialog);
}

void KKSMainWindow::slotShowTSD()
{
    slotCreateNewObjEditor(IO_IO_ID, IO_TSD_ID, KKSList<const KKSFilterGroup*>(), "");
}

void KKSMainWindow::slotCreateCat()
{
    if(!kksSito->db()->connected())
        return;

    KKSCatEditorFactory * catf = kksSito->catf();

    KKSList<const KKSFilterGroup*> filterGroups;
    KKSCatEditor *catEditor = catf->createCategoryEditor (-1/*IO_CAT_ID*/, filterGroups, false, 3, false, Qt::NonModal, this, Qt::Dialog);
    if (!catEditor)
        return;
}

void KKSMainWindow::slotViewCat()
{
    if(!kksSito->db()->connected())
        return;

    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;
    KKSObject * refCatTypeObj = kksSito->loader()->loadIO (IO_CAT_TYPE_ID, true);//IO_CAT_ID);
    if (!refCatTypeObj || !refCatTypeObj->category() || !refCatTypeObj->category()->tableCategory())
        return;

    KKSFilter * filter = refCatTypeObj->category()->tableCategory()->createFilter(1, QString::number (10), KKSFilter::foNotEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();

    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    refCatTypeObj->release ();

    KKSCategoryTemplateWidget* ctw = kksSito->catf()->viewCategories (filterGroups, 
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
    if(!kksSito->db()->connected())
        return;

    KKSList<const KKSFilterGroup *> filterGroups;
    KKSList<const KKSFilter*> filters;
    KKSObject * refAttrObj = kksSito->loader()->loadIO (IO_ATTR_ID, true);
    if (!refAttrObj || !refAttrObj->category() || !refAttrObj->category()->tableCategory())
        return;

    KKSFilter * filter = refAttrObj->category()->tableCategory()->createFilter(15, QString("true"), KKSFilter::foNotEq);
    if(!filter)
        return;

    filters.append(filter);
    filter->release();

    KKSFilterGroup * group = new KKSFilterGroup(true);
    group->setFilters(filters);
    filterGroups.append(group);
    group->release();

    refAttrObj->release ();

    KKSAttributesEditor * aEditor = kksSito->attrf()->viewAttributes (filterGroups,
                                                                      false,
                                                                      this->centralWidget (),
                                                                      Qt::Dialog);
    m_mdiArea->addSubWindow (aEditor);
    QSizePolicy attrSP (QSizePolicy::Expanding, QSizePolicy::Expanding);
    aEditor->setSizePolicy (attrSP);
    aEditor->showNormal ();}

void KKSMainWindow::slotEditOSS()
{
    KKSStuffFactory *stf = kksSito->sf ();
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