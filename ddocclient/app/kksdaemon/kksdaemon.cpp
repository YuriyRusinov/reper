#include "kksdaemon.h"

#include <kksdatabase.h>
#include <QSettings>

#include <QtDebug>
#include <QFile>

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
    sPgPass = QString("pgpass.conf");
#else
    sPgPass = QString("./.pgpass");
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



     QFile file(QString("%1").arg(sPgPass));
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
    sPsqlPath = s->value("psqlPath").toString();


    if(ipServer.isEmpty()){
        ipServer = "127.0.0.1";
        s->setValue("ip", ipServer);
    }
    if(database.isEmpty()){
        database = "t83_6";
        s->setValue("database", database);
    }
    if(port.isEmpty()){
        port = "5433";
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

    if(sPsqlPath.isEmpty()){
#ifdef WIN32
        sPsqlPath = QString("\"C:\\Program Files (x86)\\PostgreSQL9\\9.2\\bin\\psql.exe\"");
#else
        sPsqlPath = QString("/opt/postgresql/bin/psql");
#endif
        s->setValue("psqlPath", sPsqlPath);
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
    if(!m_db)
        return;
    
    if( !payload)
        return;

    if(strcmp(notify_name, "chains_data") != 0)
        return;

    QString p = QString("%1").arg(payload);
    int id = p.toInt();
    //id = 1;

    QString sql = QString("select * from hGetHandlerInfo(%1)").arg(id); //����� ��� ������� ������������ ��������� ���������������� ���� handled_time
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
        
        
        //QProcess * p = new QProcess();

        //--connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished1(int, QProcess::ExitStatus)));
        //--connect(p, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
        //--qRegisterMetaType<QProcess::ProcessState>("QProcess::ProcessState");
        //--connect(p, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(processStateChanged(QProcess::ProcessState)));

        //QStringList e = QProcess::systemEnvironment();
        //e.append(QString("PGPASSFILE=%1").arg(m_parent->sPgPass) );
        //p->setEnvironment(e);

        QString sql = QString("\"select hStartHandler('%1', %2)\"").arg(service).arg(id);
        
        QString program = QString("%1 -h %2 -p %3 -U %4 -c %5 %6")
                           .arg(m_parent->sPsqlPath)
                           .arg(m_parent->ipServer)
                           .arg(m_parent->port)
                           .arg(m_parent->user)
                           .arg(sql)
                           .arg(m_parent->database);

        //p->start(program);
        QProcess::startDetached(program);

        /*bool ok = p->waitForStarted();
        if(!ok){
            qWarning() << "Program " << service << " cannot start!";
            delete p;
            return;
        }
        Q_PID pid = p->pid();
        if(pid == 0){
            qWarning() << "Program " << service << " cannot start!";
            delete p;
            return;
        }
        */
    }
}

/*
void DDocServerListener::processFinished1(int code, QProcess::ExitStatus status)
{
    QProcess * p = qobject_cast<QProcess *>(this->sender());
    if(!p)
        return;

    p->kill();
}

void DDocServerListener::processError(QProcess::ProcessError err)
{
    QProcess * p = qobject_cast<QProcess *>(this->sender());
    if(!p)
        return;
}

void DDocServerListener::processStateChanged(QProcess::ProcessState state)
{
    QProcess * p = qobject_cast<QProcess *>(this->sender());
    if(!p)
        return;
}

*/