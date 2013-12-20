#include "dataelement.h"

DataElement::DataElement()
{

}

void DataElement::setData(const QString& data,int rhs)
{
    switch(rhs)
    {
        case sqv_data::TransportTask:
        {
            transportTask = data;
            break;
        }
        case sqv_data::DateAndTime:
        {
            dateAndtime = data;
            break;
        }
        case sqv_data::Organization:
        {
            organization = data;
            break;
        }
        case sqv_data::EntityUID:
        {
            entityUID = data;
            break;
        }
        case sqv_data::EntityName:
        {
            entityName = data;
            break;
        }
        case sqv_data::EntityType:
        {
            entityType = data;
            break;
        }
        case sqv_data::SyncType:
        {
            syncType = data;
            break;
        }
        case sqv_data::SyncResult:
        {
            syncResult = data;
            break;
        }
        default:
            return;
    }
}

QString DataElement::getData(int rhs) const
{
    switch(rhs)
    {
        case sqv_data::TransportTask:
        {
            return transportTask;
            break;
        }
        case sqv_data::DateAndTime:
        {
            return dateAndtime;
            break;
        }
        case sqv_data::Organization:
        {
            return organization;
            break;
        }
        case sqv_data::EntityUID:
        {
            return entityUID;
            break;
        }
        case sqv_data::EntityName:
        {
            return entityName;
            break;
        }
        case sqv_data::EntityType:
        {
            return entityType;
            break;
        }
        case sqv_data::SyncType:
        {
            return syncType;
            break;
        }
        case sqv_data::SyncResult:
        {
            return syncResult;
            break;
        }
        default:
            return QString();
    }
}
