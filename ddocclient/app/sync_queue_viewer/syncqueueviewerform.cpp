#include "syncqueueviewerform.h"

//*****Создание и уничтожение экземпляра класса*****
SyncQueueViewerForm :: SyncQueueViewerForm(KKSDatabase * adb, QWidget * parent) :
    QDialog (parent), model(0), flag_clicked(false),t_dataRow(-1),b_dataRow(-1),flag_error(false)
{	
	//****Создание элементов интерфейса и их группировка****
	qpb_filters = new QPushButton(tr("Filters"),this);//Кнопка вызова диалога фильтров
	qpb_delete  = new QPushButton(tr("Delete"),this); //Кнопка удаления
	qpb_restart = new QPushButton(tr("Restart"),this);//Кнопка перезапуска
	qpb_cancel  = new QPushButton(tr("Cancel"),this); //Кнопка отмены
	qpb_save    = new QPushButton(tr("Save"),this);   //Кнопка записи
	qpb_exit    = new QPushButton(tr("Exit"),this);   //Кнопка выхода
	qpb_view    = new QPushButton(tr("View"),this);   //Кнопка выполнения запроса и отображения данных

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

	setWindowTitle(tr("Data base view"));

	model = new SyncQueueItemModel(countRow,countColumn,this);

	//
	//Размещение поля вывода запроса в главной форме
    //
	qvbl_main->addWidget(syncQueueTreeWnd);
	//**********
	
	//
	//Флаги открытия курсора устанавливаются в false
	//
    cursor_open = false;

	filtersDialog = new FiltersForm(this);
	filtersDialog->setVisible(false);
	
	//
	//Подключение элементов интерфейса
	//
	QObject::connect(qpb_exit,SIGNAL(clicked()),this,SLOT(close()));
	QObject::connect(qpb_filters,SIGNAL(clicked()),this,SLOT(slot_filters_setup()));
	QObject::connect(qpb_view,SIGNAL(clicked()),this,SLOT(slot_viewClicked()));

	QObject::connect(filtersDialog,SIGNAL(accepted()),this,SLOT(slot_accepted()));
	QObject::connect(filtersDialog,SIGNAL(rejected()),this,SLOT(slot_rejected()));

	QObject::connect(syncQueueTreeWnd,SIGNAL(signal_viewRows(int,int)),this,SLOT(slot_updateModelData(int,int)));
}

SyncQueueViewerForm::~SyncQueueViewerForm(void)
{
    db->close("sync_cursor_file");
    db->close("sync_cursor");
    db->close("init_cursor");

    db->commit();

	delete filtersDialog;
}
//**********

//
//Обработчик события изменения размера
//
void SyncQueueViewerForm::resizeEvent(QResizeEvent * pe)
{
	refreshData();
}
//
//Слот вызова диалога для установки фильтров
//
void SyncQueueViewerForm::slot_filters_setup()
{
	filtersDialog->setVisible(true);
}
//
//Слот обращения к базе данных
//
void SyncQueueViewerForm::slot_viewClicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

	syncQueueTreeWnd->clear();
	model->clear();

	countColumn = -1;
	countRow    = -1;

	t_dataRow = -1;
	b_dataRow = -1;

	if(!openCursor())
	{
		return;
	}

	//Задание количества столбцов
	countColumn = TableView::TABLE_COLUMN_COUNT_VIEW;
	//Получение количества строк в курсоре
    countRow = countInCursor();

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

	if(!syncQueueTreeWnd->setRowCount(countRow))
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

	//*****Настройка элемента отображения в соответствии с курсором*****
	syncQueueTreeWnd->setModel(model);

	syncQueueTreeWnd->slot_viewRows();

	model->setEmptyData(false);
	
	syncQueueTreeWnd->updateData();
	//**********

    QApplication::restoreOverrideCursor();
}
//
//Слот обновления данных модели
//
void SyncQueueViewerForm::slot_updateModelData(int i_topRow,int i_bottomRow)
{
	if(i_topRow > i_bottomRow || i_topRow < 0 || i_bottomRow <0)
		return;
	
	DBdata(i_topRow,i_bottomRow);
}

//*****Закрытые функции*****
//
//Возвращение числа колонок в курсоре
//
int SyncQueueViewerForm::countInCursor()
{
	//
	//Если курсор закрыт вернуть -1
	//
	if(!cursor_open)
		return -1;

	//
	//Инициализация указателя на результат запроса значением 0
	//
	KKSResult* res = 0;

	//
	//Инициализация переменной cur с целью определения количества опрашиваемых таблиц
    //
	int cur = sqlCursorTF.indexOf(" from ");
	//
	//Если таблиц нет вернуть 0
    //
	if(cur == -1)
        return 0;
	//
	//Запрос по количеству записей в таблице from out_sync_queue
	//
	QString newSql = QString("select count(*) from out_sync_queue q where 1=1 ");// from out_sync_queue");//where 1=1
    newSql += sqlCursorFilters;
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
		db->commit();
		countRow = 0;
        return 0;
    }
}
//
//Функция открытия курсора
//
bool SyncQueueViewerForm::openCursor()
{
	//
	//Проверка существования курсора, в случае если истина - закрытие
    //
	if(cursor_open)
    {       
		closeCursor();
    }

	//
	//Установка флага открытия курсора в false
	//
	cursor_open = false;

    sqlCursorColumns.clear();
	sqlCursorTF.clear();
	sqlCursorFilters.clear();

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
    
    */

	//
	//Запрос к базе данных в формате QString
	//
	sqlCursorColumns = QString(" \
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
						");
	sqlCursorTF = QString(" \
                            from  \
                                out_sync_queue q \
								inner join io_objects io on (q.entity_table = io.table_name) \
                                left join organization o on (o.id = q.id_organization) \
                                left join organization_transport ot on (o.id = ot.id_organization) \
                                left join transport t on (ot.id_transport = t.id) \
                            where \
                                1=1 \
                        ");

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

	QString sqlCursor;

	sqlCursor = sqlCursorColumns + sqlCursorTF + sqlCursorFilters;
	//
	//Упорядочивание таблицы по первому столбцу
	//
    sqlCursor += QString(" order by 1");

	//
	//Указатель на результат запроса
	//
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
		return false;
	}

	//
	//Установка флага курсора в положение открыт
    //
	cursor_open = true;

	return true;
}
//
//Функция получения данных из базы данных
//
int SyncQueueViewerForm::DBdata(int i_topRow,int i_bottomRow)
{
	QVector<QString>* v_DBData = 0;
	int pos = -1;
	KKSResult* res = 0;
	int col = -1;

	if(t_dataRow == -1 && b_dataRow == -1)
	{
		t_dataRow = i_topRow;
		b_dataRow = i_bottomRow;

		if(syncQueueTreeWnd->model())
		{
			v_DBData = new QVector<QString>;
			
			for(int i = 0; i < (i_bottomRow - i_topRow); i++)
			{			
				pos = t_dataRow+i;

				res = db->fetch("sync_cursor", 4, pos+1);
		
				if(!res)
				{
					db->commit();
					QMessageBox::information(0,tr("fetch break"),tr("Cursor can't return data!"));
					return -1;
				}

				for(int j = 0; j < TableView::TABLE_COLUMN_COUNT ; j++)
				{
					switch(j)
					{
						case 1:
							v_DBData->append(res->getCellAsString(0,j));break;
						case 5:
						{
							QDateTime qdt_buffer =	res->getCellAsDateTime(0,j);
														
							v_DBData->append(qdt_buffer.toString("hh:mm:ss dd.MM.yyyy"));
							break;
						}
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
		}

		model->setDataVector(*v_DBData);
		model->setWindowIndex(i_topRow,i_bottomRow);

		delete v_DBData;

		return 0;
	}
	//
	//Случай полного смещения видимого окна
	//
	if( syncQueueTreeWnd->model() && ((i_topRow - t_dataRow) > (b_dataRow - t_dataRow) || (t_dataRow - i_topRow) > (b_dataRow - t_dataRow)))
	{
		t_dataRow = i_topRow;
		b_dataRow = i_bottomRow;

		v_DBData = new QVector<QString>;

		model->setWindowIndex(i_topRow,i_bottomRow);
			
		for(int i = 0; i < (i_bottomRow - i_topRow); i++)
		{			
			pos = t_dataRow+i;

			res = db->fetch("sync_cursor", 4, pos+1);
		
			if(!res)
			{
				db->commit();
				QMessageBox::information(0,tr("fetch break"),tr("Cursor can't return data!"));
				return -1;
			}

			for(int j = 0; j < TableView::TABLE_COLUMN_COUNT ; j++)
			{
				switch(j)
				{
					case 1:
						v_DBData->append(res->getCellAsString(0,j));break;
					case 5:
					{
						QDateTime qdt_buffer = res->getCellAsDateTime(0,j);
														
						v_DBData->append(qdt_buffer.toString("hh:mm:ss dd.MM.yyyy"));
						break;	
					}
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

		model->setDataVector(*v_DBData);
		syncQueueTreeWnd->updateData();

		delete v_DBData;	

		return 0;
	}
	//
	//Случай частичного смещения видимого окна
	//
	if(syncQueueTreeWnd->model() && (i_topRow - t_dataRow) < 0)
	{
		col = t_dataRow - i_topRow;

		if((b_dataRow - col) > t_dataRow)
		{
			for(int i = 0; i < col ; i++)
			{
				model->deleteDataRow((b_dataRow - t_dataRow) - col);
			}

			for(int i = 0; i < col ; i++)
			{
				v_DBData = new QVector<QString>;
				
				pos = t_dataRow -  i - 1;

				res = db->fetch("sync_cursor", 4, pos+1);
		
				if(!res)
				{
					db->commit();
					QMessageBox::information(0,tr("fetch break"),tr("Cursor can't return data!"));
					return -1;
				}

				for(int j = 0; j < TableView::TABLE_COLUMN_COUNT ; j++)
				{
					switch(j)
					{
						case 1:
							v_DBData->append(res->getCellAsString(0,j));break;
						case 5:
						{
							QDateTime qdt_buffer = res->getCellAsDateTime(0,j);
														
							v_DBData->append(qdt_buffer.toString("hh:mm:ss dd.MM.yyyy"));
							break;	
						}
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

				model->insertDataRow(0,*v_DBData);
				delete v_DBData;
			}
		}		
	}
	else
	{
		col = i_topRow - t_dataRow;

		if((t_dataRow + col) < b_dataRow)
		{
			for(int i = 0; i < col ; i++)
			{
				model->deleteDataRow(0);
			}

			for(int i = 0; i < col ; i++)
			{
				v_DBData = new QVector<QString>;
				
				pos = b_dataRow +  i;

				res = db->fetch("sync_cursor", 4, pos+1);
		
				if(!res)
				{
					db->commit();
					QMessageBox::information(0,tr("fetch break"),tr("Cursor can't return data!"));
					return -1;
				}

				for(int j = 0; j < TableView::TABLE_COLUMN_COUNT ; j++)
				{
					switch(j)
					{
						case 1:
							v_DBData->append(res->getCellAsString(0,j));break;
						case 5:
						{
							QDateTime qdt_buffer = res->getCellAsDateTime(0,j);
														
							v_DBData->append(qdt_buffer.toString("hh:mm:ss dd.MM.yyyy"));
							break;	
						}
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

				model->insertDataRow(b_dataRow - (t_dataRow + col) + i,*v_DBData);
				delete v_DBData;
			}
		}		
	}

	model->setWindowIndex(i_topRow,i_bottomRow);

	t_dataRow = i_topRow;
	b_dataRow = i_bottomRow;

	syncQueueTreeWnd->updateData();

	return 0;
}
//
//Установка значения полосы прокрутки
//
void SyncQueueViewerForm::setScrollValue(int i_value)
{
	syncQueueTreeWnd->setScrollValue(i_value);
}
//
//Функция закрытия курсора
//
void SyncQueueViewerForm::closeCursor()
{
	db->close("sync_cursor");
}
//
//Функция инициализации виджета
//
void SyncQueueViewerForm::initWidget()
{

}
//**********

/*void SyncQueueViewerForm::leaveEvent(QEvent * i_event)
{
	if (!flag_clicked) 
	{
        flag_clicked = true;
	}

	QDialog::leaveEvent(i_event);
}

void SyncQueueViewerForm::enterEvent (QEvent * i_event)
{
	if(flag_clicked)
	{
		flag_clicked = false;
	}

	QDialog::enterEvent(i_event);
}*/

void SyncQueueViewerForm::refreshData()
{
	if(!cursor_open)
		return;
	if(flag_clicked)
		return;

	syncQueueTreeWnd->clear();
	model->clear();
		
	syncQueueTreeWnd->setRowCount(countRow);

	model->setColumn(countColumn);
	model->setRow(countRow);
	model->setEmptyData(true);

	t_dataRow = -1;
	b_dataRow = -1;

	//*****Настройка элемента отображения в соответствии с курсором*****
	syncQueueTreeWnd->setModel(model);

	syncQueueTreeWnd->slot_viewRows();

	model->setEmptyData(false);
	
	syncQueueTreeWnd->updateData();
	//**********
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