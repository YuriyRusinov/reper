#ifndef DYNDOC_DATA_STREAM_H
#define DYNDOC_DATA_STREAM_H

#include <QObject>
#include <QMessageBox>

#include "UserInterface/DataElements/viewElement.h"
#include "Namespaces/netStruct.h"
#include "kksdatabase.h"

class dataStream : public QObject
{
    Q_OBJECT

public:
    explicit dataStream(KKSDatabase* adb,QObject* parent = 0);
    virtual ~dataStream();

    dyndoc_mainStructs::dbInf getDbInf() const;
    const QList<dyndoc_mainStructs::hostModelInf>& getData();

signals:
    void signalDataBaseInf(dyndoc_mainStructs::dbInf* rhs);

private:
    bool flagDBisOpen;

    QString sqlQuery;

    KKSDatabase* db;
    KKSResult* res;

    QList<dyndoc_mainStructs::hostModelInf> organizations;

    dataStream(dataStream& adb);
    const dataStream& operator=(const dataStream& rhs);

    inline void initQuery();
    void openDB();
    void closeDB();

    void executeQuery();
    void createOrganizationsListFromRes();
};

#endif
