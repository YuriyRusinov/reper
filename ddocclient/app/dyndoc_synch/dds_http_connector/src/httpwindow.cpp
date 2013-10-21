/****************************************************************************
**
** Copyright (C) 2004-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <QtGui>
#include <QtNetwork>

#include "httpwindow.h"
#include "timerform.h"
//#include "ui_authenticationdialog.h"
#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <JKKSIOUrl.h>
#include <JKKSMessage.h>

//#define _HTTP_DEBUG

dds_HttpWindow::dds_HttpWindow(QWidget *parent)
    : QDialog(parent)
{
    QSettings settings (QCoreApplication::applicationDirPath ()+"/http.ini", QSettings::IniFormat);
    
    QString dbName;//(settings.value("Database/database", "tsync_db1").toString());
    QString host;//(settings.value("Database/host", "192.168.17.176").toString());
    QString user;//(settings.value("Database/user", "jupiter").toString());
    QString password;//(settings.value("Database/password", "111").toString());
    int port;//(settings.value("Database/port", "5432").toInt());
    
    QString http_host;//(settings.value("Http/host", "192.168.17.56").toString()); //адрес приложени€ "—ервер"
    int http_port;//(settings.value("Http/port", "6000").toInt());// порт сервера

    int transport;//(settings.value("Http/transport","1").toInt());  
    QString addr;//(settings.value("Http/local","a01").toString());

    //QString server_host;//(settings.value("Http/server_host", "192.168.17.56").toString());
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
    //addr = settings.value ("portal", "a01").toString ();
    addr = QString();//!!!!
    settings.endGroup ();

    settings.beginGroup ("Http");
    http_host = settings.value ("host", "127.0.0.1").toString ();
    http_port = settings.value ("port", "8001").toInt ();
  
    //server_host = settings.value ("server_host", "127.0.0.1").toString ();
    server_port = settings.value ("server_port", "8001").toInt ();
    settings.endGroup ();

    //QHostAddress address(server_host);

    urlLineEdit = new QLineEdit("http://" + http_host + ":" + QString::number(http_port) + "/" + addr);


    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, server_port)) {
        QMessageBox::critical(this, tr("DynamicDocs Interactor "),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }



    loader = new (std::nothrow) JKKSLoader (host, dbName, user, password, port, transport);
    localDBInfo = QString("DBHost: %1, DBName: %2").arg(host).arg(dbName);
    if (loader && loader->connectToDb ())
    {
     
    }
    else if (loader)
    {
        QMessageBox::critical(this, tr("DynamicDocs Interactor"),
                              tr("Unable to connect to the database: %1.")
                              .arg(host));
        close();
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(loadData()));


    urlLabel = new QLabel(tr("&URL:"));
    urlLabel->setBuddy(urlLineEdit);
    statusLabel = new QLabel(tr("Please enter the URL for system where you want send datas."));

    startButton = new QPushButton(tr("Send"));
    startButton->setDefault(true);
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    progressDialog = new QProgressDialog(this);

    http = new QHttp(this);

    //срабатывает, когда в методе httpRequestFinished пришел очередной ответ и удалена запись из списка http_messages
    connect(this, SIGNAL(httpMessageRemoved(int)), this, SLOT(slotHttpMessageRemoved(int)));

    connect(urlLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableDownloadButton()));
    connect(http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
    connect(http, SIGNAL(dataReadProgress(int, int)),
            this, SLOT(updateDataReadProgress(int, int)));
    connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
/*  connect(http, SIGNAL(authenticationRequired(const QString &, quint16, QAuthenticator *)),
            this, SLOT(slotAuthenticationRequired(const QString &, quint16, QAuthenticator *)));
#ifndef QT_NO_OPENSSL
    connect(http, SIGNAL(sslErrors(const QList<QSslError> &)),
            this, SLOT(sslErrors(const QList<QSslError> &)));
#endif */
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(startTimer()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(urlLabel);
    topLayout->addWidget(urlLineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(buttonBox);
    mainLayout->addWidget(progressDialog);
    setLayout(mainLayout);


    //responses << "HTTP/1.1 200 OK ";
    //responses << "Date:";
    //responses << "Server: DYNAMICDOCS INTERACTOR/1.0.0";
    //responses << "Last-Modified: Mon, 14 Jun 2010 06:04:27 GMT";
    //responses << "Content-Type: text/html";
    //responses << "Content-Length: 500";
    //responses << "Content-Location: http://";

    setWindowTitle(tr("DynamicDocs Interactor"));
    urlLineEdit->setFocus();

    user_timer = 36000; //by defaul set timer


    dds_TimerForm * timerForm = new dds_TimerForm ();
    
    if (!timerForm)
        return ;
    
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
		//if(user_timer < 10000)
		//{
		//	user_timer = 36000;
		//}

        startTimer();
        delete timerForm;
    }
}


void dds_HttpWindow::startTimer()
{
    
    startButton->setEnabled(false);
    urlLineEdit->setEnabled(false);

    if(manual == false)
	{
		//QTimer *timer = new QTimer(this);
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

void dds_HttpWindow::startProc()

{
#ifdef _HTTP_DEBUG
    qWarning() << "Entered to startProc()" << localDBInfo;
#endif

    if(manual == false){
        m_timer->stop();
#ifdef _HTTP_DEBUG
        qWarning() << "Timer stopped" << localDBInfo;
#endif
    }
    
    //QUrl url(urlLineEdit->text());
 

    //QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    //http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
    
    /*if (!url.userName().isEmpty())
        http->setUser(url.userName(), url.password());*/

    httpRequestAborted = false;
    httpTransferComplete = false;

    //QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    //if (path.isEmpty())
    //    path = "/";


    
    messageList = loader->readMessages();
    count_send = 0;


    if(messageList.size() > 0 )
    {

        progressDialog->setWindowTitle(tr("DynamicDocs Interactor"));
        progressDialog->setLabelText(tr("Sendind data %1.").arg("DynamicDocs Interactor"));

        progressDialog->show();
        progressDialog->setMaximum(messageList.size());

        httpMessages.clear();

        for (QList<JKKSPMessWithAddr *>::const_iterator iterator = messageList.constBegin();iterator != messageList.constEnd();++iterator)
        {
            bool stat = sendOutMessage((*iterator)) ;
        }

        while(!messageList.isEmpty())
            delete messageList.takeFirst();
    }
    else{
        startButton->setEnabled(true);
        urlLineEdit->setEnabled(true);
        if(manual == false){
            m_timer->start();
#ifdef _HTTP_DEBUG
            qWarning() << "Timer started" << localDBInfo;
#endif
        }
    }

    QList<JKKSFilePart *> files = loader->readFileParts();
    qint64 position = 0;
    int block = _MAX_FILE_BLOCK2;
    int cnt = files.count();
    
    for(int i=0; i<cnt; i++){
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
        
            if(readed == -1)//при ошибке чтени€ выходим на следующий файл, текущий не передаем
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
            
            bool stat = sendOutMessage(pMessWithAddr) ;
            
            if(eof){
                httpMessages.insert(httpGetId, qMakePair(pMessWithAddr->id, (qint64)pMessWithAddr->pMess.getType()) );
                break;
            }
            
            delete pMessWithAddr;
        }
        while(1);
    }

    while(!files.isEmpty())
        delete files.takeFirst();

}

void dds_HttpWindow::cancelDownload()
{
#ifdef _HTTP_DEBUG
    qWarning() << "Entered to cancelDownload" << localDBInfo;
#endif

    statusLabel->setText(tr("Data transfer canceled."));
    httpRequestAborted = true;
    http->abort();
    startButton->setEnabled(true);
    
    if(manual == false){
        m_timer->start();
#ifdef _HTTP_DEBUG
        qWarning() << "Timer started" << localDBInfo;
#endif
    }

}

void dds_HttpWindow::httpRequestFinished(int requestId, bool error)
{
    bool bFound = false;
    uint messCount = httpMessages.count();
    if(messCount > 0)
    {
        QPair<qint64, qint64> defValue = QPair<qint64, qint64>();
        QPair<qint64, qint64> t = httpMessages.value(requestId, defValue);
        if(t == defValue){
            bFound = false;
        }
        else
        {
            if(!httpRequestAborted){
                if(!setMessageAsSended(t.first, t.second))
    		    {
                    qWarning() << "ERROR: Cannot mark message as sended! Database Error";
                }
            }

            bFound = true;
            httpMessages.remove(requestId);
            emit httpMessageRemoved(count_send++);
        }
    }

    if(messCount > 0){
        if(!bFound)
        {}//qWarning() << "ERROR: Cannot find corresponding requestId in http_message map!";
        else{   
            if (error) {
                httpRequestAborted = true;
                statusLabel->setText(tr("ERROR: Data transfer failed: %1.").arg(http->errorString()));
                qWarning() << tr("ERROR: Data transfer failed: %1.").arg(http->errorString());
            } 
	        else {
                statusLabel->setText(tr("Data transfer http %1 complete.").arg(requestId));
                httpTransferComplete = true;
            }
        }
    }
}

void dds_HttpWindow::slotHttpMessageRemoved(int progress)
{
    //qWarning() << "Entered in slotHttpMessageRemoved()";

    //maximum выставл€етс€ в методе startProc() при чтении кол-ва сообщений на отправку
    if(progressDialog->maximum () == progress){
        progressDialog->setValue(progress);
        progressDialog->hide();
    }
    else{
        progressDialog->setValue(progress);
    }

    if(httpMessages.count() > 0){ 
#ifdef _HTTP_DEBUG
        qWarning() << "Skip to start timer bacause of http_message map does not empty (current count = " << httpMessages.count() << ")";
#endif
        return;
    }

    progressDialog->setValue(progressDialog->maximum());
    
    if(!progressDialog->isHidden())
        progressDialog->hide();
	
    if(manual == false){
        m_timer->start();
#ifdef _HTTP_DEBUG
        qWarning() << "Timer started" << localDBInfo;
#endif
    }
    else
        startButton->setEnabled(true);
}

void dds_HttpWindow::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    int status = responseHeader.statusCode();
    switch (status) {
    case 200:                   // Ok
    case 301:                   // Moved Permanently
    case 302:                   // Found
    case 303:                   // See Other
    case 307:                   // Temporary Redirect
    case 205:
        // these are not error conditions
        break;

    default:
        //QMessageBox::information(this, tr("HTTP"),
        //                         tr("Data transfer failed: %1.")
        //                         .arg(responseHeader.reasonPhrase()));
        httpRequestAborted = true;
        progressDialog->hide();
        http->abort();

        if(manual == false){
            m_timer->start();
#ifdef _HTTP_DEBUG
            qWarning() << "Timer started" << localDBInfo;
#endif
        }

    }
}

void dds_HttpWindow::updateDataReadProgress(int bytesRead, int totalBytes)
{
    if (httpRequestAborted)
        return;

    progressDialog->setMaximum(totalBytes);
    progressDialog->setValue(bytesRead);
}

void dds_HttpWindow::enableDownloadButton()
{
    startButton->setEnabled(!urlLineEdit->text().isEmpty());
}

bool dds_HttpWindow::setMessageAsSended(const int & id, const int & type)
{
    //qDebug() << __PRETTY_FUNCTION__ << id << type;
    bool result = false;
    if(loader)
        result = loader->setAsSended(id, type);

    return result;
}

bool dds_HttpWindow::sendOutMessage(const JKKSPMessWithAddr * message, bool filePartsFlag)
{
    
    if(message == NULL)
        return false;

    if(http == NULL)
        return false;
        
    httpRequestAborted = false;
    httpTransferComplete = false;

    QByteArray byteArray = message->pMess.serialize();  
    if(byteArray.size() == 0)
        return false;

    JKKSAddress addr = message->addr;
    
    QString sUrl = QString("http://" + addr.address() + ":" + QString::number(addr.port()) + "/");
    QUrl url(sUrl);
    QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
    http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
    
    /*if (!url.userName().isEmpty())
        http->setUser(url.userName(), url.password());*/

    QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
    if (path.isEmpty())
        path = "/";



    // encrypt
/*
#ifndef WIN32
	unsigned long crc;
    int key = 0;
    
    encrypt(&byteArray, crc, key, CFB, ENCRYPT);
    CryptMessage cryptMessage(byteArray, key, crc);
    byteArray = cryptMessage.serialize();
	
#endif
*/
	if(filePartsFlag)
	{
		QEventLoop eventLoop;
		connect(http,SIGNAL(requestFinished(int,bool)),&eventLoop,SLOT(quit()));
	//!!!!!!!!!
	//error
        httpGetId = http->post ( path, byteArray ) ;

		eventLoop.exec();
	}
	else
        httpGetId = http->post ( path, byteArray ) ;

    //QHttpResponseHeader responseHeader = http->lastResponse();
    //if(responseHeader.statusCode() != 401 && //packet was ignored by receiver organization
    //   responseHeader.statusCode() != 400 ) //ERROR
    //{ 

    if(message->pMess.getType() != JKKSMessage::atFilePart)//дл€ файлов, передаваемых част€ми информаци€ в этот список заноситс€ отдельно
        httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
    
    //}
        
    /* JKKSPMessage pMessage(byteArray);//byteArray);
     qDebug() << "Message type: " << pMessage.getType();*/
  
    return true;

}

void dds_HttpWindow::loadData()
{
   /* QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    
    for (int j = 0 ; j < responses.size(); j++)
    { 
        out << responses.at(j);
    }

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    */

    QHttpResponseHeader response; 

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));



    int numRead = 0, numReadTotal = 0;

    
    QByteArray all_data,bt;

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

    // QMessageBox::critical(this, "test","Read data from DynamicDocs Client. Datas size: " + QString::number(numReadTotal));
    //qDebug() << "Receive data size: " << QString::number(numReadTotal);

    QByteArray block;
    QString str;

    if(numReadTotal == 0){

        //qDebug() << "Receive data size: " << QString::number(numReadTotal);
        response = QHttpResponseHeader( 200 , "HTTP/1.1 200 OK" );
		QString str = response.toString();
		block.append(str);
        clientConnection->write(block);
        
        clientConnection->disconnectFromHost();
        
        return;
    }
     

     QHttpRequestHeader header( all_data );


    QByteArray bad_block;
    QDataStream ot(&bad_block, QIODevice::WriteOnly);
    ot.setVersion(QDataStream::Qt_4_0);

    ot << (quint16)0;
    
    ot << "HTTP/1.1 400 Bad Request";
    ot << "Date:";
    ot << "Server: DYNAMICDOCS INTERACTOR/1.0.0";
    ot << "Last-Modified: Mon, 14 Jun 2010 06:04:27 GMT";
    ot << "Content-Type: text/html";
    ot << "Content-Length: 500";
    ot << "Content-Location: http://";        

    ot.device()->seek(0);
    ot << (quint16)(bad_block.size() - sizeof(quint16));


    QByteArray ignore_block;
    QDataStream oti(&ignore_block, QIODevice::WriteOnly);
    oti.setVersion(QDataStream::Qt_4_0);

    oti << (quint16)0;

    oti << "HTTP/1.1 204 Bad Request";
    oti << "Date:";
    oti << "Server: DYNAMICDOCS INTERACTOR/1.0.0";
    oti << "Last-Modified: Mon, 14 Jun 2010 06:04:27 GMT";
    oti << "Content-Type: text/html";
    oti << "Content-Length: 500";
    oti << "Content-Location: http://";        

    oti.device()->seek(0);
    oti << (quint16)(ignore_block.size() - sizeof(quint16));

     if(header.isValid() && header.hasContentLength ())
     {
         int sz_dt = header.contentLength();
         all_data = all_data.right(sz_dt); 

         QByteArray byteArray = all_data; 

/*
#ifndef WIN32
         CryptMessage cryptMessage(all_data);
            byteArray = cryptMessage.message;
            unsigned long new_crc = 0;
            int key = cryptMessage.hash - qHash(cryptMessage.message);
            
            encrypt(&byteArray, new_crc, key, CFB, DECRYPT);
	    qDebug() << "Crypt message.hash" << cryptMessage.hash;
	    qDebug() << "Crypt message.crc: " <<  cryptMessage.CRC;
	    qDebug() << "Encrypt crc: " <<  new_crc;
	    qDebug() << "key: " << key;
#endif
*/
            JKKSPMessage pMessage(byteArray);//byteArray);
            
            int res = loader->writeMessage(pMessage);
            if(res <= 0){
                qDebug() << "Message type: " << pMessage.getType();
                qDebug() << "Write message status : " << res;
            }

           

            if(res == ERROR_CODE)
            {
				response = QHttpResponseHeader( 400 , "HTTP/1.1 205 Bad Request" );
				str = response.toString();
				block.append(str);

                clientConnection->write(block);
            }
            else if(res == OK_CODE)
            {
				response = QHttpResponseHeader( 200 , "HTTP/1.1 200 OK" );
				QString str = response.toString();
				block.append(str);
                clientConnection->write(block);
            }
            else if(res == IGNORE_CODE)
            {
				response = QHttpResponseHeader( 204 , "HTTP/1.1 204 Bad Request" );
				QString str = response.toString();
				block.append(str);
                clientConnection->write(block);
            }

           clientConnection->disconnectFromHost();
     }
     else
     {
		    response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
			str = response.toString();
		    block.append(str);
            clientConnection->write(block);
            clientConnection->disconnectFromHost();
     }
}


/*void dds_HttpWindow::slotAuthenticationRequired(const QString &hostName, quint16, QAuthenticator *authenticator)
{
    QDialog dlg;
    Ui::Dialog ui;
    ui.setupUi(&dlg);
    dlg.adjustSize();
    ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm()).arg(hostName));
    
    if (dlg.exec() == QDialog::Accepted) {
        authenticator->setUser(ui.userEdit->text());
        authenticator->setPassword(ui.passwordEdit->text());
    }
}*/

/*#ifndef QT_NO_OPENSSL
void dds_HttpWindow::sslErrors(const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }
    
    if (QMessageBox::warning(this, tr("HTTP Example"),
                             tr("One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        http->ignoreSslErrors();
    }
}
#endif*/

