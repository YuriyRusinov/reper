
#ifndef HTTPWINDOW_H
#define HTTPWINDOW_H

#include <QDialog>

#include <JKKSLoader.h>
#include <JKKSPMessage.h>
//gui
class QDialogButtonBox;
class QFile;
class QLabel;
class QLineEdit;
class QProgressDialog;
class QPushButton;
//net
class QHttp;
class QHttpResponseHeader;
class QTcpServer;
class QTcpSocket;
class QSslError;
class QAuthenticator;


class HttpWindow : public QDialog
{
    Q_OBJECT

public:
    HttpWindow(QWidget *parent = 0);
    
    
    bool sendOutMessage( const JKKSPMessWithAddr * message, 
                         bool bSync = true, // флаг передачи данных в синхронном режиме. TRUE приводит к тому, что включается синхронный режим для QHttp::post()
                         bool isLastFilePart = false); //означает, что передается последняя часть файла, передаваемого частями
    bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//последний параметр - флаг успешности доставки сообщения

    bool doNotStart() const {return m_doNotStart;}
    
private slots:
    void startProc();
    void startTimer();
    
    void loadData();//tcp-сервер получил данные от передающей стороны
    
    void httpRequestFinished(int requestId, bool error);//обрабатываем ответ для сообщений
    void pingHttpRequestFinished(int requestId, bool error);//обрабатываем ответ для пингов

    int processMessage(const QByteArray & ba, QTcpSocket * clientConnection);
    int processPingResponse(const JKKSPing * ping);
    int processNotification(const QByteArray & ba, QTcpSocket * clientConnection);
    int processPingNotification(int idMsg, int result);
    
    void sendOKBlock(QTcpSocket * clientConnection, bool withData);//сгенерировать серверу отвт OK на запрос передающей стороны
    void sendBadBlock(QTcpSocket * clientConnection);//сгенерировать серверу отвт Bad response на запрос передающей стороны

    void slotHttpMessageRemoved(int progress);

    int sendPings();
    bool sendOutPing( const JKKSPMessWithAddr * ping);

protected:
    void closeEvent(QCloseEvent * event);

signals:
    void httpMessageRemoved(int progress);
    void needToExitEventLoop(); //генерируется в httpRequestFinished. Означает, что завершился вызов __нашего__ вызова post-> . Говорит о том, что надо завершить EventLoop и приступить к отправке следующего пакета
    void pingsSended(QMap<QString, JKKSPing> pings);//генерируется, когда отправка всех пингов на все интересующие оргнизации завершена и пришли все ответы на эти пинги
    void pingsSentCompleted();//генерируется, когда отправка всех пингов на все интересующие оргнизации завершена и пришли все ответы на эти пинги. Завершает соответствующий QEventLoop

private:
    QLabel *statusLabel;

    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QList<JKKSPMessWithAddr *> messageList;//список сообщений для отправки. Запрашивается из JKKSLoader и очищается сразу после отправки (вызова sendOutMessage() или sendPings() )

    QMap<int, QPair<qint64, qint64> > httpMessages;//отправленные сообщения через this->http. В качестве ключа используется идентификатор запроса из метода QHttp::post()
    QMap<int, JKKSPing> pingHttpMessages; //отправленные пинги через this->pingHttp. В качестве ключа используется идентификатор запроса из метода QHttp::post()

    QMap<QString, JKKSPing> m_pings; //в качестве ключа используется email_prefix целевой организации

    QString gatewayHost;//хост и порт шлюза ТПС, если используется ТПС для отправки сообщений
    int gatewayPort;

    QHttp *http; //используется для отправки сообщений
    QHttp *pingHttp; //используется для отправки пингов
    JKKSLoader * loader;

    QTcpServer *tcpServer;

    QTimer * m_timer;
    int user_timer; //интервал таймера на опрос БД. Важно! при отправке данных таймер приостанавливается до окончания передачи
    
    int cntMsgSended;//количество отправленных сообщений
    int msgForSent; //количество сообщений, подготовленных для отправки
    int cntFilesSended;//количество отправленных файлов
    int filesForSent; //количество файлов, подготовленных для отправки
    int filePartsForSent;//количество частей файлов для отправки
    int cntFilePartsSended;//количество отправленных частей файлов

    int cntPingsSended;//количество отправленных пингов
    int pingsForSent;//количество пингов, подготовленных для отправки
    
	bool manual; //true - ручной режим опроса БД 

    bool m_doNotStart;//если запуск tcpServer завершился неудачно или иные проблемы возникли в конструкторе, то данный флаг выставляется в true и означает, что не надо запускать приложение
};

#endif
