#include "syncqueueitemmodel.h"


SyncQueueItemModel::SyncQueueItemModel(int iCountRow, 
                                       int iCountColumn, 
                                       QObject * parent) : QAbstractTableModel(parent)
{
	init(iCountRow,iCountColumn);

    l_modelData = new QList<DataElement>;
}

inline void SyncQueueItemModel::init(int i_row,int i_column)
{
    countColumn = i_column;
    window.totalRowCount = i_row;

    window.visibleWindow.topRowIndex = -1;
    window.visibleWindow.bottomRowIndex = -1;

    emptyData = true;
}

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
        return window.totalRowCount;
}

int SyncQueueItemModel::getTopViewRow() const
{
    return window.visibleWindow.topRowIndex;
}

int SyncQueueItemModel::getBottomViewRow() const
{
    return window.visibleWindow.bottomRowIndex;
}

bool SyncQueueItemModel::setColumn(const int i_totalColumn)
{
	if(i_totalColumn <= 0)
		return false;
	
	countColumn = i_totalColumn;

    return true;
}

bool SyncQueueItemModel::setRow(const int i_totalRow)
{
	if(i_totalRow <= 0)
		return false;
	
    window.totalRowCount = i_totalRow;

	return true;
}

void SyncQueueItemModel::slot_setData(sqv_data::modifyModelData rhs)
{
    if(rhs.data->isEmpty())
    {
        l_modelData->clear();
        delete rhs.data;

        return;
    }

    switch(rhs.windowInf.flag_offsetPosition)
    {
        case sqv_data::newWindow:
        case sqv_data::newScreen:
        {
            l_modelData->clear();
            copyDataList(*(rhs.data));

            window.visibleWindow = rhs.windowInf.visibleWindow;

            break;
        }
        case sqv_data::goUp:
        {
            addRowsForward(*(rhs.data));

            window.visibleWindow = rhs.windowInf.visibleWindow;

            break;
        }
        case sqv_data::goDown:
        {
            addRowsBackward(*(rhs.data));

            window.visibleWindow = rhs.windowInf.visibleWindow;

            break;
        }
        default:
        {
            l_modelData->clear();
        }
    }

    if(!l_modelData->isEmpty())
        emptyData = false;
    else
        emptyData = true;

    delete rhs.data;
}

void SyncQueueItemModel::copyDataList(const QList<DataElement>& i_modelData)
{
    DataElement buf;

    foreach(buf,i_modelData)
    {
        l_modelData->append(buf);
    }
}

void SyncQueueItemModel::addRowsForward(const QList<DataElement>& i_modelData)
{
    QListIterator<DataElement> iter(i_modelData);
    iter.toBack();

    while(iter.hasPrevious())
    {
        l_modelData->pop_back();
        l_modelData->push_front(iter.previous());
    }
}

void SyncQueueItemModel::addRowsBackward(const QList<DataElement>& i_modelData)
{
    DataElement buf;

    foreach(buf,i_modelData)
    {
        l_modelData->pop_front();
        l_modelData->push_back(buf);
    }
}

void SyncQueueItemModel::slot_newWindow(sqv_data::scrollWindow rhs)
{
    emit signal_scrollScreen(rhs);
}

QVariant SyncQueueItemModel::data(const QModelIndex & index, int role) const
{
	if(emptyData)
		return QVariant();	
	
    if(visualRow(index.row()))
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

void SyncQueueItemModel::setEmptyData(bool newFlag)
{
	emptyData = newFlag;
}

bool SyncQueueItemModel::getEmptyData() const
{
	return emptyData;
}

QVariant SyncQueueItemModel::headerData(int section, 
                                        Qt::Orientation orientation,
                                        int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section)
        {
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
            default:// онтейнер дл€ храени€ данных полученных по запросу
                return QVariant();
        }
    }

    return QVariant();
}

bool SyncQueueItemModel::hasChildren ( const QModelIndex & parent ) 
{
	return false;
}

bool SyncQueueItemModel::visualRow(int rhs) const
{
    if(window.visibleWindow.topRowIndex <= rhs || window.visibleWindow.bottomRowIndex >= rhs)
        return true;

    return false;
}

inline QVariant SyncQueueItemModel::dataDecoration(int i_index) const
{
    int k = (i_index - window.visibleWindow.topRowIndex);

    if(l_modelData->value(k).getData(sqv_data::SyncResult) == "4")
	{
		return QVariant(QPixmap(":/ddoc/cancel.png").scaled(QSize(24,24)));
	}
    if(l_modelData->value(k).getData(sqv_data::SyncResult) == "3")
	{
		return QVariant(QPixmap(":/ddoc/accept.png").scaled(QSize(24,24)));
	}
    if(l_modelData->value(k).getData(sqv_data::SyncResult) == "2")
	{
		return QVariant(QPixmap(":/ddoc/category_copy.png").scaled(QSize(24,24)));
	}

	return QVariant();
}

inline QVariant SyncQueueItemModel::dataBackground(int i_index) const
{
    int k = (i_index - window.visibleWindow.topRowIndex);

    if(l_modelData->value(k).getData(sqv_data::SyncResult) == "4")
	{
		return Qt::red;
	}
    if(l_modelData->value(k).getData(sqv_data::SyncResult) == "3")
	{
		return Qt::green;
	}
    if(l_modelData->value(k).getData(sqv_data::SyncResult) == "2")
	{
		return Qt::yellow;
	}

	return QVariant();
}

inline QVariant SyncQueueItemModel::dataDisplay(int i_indexRow,int i_indexColumn) const
{
    int k = (i_indexRow - window.visibleWindow.topRowIndex);

    QString res = returnData(k,i_indexColumn);

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

inline QString SyncQueueItemModel::returnData(int row_index, int rhs) const
{
    switch(rhs)
    {
        case 0:
        {
            return l_modelData->value(row_index).getData(sqv_data::TransportTask);
        }
        case 1:
        {
            return l_modelData->value(row_index).getData(sqv_data::DateAndTime);
        }
        case 2:
        {
            return l_modelData->value(row_index).getData(sqv_data::Organization);
        }
        case 3:
        {
            return l_modelData->value(row_index).getData(sqv_data::EntityUID);
        }
        case 4:
        {
            return l_modelData->value(row_index).getData(sqv_data::EntityName);
        }
        case 5:
        {
            return l_modelData->value(row_index).getData(sqv_data::EntityType);
        }
        case 6:
        {
            return l_modelData->value(row_index).getData(sqv_data::SyncType);
        }
        case 7:
        {
            return l_modelData->value(row_index).getData(sqv_data::SyncResult);
        }
    }

    return QString();
}
