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
    
    KKSResult * res = NULL;             //��������� ������� ���� ������
    QString v = "";                     //������ ������
    QDateTime dt = dt.currentDateTime();//������� �����
    int pos = 0;                        //������� � ���� ������
    int roleIndex = 0;                  //
  
	//���� ������ ������� ��� ���� �� ����� �������� "��������� ������ ������������ ��� �����", ������� ������
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
	//���� ������ ������� ������� ������
    if(index.column() < 0 || index.column() > _TABLE_COLUMN_COUNT_)
        return QVariant();

    //���� ������� 1
    if(index.column() == 0)
    {
		//������� ������ � ���� ������ +1 �� �������
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 0);
            delete res;
        }

        return v;  
    }
	//���� ������� 2
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
	//���� ������� 3
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
	//���� ������� 4
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
	//���� ������� 5
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
	//���� ������� 6
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
	//���� ������� 7
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
	//���� ������� 8
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
	//���� ������� 9
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
	//���� ������� 10
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
	//���� ������� 11
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


//*****��������� ���������� ��������*****
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
/* ������ � �������?
    sqlCursor = QString(" \
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
                        ");
*/
