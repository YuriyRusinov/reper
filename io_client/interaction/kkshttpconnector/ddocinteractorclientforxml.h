
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

/*������ ����� ������������ ����� ��������� �������������� � �������� ��������� �� ������ �������� XML-��������.

�� ������������ ��� ������ ������� �� out_external_queue � �������� XML-��������� �����������,
������� ����� ���� ������������ ��� �������� ���������, ��� � ���������� ��������� DynamicDocs Server.
������ ������������� �� HTTP-��������� �� �������� ����� � ����.
������������� �� ����������� ������� �� ���������.
����� ������� ������ ��������� �������������� �� ������������ ����������������� ��������.
����������������� �������� ������������ ��������� ��������������, �������������� ������� DDocInteractorClient.
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
                         bool bSync = true); // ���� �������� ������ � ���������� ������. TRUE �������� � ����, ��� ���������� ���������� ����� ��� QHttp::request()
    
    //bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//��������� �������� - ���� ���������� �������� ���������

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

    void needToExitEventLoop(); //������������ � httpRequestFinished. ��������, ��� ���������� ����� __������__ ������ post-> . ������� � ���, ��� ���� ��������� EventLoop � ���������� � �������� ���������� ������

    void sendingStarted();//������������, ����� ���������� ��������� ������ � �� �������� ����������
    void sendingCompleted();//������������, ����� �������� ������ ���������� ���������

private:
    JKKSLoader * m_loader;
    DDocInteractorBase * m_parent;

    QHttp *http; //������������ ��� �������� ��������� (���������)

    bool manual; //true - ������ ����� ������ �� 

    QTimer * m_timer;
    int m_interval; //�������� ������� �� ����� ��. �����! ��� �������� ������ ������ ������������������ �� ��������� ��������
    bool m_isExiting;//true - ���������� ���������� ������ ����������. ���������� ��������� ������ ������
    bool m_isSending; // true - ���������� �������� ������ �����������. � ���� ������ ���������� ����������, ����� ������ (���� ��������) �� ����� "�������������������" ������ ������ startProc()

    QList<JKKSPMessWithAddr *> messageList;//������ ��������� ��� ��������. ������������� �� JKKSLoader � ��������� ����� ����� �������� (������ sendOutMessage() ��� sendPings() )
    QMap<int, QPair<qint64, qint64> > httpMessages;//������������ ��������� ����� this->http. � �������� ����� ������������ ������������� ������� �� ������ QHttp::request()


    QString gatewayHost;//���� � ���� ����� ���, ���� ������������ ��� ��� �������� ���������
    int gatewayPort;
};


#endif
