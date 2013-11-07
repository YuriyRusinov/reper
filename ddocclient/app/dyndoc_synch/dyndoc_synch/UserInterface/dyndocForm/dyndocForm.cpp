#include "UserInterface/dyndocForm/dyndocForm.h"
#include "ui_dyndoc_synch_form.h"

dyndoc_synch_form::dyndoc_synch_form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dyndoc_synch_form)
{
    ui->setupUi(this);
}

dyndoc_synch_form::~dyndoc_synch_form()
{   
    delete ui;
}

//*****
void dyndoc_synch_form::init()
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

void dyndoc_synch_form::initData()
{
    viewModel->setColumnCount(8);
}

void dyndoc_synch_form::initSlots() const
{
    connect(ui->qpb_exit,SIGNAL(clicked()),this,SLOT(close()));

    connect(ui->qpb_start_synch,SIGNAL(clicked()),this,SLOT(slot_startSyncronizationClicked()));
    connect(ui->qpb_stop_synch,SIGNAL(clicked()),this,SLOT(slot_stopSyncronizationClicked()));
    connect(ui->qpb_poll,SIGNAL(clicked()),this,SLOT(slot_pollClicked()));
    connect(ui->qpb_display_log,SIGNAL(clicked()),this,SLOT(slot_displayLogClicked()));
    connect(ui->qpb_impinit_synch,SIGNAL(clicked()),this,SLOT(slot_implementInitialSyncronizationClicked()));
    connect(ui->qpb_param,SIGNAL(clicked()),this,SLOT(slot_parametersClicked()));
}

void dyndoc_synch_form::initInterface()
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
void dyndoc_synch_form::setViewWidget(dyndocView *new_viewWidget)
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

void dyndoc_synch_form::newViewWidgetIsTrue(dyndocView* new_viewWidget)
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

void dyndoc_synch_form::newViewWidgetIsFalse()
{
    if(verificationExistingFrameLayout())
    {
        deleteOldViewWidget();
    }
}

bool dyndoc_synch_form::verificationExistingFrameLayout()
{
    QLayout* frameLayout = ui->frame->layout();

    if(frameLayout)
        return true;
    else
        return false;
}

void dyndoc_synch_form::deleteOldViewWidget()
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

void dyndoc_synch_form::createFrameLayout()
{
    QVBoxLayout* frameLayout = new QVBoxLayout;
    ui->frame->setLayout(frameLayout);
}

void dyndoc_synch_form::addViewWidgetToFrameLayout(dyndocView *new_viewWidget)
{
    viewWidget = new_viewWidget;
    viewWidget->setParent(ui->frame);

    QLayout* frameLayout = ui->frame->layout();

    frameLayout->addWidget(viewWidget);

    viewModel = (dyndocModel*) viewWidget->model();
}

//*****

void dyndoc_synch_form::setDbInf(dyndoc_mainStructs::dbInf dataBaseInf)
{
    db = dataBaseInf;
}

void dyndoc_synch_form::reciveData(const QList<dyndoc_mainStructs::hostModelInf>& orgsForModel)
{
    QStandardItem* rootItem = viewModel->invisibleRootItem();

    addChilds(*rootItem,QString(""),orgsForModel);
}

int dyndoc_synch_form::addChilds(QStandardItem& rhs,const QString& text,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
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

int dyndoc_synch_form::addColumns(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
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

int dyndoc_synch_form::addColumnName(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
{
    QStandardItem* item = new QStandardItem;

    item->setData(dataTable.at(query_index).structInf.name,Qt::DisplayRole);
    item->setData(dataTable.at(query_index).structInf.name,Qt::ToolTipRole);
    item->setData(dataTable.at(query_index).structInf.id,Qt::UserRole+1);
    item->setData(dataTable.at(query_index).structInf.email_prefix,Qt::UserRole+2);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setTextAlignment(Qt::AlignVCenter);
    item->setData(Qt::red,Qt::BackgroundColorRole);

    if(item->data(Qt::UserRole+1).toString() == "localorg-organization-1")
    {
         QFont boldItemFont = item->font();
         boldItemFont.setBold(true);
         item->setFont(boldItemFont);

         item->setData(QString("*") + item->data(Qt::DisplayRole).toString(),Qt::DisplayRole);
    }

    rhs.append(item);

    return 0;
}

int dyndoc_synch_form::addColumnProtocol(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
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

int dyndoc_synch_form::addColumnAdress(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
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

int dyndoc_synch_form::addColumnPort(QList<QStandardItem*>& rhs,int query_index,const QList<dyndoc_mainStructs::hostModelInf>& dataTable)
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

int dyndoc_synch_form::addColumnSynch(QList<QStandardItem*>& rhs)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Unchecked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(tr("false"),Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}

int dyndoc_synch_form::addColumnDb(QList<QStandardItem*>& rhs)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Unchecked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(tr("false"),Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}

int dyndoc_synch_form::addColumnInit(QList<QStandardItem*>& rhs)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Unchecked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(tr("false"),Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}

int dyndoc_synch_form::addColumnTrans(QList<QStandardItem*>& rhs)
{
    QStandardItem* item = new QStandardItem;

    item->setData(Qt::Unchecked,Qt::CheckStateRole);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(tr("false"),Qt::ToolTipRole);

    rhs.append(item);

    return 0;
}
//*****

//*****
void dyndoc_synch_form::slot_startSyncronizationClicked()
{
    emit signalStartSyncronization();
}

void dyndoc_synch_form::slot_stopSyncronizationClicked()
{
    emit signalStopSyncronization();
}

void dyndoc_synch_form::slot_pollClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("poll"));
    msgBox.exec();
}

void dyndoc_synch_form::slot_displayLogClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("display log"));
    msgBox.exec();
}

void dyndoc_synch_form::slot_implementInitialSyncronizationClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("initial synch"));
    msgBox.exec();
}

void dyndoc_synch_form::slot_parametersClicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText(tr("param"));
    msgBox.exec();
}


