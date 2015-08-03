/* 
 * File:   JKKSOrgPackage.h
 * Author: rusinov
 *
 * Created on 6 јпрель 2011 г., 18:01
 */

#ifndef __KKSSITOOOM_JKKSORGPACKAGE_H
#define	__KKSSITOOOM_JKKSORGPACKAGE_H

#include "kksinteractor_config.h"
#include "jkksuid.h"
#include "JKKSMessage.h"
#include "JKKSOrganization.h"
#include "JKKSTransport.h"

class QDataStream;

class JKKSLoader;

class _I_EXPORT JKKSOrgPackage : public JKKSMessage, public JKKSUID
{
public:
    JKKSOrgPackage (qint64 id=-1, 
                    const JKKSAddress & addr = JKKSAddress(), 
                    const QString& code=QString(), 
                    const QString& uid=QString());

    JKKSOrgPackage (const JKKSOrgPackage& OP);

    virtual QByteArray serialize (void) const;
    virtual int unserialize (const QByteArray& mess);
    virtual qint64 id (void) const;
    virtual int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
    virtual JKKSMessageType getMessageType (void) const;

    const QMap<qint64, JKKSTransport>& getTransports (void) const;
    void setTransports (const QMap<qint64, JKKSTransport>& T);

    const QMap<qint64, JKKSOrganization>& getOrgs (void) const;
    void setOrgs (const QMap<qint64, JKKSOrganization>& orgs);

    void setId (qint64 id);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSOrgPackage& OP);
    friend QDataStream& operator>> (QDataStream& in, JKKSOrgPackage& OP);

private:
    //
    // Variables
    //
    qint64 m_id;
    QMap<qint64, JKKSTransport> Transports;
    QMap<qint64, JKKSOrganization> Orgs;
};

#endif	/* __KKSSITOOOM_JKKSORGPACKAGE_H */

