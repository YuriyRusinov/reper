#include <kksdatabase.h>
#include <QtGlobal>
#include <QtDebug>
#include <QBrush>

#include "kksresult.h"
#include "syncqueueitemmodel.h"
#include "syncqueueviewerform.h"

SyncQueueItemModel::SyncQueueItemModel(int iCountRow, 
                                       int iCountColumn, 
                                       QObject * parent) : QAbstractTableModel(parent)
{
    modelData = new QVector< QString >();
	
	countRow = iCountRow;
    countColumn = iCountColumn;

	topRowIndex = 0;
	bottomRowIndex = 0;

	emptyData = true;
}

//***** *****
QVariant SyncQueueItemModel::data(const QModelIndex & index, int role) const
{
	//Если индекс неверен или роль не имеет значения "выбранные данные отображаются как текст", вернуть ошибку
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::BackgroundRole))
        return QVariant();
	//Если индекс неверен вернуть ошибку
    if(index.column() < 0 || index.column() > TableView::TABLE_COLUMN_COUNT)
        return QVariant();

	if(index.row() >= topRowIndex && index.row() <= bottomRowIndex && role == Qt::BackgroundRole && !emptyData)
	{
		if(modelData->value(TableView::TABLE_COLUMN_COUNT*(index.row()-topRowIndex)+index.column()) == "1")
		{
			//QBrush* r_brush = new QBrush();
			return Qt::red;
		}
	}

	if(index.row() >= topRowIndex && index.row() <= bottomRowIndex && role == Qt::DisplayRole && !emptyData)
	{
		int k = TableView::TABLE_COLUMN_COUNT*(index.row()-topRowIndex)+index.column();
		return modelData->value(k);
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
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
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
    }

    return QVariant();
}

//Установка размеров видимого окна
void SyncQueueItemModel::setWindowIndex(int input_topRowIndex,int input_bottomRowIndex)
{
	topRowIndex = input_topRowIndex; 
	bottomRowIndex = input_bottomRowIndex;

	//modelData->clear();
	//modelData->resize((bottomRowIndex-topRowIndex)*16);
}

bool SyncQueueItemModel::setEmptyData(bool newFlag)
{
	bool oldEmptyData = emptyData;
	emptyData = newFlag;
	return oldEmptyData;
}