#include "UserInterface/NetworkStream/netThread.h"

netThread::netThread(QObject* parent):
    QThread(parent)
{
    httpWin = 0;
    stop = false;

    //moveToThread(this);
}

netThread::~netThread()
{

}

void netThread::run()
{
    //while(true)
    //{
        //if(stop && !httpWin)
            //break;
    //}
    exec();
}

void netThread::slotSynchStart()
{
    //httpWin = new HttpWindow;
    //httpWin->show();
    //httpWin->exec();
}

void netThread::slotSynchStop()
{
    //httpWin->close();
    //delete httpWin;
    //httpWin = 0;
}

void netThread::slotThreadStop()
{
    //slotSynchStop();
    //stop = true;

    emit this->finished();
}
