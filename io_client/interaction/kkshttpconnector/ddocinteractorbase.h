
#ifndef DDOCINTERACTORBASE_H
#define DDOCINTERACTORBASE_H

#include <JKKSLoader.h>
#include "config_httpconnector.h"

#include <JKKSPing.h>

class DDocInteractorClient;
class DDocInteractorClientForXML;
class DDocInteractorServer;

class __HTTP_CONNECTOR_EXPORT DDocInteractorBase : public QObject
{
    Q_OBJECT
public:
    friend class DDocInteractorWindow;

    DDocInteractorBase(QObject * parent);
    ~DDocInteractorBase();

    int start(bool mode, int interval, bool bMain = true, bool bXML = false); //3 - надо ли начинать опрашивать главную исходящую очередь
                                                                              //4 - надо ли начинать опрашивать исходящую очередь для XML-сообщений

    bool isExiting(){return m_isExiting;}
    void setIsExiting(){m_isExiting = true;}

    friend class DDocInteractorServer;
    friend class DDocInteractorClient;
    friend class DDocInteractorClientForXML;

public slots:
    void slotExitThreads();
    
    void slotStartClient();
    void slotStopClient();

    void slotStartClientForXML();
    void slotStopClientForXML();

private slots:

    void _startSending() { emit startSending(); }
    void _startSendingForXML() { emit startSendingForXML(); }

signals:
    void pingsSended(QMap<QString, JKKSPing> pings);//генерируется, когда отправка всех пингов на все интересующие оргнизации завершена и пришли все ответы на эти пинги
    void pingsSentCompleted();//генерируется, когда отправка всех пингов на все интересующие оргнизации завершена и пришли все ответы на эти пинги. Завершает соответствующий QEventLoop

    void sendingStarted();//генерируется, когда происходит в потоке чтения БД получение данных и их отправка получателю
    void sendingCompleted();//генерируется, когда отправка данных получателю завершена

    void startSending();//генерируется, когда нажата кнопка "start" и необходимо начать опрос БД и отправку данных (в ручном режиме)
    void startSendingForXML();//генерируется, когда нажата кнопка "start" и необходимо начать опрос БД и отправку данных (в ручном режиме) для очереди XML-сообщений

    void refreshTimer(int interval); //изменить значение интервала таймера

    void theSignal();
    void exitThreads();//генерируется, когда требуется завершить работу потоков

    void hideManualStartButton();//генерируется, если выбран автоматический режим функционирования. Скрывает кнопку ручного опроса БД на форме
    void showManualStartButton();//генерируется, если выбран ручной режим функционирования. Показывает кнопку ручного опроса БД на форме


private:
    QMap<QString, JKKSPing> m_pings; //в качестве ключа используется email_prefix целевой организации

    int cntMsgSended;//количество отправленных сообщений
    int msgForSent; //количество сообщений, подготовленных для отправки
    int cntFilesSended;//количество отправленных файлов
    int filesForSent; //количество файлов, подготовленных для отправки
    int filePartsForSent;//количество частей файлов для отправки
    int cntFilePartsSended;//количество отправленных частей файлов

    int cntPingsSended;//количество отправленных пингов
    int pingsForSent;//количество пингов, подготовленных для отправки

    DDocInteractorServer * m_server;
    DDocInteractorClient * m_client;
    DDocInteractorClientForXML * m_clientForXML;

    JKKSLoader * m_loader;

    bool m_started;
    bool m_isExiting;  //true - если завершаем работу (т.е. нажали на кнопку закрытия приложения)
};

#endif
