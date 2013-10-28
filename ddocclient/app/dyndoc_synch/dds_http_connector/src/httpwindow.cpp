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

#include "httpwindow.h"
#include <kksresult.h>
#include <kksdatabase.h>
#include <defines.h>
#include <KKSList.h>
#include <JKKSIOUrl.h>
#include <JKKSMessage.h>

dds_HttpWindow::dds_HttpWindow(dyndoc_HTTPconnector::HTTPsettings& settings,QObject *parent)
    : QObject(parent)
{
    init(settings);
}

void dds_HttpWindow::init(dyndoc_HTTPconnector::HTTPsettings& settings)
{
    try
    {
        init_settings(settings);
        init_interface();
        init_TCPserver();
        init_loader();
        init_connections();
    }
    catch(dyndoc_HTTPconnector::HTTPconnectorError& errorMessage)
    {
        QMessageBox msgBox;
        msgBox.setText(errorMessage.getError());
        msgBox.exec();
    }
    catch(...)
    {
        throw;
    }
}

void dds_HttpWindow::init_TCPserver()// запускаем сервер на прослушку порта
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, server_port))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("DynamicDocs Interactor "));
        msgBox.setText(tr("Unable to start the server: ") + tcpServer->errorString());
        msgBox.exec();

        throw dyndoc_HTTPconnector::TCPserverError();
    }
}

void dds_HttpWindow::init_loader()// создаем лоадер
{
    loader = new (std::nothrow) JKKSLoader (host, dbName, user, password, port, transport);
    localDBInfo = QString("DBHost: %1, DBName: %2").arg(host).arg(dbName);
    if( loader && !(loader && loader->connectToDb()) )
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("DynamicDocs Interactor "));
        msgBox.setText(tr("Unable to connect to the database:: %1") + host);
        msgBox.exec();

        throw dyndoc_HTTPconnector::loaderError();
    }
}

void dds_HttpWindow::init_connections()
{
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(loadData()));

    //срабатывает, когда в методе httpRequestFinished пришел очередной ответ и удалена запись из списка http_messages
    connect(this, SIGNAL(httpMessageRemoved(int)), this, SLOT(slotHttpMessageRemoved(int)));

    connect(http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
    connect(http, SIGNAL(dataReadProgress(int, int)),
            this, SLOT(updateDataReadProgress(int, int)));
    connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
}

void dds_HttpWindow::init_interface()
{
    http = new QHttp(this);
}

void dds_HttpWindow::init_settings(dyndoc_HTTPconnector::HTTPsettings& settings)
{
    init_settingsDB(settings);
    init_settingsTransportProtocol(settings);
    init_settingsServerAndLocal(settings);
}

void dds_HttpWindow::init_settingsDB(dyndoc_HTTPconnector::HTTPsettings& settings)
{
    dbName = settings.dbName;//settings.value ("database").toString ();
    host = settings.host;//settings.value ("host", "127.0.0.1").toString ();
    user = settings.user;//settings.value ("user", "jupiter").toString ();
    password = settings.password;//settings.value("password", "111").toString ();
    port = settings.port;//settings.value("port", "5432").toInt ();
}

void dds_HttpWindow::init_settingsTransportProtocol(dyndoc_HTTPconnector::HTTPsettings& settings)
{
    transport = settings.transport;//settings.value ("transport","1").toInt ();
    addr = QString();//!!!!
}

void dds_HttpWindow::init_settingsServerAndLocal(dyndoc_HTTPconnector::HTTPsettings& settings)
{
    http_host = settings.http_host;//settings.value ("host", "127.0.0.1").toString ();
    http_port = settings.http_port;//settings.value ("port", "8001").toInt ();

    server_port = settings.server_port;//settings.value ("server_port", "8001").toInt ();
}

void dds_HttpWindow::startProc()
{
#ifdef _HTTP_DEBUG
    qWarning() << "Entered to startProc()" << localDBInfo;
#endif

    httpRequestAborted = false;
    httpTransferComplete = false;

    messageList = loader->readMessages();
    count_send = 0;

    if(messageList.size() > 0 )
    {
        httpMessages.clear();

        for (QList<JKKSPMessWithAddr *>::const_iterator iterator = messageList.constBegin();iterator != messageList.constEnd();++iterator)
        {
            bool stat = sendOutMessage((*iterator)) ;
        }

        while(!messageList.isEmpty())
            delete messageList.takeFirst();
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

    httpRequestAborted = true;
    http->abort();

    emit signalCancelDownload();
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
                emit signalErrorDataTransferFailed(http->errorString());
                qWarning() << tr("ERROR: Data transfer failed: %1.").arg(http->errorString());
            } 
	        else {
                emit signalDataTransferComplete();
                httpTransferComplete = true;
            }
        }
    }
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
        http->abort();
    }
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

    if(message->pMess.getType() != JKKSMessage::atFilePart)//для файлов, передаваемых частями информация в этот список заносится отдельно
        httpMessages.insert(httpGetId, qMakePair(message->id, (qint64)message->pMess.getType()) );
    
    //}
        
    /* JKKSPMessage pMessage(byteArray);//byteArray);
     qDebug() << "Message type: " << pMessage.getType();*/
  
    return true;

}

void dds_HttpWindow::loadData()
{
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

void dds_HttpWindow::run()
{
    startProc();
}

//*****************************************************
dyndoc_HTTPconnector::HTTPconnectorError::HTTPconnectorError()
{
}

dyndoc_HTTPconnector::HTTPconnectorError::~HTTPconnectorError()
{
}

QString dyndoc_HTTPconnector::HTTPconnectorError::getError() const
{
    return QString("Connector error!");
}

dyndoc_HTTPconnector::TCPserverError::TCPserverError()
{
}

dyndoc_HTTPconnector::TCPserverError::~TCPserverError()
{
}

QString dyndoc_HTTPconnector::TCPserverError::getError() const
{
    return QString("TCP server error!");
}

dyndoc_HTTPconnector::loaderError::loaderError()
{
}

dyndoc_HTTPconnector::loaderError::~loaderError()
{
}

QString dyndoc_HTTPconnector::loaderError::getError() const
{
    return QString("Loader error!");
}

