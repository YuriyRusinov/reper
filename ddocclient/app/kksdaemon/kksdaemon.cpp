#include "kksdaemon.h"

#include <kksdatabase.h>
#include <QSettings>

#include <QtDebug>
#include <QFile>
#include <QProcess>

KKSDaemon::KKSDaemon(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "KKSDaemon Service")
{
    setServiceDescription("KKSDaemon service for DynamicDocs Server");
    setServiceFlags(QtServiceBase::CanBeSuspended);

    bNeedExit = false;
    bPause = false;
    db = NULL;
    listener = NULL;

#ifdef WIN32
    sPgPass = QString(".pgpass");
    sExport = QString("export");
#else
    sPgPass = QString("pgpass.conf");
    sExport = QString("set");
#endif
}

KKSDaemon::~KKSDaemon()
{
    if(db){
        db->disconnect();
        delete db;
    }

    if(listener){
        listener->quit();
        delete listener;
    }
}

void KKSDaemon::start()
{
    //QCoreApplication * app = application();
    db = new KKSPGDatabase(); 
    
    readSettings();

    bool ok = db->connect(ipServer, database, user, passwd, port);
    if(!ok)
        return;



     QFile file(QString("./%1").arg(sPgPass));
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

     QTextStream out(&file);
     out << "*:*:*:" << user << ":" << passwd << "\n";
     

    db->startListen();

    listener = new DDocServerListener(db, 0);
    listener->setDaemon(this);
    listener->start();
    
}

void KKSDaemon::stop()
{
    bNeedExit = true;
}

void KKSDaemon::pause()
{
    bPause = true;
}

void KKSDaemon::resume()
{
    bPause = false;
}

void KKSDaemon::processCommand(int code)
{
    if(code == 0){
    }
}

void KKSDaemon::readSettings()
{
    QSettings * s = new QSettings("DynamicSoft" , "KKSDaemon Service 1.0.0");
    //s->readSettings ( );

    s->beginGroup ("System settings/Database");
    ipServer = s->value("ip").toString();
    database = s->value("database").toString();
    port = s->value("port").toString();
    user = s->value("user").toString();
    passwd = s->value("passwd").toString();


    if(ipServer.isEmpty()){
        ipServer = "192.168.0.3";
        s->setValue("ip", ipServer);
    }
    if(database.isEmpty()){
        database = "t83_6";
        s->setValue("database", database);
    }
    if(port.isEmpty()){
        port = "5432";
        s->setValue("port", port);
    }
    if(user.isEmpty()){
        user = "admin";
        s->setValue("user", user);
    }
    if(passwd.isEmpty()){
        passwd = "111";
        s->setValue("passwd", passwd);
    }

    s->endGroup (); // Database

}

void DDocServerListener::run()
{
    if(!m_db)
        return;

    m_db->addListener(this, "chains_data");

    while(1){
        if(m_parent && m_parent->stopped())
            break;
        if(m_parent && m_parent->paused())
            continue;

        m_db->checkNotifies();
    }
}

void DDocServerListener::notify( char* notify_name, char * payload )
{
    if(!m_db || !payload)
        return;

    if(strcmp(notify_name, "chains_data") != 0)
        return;

    QString p = QString("%1").arg(payload);
    int id = p.toInt();

    QString sql = QString("hGetHandlerInfo(%1)").arg(id); //также эта функция щсуществляет изменение соответствующего поля handled_time
    KKSResult * res = m_db->execute(sql.toLocal8Bit().constData());
    if(!res)
        return;
    if(res->getRowCount() != 1){
        delete res;
        return;
    }

    int idHandler = res->getCellAsInt(0, 0);
    QString name = res->getCellAsString(0, 1);
    QString service = res->getCellAsString(0, 3);
    QString extraParams = res->getCellAsString(0, 4);
    bool isExternal = res->getCellAsBool(0, 5);

    delete res;

    if(isExternal){
        qint64 pid = 0;
        QStringList arguments;
        arguments << QString("%1").arg(id);
        QProcess::startDetached ( service, arguments, ".", &pid );
        if(pid <= 0){
            qWarning() << "Program " << service << " cannot start!";
        }
    }
    else{
        QStringList arguments;
        QString argument = QString("-h %1 -p %2 -U %3 -c \"select hStartHandler('%4', %5)\" %6")
                               .arg(m_parent->ipServer)
                               .arg(m_parent->port)
                               .arg(m_parent->user)
                               .arg(service)
                               .arg(id)
                               .arg(m_parent->database);
        arguments << argument;

        qint64 pid = 0;
        QProcess::startDetached ( QString("%1 PGPASSFILE=./%2 & psql").arg(m_parent->sExport).arg(m_parent->sPgPass), 
                                  arguments, 
                                  ".", 
                                  &pid );
        if(pid <= 0){
            qWarning() << "Program " << service << " cannot start!";
        }
        QString sql = QString("select hStartHandler('%1', %2)").arg(service).arg(id);
        m_db->execute(sql.toLocal8Bit().constData());
    }

   
}
