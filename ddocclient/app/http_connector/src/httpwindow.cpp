
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

    //�����������, ����� � ������ httpRequestFinished ������ ��������� ����� � ������� ������ �� ������ http_messages
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
    
    //������� �������� ������ �� ���������� ����� �����
    messageList = loader->readPingResults(receivers);
    if(messageList.count() > 0 )
    {
        qWarning() << tr("Found %1 ping results for sent").arg(QString::number(messageList.count()));
        httpMessages.clear();

        for (QList<JKKSPMessWithAddr *>::const_iterator iterator = messageList.constBegin();iterator != messageList.constEnd();++iterator)
        {
            bool stat = sendOutMessage((*iterator), true, false) ; //������ �������� �������� �� ��, ��� ������ ����� ������� ��������� ��� ����������. 
                                                                    //����� ����������. �.�. ������� �� ����� �������, ���� �� ���������� �������� ������ ���������
                                                                    //������ ��� ���������� � �������� ����������
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

    //����� �������� ����� ("������") �� ��� �����������, �� ������� ���������� ��������� ������
    //� ������, ���� �� �����-���� ����������� ����� ������ �������� - �� ����� ����������� ������ �� ����������
    //���������� ����� ����������
    //� ����� ���� ��� ������, � ����� ��� ������ ������ - ��������� ����� ������ � �������� �������� ���������
    m_pings = loader->createPings(receivers);
    pingsForSent = m_pings.count();
    cntPingsSended = 0;

    if(pingsForSent > 0)
        qWarning() << tr("Starting to send %1 ping requests.\nSending messages will be suspended until all ping results do not come back").arg(QString::number(pingsForSent));

    //* ��������� ����� � ���� ���� ���� ������� �� ����������*/ 
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
            //������ �������� �������� �� ��, ��� ������ ����� ������� ��������� ��� ����������. 
            //����� ���������. �.�. ������� ����� �������, ���� �� ���������� �������� ������ ���������
            //������ ��� ���������� � �������� ����������
            //�������:
            //1 - ���� ��� �������� ����������� ��������� ��������� ����, �� ����������� �� ����������� �� �����. ������� ��������������� JKKSPing
            //2 - � �������� �������� ������������ ����� ���������� ��������, ����� ������������ ���������� ������ ��������� ������ �� �������� ����� ������ �������. � ��� �� ���������
            //3 - ������ ���������� - �������� ������ ���������, ������ ����� ����� �� �������� � ����������������� ��������. � ����� ����������� ����� �� ������� �����������
            bool stat = sendOutMessage((*iterator), true, false);
            
            //���� ���� ������� ������� �� ���������� ���������
            if(stat == false){
                cntMsgSended++; //���-���� ������� ��� ������������
                
                //���� ��������� ��
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
            isFirst = false;//����� ��� ����� ����� �� �������� �������
        
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
            pM.setReceiverUID(part.receiverUID());
            pM.setSenderUID(part.senderUID());
            
            JKKSPMessWithAddr * pMessWithAddr = new JKKSPMessWithAddr(pM, part.getAddr(), part.id());
            
            //����� ������ ����������� �������� ���������
            bool stat = sendOutMessage(pMessWithAddr, true, eof) ;

            if(!stat){
                cntFilePartsSended++; //���-���� ������� ����� ����� ������������
                if(eof)
                    cntFilesSended++;//���� ���������� ��������� ����� �����, �� � ��� ���� ������� ������������
                
                //���� ��������� ��
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

//����� http-������� �� ������ ���� POST
void HttpWindow::httpRequestFinished(int requestId, bool error)
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

    //���� �� ���������� ����� ����� � ��� �� �������� ���������
    if( (JKKSMessage::JKKSMessageType) t.second == JKKSMessage::atUnknownType)
        return;

    emit httpMessageRemoved(cntMsgSended + cntFilesSended);

    if (error) {
        qCritical() << tr("ERROR: Data transfer for requestId = %1 failed: %2").arg(requestId).arg(http->errorString());
        
        //����� ���� �������� ���������� � ��������� ������� ����������� (� ������).
        //������� �� ����������, ����� ����������� ��������� �� ������� �����.
        QString emailPrefix = loader->getReceiverEmailPrefix(t.first, t.second);
        if(emailPrefix.isEmpty())
            return;//�� ��� ��� �� ������ ����

        JKKSPing p = m_pings.value(emailPrefix);
        p.setState1(0);
        p.setState2(0);
        m_pings.insert(emailPrefix, p);

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
    else{
        //� ��������� ������ �������� ���������� � ������� ����������� (� �����)
        //������ �� ����������
        QString emailPrefix = loader->getReceiverEmailPrefix(t.first, t.second);
        if(emailPrefix.isEmpty())
            return;//�� ��� ��� �� ������ ����

        JKKSPing p = m_pings.value(emailPrefix);
        p.setState1(0);
        p.setState2(0);
        m_pings.insert(emailPrefix, p);

        return;
    }
}

void HttpWindow::slotHttpMessageRemoved(int sendedCount)
{
    //msgForSent + filesForSent ������������ � ������ startProc() ��� ������ ���-�� ��������� � ������ �� ��������

    if((msgForSent + filesForSent) > sendedCount){ 
        return;
    }
    
    //���� �������������� ��������� �� �������� ������ ��� - �������� ������ � ���� ������ �� ��
    qWarning() << tr("Waiting for data to sent...");
    if(manual == false){
        m_timer->start();
    }
    else
        startButton->setEnabled(true); //���� ������� ������ (����������) ����� - ������ ��������� ������ ��������
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

    //��� ������� �� ����� �� ���� ��������� ������� �� ����������
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

        int httpGetId = http->post ( path, byteArray ) ;

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

        qWarning() << tr("Message sending request sheduled. requestId = %1").arg(httpGetId);
		
        eventLoop.exec();
	}
    else 
    {
        int httpGetId = http->post ( path, byteArray ) ;

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
    if(index < 13){ //���� ���� ����� 1 � 1, �� ������ ��������� ���������� �� ����� ��� �� 13-� �������
        sendBadBlock(clientConnection);
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
        qCritical() << "ERROR! Hash sum for income message is inconsistent!";
        sendBadBlock(clientConnection);
        return -1;
    }

    JKKSPMessage pMessage(byteArray);

    //��������� hash-����� ��������� ������
    QByteArray storedHash = pMessage.cryptoHash();
            
    //���� ������ ��������� - ����� �� ����, �� ���������� ���������� ��� � ��������� ������ ������
    //��� ���� ����� ��������� ���������� ������������ ������ �� ����
    //� ���� �� ���-�� ����� ���-�� �������������� ��� �������� - ������������� �������
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

    //� ��������� ����� �� ��������, ��� ��� ����������� �� ���������� ��������� ��������� ��������� � ��
    //�� � �������� ������ �� ������ �������-����������� (�� ��� ����� (���)) ���������� OK
    //��� ����� �� ������� � ���, ��� ���� ������ ��������� ��������� �������
    //��� ���� ��������� ��������� ����� ��������� �������� � ���� ����. ���������
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

//������ ��������� � �������� �� ������-���������� ����� ������������� ���������
//������ ���� ���� ������ ����������� ������ ��� �������������� ����� ���� (���)
//����������� ����� ���� ���� �������� ��� ���������� ��������
//� ��������� ������ �� ������ �������� ��������� ��� ��������������
//� ������ �������� �������� ������ ������ �� ����, �.�. ��� �� ����� �������� ��������� � ������� ����������� � ��� �� ��� ��������� �������� ��� ������������
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
        index = ba.indexOf("&unp"); //��������� ����� ����� "��������" �������
    
    QByteArray byteArray = ba.mid(6, index-6-2);//6 - ������ ������ ���� � �������������� ���������. 2 ��������� �����  - ��� ��� ���������
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
    if(index < 13){ //���� ���� ����� 1 � 1, �� ������ ��������� ���������� �� ����� ��� �� 13-� �������
        qCritical() << tr("ERROR! Cannot parse result status of the notification");
        sendBadBlock(clientConnection);
        return -1;
    }

    byteArray = ba.mid(index + 9);
    int receiverResult = byteArray.toInt();
    bool ok = false;

    //������������ �����
    //���� ������ ����� �� �������� �����, �� ���� �������� �������� ���������������� ����� � ������ � ��������� ������ ������
    //���� ��������� ��������� �������������, ���������� ������������ ������ �� �����������, �.�. ����, ���� �� �������� �� ������ ��������� ��������� � ��� ������������� �����
    //���� ������������� - �� ����� ������������ ������ ����������� �� ����, �.�. ������� ������ �� �����
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

//������������ �����
//���� ������ ����� �� �������� �����, �� ���� �������� �������� ���������������� ����� � ������ � ��������� ������ ������
//���� ��������� ��������� �������������, ���������� ������������ ������ �� �����������, �.�. ����, ���� �� �������� �� ������ ��������� ��������� � ��� ������������� �����
//���� ������������� - �� ����� ������������ ������ ����������� �� ����, �.�. ������� ������ �� �����
//���� ��� ����� ���������� - ���������� �������
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

//���� ������ ��������� - ����� �� ����, �� ���������� ���������� ��� � ��������� ������ ������
//��� ���� ����� ��������� ���������� ������������ ������ �� ����
//� ���� �� ���-�� ����� ���-�� �������������� ��� �������� - ������������� �������
int HttpWindow::processPingResponse(const JKKSPing * ping)
{
    if(!ping){
        return 1;
    }

    cntPingsSended++;

    for (QMap<QString, JKKSPing>::iterator pa = m_pings.begin(); pa != m_pings.end(); pa++){
        //��������� � ������ �� ���� ��� ���������� ����� ��� ������������� � �����������-����������� ����� 
        //(� ������ �� ������������ � �������� JKKSPing::id())
        //�� �������� ������������ �� �������������� �����������-���������� ����� (� ������� �� ����������� �����).
        //�� ��������� � �������� id_external_queue (��������������) �� ������� ��
        if(pa.value().idOrgTo() == ping->idOrgTo()){ 
            pa.value().setVersionTo(ping->versionTo());
            pa.value().setState1(ping->state1());
            pa.value().setState2(ping->state2());
            pa.value().setState3(ping->state3());
            pa.value().setState4(ping->state4());
        }
    }
    
    //pingsForSent ����� ���� ������, ��� cntPingsSended (�.�. ������ ������� ������, ��� �����������)
    //�.�. ����� ������ ����� �� ����, ������� �� ����. ��� ��������, ����� ��������� �������� ������ �� ����, �� �������� ������, �������� ������.
    //� �����, ����� ��������� �����, ����� �� ���� ������.
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

    //�����!!! 
    //��� �������� ������ ����� ���� ��� ������������ �������� ��������������� ����������� �������������� ������������� ���������
    //� ����� ���������� ������������� �������������� ������������� ��������� ����������� ��� ����� (��� �����), ��������������� ���
    //(�.�. �� ����� ������� ���� �������������). ��� ����������� �� ������ �� ������ ��������� ������������ ������ ����������� kksinteractor � http_connector
    //������ � ���� ��������� ���������, ����������� � ���� ���, �����������.
    //
    //������ ���������: mesid=<ID><type_digit_1><type_digit_2>&unp=<email_prefix>&data="........(base64)......"
    //���� ��� ����� ���� �����, ����� ��� �������� 0. �.�. 00 - atCommand, 02 - atDocument, � �.�.
    
    QString mesId;
    int t = JKKSMessage::atPing; // = 13
    if(t >= 0 && t <= 9){
        mesId = QString("0%1").arg(QString::number(t));
    }
    else{
        mesId = QString("%1").arg(QString::number(t));
    }

    //������� ������ ��������� � ��������������� ���������� ��������� �������� ������ ���
    QString s = QString("mesid=%1%2&unp=%3&data=\"")
                          .arg(QString::number(ping->id)) //������������� �����. ����� �������������� ����������� �� ������� organization
                          .arg(mesId) 
                          .arg(ping->unp.isEmpty() ? QString("1919") : ping->unp);//�������� ����� ���������� (���������� email_prefix)

    byteArray.prepend(s.toAscii());
    byteArray.append("\"");
    //hash-�����
    byteArray.append("&hash=\"");
    hash = hash.toBase64();
    byteArray.append(hash);
    byteArray.append("\"");
    

    JKKSAddress addr = ping->addr;

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
    
    pingHttp->setHost(h, mode, p);

    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";

    //���������� ����� ����������
    //� ����� ���� ��� ������, � ����� ��� ������ ������ - ��������� ����� ������ � �������� �������� ���������
    int httpGetId = pingHttp->post ( path, byteArray ) ;

    //�� ����� ����� ����� ������ ��� ���� �� ������ �������������� �� �������� ������, ��������� email_prefix �����������-���������� �����
    JKKSPing pp = m_pings.value(ping->pMess.receiverUID());
    pingHttpMessages.insert(httpGetId, pp);

    qWarning() << tr("Ping sending request sheduled. requestId = %1").arg(httpGetId);
  
    return true;
}

//����� http-������� �� ������ �������� ����� 
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

    //���� �� ������ ��������� ���� - �������� ��� ��� ������������� � �������� ������������� (�������� ������� �� ��������)
    if (error) {
        qCritical() << tr("ERROR: Data transfer for requestId = %1 failed: %2").arg(requestId).arg(pingHttp->errorString());
        t.setState1(0);
        t.setState2(0);
        t.setState3(0);
        t.setState4(0);
        processPingResponse(&t);
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
        qWarning() << tr("Ping request delivered to destination organization. Waiting for answer");
        return;
    }

    //����� ��������, ��� ����� ������ �� �����
    //� � ������, ���� ����� �������� " ERROR ", �������� ��������� ��� ������������
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