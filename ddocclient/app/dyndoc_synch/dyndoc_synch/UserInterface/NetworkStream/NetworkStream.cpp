#include "UserInterface/NetworkStream/NetworkStream.h"

networkStream::networkStream(QObject *parent):
    QObject(parent)
{

}

networkStream::~networkStream()
{

}

void networkStream::slotStartSyncronization()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Net modul"));
    msgBox.setText(tr("syncronization start"));
    msgBox.exec();
}

void networkStream::slotStopSyncronization()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Net modul"));
    msgBox.setText(tr("syncronization stop"));
    msgBox.exec();
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
