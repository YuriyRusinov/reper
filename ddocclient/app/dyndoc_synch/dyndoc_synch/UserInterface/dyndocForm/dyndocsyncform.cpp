#include "UserInterface/dyndocForm/dyndocsyncform.h"
#include "ui_dyndoc_sync_form.h"

DyndocSyncForm::DyndocSyncForm(QWidget *parent) :
    QDialog(parent)
{
    ui = new Ui::dyndoc_sync_form;

    ui->setupUi(this);
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
    item->setData(dataTable.at(query_index).structInf.idNum,Qt::UserRole+1);
    item->setData(dataTable.at(query_index).structInf.id,Qt::UserRole+2);
    item->setData(dataTable.at(query_index).structInf.email_prefix,Qt::UserRole+3);

    QPixmap icon;
    icon.loadFromData(dataTable.at(query_index).icon.toUtf8());
    item->setData(icon.scaled(24,24),Qt::DecorationRole);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignVCenter);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    if(item->data(Qt::UserRole+2).toString() == "localorg-organization-1")
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

//*****
void DyndocSyncForm::slot_startSyncronizationClicked()
{
    emit signalStartSyncronization();
}

void DyndocSyncForm::slot_stopSyncronizationClicked()
{
    emit signalStopSyncronization();
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
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("param"));
    msgBox.exec();
}


