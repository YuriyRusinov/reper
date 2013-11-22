#ifndef DYNDOC_NETWORK_STREAM_H

#include <QObject>
#include <QMessageBox>

#include "Namespaces/netStruct.h"
#include "netThread.h"

#include <ddocinteractorwindow.h>
#include <ddocinteractorbase.h>
#include <timerform.h>

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

    void signalPingsSended(QMap<QString,JKKSPing> rhs);

public slots:
    void slotStartSyncronization();
    void slotStopSyncronization();

    void slotPoll();
    void slotImplementInitialSyncronization();

    void slotPingsSended(QMap<QString,JKKSPing> rhs);

private:
    networkStream(networkStream& adb);
    const networkStream& operator=(const networkStream& rhs);

    DDocInteractorWindow* httpWin;
    DDocInteractorBase* m_base;
};

#endif
