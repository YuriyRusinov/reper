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

    void readSettings();

    bool bNeedExit;
    bool bPause;
    bool bNeedAnalyzeDb; //флаг определяет необходимость работы механизма автоматической разводки документов по рубрикам
    bool bNeedGenerateStreams; //флаг определяет необходимость запуска механизма генерации потоков
    
    KKSPGDatabase * db;
    KKSPGDatabase * dbTimer;//для автоматической разводки документов по рубрикам
    KKSPGDatabase * dbStreams; //для генерации потоков сообщений
    
    DDocServerListener * listener;
    DDocStreamsGenerator * streamsGenerator;

    QHttp http;

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
