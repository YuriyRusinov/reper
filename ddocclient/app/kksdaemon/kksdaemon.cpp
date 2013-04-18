#include "kksdaemon.h"

#include <kksdatabase.h>
#include <QSettings>
#include <QUrl>

#include <QtDebug>


KKSDaemon::KKSDaemon(int argc, char **argv) : 
    QObject(),
    QtService<QCoreApplication>(argc, argv, "KKSDaemon Service")
{
    setServiceDescription("KKSDaemon service for DynamicDocs Server");
    setServiceFlags(QtServiceBase::CanBeSuspended);

    fLog = new QFile("kksdaemon.log");
    if(!fLog->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    fLogOut = new QTextStream(fLog);
    //fLogOut.setDevice(fLog);

    bNeedExit = false;
    bPause = false;
    db = NULL;
	dbTimer = NULL;
    listener = NULL;
	m_timerInterval = 60000;//1 min

#ifdef WIN32
    sPgPass = QString("pgpass.conf");
#else
    sPgPass = QString("./.pgpass");
#endif
}

KKSDaemon::~KKSDaemon()
{
    if(fLogOut){
        delete fLogOut;
    }
    if(fLog){
        fLog->close();
        delete fLog;
    }

    if(db){
        db->disconnect();
        delete db;
    }

    if(dbTimer){
		dbTimer->disconnect();
		delete dbTimer;
    }

	if(listener){
        listener->quit();
        delete listener;
    }
}

void KKSDaemon::start()
{
    (*fLogOut) << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm.ss") << " --- " << "KKSDaemon started\n";
    fLogOut->flush();
    //QCoreApplication * app = application();
    db = new KKSPGDatabase(); 
    dbTimer = new KKSPGDatabase();

    readSettings();

    bool ok = db->connect(ipServer, database, user, passwd, port);
    if(!ok)
        return;

    ok = dbTimer->connect(ipServer, database, user, passwd, port);
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

	//m_timer.setInterval(m_timerInterval);
	//connect(&m_timer, SIGNAL(timeout()), this, SLOT(analyzeDb()));
	//m_timer.start();
    
}

void KKSDaemon::analyzeDb()
{
	(*fLogOut) << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm.ss") << " --- " << "Start analyzing database...\n";
    fLogOut->flush();

    QString sql = QString("select cmdanalyzejournal()");
	KKSResult * res = dbTimer->execute(sql.toLocal8Bit().constData());
	if(!res)
		return;

	if(res->getRowCount() != 1){
		delete res;
		return;
	}

	int r = res->getCellAsInt(0, 0);
	delete res;
	if(r != 1)
		return;

	sql = QString("select putIOIntoRubric()");
	res = dbTimer->execute(sql.toLocal8Bit().constData());
	if(!res)
		return;

	if(res->getRowCount() != 1){
		delete res;
		return;
	}

	r = res->getCellAsInt(0, 0);
	delete res;
	if(r != 1)
		return;

    (*fLogOut) << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm.ss") << " --- " << "Successfully analyzed\n";
    fLogOut->flush();
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

	m_timerInterval = s->value("timerInterval").toInt();


    if(ipServer.isEmpty()){
        ipServer = "127.0.0.1";
        s->setValue("ip", ipServer);
    }
    if(database.isEmpty()){
        database = "main_db";
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

    if(sPsqlPath.isEmpty()){
#ifdef WIN32
        sPsqlPath = QString("\"C:\\Program Files\\PostgreSQL\\9.2\\bin\\psql.exe\"");
#else
        sPsqlPath = QString("/opt/postgresql/bin/psql");
#endif
        s->setValue("psqlPath", sPsqlPath);
    }

	if(m_timerInterval <= 0){
		m_timerInterval = 60000;
		s->setValue("timerInterval", m_timerInterval);
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

    QString sql = QString("select * from hGetHandlerInfo(%1)").arg(id); //также эта функция осуществляет изменение соответствующего поля handled_time
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
    QString hHost = res->getCellAsString(0, 6);
    int hPort = res->getCellAsInt(0, 7);

    delete res;

    if(isExternal){
        
        QString uri = QString("http://") + hHost + QString(":") + QString::number(hPort) + QString("/") + service + QString("?") + QString::number(id); // + extraParams;
        m_parent->http.setHost(hHost, hPort);
        int uid = m_parent->http.get(QUrl::toPercentEncoding(uri));
        
        //qint64 pid = 0;
        //QStringList arguments;
        //arguments << QString("%1").arg(id);
        //QProcess::startDetached ( service, arguments, ".", &pid );
        //if(pid <= 0){
        //    qWarning() << "Program " << service << " cannot start!";
        //}
    }
    else{
        
        QString sql = QString("\"select hStartHandler('%1', %2)\"").arg(service).arg(id);
        
        QString program = QString("%1 -h %2 -p %3 -U %4 -c %5 %6")
                           .arg(m_parent->sPsqlPath)
                           .arg(m_parent->ipServer)
                           .arg(m_parent->port)
                           .arg(m_parent->user)
                           .arg(sql)
                           .arg(m_parent->database);

        QProcess::startDetached(program);
    }
}

