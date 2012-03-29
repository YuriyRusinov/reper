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
        QByteArray serialize (void) const;

        QStringList getFiles (void) const;
        void setFiles (const QStringList& files);

        unsigned char urgencyLevel() const {return m_urgencyLevel;}//категория срочности. Возвращается в формате, адаптированном для транспорта Юпитер
        unsigned char macLabel() const {return m_macLabel;}//гриф секретности. Возвращается в формате, адаптированном для транспорта Юпитер

        void setUrgencyLevel(const QString & code);//берется первый символ
        void setMacLabel(const QString & mac);//бедется первый символ

private:
        QByteArray data;
        int messType;
        QStringList urlList;

    public:
        QString senderUID;
        QString receiverUID;
        bool verifyReceiver;
        unsigned char m_urgencyLevel;//хранится в поле code таблицы urgency_levels (должно быть A, B, C, соответственно "внеочередная", "срочная", "обычная")
        unsigned char m_macLabel;//возвращается функцией сервера (должно быть 0, 1, 2, 3 - НС, С, СС, ОВ)
        
};

class _I_EXPORT JKKSPMessWithAddr
{
    public:
        JKKSPMessage pMess;
        QString addr;
        int id;

        JKKSPMessWithAddr (void);
        JKKSPMessWithAddr (const JKKSPMessage& pm, const QString& a, const int& c);
        JKKSPMessWithAddr (JKKSPMessage *, const QString& a, const int& c);
        ~JKKSPMessWithAddr (void);
};

#endif
