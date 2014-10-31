
#include <QtNetwork>
#include <QCryptographicHash>

#include <QXmlInputSource>
#include <QXmlSimpleReader>

#include "ddocinteractorbase.h"
#include "ddocinteractorserver.h"
#include <defines.h>
#include <kksdebug.h>
#include <JKKSIOUrl.h>
#include <JKKSMessage.h>
#include <kksclient_name.h>

void DDocTcpServer::incomingConnection(int socketDescriptor)
{
    emit connectionArrived(socketDescriptor);
}

DDocInteractorServer::DDocInteractorServer(JKKSLoader* loader, DDocInteractorBase * parent) 
    : QThread(parent), 
      m_loader(NULL), 
      m_parent(NULL),
      m_succesed(false),
      m_tcpServer(NULL),
      m_isExiting(false),
      m_waitClientConnectionTimeout(1000)
{
    m_loader = loader; 
    m_parent = parent;
}

DDocInteractorServer::~DDocInteractorServer()
{
    wait();

    if(m_tcpServer)
        delete m_tcpServer;
}

/***************************/
/***************************/

void DDocInteractorServer::run()
{
    exec();
}

void DDocInteractorServer::init()
{
    m_succesed = false;
    if(!m_loader){
        return;
    }
    
    m_tcpServer = new DDocTcpServer(this);
    if (!m_tcpServer->listen(QHostAddress::Any, m_port)) {
        return;
    }

    m_succesed = true;
    connect(m_tcpServer, SIGNAL(connectionArrived(int)), this, SLOT(loadData(int)));
}

void DDocInteractorServer::loadData(int socketDescriptor)
{
    QTcpSocket * clientConnection = new QTcpSocket();
    if(!clientConnection->setSocketDescriptor(socketDescriptor)){
        qCritical() << tr("Bad socket! Cannot create connection!");
        return;
    }

    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    int numRead = 0, numReadTotal = 0;
    
    QByteArray all_data, bt;

    bool ok = clientConnection->waitForReadyRead();
    if(!ok){
        sendBadBlock(clientConnection);
        clientConnection->disconnectFromHost();
        clientConnection->waitForDisconnected();
        return;
    }

    forever {
        bt  = clientConnection->read(1000);
        QString err = clientConnection->errorString();
        //set data
        numRead = bt.size();
        
        all_data += bt;
        numReadTotal += numRead;
         
        if (numRead == 0 && 
            !clientConnection->waitForReadyRead(m_waitClientConnectionTimeout)
            )
        {
            break;
        }
    }

    if(numReadTotal == 0){
        sendBadBlock(clientConnection);
        clientConnection->disconnectFromHost();
        clientConnection->waitForDisconnected();
        return;
    }

    QHttpRequestHeader header( all_data );

    bool b = header.hasContentLength();

    if(b)
    {
        kksInfo() << tr("New income data found on socket. Start processing...");

        int sz_dt = header.contentLength();
        all_data = all_data.right(sz_dt); 

        //�������� ��������� ����� ���� ���� �����:
        // - ��������� �� ���������� ������� DynamicDocs
        // - ��������� � �������� XML-�������, ������� � ���� ������� ����� ���� ���� �����:
        //       - ��������������� ��������  ������ IRL
        //       - ������ shushun (���� ����, ����-��������������)
        
        //������� ��������� ������ ������������� � XML
        QByteArray byteArray = QByteArray::fromPercentEncoding(all_data);
        //byteArray = qUncompress(byteArray);
        byteArray = QByteArray::fromBase64(byteArray);
        
        QXmlInputSource * xmlInput = new QXmlInputSource();
        xmlInput->setData(byteArray);
        QXmlSimpleReader xmlReader;
        bool ok = xmlReader.parse(xmlInput);
        if(ok){
            QString xml = xmlInput->data();
            processXMLMessage(xml, clientConnection);
        }
        else{
            //������ ������ �� ���������� ������� DynamicDocs

            QMap<QString, QByteArray> byteArray = parsePost(all_data);

            //�������� ������ ����� ���� �� ��������� ��������:
            //mesid=<ID><type_digit_1><type_digit_2>&uno=<email_prefix>&data="........(base64)......"
            //mesid=<ID><type_digit_1><type_digit_2>&uno=<email_prefix>&received=<code>
            //� ������ ������ ��� mesid � uno �� ���������. �� ���� � ����������� �� ���� ��������� ��������� ��� ����������
            //�� ������ ������ ��� ��������� ������������� � ��� ���������, ������� ���� �������� � ���������� � ��������� � received �����������
            if(byteArray.contains("mesid") && (byteArray.contains("unp") || byteArray.contains("uno")) && byteArray.contains("received")){
                processNotification(byteArray, clientConnection);
            }
            else if(byteArray.contains("mesid") && (byteArray.contains("uno") || byteArray.contains("unp"))&& byteArray.contains("data") && byteArray.contains("hash") ){
                processMessage(byteArray, clientConnection);
            }
            else{
                sendBadBlock(clientConnection);
            }
        }
     }
    else
    {
        qCritical() << tr("New income data found on socket. But correct data does not found in incomming request");
        sendBadBlock(clientConnection);
    }

    clientConnection->disconnectFromHost();
    clientConnection->waitForDisconnected();
    
    kksDebug() << tr("Disconnected from socket");
}

void DDocInteractorServer :: sendOKBlock(QTcpSocket * clientConnection, bool withData)
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

void DDocInteractorServer :: sendBadBlock(QTcpSocket * clientConnection)
{
    if(!clientConnection)
        return;

    qCritical() << tr("Found inconsistent income data on socket! Sending \"Bad request\" response to client") ;

    QByteArray block;
    QHttpResponseHeader response = QHttpResponseHeader( 400 , "HTTP/1.1 400 Bad Request" );
    QString str = response.toString();
    block.append(str);
    clientConnection->write(block);

}

//������ ��������� � XML-�������
//���� ����������� ������ � �������� ��������� � ������� in_external_queue
int DDocInteractorServer::processXMLMessage(const QString & xml, QTcpSocket * clientConnection)
{

    bool ok = m_loader->beginTransaction();
    if(!ok){
        qCritical() << QObject::tr("Cannot start transaction for message writing");
        sendBadBlock(clientConnection);
        return 1;
    }
        
    int res = m_loader->writeXMLMessage(xml);
    if(res <= 0){
        kksCritical() << tr("Cannot processing income XML message! Data was not written to database");
        kksCritical() << tr("XML = %1").arg(xml);

        bool ok = m_loader->rollbackTransaction();
        if(!ok){
            kksCritical() << QObject::tr("Cannot rollback transaction for message writing");
            sendBadBlock(clientConnection);
            return 1;
        }

    }
    else{    
        kksInfo() << tr("Income XML message was successfully written to database");
        bool ok = m_loader->commitTransaction();
        if(!ok){
            kksCritical() << QObject::tr("Cannot commit transaction for message writing");
            sendBadBlock(clientConnection);
            return 1;
        }

        res = OK_CODE;
    }

    // --������� -- � ��������� ����� �� ��������, ��� ��� ����������� �� ���������� ��������� ��������� ��������� � ��
    // --������� -- �� � �������� ������ �� ������ �������-����������� (�� ��� ����� (���)) ���������� OK
    // --������� -- ��� ����� �� ������� � ���, ��� ���� ������ ��������� ��������� �������
    // --������� -- ��� ���� ��������� ��������� ����� ��������� �������� � ���� ����. ���������

    if(res == OK_CODE)
    {
        sendOKBlock(clientConnection, true);    
    }
    else if(res == ERROR_CODE)
    {
        sendBadBlock(clientConnection);
    }
    else if(res == IGNORE_CODE)
    {
        sendOKBlock(clientConnection, true);
    }
    else{
        sendBadBlock(clientConnection);//������ ������������� ����� ��� 0
    }

    return 1;
}

int DDocInteractorServer::processMessage(QMap <QString, QByteArray> &post_data, QTcpSocket * clientConnection)
{

    if(!post_data.contains("mesid") ||  (!post_data.contains("uno") && !post_data.contains("unp"))  || !post_data.contains("data") ||!post_data.contains("hash")) {
        sendBadBlock(clientConnection);
        return -1;
    }

    //��������� hash-����� ��������� ������
    QByteArray etaloneHash = post_data["hash"];
    etaloneHash = QByteArray::fromPercentEncoding(etaloneHash);
    etaloneHash = QByteArray::fromBase64(etaloneHash);

    QByteArray byteArray = post_data["data"];
	byteArray = QByteArray::fromPercentEncoding(byteArray);
	byteArray = qUncompress(byteArray);
	byteArray = QByteArray::fromBase64(byteArray);

    //����������� hash-����� ��������� ������
    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha1);
    if(etaloneHash != hash){
        qCritical() << tr("Hash sum for income message is inconsistent!");
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
        JKKSPing * ping = (JKKSPing *)m_loader->unpackMessage(pMessage);
        kksInfo() << QObject::tr("Ping response received and saved");
        processPingResponse(ping);
        sendOKBlock(clientConnection, true);
        return 1;
    }

    bool ok = m_loader->beginTransaction();
    if(!ok){
        qCritical() << QObject::tr("Cannot start transaction for message writing");
        sendBadBlock(clientConnection);
        return 1;
    }

    int res = m_loader->writeMessage(pMessage);
    if(res <= 0){
        kksCritical() << tr("Cannot processing income message! Data was not writen to database");
        kksCritical() << tr("Sender = %1, receiver = %2, message type = %3. %4")
                                       .arg(pMessage.senderUID())
                                       .arg(pMessage.receiverUID())
                                       .arg(pMessage.getType())
                                       .arg(res == IGNORE_CODE ? tr("Message was IGNORED!") : tr("Database error!"));

        bool ok = m_loader->rollbackTransaction();
        if(!ok){
            kksCritical() << QObject::tr("Cannot rollback transaction for message writing");
            sendBadBlock(clientConnection);
            return 1;
        }

    }
    else{    
        kksInfo() << tr("Income message was successfully written to database");
        bool ok = m_loader->commitTransaction();
        if(!ok){
            kksCritical() << QObject::tr("Cannot commit transaction for message writing");
            sendBadBlock(clientConnection);
            return 1;
        }

        res = OK_CODE;
    }

    // --������� -- � ��������� ����� �� ��������, ��� ��� ����������� �� ���������� ��������� ��������� ��������� � ��
    // --������� -- �� � �������� ������ �� ������ �������-����������� (�� ��� ����� (���)) ���������� OK
    // --������� -- ��� ����� �� ������� � ���, ��� ���� ������ ��������� ��������� �������
    // --������� -- ��� ���� ��������� ��������� ����� ��������� �������� � ���� ����. ���������

    if(res == OK_CODE)
    {
        sendOKBlock(clientConnection, true);    
    }
    else if(res == ERROR_CODE)
    {
        sendBadBlock(clientConnection);
    }
    else if(res == IGNORE_CODE)
    {
        sendOKBlock(clientConnection, true);
    }
    else{
        sendBadBlock(clientConnection);//������ ������������� ����� ��� 0
    }

    return 1;
}

//������ ��������� � �������� �� ������-���������� ����� ������������� ���������
//������ ���� ���� ������ ����������� ������ ��� �������������� ����� ���� (���)
//����������� ����� ���� ���� �������� ��� ���������� ��������
//� ��������� ������ �� ������ �������� ��������� ��� ��������������
//� ������ �������� �������� ������ ������ �� ����, �.�. ��� �� ����� �������� ��������� � ������� ����������� � ��� �� ��� ��������� �������� ��� ������������
int DDocInteractorServer::processNotification(QMap <QString, QByteArray> &post_data, QTcpSocket * clientConnection)

{
    if(!post_data.contains("mesid") ||  (!post_data.contains("uno") && !post_data.contains("unp")) || !post_data.contains("received")){
        kksCritical() << tr("Found inconsistent incoming notification! Bad format");
        sendBadBlock(clientConnection);
        return -1;
    }

    qint64 idMsg = -1;
    int msgType = -1;
    QByteArray byteArray = post_data["mesid"];//6 - ������ ������ ���� � �������������� ���������. 2 ��������� �����  - ��� ��� ���������
    idMsg = (byteArray.left(byteArray.length()-2)).toLongLong();
	kksDebug() << QString("Mes ID: ") << idMsg;

    if(idMsg <= 0){
        kksCritical() << tr("Cannot parse MESID of the notification");
        sendBadBlock(clientConnection);
        return -1;
    }

    msgType = (byteArray.right(byteArray.length()-2)).toInt();
    if(msgType < 0){
        qCritical() << tr("Cannot parse MESID (type part) of the notification ");
        sendBadBlock(clientConnection);
        return -1;
    }

    byteArray = post_data["received"];

    int receiverResult = byteArray.toInt();
    bool ok = false;

    //������������ �����
    //���� ������ ����� �� �������� �����, �� ���� �������� �������� ���������������� ����� � ������ � ��������� ������ ������
    //���� ��������� ��������� �������������, ���������� ������������ ������ �� �����������, �.�. ����, ���� �� �������� �� ������ ��������� ��������� � ��� ������������� �����
    //���� ������������� - �� ����� ������������ ������ ����������� �� ����, �.�. ������� ������ �� �����
    if(msgType == (int)JKKSMessage::atPing){

        kksInfo() << tr("Receive ping notification from gateway. Id = %1, result = %2.").arg(idMsg).arg(receiverResult);
        processPingNotification(idMsg, receiverResult);
        sendOKBlock(clientConnection, true);

        return 1;
    }

    if(receiverResult != 1){

        //����� ���� �������� ���������� � ��������� ������� ����������� (� ������).
        //������� �� ����������, ����� ����������� ��������� �� ������� �����.
        QString emailPrefix = m_loader->getReceiverEmailPrefix(idMsg, msgType);
        if(emailPrefix.isEmpty())
            return 1;//�� ��� ��� �� ������ ����

        JKKSPing p = m_parent->m_pings.value(emailPrefix);
        p.setState1(0);
        p.setState2(0);
        m_parent->m_pings.insert(emailPrefix, p);

        qCritical() << tr("Message with id = %1 and type = %2 does not received by receiver! Result = %3")
                                          .arg(idMsg) 
                                          .arg(msgType)
                                          .arg(receiverResult);

        ok = setMessageAsSended(idMsg, msgType, false);
    }
    else {
        kksInfo() << QObject::tr("Correct notification result received from gateway");
        ok = true;//setMessageAsSended(idMsg, msgType);
    }

    if(!ok)
    {
        qCritical() << tr("Cannot mark message as sended! Database Error. idMsg = %1, type = %2")
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
int DDocInteractorServer::processPingNotification(int idMsg, int result)
{
    if(result != 1){
        qCritical() << tr("Ping request notification received. Destination organization cannot receive ping request!");
        m_parent->cntPingsSended++;
    }

    kksInfo() << tr("Ping request notification received. Destination organization successfully received ping request");

    for (QMap<QString, JKKSPing>::iterator pa = m_parent->m_pings.begin(); pa != m_parent->m_pings.end(); pa++){
        if(pa.value().id() == idMsg){
            pa.value().setState1(result != 1 ? 0 : 1);
            pa.value().setState2(1);
            //pa.value().setState3(0);
            //pa.value().setState4(0);
        }
    }
    
    if(m_parent->pingsForSent <= m_parent->cntPingsSended){
        emit pingsSended(m_parent->m_pings);
        emit pingsSentCompleted();
    }

    return 1;
}

//���� ������ ��������� - ����� �� ����, �� ���������� ���������� ��� � ��������� ������ ������
//��� ���� ����� ��������� ���������� ������������ ������ �� ����
//� ���� �� ���-�� ����� ���-�� �������������� ��� �������� - ������������� �������
int DDocInteractorServer::processPingResponse(const JKKSPing * ping)
{
    if(!ping){
        return 1;
    }

    m_parent->cntPingsSended++;

    for (QMap<QString, JKKSPing>::iterator pa = m_parent->m_pings.begin(); pa != m_parent->m_pings.end(); pa++){
        //��������� � ������ �� ���� ��� ���������� ����� ��� ������������� � �����������-����������� ����� 
        //(� ������ �� ������������ � �������� JKKSPing::id())
        //�� �������� ������������ �� �������������� �����������-���������� ����� (� ������� �� ����������� �����).
        //�� ��������� � �������� id_external_queue (�������������� ��� 0 (��� ������ ��������� �������������� �������������)) �� ������� ��
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

bool DDocInteractorServer::setMessageAsSended(const qint64 & id, const int & type, bool sended)
{
    bool result = false;
    if(m_loader)
        result = m_loader->setAsSended(id, type, sended);

    return result;
}

QMap<QString, QByteArray> DDocInteractorServer::parsePost (const QByteArray &array)
{
    int i=0;
    QMap <QString, QByteArray> message;
    QString buff_k;
    QByteArray buff_v;
    
    for (i=0; i<array.length();i++){
        if (i==0 || buff_k.isEmpty()){
            buff_k=array.at(i);
            buff_v="";
        }
        else if (buff_v.isEmpty() && array.at(i)!='='){
            buff_k+=array.at(i);
        }
        else if (array.at(i)=='=' && buff_v.isEmpty()){
            buff_v.append(array.at(++i));
        } 
        else if ( (array.at(i)!='&' && i != array.length()-1) && !buff_k.isEmpty()){
            buff_v.append(array.at(i));
        }
        else {
            if((array.at(i)=='&' || i==array.length()-1) && !buff_k.isEmpty() && !buff_v.isEmpty()){
                if(i==array.length()-1)
                    buff_v.append(array.at(i));
                message[buff_k]=buff_v;
                buff_k="";
                buff_v="";
            }
            else{
	            QMap <QString, QByteArray> _message; // And some little piece of shit
		        return _message;
            }
        }
    }
    
    return message;
}
