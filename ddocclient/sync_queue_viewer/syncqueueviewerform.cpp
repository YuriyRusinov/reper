#include "syncqueueviewerform.h"

SyncQueueViewerForm :: SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent) :
    QDialog(parent),model(0),flag_clicked(false),t_dataRow(-1),b_dataRow(-1)
{	
    createInteface();

    dataPoint = new SQV_DataStream(adb,this);

    createConnections();
}

inline void SyncQueueViewerForm::createInteface()
{
    setWindowTitle(tr("Data base view"));

    qpb_filters = new QPushButton(tr("Filters"),this);
    qpb_delete  = new QPushButton(tr("Delete"),this);
    qpb_restart = new QPushButton(tr("Restart"),this);
    qpb_cancel  = new QPushButton(tr("Cancel"),this);
    qpb_save    = new QPushButton(tr("Save"),this);
    qpb_exit    = new QPushButton(tr("Exit"),this);
    qpb_view    = new QPushButton(tr("View"),this);

    qpb_filters->setIcon(QIcon(":/ddoc/show_search_queries.png"));
    qpb_delete->setIcon(QIcon(":/ddoc/delete.png"));
    qpb_restart->setIcon(QIcon(":/ddoc/reload.png"));
    qpb_cancel->setIcon(QIcon(":/ddoc/cancel.png"));
    qpb_save->setIcon(QIcon(":/ddoc/folder_edit.png"));
    qpb_exit->setIcon(QIcon(":/ddoc/io_close.png"));
    qpb_view->setIcon(QIcon(":/ddoc/view_all.png"));

    QSplitter* qsh_cBe_splitter = new QSplitter(Qt::Horizontal,this);

    QHBoxLayout* qhbl_topButtons = new QHBoxLayout();
    qhbl_topButtons->addWidget(qpb_filters);
    qhbl_topButtons->addWidget(qpb_view);
    qhbl_topButtons->addWidget(qpb_delete);
    qhbl_topButtons->addWidget(qpb_restart);
    qhbl_topButtons->addWidget(qpb_cancel);
    qhbl_topButtons->addWidget(qpb_save);
    qhbl_topButtons->addWidget(qsh_cBe_splitter);
    qhbl_topButtons->addWidget(qpb_exit);

    QVBoxLayout* qvbl_main = new QVBoxLayout;
    qvbl_main->addLayout(qhbl_topButtons);
    setLayout(qvbl_main);

    model = new SyncQueueItemModel(countRow,countColumn,this);

    syncQueueTreeWnd = new SyncQueueView(this);
    syncQueueTreeWnd->setObjectName(QString::fromUtf8("syncQueueTreeWnd"));

    qvbl_main->addWidget(syncQueueTreeWnd);

    filtersDialog = new FiltersForm(this);
    filtersDialog->setVisible(false);
}

inline void SyncQueueViewerForm::createConnections()
{
    connect(qpb_filters,SIGNAL(clicked()),this,SLOT(slot_filters_setup()));
    connect(qpb_view,SIGNAL(clicked()),this,SLOT(slot_viewClicked()));
    connect(qpb_exit,SIGNAL(clicked()),this,SLOT(close()));

    connect(filtersDialog,SIGNAL(accepted()),this,SLOT(slot_accepted()));
    connect(filtersDialog,SIGNAL(rejected()),this,SLOT(slot_rejected()));

    connect(syncQueueTreeWnd,SIGNAL(signal_viewRows(int,int)),this,SLOT(slot_updateModelData(int,int)));

    connect(this,SIGNAL(signal_visionData()),syncQueueTreeWnd,SLOT(slot_viewRows()));
    connect(syncQueueTreeWnd,SIGNAL(signal_scrollScreen(sqv_data::scrollWindow)),model,SLOT(slot_newWindow(sqv_data::scrollWindow)));
    connect(model,SIGNAL(signal_scrollScreen(sqv_data::scrollWindow)),dataPoint,SLOT(slot_getData(sqv_data::scrollWindow)));
    connect(dataPoint,SIGNAL(signal_data(sqv_data::modifyModelData)),model,SLOT(slot_setData(sqv_data::modifyModelData)));
}

SyncQueueViewerForm::~SyncQueueViewerForm(void)
{
	delete filtersDialog;
    delete dataPoint;
}

void SyncQueueViewerForm::resizeEvent(QResizeEvent * pe)
{   
    if(flag_clicked)
        emit signal_visionData();
}

void SyncQueueViewerForm::slot_filters_setup()
{
	filtersDialog->setVisible(true);
}

void SyncQueueViewerForm::slot_accepted()
{
    org_list.clear();
    obj_list.clear();
    res_list.clear();

    dateFrom.clear();
    dateTo.clear();

    org_list = filtersDialog->getOrganization();
    obj_list = filtersDialog->getObjectType();
    res_list = filtersDialog->getResult();

    dateFrom = filtersDialog->getDateFrom();
    dateTo   = filtersDialog->getDateTo();

    sqlCursorFilters.clear();
}

void SyncQueueViewerForm::slot_rejected()
{

}

void SyncQueueViewerForm::slot_viewClicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    flag_clicked = true;

	countColumn = -1;
	countRow    = -1;

	t_dataRow = -1;
	b_dataRow = -1;

    createFilters();

    if(!dataPoint->openCursor(sqlCursorFilters))
    {
        return;
    }

    countColumn = sqv_data::TABLE_COLUMN_COUNT_VIEW;
    countRow = dataPoint->countInCursor(sqlCursorFilters);

	if(countRow == -1)
	{
		QMessageBox::information(0,tr("countRow"),tr("countRow = -1!"));
		QApplication::restoreOverrideCursor();
		return;
	}
	if(!countRow)
	{
		QMessageBox::information(0,tr("Data error!"),tr("No data in cursor!"));
		QApplication::restoreOverrideCursor();
		return;
	}

    if(!syncQueueTreeWnd->setTotalRowCount(countRow))
	{
		QMessageBox::information(0,tr("syncQueueTreeWnd->setRowCount(countRow)"),tr("!countRow"));
		return;
	}
	if(!model->setColumn(countColumn))
	{	
		QMessageBox::information(0,tr("!model->setColumn(countColumn)"),tr("!countColumn"));
		return;
	}
	if(!model->setRow(countRow))
	{
        QMessageBox::information(0,tr("!model->setRow(countRow)"),tr("!countRow"));
		return;
	}

	syncQueueTreeWnd->setModel(model);

    emit signal_visionData();

    syncQueueTreeWnd->updateData();

    QApplication::restoreOverrideCursor();
}

inline void SyncQueueViewerForm::createFilters()
{
    sqlCursorFilters.clear();

    if(!obj_list.isEmpty() && obj_list.size() != 10)
    {
        sqlCursorFilters = sqlCursorFilters + QString(" and q.entity_type in ( ");

        for(int i=0; i < obj_list.size() ; i++)
        {
            sqlCursorFilters = sqlCursorFilters+ obj_list.value(i);

            if(i != obj_list.size() - 1)
                sqlCursorFilters += QString(", ");
        }

        sqlCursorFilters += QString(" )");
    }

    if(!res_list.isEmpty() && res_list.size() != 4)
    {
        sqlCursorFilters = sqlCursorFilters + QString(" and q.sync_result in ( ");

        for(int i=0; i < res_list.size() ; i++)
        {
            sqlCursorFilters = sqlCursorFilters+ res_list.value(i);

            if(i != res_list.size() - 1)
                sqlCursorFilters += QString(", ");
        }

        sqlCursorFilters += QString(" )");
    }

    if(!dateFrom.isEmpty())
    {
        if(!dateFrom.startsWith("to_timestamp"))
        {
            dateFrom = QString("to_timestamp('") + dateFrom + QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp");
            sqlCursorFilters = sqlCursorFilters + QString(" and q.last_update >= ") + dateFrom + QString(" ");
        }
    }

    if(!dateTo.isEmpty())
    {
        if(!dateTo.startsWith("to_timestamp"))
        {
            dateTo = QString("to_timestamp('") + dateTo + QString("', 'DD.MM.YYYY HH24:MI:SS')::timestamp");
            sqlCursorFilters = sqlCursorFilters + QString(" and q.last_update <= ") + dateTo + QString(" ");
        }
    }
}
