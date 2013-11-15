
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
                         bool bSync = true, // ���� �������� ������ � ���������� ������. TRUE �������� � ����, ��� ���������� ���������� ����� ��� QHttp::request()
                         bool isLastFilePart = false); //��������, ��� ���������� ��������� ����� �����, ������������� �������
    bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//��������� �������� - ���� ���������� �������� ���������

    int sendPings(bool bSync); //�������� - ��������� ��������� (��/���)
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
    bool verifyPings();//�������� ����������� ������� �������� ����� �����.

public slots:
    void startProc();
    void init();
    void slotRefreshTimer(int interval);

private slots:
    void createTimer();


    void httpRequestFinished(int requestId, bool error);//������������ ����� ��� ���������
    void pingHttpRequestFinished(int requestId, bool error);//������������ ����� ��� ������
    void pingResHttpRequestFinished(int requestId, bool error);//������������ ��������� ���������� ������ �� �����

    void slotHttpMessageRemoved(int progress);

signals:
    void pingsSended(QMap<QString, JKKSPing> pings);//������������, ����� �������� ���� ������ �� ��� ������������ ���������� ��������� � ������ ��� ������ �� ��� �����
    void pingsSentCompleted();//������������, ����� �������� ���� ������ �� ��� ������������ ���������� ��������� � ������ ��� ������ �� ��� �����. ��������� ��������������� QEventLoop

    void needToExitEventLoop(); //������������ � httpRequestFinished. ��������, ��� ���������� ����� __������__ ������ post-> . ������� � ���, ��� ���� ��������� EventLoop � ���������� � �������� ���������� ������
    void needToExitEventLoopPing();//������������ � pingHttpRequestFinished. ��������, ��� ���������� ����� __������__ ������ post-> . ������� � ���, ��� ���� ��������� EventLoop � ���������� � �������� ���������� ������ ��� ������
    void httpMessageRemoved(int progress);

    void sendingStarted();//������������, ����� ���������� ��������� ������ � �� �������� ����������
    void sendingCompleted();//������������, ����� �������� ������ ���������� ���������

    //void showStatusText(QString);//������������, ����� ��������� ������� �� ����� ��������� ����� � ������� ��������� �������� ������

private:
    JKKSLoader * m_loader;
    DDocInteractorBase * m_parent;

    QHttp *http; //������������ ��� �������� ��������� (���������)
    QHttp *pingHttp; //������������ ��� �������� ������ (���������)
    QHttp *pingResHttp; //������������ ��� �������� ������� �� ����� (����������)

	bool manual; //true - ������ ����� ������ �� 

    QTimer * m_timer;
    int m_interval; //�������� ������� �� ����� ��. �����! ��� �������� ������ ������ ������������������ �� ��������� ��������
    bool m_isExiting;//true - ���������� ���������� ������ ����������. ���������� ��������� ������ ������

    QList<JKKSPMessWithAddr *> messageList;//������ ��������� ��� ��������. ������������� �� JKKSLoader � ��������� ����� ����� �������� (������ sendOutMessage() ��� sendPings() )
    QMap<int, QPair<qint64, qint64> > httpMessages;//������������ ��������� ����� this->http. � �������� ����� ������������ ������������� ������� �� ������ QHttp::request()
    QMap<int, JKKSPing> pingHttpMessages; //������������ ����� ����� this->pingHttp. � �������� ����� ������������ ������������� ������� �� ������ QHttp::request()
    QMap<int, QPair<qint64, qint64> > pingResHttpMessages;//������������ ������ �� ����� ����� this->pingResHttp. � �������� ����� ������������ ������������� ������� �� ������ QHttp::request()


    QString gatewayHost;//���� � ���� ����� ���, ���� ������������ ��� ��� �������� ���������
    int gatewayPort;
};


#endif
