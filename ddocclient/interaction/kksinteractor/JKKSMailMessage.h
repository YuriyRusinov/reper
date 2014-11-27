/***********************************************************************
 * Module:  JKKSMessage.h
 * Author:  yuriyrusinov
 * Modified: 2 мая 2009 г. 17:30
 * Purpose: Declaration of the class JKKSMailMessage
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSMailMessage_h)
#define __KKSSITOOOM_JKKSMailMessage_h

#include "kksinteractor_config.h"

#include <QString>
#include <QMap>
#include <QDateTime>

#include "JKKSMessage.h"
#include "JKKSDocument.h"
#include "jkksuid.h"

class _I_EXPORT JKKSMailConfirmation: public JKKSMessage
{
    public:
        
        JKKSMailConfirmation(int confirmationType,
                             qint64 idMess=-1, 
                             qint64 extraId=-1,
                             const QDateTime & readDatetime = QDateTime::currentDateTime(),
                             const QDateTime & receiveDatetime = QDateTime::currentDateTime(),
                             const JKKSAddress & addr=JKKSAddress(),
                             const QString & kvs=QString());

        JKKSMailConfirmation(const JKKSMailConfirmation & cfm);
        virtual ~JKKSMailConfirmation();

        QByteArray serialize (void) const;
        int unserialize (const QByteArray& mess);
        int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
        JKKSMessageType getMessageType (void) const;

        qint64 id (void) const;
        void setId (qint64 id);
        
        qint64 extraId() const;
        void setExtraId(qint64 id);

        const QDateTime & readDatetime() const;
        void setReadDatetime(const QDateTime & dt);

        const QDateTime & receiveDatetime() const;
        void setReceiveDatetime(const QDateTime & dt);

        int confirmationType() const;

private:
        qint64 m_id;//m_src_id
        qint64 m_dst_id;//extra_id
        QDateTime m_readDatetime;
        QDateTime m_receiveDatetime;
        int m_confirmationType; // 0 - квитанция о получении
                                // 1 - квитанция о прочтении

};

class _I_EXPORT JKKSMailMessage : public JKKSMessage, public JKKSUID
{
    public:
        JKKSMailMessage (qint64 idMess=-1, 
                         qint64 idObject=-1, 
                         const QString& messBody=QString(), 
                         const QString& u_idDlReceiver=QString(),
                         const QString& u_idDlSender=QString(), 
                         const QDateTime& dt = QDateTime::currentDateTime(), 
                         const JKKSAddress & addr=JKKSAddress(), 
                         const QString& mess_code=QString(),
                         const QString & uid=QString(),
                         const QString & inputNumber=QString(),
                         const QString & outputNumber=QString(),
                         int idUL = 1);

        JKKSMailMessage (const JKKSMailMessage& mess);
        virtual ~JKKSMailMessage (void);

        //
        // virtual functions definition
        // Это переопределение виртуальных функций базового класса
        //
        QByteArray serialize (void) const;
        int unserialize (const QByteArray& mess);
        int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
        JKKSMessageType getMessageType (void) const;

        qint64 id (void) const;
        void setId (qint64 id);

        qint64 getIO (void) const;
        void setIO (qint64 idObject);

        const QString& getMessageBody (void) const;
        void setMessageBody (const QString& mess);

        QString getIdDlTo (void) const;
        void setIdDlTo (const QString& u_idTo);

        QString getIdDlFrom (void) const;
        void setIdDlFrom (const QString& u_idFrom);

        const QDateTime& getSentDateTime (void) const;
        void setSentDateTime (const QDateTime& dt);

        const JKKSDocument& getAttachedDoc (void) const;
        void setAttachment (const JKKSDocument& doc);

        const QString & inputNumber() const {return m_inputNumber;}
        void setInputNumber(const QString & in) { m_inputNumber = in; }

        const QString & outputNumber() const {return m_outputNumber;}
        void setOutputNumber(const QString & on) { m_outputNumber = on; }

        int idUrgencyLevel() const {return m_idUrgencyLevel;}
        void setIdUrgencyLevel(int idUL) { m_idUrgencyLevel = idUL; }
private:
        qint64 idMessage;
        qint64 idIO;
        QString messageBody;
        QString u_idDlTo;
        QString u_idDlFrom;
        QDateTime sentDateTime;
        QString m_inputNumber;
        QString m_outputNumber;
        int m_idUrgencyLevel;

        JKKSDocument ioDoc;
};

#endif
