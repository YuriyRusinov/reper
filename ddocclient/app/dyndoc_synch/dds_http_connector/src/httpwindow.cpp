
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

    //�����������, ����� � ������ httpRequestFinished ������ ��������� ����� � ������� ������ �� ������ http_messages
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
        
            if(readed == -1)//��� ������ ������ ������� �� ��������� ����, ������� �� ��������
                break;

            bool eof = false;
            if(readed == 0 || readed < block){//����� ����� ���������
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

//����� http-������� �� ������ ���� POST
void dds_HttpWindow::httpRequestFinished(int requestId, bool error)
{
    /*
        ����� �������� ��� ��������
        1) ���������� ������ ���� POST, ������� ��� ������� �� ������� ������ �������� (������� ������������ http_connector)
           � ���� ������ readAll ������ ������� ���� OK, ���� ������, ������ � ��������� ������ �������� error ������ ���� ����� TRUE.
           � ������ �� �� ������ �������� ���������� (������  ������������� ��������� ����������� ������ ����� � ����.���������!!!) 
           ��������� ��� ������������ (setAsSended).
           ��� ������ - ������� ��������� �� ������.
           ���������, ������� �������� � �� ��� ��������������, ����� ��� ��������� ������ �� ����� ��������� � ������������ ������

        2) ���������� ������ ���� POST, ������� ��� ������� � ���� (���). � ���� ������ ����� �� ������� ��������� �� ��� � ���, 
           "����������" �� ��� ��� ��������� ��� ��� ��� ��������������� �������� ������.
           ��� ������ �� ���� ������ ������� �������������� �������� � ��. ������ ����� ������� ��������� �� ������.
           ��� ������ - ���������� �������� ��������� � �� ��� ������������ (���� ��������� ������ �����)
           ����� ����, ��� �������������� ����� ���� (���) ������ ��������� � ����� ��������� ������ ��������� �� �������� �������
           ����������� ������ ��������� (����� processNotification), � ������, ���� ��� �������� ��� ����� �� ����� 1, 
           �� ����������� ������ ���������, ��� ��������� ��������� �������� (��������������).
           ���������, ������� �������� � �� ��� ��������������, ����� ��� ��������� ������ �� ����� ��������� � ������������ ������

       � ����� ������ �� ������ ����� ������� ��������� �� ������ httpMessages. 
       ���� ��� � ������ ��� - ��������� ������ �� ������ (��� ��� ����� ���� ������ ������� ����)
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

    //���� �� ���������� ����� ����� � ��� �� �������� ���������
    if( (JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atUnknownType)
        return;

    emit httpMessageRemoved(cntMsgSended + cntFilesSended);

    if (error) {
        qCritical() << tr("ERROR: Data transfer for requestId = %1 failed: %2").arg(requestId).arg(http->errorString());
        return;
    } 
    
    //����� �� ��������, ��� ���� ������ ����� �� ��������� � requestId �� ������ ������� (� error = false), 
    //�� ������ ���������� ���� ����� ���������
    //���� ���� == OK ��� ��� ������, �� ��� ������ ����� ���� �������������� �������� � http_connector'�� 
    //� ��������� ������ ����� ������ ���� � �������
    // 1 OK
    // 1 ERROR 15
    //� ��� ��������, ��� �������������� �������������� ����� ���� (���)
    QByteArray ba = http->readAll();
    if(ba.length() <= 0 || ba == "OK"){//��� ��������� ���� �������� �������� �� ������� ������ � http_connector
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << "ERROR: Cannot mark message as sended! Database Error";
        }
        return;
    }

    //����� ��������, ��� ����� ������ �� �����
    //� � ������, ���� ����� �� �������� " ERROR ", �������� ��������� ��� ������������
    if( ! ba.contains(" ERROR ")){
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << "ERROR: Cannot mark message as sended! Database Error";
        }
    }

}

void dds_HttpWindow::slotHttpMessageRemoved(int sendedCount)
{
    //msgForSent + filesForSent ������������ � ������ startProc() ��� ������ ���-�� ��������� � ������ �� ��������

    if(httpMessages.count() > 0 || (msgForSent + filesForSent) != sendedCount){ 
        return;
    }
    
    //���� �������������� ��������� �� �������� ������ ��� - �������� ������ � ���� ������ �� ��
    //if(manual == false){
        //m_timer->start();
    //}
    //else
        //startButton->setEnabled(true); //���� ������� ������ (����������) ����� - ������ ��������� ������ ��������
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

    //�����!!! 
    //��� �������� ������ ����� ���� ��� ������������ �������� ��������������� ����������� �������������� ������������� ���������
    //� ����� ���������� ������������� �������������� ������������� ��������� ����������� ��� ����� (��� �����), ��������������� ���
    //(�.�. �� ����� ������� ���� �������������). ��� ����������� �� ������ �� ������ ��������� ������������ ������ ����������� kksinteractor � http_connector
    //������ � ���� ��������� ���������, ����������� � ���� ���, �����������.
    //
    //������ ���������: mesid=<ID><type_digit_1><type_digit_2>&unp=<email_prefix>&data="........(base64)......"
    //���� ��� ����� ���� �����, ����� ��� �������� 0. �.�. 00 - atCommand, 02 - atDocument, � �.�.
    
    QString mesId;
    int t = message->pMess.getType(); //��� ���������. ��. ���� ��������� � ������������ JKKSMessage::JKKSMessageType
    if(t >= 0 && t <= 9){
        mesId = QString("0%1").arg(QString::number(t));
    }
    else{
        mesId = QString("%1").arg(QString::number(t));
    }

    //������� ������ ��������� � ��������������� ���������� ��������� �������� ������ ���
    QString s = QString("mesid=%1%2&unp=%3&data=\"")
                          .arg(QString::number(message->id)) //������������� ��������� �� ��������������� �������
                          .arg(mesId) 
                          .arg(message->unp.isEmpty() ? QString("1919") : message->unp);//�������� ����� ���������� (���������� email_prefix)

    byteArray.prepend(s.toAscii());
    byteArray.append("\"");
    //hash-�����
    byteArray.append("&hash=\"");
    hash = hash.toBase64();
    byteArray.append(hash);
    byteArray.append("\"");
    

    JKKSAddress addr = message->addr;

    QString recvHost;
    int recvPort;

    //���� � ���������� ���������� �������-���������� �������, ��� ��� �������� ��������� ���������� ������������ ���� (���) ...
    // � ��� ���� ��������� http_connector ��������� � ����� ����� (���)
    if(addr.useGateway() && !gatewayHost.isEmpty() && gatewayPort > 0){
        recvHost = gatewayHost;
        recvPort = gatewayPort;
    }
    else{ //� ��������� ������ ���������� ��������� �������� �� http_connector �������� �������
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

        if(message->pMess.getType() != JKKSMessage::atFilePart){//��� ������, ������������ ������� ���������� � ���� ������ ��������� ��������
            httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
        }
        else{
            if(isLastFilePart){
                //��� �������� � ����, ��� � ������ httpRequestFinished ��������� ����� ������ �������� ����� ��������� ������ �������, ����� �������� ��������� ����� �����
                httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
            }
            else{
                //��� �������� � ����, ��� � ������ httpRequestFinished ��������� ����� �������� ����� �� ����������, ������ ������������� ������, ������� �������� eventLoop
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

        if(message->pMess.getType() != JKKSMessage::atFilePart){//��� ������, ������������ ������� ���������� � ���� ������ ��������� ��������
            httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
        }
        else{
            if(isLastFilePart){
                //��� �������� � ����, ��� � ������ httpRequestFinished ��������� ����� ������ �������� ����� ��������� ������ �������, ����� �������� ��������� ����� �����
                httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
            }
            else{
                //��� �������� � ����, ��� � ������ httpRequestFinished ��������� ����� �������� ����� �� ����������, ������ ������������� ������, ������� �������� eventLoop
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

        //�������� ������ ����� ���� �� ��������� ��������:
        //mesid=<ID><type_digit_1><type_digit_2>&uno=<email_prefix>&data="........(base64)......"
        //mesid=<ID><type_digit_1><type_digit_2>&uno=<email_prefix>&received=<code>
        //� ������ ������ ��� mesid � uno �� ���������. �� ���� � ����������� �� ���� ��������� ��������� ��� ����������
        //�� ������ ������ ��� ��������� ������������� � ��� ���������, ������� ���� �������� � ���������� � ��������� � received �����������

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
    if(index < 13){ //���� ���� ����� 1 � 1, �� ������ ��������� ���������� �� ����� ��� �� 13-� �������
        QByteArray block;

        QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
        QString str = response.toString();
        block.append(str);
        clientConnection->write(block);
        return -1;
    }

    QByteArray byteArray = ba.mid(index + 7);
    int h = byteArray.indexOf("&hash=\"");
    //��������� hash-����� ��������� ������
    QByteArray etaloneHash = byteArray.mid(h+7, byteArray.length() - (h+7) - 1);
    etaloneHash = QByteArray::fromBase64(etaloneHash);
    
    byteArray = byteArray.left(h-1);
    byteArray = qUncompress(byteArray);
    byteArray = QByteArray::fromBase64(byteArray);

    //����������� hash-����� ��������� ������
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

    //��������� hash-����� ��������� ������
    QByteArray storedHash = pMessage.cryptoHash();
            
    int res = loader->writeMessage(pMessage);
    if(res <= 0){
        qDebug() << "Message type: " << pMessage.getType();
        qDebug() << "Write message status : " << res;
    }

   
    QHttpResponseHeader response; 
    QByteArray block;
    QString str;

    //� ��������� ����� �� ��������, ��� ��� ����������� �� ���������� ��������� ��������� ��������� � ��
    //�� � �������� ������ �� ������ �������-����������� (�� ��� ����� (���)) ���������� OK
    //��� ����� �� ������� � ���, ��� ���� ������ ��������� ��������� �������
    //��� ���� ��������� ��������� ����� ��������� �������� � ���� ����. ���������
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

//������ ��������� � �������� �� ������-���������� ����� ������������� ���������
//������ ���� ���� ������ ����������� ������ ��� �������������� ����� ���� (���)
//����������� ����� ���� ���� �������� ��� ���������� ��������
//� ��������� ������ �� ������ �������� ��������� ��� ��������������
//� ������ �������� �������� ������ ������ �� ����, �.�. ��� �� ����� �������� ��������� � ������� ����������� � ��� �� ��� ��������� �������� ��� ������������
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
        index = ba.indexOf("&unp"); //��������� ����� ����� "��������" �������
    
    QByteArray byteArray = ba.mid(6, index-6-2);//6 - ������ ������ ���� � �������������� ���������. 2 ��������� �����  - ��� ��� ���������
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
    if(index < 13){ //���� ���� ����� 1 � 1, �� ������ ��������� ���������� �� ����� ��� �� 13-� �������
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
