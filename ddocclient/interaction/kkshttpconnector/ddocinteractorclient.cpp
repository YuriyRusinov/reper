
#include <QtNetwork>
#include <QTimer>
#include <QCryptographicHash>


#include "ddocinteractorclient.h"
#include "ddocinteractorbase.h"
#include <JKKSLoader.h>

#include <defines.h>
#include <kksdebug.h>
#include <KKSList.h>
#include <JKKSIOUrl.h>
#include <JKKSMessage.h>
#include <JKKSPMessage.h>
#include <kksclient_name.h>


DDocInteractorClient::DDocInteractorClient(JKKSLoader* loader, DDocInteractorBase * parent) 
    : QThread(parent), 
      m_loader(NULL), 
      m_parent(NULL),
      http(NULL),
      pingHttp(NULL),
      pingResHttp(NULL),
      manual(false),
      m_timer(NULL),
      m_interval(0),
      gatewayPort(0),
      m_isExiting(false)
{
    m_loader = loader; 
    m_parent = parent;
}

DDocInteractorClient::~DDocInteractorClient()
{
    if(m_timer){
        m_timer->stop();
        delete m_timer;
        m_timer = NULL;
    }

    disconnect(this, SIGNAL(httpMessageRemoved(int)), this, SLOT(slotHttpMessageRemoved(int)));
    disconnect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
    disconnect(pingHttp, SIGNAL(requestFinished(int, bool)), this, SLOT(pingHttpRequestFinished(int, bool)));
    disconnect(pingResHttp, SIGNAL(requestFinished(int, bool)), this, SLOT(pingResHttpRequestFinished(int, bool)));

    wait();

    if(http)
        delete http;
     
    if(pingHttp)
        delete pingHttp;

    if(pingResHttp)
        delete pingResHttp;

}

/***************************/
/***************************/

void DDocInteractorClient::run()
{
    exec();
}

void DDocInteractorClient::init()
{
    if(!m_loader)
        return;
    
    http = new QHttp(this);
    pingHttp = new QHttp(this);
    pingResHttp = new QHttp(this);

    //срабатывает, когда в методе httpRequestFinished пришел очередной ответ и удалена запись из списка http_messages
    connect(this, SIGNAL(httpMessageRemoved(int)), this, SLOT(slotHttpMessageRemoved(int)));
    //завершен запрос на передачу данных через http
    connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
    //завершен запрос на передачу данных через pingHttp
    connect(pingHttp, SIGNAL(requestFinished(int, bool)), this, SLOT(pingHttpRequestFinished(int, bool)));
    //завершен запрос на передачу данных через pingResHttp
    connect(pingResHttp, SIGNAL(requestFinished(int, bool)), this, SLOT(pingResHttpRequestFinished(int, bool)));
    
    createTimer();
}

void DDocInteractorClient::createTimer()
{
    if(manual == false)
	{
        m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(startProc()));
        m_timer->setInterval(m_interval);
        m_timer->start();
	}
}

void DDocInteractorClient::slotRefreshTimer(int interval)
{
    if(interval < 0)
        return;

    m_timer->setInterval(interval);
}

void DDocInteractorClient::startProc()

{
    if(m_isExiting){ //если m_base завершает работу (т.е. нажали на кнопку закрытия приложения)
        return;
    }

    emit sendingStarted();

    if(manual == false){
        m_timer->stop();
    }
    
    //emit showStatusText(tr("Waiting for data to sent..."));
    //kksInfo() << tr("Waiting for data to sent...");
   
    QStringList receivers;
    
    //сначала отправим ответы на полученные ранее пинги
    QList<JKKSPMessWithAddr *> pingResultsList = m_loader->readPingResults(receivers);
    if(pingResultsList.count() > 0 )
    {
        kksInfo() << tr("Found %1 ping results for sent").arg(QString::number(pingResultsList.count()));
        httpMessages.clear();

        for (QList<JKKSPMessWithAddr *>::const_iterator iterator = pingResultsList.constBegin();iterator != pingResultsList.constEnd();++iterator)
        {
            bool stat = sendOutMessage((*iterator), false, false) ;  //Второй параметр отвечает за то, что данные будут уходить синхронно или асинхронно. 
                                                                    //Здесь асинхронно. Т.е. система не будет ожидать, пока не завершится передача одного сообщения
                                                                    //прежде чем приступить к передаче следующего
        }


        while(!pingResultsList.isEmpty())
            delete pingResultsList.takeFirst();

        kksInfo() << tr("Ping results sending complete");
    }
    
    receivers.clear();
    
    messageList = m_loader->readMessages(receivers);
    QList<JKKSFilePart *> files = m_loader->readFileParts(receivers);

    if(receivers.isEmpty()){
        emit sendingCompleted();
        if(manual == false)
            m_timer->start();
        return;
    }

    //потом отправим пинги ("пузыри") во все организации, на которые необходимо отправить данные
    //в случае, если на какие-либо организации пинги прошли неудачно - на такие организации данные не отправляем
    //отправляем пинги асинхронно
    //а далее ждем все ответы, и когда все ответы придут - обновляем набор пингов и начинаем отправку сообщений
    m_parent->m_pings = m_loader->createPings(receivers);
    m_parent->pingsForSent = m_parent->m_pings.count();
    m_parent->cntPingsSended = 0;

    if(m_parent->pingsForSent > 0)
        kksWarning() << tr("Starting to send %1 ping requests.\nSending messages will be suspended until all ping results do not come back").arg(QString::number(m_parent->pingsForSent));

    
    //* Рассылаем пинги и ждем пока этот процесс не завершится*/ 
    //Важно! Нельзя здесь делать евент-луп, если пинги рассылаются синхронно, т.к. в этом случае сигнал о завершении рассылки придет раньше, чем стартует евент-луп
    
    bool bSync1 = false;
    QEventLoop eventLoop;
	connect(m_parent, SIGNAL(pingsSentCompleted()), &eventLoop, SLOT(quit()));
    connect(m_parent, SIGNAL(exitThreads()), &eventLoop, SLOT(quit()));
    
    if(sendPings(bSync1) != 0){
        eventLoop.exec();
    }
    else{
        emit sendingCompleted();
        if(manual == false){
            m_timer->start();
        }

        return;
    }

    if(m_isExiting){ //если m_base завершает работу (т.е. нажали на кнопку закрытия приложения)
        return;
    }
    
    bool somePingsOK = verifyPings();
    if(!somePingsOK){
        qCritical() << tr("All of destination organization is offline. Data sending ignored");

        emit sendingCompleted();

        if(manual == false){
            m_timer->start();
        }

        return;
    }
    

    kksInfo() << tr("Pings sending complete");
   
    m_parent->filesForSent = files.count();
    m_parent->msgForSent = messageList.size();
    m_parent->filePartsForSent = 0;

    if(m_parent->msgForSent + m_parent->filesForSent <= 0){
        emit sendingCompleted();
        if(manual == false){
            m_timer->start();
        }
        return;
    }
        
    m_parent->cntMsgSended = 0;
    m_parent->cntFilesSended = 0;
    m_parent->cntFilePartsSended = 0;

    QString msg = tr("Data transferring started ...\n"
                     "Messages for transfer: %1 ---> %3\n"
                     "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
                            .arg(m_parent->msgForSent)
                            .arg(m_parent->filesForSent)
                            .arg(m_parent->cntMsgSended)
                            .arg(m_parent->cntFilesSended)
                            .arg(m_parent->filePartsForSent)
                            .arg(m_parent->cntFilePartsSended);

    kksWarning() << msg;
    //emit showStatusText(msg);
    

    if(m_parent->msgForSent > 0 )
    {
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
            bool stat = sendOutMessage((*iterator), true, false);
            
            //если было принято решение не отправлять сообщение
            if(stat == false){
                m_parent->cntMsgSended++; //все-таки считаем его отправленным
                
                //если отправили всё
                if(m_parent->cntMsgSended >= m_parent->msgForSent && m_parent->filesForSent == 0){
                    emit sendingCompleted();
                    if(manual == false){
                        m_timer->start();
                    }
                }

            }
        }

        while(!messageList.isEmpty())
            delete messageList.takeFirst();
    }

    qint64 position = 0;
    int block = _MAX_FILE_BLOCK3;
    
    for(int i=0; i<m_parent->filesForSent; i++){
        qint64 fileSize = m_loader->getFileDataSize(files.at(i)->getIdUrl());
        qint64 partsCount = fileSize / (qint64 )_MAX_FILE_BLOCK3 ;
        if(fileSize > 0 && partsCount == 0)
            partsCount = 1;

        m_parent->filePartsForSent = (int)partsCount;
        m_parent->cntFilePartsSended = 0;

        QString msg = tr("Data transferring started ...\n\n"
                                "Messages for transfer: %1 ---> %3\n"
                                "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
                                .arg(m_parent->msgForSent)
                                .arg(m_parent->filesForSent)
                                .arg(m_parent->cntMsgSended)
                                .arg(m_parent->cntFilesSended)
                                .arg(m_parent->filePartsForSent)
                                .arg(m_parent->cntFilePartsSended);
        //emit showStatusText(msg);
        kksInfo() << msg;
        

        bool isFirst = true;
        do
        {
            //JKKSPMessWithAddr * pMes = NULL;
            qint64 readed = 0;
            QByteArray data = m_loader->readFilePartData(files.at(i)->getAbsUrl(), block, position, & readed);
            position += readed;
            
            JKKSFilePart part(*(files.at(i)));
            part.setAbsUrl(files.at(i)->getAbsUrl());
            part.setAddr(files.at(i)->getAddr());
            part.setId(files.at(i)->id());
            part.setIdUrl(files.at(i)->getIdUrl());
            part.setIsFirst(isFirst);
            isFirst = false;//далее все части файла не являются первыми
        
            if(readed == -1)//при ошибке чтения выходим на следующий файл, текущий не передаем
                break;

            bool eof = false;
            if(readed == 0 || readed < block){//конец файла достигнут
                part.setIsLast(true);
                eof = true;
            }
        
            if(readed > 0)
                part.setData(data);

            JKKSPMessage pM(part.serialize(), JKKSMessage::atFilePart);
            pM.setVerifyReceiver(false);
            pM.setReceiverUID(part.receiverUID());
            pM.setSenderUID(part.senderUID());
            
            JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr(pM, part.getAddr(), part.id());
            pMessWithAddr->unp = part.receiverUID();
            
            //части файлов обязательно передаем синхронно
            bool stat = sendOutMessage(pMessWithAddr, true, eof) ;

            if(!stat){
                m_parent->cntFilePartsSended++; //все-таки считаем часть файла отправленной
                if(eof)
                    m_parent->cntFilesSended++;//если отправляли последнюю часть файла, то и сам файл считаем отправленным
                
                //если отправили всё
                if(m_parent->cntFilesSended >= m_parent->filesForSent){
                    emit sendingCompleted();
                    if(manual == false){
                        m_timer->start();
                    }
                }
            }

            if(eof){
                break;
            }
            
            delete pMessWithAddr;
        }
        while(1);
    }

    while(!files.isEmpty())
        delete files.takeFirst();

    emit sendingCompleted();

}

//ответ http-сервера на запрос типа POST
void DDocInteractorClient::httpRequestFinished(int requestId, bool error)
{
    /*
        Здесь возможны два варианта
        1) Завершился запрос типа POST, который был передан на целевой объект напрямую (другому аналогичному http_connector)
           В этом случае readAll должна вернуть либо OK, либо ничего, причем в последнем случае параметр error должен быть равен TRUE.
           В случае ОК мы должны отметить переданное (статус  корректностии обработки получателем придет позже в спец.квитанции!!!) 
           сообщение как доставленное (setAsSended).
           При ошибке - вывести сообщение об ошибке.
           Сообщения, которые помечены в БД как недоставленные, будут при следующем опросе БД снова находится в отправляемых данных

        2) Завершился запрос типа POST, который был передан в шлюз (ТПС). В этом случае здесь мы ожидаем квитанцию от ТПС о том, 
           "подхватила" ли она это сообщение или при его транспортировке возникла ошибка.
           При ошибке не надо делать никаких дополнительных действий в БД. Однако стоит вывести сообщение об ошибке.
           При успехе - необходимо пометить сообщение в БД как отправленное (факт обработки придет позже)
           Кроме того, при взаимодействии через шлюз (ТПС) придет квитанция о факте доведения шлюзом сообщения до целевого объекта
           Обрабатывая данную квитанцию (метод processNotification), в случае, если код возврата там будет не равен 1, 
           мы переотметим данное сообщение, как требующее повторной отправки (недоставленное).
           Сообщения, которые помечены в БД как недоставленные, будут при следующем опросе БД снова находится в отправляемых данных

       В любом случае мы должны будем удалить сообщение из списка httpMessages. 
       Если его в списке нет - требуется ничего не делать (ибо это может быть просто сетевой спам)
    */
    
    uint messCount = httpMessages.count();
    if(messCount <= 0)
        return;
    
    QPair<qint64, qint64> defValue = QPair<qint64, qint64>();
    QPair<qint64, qint64> t = httpMessages.value(requestId, defValue);
    if(t == defValue){
        return;
    }
    
    kksDebug() << tr("Message sending request completed. Internal request ID = %1").arg(requestId);
    emit needToExitEventLoop();

    httpMessages.remove(requestId);
    
    if( (JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atFilePart){
        kksInfo() << tr("Next file transferred (%1 from %2)").arg(m_parent->cntFilesSended).arg(m_parent->filesForSent);
        m_parent->cntFilesSended++;
    }
    else if((JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atUnknownType){
        kksInfo() << tr("Next file part transferred (%1 from %2)").arg(m_parent->cntFilePartsSended).arg(m_parent->filePartsForSent);
        m_parent->cntFilePartsSended++;
    }
    else{
        kksInfo() << tr("Next message transferred (%1 from %2)").arg(m_parent->cntMsgSended).arg(m_parent->msgForSent);
        m_parent->cntMsgSended++;
    }

    QString msg = tr("Data transferring started ...\n\n"
                            "Messages for transfer: %1 ---> %3\n"
                            "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
                            .arg(m_parent->msgForSent)
                            .arg(m_parent->filesForSent)
                            .arg(m_parent->cntMsgSended)
                            .arg(m_parent->cntFilesSended)
                            .arg(m_parent->filePartsForSent)
                            .arg(m_parent->cntFilePartsSended);
    
    //emit showStatusText(msg);
    kksInfo() << msg;

    //если мы отправляли часть файла и она не является последней
    if( (JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atUnknownType)
        return;

    emit httpMessageRemoved(m_parent->cntMsgSended + m_parent->cntFilesSended);
    
    bool bError = error;
    QHttpResponseHeader header = http->lastResponse();
    if(!header.isValid()){
        bError = true;
    }

    if(header.statusCode() != 200 ){ //OK code
        bError = true;
    }

    if (bError) {
        qCritical() << tr("Message sending request failed! Internal request ID = %1").arg(requestId);
        
        //здесь надо обновить информацию о состоянии целевой организации (в пингах).
        //Сделать ее неактивной, чтобы последующие сообщения не слались далее.
        QString emailPrefix = m_loader->getReceiverEmailPrefix(t.first, t.second);
        if(emailPrefix.isEmpty())
            return;//но это так не должно быть

        JKKSPing defValue = JKKSPing();
        JKKSPing p = m_parent->m_pings.value(emailPrefix, defValue);

        p.setState1(0);
        p.setState2(0);
        m_parent->m_pings.insert(emailPrefix, p);

        return;
    } 
    
    //здесь мы полагаем, что если пришел ответ на сообщение с requestId из нашего перечня (и error = false), 
    //мы должны обработать тело этого сообщения
    //если тело == OK или оно пустое, то это скорее всего было взаимодействие напрямую с http_connector'ом 
    //в противном случае ответ должен быть в формате
    // 1 OK
    // 1 ERROR 15
    //и это означает, что взаимодействие осуществлялось через шлюз (ТПС)
    QByteArray ba = http->readAll();
    if(ba.length() <= 0 || ba == "OK"){//это сообщение было передано напрямую на целевой объект в http_connector
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << tr("Cannot mark message as sended! Database Error. Message ID = %1, Message type = %2. Internal request ID = %3").arg(t.first).arg(t.second).arg(requestId);
        }
        return;
    }

    //далее полагаем, что ответ пришел от шлюза
    //и в случае, если ответ не содержит " ERROR ", помечаем сообщение как отправленное
    if( ! ba.contains(" ERROR ")){
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << tr("Cannot mark message as sended! Database Error. Message ID = %1, Message type = %2. Internal request ID = %3").arg(t.first).arg(t.second).arg(requestId);
        }
    }
    else{
        //в противном случае помечаем информацию о целевой организации (в пинге)
        //Делаем ее неактивной
        QString emailPrefix = m_loader->getReceiverEmailPrefix(t.first, t.second);
        if(emailPrefix.isEmpty())
            return;//но это так не должно быть

        JKKSPing defValue = JKKSPing();
        JKKSPing p = m_parent->m_pings.value(emailPrefix, defValue);

        p.setState1(0);
        p.setState2(0);
        m_parent->m_pings.insert(emailPrefix, p);

        return;
    }
}

void DDocInteractorClient::slotHttpMessageRemoved(int sendedCount)
{
    //msgForSent + filesForSent выставляется в методе startProc() при чтении кол-ва сообщений и файлов на отправку

    if((m_parent->msgForSent + m_parent->filesForSent) > sendedCount){ 
        return;
    }
    
    //если подготовленных сообщений на отправку больше нет - включаем таймер и ждем данных из БД
    kksInfo() << tr("Waiting for data to sent...");
    if(manual == false){
        m_timer->start();
    }
    else
        emit sendingCompleted(); //если активен ручной (отладочный) режим - делаем доступной кнопку отправки
}

//ответ http-сервера на запрос типа POST
void DDocInteractorClient::pingResHttpRequestFinished(int requestId, bool error)
{
    /*
        Здесь возможны два варианта
        1) Завершился запрос типа POST, который был передан на целевой объект напрямую (другому аналогичному http_connector)
           В этом случае readAll должна вернуть либо OK, либо ничего, причем в последнем случае параметр error должен быть равен TRUE.
           В случае ОК мы должны отметить переданное (статус  корректностии обработки получателем придет позже в спец.квитанции!!!) 
           сообщение как доставленное (setAsSended).
           При ошибке - вывести сообщение об ошибке.
           Сообщения, которые помечены в БД как недоставленные, будут при следующем опросе БД снова находится в отправляемых данных

        2) Завершился запрос типа POST, который был передан в шлюз (ТПС). В этом случае здесь мы ожидаем квитанцию от ТПС о том, 
           "подхватила" ли она это сообщение или при его транспортировке возникла ошибка.
           При ошибке не надо делать никаких дополнительных действий в БД. Однако стоит вывести сообщение об ошибке.
           При успехе - необходимо пометить сообщение в БД как отправленное (факт обработки придет позже)
           Кроме того, при взаимодействии через шлюз (ТПС) придет квитанция о факте доведения шлюзом сообщения до целевого объекта
           Обрабатывая данную квитанцию (метод processNotification), в случае, если код возврата там будет не равен 1, 
           мы переотметим данное сообщение, как требующее повторной отправки (недоставленное).
           Сообщения, которые помечены в БД как недоставленные, будут при следующем опросе БД снова находится в отправляемых данных

       В любом случае мы должны будем удалить сообщение из списка httpMessages. 
       Если его в списке нет - требуется ничего не делать (ибо это может быть просто сетевой спам)
    */
    
    uint messCount = pingResHttpMessages.count();
    if(messCount <= 0)
        return;
    
    QPair<qint64, qint64> defValue = QPair<qint64, qint64>();
    QPair<qint64, qint64> t = pingResHttpMessages.value(requestId, defValue);
    if(t == defValue){
        return;
    }
    
    kksDebug() << tr("Ping result sending request completed. Internal request ID = %1").arg(requestId);
    //emit needToExitEventLoop();

    pingResHttpMessages.remove(requestId);
    
    kksInfo() << tr("Next ping result transferred");
    //m_parent->cntMsgSended++;
    
    //emit httpMessageRemoved(m_parent->cntMsgSended + m_parent->cntFilesSended);
    
    bool bError = error;
    QHttpResponseHeader header = pingResHttp->lastResponse();
    if(!header.isValid()){
        bError = true;
    }

    if(header.statusCode() != 200 ){ //OK code
        bError = true;
    }

    if (bError) {
        qCritical() << tr("Ping result sending request failed! Internal request ID = %1").arg(requestId);
        return;
    } 
    
    //здесь мы полагаем, что если пришел ответ на сообщение с requestId из нашего перечня (и error = false), 
    //мы должны обработать тело этого сообщения
    //если тело == OK или оно пустое, то это скорее всего было взаимодействие напрямую с http_connector'ом 
    //в противном случае ответ должен быть в формате
    // 1 OK
    // 1 ERROR 15
    //и это означает, что взаимодействие осуществлялось через шлюз (ТПС)
    QByteArray ba = pingResHttp->readAll();
    if(ba.length() <= 0 || ba == "OK"){//это сообщение было передано напрямую на целевой объект в http_connector
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << tr("Cannot mark ping result as sended! Database Error. Message ID = %1, Message type = %2. Internal request ID = %3").arg(t.first).arg(t.second).arg(requestId);
        }
        return;
    }

    //далее полагаем, что ответ пришел от шлюза
    //и в случае, если ответ не содержит " ERROR ", помечаем сообщение как отправленное
    if( ! ba.contains(" ERROR ")){
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << tr("Cannot mark ping result as sended! Database Error. Message ID = %1, Message type = %2. Internal request ID = %3").arg(t.first).arg(t.second).arg(requestId);
        }
    }
}

bool DDocInteractorClient::setMessageAsSended(const qint64 & id, const int & type, bool sended)
{
    bool result = false;
    if(m_loader)
        result = m_loader->setAsSended(id, type, sended);

    return result;
}


QByteArray DDocInteractorClient::prepareForSend(QHttpRequestHeader & header,
                                                QHttp * http,
                                                const JKKSPMessWithAddr * message, 
                                                //bool bSync,
                                                //bool isLastFilePart,
                                                bool isPing)
{
    QByteArray ba;
    if(message == NULL)
        return ba;

    if(http == NULL)
        return ba;

    if(!isPing){
        //для ответов на пинги не надо проверять активен ли получатель
        if(message->pMess.getType() != JKKSMessage::atPingResponse){
            QString receiver = message->pMess.receiverUID();
            JKKSPing ping = m_parent->m_pings.value(receiver);
            
            if(ping.created() != 1){
                //разрешаем в таком случае отсылку только для "проверки связи" при первоначальной синхронизации
                if(message->pMess.getType() != JKKSMessage::atEcho)
                    return ba;
            }

            if(!ping.completed() ||
                ping.state1() != 1 ||
                ping.state2() != 1 ||
                ping.state3() != 1 ||
                ping.state4() != 1)
            {
                return ba;
            }
        }
    }

    QByteArray byteArray = message->pMess.serialize();  
    if(byteArray.size() == 0)
        return ba;
    
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha1);

    QString unp = message->unp;

    //base64
    byteArray = byteArray.toBase64();
    //zip
    byteArray = qCompress(byteArray, 9);
    //URL Encoding
    byteArray = byteArray.toPercentEncoding("","~");

    //ВАЖНО!!! 
    //Для передачи данных через шлюз ТПС используется механизм псевдофасетного кодирования идентификатора пересылаемого сообщения
    //В конец строкового представления идентификатора пересылаемого сообщения добавляются два байта (две цифры), характеризующие тип
    //(т.е. из какой таблицы взят идентификатор). Эта особенность не влияет на логику обработки пересылаемых данных посредством kksinteractor и http_connector
    //однако в теле почтового сообщения, отдаваемого в шлюз ТПС, учитывается.
    //
    //Формат сообщения: mesid=<ID><type_digit_1><type_digit_2>&unp=<email_prefix>&data=........(base64)......
    //Если тип имеет одну цифру, перед ней ставится 0. т.е. 00 - atCommand, 02 - atDocument, и т.д.
    
    QString mesId;
    
    //тип сообщения. см. типы сообщений в перечислении JKKSMessage::JKKSMessageType
    //Для пингов = 13
    int t = message->pMess.getType(); 
    if(t >= 0 && t <= 9){
        mesId = QString("0%1").arg(QString::number(t));
    }
    else{
        mesId = QString("%1").arg(QString::number(t));
    }

    //создаем строку сообщения в унифицированном прикладном протоколе передачи данных ТПС
    QString s = QString("mesid=%1%2&unp=%3&data=")
                          .arg(QString::number(message->id)) //идентификатор сообщения из соответствующей таблицы
                                                             //для пингов - идентификатор пинга. Равен идентификатору организации из таблицы organization
                          .arg(mesId) 
                          .arg(message->unp.isEmpty() ? QString("1919") : message->unp);//Условный номер получателя (используем email_prefix)

    byteArray.prepend(s.toAscii());
    //hash-сумма
    byteArray.append("&hash=");
    hash = hash.toBase64();
    hash = hash.toPercentEncoding();
    byteArray.append(hash);
    
    //возвращаемый результат
    ba = byteArray;

    JKKSAddress addr = message->addr;

    QString recvHost;
    int recvPort;

    //если в параметрах транспорта объекта-получателя сказано, что для доставки сообщений необходимо использовать шлюз (ТПС) ...
    // и при этом локальный http_connector подключен к этому шлюзу (ТПС)
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

bool DDocInteractorClient::createRequestHeader(QHttpRequestHeader & header,
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

void DDocInteractorClient::saveRequestId(int reqId, const JKKSPMessWithAddr * message, bool isLastFilePart)
{
    if(!message)
        return;


    if(message->pMess.getType() == JKKSMessage::atPing){
        //мы имеем право взять нужный нам пинг из списка подготовленных на отправку пингов, используя email_prefix организации-получателя пинга
        JKKSPing pp = m_parent->m_pings.value(message->pMess.receiverUID());
        pingHttpMessages.insert(reqId, pp);
        return;
    }

    if(message->pMess.getType() == JKKSMessage::atPingResponse){
        pingResHttpMessages.insert(reqId, qMakePair(message->id, (qint64)message->pMess.getType()) );
        return;
    }

    if(message->pMess.getType() != JKKSMessage::atFilePart){//для файлов, передаваемых частями информация в этот список заносится отдельно
        httpMessages.insert(reqId, qMakePair(message->id, (qint64)message->pMess.getType()) );
    }
    else{
        if(isLastFilePart){
            //это приведет к тому, что в методе httpRequestFinished обработка факта полной передачи файла сработает только однажды, когда передана последняя часть файла
            httpMessages.insert(reqId, qMakePair(message->id, (qint64)message->pMess.getType()) );
        }
        else{
            //Это приведет к тому, что в методе httpRequestFinished обработки факта передачи файла не произойдет, однако сгенерируется сигнал, который завершит eventLoop
            httpMessages.insert(reqId, qMakePair(message->id, (qint64)JKKSMessage::atUnknownType) );
        }
    }
}

bool DDocInteractorClient::sendOutMessage(const JKKSPMessWithAddr * message, 
                                          bool bSync,
                                          bool isLastFilePart)
{

    if(!message)
        return false;

    bool isPing = false;
    if(message->pMess.getType() == JKKSMessage::atPing)
        isPing = true;

    QEventLoop eventLoop;
    QHttp * m_http = NULL;
    QString dbgMsg;
    if(message->pMess.getType() == JKKSMessage::atPing){
        m_http = pingHttp;
        connect(this,SIGNAL(needToExitEventLoopPing()),&eventLoop,SLOT(quit()));
        connect(m_parent, SIGNAL(exitThreads()), &eventLoop, SLOT(quit()));
        dbgMsg = tr("Ping sending request sheduled. Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = ").arg(message->pMess.receiverUID()).arg(message->addr.address()).arg(message->addr.port());

    }
    else if(message->pMess.getType() == JKKSMessage::atPingResponse){
        m_http = pingResHttp;
        connect(this,SIGNAL(needToExitEventLoopPingRes()),&eventLoop,SLOT(quit()));
        connect(m_parent, SIGNAL(exitThreads()), &eventLoop, SLOT(quit()));
        dbgMsg = tr("Ping response sending request sheduled. Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = ").arg(message->pMess.receiverUID()).arg(message->addr.address()).arg(message->addr.port());
    }
    else{
        m_http = http;
        connect(this,SIGNAL(needToExitEventLoop()),&eventLoop,SLOT(quit()));
        connect(m_parent, SIGNAL(exitThreads()), &eventLoop, SLOT(quit()));
        dbgMsg = tr("Message sending request sheduled. Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = ").arg(message->pMess.receiverUID()).arg(message->addr.address()).arg(message->addr.port());
    }

    QHttpRequestHeader header;
    QByteArray ba = prepareForSend(header, m_http, message, isPing);
    if(ba.isEmpty())
        return false;

    if(bSync)
	{
        int httpGetId = m_http->request(header, ba);
        saveRequestId(httpGetId, message, isLastFilePart);

        kksInfo() << dbgMsg << QString("%1").arg(httpGetId);
		
        eventLoop.exec();
        if(m_isExiting)
            return true;
	}
    else 
    {
        int httpGetId = m_http->request(header, ba);
        saveRequestId(httpGetId, message, isLastFilePart);
        
         kksInfo() << dbgMsg << QString("%1").arg(httpGetId);
    }
  
    return true;
}

int DDocInteractorClient::sendPings(bool bSync)
{
    if(m_parent->m_pings.isEmpty()){
        emit pingsSentCompleted();
        return 0;
    }
    
    QList<JKKSPMessWithAddr *> pingList = JKKSLoader::pingsToPMessWithAddr(m_parent->m_pings);

    for (QList<JKKSPMessWithAddr *>::const_iterator iterator = pingList.constBegin();iterator != pingList.constEnd();++iterator)
    {
        bool stat = sendOutMessage((*iterator), bSync, false);
    }

    while(!pingList.isEmpty())
        delete pingList.takeFirst();

    return 1;

}

//ответ http-сервера на запрос передачи пинга 
void DDocInteractorClient::pingHttpRequestFinished(int requestId, bool error)
{    
    uint messCount = pingHttpMessages.count();
    if(messCount <= 0)
        return;
    
    JKKSPing defValue = JKKSPing();
    JKKSPing t = pingHttpMessages.value(requestId, defValue);
    if(t == defValue){
        return;
    }
    
    pingHttpMessages.remove(requestId);
    
    emit needToExitEventLoopPing();

    bool bError = error;
    QHttpResponseHeader header = pingHttp->lastResponse();
    if(!header.isValid()){
        bError = true;
    }

    if(header.statusCode() != 200 ){ //OK code
        bError = true;
    }

    //если не смогли отправить пинг - помечаем его как отправленного и неудачно доставленного (приемная сторона не отвечает)
    if (bError) {
        qCritical() << tr("Ping sending request failed! Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = %4").arg(t.uidTo()).arg(t.getAddr().address()).arg(t.getAddr().port()).arg(requestId);
        if(!pingHttp->errorString().isEmpty())
            qCritical() << pingHttp->errorString();

        t.setState1(0);
        t.setState2(0);
        t.setState3(0);
        t.setState4(0);
        processPingResponse(&t);
        return;
    } 
    
    //здесь мы полагаем, что если пришел ответ на сообщение с requestId из нашего перечня (и error = false), 
    //мы должны обработать тело этого сообщения
    //если тело == OK или оно пустое, то это скорее всего было взаимодействие напрямую с http_connector'ом 
    //в противном случае ответ должен быть в формате
    // 1 OK
    // 1 ERROR 15
    //и это означает, что взаимодействие осуществлялось через шлюз (ТПС)
    QByteArray ba = pingHttp->readAll();
    if(ba.length() <= 0 || ba == "OK"){//это сообщение было передано напрямую на целевой объект в http_connector
        kksDebug() << tr("Ping request delivered to destination organization. Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = %4. Waiting for answer").arg(t.uidTo()).arg(t.getAddr().address()).arg(t.getAddr().port()).arg(requestId);;
        return;
    }

    //далее полагаем, что ответ пришел от шлюза
    //и в случае, если ответ содержит " ERROR ", помечаем сообщение как отправленное
    if( ba.contains(" ERROR ")){
        qCritical() << tr("Ping sending request failed! Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = %4").arg(t.uidTo()).arg(t.getAddr().address()).arg(t.getAddr().port()).arg(requestId);
        if(!pingHttp->errorString().isEmpty())
            qCritical() << pingHttp->errorString();

        t.setState1(0);
        t.setState2(0);
        t.setState3(0);
        t.setState4(0);
        processPingResponse(&t);
    }

    kksDebug() << tr("Ping sending request completed. Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = %4").arg(t.uidTo()).arg(t.getAddr().address()).arg(t.getAddr().port()).arg(requestId);
}

//если пришло сообщение - ответ на пинг, то необходимо обработать его и завершить работу метода
//при этом важно увеличить количество отправленных пингов на один
//и если их кол-во равно кол-ву подготовленных для отправко - сгенерировать сигналы
int DDocInteractorClient::processPingResponse(const JKKSPing * ping)
{
    if(!ping){
        return 1;
    }

    m_parent->cntPingsSended++;

    for (QMap<QString, JKKSPing>::iterator pa = m_parent->m_pings.begin(); pa != m_parent->m_pings.end(); pa++){
        //поскольку в ответе на пинг нет информации какой был идентификатор у организации-отправителя пинга 
        //(а именно он используется в качестве JKKSPing::id())
        //То проверку осуществляем по идентификатору организации-получателя пинга (в таблице БД отправителя пинга).
        //он заносится в качестве id_external_queue (отрицательного) на целевой БД
        if(pa.value().idOrgTo() == ping->idOrgTo()){ 
            pa.value().setVersionTo(ping->versionTo());
            pa.value().setState1(ping->state1());
            pa.value().setState2(ping->state2());
            pa.value().setState3(ping->state3());
            pa.value().setState4(ping->state4());
        }
    }
    
    //pingsForSent Может быть меньше, чем cntPingsSended (т.е. пришло ответов больше, чем запрашивали)
    //т.к. может прийти ответ на пинг, который не ждем. Это возможно, когда коннектор отправил запрос на пинг, не дождался ответа, завершил работу.
    //а потом, когда включился снова, ответ на пинг пришел.
    if(m_parent->pingsForSent <= m_parent->cntPingsSended){
        emit pingsSended(m_parent->m_pings);
        emit pingsSentCompleted();
    }

    return 1;
}

bool DDocInteractorClient::verifyPings()
{

    bool somePingsOK = false;
    bool somePingsError = false;

    for (QMap<QString, JKKSPing>::iterator pa = m_parent->m_pings.begin(); pa != m_parent->m_pings.end(); pa++){
        JKKSPing ping = pa.value();

        if(ping.created() != 1){
            kksWarning() << tr("Organization %1 with address = (IP=%2, port=%3, use gateway = %4) was not complete the first syncronization procedure." 
                                "Only first synchronization queries for that will be allowed")
                                     .arg(ping.nameTo())
                                     .arg(ping.getAddr().address())
                                     .arg(ping.getAddr().port())
                                     .arg(ping.getAddr().useGateway() ? QString("TRUE") : QString("FALSE"));
        }
        
        if( //ping.created() == 1 &&
            ping.completed() &&
            ping.state1() == 1 &&
            ping.state2() == 1 &&
            ping.state3() == 1 &&
            ping.state4() == 1)
        {
            somePingsOK = true;
        }
        else{
            kksWarning() << tr("Organization %1 with address = (IP=%2, port=%3, use gateway = %4) is offline. Data sending for that will be ignored")
                                     .arg(ping.nameTo())
                                     .arg(ping.getAddr().address())
                                     .arg(ping.getAddr().port())
                                     .arg(ping.getAddr().useGateway() ? QString("TRUE") : QString("FALSE"));
            somePingsError = true;
        }
    }

    //if(somePingsError){
    //    kksWarning() << tr("Some of destination organization is offline. Data sending for thay will be ignored.");
    //}

    return somePingsOK;
}
