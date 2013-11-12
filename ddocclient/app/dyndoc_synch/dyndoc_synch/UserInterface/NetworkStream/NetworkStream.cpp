#include "UserInterface/NetworkStream/NetworkStream.h"

networkStream::networkStream(QObject *parent):
    QObject(parent)
{
    //net = new netThread(this);

    //connect(this,SIGNAL(signalSynchStart()),net,SLOT(slotSynchStart()));
    //connect(this,SIGNAL(signalSynchStop()),net,SLOT(slotSynchStop()));
    //connect(this,SIGNAL(signalThreadStop()),net,SLOT(slotThreadStop()));
    //connect(net,SIGNAL(finished()),this,SLOT(slotPoll()));

    //net->start();
}

networkStream::~networkStream()
{

}

void networkStream::slotStartSyncronization()
{
    net = new netThread(this);
    //moveToThread(net);

    connect(this,SIGNAL(signalSynchStart()),net,SLOT(slotSynchStart()));
    connect(this,SIGNAL(signalSynchStop()),net,SLOT(slotSynchStop()));
    connect(this,SIGNAL(signalThreadStop()),net,SLOT(slotThreadStop()));
    connect(net,SIGNAL(finished()),this,SLOT(slotPoll()));

    net->start();

    emit signalSynchStart();
}

void networkStream::slotStopSyncronization()
{
    emit signalThreadStop();
}

void networkStream::slotPoll()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText("display log");
    msgBox.exec();
}

void networkStream::slotImplementInitialSyncronization()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Button test"));
    msgBox.setText("display log");
    msgBox.exec();
}
