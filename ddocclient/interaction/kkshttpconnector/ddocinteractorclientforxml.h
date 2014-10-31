
#ifndef DDOCINTERACTORCLIENT_FORXML_H
#define DDOCINTERACTORCLIENT_FORXML_H

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

/*Данный класс представляет собой интерфейс взаимодействия с внешними системами на основе обменных XML-форматов.

Он предназначен для опроса таблицы БД out_external_queue и отправки XML-сообщений получателям,
которые могут быть представлены как внешними системами, так и удаленными системами DynamicDocs Server.
Данные отправлчяются по HTTP-протоколу на заданный адрес и порт.
Подтверждения от принимающей стороны не ожидается.
Таким образом данный интерфейс взаимодействия не обеспечивает гарантированность доставки.
Гарантированность доставки обеспечивает интерфейс взаимодействия, представленный классом DDocInteractorClient.
*/
class __HTTP_CONNECTOR_EXPORT DDocInteractorClientForXML : public QThread
{
    Q_OBJECT
public:
    friend class DDocInteractorBase;

    DDocInteractorClientForXML(JKKSLoader* loader, DDocInteractorBase * parent);
    ~DDocInteractorClientForXML();

    void run();
    
    void setManual(bool m){manual = m;}
    void setInterval(int i){m_interval = i;}
    void setGateway(const QString & host, int port){gatewayHost = host; gatewayPort = port;}


private:
    bool sendOutMessage( const JKKSPMessWithAddr * message, 
                         bool bSync = true); // флаг передачи данных в синхронном режиме. TRUE приводит к тому, что включается синхронный режим для QHttp::request()
    
    //bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//последний параметр - флаг успешности доставки сообщения

    void saveRequestId(int reqId, const JKKSPMessWithAddr * message);
    QByteArray prepareForSend(QHttpRequestHeader & header,
                              QHttp * http,
                              const JKKSPMessWithAddr * message);

    bool createRequestHeader(QHttpRequestHeader & header,
                             const QString & host, 
                             int port, 
                             //bool https, 
                             int length);

public slots:
    void startProc();
    void init();
    void slotRefreshTimer(int interval);

private slots:
    void createTimer();
    void httpRequestFinished(int requestId, bool error);

signals:

    void needToExitEventLoop(); //генерируется в httpRequestFinished. Означает, что завершился вызов __нашего__ вызова post-> . Говорит о том, что надо завершить EventLoop и приступить к отправке следующего пакета

    void sendingStarted();//генерируется, когда происходит получение данных и их отправка получателю
    void sendingCompleted();//генерируется, когда отправка данных получателю завершена

private:
    JKKSLoader * m_loader;
    DDocInteractorBase * m_parent;

    QHttp *http; //используется для отправки сообщений (синхронно)

    bool manual; //true - ручной режим опроса БД 

    QTimer * m_timer;
    int m_interval; //интервал таймера на опрос БД. Важно! при отправке данных таймер приостанавливается до окончания передачи
    bool m_isExiting;//true - происходит завершение работы приложения. Необходимо завершить работу потока
    bool m_isSending; // true - происходит передача данных получателям. В этом случае необходимо обеспечить, чтобы таймер (даже случайно) не начал "несанкционированный" запуск метода startProc()

    QList<JKKSPMessWithAddr *> messageList;//список сообщений для отправки. Запрашивается из JKKSLoader и очищается сразу после отправки (вызова sendOutMessage() или sendPings() )
    QMap<int, QPair<qint64, qint64> > httpMessages;//отправленные сообщения через this->http. В качестве ключа используется идентификатор запроса из метода QHttp::request()


    QString gatewayHost;//хост и порт шлюза ТПС, если используется ТПС для отправки сообщений
    int gatewayPort;
};


#endif
