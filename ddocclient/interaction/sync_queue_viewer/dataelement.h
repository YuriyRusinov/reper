#ifndef DATAELEMENT_H
#define DATAELEMENT_H

#include <QString>
#include <QDateTime>

class DataElement;

namespace sqv_data
{
    enum {TransportTask,DateAndTime,Organization,EntityUID,EntityName,EntityType,SyncType,SyncResult};
    enum {noMovement,newWindow,goDown,goUp,newScreen};

    struct window
    {
        int topRowIndex;
        int bottomRowIndex;
    };

    struct scrollWindow
    {
        window visibleWindow;

        int offset;
        int flag_offsetPosition;
    };

    struct viewWindowOptions
    {
        int totalRowCount;

        window visibleWindow;
    };

    struct modifyModelData
    {
        scrollWindow windowInf;
        QList<DataElement>* data;
    };

    const unsigned int TABLE_COLUMN_COUNT = 17;
    const unsigned int TABLE_COLUMN_COUNT_VIEW = 8;
    const unsigned int ROW_HEIGHT = 24;
}

class DataElement
{
public:
    DataElement();

    void setData(const QString& data,int rhs = sqv_data::TransportTask);
    QString getData(int rhs = sqv_data::TransportTask) const;

private:
    QString transportTask;
    QString dateAndtime;
    QString organization;
    QString entityUID;
    QString entityName;
    QString entityType;
    QString syncType;
    QString syncResult;
};

#endif // DATAELEMENT_H
