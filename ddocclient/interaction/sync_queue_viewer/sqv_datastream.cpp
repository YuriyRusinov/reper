#include "sqv_datastream.h"
#include <QDebug>

SQV_DataStream::SQV_DataStream(KKSDatabase *adb, QObject *parent) :
    QObject(parent)
{
    db = adb;
    flag_openCursor = false;

    cursorName = "sync_cursor";

    initQuary();
}

SQV_DataStream::~SQV_DataStream()
{
    db->close(cursorName);
    db->commit();
}

bool SQV_DataStream::openCursor(const QString& filtersVariable)
{
    refreshCursor();
    initQuary();
    setQuaryFilters(filtersVariable);

    KKSResult * res = 0;

    db->begin();
    res = db->declare(cursorName,sqlQuary);

    if(!res)
    {
        db->commit();
        return false;
    }

    flag_openCursor = true;

    return true;
}

int SQV_DataStream::countInCursor(const QString& filtersVariable)
{
    if(!flag_openCursor)
        return -1;

    KKSResult* res = 0;

    int cur = sqlQuary.indexOf(" from ");
    if(cur == -1)
        return 0;

    QString newSql = QString("select count(*) from out_sync_queue q where 1=1 ");
    newSql += filtersVariable;
    res = db->execute(newSql);

    int i;
    if(res)
    {
        i = res->getCellAsInt(0, 0);

        delete res;
        return i;
    }
    else
    {
        db->commit();
        return 0;
    }
}

sqv_data::modifyModelData SQV_DataStream::DBdata(sqv_data::scrollWindow &rhs)
{
    QList<DataElement>* dataForTable = new QList<DataElement>;

    switch(rhs.flag_offsetPosition)
    {
        case sqv_data::newWindow:
        case sqv_data::newScreen:
        {
            topViewRowIndex = rhs.visibleWindow.topRowIndex;
            bottomViewRowIndex = rhs.visibleWindow.bottomRowIndex;

            newWindowData(*dataForTable);
            break;
        }
        case sqv_data::goDown:
        {
            int offset = rhs.offset;

            topViewRowIndex = rhs.visibleWindow.topRowIndex;
            bottomViewRowIndex = rhs.visibleWindow.bottomRowIndex;

            windowMoveDown(*dataForTable,offset);
            break;
        }
        case sqv_data::goUp:
        {
            int offset = rhs.offset;

            topViewRowIndex = rhs.visibleWindow.topRowIndex;
            bottomViewRowIndex = rhs.visibleWindow.bottomRowIndex;

            windowMoveUp(*dataForTable,offset);
            break;
        }
    }

    sqv_data::modifyModelData data;
    data.data = dataForTable;
    data.windowInf = rhs;

    return data;
}

void SQV_DataStream::newWindowData(QList<DataElement> &rhs)
{
    KKSResult* res = 0;

    for(int i = 0; i < (bottomViewRowIndex - topViewRowIndex); i++)
        rhs.append(DataElement());

    int pos = topViewRowIndex;

    QList<DataElement>::Iterator iter = rhs.begin();

    while(iter != rhs.end())
    {      
        res = db->fetch(cursorName,4,pos+1);

        if(!res)
        {
            db->commit();
            QMessageBox::information(0,tr("fetch break"),tr("Cursor can't return data!"));
            rhs.clear();
        }

        for(unsigned int j = 0; j < sqv_data::TABLE_COLUMN_COUNT ; j++)
        {
            switch(j)
            {
                case 1:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::TransportTask);
                    break;
                }
                case 5:
                {
                    QDateTime qdt_buffer =	res->getCellAsDateTime(0,j);

                    iter->setData(qdt_buffer.toString("hh:mm:ss dd.MM.yyyy"),sqv_data::DateAndTime);
                    break;
                }
                case 8:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::Organization);
                    break;
                }
                case 9:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityUID);
                    break;
                }
                case 11:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityName);
                    break;
                }
                case 12:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityType);
                    break;
                }
                case 13:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::SyncType);
                    break;
                }
                case 14:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::SyncResult);
                    break;
                }
            }
        }

        pos++;
        ++iter;
    }

    delete res;
}

void SQV_DataStream::windowMoveUp(QList<DataElement>& rhs, int offset)
{
    DataElement buf;
    KKSResult* res = 0;

    for(int i = 0; i < offset; i++)
        rhs.append(DataElement());

    int pos = topViewRowIndex;

    QList<DataElement>::Iterator iter = rhs.end();

    while(iter != rhs.begin())
    {
        res = db->fetch(cursorName,4,pos+1);

        if(!res)
        {
            db->commit();
            QMessageBox::information(0,tr("fetch break"),tr("Cursor can't return data!"));
            rhs.clear();
        }

        for(unsigned int j = 0; j < sqv_data::TABLE_COLUMN_COUNT ; j++)
        {
            switch(j)
            {
                case 1:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::TransportTask);
                    break;
                }
                case 5:
                {
                    QDateTime qdt_buffer =	res->getCellAsDateTime(0,j);

                    iter->setData(qdt_buffer.toString("hh:mm:ss dd.MM.yyyy"),sqv_data::DateAndTime);
                    break;
                }
                case 8:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::Organization);
                    break;
                }
                case 9:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityUID);
                    break;
                }
                case 11:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityName);
                    break;
                }
                case 12:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityType);
                    break;
                }
                case 13:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::SyncType);
                    break;
                }
                case 14:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::SyncResult);
                    break;
                }
            }
        }

        pos--;
        --iter;
    }

    delete res;
}

void SQV_DataStream::windowMoveDown(QList<DataElement>& rhs,int offset)
{
    DataElement buf;
    KKSResult* res = 0;

    for(int i = 0; i < offset; i++)
        rhs.append(DataElement());

    int pos = bottomViewRowIndex;

    QList<DataElement>::Iterator iter = rhs.begin();

    while(iter != rhs.end())
    {
        res = db->fetch(cursorName,4,pos+1);

        if(!res)
        {
            db->commit();
            QMessageBox::information(0,tr("fetch break"),tr("Cursor can't return data!"));
            rhs.clear();
        }

        for(unsigned int j = 0; j < sqv_data::TABLE_COLUMN_COUNT ; j++)
        {
            switch(j)
            {
                case 1:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::TransportTask);
                    break;
                }
                case 5:
                {
                    QDateTime qdt_buffer =	res->getCellAsDateTime(0,j);

                    iter->setData(qdt_buffer.toString("hh:mm:ss dd.MM.yyyy"),sqv_data::DateAndTime);
                    break;
                }
                case 8:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::Organization);
                    break;
                }
                case 9:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityUID);
                    break;
                }
                case 11:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityName);
                    break;
                }
                case 12:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::EntityType);
                    break;
                }
                case 13:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::SyncType);
                    break;
                }
                case 14:
                {
                    iter->setData(res->getCellAsString(0,j),sqv_data::SyncResult);
                    break;
                }
            }
        }

        pos++;
        ++iter;
    }

    delete res;
}

void SQV_DataStream::initQuary()
{
    sqlQuary = QString(" \
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
                    inner join io_objects io on (q.entity_table = io.table_name) \
                    left join organization o on (o.id = q.id_organization) \
                    left join organization_transport ot on (o.id = ot.id_organization) \
                    left join transport t on (ot.id_transport = t.id) \
                    where \
                        1=1 \
                ");
}

QStringList SQV_DataStream::orgNames()
{
    QString quary = QString("select organization.\"name\" from organization");

    KKSResult* res = 0;
    res = db->execute(quary);

    QStringList orgNameList;

    for(int i = 0; i < res->getRowCount(); i++)
    {
        orgNameList << res->getCellAsString(i,0);
    }

    return orgNameList;
}

void SQV_DataStream::setQuaryFilters(const QString& filtersVariable)
{
    sqlQuary += filtersVariable;
    sqlQuary += QString(" order by 1");
}

void SQV_DataStream::refreshCursor()
{
    if(flag_openCursor)
    {
        closeCursor();
        flag_openCursor = false;
    }
}

void SQV_DataStream::closeCursor()
{
    db->close(cursorName);
}

bool SQV_DataStream::cursorIsOpen() const
{
    return flag_openCursor;
}

void SQV_DataStream::slot_getData(sqv_data::scrollWindow rhs)
{
    emit signal_data(DBdata(rhs));
}
