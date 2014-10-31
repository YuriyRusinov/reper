
#include <QtNetwork>
#include <QTimer>
#include <QCryptographicHash>


#include "ddocinteractorclientforxml.h"
#include "ddocinteractorbase.h"
#include <JKKSLoader.h>

#include <defines.h>
#include <kksdebug.h>
#include <kksclient_name.h>
#include <KKSList.h>

#include <JKKSIOUrl.h>
#include <JKKSMessage.h>
#include <JKKSPMessage.h>
#include <JKKSXMLMessage.h>


DDocInteractorClientForXML::DDocInteractorClientForXML(JKKSLoader* loader, DDocInteractorBase * parent) 
    : QThread(parent), 
      m_loader(NULL), 
      m_parent(NULL),
      http(NULL),
      manual(false),
      m_timer(NULL),
      m_interval(0),
      gatewayPort(0),
      m_isExiting(false),
      m_isSending(false)
{
    m_loader = loader; 
    m_parent = parent;
}

DDocInteractorClientForXML::~DDocInteractorClientForXML()
{
    if(m_timer){
        m_timer->stop();
        delete m_timer;
        m_timer = NULL;
    }

    wait();

    if(http)
        delete http;
}

/***************************/
/***************************/

void DDocInteractorClientForXML::run()
{
    exec();
}

void DDocInteractorClientForXML::init()
{
    if(!m_loader)
        return;
    
    http = new QHttp(this);

    //завершен запрос на передачу данных через http
    connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));

    createTimer();
}

void DDocInteractorClientForXML::httpRequestFinished(int requestId, bool error)
{
    uint messCount = httpMessages.count();
    if(messCount <= 0)
        return;
    
    QPair<qint64, qint64> defValue = QPair<qint64, qint64>();
    QPair<qint64, qint64> t = httpMessages.value(requestId, defValue);
    if(t == defValue){
        return;
    }

    kksInfo() << tr("XML Message sending request completed. Internal request ID = %1").arg(requestId);
    emit needToExitEventLoop();
    
    httpMessages.remove(requestId);

    /*if(httpMessages.count() == 0){
        if(manual == false)
            if(m_timer)
                m_timer->start();
        else
            emit sendingCompleted(); //если активен ручной (отладочный) режим - делаем доступной кнопку отправки
    }
    */
    
    bool bError = error;
    QHttpResponseHeader header = http->lastResponse();
    if(!header.isValid()){
        bError = true;
    }

    if(header.statusCode() != 200 ){ //OK code
        bError = true;
    }

    if (bError) {
        kksCritical() << tr("Message sending request failed! Internal request ID = %1").arg(requestId);
        return;
    } 
    
    //здесь мы полагаем, что если пришел ответ на сообщение с requestId из нашего перечня (и error = false), 
    //мы должны обработать тело этого сообщения
    //если тело == OK или оно пустое, то это скорее всего было взаимодействие напрямую с http_connector'ом 
    //QByteArray ba = http->readAll();
    //if(ba.length() <= 0 || ba == "OK"){//это сообщение было передано напрямую на целевой объект в http_connector
    //    if(!setMessageAsSended(t.first, t.second)){
    //        kksCritical() << tr("Cannot mark message as sended! Database Error. Message ID = %1, Message type = %2. Internal request ID = %3").arg(t.first).arg(t.second).arg(requestId);
    //    }
    //    return;
    //}
}

void DDocInteractorClientForXML::createTimer()
{
    if(manual == false){
        if(m_timer)
            return;

        m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(startProc()));
        m_timer->setInterval(m_interval);
        m_timer->start();
    }
}

void DDocInteractorClientForXML::slotRefreshTimer(int interval)
{
    if(interval < 0)
        return;

    if(m_timer)
        m_timer->setInterval(interval);
}

void DDocInteractorClientForXML::startProc()

{
    if(m_isExiting){ //если m_base завершает работу (т.е. нажали на кнопку закрытия приложения)
        return;
    }
    
    if(m_isSending)
        return;

    m_isSending = true;

    emit sendingStarted();

    if(manual == false){
        m_timer->stop();
    }
    
    QStringList receivers;
    
    messageList = m_loader->readXMLMessages(receivers);//кроме того, SQL-функция все прочитанные здесь сообщения пометит как отправленные

    if(receivers.isEmpty()){
        emit sendingCompleted();
        if(manual == false)
            m_timer->start();
        
        m_isSending = false;
        return;
    }

    if(m_isExiting){ //если m_base завершает работу (т.е. нажали на кнопку закрытия приложения)
        m_isSending = false;
        return;
    }
    
    int msgForSent = messageList.size();
    
    if(msgForSent > 0 )
    {
        kksImportantInfo() << QObject::tr("Found next %1 messages for sent").arg(msgForSent);
        httpMessages.clear();

        for (QList<JKKSPMessWithAddr *>::const_iterator iterator = messageList.constBegin();iterator != messageList.constEnd();++iterator)
        {
            //Второй параметр отвечает за то, что данные будут уходить синхронно или асинхронно. 
            //Здесь синхронно. Т.е. система будет ожидать, пока не завершится передача одного сообщения
            //прежде чем приступить к передаче следующего
            //Причины:
            //1 - Если при передаче предыдущего сообщения произошел сбой, то последующие мы переджавать не будем. Обновим соответствующий JKKSPing
            //2 - В процессе передачи теоретически может возникнуть ситуация, когда отправленное асинхронно вторым сообщение придет на приемный конец раньше первого. А это не правильно
            //3 - Задача транспорта - передать данные полностью, причем здесь важна не скорость а гарантированность доставки. И здесь асинхронный режим не быстрее синхронного
            
            if(!(*iterator))
                continue;
            
            try{
                qint64 id = (*iterator)->id;
                if(id <= 0)
                    continue;
            }
            catch(...){
                kksCritical() << QObject::tr("Found internal data corruption! Some out messages will be skipped!");
                continue;
            }
            /*bool stat = */sendOutMessage((*iterator), true /*синхронно*/);
        }

        kksImportantInfo() << QObject::tr("All found %1 messages sended! ").arg(msgForSent);

        while(!messageList.isEmpty()){
            JKKSPMessWithAddr * m = messageList.takeFirst();
            if(m)
                delete m;
            m = NULL;
        }
    }

    if(manual == true)
        emit sendingCompleted();
    else
        if(m_timer)
            m_timer->start();

    m_isSending = false;
}


/*
bool DDocInteractorClientForXML::setMessageAsSended(const qint64 & id, const int & type, bool sended)
{
    bool result = false;
    if(m_loader)
        result = m_loader->setAsSended(id, type, sended);

    return result;
}
*/

QByteArray DDocInteractorClientForXML::prepareForSend(QHttpRequestHeader & header,
                                                      QHttp * http,
                                                      const JKKSPMessWithAddr * message)
{
    QByteArray ba;
    if(message == NULL)
        return ba;

    if(http == NULL)
        return ba;

    JKKSXMLMessage * xml = 0;
    switch (message->pMess.getType())
    {
        case JKKSMessage::atXMLMessage: 
            xml = new JKKSXMLMessage(); break;
        default: 
            kksCritical() << QObject::tr("Error: unknown message type");
    }

    if (xml){
        xml->unserialize (message->pMess.getData());
    }
    else{
        kksCritical() << QObject::tr("Unsupported message format! XMLMessage expected!");
        return ba;
    }

    QByteArray byteArray = xml->xmlData().toUtf8();
    delete xml;

    if(byteArray.size() == 0)
        return ba;
    
    //base64
    byteArray = byteArray.toBase64();
    //URL Encoding
    byteArray = byteArray.toPercentEncoding("","~");

    //возвращаемый результат
    ba = byteArray;

    JKKSAddress addr = message->addr;

    QString recvHost;
    int recvPort;

    //если в параметрах транспорта объекта-получателя сказано, что для доставки сообщений необходимо использовать шлюз ...
    // и при этом локальный http_connector подключен к этому шлюзу
    if(addr.useGateway() && !gatewayHost.isEmpty() && gatewayPort > 0){
        recvHost = gatewayHost;
        recvPort = gatewayPort;
    }
    else{ //в противном случае отправляем сообщение напрямую на http_connector целевого объекта
        recvHost = addr.address();
        recvPort = addr.port();
    }
    
    QUrl url;
    url.setHost(recvHost);
    url.setPort(recvPort);
    url.setScheme("http");
    
    QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    QString h = url.host();
    int p = url.port(8080);
    
    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";

    bool ok = createRequestHeader(header, h, p, ba.length());    
    if(!ok)
        return QByteArray();
   
    http->setHost(h, mode, p); 
  
    return ba;
}

bool DDocInteractorClientForXML::createRequestHeader(QHttpRequestHeader & header,
                                                     const QString & host, 
                                                     int port, 
                                                     //bool https, 
                                                     int length)
{
    header = QHttpRequestHeader("POST", "/");

    header.setValue("Host", host + ":" + QString::number(port));
    header.setContentType("application/x-www-form-urlencoded"); // important
  	header.setContentLength(length); 

    return true;
}


bool DDocInteractorClientForXML::sendOutMessage(const JKKSPMessWithAddr * message, 
                                                bool bSync)
{   
    if(!message)
        return false;

    QEventLoop eventLoop;
    QHttp * m_http = NULL;
    QString dbgMsg;

    m_http = http;
    connect(this,SIGNAL(needToExitEventLoop()),&eventLoop,SLOT(quit()));
    connect(m_parent, SIGNAL(exitThreads()), &eventLoop, SLOT(quit()));
    dbgMsg = tr("Message sending request sheduled. Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = ").arg(message->pMess.receiverUID()).arg(message->addr.address()).arg(message->addr.port());
    

    QHttpRequestHeader header;
    QByteArray ba = prepareForSend(header, m_http, message);
    if(ba.isEmpty())
        return false;

    if(bSync)
	{
        int httpGetId = m_http->request(header, ba);
        saveRequestId(httpGetId, message);

        kksInfo() << dbgMsg << QString("%1").arg(httpGetId);
		
        eventLoop.exec();
        if(m_isExiting)
            return true;
	}
    else 
    {
        int httpGetId = m_http->request(header, ba);
        saveRequestId(httpGetId, message);
        
        kksInfo() << dbgMsg << QString("%1").arg(httpGetId);
    }
  
    return true;
}

void DDocInteractorClientForXML::saveRequestId(int reqId, const JKKSPMessWithAddr * message)
{
    if(!message)
        return;

    if(message->pMess.getType() != JKKSMessage::atXMLMessage)
        return;

    httpMessages.insert(reqId, qMakePair(message->id, (qint64)message->pMess.getType()) );
}