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
    modelData = 0;
	
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
	if(index.column() < 0 || index.column() > TableView::TABLE_COLUMN_COUNT_VIEW)
        return QVariant();

	if(index.row() >= topRowIndex && index.row() <= bottomRowIndex && role == Qt::BackgroundRole && !emptyData)
	{
		if(modelData->value(TableView::TABLE_COLUMN_COUNT_VIEW*(index.row()-topRowIndex)+index.column()) == "1")
		{
			return Qt::red;
		}
	}

	if(index.row() >= topRowIndex && index.row() <= bottomRowIndex && role == Qt::DisplayRole && !emptyData)
	{
		int k = TableView::TABLE_COLUMN_COUNT_VIEW*(index.row()-topRowIndex)+index.column();
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

//Установка размеров видимого окна
void SyncQueueItemModel::setWindowIndex(int input_topRowIndex,int input_bottomRowIndex)
{
	topRowIndex = input_topRowIndex; 
	bottomRowIndex = input_bottomRowIndex;
}

bool SyncQueueItemModel::setEmptyData(bool newFlag)
{
	bool oldEmptyData = emptyData;
	emptyData = newFlag;
	return oldEmptyData;
}

//
//Функция передачи данных в модель
//
void SyncQueueItemModel::setDataVector(QVector< QString >* i_modelData)
{
	if(modelData)
		delete modelData;
		
	modelData = i_modelData;
}
