
#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QDialog>
#include <QThread>

#include <JKKSLoader.h>
#include <JKKSPMessage.h>
#include "config_httpconnector.h"

//gui
class QLabel;
class QPushButton;
class QCheckBox;
class QGroupBox;

class DDocInteractorBase;


class __HTTP_CONNECTOR_EXPORT DDocInteractorWindow : public QDialog
{
    Q_OBJECT

public:

    DDocInteractorWindow(QWidget *parent = 0);
    void setInteractorBase(DDocInteractorBase * b);
    void setTimerParams(int v, int u, bool m);
    
public slots:
    void closeApp();
    void stopListen();

protected:
    void closeEvent(QCloseEvent * event);

private slots:
    void slotSendingStarted();
    void slotSendingCompleted();
    //void showStatusText(QString str);
    void slotHideManualStartButton();
    void slotShowManualStartButton();
    void slotSetTimerParams();
    void slotShowConnInfo();

    void slotStopClients();
    void slotStartClients();
    void slotStartSending();

    void slotCheckBoxStateChanged(int state);


signals:
    void exitThreads();

    void stopReadDatabase();
    void startReadDatabase();
    void refreshTimer(int interval);

private:
    //QLabel *statusLabel;

    QPushButton  *manualStartButton;
    QPushButton * quitButton;
    QPushButton * stopButton;
    
    QGroupBox * gbQueues;
    QCheckBox * chQueryXML; //если выставлен, то опрашивается очередь исходящих XML-сообщений (без поддержки гарантированной доставки
    QCheckBox * chQueryMain;//если выставлен, то опрашивается основная исходящая очередь в формате DynamicDocs Interactor

    DDocInteractorBase * m_base;

    int m_timerInterval;
    int m_timerUnits;
    bool m_startManually;
};

#endif
