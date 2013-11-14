
#ifndef DDOCINTERACTORBASE_H
#define DDOCINTERACTORBASE_H

#include <JKKSLoader.h>
#include "config_httpconnector.h"

#include <JKKSPing.h>

class DDocINteractorClient;
class DDocInteractorServer;

class __HTTP_CONNECTOR_EXPORT DDocInteractorBase : public QObject
{
    Q_OBJECT
public:
    DDocInteractorBase(QObject * parent);
    ~DDocInteractorBase();

    int start(bool mode, int interval);

    bool isExiting(){return m_isExiting;}
    void setIsExiting(){m_isExiting = true;}

    friend class DDocInteractorServer;
    friend class DDocInteractorClient;

public slots:
    void slotExitThreads();
    void slotStartClient();
    void slotStopClient();

signals:
    void pingsSended(QMap<QString, JKKSPing> pings);//генерируется, когда отправка всех пингов на все интересующие оргнизации завершена и пришли все ответы на эти пинги
    void pingsSentCompleted();//генерируется, когда отправка всех пингов на все интересующие оргнизации завершена и пришли все ответы на эти пинги. Завершает соответствующий QEventLoop
    void sendingStarted();//генерируется, когда происходит в потоке чтения БД получение данных и их отправка получателю
    void sendingCompleted();//генерируется, когда отправка данных получателю завершена
    void startSending();//генерируется, когда нажата кнопка "start" и необходимо начать опрос БД и отправку данных (в ручном режиме)
    void showStatusText(QString);//генерируется, когда требуется вывести на форму нектороый текст о текущем состоянии отправки данных

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
    JKKSLoader * m_loader;

    bool m_started;
    bool m_isExiting;  //true - если завершаем работу (т.е. нажали на кнопку закрытия приложения)
};

#endif
