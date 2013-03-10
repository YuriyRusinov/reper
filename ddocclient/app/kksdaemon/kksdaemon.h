#ifndef KKS_DAEMON_H
#define KKS_DAEMON_H

#include <QCoreApplication>
//#include <QtGui/QDesktopWidget>
//#include <QtGui/QLabel>
#include <QtCore/QDir>
#include <QThread>
#include <QtCore/QSettings>
#include "qtservice.h"
#include "pqnotify.h"
#include "kkspgdatabase.h"

class KKSDaemon;

class DDocServerListener : public QThread, public IKKSListener
{
public:
    DDocServerListener(KKSPGDatabase * db, QObject * parent):QThread(parent), m_db(NULL), m_parent(NULL){m_db = db;}
    ~DDocServerListener(){}

    void run();
    void notify( char* notify_name, char * payload );
    void setDaemon(KKSDaemon * d){m_parent = d;}


private:
    KKSPGDatabase * m_db;
    KKSDaemon * m_parent;
};

class KKSDaemon : public QtService<QCoreApplication>
{
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


private:

    friend class DDocServerListener;
    void readSettings();

    bool bNeedExit;
    bool bPause;
    KKSPGDatabase * db;
    DDocServerListener * listener;

    QString ipServer;
    QString database;
    QString port;
    QString user;
    QString passwd;

    QString sPgPass;
    QString sExport;
};


#endif