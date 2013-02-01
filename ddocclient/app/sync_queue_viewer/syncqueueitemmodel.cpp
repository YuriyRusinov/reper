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

QVariant SyncQueueItemModel::data(const QModelIndex & index, int role) const
{
    if(move)
        return QVariant();
    
    KKSResult * res = NULL;
    QString v = "";
    QDateTime dt = dt.currentDateTime();
    int pos = 0;
    int roleIndex = 0;

    if(role > Qt::UserRole)
    {
        switch(role)
        {
            case Qt::UserRole + 100:
                roleIndex = 0;
                break;
            case Qt::UserRole + 101:
                roleIndex = 1;
                break;
            case Qt::UserRole + 102:
                roleIndex = 2;
                break;
            case Qt::UserRole + 103:
                roleIndex = 3;
                break;
            case Qt::UserRole + 104:
                roleIndex = 4;
                break;
            case Qt::UserRole + 105:
                roleIndex = 5;
                break;
            case Qt::UserRole + 106:
                roleIndex = 6;
                break;
            case Qt::UserRole + 107:
                roleIndex = 7;
                break;
            case Qt::UserRole + 108:
                roleIndex = 8;
                break;
            case Qt::UserRole + 109:
                roleIndex = 9;
                break;
            case Qt::UserRole + 110:
                roleIndex = 10;
                break;
            case Qt::UserRole + 111:
                roleIndex = 11;
                break;
        }

        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, roleIndex);
        }

        if(res)
            delete res;

        return v;
    }
    
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    
    if(index.column() < 0 || index.column() > _TABLE_COLUMN_COUNT_)
        return QVariant();

    
    if(index.column() == 0)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 1);
        }

        if(res)
            delete res;

        return v;  
    }

    if(index.column() == 1)
    {
        pos = index.row() + 1;
        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            //время в формате 2007-01-15 13:33:32.403797
            dt = res->getCellAsDateTime(0, 5);
            v = dt.toString("hh:mm::ss dd-MM-yyyy");
            delete res;
        }

        return v;
    }
    

    /*
    pos = index.row() + 1;

    res = db->fetch("sync_cursor", 4, pos);

    if(res)
    {
        v = res->getCellAsString(0, index.column());
    }

    if(res)
        delete res;

    return v;
    */

    /*  потом может пригодиться, чтобы более правильно визуализировать данные. Например, дату и время и т.п.
    if(index.column() == 0)
    {
        pos = index.row() + 1;

        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 2);
        }

        if(res)
            delete res;

        return v;
    }

    if(index.column() == 1)
    {
        pos = index.row() + 1;
        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            v = res->getCellAsString(0, 4);
        }

        if(res)
            delete res;
        return v;
    }

    if(index.column() == 2)
    {
        pos = index.row() + 1;
        res = db->fetch("sync_cursor", 4, pos);

        if(res)
        {
            //время в формате 2007-01-15 13:33:32.403797
            dt = res->getCellAsDateTime(0, 7);
            v = dt.toString("hh:mm::ss dd-MM-yyyy");
            delete res;
        }

        return v;
    }
    */

    return QVariant();
}

int SyncQueueItemModel::columnCount ( const QModelIndex & parent ) const
{
    Q_UNUSED ( parent );
    return countColumn;
}

int SyncQueueItemModel::rowCount ( const QModelIndex & parent ) const
{
    Q_UNUSED ( parent );
    return countRow;
}

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
