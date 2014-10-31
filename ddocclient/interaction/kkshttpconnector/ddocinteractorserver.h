
#ifndef DDOCINTERACTORSERVER_H
#define DDOCINTERACTORSERVER_H

#include <QThread>
#include <QTcpServer>

#include <JKKSLoader.h>
#include <JKKSPMessage.h>
#include "config_httpconnector.h"
//net
class QHttpResponseHeader;
//class QTcpServer;
class QTcpSocket;
class QSslError;
class QAuthenticator;

class JKKSLoader;
class DDocInteractorBase;

class __HTTP_CONNECTOR_EXPORT DDocTcpServer : public QTcpServer
{
   Q_OBJECT

public:
    DDocTcpServer(QObject *parent = 0): QTcpServer(parent){}

signals:
     void connectionArrived(int);

protected:
     void incomingConnection(int socketDescriptor);

};

class __HTTP_CONNECTOR_EXPORT DDocInteractorServer : public QThread
{
    Q_OBJECT
public:
    friend class DDocInteractorBase;

    DDocInteractorServer(JKKSLoader* loader, DDocInteractorBase * parent);
    ~DDocInteractorServer();

    void run();
    bool successed(){return m_succesed;}
    void setPort(int port) {m_port = port;}
    void setWaitClientConnectionTimeout(int t = 1000) {m_waitClientConnectionTimeout = t;}
    QString errorString(){return m_tcpServer->errorString();}

public slots:
    void init();

private slots:
    void loadData(int socketDescriptor);//tcp-������ ������� ������ �� ���������� �������

    void sendOKBlock(QTcpSocket * clientConnection, bool withData);//������������� ������� ���� OK �� ������ ���������� �������
    void sendBadBlock(QTcpSocket * clientConnection);//������������� ������� ���� Bad response �� ������ ���������� �������

signals:
    void pingsSended(QMap<QString, JKKSPing> pings);//������������, ����� �������� ���� ������ �� ��� ������������ ���������� ��������� � ������ ��� ������ �� ��� �����
    void pingsSentCompleted();//������������, ����� �������� ���� ������ �� ��� ������������ ���������� ��������� � ������ ��� ������ �� ��� �����. ��������� ��������������� QEventLoop

private:
    int processXMLMessage(const QString & xml, QTcpSocket * clientConnection);
    int processMessage(QMap <QString, QByteArray> & post_data, QTcpSocket * clientConnection);
    int processNotification(QMap <QString, QByteArray> & post_data, QTcpSocket * clientConnection);

    QMap<QString, QByteArray> parsePost (const QByteArray &array);

    int processPingResponse(const JKKSPing * ping);
    int processPingNotification(int idMsg, int result);
    bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//��������� �������� - ���� ���������� �������� ���������

private:
    DDocTcpServer * m_tcpServer;
    JKKSLoader * m_loader;
    DDocInteractorBase * m_parent;
    int m_port;
    bool m_succesed;//������ TCP-������� ������ �������
    bool m_isExiting;

    int m_waitClientConnectionTimeout; //����-��� ��� ������� QTcpSocket::waitForReadyRead()
                                       //�������� � �������������
                                       // �� ��������� 1000


};

#endif
