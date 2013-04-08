#include "syncqueueitemmodel.h"


SyncQueueItemModel::SyncQueueItemModel(int iCountRow, 
                                       int iCountColumn, 
                                       QObject * parent) : QAbstractTableModel(parent)
{
	init(iCountRow,iCountColumn);

	modelData = new QVector<QString>();
}

//*****Функции для работы с параметрами строк и столбцов таблицы*****
//
//Возврат количества столбцов
//
int SyncQueueItemModel::columnCount ( const QModelIndex & parent ) const
{
    Q_UNUSED ( parent );
    return countColumn;
}
//
//Возврат количества строк потомков для узла
//
int SyncQueueItemModel::rowCount ( const QModelIndex & parent ) const
{
	if(parent.isValid())
		return 0;
	else
		return countRow;
}
//
//Возврат индекса верхней видимой строки
//
int SyncQueueItemModel::getTopViewRow() const
{
	return topRowIndex;
}
//
//Возврат индекса нижней видимой строки
//
int SyncQueueItemModel::getBottomViewRow() const
{
	return bottomRowIndex;
}
//
//Функция установки видимой области виджета отображения
//В случае если полученные индексы ошибочны возвращает false
//
bool SyncQueueItemModel::setWindowIndex(const int i_topRowIndex,const int i_bottomRowIndex)
{
	//*****Проверки на непротиворечивость индексов видимых строк*****
	if(i_topRowIndex > i_bottomRowIndex || i_topRowIndex < 0 || i_bottomRowIndex < 0  )
		return false;
	//**********

	//*****Проверки на общую непротиворечивость индексов*****
	if(i_bottomRowIndex >= countRow)
		return false;
	//**********

	topRowIndex = i_topRowIndex; 
	bottomRowIndex = i_bottomRowIndex;

	return true;
}
//
//Функция установки количества столбцов в таблице
//В случае если количество столбцов неверно возвращает false
//
bool SyncQueueItemModel::setColumn(const int i_totalColumn)
{
	if(i_totalColumn <= 0)
		return false;
	
	countColumn = i_totalColumn;

	return true;
}
//
//Функция установки количества строк в таблице
//В случае если количество столбцов неверно возвращает false
//
bool SyncQueueItemModel::setRow(const int i_totalRow)
{
	if(i_totalRow <= 0)
		return false;
	
	countRow = i_totalRow;

	return true;
}
//
//Добавление строки в позицию i_index
//Первая строка имеет индекс 0
//
int SyncQueueItemModel::insertDataRow(const int i_index,const QVector<QString>& i_dataRow)
{
	//*****Проверка на наличие данных в модели и их соответствие параметрам хранимых данных*****
	if(modelData->isEmpty() || i_dataRow.size() != countColumn)
		return 1;
	//**********

	//*****Проверка на правильное значение индекса строки*****
	if(i_index < 0 || i_index >= getViewRowsCount())
		return 2;
	//**********
	
	for(int i = 0; i < countColumn ; i++)
	{
		modelData->insert(i_index*countColumn,i_dataRow.value(countColumn - 1 - i));
	}

	return 0;
}
//
//Удаление строки из позиции i_index
//Первая строка имеет индекс 0
//
int SyncQueueItemModel::deleteDataRow(const int i_index)
{
	//*****Проверка на наличие данных в модели и их соответствие параметрам хранимых данных*****
	if(modelData->isEmpty() || i_index*countColumn+countColumn-1 >= modelData->size())
		return 1;
	//**********

	//*****Проверка на правильное значение индекса строки*****
	if(i_index < 0 || i_index > getViewRowsCount())
		return 2;
	//**********
	
	for(int i = 0; i < countColumn ; i++)
	{
		modelData->remove(i_index*countColumn);
	}

	return 0;
}
//
//Возврат отображаемого количества строк
//
int SyncQueueItemModel::getViewRowsCount() const
{
	return bottomRowIndex - topRowIndex + 1;
}
//**********

//*****Функция для получения и обработки данных*****
//
//Функция передачи данных в модель
//
int SyncQueueItemModel::setDataVector(const QVector<QString>& i_modelData)
{
	//
	//Если данные отсутствуют вернуть false
	//
	if(i_modelData.isEmpty())
		return 1;
	//
	//Если в модели уже есть данные, удалить их
	//
	if(!modelData->isEmpty())
	{
		modelData->clear();
	}
		
	for(int i = 0; i < i_modelData.size(); i++)
		modelData->append(i_modelData.value(i));

	emptyData = false;

	return 0;
}
//
//Переопределенный метод data()
//
QVariant SyncQueueItemModel::data(const QModelIndex & index, int role) const
{
	if(emptyData)
		return QVariant();	
	
	if(visualIndex(index.row()))
	{
		switch(role)
		{
			case Qt::SizeHintRole:
			{
				QSize res;
				res.setWidth(24);
				res.setHeight(24);
				return res;
			}
			case Qt::DecorationRole:
			{
				if(!index.column())	
					return dataDecoration(index.row());

				break;
			}
			case Qt::BackgroundRole:
			{
				return dataBackground(index.row());
			}
			case Qt::DisplayRole:
			{
				return dataDisplay(index.row(),index.column());
			}
			default:
			{
				return QVariant();
			}
		}
	}

    return QVariant();
}
//
//Функция установки флага пустой модели
//Предыдущее значение флага возвращается
//
void SyncQueueItemModel::setEmptyData(bool newFlag)
{
	emptyData = newFlag;
}
//
//Возврат значения флага пустой модели
//
bool SyncQueueItemModel::getEmptyData() const
{
	return emptyData;
}
//**********

//*****Установка заголовков таблицы*****
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

//*****Поскольку потомков у узлов нет функция возвращает всегда false*****
bool SyncQueueItemModel::hasChildren ( const QModelIndex & parent ) 
{
	return false;
}
//**********

//*****Вспомогательные функции*****
//
//Функция сброса модели в начальное состояние
//
void SyncQueueItemModel::clear()
{
	init();

	modelData->clear();
}
//**********

//
//Функция инициализации
//
inline void SyncQueueItemModel::init(int i_row,int i_column)
{
	countColumn = i_row;
	countRow = i_column;

	bottomRowIndex = -1;
	topRowIndex = -1;
	
	emptyData = true;
}
//
//Функция проверки попадания строки в видимое окно
//
inline int SyncQueueItemModel::visualIndex(int i_index) const
{
	if(i_index >= topRowIndex && i_index <= bottomRowIndex)
		return 1;

	return 0;
}
//
//Функция обработки Qt::DecorationRole
//
inline QVariant SyncQueueItemModel::dataDecoration(int i_index) const
{
	int k = countColumn*(i_index-topRowIndex)+countColumn-1;
	
	if(modelData->value(k) == "4")
	{
		return QVariant(QPixmap(":/ddoc/cancel.png").scaled(QSize(24,24)));
	}
	if(modelData->value(k) == "3")
	{
		return QVariant(QPixmap(":/ddoc/accept.png").scaled(QSize(24,24)));
	}
	if(modelData->value(k) == "2")
	{
		return QVariant(QPixmap(":/ddoc/category_copy.png").scaled(QSize(24,24)));
	}

	return QVariant();
}
//
//Функция обработки Qt::BackgroundRole
//
inline QVariant SyncQueueItemModel::dataBackground(int i_index) const
{
	int k = countColumn*(i_index-topRowIndex)+countColumn-1;
	
	if(modelData->value(k) == "4")
	{
		return Qt::red;
	}
	if(modelData->value(k) == "3")
	{
		return Qt::green;
	}
	if(modelData->value(k) == "2")
	{
		return Qt::yellow;
	}

	return QVariant();
}
//
//Функция обработки Qt::DisplayRole
//
inline QVariant SyncQueueItemModel::dataDisplay(int i_indexRow,int i_indexColumn) const
{
	int k = countColumn*(i_indexRow - topRowIndex) + i_indexColumn;

	QString res = modelData->value(k);

	switch(i_indexColumn)
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