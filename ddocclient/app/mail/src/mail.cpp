#include <QString>
#include <QSettings>
#include <QDebug>
#include <QBuffer>
#include <QDataStream>
#include <QFileInfo>

#include <kkspgdatabase.h>

#include <JKKSLoader.h>
#include <JKKSPMessage.h>

#include "logger.h"
#include "cryptmessage.h"

#define STOP_COMMAND "end"
//Имя задачи = 4 символа
#define JUPITER_TASK_NAME_LENGTH 4

//Для описания процесса в составе ИВК ЮПИТЕРФ
PPCA      prom_pca;

Logger logger;
JKKSLoader * loader;

QString kvs_in("tst");
QString kvs_out("tst");

bool sendOutMessage(PSTKMPR pstk, const JKKSPMessWithAddr * message)
{
    MESSAGE_PARAMS jMessage;

    QByteArray ba = message->addr.toLocal8Bit();
    const char * cAddr = ba.constData();
    memcpy(&jMessage.kvs, kvs_out.toLocal8Bit().data(), sizeof(jMessage.kvs));
    jMessage.rezim = ' ';

    printf("Receiver addres = %s\n", cAddr);

    memcpy(&jMessage.adrpol, cAddr, sizeof(jMessage.adrpol));
    //
    // toAscii->toLocal8Bit
    //
    jMessage.reftype = DATAMEMORY;

    QByteArray byteArray(message->pMess.serialize());

    // encrypt
    unsigned long crc;
    int key = 0;
    //TODO check error
    /*ulong result = */encrypt(&byteArray, crc, key, CFB, ENCRYPT);
    CryptMessage cryptMessage(byteArray, key, crc);
    byteArray = cryptMessage.serialize();

    int size = byteArray.size();
    if(size <= 0)
        printf("ERROR! data size = 0!\n");
	
    char * buffer = (char*)GetMem(pstk, size + 1);
    //TODO check error
    memcpy(buffer, byteArray.constData(), size);

    jMessage.data = buffer;
    jMessage.datalen = size;
    jMessage.pr_code = false;
    jMessage.numcode = 0;
    jMessage.kat_sr = message->pMess.urgencyLevel(); // urgency level
    jMessage.datagrif = message->pMess.macLabel();   // secrecy grif
    jMessage.format = "";

    int res = SendData(pstk, &jMessage);
    if(res == PLOTNOERR)
        printf("SendData() invoked successfully!\n");
    else
        printf("SendData() returns %d\n", res);

    return (res == PLOTNOERR);
}


bool setMessageAsSended(const int & id, const int & type)
{
    qDebug() << __PRETTY_FUNCTION__ << id << type;
    bool result(false);
    if(loader)
        result = loader->setAsSended(id, type);
    return result;
}

void processOutMessages(PSTKMPR pstk)
{
    QList<JKKSPMessWithAddr *> messageList = loader->readMessages();
    for (QList<JKKSPMessWithAddr *>::const_iterator iterator = messageList.constBegin();\
            iterator != messageList.constEnd();\
            ++iterator) 
    {
        bool res = sendOutMessage(pstk, (*iterator));
	if(res != TRUE){
	    printf("ERROR! sendOutMessage return error!\n");
	    //break;
	}
	
        if(!setMessageAsSended((*iterator)->id, (*iterator)->pMess.getType())){
            printf("ERROR! setMessageAsSended return error!\n");
	    //break;//cannot send message or mark it as sended, break until next iteration
	}
    }

    while(!messageList.isEmpty())
        delete messageList.takeFirst();
}

//поток опроса базы данных
void database(void */*rab*/)
{
    PSTKMPR     pstk;

    if (regt(prom_pca, &pstk) != PLOTNOERR){
        logger.log("Ошибка при регистрации потока опроса базы данных");
        return;
    }

    while(prom_pca->pcastop == 0){
        processOutMessages(pstk);//process all new data
        delay(pstk, pstk->smpdelay);
    }

    unregt(pstk);
}

void processInMessages(PSTKMPR pstk, MESSAGE_PARSED jMessage)
{
    qDebug() << __PRETTY_FUNCTION__;
    u long size = 0;
    u long read_size = 0;
    sizetemp(pstk, jMessage.keytemp, &size);
    char * buffer = new (std::nothrow) char[size];
    if(buffer){
        if(readtemp(pstk, jMessage.keytemp, NULL/*PSU*/, NULL/*su_ext*/, NULL/*su_size*/, buffer, size, 0/*"smesh"*/, &read_size) == PLOTNOERR){
            qDebug() << __PRETTY_FUNCTION__ << "size: " << size << "read_size" << read_size;

            CryptMessage cryptMessage(QByteArray(buffer, size));
            QByteArray byteArray = cryptMessage.message;
            unsigned long new_crc = 0;
            int key = cryptMessage.hash - qHash(cryptMessage.message);
            //TODO check error
            /*ulong result = */encrypt(&byteArray, new_crc, key, CFB, DECRYPT);
	    qDebug() << "Crypt message.hash" << cryptMessage.hash;
	    qDebug() << "Crypt message.crc: " <<  cryptMessage.CRC;
	    qDebug() << "Encrypt crc: " <<  new_crc;
	    qDebug() << "key: " << key;
            //if( cryptMessage.CRC == new_crc){
                JKKSPMessage pMessage(byteArray);
                qDebug() << "Message type: " << pMessage.getType();
                int res = loader->writeMessage(pMessage);
		if(res <= 0){
		    printf("ERROR! writeMessage return error! res = %d\n", res);
		}
            //} else
            //    logger.log("Ошибка расшифровки данных");
        } else
            logger.log("Ошибка чтения данных");
        if(deltemp(pstk, jMessage.keytemp) != PLOTNOERR)
            logger.log("Ошибка удаления данных");
        delete buffer;
    } else
        logger.log("Ошибка выделеня памяти");
}

// Поток для опроса входной очереди
void jupiter(void */*rab*/)
{
    PSTKMPR         pstk;

    if (regt(prom_pca, &pstk) != PLOTNOERR){
        logger.log("Ошибка при регистрации потока опроса очереди");
        return;
    }

    char * buffer = (char*)GetMem(pstk, PLOTMSGSIZE);
    if(buffer){
        u long          msgSize;
        MESSAGE_PARSED  jMessage;
        char            taskName[4];
        // опрос очереди
        while (prom_pca->pcastop == 0){
            vvod(pstk, taskName, buffer, &msgSize);
            switch (GetPlotAvr(pstk)){
                case PLOTERRLINENOMSG://no new message
                break;
                case PLOTNOERR:
                    if (ParseData(pstk, buffer, msgSize, &jMessage) != PLOTNOERR){
                        logger.log("Данные не обработаны");
                        break;
                    }
                    if (!memcmp(&jMessage.kvs, kvs_in.toLocal8Bit().data(), kvs_in.size())){
                        if (!strlen((char *)&jMessage.keytemp)){//данные не во временной памяти
                            ;//TODO and what???
                        } else {
                            processInMessages(pstk, jMessage);
                            break;
                        }
                    }
                break;
                // Ошибка при работе с очередью
                default:
                    logger.log("Поток опроса очереди: ошибка при чтении сообщения из очереди. Рекомендуется перезапустить систему");
                break;
            }
            delay(pstk, pstk->smpdelay);
        }
    } else
        logger.log("Ошибка выделения памяти");
    unregt(pstk);
}

int main(int /*argc*/, char * argv[])
{
    PSTKMPR    pstk;

    //register itself
    QString taskName((QFileInfo(argv[0]).fileName()).left(JUPITER_TASK_NAME_LENGTH));
    u long result = regp(taskName.toLocal8Bit().data(), &prom_pca, &pstk);
    if (result != PLOTNOERR){
        SysJurn(pstk, "Ошибка при регистрации задачи", NULL, 0);
        return result;
    }

    logger.setPSTKMPR(pstk);

    QSettings settings("/etc/kkssito/mail.ini", QSettings::IniFormat);

    QString host(settings.value("Database/host", "127.0.0.1").toString());
    QString dbName(settings.value("Database/database", "db").toString());
    QString user(settings.value("Database/user", "jupiter").toString());
    QString password(settings.value("Database/password", "111").toString());
    int port(settings.value("Database/port", "5432").toInt());
    int idTransport(settings.value("Database/transport", "1").toInt());

    kvs_out = settings.value("Jupiter/kvs-out", "tst").toString();
    kvs_in = settings.value("Jupiter/kvs-in", "tst").toString();
    
    int forceSetAddress = settings.value("Misc/update_addr", "1").toInt();

    loader = new (std::nothrow) JKKSLoader (host, dbName, user, password, port, idTransport);
    if (loader && loader->connectToDb ())
    {
        printf("Database connection: OK!\n");
        logger.log<bool>(string("Database connection: ok"), true);
        UPINFO pupinfo;
        if(GetUPInfo(pstk, &pupinfo) == PLOTNOERR){
	    
	    char obj[2];
	    obj[0] = pupinfo.objcode[0];
	    obj[1] = pupinfo.objcode[1];
	    char mod[2];
	    mod[0] = pupinfo.modcode[0];
	    mod[1] = pupinfo.modcode[1];
	    QString addr1 = QString::fromLocal8Bit(obj, 2);
	    QString addr2 = QString::fromLocal8Bit(mod, 2);
	    QString addr = addr1+addr2;
	    printf("Local transport  address is: %s\n", addr.toLocal8Bit().constData());
	    QString lAddr = loader->getLocalAddress();
	    qWarning() << "Local address received from database = " << lAddr;
            //loader->setLocalAddress(addr);

            /*
	    if(forceSetAddress == 1){
	        printf("Update local transport address in database to: %s\n", 
		               addr.toLocal8Bit().constData());
			       
	        loader->setLocalAddress(idTransport, addr);
	    }
	    else{
	        printf("Skip updating local transport address in database. Check /etc/kkssito/mail.ini\n");
	    }
	    */
        }
	
	u long     thrID;
        //register database event listener
        if (stthr(pstk, (void*)database, (void*)NULL, &thrID) != PLOTNOERR)
            logger.log ("Ошибка при инициализации потока опроса базы данных");
        else {
            //register jupiter queue listener,
            if (stthr(pstk, (void*)jupiter, (void*)NULL, &thrID) == PLOTNOERR){
                //wait for end signal from jupiter
                QString stopCommand(taskName + STOP_COMMAND);
                wait_event(pstk, (void*)(stopCommand.toLocal8Bit().data()), stopCommand.size(), -1);
            }
            else
                logger.log("Ошибка при инициализации потока опроса очереди");
        }
    }
    else if (loader)
    {
        printf("ERROR on connect!\n");
        logger.log<bool>(std::string("Database connecttion error"), false);
    }

    if (loader)
        delete loader;

    //unregister itself
    unregp(prom_pca);
    return PLOTNOERR;
}
