
#include <QtGui>
#include <QtNetwork>
#include <QCryptographicHash>

#include "httpwindow.h"
#include "timerform.h"
#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <JKKSIOUrl.h>
#include <JKKSMessage.h>
#include <kksclient_name.h>

dds_HttpWindow::dds_HttpWindow(dyndoc_mainStructs::netThreadInf rhs, QWidget *parent)
    : QDialog(parent)
{
    dataBase = new dyndoc_mainStructs::dbInf;
    transport = new dyndoc_mainStructs::transportInf;
    timer = new dyndoc_mainStructs::timerInf;

    dataBase->dbName = rhs.dataBase.dbName;
    dataBase->userName = rhs.dataBase.userName;
    dataBase->password = rhs.dataBase.password;

    dataBase->hostAddress = rhs.dataBase.hostAddress;
    dataBase->port = rhs.dataBase.port;

    transport->transportProtocol = rhs.transport.transportProtocol;

    transport->serverport = rhs.transport.serverport;
    
    transport->gateway.address = rhs.transport.gateway.address;
    transport->gateway.port = rhs.transport.gateway.port;

    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, transport->serverport)) {
        qCritical() << tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString());
        QMessageBox::critical(this, tr("DynamicDocs Interactor "),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }

    loader = new (std::nothrow) JKKSLoader (dataBase->hostAddress,dataBase->dbName,dataBase->userName,dataBase->password,dataBase->port,transport->transportProtocol);
    localDBInfo = QString("DBHost: %1, DBName: %2").arg(dataBase->hostAddress).arg(dataBase->dbName);
    if (loader && loader->connectToDb ())
    {
     
    }
    else if (loader)
    {
        qCritical() << tr("Unable to connect to the database: %1.")
                              .arg(dataBase->hostAddress);
        QMessageBox::critical(this, tr("DynamicDocs Interactor"),
                              tr("Unable to connect to the database: %1.")
                              .arg(dataBase->hostAddress));
        close();
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(loadData()));

    //statusLabel = new QLabel(tr("Waiting for data to sent..."));

    //startButton = new QPushButton(tr("Send"));
    //startButton->setDefault(true);
    //quitButton = new QPushButton(tr("Quit"));
    //quitButton->setAutoDefault(false);

    //buttonBox = new QDialogButtonBox;
    //buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    //buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    http = new QHttp(this);

    //срабатывает, когда в методе httpRequestFinished пришел очередной ответ и удалена запись из списка http_messages
    connect(this, SIGNAL(httpMessageRemoved(int)), this, SLOT(slotHttpMessageRemoved(int)));

    connect(http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
    connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)));

    connect(startButton, SIGNAL(clicked()), this, SLOT(startTimer()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(statusLabel);
    //mainLayout->addWidget(buttonBox);
    //setLayout(mainLayout);

    //setWindowTitle(tr("DynamicDocs Interactor"));

    user_timer = 36000; //by defaul set timer


    //dds_TimerForm * timerForm = new dds_TimerForm ();
    
    //if (!timerForm)
        //return ;
    
    //if (timerForm->exec () != QDialog::Accepted)
    //{
        //delete timerForm;
        //manual
        //manual = true;
    //}
    //else
    //{
        //manual = false;
        //user_timer = timerForm->getTimer();

        startTimer();
        //delete timerForm;
    //}

    msgForSent = 0;
    filesForSent = 0;
    cntFilesSended = 0;
    cntMsgSended = 0;
    cntFilePartsSended = 0;
    filePartsForSent = 0;
}

dds_HttpWindow::~dds_HttpWindow()
{
    delete dataBase;
    delete transport;
    delete timer;
}

void dds_HttpWindow::startTimer()
{
    
    //startButton->setEnabled(false);

    //if(manual == false)
    //{
       // m_timer = new QTimer(this);
        //connect(m_timer, SIGNAL(timeout()), this, SLOT(startProc()));
        //m_timer->setInterval(user_timer);
       // m_timer->start();
    //}
    //else
    //{
		startProc();
    //}
}

void dds_HttpWindow::startProc()

{
    //if(manual == false){
        //m_timer->stop();
    //}

    //statusLabel->setText(tr("Waiting for data to sent..."));
   
    //messageList = loader->readMessages();
    QList<JKKSFilePart *> files;// = loader->readFileParts();
    
    filesForSent = files.count();
    msgForSent = messageList.size();
    filePartsForSent = 0;

    if(msgForSent + filesForSent <= 0){
        //startButton->setEnabled(true);
        if(manual == false){
            //m_timer->start();
        }
        return;
    }
        
    cntMsgSended = 0;
    cntFilesSended = 0;
    cntFilePartsSended = 0;

    //statusLabel->setText(tr("Data transferring started ...\n\n"
    //                        "Messages for transfer: %1 ---> %3\n"
    //                        "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
    //                        .arg(msgForSent)
    //                        .arg(filesForSent)
    //                        .arg(cntMsgSended)
    //                        .arg(cntFilesSended)
    //                        .arg(filePartsForSent)
    //                        .arg(cntFilePartsSended));
    //statusLabel->setText(tr("Found messages for transferring: %1\nTransfered: %2").arg(QString::number(msgForSent + filesForSent)).arg(QString::number(0)));

    if(msgForSent > 0 )
    {

        httpMessages.clear();

        for (QList<JKKSPMessWithAddr *>::const_iterator iterator = messageList.constBegin();iterator != messageList.constEnd();++iterator)
        {
            bool stat = sendOutMessage((*iterator), true, false) ;
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

        //statusLabel->setText(tr("Data transferring started ...\n\n"
        //                        "Messages for transfer: %1 ---> %3\n"
        //                        "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
        //                        .arg(msgForSent)
        //                        .arg(filesForSent)
        //                        .arg(cntMsgSended)
        //                        .arg(cntFilesSended)
        //                        .arg(filePartsForSent)
        //                        .arg(cntFilePartsSended));

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
            //pM.receiverUID = part.getAddr();
            //pM.senderUID = part.getSenderAddr();
            
            JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr(pM, part.getAddr(), part.id());
            
            bool stat = sendOutMessage(pMessWithAddr, true, eof) ;
            
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
void dds_HttpWindow::httpRequestFinished(int requestId, bool error)
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
    
    qWarning() << "In httpRequestFinished() requestId = " << requestId;
    
    uint messCount = httpMessages.count();
    if(messCount <= 0)
        return;
    
    QPair<qint64, qint64> defValue = QPair<qint64, qint64>();
    QPair<qint64, qint64> t = httpMessages.value(requestId, defValue);
    if(t == defValue){
        return;
    }
    
    qWarning() << "In httpRequestFinished(). Found corresponding request. requestId = " << requestId;
    emit needToExitEventLoop();

    httpMessages.remove(requestId);
    
    if( (JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atFilePart)
        cntFilesSended++;
    else if((JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atUnknownType)
        cntFilePartsSended++;
    else
        cntMsgSended++;

    //statusLabel->setText(tr("Data transferring started ...\n\n"
    //                        "Messages for transfer: %1 ---> %3\n"
    //                        "Files for transfer: %2 (for current file: %5 parts) ---> %4 (for current file: %6 parts)")
    //                        .arg(msgForSent)
    //                        .arg(filesForSent)
    //                        .arg(cntMsgSended)
    //                        .arg(cntFilesSended)
    //                        .arg(filePartsForSent)
    //                        .arg(cntFilePartsSended));
    //statusLabel->setText(tr("Found messages for transferring: %1\nTransfered: %2").arg(QString::number(msgForSent+filesForSent)).arg(QString::number(cntMsgSended)));

    //если мы отправляли часть файла и она не является последней
    if( (JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atUnknownType)
        return;

    emit httpMessageRemoved(cntMsgSended + cntFilesSended);

    if (error) {
        qCritical() << tr("ERROR: Data transfer for requestId = %1 failed: %2").arg(requestId).arg(http->errorString());
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

}

void dds_HttpWindow::slotHttpMessageRemoved(int sendedCount)
{
    //msgForSent + filesForSent выставляется в методе startProc() при чтении кол-ва сообщений и файлов на отправку

    if(httpMessages.count() > 0 || (msgForSent + filesForSent) != sendedCount){ 
        return;
    }
    
    //если подготовленных сообщений на отправку больше нет - включаем таймер и ждем данных из БД
    //if(manual == false){
        //m_timer->start();
    //}
    //else
        //startButton->setEnabled(true); //если активен ручной (отладочный) режим - делаем доступной кнопку отправки
}

void dds_HttpWindow::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    qWarning() << "In readResponseHeader()";

    int status = responseHeader.statusCode();
    QString str = responseHeader.toString();
    switch (status) {
    case 200:                   // Ok
    case 301:                   // Moved Permanently
    case 302:                   // Found
    case 303:                   // See Other
    case 307:                   // Temporary Redirect
    case 205:
        // these are not error conditions
        break;

    default: break;

        //if(manual == false){
        //    m_timer->start();
        //}
    }
}

bool dds_HttpWindow::setMessageAsSended(const qint64 & id, const int & type, bool sended)
{
    bool result = false;
    if(loader)
        result = loader->setAsSended(id, type, sended);

    return result;
}

bool dds_HttpWindow::sendOutMessage(const JKKSPMessWithAddr * message,
                                bool filePartsFlag,
                                bool isLastFilePart)
{
    
    qWarning() << "In sendOutMessage() ";

    if(message == NULL)
        return false;

    if(http == NULL)
        return false;

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
                          .arg(QString::number(message->id)) //идентификатор сообщения из соответствующей таблице
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

        qWarning() << "1 before post-> ";
        httpGetId = http->post ( path, byteArray ) ;

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
		
        //qWarning() << "2 after post-> ";

        eventLoop.exec();
        qWarning() << "3 after eventLoop.exec() ";
	}
    else 
    {
        httpGetId = http->post ( path, byteArray ) ;

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
    }
  
    return true;
}

void dds_HttpWindow::loadData()
{
    qWarning() << "In loadData() ";

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

        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 200 , "HTTP/1.1 200 OK" );
		QString str = response.toString();
		block.append(str);
        clientConnection->write(block);
        
        clientConnection->disconnectFromHost();
        
        return;
    }

    QHttpRequestHeader header( all_data );

    bool b = header.hasContentLength();

    if(b)
    {
        qWarning() << "In loadData. Data found!";

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
            QByteArray block;

            QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
            QString str = response.toString();
            block.append(str);
            clientConnection->write(block);
        }
     }
    else
    {
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
    }

    clientConnection->disconnectFromHost();
}

int dds_HttpWindow::processMessage(const QByteArray & ba, QTcpSocket * clientConnection)
{

    if(!ba.contains("mesid=") ||  (!ba.contains("&uno=") && !ba.contains("&unp=")) || !ba.contains("&data=\"") || !ba.contains("&hash=\"")){
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
        return -1;
    }

    int index = ba.indexOf("&data=\"");
    if(index < 13){ //если даже числа 1 и 1, то данная подстрока встретится не ранее чем на 13-й позиции
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
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
        qCritical() << "ERROR! Hash sum inconsistent!";
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
        return -1;
    }

    JKKSPMessage pMessage(byteArray);

    //эталонная hash-сумма пришедших данных
    QByteArray storedHash = pMessage.cryptoHash();
            
    int res = loader->writeMessage(pMessage);
    if(res <= 0){
        qDebug() << "Message type: " << pMessage.getType();
        qDebug() << "Write message status : " << res;
    }

   
    QHttpResponseHeader response; 
    QByteArray block;
    QString str;

    //в настоящее время мы полагаем, что вне зависимости от результата обработки входящего сообщения в БД
    //мы в качестве ответа на запрос объекта-отправителя (ну или шлюза (ТПС)) возвращаем OK
    //тем самым мы говорим о том, что факт приема сообщения состоялся успешно
    //при этом результат обработки будет отправлен отдельно в виде спец. квитанции
    if(res == ERROR_CODE)
    {
		response = QHttpResponseHeader( 400 , "HTTP/1.1 200 OK" );
		str = response.toString();
		block.append(str);
        block.append("OK");

        clientConnection->write(block);
    }
    else if(res == OK_CODE)
    {
		response = QHttpResponseHeader( 200 , "HTTP/1.1 200 OK" );
		QString str = response.toString();
		block.append(str);
        block.append(QString("OK"));
        clientConnection->write(block);
    }
    else if(res == IGNORE_CODE)
    {
		response = QHttpResponseHeader( 204 , "HTTP/1.1 200 OK" );
		QString str = response.toString();
		block.append(str);
        block.append("OK");
        clientConnection->write(block);
    }

    return 1;
}

//пришла квитанция о доставке на объект-получатель ранее отправленного сообщения
//данный блок кода должен исполняться только при взаимодействии через шлюз (ТПС)
//результатом может быть факт успешной или неуспешной доставки
//в последнем случае мы должны пометить сообщение как неотправленное
//в случае успешной доставки ничего делать не надо, т.к. еще на этапе отправки сообщения с объекта отправителя в ТПС мы это сообщение отметили как доставленное
int dds_HttpWindow::processNotification(const QByteArray & ba, QTcpSocket * clientConnection)
{
    if(!ba.contains("mesid=") ||  (!ba.contains("&uno=") && !ba.contains("&unp=")) || !ba.contains("&received=")){
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
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
        qCritical() << "ERROR! Cannot parse MESID of the notification";
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
        return -1;
    }

    byteArray = ba.mid(index-2, 2);
    msgType = byteArray.toInt();
    if(msgType < 0){
        qCritical() << "ERROR! Cannot parse MESID (type part) of the notification";
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
        return -1;
    }


    index = ba.indexOf("&received=");
    if(index < 13){ //если даже числа 1 и 1, то данная подстрока встретится не ранее чем на 13-й позиции
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
        return -1;
    }

    QHttpResponseHeader response; 
    QByteArray block;
    QString str;

    byteArray = ba.mid(index + 9);
    int receiverResult = byteArray.toInt();
    bool ok = false;

    if(receiverResult != 1){
        qCritical() << "ERROR: Message with id = " << idMsg << " and type = " << msgType << " does not received by receiver! Result = " << receiverResult;
        
		response = QHttpResponseHeader( 200 , "HTTP/1.1 200 OK" );
		QString str = response.toString();
		block.append(str);
        block.append(QString("OK"));

        clientConnection->write(block);
        ok = setMessageAsSended(idMsg, msgType, false);
     }
    else {
        ok = true;//setMessageAsSended(idMsg, msgType);
    }

    if(!ok)
    {
        qCritical() << "ERROR: Cannot mark message as sended! Database Error. idMsg = " << idMsg << " type = " << msgType;
        
        response = QHttpResponseHeader( 400 , "HTTP/1.1 200 OK" );
		str = response.toString();
		block.append(str);
        block.append("OK");

        clientConnection->write(block);
    }
    else
    {
		response = QHttpResponseHeader( 200 , "HTTP/1.1 200 OK" );
		QString str = response.toString();
		block.append(str);
        block.append(QString("OK"));

        clientConnection->write(block);
    }


    return 1;
}
