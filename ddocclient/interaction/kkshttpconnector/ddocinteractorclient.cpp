
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

    //�����������, ����� � ������ httpRequestFinished ������ ��������� ����� � ������� ������ �� ������ http_messages
    connect(this, SIGNAL(httpMessageRemoved(int)), this, SLOT(slotHttpMessageRemoved(int)));
    //�������� ������ �� �������� ������ ����� http
    connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
    //�������� ������ �� �������� ������ ����� pingHttp
    connect(pingHttp, SIGNAL(requestFinished(int, bool)), this, SLOT(pingHttpRequestFinished(int, bool)));
    //�������� ������ �� �������� ������ ����� pingResHttp
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
    if(m_isExiting){ //���� m_base ��������� ������ (�.�. ������ �� ������ �������� ����������)
        return;
    }

    emit sendingStarted();

    if(manual == false){
        m_timer->stop();
    }
    
    //emit showStatusText(tr("Waiting for data to sent..."));
    //kksInfo() << tr("Waiting for data to sent...");
   
    QStringList receivers;
    
    //������� �������� ������ �� ���������� ����� �����
    QList<JKKSPMessWithAddr *> pingResultsList = m_loader->readPingResults(receivers);
    if(pingResultsList.count() > 0 )
    {
        kksInfo() << tr("Found %1 ping results for sent").arg(QString::number(pingResultsList.count()));
        httpMessages.clear();

        for (QList<JKKSPMessWithAddr *>::const_iterator iterator = pingResultsList.constBegin();iterator != pingResultsList.constEnd();++iterator)
        {
            bool stat = sendOutMessage((*iterator), false, false) ;  //������ �������� �������� �� ��, ��� ������ ����� ������� ��������� ��� ����������. 
                                                                    //����� ����������. �.�. ������� �� ����� �������, ���� �� ���������� �������� ������ ���������
                                                                    //������ ��� ���������� � �������� ����������
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

    //����� �������� ����� ("������") �� ��� �����������, �� ������� ���������� ��������� ������
    //� ������, ���� �� �����-���� ����������� ����� ������ �������� - �� ����� ����������� ������ �� ����������
    //���������� ����� ����������
    //� ����� ���� ��� ������, � ����� ��� ������ ������ - ��������� ����� ������ � �������� �������� ���������
    m_parent->m_pings = m_loader->createPings(receivers);
    m_parent->pingsForSent = m_parent->m_pings.count();
    m_parent->cntPingsSended = 0;

    if(m_parent->pingsForSent > 0)
        kksWarning() << tr("Starting to send %1 ping requests.\nSending messages will be suspended until all ping results do not come back").arg(QString::number(m_parent->pingsForSent));

    
    //* ��������� ����� � ���� ���� ���� ������� �� ����������*/ 
    //�����! ������ ����� ������ �����-���, ���� ����� ����������� ���������, �.�. � ���� ������ ������ � ���������� �������� ������ ������, ��� �������� �����-���
    
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

    if(m_isExiting){ //���� m_base ��������� ������ (�.�. ������ �� ������ �������� ����������)
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
                m_parent->cntMsgSended++; //���-���� ������� ��� ������������
                
                //���� ��������� ��
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
            pMessWithAddr->unp = part.receiverUID();
            
            //����� ������ ����������� �������� ���������
            bool stat = sendOutMessage(pMessWithAddr, true, eof) ;

            if(!stat){
                m_parent->cntFilePartsSended++; //���-���� ������� ����� ����� ������������
                if(eof)
                    m_parent->cntFilesSended++;//���� ���������� ��������� ����� �����, �� � ��� ���� ������� ������������
                
                //���� ��������� ��
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

//����� http-������� �� ������ ���� POST
void DDocInteractorClient::httpRequestFinished(int requestId, bool error)
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

    //���� �� ���������� ����� ����� � ��� �� �������� ���������
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
        
        //����� ���� �������� ���������� � ��������� ������� ����������� (� ������).
        //������� �� ����������, ����� ����������� ��������� �� ������� �����.
        QString emailPrefix = m_loader->getReceiverEmailPrefix(t.first, t.second);
        if(emailPrefix.isEmpty())
            return;//�� ��� ��� �� ������ ����

        JKKSPing defValue = JKKSPing();
        JKKSPing p = m_parent->m_pings.value(emailPrefix, defValue);

        p.setState1(0);
        p.setState2(0);
        m_parent->m_pings.insert(emailPrefix, p);

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
            qCritical() << tr("Cannot mark message as sended! Database Error. Message ID = %1, Message type = %2. Internal request ID = %3").arg(t.first).arg(t.second).arg(requestId);
        }
        return;
    }

    //����� ��������, ��� ����� ������ �� �����
    //� � ������, ���� ����� �� �������� " ERROR ", �������� ��������� ��� ������������
    if( ! ba.contains(" ERROR ")){
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << tr("Cannot mark message as sended! Database Error. Message ID = %1, Message type = %2. Internal request ID = %3").arg(t.first).arg(t.second).arg(requestId);
        }
    }
    else{
        //� ��������� ������ �������� ���������� � ������� ����������� (� �����)
        //������ �� ����������
        QString emailPrefix = m_loader->getReceiverEmailPrefix(t.first, t.second);
        if(emailPrefix.isEmpty())
            return;//�� ��� ��� �� ������ ����

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
    //msgForSent + filesForSent ������������ � ������ startProc() ��� ������ ���-�� ��������� � ������ �� ��������

    if((m_parent->msgForSent + m_parent->filesForSent) > sendedCount){ 
        return;
    }
    
    //���� �������������� ��������� �� �������� ������ ��� - �������� ������ � ���� ������ �� ��
    kksInfo() << tr("Waiting for data to sent...");
    if(manual == false){
        m_timer->start();
    }
    else
        emit sendingCompleted(); //���� ������� ������ (����������) ����� - ������ ��������� ������ ��������
}

//����� http-������� �� ������ ���� POST
void DDocInteractorClient::pingResHttpRequestFinished(int requestId, bool error)
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
    
    //����� �� ��������, ��� ���� ������ ����� �� ��������� � requestId �� ������ ������� (� error = false), 
    //�� ������ ���������� ���� ����� ���������
    //���� ���� == OK ��� ��� ������, �� ��� ������ ����� ���� �������������� �������� � http_connector'�� 
    //� ��������� ������ ����� ������ ���� � �������
    // 1 OK
    // 1 ERROR 15
    //� ��� ��������, ��� �������������� �������������� ����� ���� (���)
    QByteArray ba = pingResHttp->readAll();
    if(ba.length() <= 0 || ba == "OK"){//��� ��������� ���� �������� �������� �� ������� ������ � http_connector
        if(!setMessageAsSended(t.first, t.second)){
            qCritical() << tr("Cannot mark ping result as sended! Database Error. Message ID = %1, Message type = %2. Internal request ID = %3").arg(t.first).arg(t.second).arg(requestId);
        }
        return;
    }

    //����� ��������, ��� ����� ������ �� �����
    //� � ������, ���� ����� �� �������� " ERROR ", �������� ��������� ��� ������������
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
        //��� ������� �� ����� �� ���� ��������� ������� �� ����������
        if(message->pMess.getType() != JKKSMessage::atPingResponse){
            QString receiver = message->pMess.receiverUID();
            JKKSPing ping = m_parent->m_pings.value(receiver);
            
            if(ping.created() != 1){
                //��������� � ����� ������ ������� ������ ��� "�������� �����" ��� �������������� �������������
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

    //�����!!! 
    //��� �������� ������ ����� ���� ��� ������������ �������� ��������������� ����������� �������������� ������������� ���������
    //� ����� ���������� ������������� �������������� ������������� ��������� ����������� ��� ����� (��� �����), ��������������� ���
    //(�.�. �� ����� ������� ���� �������������). ��� ����������� �� ������ �� ������ ��������� ������������ ������ ����������� kksinteractor � http_connector
    //������ � ���� ��������� ���������, ����������� � ���� ���, �����������.
    //
    //������ ���������: mesid=<ID><type_digit_1><type_digit_2>&unp=<email_prefix>&data=........(base64)......
    //���� ��� ����� ���� �����, ����� ��� �������� 0. �.�. 00 - atCommand, 02 - atDocument, � �.�.
    
    QString mesId;
    
    //��� ���������. ��. ���� ��������� � ������������ JKKSMessage::JKKSMessageType
    //��� ������ = 13
    int t = message->pMess.getType(); 
    if(t >= 0 && t <= 9){
        mesId = QString("0%1").arg(QString::number(t));
    }
    else{
        mesId = QString("%1").arg(QString::number(t));
    }

    //������� ������ ��������� � ��������������� ���������� ��������� �������� ������ ���
    QString s = QString("mesid=%1%2&unp=%3&data=")
                          .arg(QString::number(message->id)) //������������� ��������� �� ��������������� �������
                                                             //��� ������ - ������������� �����. ����� �������������� ����������� �� ������� organization
                          .arg(mesId) 
                          .arg(message->unp.isEmpty() ? QString("1919") : message->unp);//�������� ����� ���������� (���������� email_prefix)

    byteArray.prepend(s.toAscii());
    //hash-�����
    byteArray.append("&hash=");
    hash = hash.toBase64();
    hash = hash.toPercentEncoding();
    byteArray.append(hash);
    
    //������������ ���������
    ba = byteArray;

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
        //�� ����� ����� ����� ������ ��� ���� �� ������ �������������� �� �������� ������, ��������� email_prefix �����������-���������� �����
        JKKSPing pp = m_parent->m_pings.value(message->pMess.receiverUID());
        pingHttpMessages.insert(reqId, pp);
        return;
    }

    if(message->pMess.getType() == JKKSMessage::atPingResponse){
        pingResHttpMessages.insert(reqId, qMakePair(message->id, (qint64)message->pMess.getType()) );
        return;
    }

    if(message->pMess.getType() != JKKSMessage::atFilePart){//��� ������, ������������ ������� ���������� � ���� ������ ��������� ��������
        httpMessages.insert(reqId, qMakePair(message->id, (qint64)message->pMess.getType()) );
    }
    else{
        if(isLastFilePart){
            //��� �������� � ����, ��� � ������ httpRequestFinished ��������� ����� ������ �������� ����� ��������� ������ �������, ����� �������� ��������� ����� �����
            httpMessages.insert(reqId, qMakePair(message->id, (qint64)message->pMess.getType()) );
        }
        else{
            //��� �������� � ����, ��� � ������ httpRequestFinished ��������� ����� �������� ����� �� ����������, ������ ������������� ������, ������� �������� eventLoop
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

//����� http-������� �� ������ �������� ����� 
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

    //���� �� ������ ��������� ���� - �������� ��� ��� ������������� � �������� ������������� (�������� ������� �� ��������)
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
    
    //����� �� ��������, ��� ���� ������ ����� �� ��������� � requestId �� ������ ������� (� error = false), 
    //�� ������ ���������� ���� ����� ���������
    //���� ���� == OK ��� ��� ������, �� ��� ������ ����� ���� �������������� �������� � http_connector'�� 
    //� ��������� ������ ����� ������ ���� � �������
    // 1 OK
    // 1 ERROR 15
    //� ��� ��������, ��� �������������� �������������� ����� ���� (���)
    QByteArray ba = pingHttp->readAll();
    if(ba.length() <= 0 || ba == "OK"){//��� ��������� ���� �������� �������� �� ������� ������ � http_connector
        kksDebug() << tr("Ping request delivered to destination organization. Receiver = %1, Adress = (IP = %2, port = %3). Internal request ID = %4. Waiting for answer").arg(t.uidTo()).arg(t.getAddr().address()).arg(t.getAddr().port()).arg(requestId);;
        return;
    }

    //����� ��������, ��� ����� ������ �� �����
    //� � ������, ���� ����� �������� " ERROR ", �������� ��������� ��� ������������
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

//���� ������ ��������� - ����� �� ����, �� ���������� ���������� ��� � ��������� ������ ������
//��� ���� ����� ��������� ���������� ������������ ������ �� ����
//� ���� �� ���-�� ����� ���-�� �������������� ��� �������� - ������������� �������
int DDocInteractorClient::processPingResponse(const JKKSPing * ping)
{
    if(!ping){
        return 1;
    }

    m_parent->cntPingsSended++;

    for (QMap<QString, JKKSPing>::iterator pa = m_parent->m_pings.begin(); pa != m_parent->m_pings.end(); pa++){
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
