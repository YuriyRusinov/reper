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
    int intervalMs = 10000;
    int interval = 10;
    int units = 0;//seconds
    bool mode = false; //true = manual

    TimerForm * timerForm = new TimerForm ();
    if (!timerForm)
    {
        return;
    }

    timerForm->init(interval, units, mode, true);

    if (timerForm->exec () != QDialog::Accepted)
    {
        delete timerForm;
        return;
    }
    else
    {
        mode = timerForm->startManually();
        intervalMs = timerForm->getTimerMs();
    }
    delete timerForm;

    m_base = new DDocInteractorBase(NULL);

    connect(this,SIGNAL(signalThreadStop()),m_base,SLOT(slotStopClient()));
    connect(m_base,SIGNAL(pingsSended(QMap<QString,JKKSPing>)),this,SLOT(slotPingsSended(QMap<QString,JKKSPing>)));

    //httpWin = new DDocInteractorWindow();

    //httpWin->setWindowModality(Qt::NonModal);
    //httpWin->setTimerParams(interval, units, mode);
    //httpWin->setInteractorBase(m_base);

    int ok = m_base->start(mode, intervalMs);
    if(ok != 1)
        return;

    //if(ok)
    //{
        //httpWin->show();
    //}
}

void networkStream::slotStopSyncronization()
{
    emit signalThreadStop();

    //httpWin->close();
    //delete httpWin;
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

void networkStream::slotPingsSended(QMap<QString,JKKSPing> rhs)
{
    emit signalPingsSended(rhs);
}
