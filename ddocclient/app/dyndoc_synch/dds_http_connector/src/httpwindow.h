
#ifndef DDS_HTTPWINDOW_H
#define DDS_HTTPWINDOW_H

#include <QDialog>

#include <JKKSLoader.h>
#include <JKKSPMessage.h>

#include "../dyndoc_synch/Namespaces/netStruct.h"

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


class dds_HttpWindow : public QDialog
{
    Q_OBJECT

public:
    dds_HttpWindow(dyndoc_mainStructs::netThreadInf rhs, QWidget *parent = 0);
    bool sendOutMessage( const JKKSPMessWithAddr * message, 
                         bool filePartsFlag = true, //filePartsFlag - ���� �������� ����� �� ������. �������� � ����, ��� ���������� ���������� ����� ��� QHttp::post()
                         bool isLastFilePart = false); //��������, ��� ���������� ��������� ����� �����, ������������� �������
    bool setMessageAsSended(const qint64 & id, const int & type, bool sended = true);//��������� �������� - ���� ���������� �������� ���������
    
    ~dds_HttpWindow();

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
    void needToExitEventLoop(); //������������ � httpRequestFinished. ��������, ��� ���������� ����� __������__ ������ post-> . ������� � ���, ��� ���� ��������� EventLoop � ���������� � �������� ���������� ������

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
    
    int cntMsgSended;//���������� ������������ ���������
    int msgForSent; //���������� ���������, �������������� ��� ��������
    int cntFilesSended;//���������� ������������ ������
    int filesForSent; //���������� ������, �������������� ��� ��������
    int filePartsForSent;//���������� ������ ������ ��� ��������
    int cntFilePartsSended;//���������� ������������ ������ ������
    
	bool manual;

    dyndoc_mainStructs::dbInf* dataBase;
    dyndoc_mainStructs::transportInf* transport;
    dyndoc_mainStructs::timerInf* timer;
};

#endif
