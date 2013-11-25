#include "UserInterface/dyndocForm/dyndocsyncform.h"
#include "ui_dyndoc_sync_form.h"

DyndocSyncForm::DyndocSyncForm(QWidget *parent) :
    QDialog(parent)
{
    ui = new Ui::dyndoc_sync_form;

    ui->setupUi(this);

    localOrganizationId = 0;

    m_base = 0;
    httpWin = 0;
}

DyndocSyncForm::~DyndocSyncForm()
{   
    delete ui;
}

//*****
void DyndocSyncForm::init()
{
    try
    {
        initData();
        initSlots();
        initInterface();  
    }
    catch(...)
    {
        throw;
    }
}

void DyndocSyncForm::initData()
{
    viewModel->setColumnCount(8);

    ui->qpb_stop_synch->setDisabled(true);
}

void DyndocSyncForm::initSlots() const
{
    connect(ui->qpb_exit,SIGNAL(clicked()),this,SLOT(close()));

    connect(ui->qpb_start_synch,SIGNAL(clicked()),this,SLOT(slot_startSyncronizationClicked()));
    connect(ui->qpb_stop_synch,SIGNAL(clicked()),this,SLOT(slot_stopSyncronizationClicked()));
    connect(ui->qpb_poll,SIGNAL(clicked()),this,SLOT(slot_pollClicked()));
    connect(ui->qpb_display_log,SIGNAL(clicked()),this,SLOT(slot_displayLogClicked()));
    connect(ui->qpb_impinit_synch,SIGNAL(clicked()),this,SLOT(slot_implementInitialSyncronizationClicked()));
    connect(ui->qpb_param,SIGNAL(clicked()),this,SLOT(slot_parametersClicked()));
}

void DyndocSyncForm::initInterface()
{
    viewModel->setHeaderData(0,Qt::Horizontal,QString(tr("Organization")));
    viewModel->setHeaderData(0,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    viewWidget->setColumnWidth(0,250);

    viewModel->setHeaderData(1,Qt::Horizontal,QString(tr("Protocol")));
    viewModel->setHeaderData(1,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    viewWidget->setColumnWidth(1,100);

    viewModel->setHeaderData(2,Qt::Horizontal,QString(tr("Adress")));
    viewModel->setHeaderData(2,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    viewWidget->setColumnWidth(2,100);

    viewModel->setHeaderData(3,Qt::Horizontal,QString(tr("Port")));
    viewModel->setHeaderData(3,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    viewWidget->setColumnWidth(3,100);

    viewModel->setHeaderData(4,Qt::Horizontal,QString(tr("Syncronizer\nactivity")));
    viewModel->setHeaderData(4,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    viewWidget->setColumnWidth(4,100);

    viewModel->setHeaderData(5,Qt::Horizontal,QString(tr("Database\nactivity")));
    viewModel->setHeaderData(5,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    viewWidget->setColumnWidth(5,100);

    viewModel->setHeaderData(6,Qt::Horizontal,QString(tr("Initial\nsyncronization")));
    viewModel->setHeaderData(6,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    viewWidget->setColumnWidth(6,100);

    viewModel->setHeaderData(7,Qt::Horizontal,QString(tr("Transmition")));
    viewModel->setHeaderData(7,Qt::Horizontal,Qt::AlignCenter,Qt::TextAlignmentRole);
    viewWidget->setColumnWidth(7,100);

    viewWidget->setRootIsDecorated(false);

    dyndoc_delegate* sizeDelegate = new dyndoc_delegate(this);
    viewWidget->setItemDelegate(sizeDelegate);

    this->setWindowTitle(tr("Interaction monitor"));
}
//*****

//*****
void DyndocSyncForm::setViewWidget(dyndocView *new_viewWidget)
{
    if(new_viewWidget)
    {
        newViewWidgetIsTrue(new_viewWidget);
    }
    else
    {
        newViewWidgetIsFalse();
    }
}

void DyndocSyncForm::newViewWidgetIsTrue(dyndocView* new_viewWidget)
{
    if(verificationExistingFrameLayout())
    {
        deleteOldViewWidget();
    }
    else
    {
        createFrameLayout();
    }

    addViewWidgetToFrameLayout(new_viewWidget);
}

void DyndocSyncForm::newViewWidgetIsFalse()
{
    if(verificationExistingFrameLayout())
    {
        deleteOldViewWidget();
    }
}

bool DyndocSyncForm::verificationExistingFrameLayout()
{
    QLayout* frameLayout = ui->frame->layout();

    if(frameLayout)
        return true;
    else
        return false;
}

void DyndocSyncForm::deleteOldViewWidget()
{
    QLayout* frameLayout = ui->frame->layout();

    if(frameLayout->count())
    {
        frameLayout->removeWidget(viewWidget);

        viewWidget->setParent(0);
        viewWidget = 0;
        viewModel = 0;
    }
}

void DyndocSyncForm::createFrameLayout()
{
    QVBoxLayout* frameLayout = new QVBoxLayout;
    ui->frame->setLayout(frameLayout);
}

void DyndocSyncForm::addViewWidgetToFrameLayout(dyndocView *new_viewWidget)
{
    viewWidget = new_viewWidget;
    viewWidget->setParent(ui->frame);

    QLayout* frameLayout = ui->frame->layout();

    frameLayout->addWidget(viewWidget);

    viewModel = (dyndocModel*) viewWidget->model();
}

//*****

void DyndocSyncForm::setLocalId(int id)
{
    localOrganizationId = id;
}

void DyndocSyncForm::setDbInf(dyndoc_mainStructs::dbInf dataBaseInf)
{
    db = dataBaseInf;
}

void DyndocSyncForm::reciveData(const QList<dyndoc_mainStructs::hostModelInf>& orgsForModel)
{
    QStandardItem* rootItem = viewModel->invisibleRootItem();

    addChilds(*rootItem,QString(""),orgsForModel);
}

int DyndocSyncForm::addChilds(QStandardItem& rhs,const QString& text,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
{
    int col = 0;//Индекс потомков элемента

    for(int i=0;i<dataTable.count();i++)
    {
        QString str = dataTable.at(i).structInf.parentName;

        if(text == dataTable.at(i).structInf.parentName)
        {
            QList<QStandardItem*>* row_itemList = new QList<QStandardItem*>;

            addColumns(*row_itemList,i,dataTable);
            rhs.appendRow(*row_itemList);

            delete row_itemList;
            col++;
        }
    }

    if(rhs.rowCount())//Проверка условия наличия потомков
    {
        for(int i=0;i<rhs.rowCount();i++)
        {
            addChilds(*rhs.child(i),rhs.child(i)->text(),dataTable);
        }
    }

    return 0;//
}

int DyndocSyncForm::addColumns(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
{
    addColumnName(rhs,query_index,dataTable);
    addColumnProtocol(rhs,query_index,dataTable);
    addColumnAdress(rhs,query_index,dataTable);
    addColumnPort(rhs,query_index,dataTable);
    addColumnSynch(rhs);
    addColumnDb(rhs);
    addColumnInit(rhs);
    addColumnTrans(rhs);

    return 0;
}

int DyndocSyncForm::addColumnName(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
{
    QStandardItem* item = new QStandardItem;

    item->setData(dataTable.at(query_index).structInf.name,Qt::DisplayRole);
    item->setData(dataTable.at(query_index).structInf.name,Qt::ToolTipRole);
    item->setData(dataTable.at(query_index).structInf.id,Qt::UserRole+1);
    item->setData(dataTable.at(query_index).structInf.parentName,Qt::UserRole+2);
    item->setData(dataTable.at(query_index).structInf.email_prefix,Qt::UserRole+3);

    QPixmap icon;
    icon.loadFromData(dataTable.at(query_index).icon.toUtf8());
    item->setData(icon.scaled(24,24),Qt::DecorationRole);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignVCenter);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    if(item->data(Qt::UserRole+1) == localOrganizationId)
    {
         QFont boldItemFont = item->font();
         boldItemFont.setBold(true);
         item->setFont(boldItemFont);

         item->setData(QString("*") + item->data(Qt::DisplayRole).toString(),Qt::DisplayRole);
    }

    rhs.append(item);

    return 0;
}

int DyndocSyncForm::addColumnProtocol(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
{
    QStandardItem* item = new QStandardItem;

    item->setData(dataTable.at(query_index).structInf.protocol,Qt::DisplayRole);
    item->setData(dataTable.at(query_index).structInf.protocol,Qt::ToolTipRole);

    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    rhs.append(item);

    return 0;
}

int DyndocSyncForm::addColumnAdress(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
{
    QStandardItem* item = new QStandardItem;

    item->setData(dataTable.at(query_index).address.address,Qt::DisplayRole);
    item->setData(dataTable.at(query_index).address.address,Qt::ToolTipRole);

    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    rhs.append(item);

    return 0;
}

int DyndocSyncForm::addColumnPort(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
{
    QStandardItem* item = new QStandardItem;

    item->setData(dataTable.at(query_index).address.port,Qt::DisplayRole);
    item->setData(dataTable.at(query_index).address.port,Qt::ToolTipRole);

    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    rhs.append(item);

    return 0;
}

int DyndocSyncForm::addColumnSynch(QList<QStandardItem*>& rhs)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Unchecked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(tr("false"),Qt::ToolTipRole);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    rhs.append(item);

    return 0;
}

int DyndocSyncForm::addColumnDb(QList<QStandardItem*>& rhs)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Unchecked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(tr("false"),Qt::ToolTipRole);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    rhs.append(item);

    return 0;
}

int DyndocSyncForm::addColumnInit(QList<QStandardItem*>& rhs)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Unchecked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(tr("false"),Qt::ToolTipRole);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    rhs.append(item);

    return 0;
}

int DyndocSyncForm::addColumnTrans(QList<QStandardItem*>& rhs)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Unchecked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(tr("false"),Qt::ToolTipRole);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    rhs.append(item);

    return 0;
}
//*****

void DyndocSyncForm::setConnectionSettings()
{
    QSettings settings(QCoreApplication::applicationDirPath ()+"/http.ini", QSettings::IniFormat);

    if(settings.status())
        return;

    settings.beginGroup ("Database");
    settings.setValue ("host",optionsForm->getDBhost());//db.hostAddress);
    settings.setValue ("database",optionsForm->getDBname()); //db.dbName);
    settings.setValue ("user",optionsForm->getDBuser()); //db.userName);
    settings.setValue ("password",optionsForm->getDBpass()); //db.password);
    settings.setValue ("port",optionsForm->getDBport()); //db.port);
    settings.endGroup ();

    settings.beginGroup("Transport");
    settings.setValue ("transport",optionsForm->getTransport()); //UI->lEHttpTransport->text ());
    settings.endGroup();


    settings.beginGroup ("Http"); //понимаем как адрес шлюза для отправки корреспонденции (ТПС)
    settings.setValue ("host",optionsForm->getGatewayHost()); //UI->lEHttpHost->text ());
    settings.setValue ("port",optionsForm->getGatewayPort()); //UI->lEHttpPort->text ());


    settings.setValue ("server_host",optionsForm->getServerHost()); //serverIp.toString());
    settings.setValue ("server_port",optionsForm->getServerPort()); //UI->lEServerPort->text ());
    settings.endGroup ();
}

void DyndocSyncForm::loadSettings()
{
    QSettings settings (QCoreApplication::applicationDirPath ()+"/http.ini", QSettings::IniFormat);

    if(settings.status())
        return;

    settings.beginGroup ("Database");
    optionsForm->setDBhost(settings.value("host").toString());
    optionsForm->setDBname(settings.value ("database").toString());
    optionsForm->setDBuser(settings.value ("user").toString());
    optionsForm->setDBpass(settings.value ("password").toString());
    optionsForm->setDBport(settings.value ("port").toString());
    settings.endGroup ();

    settings.beginGroup("Transport");
    optionsForm->setTransport(settings.value ("transport").toString ());
    settings.endGroup ();

    settings.beginGroup ("Http");
    optionsForm->setGatewayHost(settings.value("host").toString());
    optionsForm->setGatewayPort(settings.value("port").toString());

    optionsForm->setServerHost(settings.value ("server_host").toString ());
    optionsForm->setServerPort(settings.value ("server_port").toString ());
    settings.endGroup ();
}

//*****
void DyndocSyncForm::slot_startSyncronizationClicked()
{
    ui->qpb_start_synch->setDisabled(true);
    ui->qpb_stop_synch->setEnabled(true);

    int intervalMs = 10000;
    int interval = 10;
    int units = 0;//seconds
    bool mode = false; //true = manual

    TimerForm * timerForm = new TimerForm ();
    if (!timerForm)
    {
        return;
    }

    timerForm->init(interval, units, mode, true);

    if (timerForm->exec () != QDialog::Accepted)
    {
        delete timerForm;
        return;
    }
    else
    {
        mode = timerForm->startManually();
        intervalMs = timerForm->getTimerMs();
    }
    delete timerForm;

    httpWin = new DDocInteractorWindow(this);

    m_base = new DDocInteractorBase(NULL);

    connect(this,SIGNAL(signalStopSyncronization()),httpWin,SLOT(closeApp()));
    connect(m_base,SIGNAL(pingsSended(QMap<QString,JKKSPing>)),this,SLOT(slot_pings(QMap<QString,JKKSPing> rhs)));

    httpWin->setWindowModality(Qt::NonModal);
    httpWin->setTimerParams(interval, units, mode);
    httpWin->setInteractorBase(m_base);

    int ok = m_base->start(mode, intervalMs);
    if(ok != 1)
        return;

    if(ok)
    {
        httpWin->show();
    }
}

void DyndocSyncForm::slot_stopSyncronizationClicked()
{
    ui->qpb_stop_synch->setDisabled(true);
    ui->qpb_start_synch->setEnabled(true);

    emit signalStopSyncronization();

    m_base = 0;
    httpWin = 0;
}

void DyndocSyncForm::slot_pollClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("poll"));
    msgBox.exec();
}

void DyndocSyncForm::slot_displayLogClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("display log"));
    msgBox.exec();
}

void DyndocSyncForm::slot_implementInitialSyncronizationClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("initial synch"));
    msgBox.exec();
}

void DyndocSyncForm::slot_parametersClicked()
{
    optionsForm = new OptionsDialog(this);
    loadSettings();

    connect(optionsForm,SIGNAL(okClic()),this,SLOT(slot_optionsAccepted()));
    connect(optionsForm,SIGNAL(cancelClic()),this,SLOT(slot_optionsRejected()));

    optionsForm->show();
}

void DyndocSyncForm::slot_optionsAccepted()
{
    setConnectionSettings();

    optionsForm->close();
    delete optionsForm;
}

void DyndocSyncForm::slot_optionsRejected()
{
    optionsForm->close();
    delete optionsForm;
}

void DyndocSyncForm::slot_pings(QMap<QString,JKKSPing> rhs)
{
    QMapIterator<QString,JKKSPing> i(rhs);

    while(i.hasNext())
    {
        int color = 1;

        i.next();

        if(i.value().state1() == 1 && i.value().state2() == 1 && i.value().state3() == 1 && i.value().state4() == 1)
        {
            color = 2;
        }
        if(i.value().created() == 0 || i.value().created() == 2)
        {
            color = 0;
        }

        setColor(i.key(),color);
    }
}

void DyndocSyncForm::setColor(QString key,int color)
{
    QStandardItem* root = viewModel->invisibleRootItem();
    bool findFlag;

    root = findItem(root,key,findFlag);

    if(color == 2)
    {
        root->setData(Qt::green,Qt::BackgroundColorRole);
    }
    if(color == 1)
    {
        root->setData(Qt::yellow,Qt::BackgroundColorRole);
    }
    else
    {
        root->setData(Qt::red,Qt::BackgroundColorRole);
    }
}

QStandardItem* DyndocSyncForm::findItem(QStandardItem* it,QString key,bool& findFlag)
{
    if(it->data(Qt::UserRole+2).toString() == key)
    {
        findFlag = true;
        return it;
    }

    for(int i=0; i<it->rowCount();i++)
    {
        QStandardItem* itemChild = it->child(i);

        itemChild = findItem(itemChild,key,findFlag);

        if(findFlag)
            return itemChild;
    }

    return 0;
}
