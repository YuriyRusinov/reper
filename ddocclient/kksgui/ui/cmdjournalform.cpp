#include "cmdjournalform.h"
//#include "ui_cmd_journal_form.h"
#include "cmdjournalsettingsform.h"
#include "kkscmdjournallv.h"
#include "kkscmdjournalitem.h"

#include <QMessageBox>
#include <QTreeWidget>
#include <QMenu>
#include <QAction>
#include <QtEvents>
#include <QFrame>
#include <QMainWindow>


KKSCmdJournal::KKSCmdJournal(KKSCmdJSettings * settings,
                             const KKSMap<int, KKSEIOData*> & data,
                             int idDl,
                             QWidget *parent, 
                             Qt::WFlags flags)
    : QDockWidget(parent, flags)
{

    setObjectName(tr("Show Command Journal"));
    setWindowTitle(tr("Show Command Journal"));
    
    m_idDl = idDl;

    m_cmdJournalLv = new KKSCmdJournalLv (this);
    this->setWidget(m_cmdJournalLv);

    m_settings = settings;
    
    initSysMenu();
    initWidgetMenu();
    m_cmdJournalLv->setMenu(m_pWidgetMenu);
    connect(m_cmdJournalLv, SIGNAL(menuStarted()), this, SLOT(updateMenuItems()));

    setData(data);
}

KKSCmdJournal::~KKSCmdJournal()
{
    //удалять m_settings не надо!
}

void KKSCmdJournal::initSysMenu()
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

    m_pSysMenu = new QMenu(tr("Command Journal"), this);
    m_pSysMenu->addAction(toggleViewAction());
    m_pSysMenu->addSeparator();
    m_pSysMenu->addAction(floatingAction);
    m_pSysMenu->addSeparator();
    m_pSysMenu->addActions(areaActions->actions());
}

void KKSCmdJournal::initWidgetMenu()
{
    aSettingsCmd = new QAction(tr("Settings..."), this);
    aSettingsCmd->setStatusTip(tr("Setings of command journal"));
    connect(aSettingsCmd, SIGNAL(triggered()),
            this, SLOT(on_aSettings_triggered()));

    aCreateOutCmd = new QAction(tr("Create Command..."), this);
    aCreateOutCmd->setStatusTip(tr("Create output command"));
    connect(aCreateOutCmd, SIGNAL(triggered()),
            this, SLOT(on_aCreateOutCmd_triggered()));

    aDeleteOutCmd = new QAction(tr("Delete Command..."), this);
    aDeleteOutCmd->setStatusTip(tr("Delete output command"));
    connect(aDeleteOutCmd, SIGNAL(triggered()),
            this, SLOT(on_aDeleteOutCmd_triggered()));

    aEditOutCmd = new QAction(tr("Edit Command..."), this);
    aEditOutCmd->setStatusTip(tr("Edit output command"));
    connect(aEditOutCmd, SIGNAL(triggered()),
            this, SLOT(on_aEditOutCmd_triggered()));

    aDropPeriodCmd = new QAction(tr("Drop period command..."), this);
    aDropPeriodCmd->setStatusTip(tr("Drop period command"));
    connect(aDropPeriodCmd, SIGNAL(triggered()),
            this, SLOT(on_aDropPeriodCmd_triggered()));

    aViewCmd = new QAction(tr("View command..."), this);
    aViewCmd->setStatusTip(tr("View command"));
    connect(aViewCmd, SIGNAL(triggered()),
            this, SLOT(on_aViewCmd_triggered()));

    aViewCmdDoc = new QAction(tr("View command attachement..."), this);
    aViewCmdDoc->setStatusTip(tr("View command attachement"));
    connect(aViewCmdDoc, SIGNAL(triggered()),
            this, SLOT(on_aViewCmdDoc_triggered()));

    aViewOutCmdResult = new QAction(tr("View result..."), this);
    aViewOutCmdResult->setStatusTip(tr("View executed result of command"));
    connect(aViewOutCmdResult, SIGNAL(triggered()),
            this, SLOT(on_aViewOutCmdResult_triggered()));

    aExecuteInCmd = new QAction(tr("Execute Command..."), this);
    aExecuteInCmd->setStatusTip(tr("Execute input command"));
    connect(aExecuteInCmd, SIGNAL(triggered()),
            this, SLOT(on_aExecute_triggered()));
    aExecuteExInCmd = new QAction(tr("Choose document as command result..."), this);
    aExecuteExInCmd->setStatusTip(tr("Choose document as command result"));
    connect(aExecuteExInCmd, SIGNAL(triggered()),
            this, SLOT(on_aExecuteEx_triggered()));

    aSetExecuting = new QAction(tr("Set as executing"), this);
    aSetExecuting->setStatusTip(tr("Set Command As Executing"));
    connect(aSetExecuting, SIGNAL(triggered()),
            this, SLOT(on_aSetAsExecuting_triggered()));

    m_pWidgetMenu = new QMenu(tr("Command Journal"), this);
    
    QMenu * inCmdMenu = new QMenu(tr("Input Commands"),  this);
    inCmdMenu->addAction(aSetExecuting);
    inCmdMenu->addSeparator();
    inCmdMenu->addAction(aExecuteInCmd);
    inCmdMenu->addAction(aExecuteExInCmd);
    //inCmdMenu->addAction(aSetExecuted);
    m_pWidgetMenu->addMenu(inCmdMenu);

    QMenu * outCmdMenu = new QMenu(tr("Output Commands"),  this);
    outCmdMenu->addAction(aCreateOutCmd);
    outCmdMenu->addSeparator();
    outCmdMenu->addAction(aEditOutCmd);
    outCmdMenu->addAction(aDeleteOutCmd);
    outCmdMenu->addAction(aDropPeriodCmd);
    m_pWidgetMenu->addMenu(outCmdMenu);

    m_pWidgetMenu->addSeparator();
    m_pWidgetMenu->addAction(aViewCmd);
    m_pWidgetMenu->addAction(aViewCmdDoc);
    m_pWidgetMenu->addAction(aViewOutCmdResult);
    m_pWidgetMenu->addSeparator();

    QMenu * menu = new QMenu(tr("View"), this);
    menu->addAction(toggleViewAction());
    menu->addSeparator();
    menu->addAction(floatingAction);
    menu->addSeparator();
    menu->addActions(areaActions->actions());

    m_pWidgetMenu->addMenu(menu);

    m_pWidgetMenu->addSeparator();
    m_pWidgetMenu->addAction(aSettingsCmd);
}

void KKSCmdJournal::init()
{
    int oldId = -1;
    if(!m_data.isEmpty()){
        KKSCommand cmd = m_cmdJournalLv->currentCommand();
        if(cmd.id() > 0)
            oldId = cmd.id();
    }
    
    m_cmdJournalLv->clear();

    if(m_data.isEmpty())
        return;

    int row = 0;
    KKSMap<int, KKSEIOData*>::const_iterator pd;
    for (pd = m_data.constBegin(); pd != m_data.constEnd(); pd++)
    {
        KKSEIOData * record = pd.value();
        if(!record)
            continue;

        KKSCommand cmd;
        // № п/п
        cmd.setId(record->fieldValue("id").toInt());
        //message_body
        cmd.setMessageBody(record->fieldValue("message_body"));
        //category_name
        cmd.setCategoryName(record->fieldValue("category_name"));
        //отправ
        cmd.setDlFrom(record->fieldValue("dl_from_name"));
        //получатель результата
        cmd.setDlTo(record->fieldValue("dl_to_name"));
        //ответственный исполнитель
        cmd.setDlExecutor(record->fieldValue("dl_executor_name") + " (" + record->fieldValue("executor_name") + ")");
        //planned_exec
        QDateTime dt = KKSValue::stringToDateTime(record->fieldValue("exec_datetime"));//planned_exec
        cmd.setExecDateTime(dt);
        //insert_datetime
        dt = KKSValue::stringToDateTime(record->fieldValue("insert_time"));
        cmd.setInsertTime(dt);
        //periodical
        cmd.setExecPeriod(record->fieldValue("exec_period"));
        //receive_datetime
        dt = KKSValue::stringToDateTime(record->fieldValue("receive_datetime"));
        cmd.setReceiveDateTime(dt);
        //accepted_datetime
        dt = KKSValue::stringToDateTime(record->fieldValue("accepted_datetime"));
        cmd.setAcceptedDateTime(dt);
        //real_exec
        dt = KKSValue::stringToDateTime(record->fieldValue("real_exec"));
        cmd.setRealExec(dt);

        cmd.setCategoryId(record->fieldValue("id_io_category").toInt());
        cmd.setIoObjectId(record->fieldValue("id_io_object_att").toInt());
        cmd.setCmdState((KKSCommand::CmdState)record->fieldValue("id_jr_state").toInt());
        cmd.setCmdStateName(record->fieldValue("jr_state_name"));

        int idDlExecutor = record->fieldValue("id_dl_executor").toInt();
        int idDlFrom = record->fieldValue("id_dl_from").toInt();
        int cmdTypeEx = record->fieldValue("cmd_type").toInt();
        
        //KKSCommand::CmdTypeEx typeEx;
        cmd.setCmdTypeEx((KKSCommand::CmdTypeEx)cmdTypeEx);

        KKSCommand::CmdType type;
        if(m_idDl == idDlExecutor)
            type = KKSCommand::ctIn;
        else if(m_idDl == idDlFrom)
            type = KKSCommand::ctOut;
        else if(cmdTypeEx == KKSCommand::jtIn)
            type = KKSCommand::ctAllIn;
        else if(cmdTypeEx == KKSCommand::cteControl)
            type = KKSCommand::ctOut;
        else
            type = KKSCommand::ctAllOut;

        cmd.setCmdType(type);

        bool b = false;
        if(record->fieldValue("is_archived") == QObject::tr("Yes"))
            b = true;
        cmd.setIsArchived(b);

        //read_datetime_e
        dt = KKSValue::stringToDateTime(record->fieldValue("read_datetime_e"));
        cmd.setReadDateTimeE(dt);

        //read_datetime_c
        dt = KKSValue::stringToDateTime(record->fieldValue("read_datetime_c"));
        cmd.setReadDateTimeC(dt);

        m_cmdJournalLv->addCommand(cmd);
        if(cmd.id() == oldId){
            QModelIndex index = m_cmdJournalLv->model()->index(row, 0);
            m_cmdJournalLv->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Rows);
        }

        row ++;
    }

    m_cmdJournalLv->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void KKSCmdJournal::on_aSettings_triggered()
{
    KKSCmdJSettings settings = KKSCmdJSettings(*m_settings);
    CmdJournalSettingsForm * f = new CmdJournalSettingsForm(settings, this);
    if(f->exec() == QDialog::Accepted){
        settings = f->settings();
    
        m_settings->bAllInput = settings.bAllInput;
        m_settings->bAllOutput = settings.bAllOutput;
        m_settings->bMyInput = settings.bMyInput;
        m_settings->bInputExecuted = settings.bInputExecuted;
        m_settings->bOutputExecuted = settings.bOutputExecuted;
        m_settings->bInputArchived = settings.bInputArchived;
        m_settings->bOutputArchived = settings.bOutputArchived;
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

void KKSCmdJournal::on_aExecute_triggered()
{
    
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canExecuteCmd(cmd);
    if(!ok)
        return;

    int idCategory = cmd.categoryId();
    int idJournal = cmd.id();

    if(idCategory <= 0 || idJournal <= 0){
        QMessageBox::critical(this, 
                              tr("Error"), 
                              tr("You should select one command!"), 
                              QMessageBox::Ok);
        return;
    }

    emit executeClicked(idJournal, idCategory);
}

void KKSCmdJournal::on_aExecuteEx_triggered()
{
    
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canExecuteCmd(cmd);
    if(!ok)
        return;

    int idCategory = cmd.categoryId();
    int idJournal = cmd.id();

    if(idCategory <= 0 || idJournal <= 0){
        QMessageBox::critical(this, 
                              tr("Error"), 
                              tr("You should select one command!"), 
                              QMessageBox::Ok);
        return;
    }

    emit executeExClicked(idJournal, idCategory);
}

int KKSCmdJournal::currentJournalRecord() const
{
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    int idJournal = cmd.id();

    return idJournal;
}

void KKSCmdJournal::setData(const KKSMap<int, KKSEIOData*> & data)
{
    m_data = data;
    init();
}


void KKSCmdJournal::contextMenuEvent(QContextMenuEvent *e)
{
    e->accept();
    
    updateMenuItems();

    m_pWidgetMenu->exec(e->globalPos());
}

void KKSCmdJournal::updateMenuItems()
{
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    
    aExecuteInCmd->setEnabled(canExecuteCmd(cmd));
    aSetExecuting->setEnabled(canSetAsExecuting(cmd));
    aEditOutCmd->setEnabled(canEditOutCmd(cmd));
    aDeleteOutCmd->setEnabled(canDeleteOutCmd(cmd));
    aViewOutCmdResult->setEnabled(canViewResultsCmd(cmd));
    aViewCmdDoc->setEnabled(canViewCmdDoc(cmd));
    aDropPeriodCmd->setEnabled(canDropPeriodCmd(cmd));
}

bool KKSCmdJournal::event(QEvent *e)
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

void KKSCmdJournal::place(Qt::DockWidgetArea area, bool p)
{
    if (!p) 
        return;

    QMainWindow *mainWindow = qobject_cast<QMainWindow *>(parentWidget());
    if(mainWindow)
        mainWindow->addDockWidget(area, this);
}

void KKSCmdJournal::changeFloating(bool floating)
{ 
    setFloating(floating); 
}


void KKSCmdJournal::placeLeft(bool p)
{ 
    place(Qt::LeftDockWidgetArea, p); 
}

void KKSCmdJournal::placeRight(bool p)
{ 
    place(Qt::RightDockWidgetArea, p); 
}

void KKSCmdJournal::placeTop(bool p)
{ 
    place(Qt::TopDockWidgetArea, p); 
}

void KKSCmdJournal::placeBottom(bool p)
{ 
    place(Qt::BottomDockWidgetArea, p); 
}

/**/
bool KKSCmdJournal::canSetAsExecuting(const KKSCommand & cmd)
{
    int id = cmd.id();
    if(id <= 0)
        return false;
    
    if(cmd.cmdType() != KKSCommand::ctIn)
        return false;

    if(cmd.cmdState() == KKSCommand::csExecuted || 
        cmd.cmdState() == KKSCommand::csExecuting ||
        cmd.cmdState() == KKSCommand::csSavedAsDraft)
    {
        return false;
    }
    return true;
}
bool KKSCmdJournal::canDeleteOutCmd(const KKSCommand & cmd)
{
    //пока запрещаем удалять распоряжения, в том числе просто выданные
    return false;
    
    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctOut &&
        cmd.cmdType() != KKSCommand::ctAllOut)
    {
        return false;
    }

    if(cmd.cmdState() != KKSCommand::csOuted || 
        cmd.cmdState() != KKSCommand::csPlanned)
    {
        return false;
    }

    return true;
}
bool KKSCmdJournal::canEditOutCmd(const KKSCommand & cmd)
{
    //пока запрещаем редактировать распоряжения
    return false;

    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctOut &&
        cmd.cmdType() != KKSCommand::ctAllOut)
    {
        return false;
    }

    if(cmd.cmdState() != KKSCommand::csOuted || 
        cmd.cmdState() != KKSCommand::csPlanned)
    {
        return false;
    }

    return true;
}
bool KKSCmdJournal::canDropPeriodCmd(const KKSCommand & cmd)
{
    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctOut &&
        cmd.cmdType() != KKSCommand::ctAllOut)
    {
        return false;
    }
    
    if(cmd.execPeriod().isEmpty())
        return false;

    return true;
}
bool KKSCmdJournal::canExecuteCmd(const KKSCommand & cmd)
{
    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdType() != KKSCommand::ctIn &&
        cmd.cmdType() != KKSCommand::ctAllIn)
    {
        return false;
    }

    if(cmd.cmdState() != KKSCommand::csExecuting){
        return false;
    }

    return true;
}
bool KKSCmdJournal::canViewResultsCmd(const KKSCommand & cmd)
{
    if(cmd.id() <= 0)
        return false;

    if(cmd.cmdState() == KKSCommand::csExecuted){
        return true;
    }

    if(cmd.cmdState() == KKSCommand::csSavedAsDraft && 
       cmd.cmdType() == KKSCommand::ctIn)//ctAllIn не подходит ибо только автор может просмотреть свой черновик
    {
        return true;
    }

    return false;
}

bool KKSCmdJournal::canViewCmdDoc(const KKSCommand & cmd)
{
    if(cmd.id() <= 0)
        return false;

    if(cmd.ioObjectId() > 0){
        return true;
    }

    return false;
}
/**/

void KKSCmdJournal::on_aSetAsExecuting_triggered()
{
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canSetAsExecuting(cmd);
    if(ok)    
        emit setAsExecuting(cmd.id());
}

void KKSCmdJournal::on_aDeleteOutCmd_triggered()
{
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canDeleteOutCmd(cmd);
    if(ok)
        emit setAsArchived(cmd.id(), true);
}

void KKSCmdJournal::on_aCreateOutCmd_triggered()
{
    emit createOutCmd();
}

void KKSCmdJournal::on_aEditOutCmd_triggered()
{
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canEditOutCmd(cmd);
    if(ok)
        emit editOutCmd(cmd.id());
}

void KKSCmdJournal::on_aDropPeriodCmd_triggered()
{
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    bool ok = canDropPeriodCmd(cmd);
    if(ok)
        emit dropPeriodCmd(cmd.id());
}

void KKSCmdJournal::on_aViewOutCmdResult_triggered()
{
    int id = currentJournalRecord();
    if(id > 0){
        KKSCommand cmd = m_cmdJournalLv->currentCommand();
        bool bShowExecButton = false;
        if(cmd.cmdState() == KKSCommand::csSavedAsDraft)
            bShowExecButton = true;

        emit viewResultCmd(id, bShowExecButton);
    }
}

void KKSCmdJournal::on_aViewCmdDoc_triggered()
{
    KKSCommand cmd = m_cmdJournalLv->currentCommand();
    int idObject = cmd.ioObjectId();
    //int id = currentJournalRecord();
    if(idObject > 0)
        emit viewCmdDoc(idObject);
}

void KKSCmdJournal::on_aViewCmd_triggered()
{
    int id = currentJournalRecord();
    if(id > 0)
        emit viewCmd(id);
}
