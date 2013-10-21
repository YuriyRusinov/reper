
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
    bool sendOutMessage( const JKKSPMessWithAddr * message, bool filePartsFlag = true);//filePartsFlag - флаг передачи файла по частям
    bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//последний параметр - флаг успешности доставки сообщения
    
private slots:
    void startProc();
    void startTimer();
    void loadData();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);

    int processMessage(const QByteArray & ba, QTcpSocket * clientConnection);
    int processNotification(const QByteArray & ba, QTcpSocket * clientConnection);

    void slotHttpMessageRemoved(int progress);

signals:
    void httpMessageRemoved(int progress);

private:
    QLabel *statusLabel;

    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QList<JKKSPMessWithAddr *> messageList;

    QMap<int, QPair<qint64, qint64> > httpMessages;

    QString gatewayHost;
    int gatewayPort;

    QHttp *http;
    JKKSLoader * loader;

    QTimer * m_timer;
    QString localDBInfo;
    
    QTcpServer *tcpServer;
    QStringList responses;

    int httpGetId;
    int user_timer;
    
    int cntMsgSended;//количество отправленных сообщений
    int msgForSent; //количество сообщений, подготовленных для отправки
    int cntFilesSended;//количество отправленных файлов
    int filesForSent; //количество файлов, подготовленных для отправки
    
	bool manual;
};

#endif
