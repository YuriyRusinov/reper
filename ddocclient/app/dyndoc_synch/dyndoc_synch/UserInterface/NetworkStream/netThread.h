#ifndef NETTHREAD_H
#define NETTHREAD_H

#include <QThread>

#include <httpwindow.h>

class HttpWindow;

class netThread : public QThread
{
    Q_OBJECT

public:
    explicit netThread(QObject *parent = 0);
    ~netThread();

    void run();

public slots:
    void slotSynchStart();
    void slotSynchStop();
    void slotThreadStop();

private:
    volatile bool stop;

    HttpWindow* httpWin;
};

#endif
