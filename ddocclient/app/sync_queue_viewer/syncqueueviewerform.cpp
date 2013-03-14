#include <QMessageBox>
#include <QApplication>
#include <QStandardItem>

#include "kksdatabase.h"

#include "syncqueueviewerform.h"
#include "syncqueueview.h"
#include "syncqueueviewform.h"
#include "filtersform.h"

SyncQueueViewerForm :: SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent) :
    QDialog (parent)
{
	//****Создание элементов интерфейса и их группировка****
	qpb_filters = new QPushButton(tr("Filters"),this);//Кнопка вызова диалога фильтров
	qpb_delete  = new QPushButton(tr("Delete"),this); //Кнопка удаления
	qpb_restart = new QPushButton(tr("Restart"),this);//Кнопка перезапуска
	qpb_cancel  = new QPushButton(tr("Cancel"),this); //Кнопка отмены
	qpb_save    = new QPushButton(tr("Save"),this);   //Кнопка записи
	qpb_exit    = new QPushButton(tr("Exit"),this);   //Кнопка выхода
	qpb_view    = new QPushButton(tr("View"),this);   //Кнопка выполнения запроса и отображения данных

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

	//*****Инициализация указателя на базу данных*****
    db = adb;
	//**********

	//*****Виджеты предназначенные для вывода данных*****
	//
	//Создание виджета вывода результатов запроса
	//
	syncQueueTreeWnd = new SyncQueueView(this);
    syncQueueTreeWnd->setObjectName(QString::fromUtf8("syncQueueTreeWnd"));

	//
	//Размещение поля вывода запроса в главной форме
    //
	qvbl_main->addWidget(syncQueueTreeWnd);
	//**********
	
	//
	//Флаги открытия курсора устанавливаются в false
	//
    cursor_open = false;
    cursor_open_file = false;
    count_colomn_logfile = 8;

	//
	//Подключение элементов интерфейса
	//
	QObject::connect(qpb_exit,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(qpb_filters,SIGNAL(clicked()),this,SLOT(slot_filters_setup()));
	QObject::connect(qpb_view,SIGNAL(clicked()),this,SLOT(on_pbView_clicked()));

	QObject::connect(syncQueueTreeWnd,SIGNAL(signal_viewRows(int,int)),this,SLOT(slot_updateModelData(int,int)));
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
	//
	//Инициализация указателя на результат запроса значением NULL
	//
	KKSResult* res = NULL;

	//
	//Инициализация переменной cur с целью определения количества опрашиваемых таблиц
    //
	int cur = sqlCursor.indexOf(" from ");
	//
	//Если таблиц нет вернуть 0
    //
	if(cur == -1)
        return 0;
	//
	//Запрос по количеству записей в таблице from out_sync_queue
	//
	QString newSql = QString("select count(*) from out_sync_queue");//where 1=1
    res = db->execute(newSql);

	//
	//Возвращаемое значение
    //
	int i;
	//
	//Если количество столбцов не 0 то возвращаем его
    //
	if(res)
    {
        i = res->getCellAsInt(0, 0);
        countRow = i;

        delete res;
        return i;
    }
	//
	//Иначе возвращаем 0
    //
	else
    {
        countRow = 0;
        return 0;
    }
}



void SyncQueueViewerForm::on_pbView_clicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

	//
	//Проверка существования курсора, в случае если истина - закрытие
    //
	if(cursor_open)
    {
        db->close("sync_cursor");
        db->close("sync_cursor_file");
        db->close("init_cursor");
    }

    sqlCursor.clear();


    /*
                            select \
                                t.id, --ID транспортной задачи \
                                t.name, --(*) Транспортная задача \
                                t.local_address, -- локальный адрес транспорта \
                                ot.address,-- адрес целевой организации \
                                q.unique_id, --уникальный идентификатор записи очереди  \
                                q.last_update, -- (*) дата и время отправки \
                                q.id, -- идентификатор записи очереди \
                                q.id_organization, --идентификатор целевой организации \
                                o.name, -- (*) Целевая организация \
                                q.id_entity, -- (*) ИД пересылаемой записи \
                                q.entity_table, -- название таблицы в которой содержится пересылаемая запись \
                                io.name, --(*) название ИО \
                                q.entity_type, --(*) тип ИО (!!!!!) \
                                q.sync_type,  --(*) тип синхронизации (!!!!) \
                                q.sync_result,  --(*) результат синхронизации (!!!!!) \
                                q.entity_uid,  --уникальный идентификатор пересылаемой записи справочника \
                                q.entity_io_uid  -- уникальный идентификатор справочника \
                            from  \
                                out_sync_queue q \
                                inner join organization o on (o.id = q.id_organization) \
                                inner join organization_transport ot on (o.id = ot.id_organization) \
                                inner join transport t on (ot.id_transport = t.id) \
                                inner join io_objects io on (q.entity_table = io.table_name) \
                            where \
                                1=1 \
    
    //*/

	//***** *****
	//
	//Запрос к базе данных в формате QString
	//
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
								io.name, \
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

	//Выбрать все данные из таблицы и упорядочить по первому столбцу
	//sqlCursor = QString("select * from out_sync_queue where 1=1");
/*

    QString aa = filterF->getOrg();
    if(!aa.isEmpty()){
        sqlCursor += QString(" and q.id_organization in (%1) ").arg(aa);
    }

    aa = filterF->getObjectT();
    if(!aa.isEmpty()){
        sqlCursor += QString(" and q.entity_type in (%1) ").arg(aa);
    }

    aa = filterF->getRes();
    if(!aa.isEmpty()){
        sqlCursor += QString(" and q.sync_result in (%1) ").arg(aa);
    }

    if(filterF->useStartDate()){
        QString tVal = filterF->getStartDateTime().toString("dd.MM.yyyy hh:mm:ss");
        QString dtStart = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);

        sqlCursor += QString(" and q.last_update >= %1 ").arg(tVal);
    }

    if(filterF->useEndDate()){
        QString tVal = filterF->getEndDateTime().toString("dd.MM.yyyy hh:mm:ss");
        QString dtEnd = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);

        sqlCursor += QString(" and q.last_update <= %1 ").arg(tVal);
    }

*/
	//
	//Упорядочивание таблицы по первому столбцу
	//
    sqlCursor += QString(" order by 1");
	//**********

	//
	//Установка флага курсора в положение открыт
    //
	cursor_open = true;

	KKSResult * res = 0;

	//
	//Открытие базы данных
	//
	db->begin();
	
	//
	//Объявление курсора
	//
	res = db->declare("sync_cursor", sqlCursor);
    
	if(!res)
	{
		db->commit();
		QMessageBox::information(0,tr("Information"),tr("Can't declare cursor!"));
	}

	res = db->declare("sync_cursor_file", sqlCursor);
    
	if(!res)
	{
		db->commit();
		QMessageBox::information(0,tr("Information"),tr("Can't declare cursor!"));
	}
	
	res = db->declare("init_cursor", sqlCursor);

	if(!res)
	{
		db->commit();
		QMessageBox::information(0,tr("Information"),tr("Can't declare cursor!"));
	}

	//Задание количества столбцов
	countColumn = TableView::TABLE_COLUMN_COUNT_VIEW;
	//Получение количества строк в курсоре
    countRow = countInCursor();

	syncQueueTreeWnd->setRowCount(countRow);
	model = new SyncQueueItemModel(countRow,countColumn,this);
	
	//*****Настройка элемента отображения в соответствии с курсором*****
	syncQueueTreeWnd->setModel(model);

	syncQueueTreeWnd->slot_resizeEvent();

	model->setEmptyData(false);

	syncQueueTreeWnd->UpdateData();
	//**********

    QApplication::restoreOverrideCursor();
}

//Слот вызова диалога для установки фильтров
void SyncQueueViewerForm::slot_filters_setup()
{
	FiltersForm* filtersDialog = new FiltersForm(this);

	if(filtersDialog->exec() == QDialog::Accepted)
	{
		QMessageBox::information(0,"Information","Yes");
	}

	delete filtersDialog;
}

//
//Слот обновления данных модели
//
void SyncQueueViewerForm::slot_updateModelData(int input_topRow,int input_bottomRow)
{
	DBdata(input_topRow,input_bottomRow);
}
		
void SyncQueueViewerForm::DBdata(int input_topRow,int input_bottomRow)
{
	QVector<QString>* v_DBData = new QVector<QString>();

	int pos = input_topRow;
    KKSResult * res = NULL;             //Результат запроса базы данных
	QString dd;
	
	for(int i = 0 ; i < (input_bottomRow-input_topRow); i++)
	{
		pos = input_topRow+i;
		
		res = db->fetch("sync_cursor", 4, pos+1);

		if(!res)
		{
			db->commit();
			QMessageBox::information(0,tr("fetch break"),tr("Cursor can't return data!"));
			return;
		}

		for(int j = 0; j < TableView::TABLE_COLUMN_COUNT ; j++)
		{
			switch(j)
			{
				case 1:
					v_DBData->append(res->getCellAsString(0,j));break;
				case 5:
					v_DBData->append(res->getCellAsString(0,j));break;
				case 8:
					v_DBData->append(res->getCellAsString(0,j));break;
				case 9:
					v_DBData->append(res->getCellAsString(0,j));break;
				case 11:
					v_DBData->append(res->getCellAsString(0,j));break;
				case 12:
					v_DBData->append(res->getCellAsString(0,j));break;
				case 13:
					v_DBData->append(res->getCellAsString(0,j));break;
				case 14:
					v_DBData->append(res->getCellAsString(0,j));break;
			}
		}
	}

	model->setWindowIndex(input_topRow,input_bottomRow);	
	model->setDataVector(v_DBData);
}