#include <QSettings>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>

#include <kksdebug.h>
#include "ddocinteractorbase.h"

#include "ddocinteractorclient.h"
#include "ddocinteractorserver.h"

DDocInteractorBase::DDocInteractorBase(QObject * parent) 
   : QObject(parent),
   m_server(NULL),
   m_client(NULL),
   m_started(false),
   m_isExiting(false)
{
    msgForSent = 0;
    filesForSent = 0;
    cntFilesSended = 0;
    cntMsgSended = 0;
    cntFilePartsSended = 0;
    filePartsForSent = 0;
    cntPingsSended = 0;
    pingsForSent = 0;
}

DDocInteractorBase::~DDocInteractorBase()
{
    if(m_server){
        m_server->quit();
        delete m_server;
    }

    if(m_client){
        m_client->quit();
        delete m_client;
    }
}

int DDocInteractorBase::start(bool mode, int interval)
{
    if(m_started)
        return 1;

    QSettings settings (QCoreApplication::applicationDirPath ()+"/http.ini", QSettings::IniFormat);

    QString dbName;//(settings.value("Database/database", "tsync_db1").toString());
    QString host;//(settings.value("Database/host", "192.168.17.176").toString());
    QString user;//(settings.value("Database/user", "jupiter").toString());
    QString password;//(settings.value("Database/password", "111").toString());
    int port;//(settings.value("Database/port", "5432").toInt());


    int transport;//(settings.value("Http/transport","1").toInt());  

    int server_port;//(settings.value("Http/server_port", "6000").toInt());
      
    settings.beginGroup ("Database");
    dbName = settings.value ("database").toString ();
    host = settings.value ("host", "127.0.0.1").toString ();
    user = settings.value ("user", "jupiter").toString ();
    password = settings.value("password", "111").toString ();
    port = settings.value("port", "5432").toInt ();
    settings.endGroup ();

    settings.beginGroup("Transport");
    transport = settings.value ("transport","1").toInt ();
    settings.endGroup ();

    settings.beginGroup ("Http");
  
    server_port = settings.value ("server_port", "8080").toInt ();
    
    QString gatewayHost = settings.value ("host", "").toString();
    int gatewayPort = settings.value ("port", "0").toInt();

    settings.endGroup ();


    m_loader = new (std::nothrow) JKKSLoader (host, dbName, user, password, port, transport);
    if (m_loader && m_loader->connectToDb ())
    {
        kksInfo() << tr("Successfully connected to database. Host = %1:%2, database = %3, user = %4, transport ID = %5").arg(host).arg(port).arg(dbName).arg(user).arg(transport);
    }
    else if (m_loader)
    {
        delete m_loader;
        qCritical() << tr("Unable to connect to the database: %1")
                              .arg(host);
        QMessageBox::critical(0, tr("DynamicDocs Interactor"),
                              tr("Unable to connect to the database: %1")
                              .arg(host));
        return 0;
    }
    
    m_client = new DDocInteractorClient(m_loader, this);
    connect(m_client, SIGNAL(finished()), m_client, SLOT(deleteLater()));
    m_client->setMode(mode);
    m_client->setInterval(interval);
    m_client->setGateway(gatewayHost, gatewayPort);
    
    if(mode){//manual
        connect(this, SIGNAL(startSending()), m_client, SLOT(startProc()));
        connect(m_client, SIGNAL(sendingStarted()), this, SIGNAL(sendingStarted()));
        connect(m_client, SIGNAL(sendingCompleted()), this, SIGNAL(sendingCompleted()));
        emit showManualStartButton();
    }
    else{
        emit hideManualStartButton();
    }

    connect(m_client, SIGNAL(pingsSended(QMap<QString, JKKSPing>)), this, SIGNAL(pingsSended(QMap<QString, JKKSPing>)));
    connect(m_client, SIGNAL(pingsSentCompleted()), this, SIGNAL(pingsSentCompleted()));
    connect(this, SIGNAL(refreshTimer(int)), m_client, SLOT(slotRefreshTimer(int)));
   
    connect(this, SIGNAL(theSignal()), m_client, SLOT(init()));
    m_client->start();
    emit theSignal();
    disconnect(this, SIGNAL(theSignal()), m_client, SLOT(init()));

    m_server = new DDocInteractorServer(m_loader, this);
    connect(m_server, SIGNAL(finished()), m_server, SLOT(deleteLater()));
    m_server->setPort(server_port);
    connect(m_server, SIGNAL(pingsSended(QMap<QString, JKKSPing>)), this, SIGNAL(pingsSended(QMap<QString, JKKSPing>)));
    connect(m_server, SIGNAL(pingsSentCompleted()), this, SIGNAL(pingsSentCompleted()));

    connect(this, SIGNAL(theSignal()), m_server, SLOT(init()));
    m_server->start();
    emit theSignal();
    disconnect(this, SIGNAL(theSignal()), m_server, SLOT(init()));

    
    if(!m_server->successed()){
        qCritical() << tr("Unable to start the server: %1")
                              .arg(m_server->errorString());
        QMessageBox::critical(0, tr("DynamicDocs Interactor "),
                              tr("Unable to start the server: %1")
                              .arg(m_server->errorString()));

        m_server->quit();
        m_client->quit();
        delete m_client;
        delete m_server;

        return 0;
    }

    m_started = true;

    return 1;
}

void DDocInteractorBase::slotExitThreads()
{
    if(!m_server && !m_client)
        return;

    m_isExiting = true;
    m_client->m_isExiting = true;
    m_server->m_isExiting = true;

    emit exitThreads();

    m_server->quit();
    m_client->quit();
    
    m_server->wait();
    m_client->wait();

    delete m_server;
    delete m_client;
    
    m_server = NULL;
    m_client = NULL;

}

void DDocInteractorBase::slotStartClient()
{
    //для этого достаточно активизировать таймер
    if(!m_client)
        return;

    if(!m_client->manual)//если и так в авто-режиме, ничего запускать не надо
        return;

    m_client->manual = false;
    disconnect(this, SIGNAL(startSending()), m_client, SLOT(startProc()));
    disconnect(m_client, SIGNAL(sendingStarted()), this, SIGNAL(sendingStarted()));
    disconnect(m_client, SIGNAL(sendingCompleted()), this, SIGNAL(sendingCompleted()));
    
    if(!m_client->m_timer)
        m_client->createTimer();
    else
        m_client->m_timer->start();
}

void DDocInteractorBase::slotStopClient()
{
    //для этого достаточно остановить таймер и перевести поток в ручной режим
    if(!m_client)
        return;

    m_client->manual = true; //выставление этого параметра в true приведет к тому, что даже если таймер уже остановлен для отправки данных, он не запустится вновь
    if(m_client->m_timer)
        m_client->m_timer->stop();

    connect(this, SIGNAL(startSending()), m_client, SLOT(startProc()));
    connect(m_client, SIGNAL(sendingStarted()), this, SIGNAL(sendingStarted()));
    connect(m_client, SIGNAL(sendingCompleted()), this, SIGNAL(sendingCompleted()));

}
