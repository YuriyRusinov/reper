#ifndef SQV_DATASTREAM_H
#define SQV_DATASTREAM_H

#include <QObject>
#include <QMessageBox>

#include "kksdatabase.h"
#include "dataelement.h"

class SQV_DataStream : public QObject
{
    Q_OBJECT
public:
    explicit SQV_DataStream(KKSDatabase * adb,QObject *parent = 0);
    ~SQV_DataStream();

    bool openCursor(const QString& filtersVariable);
    int countInCursor(const QString& filtersVariable);
    sqv_data::modifyModelData DBdata(sqv_data::scrollWindow& rhs);

    bool cursorIsOpen() const;

    QStringList orgNames();

public slots:
    void slot_getData(sqv_data::scrollWindow rhs);

signals:
    void signal_data(sqv_data::modifyModelData rhs);

private:
    KKSDatabase * db;

    bool flag_openCursor;

    int topViewRowIndex;
    int bottomViewRowIndex;

    QString sqlQuary;
    QString cursorName;

    void initQuary();

    void setQuaryFilters(const QString &filtersVariable);

    void refreshCursor();
    void closeCursor();

    void newWindowData(QList<DataElement>& rhs);
    void windowMoveUp(QList<DataElement>& rhs,int offset);
    void windowMoveDown(QList<DataElement>& rhs,int offset);
};

#endif // SQV_DATASTREAM_H
