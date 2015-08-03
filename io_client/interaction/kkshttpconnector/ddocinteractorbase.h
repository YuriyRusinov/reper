
#ifndef DDOCINTERACTORBASE_H
#define DDOCINTERACTORBASE_H

#include <JKKSLoader.h>
#include "config_httpconnector.h"

#include <JKKSPing.h>

class DDocInteractorClient;
class DDocInteractorClientForXML;
class DDocInteractorServer;

class __HTTP_CONNECTOR_EXPORT DDocInteractorBase : public QObject
{
    Q_OBJECT
public:
    friend class DDocInteractorWindow;

    DDocInteractorBase(QObject * parent);
    ~DDocInteractorBase();

    int start(bool mode, int interval, bool bMain = true, bool bXML = false); //3 - ���� �� �������� ���������� ������� ��������� �������
                                                                              //4 - ���� �� �������� ���������� ��������� ������� ��� XML-���������

    bool isExiting(){return m_isExiting;}
    void setIsExiting(){m_isExiting = true;}

    friend class DDocInteractorServer;
    friend class DDocInteractorClient;
    friend class DDocInteractorClientForXML;

public slots:
    void slotExitThreads();
    
    void slotStartClient();
    void slotStopClient();

    void slotStartClientForXML();
    void slotStopClientForXML();

private slots:

    void _startSending() { emit startSending(); }
    void _startSendingForXML() { emit startSendingForXML(); }

signals:
    void pingsSended(QMap<QString, JKKSPing> pings);//������������, ����� �������� ���� ������ �� ��� ������������ ���������� ��������� � ������ ��� ������ �� ��� �����
    void pingsSentCompleted();//������������, ����� �������� ���� ������ �� ��� ������������ ���������� ��������� � ������ ��� ������ �� ��� �����. ��������� ��������������� QEventLoop

    void sendingStarted();//������������, ����� ���������� � ������ ������ �� ��������� ������ � �� �������� ����������
    void sendingCompleted();//������������, ����� �������� ������ ���������� ���������

    void startSending();//������������, ����� ������ ������ "start" � ���������� ������ ����� �� � �������� ������ (� ������ ������)
    void startSendingForXML();//������������, ����� ������ ������ "start" � ���������� ������ ����� �� � �������� ������ (� ������ ������) ��� ������� XML-���������

    void refreshTimer(int interval); //�������� �������� ��������� �������

    void theSignal();
    void exitThreads();//������������, ����� ��������� ��������� ������ �������

    void hideManualStartButton();//������������, ���� ������ �������������� ����� ����������������. �������� ������ ������� ������ �� �� �����
    void showManualStartButton();//������������, ���� ������ ������ ����� ����������������. ���������� ������ ������� ������ �� �� �����


private:
    QMap<QString, JKKSPing> m_pings; //� �������� ����� ������������ email_prefix ������� �����������

    int cntMsgSended;//���������� ������������ ���������
    int msgForSent; //���������� ���������, �������������� ��� ��������
    int cntFilesSended;//���������� ������������ ������
    int filesForSent; //���������� ������, �������������� ��� ��������
    int filePartsForSent;//���������� ������ ������ ��� ��������
    int cntFilePartsSended;//���������� ������������ ������ ������

    int cntPingsSended;//���������� ������������ ������
    int pingsForSent;//���������� ������, �������������� ��� ��������

    DDocInteractorServer * m_server;
    DDocInteractorClient * m_client;
    DDocInteractorClientForXML * m_clientForXML;

    JKKSLoader * m_loader;

    bool m_started;
    bool m_isExiting;  //true - ���� ��������� ������ (�.�. ������ �� ������ �������� ����������)
};

#endif
