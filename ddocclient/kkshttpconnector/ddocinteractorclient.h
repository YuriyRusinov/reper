
#ifndef DDOCINTERACTORCLIENT_H
#define DDOCINTERACTORCLIENT_H

#include <QThread>
#include <QHttpRequestHeader>
#include <QMap>
#include <QPair>
#include "config_httpconnector.h"

#include <JKKSPing.h>

//net
class QHttp;
class QHttpResponseHeader;
class QSslError;
class QAuthenticator;
class QTimer;

class JKKSLoader;
class DDocInteractorBase;
class JKKSPMessWithAddr;

class __HTTP_CONNECTOR_EXPORT DDocInteractorClient : public QThread
{
    Q_OBJECT
public:
    friend class DDocInteractorBase;

    DDocInteractorClient(JKKSLoader* loader, DDocInteractorBase * parent);
    ~DDocInteractorClient();

    void run();
    
    void setMode(bool m){manual = m;}
    void setInterval(int i){m_interval = i;}
    void setGateway(const QString & host, int port){gatewayHost = host; gatewayPort = port;}


private:
    bool sendOutMessage( const JKKSPMessWithAddr * message, 
                         bool bSync = true, // флаг передачи данных в синхронном режиме. TRUE приводит к тому, что включается синхронный режим для QHttp::request()
                         bool isLastFilePart = false); //означает, что передается последняя часть файла, передаваемого частями
    bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//последний параметр - флаг успешности доставки сообщения

    int sendPings(bool bSync); //параметр - рассылать синхронно (да/нет)
    //bool sendOutPing( const JKKSPMessWithAddr * ping);
    int processPingResponse(const JKKSPing * ping);

    QByteArray prepareForSend(QHttpRequestHeader & header,
                              QHttp * http,
                              const JKKSPMessWithAddr * message, 
                              bool bSync,
                              bool isLastFilePart,
                              bool isPing);

    bool createRequestHeader(QHttpRequestHeader & header,
                             const QString & host, 
                             int port, 
                             bool https, 
                             int length);

    void saveRequestId(int reqId, const JKKSPMessWithAddr * message, bool isLastFilePart);
    bool verifyPings();//проверка доступности целевых объектов через пинги.

public slots:
    void startProc();
    void init();
    void slotRefreshTimer(int interval);

private slots:
    void createTimer();


    void httpRequestFinished(int requestId, bool error);//обрабатываем ответ для сообщений
    void pingHttpRequestFinished(int requestId, bool error);//обрабатываем ответ для пингов
    void pingResHttpRequestFinished(int requestId, bool error);//обрабатываем результат получаения ответа на пинги

    void slotHttpMessageRemoved(int progress);

signals:
    void pingsSended(QMap<QString, JKKSPing> pings);//генерируется, когда отправка всех пингов на все интересующие оргнизации завершена и пришли все ответы на эти пинги
    void pingsSentCompleted();//генерируется, когда отправка всех пингов на все интересующие оргнизации завершена и пришли все ответы на эти пинги. Завершает соответствующий QEventLoop

    void needToExitEventLoop(); //генерируется в httpRequestFinished. Означает, что завершился вызов __нашего__ вызова post-> . Говорит о том, что надо завершить EventLoop и приступить к отправке следующего пакета
    void needToExitEventLoopPing();//генерируется в pingHttpRequestFinished. Означает, что завершился вызов __нашего__ вызова post-> . Говорит о том, что надо завершить EventLoop и приступить к отправке следующего пакета для пингов
    void httpMessageRemoved(int progress);

    void sendingStarted();//генерируется, когда происходит получение данных и их отправка получателю
    void sendingCompleted();//генерируется, когда отправка данных получателю завершена

    //void showStatusText(QString);//генерируется, когда требуется вывести на форму нектороый текст о текущем состоянии отправки данных

private:
    JKKSLoader * m_loader;
    DDocInteractorBase * m_parent;

    QHttp *http; //используется для отправки сообщений (синхронно)
    QHttp *pingHttp; //используется для отправки пингов (синхронно)
    QHttp *pingResHttp; //используется для отправки ответов на пинги (асинхронно)

	bool manual; //true - ручной режим опроса БД 

    QTimer * m_timer;
    int m_interval; //интервал таймера на опрос БД. Важно! при отправке данных таймер приостанавливается до окончания передачи
    bool m_isExiting;//true - происходит завершение работы приложения. Необходимо завершить работу потока

    QList<JKKSPMessWithAddr *> messageList;//список сообщений для отправки. Запрашивается из JKKSLoader и очищается сразу после отправки (вызова sendOutMessage() или sendPings() )
    QMap<int, QPair<qint64, qint64> > httpMessages;//отправленные сообщения через this->http. В качестве ключа используется идентификатор запроса из метода QHttp::request()
    QMap<int, JKKSPing> pingHttpMessages; //отправленные пинги через this->pingHttp. В качестве ключа используется идентификатор запроса из метода QHttp::request()
    QMap<int, QPair<qint64, qint64> > pingResHttpMessages;//отправленные ответы на пинги через this->pingResHttp. В качестве ключа используется идентификатор запроса из метода QHttp::request()


    QString gatewayHost;//хост и порт шлюза ТПС, если используется ТПС для отправки сообщений
    int gatewayPort;
};


#endif
