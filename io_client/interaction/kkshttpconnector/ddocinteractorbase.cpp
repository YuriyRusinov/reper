#include <QSettings>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>

#include <kksdebug.h>
#include "ddocinteractorbase.h"

#include "ddocinteractorclient.h"
#include "ddocinteractorclientforxml.h"
#include "ddocinteractorserver.h"

DDocInteractorBase::DDocInteractorBase(QObject * parent) 
   : QObject(parent),
   m_server(NULL),
   m_client(NULL),
   m_clientForXML(NULL),
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

    if(m_clientForXML){
        m_clientForXML->quit();
        delete m_clientForXML;
    }
}

int DDocInteractorBase::start(bool mode, int interval, bool bMain, bool bXML)
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

    int waitClientConnectionTimeout = 1000; //тайм-аут для функции QTcpSocket::waitForReadyRead()
                                            //задается в миллисекундах
                                            // по умолчанию 1000
      
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

    waitClientConnectionTimeout = settings.value("server_timeout", "1000").toInt();//in msec

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
    if(mode || !bMain)
        m_client->setManual(true); //true - is manual mode
    else{
        m_client->setManual(false);//если сказано не запускать поток опроса основной очереди (галочка не стоит), то считаем, что она работает в ручном режиме
    }

    m_client->setInterval(interval);
    m_client->setGateway(gatewayHost, gatewayPort);
    
    if(mode || !bMain){//manual
        connect(this, SIGNAL(startSending()), m_client, SLOT(startProc()));
        connect(m_client, SIGNAL(sendingStarted()), this, SIGNAL(sendingStarted()));
        connect(m_client, SIGNAL(sendingCompleted()), this, SIGNAL(sendingCompleted()));
        //emit showManualStartButton();
    }
    else{
        //emit hideManualStartButton();
    }

    connect(m_client, SIGNAL(pingsSended(QMap<QString, JKKSPing>)), this, SIGNAL(pingsSended(QMap<QString, JKKSPing>)));
    connect(m_client, SIGNAL(pingsSentCompleted()), this, SIGNAL(pingsSentCompleted()));
    connect(this, SIGNAL(refreshTimer(int)), m_client, SLOT(slotRefreshTimer(int)));
   
    connect(this, SIGNAL(theSignal()), m_client, SLOT(init()));
    m_client->start();
    emit theSignal();
    disconnect(this, SIGNAL(theSignal()), m_client, SLOT(init()));

/****/
    m_clientForXML = new DDocInteractorClientForXML(m_loader, this);
    connect(m_clientForXML, SIGNAL(finished()), m_clientForXML, SLOT(deleteLater()));
    if(mode || !bXML)
        m_clientForXML->setManual(true); //true - is manual mode
    else{
        m_clientForXML->setManual(false);//если сказано не запускать поток опроса основной очереди (галочка не стоит), то считаем, что она работает в ручном режиме
    }
    m_clientForXML->setInterval(interval);
    m_clientForXML->setGateway(gatewayHost, gatewayPort);
    
    if(mode || !bXML){//manual
        connect(this, SIGNAL(startSendingForXML()), m_clientForXML, SLOT(startProc()));
        connect(m_clientForXML, SIGNAL(sendingStarted()), this, SIGNAL(sendingStarted()));
        connect(m_clientForXML, SIGNAL(sendingCompleted()), this, SIGNAL(sendingCompleted()));
        //emit showManualStartButton();
    }
    else{
        //emit hideManualStartButton();
    }

    if(mode)// || !(bXML && bMain)){
        emit showManualStartButton();
    else
        emit hideManualStartButton();

    connect(this, SIGNAL(refreshTimer(int)), m_clientForXML, SLOT(slotRefreshTimer(int)));
   
    connect(this, SIGNAL(theSignal()), m_clientForXML, SLOT(init()));
    m_clientForXML->start();
    emit theSignal();
    disconnect(this, SIGNAL(theSignal()), m_clientForXML, SLOT(init()));

/****/
    m_server = new DDocInteractorServer(m_loader, this);
    m_server->setWaitClientConnectionTimeout(waitClientConnectionTimeout);
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
        m_clientForXML->quit();
        delete m_client;
        delete m_clientForXML;
        delete m_server;

        return 0;
    }

    m_started = true;

    return 1;
}

void DDocInteractorBase::slotExitThreads()
{
    if(!m_server && !m_client && !m_clientForXML)
        return;

    m_isExiting = true;

    m_client->m_isExiting = true;
    m_clientForXML->m_isExiting = true;
    m_server->m_isExiting = true;

    emit exitThreads();

    m_server->quit();
    m_client->quit();
    m_clientForXML->quit();
    
    m_server->wait();
    m_client->wait();
    m_clientForXML->wait();

    delete m_server;
    delete m_client;
    delete m_clientForXML;
    
    m_server = NULL;
    m_client = NULL;
    m_clientForXML = NULL;

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

void DDocInteractorBase::slotStartClientForXML()
{
    //для этого достаточно активизировать таймер
    if(!m_clientForXML)
        return;

    if(!m_clientForXML->manual)//если и так в авто-режиме, ничего запускать не надо
        return;

    m_clientForXML->manual = false;
    disconnect(this, SIGNAL(startSending()), m_clientForXML, SLOT(startProc()));
    disconnect(m_clientForXML, SIGNAL(sendingStarted()), this, SIGNAL(sendingStarted()));
    disconnect(m_clientForXML, SIGNAL(sendingCompleted()), this, SIGNAL(sendingCompleted()));
    
    if(!m_clientForXML->m_timer)
        m_clientForXML->createTimer();
    else
        m_clientForXML->m_timer->start();
}

void DDocInteractorBase::slotStopClientForXML()
{
    //для этого достаточно остановить таймер и перевести поток в ручной режим
    if(!m_clientForXML)
        return;

    m_clientForXML->manual = true; //выставление этого параметра в true приведет к тому, что даже если таймер уже остановлен для отправки данных, он не запустится вновь
    if(m_clientForXML->m_timer)
        m_clientForXML->m_timer->stop();

    connect(this, SIGNAL(startSending()), m_clientForXML, SLOT(startProc()));
    connect(m_clientForXML, SIGNAL(sendingStarted()), this, SIGNAL(sendingStarted()));
    connect(m_clientForXML, SIGNAL(sendingCompleted()), this, SIGNAL(sendingCompleted()));

}
