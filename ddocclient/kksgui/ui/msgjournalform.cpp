#include "msgjournalform.h"
//#include "ui_cmd_journal_form.h"
#include "msgjournalsettingsform.h"
#include "kksmsgjournallv.h"
#include "kksmsgjournalitem.h"
#include <KKSFile.h>
#include "KKSFilesForm.h"

#include <QMessageBox>
#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QtEvents>
#include <QFrame>
#include <QMainWindow>
#include <QFileDialog>

KKSMsgJournal::KKSMsgJournal(KKSMsgJSettings * settings,
                             const KKSMap<int, KKSEIOData*> & data,
                             int idDl,
                             QWidget *parent, 
                             Qt::WFlags flags)
    : QDockWidget(parent, flags)
{

    setObjectName(tr("Show Message Journal"));
    setWindowTitle(tr("Show Message Journal"));
    
    m_idDl = idDl;

    m_msgJournalLv = new KKSMsgJournalLv(this);
    this->setWidget(m_msgJournalLv);

    m_settings = settings;
    
    initSysMenu();
    initWidgetMenu();
    m_msgJournalLv->setMenu(m_pWidgetMenu);
    connect(m_msgJournalLv, SIGNAL(menuStarted()), this, SLOT(updateMenuItems()));

    setData(data);
}

KKSMsgJournal::~KKSMsgJournal()
{
    //удалять m_settings не надо!
}

void KKSMsgJournal::initSysMenu()
{
    floatingAction = new QAction(tr("Floating"), this);
    floatingAction->setCheckable(true);
    connect(floatingAction, SIGNAL(triggered(bool)), SLOT(changeFloating(bool)));

    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);

    leftAction = new QAction(tr("Place on Left") , this);
    leftAction->setCheckable(true);
    connect(leftAction, SIGNAL(triggered(bool)), SLOT(placeLeft(bool)));

    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, SIGNAL(triggered(bool)), SLOT(placeRight(bool)));

    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, SIGNAL(triggered(bool)), SLOT(placeTop(bool)));

    bottomAction = new QAction(tr("Place on Bottom") , this);
    bottomAction->setCheckable(true);
    connect(bottomAction, SIGNAL(triggered(bool)), SLOT(placeBottom(bool)));

    areaActions->addAction(leftAction);
    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);
    areaActions->addAction(bottomAction);

    m_pSysMenu = new QMenu(tr("Message Journal"), this);
    m_pSysMenu->addAction(toggleViewAction());
    m_pSysMenu->addSeparator();
    m_pSysMenu->addAction(floatingAction);
    m_pSysMenu->addSeparator();
    m_pSysMenu->addActions(areaActions->actions());
}

void KKSMsgJournal::initWidgetMenu()
{
    aSettingsMsg = new QAction(tr("Settings..."), this);
    aSettingsMsg->setStatusTip(tr("Setings of message journal"));
    connect(aSettingsMsg, SIGNAL(triggered()),
            this, SLOT(on_aSettings_triggered()));

    aCreateOutMsg = new QAction(tr("Create Message..."), this);
    aCreateOutMsg->setStatusTip(tr("Create output message"));
    connect(aCreateOutMsg, SIGNAL(triggered()),
            this, SLOT(on_aCreateOutMsg_triggered()));

    aSetReaded = new QAction(tr("Mark as readed"), this);
    aSetReaded->setStatusTip(tr("Mark input message as readed"));
    connect(aSetReaded, SIGNAL(triggered()),
            this, SLOT(on_aSetReaded_triggered()));

    //aDeleteOutCmd = new QAction(tr("Delete Command..."), this);
    //aDeleteOutCmd->setStatusTip(tr("Delete output command"));
    //connect(aDeleteOutCmd, SIGNAL(triggered()),
    //        this, SLOT(on_aDeleteOutCmd_triggered()));

    //aEditOutCmd = new QAction(tr("Edit Command..."), this);
    //aEditOutCmd->setStatusTip(tr("Edit output command"));
    //connect(aEditOutCmd, SIGNAL(triggered()),
    //        this, SLOT(on_aEditOutCmd_triggered()));

    aViewOutMsgResult = new QAction(tr("View message attachment..."), this);
    aViewOutMsgResult->setStatusTip(tr("View document in message attachment"));
    connect(aViewOutMsgResult, SIGNAL(triggered()),
            this, SLOT(on_aViewOutMsgResult_triggered()));

    aViewMsg = new QAction(tr("View message..."), this);
    aViewMsg->setStatusTip(tr("View message"));
    connect(aViewMsg, SIGNAL(triggered()),
            this, SLOT(on_aViewMsg_triggered()));

    aViewOutMsgFile = new QAction(tr("View message attachment files..."), this);
    aViewOutMsgFile->setStatusTip(tr("View message files"));
    connect(aViewOutMsgFile, SIGNAL(triggered()),
            this, SLOT(viewMsgFiles()));

    //aExecuteInCmd->setStatusTip(tr("Execute input command"));
    //connect(aExecuteInCmd, SIGNAL(triggered()),
    //        this, SLOT(on_aExecute_triggered()));
    //aSetExecuting = new QAction(tr("Set as executing"), this);
    //aSetExecuting->setStatusTip(tr("Set Command As Executing"));
    //connect(aSetExecuting, SIGNAL(triggered()),
    //        this, SLOT(on_aSetAsExecuting_triggered()));

    m_pWidgetMenu = new QMenu(tr("Message Journal"), this);
    
    QMenu * inMsgMenu = new QMenu(tr("Input Messages"),  this);
    //inCmdMenu->addAction(aExecuteInCmd);
    inMsgMenu->addAction(aSetReaded);
    //inMsgMenu->addSeparator();
    //inCmdMenu->addAction(aSetExecuted);
    m_pWidgetMenu->addMenu(inMsgMenu);

    QMenu * outMsgMenu = new QMenu(tr("Output Messages"),  this);
    outMsgMenu->addAction(aCreateOutMsg);
    outMsgMenu->addSeparator();
    //outCmdMenu->addAction(aEditOutCmd);
    //outCmdMenu->addAction(aDeleteOutCmd);
    m_pWidgetMenu->addMenu(outMsgMenu);

    m_pWidgetMenu->addSeparator();
    m_pWidgetMenu->addAction (aViewOutMsgResult);
    m_pWidgetMenu->addAction (aViewOutMsgFile);
    m_pWidgetMenu->addAction (aViewMsg);
    m_pWidgetMenu->addSeparator();

    QMenu * menu = new QMenu(tr("View"), this);
    menu->addAction(toggleViewAction());
    menu->addSeparator();
    menu->addAction(floatingAction);
    menu->addSeparator();
    menu->addActions(areaActions->actions());

    m_pWidgetMenu->addMenu(menu);

    m_pWidgetMenu->addSeparator();
    m_pWidgetMenu->addAction(aSettingsMsg);
}

void KKSMsgJournal::init()
{
    int oldId = -1;
    if(!m_data.isEmpty()){
        KKSMessage msg = m_msgJournalLv->currentMessage();
        if(msg.id() > 0)
            oldId = msg.id();
    }
    
    m_msgJournalLv->clear();

    if(m_data.isEmpty())
        return;

    int row = 0;
    KKSMap<int, KKSEIOData*>::const_iterator pd;
    for (pd = m_data.constBegin(); pd != m_data.constEnd(); pd++)
    {
        KKSEIOData * record = pd.value();
        if(!record)
            continue;

        KKSMessage msg;
        // № п/п
        msg.setId(record->fieldValue("id").toInt());
        
        msg.setObjectId(record->fieldValue("id_io_object").toInt());
        msg.setObjectName(record->fieldValue("object_name"));
        
        msg.setCategoryId(record->fieldValue("id_io_category").toInt());
        msg.setCategoryName(record->fieldValue("category_name"));


        int idDlReceiver = record->fieldValue("id_dl_receiver").toInt();
        int idDlSender = record->fieldValue("id_dl_sender").toInt();
        int jrType = record->fieldValue("msg_type").toInt();
        Q_UNUSED (jrType);
        KKSMessage::MsgType type;
        if(m_idDl == idDlReceiver)
            type = KKSMessage::mtIn;
        else if(m_idDl == idDlSender)
            type = KKSMessage::mtOut;
        else
            type = KKSMessage::mtIn;

        msg.setMsgType(type);

        msg.setDlTo(record->fieldValue("dl_receiver_name"));
        msg.setDlFrom(record->fieldValue("dl_sender_name"));

        QDateTime dt = KKSValue::stringToDateTime(record->fieldValue("sent_datetime"));
        msg.setSentDateTime(dt);

        dt = KKSValue::stringToDateTime(record->fieldValue("receive_datetime"));
        msg.setReceiveDateTime(dt);

        dt = KKSValue::stringToDateTime(record->fieldValue("read_datetime"));
        msg.setReadDateTime(dt);

        dt = KKSValue::stringToDateTime(record->fieldValue("insert_time"));
        msg.setInsertTime(dt);

        msg.setMessageBody(record->fieldValue("message_body"));
        
        m_msgJournalLv->addMessage(msg);
        if(msg.id() == oldId){
            QModelIndex index = m_msgJournalLv->model()->index(row, 0);
            m_msgJournalLv->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Rows);
        }
        
        row ++;
    }

    m_msgJournalLv->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void KKSMsgJournal::on_aSettings_triggered()
{
    KKSMsgJSettings settings = KKSMsgJSettings(*m_settings);
    MsgJournalSettingsForm * f = new MsgJournalSettingsForm(settings, this);
    if(f->exec() == QDialog::Accepted){
        settings = f->settings();
    
        //m_settings->bAllInput = settings.bAllInput;
        //m_settings->bAllOutput = settings.bAllOutput;
        m_settings->bMyInput = settings.bMyInput;
        //m_settings->bInputExecuted = settings.bInputExecuted;
        //m_settings->bOutputExecuted = settings.bOutputExecuted;
        //m_settings->bInputArchived = settings.bInputArchived;
        //m_settings->bOutputArchived = settings.bOutputArchived;
        m_settings->bMyOutput = settings.bMyOutput;
        m_settings->bUseInputDate = settings.bUseInputDate;
        m_settings->bUseOutputDate = settings.bUseOutputDate;
        m_settings->dInputDate = settings.dInputDate;
        m_settings->dOutputDate = settings.dOutputDate;
        m_settings->queryInterval = settings.queryInterval;

        emit settingsChanged();
    }

    delete f;
}

int KKSMsgJournal::currentJournalRecord() const
{
    KKSMessage msg = m_msgJournalLv->currentMessage();
    int idJournal = msg.id();

    return idJournal;
}

void KKSMsgJournal::setData(const KKSMap<int, KKSEIOData*> & data)
{
    m_data = data;
    init();
}


void KKSMsgJournal::contextMenuEvent(QContextMenuEvent *e)
{
    e->accept();
    
    updateMenuItems();

    m_pWidgetMenu->exec(e->globalPos());
}

void KKSMsgJournal::updateMenuItems()
{
    KKSMessage msg = m_msgJournalLv->currentMessage();

    bool isCanViewAttachment = canViewAttachement (msg);
    aViewOutMsgResult->setEnabled (isCanViewAttachment);
    if (isCanViewAttachment)
        emit loadMsgFiles (msg.objectId());
    else
        aViewOutMsgFile->setEnabled (false);
    bool isSetRead = canSetAsReaded (msg);
    aSetReaded->setEnabled (isSetRead);
}

void KKSMsgJournal :: isFilesEnabled (const KKSList<KKSFile *>& files)
{
    int nFiles = files.count();
    aViewOutMsgFile->setEnabled (nFiles > 0);
    msgFiles = files;
}

bool KKSMsgJournal::event(QEvent *e)
{
    if (e->type() != QEvent::Polish)
        return QDockWidget::event(e);

    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    const Qt::DockWidgetArea area = mainWindow->dockWidgetArea(this);
    const Qt::DockWidgetAreas areas = allowedAreas();

    if (windowType() == Qt::Drawer) {
        floatingAction->setEnabled(false);
    } else {
        floatingAction->setChecked(isWindow());
    }

    leftAction->blockSignals(true);
    rightAction->blockSignals(true);
    topAction->blockSignals(true);
    bottomAction->blockSignals(true);

    leftAction->setChecked(false);
    rightAction->setChecked(false);
    topAction->setChecked(false);
    bottomAction->setChecked(false);

    leftAction->setChecked(area == Qt::LeftDockWidgetArea);
    rightAction->setChecked(area == Qt::RightDockWidgetArea);
    topAction->setChecked(area == Qt::TopDockWidgetArea);
    bottomAction->setChecked(area == Qt::BottomDockWidgetArea);

    leftAction->blockSignals(false);
    rightAction->blockSignals(false);
    topAction->blockSignals(false);
    bottomAction->blockSignals(false);

    return QDockWidget::event(e);
}

void KKSMsgJournal::place(Qt::DockWidgetArea area, bool p)
{
    if (!p) 
        return;

    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    if(mainWindow)
        mainWindow->addDockWidget(area, this);
}

void KKSMsgJournal::changeFloating(bool floating)
{ 
    setFloating(floating); 
}


void KKSMsgJournal::placeLeft(bool p)
{ 
    place(Qt::LeftDockWidgetArea, p); 
}

void KKSMsgJournal::placeRight(bool p)
{ 
    place(Qt::RightDockWidgetArea, p); 
}

void KKSMsgJournal::placeTop(bool p)
{ 
    place(Qt::TopDockWidgetArea, p); 
}

void KKSMsgJournal::placeBottom(bool p)
{ 
    place(Qt::BottomDockWidgetArea, p); 
}

void KKSMsgJournal::on_aCreateOutMsg_triggered()
{
    emit createOutMsg();
}

void KKSMsgJournal::on_aViewMsg_triggered()
{
    int id = currentJournalRecord();
    if(id > 0)
        emit viewMsg(id);
}

void KKSMsgJournal::on_aViewOutMsgResult_triggered()
{
    KKSMessage msg = m_msgJournalLv->currentMessage();
    int idObject = msg.objectId();
    //int id = currentJournalRecord();
    if(idObject > 0)
        emit viewResultMsg(idObject);
}

void KKSMsgJournal :: viewMsgFiles ()
{
    int nFiles = msgFiles.count();
    qDebug () << __PRETTY_FUNCTION__ << nFiles;
    if (nFiles == 1)
    {
        KKSFile * f = msgFiles[0];
        prepareFile (f);
    }
    else
    {
        KKSFilesForm * filesForm = new KKSFilesForm (msgFiles);
        qDebug () << __PRETTY_FUNCTION__ << nFiles;
        if (filesForm && filesForm->exec () == QDialog::Accepted)
        {
            KKSFile * f = filesForm->getFile();
            prepareFile (f);
        }
        if (filesForm)
            delete filesForm;
    }
}

void KKSMsgJournal::prepareFile (KKSFile *f)
{
    if (!f)
    {
        qWarning() << tr("Choose file for download!");
        QMessageBox::warning(this, 
                             tr("Download file"), 
                             tr("Choose file for download!"), 
                             QMessageBox::Ok);

        return;
    }
    
    if(!f->loaded() || f->localUrl().isEmpty())
    {
        int answer = QMessageBox::question(this, 
                                           tr("Open file"), 
                                           tr("The file is not downloaded!\n"
                                              "Do you want to download it?"), 
                                           QMessageBox::Yes | QMessageBox::No);
        if(answer == QMessageBox::No)
            return;
    }

    if (f->id() <= 0)
    {
        qCritical() << tr("The file is not saved to database, just locally added!\n"
                                 "You can't download it");
        QMessageBox::critical(this, 
                              tr("Download file"), 
                              tr("The file is not saved to database, just locally added!\n"
                                 "You can't download it"), 
                              QMessageBox::Ok);
        return;
    }

    if (f->loaded())
    {
        int answer = QMessageBox::question(this, 
                                           tr("Download file"), 
                                           tr("The file is already downloaded!\n"
                                              "Do you want to download it once more?"), 
                                           QMessageBox::Yes | QMessageBox::No);
        if (answer == QMessageBox::No)
            return;
    }
    
    QString filter = QString("(*.") + f->srcExt() + ")";
    QString selectedFilter;
    QString localFile = QFileDialog::getSaveFileName(this, 
                                                     tr("Choose file to save"),
                                                     QString(),
                                                     filter,
                                                     &selectedFilter);

    if (localFile.isEmpty())
        return;

    f->setLocalUrl (localFile);

    emit downloadFile (f, this);//msgFiles[0]);
    if (!f->loaded() || f->localUrl().isEmpty())
    {
        int answer = QMessageBox::question(this, 
                                           tr("Open file"), 
                                           tr("The file is not downloaded!\n"
                                              "Do you want to download it?"), 
                                           QMessageBox::Yes | QMessageBox::No);
        if(answer == QMessageBox::No)
            return;
    }

    if (!f->open())
    {
        qCritical() << tr("Cannot open the file!\n\n"
                                 "If you about to open file of Microsoft Visio type,\n"
                                 "try to rename it with extention. Example: filename.vsd");
        QMessageBox::critical(this, 
                              tr("Open file"), 
                              tr("Cannot open the file!\n\n"
                                 "If you about to open file of Microsoft Visio type,\n"
                                 "try to rename it with extention. Example: filename.vsd"), 
                              QMessageBox::Ok);
    }
}

void KKSMsgJournal::on_aSetReaded_triggered()
{
    int id = currentJournalRecord();
    if(id > 0)
        emit markAsReaded(id);
}

void KKSMsgJournal::slotShowError(QString error) const
{
    qCritical() << error;
    QMessageBox::critical(this->widget(), tr("Error"), error, QMessageBox::Ok);
}

bool KKSMsgJournal::canSetAsReaded(const KKSMessage & msg)
{
    if(msg.id() <= 0)
        return false;

    if(msg.msgType() != KKSMessage::mtIn)
    {
        return false;
    }

    QString text = msg.readDateTime().toString("dd.MM.yyyy");
    if(!text.isEmpty())
        return false;

    return true;
}

bool KKSMsgJournal::canViewAttachement(const KKSMessage & msg)
{
    if(msg.id() <= 0)
        return false;

    if(msg.objectId() <= 0)
        return false;
    
    return true;
}
