/***********************************************************************
 * Module:  JKKSMessage.h
 * Author:  saa & yuriyrusinov
 * Modified: 07 апреля 2009 г. 11:10:40
 * Purpose: Declaration of the class JKKSPMessage
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSPMessage_h)
#define __KKSSITOOOM_JKKSPMessage_h

#include "kksinteractor_config.h"

#include <QByteArray>
#include <QString>
#include <QBuffer>
#include <QDataStream>
#include <QStringList>
#include <QDebug>

#include "JKKSMessage.h"

class _I_EXPORT JKKSPMessage
{
    public:
        JKKSPMessage (const QByteArray& mess, 
                      const int & type, 
                      const QString& sender_uid=QString(),
                      const QString& receiver_uid=QString());

        JKKSPMessage (void);
        JKKSPMessage (const JKKSPMessage& pmess);
        JKKSPMessage (const QByteArray& mess);

        QByteArray getData (void) const;
        int getType (void) const;
        void setType(int t);
        QByteArray serialize (void) const;

        QStringList getFiles (void) const;
        void setFiles (const QStringList& files);

        const QByteArray & cryptoHash() const; //hash-сумма. Возвращается сохраненное значение. Если оно пустое, то сначала пересчитывается
        const QByteArray & calculateCryptoHash() const;//вычисление hash-суммы. Вызывается метод serialize()


        unsigned char urgencyLevel() const {return m_urgencyLevel;}//категория срочности. Возвращается в формате, адаптированном для транспорта Юпитер
        unsigned char macLabel() const {return m_macLabel;}//гриф секретности. Возвращается в формате, адаптированном для транспорта Юпитер

        void setUrgencyLevel(const QString & code);//берется первый символ
        void setMacLabel(const QString & mac);//бедется первый символ

        const QString & senderUID() const{return m_senderUID;}
        const QString & receiverUID() const {return m_receiverUID;}
        bool verifyReceiver() const {return m_verifyReceiver;}

        void setVerifyReceiver(bool b) {m_verifyReceiver = b;}
        void setSenderUID(const QString & s) {m_senderUID = s;}
        void setReceiverUID(const QString & s) {m_receiverUID = s;}


private:
        QByteArray data;
        int messType;
        QStringList urlList;//файлы
        mutable QByteArray m_cryptoHash;// hash-сумма содержимого сообщения

    //public:
        QString m_senderUID;//уникальный идентификатор организации-отправителя
        QString m_receiverUID;//уникальный идентификатор организации-получателя
        bool m_verifyReceiver;//флаг необходимости проверки получателя на приемном конце (т.е. что посылка предназначается именно той организации, которая находится на приемном конце
        unsigned char m_urgencyLevel;//хранится в поле code таблицы urgency_levels (должно быть A, B, C, соответственно "внеочередная", "срочная", "обычная")
        unsigned char m_macLabel;//возвращается функцией сервера (должно быть 0, 1, 2, 3 - НС, С, СС, ОВ)
        
};

class _I_EXPORT JKKSPMessWithAddr
{
    public:
        JKKSPMessage pMess;
        JKKSAddress addr; //адрес (IP+порт), на который отправляется сообщение
        QString unp;// условный номер получателя сообщения. В качестве такового используется поле email_prefix организации (в настоящее время используется только для отправки через ТПС)
        qint64 id; //идентификатор сообщения из табдицы out_sync_queue или message_journal или command_journal или out_external_queue

        JKKSPMessWithAddr (void);
        JKKSPMessWithAddr (const JKKSPMessage& pm, const JKKSAddress & a, const qint64& c);
        JKKSPMessWithAddr (JKKSPMessage *, const JKKSAddress & a, const qint64& c);
        ~JKKSPMessWithAddr (void);
};

#endif
