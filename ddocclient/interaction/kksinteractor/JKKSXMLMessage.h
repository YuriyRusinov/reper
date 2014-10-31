/***********************************************************************
 * Module:  JKKSXMLMessage.h
 * Author:  ksa
 * Modified: 07 окт€бр€ 2014 г. 11:36:40
 * Purpose: Declaration of the class JKKSXMLMessage
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSXMLMessage_h)
#define __KKSSITOOOM_JKKSXMLMessage_h

#include <QDateTime>
#include <QTextStream>

#include "kksinteractor_config.h"

#include "JKKSMessage.h"
#include "jkksuid.h"

class JKKSLoader;

class _I_EXPORT JKKSXMLMessage: public JKKSMessage
{
    public:
        
        JKKSXMLMessage(qint64 idQueue = -1, 
                       int idFormat = 1, //IRL for default
                       int idOrganization = -1, //receuver org (not used by default)
                       int interactionType = 2, // update if exists (create if not exists)
                       const QString & xmlData = QString(),
                       const JKKSAddress & addr=JKKSAddress(),
                       const QString & kvs=QString());

        JKKSXMLMessage(const JKKSXMLMessage& xml);
        virtual ~JKKSXMLMessage();

        QByteArray serialize (void) const;
        int unserialize (const QByteArray& mess);
        int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
        JKKSMessageType getMessageType (void) const;

        qint64 id (void) const;
        void setId (qint64 id);

        int idFormat (void) const;
        void setIdFormat (int idFmt);

        int idOrganization (void) const;
        void setIdOrganization (int idOrg);
                
        int interactionType (void) const;
        void setInteractionType (int type);

        const QString & xmlData (void) const;
        void setXMLData (const QString & xml);

/*
        const QString & extraId (void) const;
        void setExtraId (const QString & uid);
*/

private:
        qint64 m_id;//src_id
        int m_idOrganization;
        int m_idFormat;
        int m_interactionType;
        QString m_xmlData;
/*
        QString unique_id;//extraID
        qint64 m_idState;
        QDateTime m_acceptedDatetime;
        QDateTime m_receiveDatetime;
*/

};

#endif
