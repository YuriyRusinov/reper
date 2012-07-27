/***********************************************************************
 * Module:  JKKSMessage.h
 * Author:  saa & yuriyrusinov
 * Modified: 30 марта 2009 г. 18:05:40
 * Purpose: Declaration of the class JKKSMessage
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSMessage_h)
#define __KKSSITOOOM_JKKSMessage_h

#include "kksinteractor_config.h"

#include <QString>
#include <QMap>

#include "JKKSCategory.h"

class JKKSCategory;
class JKKSLoader;

class _I_EXPORT JKKSMessage
{
    public:
        JKKSMessage (const QString& address=QString(), const QString& code=QString());
        JKKSMessage (const JKKSMessage& mess);
        virtual ~JKKSMessage (void);

        QString getAddr (void) const;
        void setAddr (const QString& addr);

        QString getCode (void) const;
        void setCode (const QString& code);

        virtual QByteArray serialize (void) const;
        virtual int unserialize (const QByteArray& mess)=0;

        virtual const QMap<int, JKKSCategory>& getCategory (void) const;
        virtual void setCategory (const QMap<int, JKKSCategory>& catMap);
        
        virtual int id (void) const=0;
        virtual int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID) = 0;

        enum JKKSMessageType
        {
            atUnknownType = -1,
            atCommand = 0,
            atDocument = 1,
            atMailMessage = 2,
            atMailConfirmation = 3,
            atCmdConfirmation = 4,
            atRecord = 5,
            atRecConfirmation = 6,
            atCategory = 7,
            atOrganization = 8,
            atPosition = 10,
            atOrgPackage = 11,
            atFilePart = 12
        };

        virtual JKKSMessageType getMessageType (void) const;

    private:
        QString m_addr;
        QString m_kvs;
        QMap<int, JKKSCategory> c;
};

#endif
