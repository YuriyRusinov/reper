#include "kksdaemon.h"

#include <kksdatabase.h>
#include <QSettings>
#include <QUrl>
#include <QApplication>

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
    bNeedGenerateStreams = false;
    bNeedCaptureNotify = false;
    bNeedHandlersShedule = false;
    
    dbNotify = NULL;
    dbTimer = NULL;
    dbStreams = NULL;
    dbSheduledHandlers = NULL;

    listener = NULL;
    streamsGenerator = NULL;
    handlersSheduler = NULL;

    m_timerInterval = 60000;//1 min

#ifdef WIN32
    sPgPass = QString("pgpass.conf");
#else
    sPgPass = QString("./.pgpass");
#endif

    http = new QHttp(this);
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

    if(dbNotify){
        dbNotify->disconnect(false);
        delete dbNotify;
    }

    if(dbTimer){
        dbTimer->disconnect(false);
        delete dbTimer;
    }

    if(dbStreams){
        dbStreams->disconnect(false);
        delete dbStreams;
    }

    if(dbSheduledHandlers){
        dbSheduledHandlers->disconnect(false);
        delete dbSheduledHandlers;
    }

    if(listener){
        listener->quit();
        delete listener;
    }

    if(streamsGenerator){
        streamsGenerator->quit();
        delete streamsGenerator;
    }

    if(handlersSheduler){
        handlersSheduler->quit();
        delete handlersSheduler;
    }

}

void KKSDaemon::start()
{
    (*fLogOut) << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm.ss") << " --- " << "KKSDaemon started\n";
    fLogOut->flush();
    
    dbNotify = new KKSPGDatabase(); 
    dbTimer = new KKSPGDatabase();
    dbStreams = new KKSPGDatabase();
    dbSheduledHandlers = new KKSPGDatabase();

    if(!readSettings()){
        (*fLogOut) << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm.ss") << " --- " << "Cannot open config file! Exited!\n";
        fLogOut->flush();

        return;
    }

    bool ok = false;
    
    if(bNeedCaptureNotify){
        ok = dbNotify->connect(ipServer, database, user, passwd, port);
        if(!ok)
            return;
    }

    if(bNeedAnalyzeDb){
        ok = dbTimer->connect(ipServer, database, user, passwd, port);
        if(!ok)
            return;
    }

    if(bNeedGenerateStreams){
        ok = dbStreams->connect(ipServer, database, user, passwd, port);
        if(!ok)
            return;
    }

    if(bNeedHandlersShedule){
        ok = dbSheduledHandlers->connect(ipServer, database, user, passwd, port);
        if(!ok)
            return;
    }

    QFile file(QString("%1").arg(sPgPass));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "*:*:*:" << user << ":" << passwd << "\n";



    if(bNeedCaptureNotify){
        dbNotify->startListen();

        listener = new DDocServerListener(dbNotify, 0);
        listener->setDaemon(this);
        listener->start();
    }

    if(bNeedGenerateStreams){
        streamsGenerator = new DDocStreamsGenerator(dbStreams, 0);
        streamsGenerator->setDaemon(this);
        streamsGenerator->start();
    }

    if(bNeedHandlersShedule){
        handlersSheduler = new DDocHandlersSheduler(dbSheduledHandlers, 0);
        handlersSheduler->setDaemon(this);
        handlersSheduler->start();
    }

    if(bNeedAnalyzeDb){
        m_timer.setInterval(m_timerInterval);
        connect(&m_timer, SIGNAL(timeout()), this, SLOT(analyzeDb()));
        m_timer.start();
    }
    
}

/* автоматическая разводка документов по рубрикам и выдача очередного периодического распоряжения 
(анализ журнала распоряжений на предмет наличия периодических)*/
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

bool KKSDaemon::readSettings()
{
    //QSettings * s = new QSettings("DynamicSoft" , "KKSDaemon Service 1.2.0");
    QString appPath = QApplication::applicationDirPath();
    QString fileName = appPath + "/kksdaemon.conf";
    QFile f(fileName);
    //if(!f.exists()){
    //    return false;
    //}
    QFileInfo fi(f);

    QSettings * s = new QSettings(fi.absoluteFilePath(), QSettings::IniFormat);
    //s->readSettings ( );

    //s->beginGroup ("System settings/Database");
    ipServer = s->value("ip").toString();
    database = s->value("database").toString();
    port = s->value("port").toString();
    user = s->value("user").toString();
    passwd = s->value("passwd").toString();
    sPsqlPath = s->value("psqlPath").toString();

    m_timerInterval = s->value("timerInterval").toInt();
    
    bNeedGenerateStreams = s->value("generateStreams").toBool();
    bNeedAnalyzeDb = s->value("autoRubrication").toBool();
    bNeedCaptureNotify = s->value("processServices").toBool();
    bNeedHandlersShedule = s->value("sheduleHandlers").toBool();


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
        sPsqlPath = QString("/opt/ddocserver/bin/psql");
#endif
        s->setValue("psqlPath", sPsqlPath);
    }

    if(m_timerInterval <= 0){
        m_timerInterval = 60000;
        s->setValue("timerInterval", m_timerInterval);
    }

    if(s->value("generateStreams").isNull()){
        bNeedGenerateStreams = true;
        s->setValue("generateStreams", true);
    }

    if(s->value("sheduleHandlers").isNull()){
        bNeedHandlersShedule = true;
        s->setValue("sheduleHandlers", true);
    }

    if(s->value("autoRubrication").isNull()){
        bNeedAnalyzeDb = true;
        s->setValue("autoRubrication", true);
    }

    if(s->value("processServices").isNull()){
        bNeedCaptureNotify = true;
        s->setValue("processServices", true);
    }

    //s->endGroup (); // Database

    delete s;

    return true;
}

void DDocStreamsGenerator::run()
{
    if(!m_db)
        return;

    
    while(1){

        if(m_parent && m_parent->stopped())
            break;
        if(m_parent && m_parent->paused())
            continue;

        QString sql = QString("select getMessageStreams()");
        KKSResult * res = m_db->execute(sql.toLocal8Bit().constData());
        if(!res)
            continue;

        if(res->getRowCount() != 1){
            delete res;
            continue;
        }

        int r = res->getCellAsInt(0, 0);
        delete res;
        if(r != 1)
            continue;

    }
    
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

    //int idHandler = res->getCellAsInt(0, 0);
    QString name = res->getCellAsString(0, 1);
    QString service = res->getCellAsString(0, 3);
    QString extraParams = res->getCellAsString(0, 4);
    bool isExternal = res->getCellAsBool(0, 5);
    QString hHost = res->getCellAsString(0, 6);
    int hPort = res->getCellAsInt(0, 7);

    delete res;

    if(isExternal){

        if(hHost.isEmpty() && service.isEmpty())
            return; //нечего запускать
        
        QString uri = QString("http://") + hHost + QString(":") + QString::number(hPort) + QString("/") + service;
	    //QString uri = QString("/") + service + QString("?id=") + QString::number(id); // + extraParams;
	    QUrl u(uri);


        QHttp::ConnectionMode mode = QHttp::ConnectionModeHttp;
        m_parent->http->setHost(u.host(), mode, u.port());
    
        //QByteArray path = QUrl::toPercentEncoding(u.path(), "!$&'()*+,;=:@/");
        QByteArray path = QUrl::toPercentEncoding(u.path(), "!$&'()*+,;=:@/");
        path += QString("?id=") + QString::number(id); // + extraParams;
        
        QHttpRequestHeader h = QHttpRequestHeader("GET", path);
        h.setValue("Host", u.host());
        h.setValue("Port", QString::number(u.port()));
        

	    ////QEventLoop eventLoop;
	    ////connect(m_parent->http,SIGNAL(requestFinished(int,bool)),&eventLoop,SLOT(quit()));
        //httpGetId = http->post ( path, byteArray ) ;
        //int uid = m_parent->http->get(path);
        //int uid = 
        m_parent->http->request(h);
	    ////eventLoop.exec();

        //qWarning() << "!!!!!!!  " << uid;
        
        //qint64 pid = 0;
        //QStringList arguments;
        //arguments << QString("%1").arg(id);
        //QProcess::startDetached ( service, arguments, ".", &pid );
        //if(pid <= 0){
        //    qWarning() << "Program " << service << " cannot start!";
        //}
    }
    else{

        if(service.isEmpty())
            return; //нечего запускать. Имя хранимой процедуры не задано
        
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


void DDocHandlersSheduler::run()
{
    if(!m_db)
        return;

    
    //QString processedEntries;
    while(1){

        if(m_parent && m_parent->stopped())
            break;
        if(m_parent && m_parent->paused())
            continue;

        QString sql;
        //if(processedEntries.isEmpty())
            sql = QString("select * from getSheduledHandlers()");
        //else
        //    sql = QString("select * from getSheduledHandlers() where id_shedule not in (%1)");

        KKSResult * res = m_db->execute(sql.toLocal8Bit().constData());
        if(!res)
            continue;

        int cnt = res->getRowCount();
        if(cnt <= 0){
            delete res;
            continue;
        }

        for(int row=0; row<cnt; row++){

            Service srv;
            srv.idShedule = res->getCellAsInt64(row, 0);
            srv.id = res->getCellAsInt64(row, 1);
            srv.name = res->getCellAsString(row, 2);
            srv.signature = res->getCellAsString(row, 3);
            srv.host = res->getCellAsString(row, 4);
            srv.port = res->getCellAsInt(row, 5);
            srv.extraParams = res->getCellAsString(row, 6);
            srv.inData = res->getCellAsString(row, 7);
            srv.isExternal = res->getCellAsBool(row, 8);

            //if(processedEntries.isEmpty())
            startService(srv);
        }

        delete res;
    }
    
}

void DDocHandlersSheduler::startService(const Service & srv)
{
    if(srv.id <= 0)
        return;


    if(srv.isExternal){
        
        QString uri = QString("http://") + srv.host + QString(":") + QString::number(srv.port) + QString("/") + srv.signature;
	    //QString uri = QString("/") + service + QString("?id=") + QString::number(id); // + extraParams;
	    QUrl u(uri);


        QHttp::ConnectionMode mode = QHttp::ConnectionModeHttp;
        m_parent->http->setHost(u.host(), mode, u.port());
    
        //QByteArray path = QUrl::toPercentEncoding(u.path(), "!$&'()*+,;=:@/");
        QByteArray path = QUrl::toPercentEncoding(u.path(), "!$&'()*+,;=:@/");
        //path += QString("?id=") + QString::number(srv.id); // + extraParams;
        
        QHttpRequestHeader h = QHttpRequestHeader("GET", path);
        h.setValue("Host", u.host());
        h.setValue("Port", QString::number(u.port()));
        

	    ////QEventLoop eventLoop;
	    ////connect(m_parent->http,SIGNAL(requestFinished(int,bool)),&eventLoop,SLOT(quit()));
        //httpGetId = http->post ( path, byteArray ) ;
        //int uid = m_parent->http->get(path);
        //int uid = 
        m_parent->http->request(h);
	    ////eventLoop.exec();

        //qWarning() << "!!!!!!!  " << uid;
        
        //qint64 pid = 0;
        //QStringList arguments;
        //arguments << QString("%1").arg(id);
        //QProcess::startDetached ( service, arguments, ".", &pid );
        //if(pid <= 0){
        //    qWarning() << "Program " << service << " cannot start!";
        //}
    }
    else{
        
        QString sql = QString("\"select * from hStartSheduledHandler(%1, '%2', %3)\"")
                                    .arg(srv.idShedule)
                                    .arg(srv.signature)
                                    .arg(srv.inData.isEmpty() ? QString("NULL") : QString("'" + srv.inData + "'"));
        
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

