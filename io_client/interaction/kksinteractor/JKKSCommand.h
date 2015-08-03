/***********************************************************************
 * Module:  JKKSCommand.h
 * Author:  saa & yuriyrusinov
 * Modified: 30 марта 2009 г. 18:47:40
 * Purpose: Declaration of the class JKKSCommand
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSCommand_h)
#define __KKSSITOOOM_JKKSCommand_h

#include <QDateTime>
#include <QTextStream>

#include "kksinteractor_config.h"

#include "JKKSMessage.h"
#include "jkksuid.h"
#include "JKKSDocument.h"

/*
 * Класс командных сообщений
 */

class JKKSCategory;
class JKKSLoader;

class _I_EXPORT JKKSCmdConfirmation: public JKKSMessage
{
    public:
        
        JKKSCmdConfirmation(qint64 idCmd=-1, 
                            const QString & extraId = QString(),
                            qint64 idState = 3,
                            const QDateTime & acceptedDatetime = QDateTime::currentDateTime(),
                            const QDateTime & receiveDatetime = QDateTime::currentDateTime(),
                            const JKKSAddress & addr=JKKSAddress(),
                            const QString & kvs=QString());

        JKKSCmdConfirmation(const JKKSCmdConfirmation & cfm);
        virtual ~JKKSCmdConfirmation();

        QByteArray serialize (void) const;
        int unserialize (const QByteArray& mess);
        int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
        JKKSMessageType getMessageType (void) const;

        qint64 id (void) const;
        void setId (qint64 id);

        const QString & extraId (void) const;
        void setExtraId (const QString & uid);

        qint64 idState() const;
        void setIdState(qint64 state);

        const QDateTime & acceptedDatetime() const;
        void setAcceptedDatetime(const QDateTime & dt);

        const QDateTime & receiveDatetime() const;
        void setReceiveDatetime(const QDateTime & dt);

private:
        qint64 m_id;//src_id
        QString unique_id;//extraID
        qint64 m_idState;
        QDateTime m_acceptedDatetime;
        QDateTime m_receiveDatetime;

};

class _I_EXPORT JKKSCommand : public JKKSMessage, public JKKSUID
{
    public:
        JKKSCommand (qint64 id=-1, 
                     const QString& u_idFrom=QString(),
                     const QString& dl_from_name=QString(), 
                     const QString& u_idExec=QString(), 
                     const QString& dl_executor_name=QString(), 
                     const JKKSAddress & eAddr = JKKSAddress(), 
                     const JKKSAddress & eOrgAddr = JKKSAddress(), 
                     const QString& u_idTo=QString(), 
                     const QString& dl_to_name=QString(), 
                     qint64 idIoCat=-1, 
                     const QString& category_code=QString(), 
                     qint64 idJrSt=-1, 
                     const QDateTime& iTime=QDateTime(), 
                     const QDateTime& eTime=QDateTime(), 
                     int e_t_interval=1, 
                     int t_unit=6, //days
                     const QString& mess_body=QString(), 
                     const JKKSAddress & address = JKKSAddress(), 
                     const QString& code=QString(),
                     const QString& uid=QString(),
                     const QString & inputNumber=QString(),
                     const QString & outputNumber=QString(),
                     qint64 idUL = 1,
                     qint64 idObj=-1);

        JKKSCommand (const JKKSCommand& command);
        virtual ~JKKSCommand (void);

        QByteArray serialize (void) const;
        int unserialize (const QByteArray& mess);

        int writeToDB(const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);

        JKKSMessageType getMessageType (void) const;

        const QMap<qint64, JKKSCategory>& getCategory (void) const;
        void setCategory (const QMap<qint64, JKKSCategory>& catMap);

        qint64 id (void) const;
        void setId (qint64 id);

        QString getDlFrom (void) const;
        void setDlFrom (const QString& u_idFrom);
        QString getDlFromName (void) const;
        void setDlFromName (const QString& dl_from_name);

        QString getDlExec (void) const;
        void setDlExec (const QString& u_idExec);
        QString getDlExecutorName (void) const;
        void setDlExecutorName (const QString& dl_executor_name);

        const JKKSAddress & getEAddr (void) const;
        void setEAddr (const JKKSAddress & eAddr);

        const JKKSAddress & getEOrg (void) const;
        void setEOrg (const JKKSAddress & eOrg);

        QString getDlTo (void) const;
        void setDlTo (const QString& u_idTo);
        QString getDlToName (void) const;
        void setDlToName (const QString& dl_to_name);

        qint64 getIdIoCat (void) const;
        void setIdIoCat (qint64 id_io_category);
        QString getCategoryCode (void) const;
        void setCategoryCode (const QString& category_code);

        qint64 getJournalState (void) const;
        void setJournalState (qint64 idState);

        const QDateTime& getInsertTime (void) const;
        void setInsertTime (const QDateTime& iTime);

        const QDateTime& getExecTime (void) const;
        void setExecTime (const QDateTime& eTime);

        QString getTimeInterval (void) const;
        void setTimeInterval (int val, int unit=6);//days

        QString getMessageBody (void) const;
        void setMessageBody (const QString& mess);

        const QString & inputNumber() const {return m_inputNumber;}
        void setInputNumber(const QString & in) { m_inputNumber = in; }

        const QString & outputNumber (void) const {return m_outputNumber;}
        void setOutputNumber(const QString & on) { m_outputNumber = on; }

        qint64 idUrgencyLevel (void) const {return m_idUrgencyLevel; }
        void setIdUrgencyLevel (qint64 idUL) { m_idUrgencyLevel = idUL; }

        qint64 idObject (void) const { return m_idObject; }
        void setIdObject(qint64 idObj) { m_idObject = idObj; }

        const JKKSDocument& getAttachedDoc (void) const;
        void setAttachment (const JKKSDocument& doc);
private:
        //
        // Functions
        //
        QTextStream& loadFromBuffer (QTextStream& f, QString& str);

    private:
        //
        // Variables
        //
        qint64 idCommand;
        QString u_idDlFrom;
        QString dlFromName;
        QString u_idDlExecutor;
        QString dlExecutorName;
        JKKSAddress executorAddress; //not used
        JKKSAddress execOrgAddress; //not used
        QString u_idDlTo;
        QString dlToName;
        qint64 idIOCategory;
        QString categoryCode;
        qint64 idJrState;
        QDateTime insertTime;
        QDateTime execTime;
        int exec_time_interval;
        int time_unit;
        QString messageBody;
        QString m_inputNumber;
        QString m_outputNumber;
        qint64 m_idUrgencyLevel;
        qint64 m_idObject;

        QMap<qint64, JKKSCategory> category;
        JKKSDocument ioDoc;
};

#endif
