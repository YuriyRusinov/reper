#ifndef KKS_NOTIFY_RECEIVER_H
#define KKS_NOTIFY_RECEIVER_H

#include <QThread>
#include <QProcess>
#include "pqnotify.h"
#include "kksdatabase.h"

class KKSCoreApplication;

class KKSNotifyReceiver : public QThread, public IKKSListener
{
    Q_OBJECT
public:
    KKSNotifyReceiver(KKSDatabase * db, QObject * parent):QThread(parent), m_db(NULL), m_parent(NULL){m_db = db;}
    ~KKSNotifyReceiver(){}

    void run();
    void notify( char* notify_name, char * payload );
    void setKKSCoreApp(KKSCoreApplication * a){m_parent = a;}

signals:
    void databaseNotifyReceived(const QString & nName, const QString & tableName, const QString & idRecord);
private:
    KKSDatabase * m_db;
    KKSCoreApplication * m_parent;
};

#endif
