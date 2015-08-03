#ifndef KKS_DAEMON_H
#define KKS_DAEMON_H

#include <QCoreApplication>
//#include <QtGui/QDesktopWidget>
//#include <QtGui/QLabel>
#include <QtCore/QDir>
#include <QThread>
#include <QTimer>
#include <QHttp>
#include <QtCore/QSettings>
#include <QProcess>
#include <QFile>
#include <QTextStream>

#include "qtservice.h"
#include "pqnotify.h"
#include "kkspgdatabase.h"


class KKSDaemon;

class DDocStreamsGenerator : public QThread
{
    Q_OBJECT
public:
    DDocStreamsGenerator(KKSPGDatabase * db, QObject * parent):QThread(parent), m_db(NULL), m_parent(NULL){m_db = db;}
    ~DDocStreamsGenerator(){}

    void run();
    void setDaemon(KKSDaemon * d){m_parent = d;}

private:
    KKSPGDatabase * m_db;
    KKSDaemon * m_parent;
};

class DDocHandlersSheduler : public QThread
{
    Q_OBJECT
public:
    DDocHandlersSheduler(KKSPGDatabase * db, QObject * parent):QThread(parent), m_db(NULL), m_parent(NULL){m_db = db;}
    ~DDocHandlersSheduler(){}

    void run();
    void setDaemon(KKSDaemon * d){m_parent = d;}

private:
    KKSPGDatabase * m_db;
    KKSDaemon * m_parent;
    
    struct Service{
        qint64 idShedule;
        qint64 id;
        QString name;
        bool isExternal;
        QString host;
        int port;
        QString signature;
        QString extraParams;
        QString inData;
    };

    void startService(const Service & srv);
};

class DDocServerListener : public QThread, public IKKSListener
{
    Q_OBJECT
public:
    DDocServerListener(KKSPGDatabase * db, QObject * parent):QThread(parent), m_db(NULL), m_parent(NULL){m_db = db;}
    ~DDocServerListener(){}

    void run();
    void notify( char* notify_name, char * payload );
    void setDaemon(KKSDaemon * d){m_parent = d;}


//private slots:
//    void processFinished1(int code, QProcess::ExitStatus status);
//    void processError(QProcess::ProcessError);
//    void processStateChanged(QProcess::ProcessState);
private:
    KKSPGDatabase * m_db;
    KKSDaemon * m_parent;
};

class KKSDaemon : public QObject, public QtService<QCoreApplication>
{
    Q_OBJECT
public:
    KKSDaemon(int argc, char **argv);
    ~KKSDaemon();

    bool paused() const {return bPause;}
    bool stopped() const {return bNeedExit;}

protected:

    void start();
    void stop();
    void pause();
    void resume();
    void processCommand(int code);

private slots:
    void analyzeDb();

private:

    friend class DDocServerListener;
    friend class DDocStreamsGenerator;
    friend class DDocHandlersSheduler;

    bool readSettings();

    bool bNeedExit;
    bool bPause;
    bool bNeedAnalyzeDb; //���� ���������� ������������� ������ ��������� �������������� �������� ���������� �� ��������
    bool bNeedGenerateStreams; //���� ���������� ������������� ������� ��������� ��������� �������
    bool bNeedCaptureNotify;//������ �� ��������� � ��������� � ������� ��������� �� ��������� ������
    bool bNeedHandlersShedule;//���� ���������� ������������� ������� �������� �� ����������
    
    KKSPGDatabase * dbNotify;//��� ��������� �� � ��������
    KKSPGDatabase * dbTimer;//��� �������������� �������� ���������� �� ��������
    KKSPGDatabase * dbStreams; //��� ��������� ������� ���������
    KKSPGDatabase * dbSheduledHandlers; //��� ������� �������� �� ����������
    
    DDocServerListener * listener;
    DDocStreamsGenerator * streamsGenerator;
    DDocHandlersSheduler * handlersSheduler;

    QHttp * http;

    QTimer m_timer;

    QFile * fLog;
    QTextStream * fLogOut;

    QString ipServer;
    QString database;
    QString port;
    QString user;
    QString passwd;

    int m_timerInterval;

    QString sPgPass;
    QString sPsqlPath;
};


#endif
