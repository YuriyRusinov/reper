
#include <QtGui>
#include <QtNetwork>
#include <QCryptographicHash>
#include <QDockWidget>

#include "httpwindow.h"
#include "timerform.h"
#include <kkssito.h>
#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <JKKSIOUrl.h>
#include <JKKSMessage.h>
#include <kksclient_name.h>

HttpWindow::HttpWindow(QWidget *parent)
    : QDialog(parent)
{
    QSettings settings (QCoreApplication::applicationDirPath ()+"/http.ini", QSettings::IniFormat);
    
    m_doNotStart = false;

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
    
    gatewayHost = settings.value ("host", "").toString();
    gatewayPort = settings.value ("port", "0").toInt();

    settings.endGroup ();

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, server_port)) {
        qCritical() << tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString());
        QMessageBox::critical(this, tr("DynamicDocs Interactor "),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        m_doNotStart = true;
        return;
    }

    loader = new (std::nothrow) JKKSLoader (host, dbName, user, password, port, transport);
    if (loader && loader->connectToDb ())
    {
     
    }
    else if (loader)
    {
        qCritical() << tr("Unable to connect to the database: %1.")
                              .arg(host);
        QMessageBox::critical(this, tr("DynamicDocs Interactor"),
                              tr("Unable to connect to the database: %1.")
                              .arg(host));
        m_doNotStart = true;
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(loadData()));

    
    statusLabel = new QLabel(tr("Waiting for data to sent..."));

    startButton = new QPushButton(tr("Send"));
    startButton->setDefault(true);
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    QDockWidget * w = (QDockWidget*)kksSito->dbgWidget();
    mainLayout->addWidget(w->widget());
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("DynamicDocs Server Interactor"));


    http = new QHttp(this);
    pingHttp = new QHttp(this);

    //срабатывает, когда в методе httpRequestFinished пришел очередной ответ и удалена запись из списка http_messages
    connect(this, SIGNAL(httpMessageRemoved(int)), this, SLOT(slotHttpMessageRemoved(int)));

    connect(http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));

    connect(pingHttp, SIGNAL(requestFinished(int, bool)),
            this, SLOT(pingHttpRequestFinished(int, bool)));

    connect(startButton, SIGNAL(clicked()), this, SLOT(startTimer()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    user_timer = 36000; //by defaul set timer


    TimerForm * timerForm = new TimerForm ();
    
    if (!timerForm){
        m_doNotStart = true;
        return ;
    }
    
    if (timerForm->exec () != QDialog::Accepted)
    {
        delete timerForm;
        //manual
		manual = true;
    }
    else
    {
		manual = false;
        user_timer = timerForm->getTimer();

        startTimer();
        delete timerForm;
    }

    msgForSent = 0;
    filesForSent = 0;
    cntFilesSended = 0;
    cntMsgSended = 0;
    cntFilePartsSended = 0;
    filePartsForSent = 0;
    cntPingsSended = 0;
    pingsForSent = 0;
}


void HttpWindow::startTimer()
{
    
    startButton->setEnabled(false);

    if(manual == false)
	{
        m_timer = new QTimer(this);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(startProc()));
        m_timer->setInterval(user_timer);
        m_timer->start();
	}
	else
	{
		startProc();
	}
}

void HttpWindow::startProc()

{
    if(manual == false){
        m_timer->stop();
    }

    statusLabel->setText(tr("Waiting for data to sent..."));
   
    QStringList receivers;
    
    //сначала отправим ответы на полученные ранее пинги
    messageList = loader->readPingResults(receivers);
    if(messageList.count() > 0 )
    {
        qWarning() << tr("Found %1 ping results for sent").arg(QString::number(messageList.count()));
        httpMessages.clear();

        for (QList<JKKSPMessWithAddr *>::const_iterator iterator = messageList.constBegin();iterator != messageList.constEnd();++iterator)
        {
            bool stat = sendOutMessage((*iterator), true, false) ; //Второй параметр отвечает за то, что данные будут уходить синхронно или асинхронно. 
                                                                    //Здесь асинхронно. Т.е. система не будет ожидать, пока не завершится передача одного сообщения
                                                                    //прежде чем приступить к передаче следующего
        }


        while(!messageList.isEmpty())
            delete messageList.takeFirst();

        qWarning() << tr("Ping results sending complete");
    }
    
    receivers.clear();
    
    messageList = loader->readMessages(receivers);
    QList<JKKSFilePart *> files = loader->readFileParts(receivers);

    if(receivers.isEmpty()){
        startButton->setEnabled(true);
        if(manual == false)
            m_timer->start();
        return;
    }

    //потом отправим пинги ("пузыри") во все организации, на которые необходимо отправить данные
    //в случае, если на какие-либо организации пинги прошли неудачно - на такие организации данные не отправляем
    //отправляем пинги асинхронно
    //а далее ждем все ответы, и когда все ответы придут - обновляем набор пингов и начинаем отправку сообщений
    m_pings = loader->createPings(receivers);
    pingsForSent = m_pings.count();
    cntPingsSended = 0;

    if(pingsForSent > 0)
        qWarning() << tr("Starting to send %1 ping requests.\nSending messages will be suspended until all ping results do not come back").arg(QString::number(pingsForSent));

    //* Рассылаем пинги и ждем пока этот процесс не завершится*/ 
	QEventLoop eventLoop;
	connect(this,SIGNAL(pingsSentCompleted()),&eventLoop,SLOT(quit()));
    if(sendPings() != 0)
        eventLoop.exec();
    else{
        startButton->setEnabled(true);
        if(manual == false){
            m_timer->start();
        }

        return;
    }

    qWarning() << tr("Pings sending complete");
    //**/

   
    filesForSent = files.count();
    msgForSent = messageList.size();
    filePartsForSent = 0;

    if(msgForSent + filesForSent <= 0){
        startButton->setEnabled(true);
        if(manual == false){
            m_timer->start();
        }
        return;
    }
        
    cntMsgSended = 0;
    cntFilesSended = 0;
    cntFilePartsSended = 0;

    QString msg = tr("Data transferring started ...\n"
                     "Messages for transfer: %1 ---> %3\n"
                     "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
                            .arg(msgForSent)
                            .arg(filesForSent)
                            .arg(cntMsgSended)
                            .arg(cntFilesSended)
                            .arg(filePartsForSent)
                            .arg(cntFilePartsSended);

    qWarning() << msg;
    statusLabel->setText(msg);

    if(msgForSent > 0 )
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
                cntMsgSended++; //все-таки считаем его отправленным
                
                //если отправили всё
                if(cntMsgSended >= msgForSent && filesForSent == 0){
                    startButton->setEnabled(true);
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
    
    for(int i=0; i<filesForSent; i++){
        qint64 fileSize = loader->getFileDataSize(files.at(i)->getIdUrl());
        qint64 partsCount = fileSize / (qint64 )_MAX_FILE_BLOCK3 ;
        if(fileSize > 0 && partsCount == 0)
            partsCount = 1;

        filePartsForSent = (int)partsCount;
        cntFilePartsSended = 0;

        statusLabel->setText(tr("Data transferring started ...\n\n"
                                "Messages for transfer: %1 ---> %3\n"
                                "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
                                .arg(msgForSent)
                                .arg(filesForSent)
                                .arg(cntMsgSended)
                                .arg(cntFilesSended)
                                .arg(filePartsForSent)
                                .arg(cntFilePartsSended));

        bool isFirst = true;
        do
        {
            JKKSPMessWithAddr * pMes = NULL;
            qint64 readed = 0;
            QByteArray data = loader->readFilePartData(files.at(i)->getAbsUrl(), block, position, & readed);
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
            
            //части файлов обязательно передаем синхронно
            bool stat = sendOutMessage(pMessWithAddr, true, eof) ;

            if(!stat){
                cntFilePartsSended++; //все-таки считаем часть файла отправленной
                if(eof)
                    cntFilesSended++;//если отправляли последнюю часть файла, то и сам файл считаем отправленным
                
                //если отправили всё
                if(cntFilesSended >= filesForSent){
                    startButton->setEnabled(true);
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

}

//ответ http-сервера на запрос типа POST
void HttpWindow::httpRequestFinished(int requestId, bool error)
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

    bool bFound = false;
    
    uint messCount = httpMessages.count();
    if(messCount <= 0)
        return;
    
    QPair<qint64, qint64> defValue = QPair<qint64, qint64>();
    QPair<qint64, qint64> t = httpMessages.value(requestId, defValue);
    if(t == defValue){
        return;
    }
    
    qWarning() << tr("Message sending request completed. requestId = ") << requestId;
    emit needToExitEventLoop();

    httpMessages.remove(requestId);
    
    if( (JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atFilePart){
        qWarning() << tr("Next file transferred (%1 from %2)").arg(cntFilesSended).arg(filesForSent);
        cntFilesSended++;
    }
    else if((JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atUnknownType){
        qWarning() << tr("Next file part transferred (%1 from %2)").arg(cntFilePartsSended).arg(filePartsForSent);
        cntFilePartsSended++;
    }
    else{
        qWarning() << tr("Next message transferred (%1 from %2)").arg(cntMsgSended).arg(msgForSent);
        cntMsgSended++;
    }

    statusLabel->setText(tr("Data transferring started ...\n\n"
                            "Messages for transfer: %1 ---> %3\n"
                            "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
                            .arg(msgForSent)
                            .arg(filesForSent)
                            .arg(cntMsgSended)
                            .arg(cntFilesSended)
                            .arg(filePartsForSent)
                            .arg(cntFilePartsSended));

    //если мы отправляли часть файла и она не является последней
    if( (JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atUnknownType)
        return;

    emit httpMessageRemoved(cntMsgSended + cntFilesSended);

    if (error) {
        qCritical() << tr("ERROR: Data transfer for requestId = %1 failed: %2").arg(requestId).arg(http->errorString());
        
        //здесь надо обновить информацию о состоянии целевой организации (в пингах).
        //Сделать ее неактивной, чтобы последующие сообщения не слались далее.
        QString emailPrefix = loader->getReceiverEmailPrefix(t.first, t.second);
        if(emailPrefix.isEmpty())
            return;//но это так не должно быть

        JKKSPing p = m_pings.value(emailPrefix);
        p.setState1(0);
        p.setState2(0);
        m_pings.insert(emailPrefix, p);

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
            qCritical() << "ERROR: Cannot mark message as sended! Database Error";
        }
        return;
    }

    //далее полагаем, что ответ пришел от шлюза
    //и в случае, если ответ не содержит " ERROR ", помечаем сообщение как отправленное
    if( ! ba.contains(" ERROR ")){
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << "ERROR: Cannot mark message as sended! Database Error";
        }
    }
    else{
        //в противном случае помечаем информацию о целевой организации (в пинге)
        //Делаем ее неактивной
        QString emailPrefix = loader->getReceiverEmailPrefix(t.first, t.second);
        if(emailPrefix.isEmpty())
            return;//но это так не должно быть

        JKKSPing p = m_pings.value(emailPrefix);
        p.setState1(0);
        p.setState2(0);
        m_pings.insert(emailPrefix, p);

        return;
    }
}

void HttpWindow::slotHttpMessageRemoved(int sendedCount)
{
    //msgForSent + filesForSent выставляется в методе startProc() при чтении кол-ва сообщений и файлов на отправку

    if((msgForSent + filesForSent) > sendedCount){ 
        return;
    }
    
    //если подготовленных сообщений на отправку больше нет - включаем таймер и ждем данных из БД
    qWarning() << tr("Waiting for data to sent...");
    if(manual == false){
        m_timer->start();
    }
    else
        startButton->setEnabled(true); //если активен ручной (отладочный) режим - делаем доступной кнопку отправки
}

bool HttpWindow::setMessageAsSended(const qint64 & id, const int & type, bool sended)
{
    bool result = false;
    if(loader)
        result = loader->setAsSended(id, type, sended);

    return result;
}

bool HttpWindow::sendOutMessage(const JKKSPMessWithAddr * message, 
                                bool filePartsFlag,
                                bool isLastFilePart)
{
    if(message == NULL)
        return false;

    if(http == NULL)
        return false;

    //для ответов на пинги не надо проверять активен ли получатель
    if(message->pMess.getType() != JKKSMessage::atPingResponse){
        QString receiver = message->pMess.receiverUID();
        JKKSPing ping = m_pings.value(receiver);
        if( ping.created() != 1 ||
            !ping.completed() ||
            ping.state1() != 1 ||
            ping.state2() != 1 ||
            ping.state3() != 1 ||
            ping.state4() != 1)
        {
            qCritical() << tr("Destination organization does not active! Data sending for that skipped!");
            qCritical() << tr("Receiver address = (IP=%1, port=%2, use gateway = %3)")
                                    .arg(message->addr.address())
                                    .arg(message->addr.port())
                                    .arg(message->addr.useGateway() ? QString("TRUE") : QString("FALSE"));
            return false;
        }
    }

    QByteArray byteArray = message->pMess.serialize();  
    if(byteArray.size() == 0)
        return false;
    
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha1);

    QString unp = message->unp;

    //base64
    byteArray = byteArray.toBase64();
    //zip
    byteArray = qCompress(byteArray, 9);

    //ВАЖНО!!! 
    //Для передачи данных через шлюз ТПС используется механизм псевдофасетного кодирования идентификатора пересылаемого сообщения
    //В конец строкового представления идентификатора пересылаемого сообщения добавляются два байта (две цифры), характеризующие тип
    //(т.е. из какой таблицы взят идентификатор). Эта особенность не влияет на логику обработки пересылаемых данных посредством kksinteractor и http_connector
    //однако в теле почтового сообщения, отдаваемого в шлюз ТПС, учитывается.
    //
    //Формат сообщения: mesid=<ID><type_digit_1><type_digit_2>&unp=<email_prefix>&data="........(base64)......"
    //Если тип имеет одну цифру, перед ней ставится 0. т.е. 00 - atCommand, 02 - atDocument, и т.д.
    
    QString mesId;
    int t = message->pMess.getType(); //тип сообщения. см. типы сообщений в перечислении JKKSMessage::JKKSMessageType
    if(t >= 0 && t <= 9){
        mesId = QString("0%1").arg(QString::number(t));
    }
    else{
        mesId = QString("%1").arg(QString::number(t));
    }

    //создаем строку сообщения в унифицированном прикладном протоколе передачи данных ТПС
    QString s = QString("mesid=%1%2&unp=%3&data=\"")
                          .arg(QString::number(message->id)) //идентификатор сообщения из соответствующей таблицы
                          .arg(mesId) 
                          .arg(message->unp.isEmpty() ? QString("1919") : message->unp);//Условный номер получателя (используем email_prefix)

    byteArray.prepend(s.toAscii());
    byteArray.append("\"");
    //hash-сумма
    byteArray.append("&hash=\"");
    hash = hash.toBase64();
    byteArray.append(hash);
    byteArray.append("\"");
    

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
    
    http->setHost(h, mode, p);

    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";

	if(filePartsFlag)
	{
		QEventLoop eventLoop;
		connect(this,SIGNAL(needToExitEventLoop()),&eventLoop,SLOT(quit()));

        int httpGetId = http->post ( path, byteArray ) ;

        if(message->pMess.getType() != JKKSMessage::atFilePart){//для файлов, передаваемых частями информация в этот список заносится отдельно
            httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
        }
        else{
            if(isLastFilePart){
                //это приведет к тому, что в методе httpRequestFinished обработка факта полной передачи файла сработает только однажды, когда передана последняя часть файла
                httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
            }
            else{
                //Это приведет к тому, что в методе httpRequestFinished обработки факта передачи файла не произойдет, однако сгенерируется сигнал, который завершит eventLoop
                httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)JKKSMessage::atUnknownType) );
            }
        }

        qWarning() << tr("Message sending request sheduled. requestId = %1").arg(httpGetId);
		
        eventLoop.exec();
	}
    else 
    {
        int httpGetId = http->post ( path, byteArray ) ;

        if(message->pMess.getType() != JKKSMessage::atFilePart){//для файлов, передаваемых частями информация в этот список заносится отдельно
            httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
        }
        else{
            if(isLastFilePart){
                //это приведет к тому, что в методе httpRequestFinished обработка факта полной передачи файла сработает только однажды, когда передана последняя часть файла
                httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
            }
            else{
                //Это приведет к тому, что в методе httpRequestFinished обработки факта передачи файла не произойдет, однако сгенерируется сигнал, который завершит eventLoop
                httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)JKKSMessage::atUnknownType) );
            }
        }
        
         qWarning() << tr("Message sending request sheduled. requestId = %1").arg(httpGetId);

    }
  
    return true;
}

void HttpWindow::loadData()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    int numRead = 0, numReadTotal = 0;
    
    QByteArray all_data, bt;

    forever {
        bt  = clientConnection->read(1000);

        //set data
        numRead = bt.size();
        
        all_data += bt;
        numReadTotal += numRead;
         
        if (numRead == 0 && 
            !clientConnection->waitForReadyRead(1000)
            )
        {
            break;
        }
    }

    if(numReadTotal == 0){
        sendOKBlock(clientConnection, false);
        return;
    }

    QHttpRequestHeader header( all_data );

    bool b = header.hasContentLength();

    if(b)
    {
        qWarning() << tr("Income message arrived");

        int sz_dt = header.contentLength();
        all_data = all_data.right(sz_dt); 

        QByteArray byteArray = all_data;

        //Входящие данные имеют один из следующих форматов:
        //mesid=<ID><type_digit_1><type_digit_2>&uno=<email_prefix>&data="........(base64)......"
        //mesid=<ID><type_digit_1><type_digit_2>&uno=<email_prefix>&received=<code>
        //в первом случае нам mesid и uno не интересны. Но надо в зависимости от типа сообщения правильно его обработать
        //во втором случае нам интересен идентификатор и тип сообщения, которое было доведено и обработано с указанным в received результатом

        if(byteArray.contains("mesid=") && (byteArray.contains("&uno=") || byteArray.contains("&unp=")) && byteArray.contains("&received=")){
            processNotification(byteArray, clientConnection);
        }
        else if(byteArray.contains("mesid=") && (byteArray.contains("&uno=") || byteArray.contains("&unp=")) && byteArray.contains("&data=\"") && byteArray.contains("&hash=\"")){
            processMessage(byteArray, clientConnection);
        }
        else{
            sendBadBlock(clientConnection);
        }
     }
    else
    {
        sendBadBlock(clientConnection);
    }

    clientConnection->disconnectFromHost();
    qWarning() << tr("Disconnected from socket");
}

int HttpWindow::processMessage(const QByteArray & ba, QTcpSocket * clientConnection)
{

    if(!ba.contains("mesid=") ||  (!ba.contains("&uno=") && !ba.contains("&unp=")) || !ba.contains("&data=\"") || !ba.contains("&hash=\"")){
        sendBadBlock(clientConnection);
        return -1;
    }

    int index = ba.indexOf("&data=\"");
    if(index < 13){ //если даже числа 1 и 1, то данная подстрока встретится не ранее чем на 13-й позиции
        sendBadBlock(clientConnection);
        return -1;
    }

    QByteArray byteArray = ba.mid(index + 7);
    int h = byteArray.indexOf("&hash=\"");
    //эталонная hash-сумма пришедших данных
    QByteArray etaloneHash = byteArray.mid(h+7, byteArray.length() - (h+7) - 1);
    etaloneHash = QByteArray::fromBase64(etaloneHash);
    
    byteArray = byteArray.left(h-1);
    byteArray = qUncompress(byteArray);
    byteArray = QByteArray::fromBase64(byteArray);

    //вычисленная hash-сумма пришедших данных
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha1);
    if(etaloneHash != hash){
        qCritical() << "ERROR! Hash sum for income message is inconsistent!";
        sendBadBlock(clientConnection);
        return -1;
    }

    JKKSPMessage pMessage(byteArray);

    //эталонная hash-сумма пришедших данных
    QByteArray storedHash = pMessage.cryptoHash();
            
    //если пришло сообщение - ответ на пинг, то необходимо обработать его и завершить работу метода
    //при этом важно увеличить количество отправленных пингов на один
    //и если их кол-во равно кол-ву подготовленных для отправко - сгенерировать сигналы
    if(pMessage.getType() == (int)JKKSMessage::atPingResponse){
        JKKSPing * ping = (JKKSPing *)loader->unpackMessage(pMessage);
        processPingResponse(ping);
        sendOKBlock(clientConnection, true);
        return 1;
    }

    int res = loader->writeMessage(pMessage);
    if(res <= 0){
        qCritical() << tr("ERROR! Cannot processing income message! Data was not write to database");
        qCritical() << "Message type: " << pMessage.getType() << ". Write message status : " << res;
    }

    //в настоящее время мы полагаем, что вне зависимости от результата обработки входящего сообщения в БД
    //мы в качестве ответа на запрос объекта-отправителя (ну или шлюза (ТПС)) возвращаем OK
    //тем самым мы говорим о том, что факт приема сообщения состоялся успешно
    //при этом результат обработки будет отправлен отдельно в виде спец. квитанции
    if(res == ERROR_CODE)
    {
        sendOKBlock(clientConnection, true);
    }
    else if(res == OK_CODE)
    {
        sendOKBlock(clientConnection, true);    
    }
    else if(res == IGNORE_CODE)
    {
        sendOKBlock(clientConnection, true);
    }

    return 1;
}

//пришла квитанция о доставке на объект-получатель ранее отправленного сообщения
//данный блок кода должен исполняться только при взаимодействии через шлюз (ТПС)
//результатом может быть факт успешной или неуспешной доставки
//в последнем случае мы должны пометить сообщение как неотправленное
//в случае успешной доставки ничего делать не надо, т.к. еще на этапе отправки сообщения с объекта отправителя в ТПС мы это сообщение отметили как доставленное
int HttpWindow::processNotification(const QByteArray & ba, QTcpSocket * clientConnection)
{
    if(!ba.contains("mesid=") ||  (!ba.contains("&uno=") && !ba.contains("&unp=")) || !ba.contains("&received=")){
        qCritical() << tr("Found inconsistent incoming notification! Bad format");
        sendBadBlock(clientConnection);
        return -1;
    }

    qint64 idMsg = -1;
    int msgType = -1;
    int index = ba.indexOf("&uno");
    if(index <= 0)
        index = ba.indexOf("&unp"); //попробуем тогда взять "запасной" вариант
    
    QByteArray byteArray = ba.mid(6, index-6-2);//6 - индекс начала цифр в идентификаторе сообщения. 2 последние цифры  - это тип сообщения
    idMsg = byteArray.toLongLong();
    if(idMsg <= 0){
        qCritical() << tr("ERROR! Cannot parse MESID of the notification");
        sendBadBlock(clientConnection);
        return -1;
    }

    byteArray = ba.mid(index-2, 2);
    msgType = byteArray.toInt();
    if(msgType < 0){
        qCritical() << tr("ERROR! Cannot parse MESID (type part) of the notification");
        sendBadBlock(clientConnection);
        return -1;
    }


    index = ba.indexOf("&received=");
    if(index < 13){ //если даже числа 1 и 1, то данная подстрока встретится не ранее чем на 13-й позиции
        qCritical() << tr("ERROR! Cannot parse result status of the notification");
        sendBadBlock(clientConnection);
        return -1;
    }

    byteArray = ba.mid(index + 9);
    int receiverResult = byteArray.toInt();
    bool ok = false;

    //обрабатываем пинги
    //если пришел ответ на передачу пинга, то надо обновить значение соответствующего пинга в списке и завершить работу метода
    //если результат квитанции положительный, количество отправленных пингов не увеличиваем, т.к. ждем, пока из целеволй БД придет результат обработки в ней отправленного пинга
    //если отрицательный - то число отправленных пингов увеличиваем на один, т.к. другого ответа не будет
    if(msgType == (int)JKKSMessage::atPing){

        qWarning() << tr("Receive ping notification from gateway. Id = %1, result = %2.").arg(idMsg).arg(receiverResult);
        processPingNotification(idMsg, receiverResult);
        sendOKBlock(clientConnection, true);

        return 1;
    }

    if(receiverResult != 1){
        qCritical() << tr("ERROR: Message with id = %1 and type = %2 does not received by receiver! Result = %3")
                                          .arg(idMsg) 
                                          .arg(msgType)
                                          .arg(receiverResult);

        ok = setMessageAsSended(idMsg, msgType, false);
     }
    else {
        ok = true;//setMessageAsSended(idMsg, msgType);
    }

    if(!ok)
    {
        qCritical() << tr("ERROR: Cannot mark message as sended! Database Error. idMsg = %1, type = %2")
                                            .arg(idMsg)
                                            .arg(msgType);
        sendOKBlock(clientConnection, true);        
    }
    else
    {
        sendOKBlock(clientConnection, true);
    }


    return 1;
}

//обрабатываем пинги
//если пришел ответ на передачу пинга, то надо обновить значение соответствующего пинга в списке и завершить работу метода
//если результат квитанции положительный, количество отправленных пингов не увеличиваем, т.к. ждем, пока из целеволй БД придет результат обработки в ней отправленного пинга
//если отрицательный - то число отправленных пингов увеличиваем на один, т.к. другого ответа не будет
//если все пинги отправлены - генерируем сигналы
int HttpWindow::processPingNotification(int idMsg, int result)
{
    if(result != 1){
        qCritical() << tr("ERROR! Destination organization cannot receive ping!");
        cntPingsSended++;
    }

    for (QMap<QString, JKKSPing>::iterator pa = m_pings.begin(); pa != m_pings.end(); pa++){
        if(pa.value().id() == idMsg){
            pa.value().setState1(result != 1 ? 0 : 1);
            pa.value().setState2(1);
            //pa.value().setState3(0);
            //pa.value().setState4(0);
        }
    }
    
    if(pingsForSent <= cntPingsSended){
        emit pingsSended(m_pings);
        emit pingsSentCompleted();
    }

    return 1;
}

//если пришло сообщение - ответ на пинг, то необходимо обработать его и завершить работу метода
//при этом важно увеличить количество отправленных пингов на один
//и если их кол-во равно кол-ву подготовленных для отправко - сгенерировать сигналы
int HttpWindow::processPingResponse(const JKKSPing * ping)
{
    if(!ping){
        return 1;
    }

    cntPingsSended++;

    for (QMap<QString, JKKSPing>::iterator pa = m_pings.begin(); pa != m_pings.end(); pa++){
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
    if(pingsForSent <= cntPingsSended){
        emit pingsSended(m_pings);
        emit pingsSentCompleted();
    }

    return 1;
}

int HttpWindow::sendPings()
{
    if(m_pings.isEmpty()){
        emit pingsSentCompleted();
        return 0;
    }
    
    QList<JKKSPMessWithAddr *> pingList = JKKSLoader::pingsToPMessWithAddr(m_pings);

    for (QList<JKKSPMessWithAddr *>::const_iterator iterator = pingList.constBegin();iterator != pingList.constEnd();++iterator)
    {
        bool stat = sendOutPing((*iterator)) ;
    }

    while(!pingList.isEmpty())
        delete pingList.takeFirst();

    return 1;

}

bool HttpWindow::sendOutPing(const JKKSPMessWithAddr *ping)
{
    if(ping == NULL)
        return false;

    if(pingHttp == NULL)
        return false;

    QByteArray byteArray = ping->pMess.serialize();  
    if(byteArray.size() == 0)
        return false;
    
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha1);

    QString unp = ping->unp;

    //base64
    byteArray = byteArray.toBase64();
    //zip
    byteArray = qCompress(byteArray, 9);

    //ВАЖНО!!! 
    //Для передачи данных через шлюз ТПС используется механизм псевдофасетного кодирования идентификатора пересылаемого сообщения
    //В конец строкового представления идентификатора пересылаемого сообщения добавляются два байта (две цифры), характеризующие тип
    //(т.е. из какой таблицы взят идентификатор). Эта особенность не влияет на логику обработки пересылаемых данных посредством kksinteractor и http_connector
    //однако в теле почтового сообщения, отдаваемого в шлюз ТПС, учитывается.
    //
    //Формат сообщения: mesid=<ID><type_digit_1><type_digit_2>&unp=<email_prefix>&data="........(base64)......"
    //Если тип имеет одну цифру, перед ней ставится 0. т.е. 00 - atCommand, 02 - atDocument, и т.д.
    
    QString mesId;
    int t = JKKSMessage::atPing; // = 13
    if(t >= 0 && t <= 9){
        mesId = QString("0%1").arg(QString::number(t));
    }
    else{
        mesId = QString("%1").arg(QString::number(t));
    }

    //создаем строку сообщения в унифицированном прикладном протоколе передачи данных ТПС
    QString s = QString("mesid=%1%2&unp=%3&data=\"")
                          .arg(QString::number(ping->id)) //идентификатор пинга. Равен идентификатору организации из таблицы organization
                          .arg(mesId) 
                          .arg(ping->unp.isEmpty() ? QString("1919") : ping->unp);//Условный номер получателя (используем email_prefix)

    byteArray.prepend(s.toAscii());
    byteArray.append("\"");
    //hash-сумма
    byteArray.append("&hash=\"");
    hash = hash.toBase64();
    byteArray.append(hash);
    byteArray.append("\"");
    

    JKKSAddress addr = ping->addr;

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
    
    pingHttp->setHost(h, mode, p);

    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";

    //отправляем пинги асинхронно
    //а далее ждем все ответы, и когда все ответы придут - обновляем набор пингов и начинаем отправку сообщений
    int httpGetId = pingHttp->post ( path, byteArray ) ;

    //мы имеем право взять нужный нам пинг из списка подготовленных на отправку пингов, используя email_prefix организации-получателя пинга
    JKKSPing pp = m_pings.value(ping->pMess.receiverUID());
    pingHttpMessages.insert(httpGetId, pp);

    qWarning() << tr("Ping sending request sheduled. requestId = %1").arg(httpGetId);
  
    return true;
}

//ответ http-сервера на запрос передачи пинга 
void HttpWindow::pingHttpRequestFinished(int requestId, bool error)
{
    bool bFound = false;
    
    uint messCount = pingHttpMessages.count();
    if(messCount <= 0)
        return;
    
    JKKSPing defValue = JKKSPing();
    JKKSPing t = pingHttpMessages.value(requestId, defValue);
    if(t == defValue){
        return;
    }
    
    pingHttpMessages.remove(requestId);

    qWarning() << tr("Ping sending request completed. requestId = ") << requestId;

    //если не смогли отправить пинг - помечаем его как отправленного и неудачно доставленного (приемная сторона не отвечает)
    if (error) {
        qCritical() << tr("ERROR: Data transfer for requestId = %1 failed: %2").arg(requestId).arg(pingHttp->errorString());
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
    QByteArray ba = http->readAll();
    if(ba.length() <= 0 || ba == "OK"){//это сообщение было передано напрямую на целевой объект в http_connector
        qWarning() << tr("Ping request delivered to destination organization. Waiting for answer");
        return;
    }

    //далее полагаем, что ответ пришел от шлюза
    //и в случае, если ответ содержит " ERROR ", помечаем сообщение как отправленное
    if( ba.contains(" ERROR ")){
        qCritical() << tr("ERROR: Data transfer for requestId = %1 failed: %2").arg(requestId).arg(pingHttp->errorString());
        t.setState1(0);
        t.setState2(0);
        t.setState3(0);
        t.setState4(0);
        processPingResponse(&t);
    }
}

void HttpWindow :: sendOKBlock(QTcpSocket * clientConnection, bool withData)
{
    if(!clientConnection)
        return;

    QByteArray block;
    QHttpResponseHeader response = QHttpResponseHeader( 200 , "HTTP/1.1 200 OK" );
	QString str = response.toString();
	block.append(str);
    if(withData)
        block.append("OK");
    
    clientConnection->write(block);
}

void HttpWindow :: sendBadBlock(QTcpSocket * clientConnection)
{
    if(!clientConnection)
        return;

    QByteArray block;
    QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
    QString str = response.toString();
    block.append(str);
    clientConnection->write(block);

}

 void HttpWindow::closeEvent(QCloseEvent *event)
 {
  
     if(tcpServer)
         delete tcpServer;
     
     if(http)
        delete http;
     
     if(pingHttp)
        delete pingHttp;
     
     event->accept();
 }