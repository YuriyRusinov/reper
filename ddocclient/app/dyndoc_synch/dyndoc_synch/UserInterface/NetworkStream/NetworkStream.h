#ifndef DYNDOC_NETWORK_STREAM_H

#include <QObject>
#include <QMessageBox>

#include "Namespaces/netStruct.h"
#include "netThread.h"

class networkStream : public QObject
{
    Q_OBJECT

public:
    networkStream(QObject * parent = 0);
    virtual ~networkStream();

signals:
    void signalSynchStart();
    void signalSynchStop();
    void signalThreadStop();

public slots:
    void slotStartSyncronization();
    void slotStopSyncronization();

    void slotPoll();
    void slotImplementInitialSyncronization();

private:
    networkStream(networkStream& adb);
    const networkStream& operator=(const networkStream& rhs);

    netThread* net;
};

#endif
