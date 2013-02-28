#include <kksdatabase.h>
#include <QtGlobal>

#include "kksresult.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

SyncQueueItemModel::SyncQueueItemModel(int iCountRow, 
                                       int iCountColumn, 
                                       KKSDatabase * adb, 
                                       QString acursorName,
                                       QObject * parent) : QAbstractTableModel(parent)
{
    countRow = iCountRow;
    countColumn = iCountColumn;

    db = adb;
    cursorName = acursorName;
    move = false;
}

//***** *****
QVariant SyncQueueItemModel::data(const QModelIndex & index, int role) const
{
    if(move)
        return QVariant();
    
    KKSResult * res = NULL;             //Результат запроса базы данных
    QString v = "";                     //Строка данных
    QDateTime dt = dt.currentDateTime();//Текущее время
    int pos = 0;                        //Позиция в базе данных
    int roleIndex = 0;                  //
  
	//Если индекс неверен или роль не имеет значения "выбранные данные отображаются как текст", вернуть ошибку
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
	//Если индекс неверен вернуть ошибку
    if(index.column() < 0 || index.column() > _TABLE_COLUMN_COUNT_)
        return QVariant();

    //Если столбец 1
    if(index.column() == 0)
    {
		//Позиция строки в базе данных +1 от индекса
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 0);
            delete res;
        }

        return v;  
    }
	//Если столбец 2
    if(index.column() == 1)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 1);
            delete res;
        }

        return v;
    }
	//Если столбец 3
    if(index.column() == 2)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 2);
            delete res;
        }

        return v;
    }
	//Если столбец 4
    if(index.column() == 3)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 3);
            delete res;
        }

        return v;
    }   
	//Если столбец 5
    if(index.column() == 4)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 4);
            delete res;
        }

		if(v=6)
		{
			setData();
		}

        return v;
    }    
	//Если столбец 6
    if(index.column() == 5)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 5);
            delete res;
        }

        return v;
    }    
	//Если столбец 7
    if(index.column() == 6)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 6);
            delete res;
        }

        return v;
    }    
	//Если столбец 8
    if(index.column() == 7)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 7);
            delete res;
        }

        return v;
    }    
	//Если столбец 9
    if(index.column() == 8)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 8);
            delete res;
        }

        return v;
    }    
	//Если столбец 10
    if(index.column() == 9)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 9);
            delete res;
        }

        return v;
    }    
	//Если столбец 11
    if(index.column() == 10)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 10);
            delete res;
        }

        return v;
    }    

    return QVariant();
}
//**********

int SyncQueueItemModel::columnCount ( const QModelIndex & parent ) const
{
    Q_UNUSED ( parent );
    return countColumn;
}

int SyncQueueItemModel::rowCount ( const QModelIndex & parent ) const
{
    //Q_UNUSED ( parent );
	if(parent.isValid())
		return 0;
	else
		return countRow;
}


//*****Установка заголовков столбцов*****
QVariant SyncQueueItemModel::headerData(int section, 
                                        Qt::Orientation orientation,
                                        int role) const
{
    /*if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Transport task");
            case 1:
                return tr("Date and time");
            case 2:
                return tr("Organization");
            case 3:
                return tr("Entity");
            case 4:
                return tr("Entity table");
            case 5:
                return tr("Entity type");
            case 6:
                return tr("Sync type");
            case 7:
                return tr("Sync result");
            case 8:
                return tr("Entity UID");
            case 9:
                return tr("Entity IO UID");
            case 10:
                return tr("ID Organization");
            case 11:
                return tr("UNIQUE_ID");
            default:
                return QVariant();
        }
    }*/

	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
        switch (section) 
		{
            case 0:
                return tr("Id integer");
            case 1:
                return tr("Id organization");
            case 2:
                return tr("Id entity integer");
            case 3:
                return tr("Entity table");
            case 4:
                return tr("Entity type");
            case 5:
                return tr("Sync type");
            case 6:
                return tr("Sync result");
            case 7:
                return tr("Entity UID");
            case 8:
                return tr("Entity IO UID");
            case 9:
                return tr("UNIQUE_ID");
            case 10:
                return tr("Last update");
            case 11:
                return tr(" ");
            default:
                return QVariant();			
		}
	}

    return QVariant();
}

//bool SyncQueueItemModel::setData ( const QModelIndex & index, const QVariant & value, int role)
//{
	
//}

//**********
/* Ошибка в запросе?
    sqlCursor = QString(" \
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
                        ");
*/
