/***********************************************************************
 * Module:  JKKSMessage.h
 * Author:  yuriyrusinov
 * Modified: 2 ��� 2009 �. 17:30
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
        
        JKKSMailConfirmation(int idMess=-1, 
                             int extraId=-1,
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

        int id (void) const;
        void setId (int id);
        
        int extraId() const;
        void setExtraId(int id);

        const QDateTime & readDatetime() const;
        void setReadDatetime(const QDateTime & dt);

        const QDateTime & receiveDatetime() const;
        void setReceiveDatetime(const QDateTime & dt);

private:
        int m_id;//m_src_id
        int m_dst_id;//extra_id
        QDateTime m_readDatetime;
        QDateTime m_receiveDatetime;

};

class _I_EXPORT JKKSMailMessage : public JKKSMessage, public JKKSUID
{
    public:
        JKKSMailMessage (int idMess=-1, 
                         int idObject=-1, 
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
        // ��� ��������������� ����������� ������� �������� ������
        //
        QByteArray serialize (void) const;
        int unserialize (const QByteArray& mess);
        int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
        int id (void) const;
        JKKSMessageType getMessageType (void) const;

        void setId (int id);

        int getIO (void) const;
        void setIO (int idObject);

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
        int idMessage;
        int idIO;
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
