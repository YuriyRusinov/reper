
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
                         bool bSync = true, // ���� �������� ������ � ���������� ������. TRUE �������� � ����, ��� ���������� ���������� ����� ��� QHttp::post()
                         bool isLastFilePart = false); //��������, ��� ���������� ��������� ����� �����, ������������� �������
    bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//��������� �������� - ���� ���������� �������� ���������

    bool doNotStart() const {return m_doNotStart;}
    
private slots:
    void startProc();
    void startTimer();
    
    void loadData();//tcp-������ ������� ������ �� ���������� �������
    
    void httpRequestFinished(int requestId, bool error);//������������ ����� ��� ���������
    void pingHttpRequestFinished(int requestId, bool error);//������������ ����� ��� ������

    int processMessage(const QByteArray & ba, QTcpSocket * clientConnection);
    int processPingResponse(const JKKSPing * ping);
    int processNotification(const QByteArray & ba, QTcpSocket * clientConnection);
    int processPingNotification(int idMsg, int result);
    
    void sendOKBlock(QTcpSocket * clientConnection, bool withData);//������������� ������� ���� OK �� ������ ���������� �������
    void sendBadBlock(QTcpSocket * clientConnection);//������������� ������� ���� Bad response �� ������ ���������� �������

    void slotHttpMessageRemoved(int progress);

    int sendPings();
    bool sendOutPing( const JKKSPMessWithAddr * ping);

protected:
    void closeEvent(QCloseEvent * event);

signals:
    void httpMessageRemoved(int progress);
    void needToExitEventLoop(); //������������ � httpRequestFinished. ��������, ��� ���������� ����� __������__ ������ post-> . ������� � ���, ��� ���� ��������� EventLoop � ���������� � �������� ���������� ������
    void pingsSended(QMap<QString, JKKSPing> pings);//������������, ����� �������� ���� ������ �� ��� ������������ ���������� ��������� � ������ ��� ������ �� ��� �����
    void pingsSentCompleted();//������������, ����� �������� ���� ������ �� ��� ������������ ���������� ��������� � ������ ��� ������ �� ��� �����. ��������� ��������������� QEventLoop

private:
    QLabel *statusLabel;

    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QList<JKKSPMessWithAddr *> messageList;//������ ��������� ��� ��������. ������������� �� JKKSLoader � ��������� ����� ����� �������� (������ sendOutMessage() ��� sendPings() )

    QMap<int, QPair<qint64, qint64> > httpMessages;//������������ ��������� ����� this->http. � �������� ����� ������������ ������������� ������� �� ������ QHttp::post()
    QMap<int, JKKSPing> pingHttpMessages; //������������ ����� ����� this->pingHttp. � �������� ����� ������������ ������������� ������� �� ������ QHttp::post()

    QMap<QString, JKKSPing> m_pings; //� �������� ����� ������������ email_prefix ������� �����������

    QString gatewayHost;//���� � ���� ����� ���, ���� ������������ ��� ��� �������� ���������
    int gatewayPort;

    QHttp *http; //������������ ��� �������� ���������
    QHttp *pingHttp; //������������ ��� �������� ������
    JKKSLoader * loader;

    QTcpServer *tcpServer;

    QTimer * m_timer;
    int user_timer; //�������� ������� �� ����� ��. �����! ��� �������� ������ ������ ������������������ �� ��������� ��������
    
    int cntMsgSended;//���������� ������������ ���������
    int msgForSent; //���������� ���������, �������������� ��� ��������
    int cntFilesSended;//���������� ������������ ������
    int filesForSent; //���������� ������, �������������� ��� ��������
    int filePartsForSent;//���������� ������ ������ ��� ��������
    int cntFilePartsSended;//���������� ������������ ������ ������

    int cntPingsSended;//���������� ������������ ������
    int pingsForSent;//���������� ������, �������������� ��� ��������
    
	bool manual; //true - ������ ����� ������ �� 

    bool m_doNotStart;//���� ������ tcpServer ���������� �������� ��� ���� �������� �������� � ������������, �� ������ ���� ������������ � true � ��������, ��� �� ���� ��������� ����������
};

#endif
