#include <QProgressDialog>
#include <QMessageBox>
#include <QFile>
#include <QWidget>
#include <QApplication>

//#include "debug.h"
#include "kksdatabase.h"

#include "syncqueueviewerform.h"
#include "syncqueueview.h"
#include "syncqueueviewform.h"
//#include "logeditsettings.h"
#include "filtersform.h"

SyncQueueViewerForm :: SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent) :
    QDialog (parent)
{
	//****�������� ��������� ���������� � �� �����������****
	qpb_filters = new QPushButton(tr("Filters"),this);//������ ������ ������� ��������
	qpb_delete  = new QPushButton(tr("Delete"),this); //������ ��������
	qpb_restart = new QPushButton(tr("Restart"),this);//������ �����������
	qpb_cancel  = new QPushButton(tr("Cancel"),this); //������ ������
	qpb_save    = new QPushButton(tr("Save"),this);   //������ ������
	qpb_exit    = new QPushButton(tr("Exit"),this);   //������ ������
	qpb_view    = new QPushButton(tr("View"),this);   //������ ���������� ������� � ����������� ������

	QSplitter* qsh_fcB_splitter = new QSplitter(Qt::Horizontal,this);
	QSplitter* qsh_cBe_splitter = new QSplitter(Qt::Horizontal,this);

	QHBoxLayout* qhbl_topButtons = new QHBoxLayout();
	qhbl_topButtons->addWidget(qpb_filters);
	qhbl_topButtons->addWidget(qsh_fcB_splitter);
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
	//********

	//**** ****
    db = adb;

	//�������� ���� ������ �������
	syncQueueTreeWnd = new SyncQueueView(this);
    syncQueueTreeWnd->setObjectName(QString::fromUtf8("syncQueueTreeWnd"));

	//���������� ���� ������ ������� � ������� �����
    qvbl_main->addWidget(syncQueueTreeWnd);

	//��������� ���� ������
    syncQueueTreeWnd->SetDB(db);

    cursor_open = false;
    cursor_open_file = false;
    count_colomn_logfile = 8;

    progress = NULL;
	//********

	//����������� ��������� ����������
	QObject::connect(qpb_exit,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(qpb_filters,SIGNAL(clicked()),this,SLOT(slot_filters_setup()));
	QObject::connect(qpb_view,SIGNAL(clicked()),this,SLOT(on_pbView_clicked()));
}

SyncQueueViewerForm:: ~SyncQueueViewerForm(void)
{
    db->close("sync_cursor_file");
    db->close("sync_cursor");
    db->close("init_cursor");

    db->commit();
}

int SyncQueueViewerForm::countInCursor()
{
	//������������� ��������� �� ��������� �������
	KKSResult* res = NULL;

	//������������� ���������� cur  � ����� ����������� ���������� ������������ ������
    int cur = sqlCursor.indexOf(" from ");
	//���� ������ ��� ������� 0
    if(cur == -1)
        return 0;
	//������ �� ���������� ������� � ������� from out_sync_queue
	QString newSql = QString("select count(*) from out_sync_queue");//where 1=1
    res = db->execute(newSql);

	//������������ ��������
    int i;
	//���� ���������� �������� �� 0 �� ���������� ���
    if(res)
    {
        i = res->getCellAsInt(0, 0);
        countRow = i;

        delete res;
        return i;
    }
	//����� ���������� 0
    else
    {
        countRow = 0;
        return 0;
    }
}



void SyncQueueViewerForm::on_pbView_clicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

	//�������� ������������� �������, � ������ ���� ������ - ��������
    if(cursor_open)
    {
        db->close("sync_cursor");
        db->close("sync_cursor_file");
        db->close("init_cursor");
    }

    sqlCursor.clear();


    /*
                            select \
                                t.id, --ID ������������ ������ \
                                t.name, --(*) ������������ ������ \
                                t.local_address, -- ��������� ����� ���������� \
                                ot.address,-- ����� ������� ����������� \
                                q.unique_id, --���������� ������������� ������ �������  \
                                q.last_update, -- (*) ���� � ����� �������� \
                                q.id, -- ������������� ������ ������� \
                                q.id_organization, --������������� ������� ����������� \
                                o.name, -- (*) ������� ����������� \
                                q.id_entity, -- (*) �� ������������ ������ \
                                q.entity_table, -- �������� ������� � ������� ���������� ������������ ������ \
                                io.name, --(*) �������� �� \
                                q.entity_type, --(*) ��� �� (!!!!!) \
                                q.sync_type,  --(*) ��� ������������� (!!!!) \
                                q.sync_result,  --(*) ��������� ������������� (!!!!!) \
                                q.entity_uid,  --���������� ������������� ������������ ������ ����������� \
                                q.entity_io_uid  -- ���������� ������������� ����������� \
                            from  \
                                out_sync_queue q \
                                inner join organization o on (o.id = q.id_organization) \
                                inner join organization_transport ot on (o.id = ot.id_organization) \
                                inner join transport t on (ot.id_transport = t.id) \
                                inner join io_objects io on (q.entity_table = io.table_name) \
                            where \
                                1=1 \
    
    */

    sqlCursor = QString(" \
                            select \
                                t.id,  \
                                t.name,  \
                                t.local_address,  \
                                ot.address, \
                                q.unique_id,   \
                                q.last_update,  \
                                q.id,  \
                                q.id_organization,  \
                                o.name,  \
                                q.id_entity,  \
                                q.entity_table,  \
                                io.name,  \
                                q.entity_type,  \
                                q.sync_type,   \
                                q.sync_result,   \
                                q.entity_uid,   \
                                q.entity_io_uid   \
                            from  \
                                out_sync_queue q \
                                inner join organization o on (o.id = q.id_organization) \
                                inner join organization_transport ot on (o.id = ot.id_organization) \
                                inner join transport t on (ot.id_transport = t.id) \
                                inner join io_objects io on (q.entity_table = io.table_name) \
                            where \
                                1=1 \
                        ");
// */

	//������� ��� ������ �� ������� � ����������� �� ������� �������
	//sqlCursor = QString("select * from out_sync_queue where 1=1");
    sqlCursor += QString(" order by 1");
	//������ ������
    cursor_open = true;

    db->begin();
	//���������� �������
    KKSResult * res = db->declare("sync_cursor", sqlCursor);
    if(!res){
        db->rollback();
        return;
    }
    
    db->declare("sync_cursor_file", sqlCursor);
    db->declare("init_cursor", sqlCursor);
	//������� ���������� ��������
    countColumn = _TABLE_COLUMN_COUNT_;
	//��������� ���������� ����� � �������
    countRow = countInCursor();

	//*****��������� �������� ����������� � ������������ � ��������*****
    syncQueueTreeWnd->SetCountCursor(countRow);

    syncQueueTreeWnd->SetSQLCursor(sqlCursor);
    syncQueueTreeWnd->InitSyncQueueView();
	//**********

    QApplication::restoreOverrideCursor();

    db->commit();
}

//���� ������ ������� ��� ��������� ��������
void SyncQueueViewerForm::slot_filters_setup()
{
	FiltersForm* filtersDialog = new FiltersForm(this);

	if(filtersDialog->exec() == QDialog::Accepted)
	{
		QMessageBox::information(0,"Information","Yes");
	}

	delete filtersDialog;
}