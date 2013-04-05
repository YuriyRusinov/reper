#include <kksdatabase.h>
#include <QtGlobal>
#include <QtDebug>
#include <QBrush>

#include "kksresult.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

SyncQueueItemModel::SyncQueueItemModel(int iCountRow, 
                                       int iCountColumn, 
                                       QObject * parent) : QAbstractTableModel(parent), modelData(0), countRow(iCountRow),
														   countColumn(iCountColumn), topRowIndex(-1), bottomRowIndex(-1), emptyData(true)
{
}

//*****������� ��� ������ � ����������� ����� � �������� �������*****
//
//������� ���������� ��������
//
int SyncQueueItemModel::columnCount ( const QModelIndex & parent ) const
{
    Q_UNUSED ( parent );
    return countColumn;
}
//
//������� ���������� ����� �������� ��� ����
//
int SyncQueueItemModel::rowCount ( const QModelIndex & parent ) const
{
	if(parent.isValid())
		return 0;
	else
		return countRow;
}
//
//������� ������� ������� ������� ������
//
int SyncQueueItemModel::getTopViewRow() const
{
	return topRowIndex;
}
//
//������� ������� ������ ������� ������
//
int SyncQueueItemModel::getBottomViewRow() const
{
	return bottomRowIndex;
}
//
//������� ��������� ������� ������� ������� �����������
//� ������ ���� ���������� ������� �������� ���������� false
//
bool SyncQueueItemModel::setWindowIndex(int i_topRowIndex,int i_bottomRowIndex)
{
	//*****�������� �� ������������������ �������� ������� �����*****
	if(i_topRowIndex > i_bottomRowIndex || i_topRowIndex < 0 || i_bottomRowIndex < 0  )
		return false;
	//**********

	//*****�������� �� ����� ������������������ ��������*****
	if(i_bottomRowIndex >= countRow)
		return false;
	//**********

	topRowIndex = i_topRowIndex; 
	bottomRowIndex = i_bottomRowIndex;

	return true;
}
//
//������� ��������� ���������� �������� � �������
//� ������ ���� ���������� �������� ������� ���������� false
//
bool SyncQueueItemModel::setColumn(int i_totalColumn)
{
	if(i_totalColumn <= 0)
		return false;
	
	countColumn = i_totalColumn;

	return true;
}
//
//������� ��������� ���������� ����� � �������
//� ������ ���� ���������� �������� ������� ���������� false
//
bool SyncQueueItemModel::setRow(int i_totalRow)
{
	if(i_totalRow <= 0)
		return false;
	
	countRow = i_totalRow;

	return true;
}
//
//���������� ������ � ������� i_index
//������ ������ ����� ������ 0
//
bool SyncQueueItemModel::insertDataRow(int i_index,const QVector<QString>* i_dataRow)
{
	//*****�������� �� ������� ������ � ������ � �� ������������ ���������� �������� ������*****
	if(modelData->isEmpty() || i_dataRow->size() != countColumn)
		return false;
	//**********

	//*****�������� �� ���������� �������� ������� ������*****
	if(i_index < 0 || i_index >= getViewRowsCount())
		return false;
	//**********
	
	for(int i = 0; i < countColumn ; i++)
	{
		modelData->insert(i_index*countColumn,i_dataRow->value(countColumn - 1 - i));
	}

	return true;
}
//
//�������� ������ �� ������� i_index
//������ ������ ����� ������ 0
//
bool SyncQueueItemModel::deleteDataRow(int i_index)
{
	//*****�������� �� ������� ������ � ������ � �� ������������ ���������� �������� ������*****
	if(modelData->isEmpty() || i_index*countColumn+countColumn-1 >= modelData->size())
		return false;
	//**********

	//*****�������� �� ���������� �������� ������� ������*****
	if(i_index < 0 || i_index > getViewRowsCount())
		return false;
	//**********
	
	for(int i = 0; i < countColumn ; i++)
	{
		modelData->remove(i_index*countColumn);
	}

	return true;
}
//
//������� ������������� ���������� �����
//
int SyncQueueItemModel::getViewRowsCount() const
{
	return bottomRowIndex - topRowIndex + 1;
}
//**********

//*****������� ��� ��������� � ��������� ������*****
//
//������� ��������� �� ������ ������
//
const QVector< QString >* const SyncQueueItemModel::getDataVector() const
{
	return modelData;
}
//
//������� �������� ������ � ������
//
bool SyncQueueItemModel::setDataVector(QVector< QString >* i_modelData)
{
	//
	//���� ������ ����������� ������� false
	//
	if(!i_modelData || i_modelData->isEmpty())
		return false;
	//
	//���� � ������ ��� ���� ������, ������� ��
	//
	if(modelData)
	{
		delete modelData;
		modelData = 0;
	}
		
	modelData = i_modelData;

	emptyData = false;
}
//
//���������������� ����� data()
//
QVariant SyncQueueItemModel::data(const QModelIndex & index, int role) const
{
	//���� ������ ������� ��� ���� �� ����� �������� "��������� ������ ������������ ��� �����", ������� ������
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::BackgroundRole && role != Qt::DecorationRole))
        return QVariant();
	//���� ������ ������� ������� ������
	if(index.column() < 0 || index.column() > countColumn)
        return QVariant();

	if(index.row() >= topRowIndex && index.row() <= bottomRowIndex && role == Qt::DecorationRole && !emptyData && !index.column())
	{
		if(modelData->value(countColumn*(index.row()-topRowIndex)+countColumn-1) == "4")
		{
            return QVariant(QPixmap(":/ddoc/cancel.png"));
		}
		if(modelData->value(countColumn*(index.row()-topRowIndex)+countColumn-1) == "3")
		{
            return QVariant(QPixmap(":/ddoc/accept.png"));;
		}
		if(modelData->value(countColumn*(index.row()-topRowIndex)+countColumn-1) == "2")
		{
            return QVariant(QPixmap(":/ddoc/category_copy.png"));;
		}
	}

	if(index.row() >= topRowIndex && index.row() <= bottomRowIndex && role == Qt::BackgroundRole && !emptyData)
	{
		if(modelData->value(countColumn*(index.row()-topRowIndex)+countColumn-1) == "4")
		{
			return Qt::red;
		}
		if(modelData->value(countColumn*(index.row()-topRowIndex)+countColumn-1) == "3")
		{
			return Qt::green;
		}
		if(modelData->value(countColumn*(index.row()-topRowIndex)+countColumn-1) == "2")
		{
			return Qt::yellow;
		}
	}

	if(index.row() >= topRowIndex && index.row() <= bottomRowIndex && role == Qt::DisplayRole && !emptyData)
	{
		int k = countColumn*(index.row()-topRowIndex)+index.column();

		QString res = modelData->value(k);

		switch(index.column())
		{
			case 5:
			{
				if(res == "1")
					res = tr("category(system)");
				if(res == "2")
					res = tr("IO(system)");
				if(res == "3")
					res = tr("IOR(directory record)");
				if(res == "4")
					res = tr("IOR(record additional table directory)");
				if(res == "5")
					res = tr("additional table directory");
				if(res == "6")
					res = tr("transfer request to the ping");
				if(res == "8")
					res = tr("transfer complete directory of organizations to the new object");
				if(res == "9")
					res = tr("transfer directory entries for all organizations involved in the exchange of information objects");
				if(res == "10")
					res = tr("public DL");

				break;
			}
			case 6:
			{
				if(res == "1")
					res = tr("create a new entity on the target");
				if(res == "2")
					res = tr("replace the entity for this on the target");
				if(res == "3")
					res = tr("delete entity on the target");

				break;
			}
			case 7:
			{
				if(res == "1")
					res = tr("expected timing");
				if(res == "2")
					res = tr("synchronized");
				if(res == "3")
					res = tr("synchronization is successful");
				if(res == "4")
					res = tr("there was an error");

				break;
			}
		}

		return res;
	}

    return QVariant();
}
//
//������� ��������� ����� ������ ������
//���������� �������� ����� ������������
//
void SyncQueueItemModel::setEmptyData(bool newFlag)
{
	emptyData = newFlag;
}
//
//������� �������� ����� ������ ������
//
bool SyncQueueItemModel::getEmptyData() const
{
	return emptyData;
}
//**********

//*****��������� ���������� �������*****
QVariant SyncQueueItemModel::headerData(int section, 
                                        Qt::Orientation orientation,
                                        int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0:
                return tr("Transport task");
            case 1:
                return tr("Date and time");
            case 2:
                return tr("Organization");
            case 3:
                return tr("Entity UID");
            case 4:
                return tr("Entity name");
            case 5:
                return tr("Entity type");
            case 6:
                return tr("Sync type");
            case 7:
                return tr("Sync result");
            default:
                return QVariant();
        }
    }

    return QVariant();
}
//**********

//*****��������� �������� � ����� ��� ������� ���������� ������ false*****
bool SyncQueueItemModel::hasChildren ( const QModelIndex & parent ) 
{
	return false;
}
//**********

//*****��������������� �������*****
//
//������� ������ ������ � ��������� ���������
//
void SyncQueueItemModel::clear()
{
	countColumn = 0;
	countRow = 0;

	bottomRowIndex = -1;
	topRowIndex = -1;

	if(modelData)
	{
		delete modelData;
		modelData = 0;
	}

	emptyData = true;
}
//**********