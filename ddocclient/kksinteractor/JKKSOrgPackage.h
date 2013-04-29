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
    JKKSOrgPackage (int id=-1, 
                    const JKKSAddress & addr = JKKSAddress(), 
                    const QString& code=QString(), 
                    const QString& uid=QString());
    JKKSOrgPackage (const JKKSOrgPackage& OP);

    virtual QByteArray serialize (void) const;
    virtual int unserialize (const QByteArray& mess);
    virtual int id (void) const;
    virtual int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
    virtual JKKSMessageType getMessageType (void) const;

    const QMap<int, JKKSTransport>& getTransports (void) const;
    void setTransports (const QMap<int, JKKSTransport>& T);

    const QMap<int, JKKSOrganization>& getOrgs (void) const;
    void setOrgs (const QMap<int, JKKSOrganization>& orgs);

    void setId (int id);

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
    int m_id;
    QMap<int, JKKSTransport> Transports;
    QMap<int, JKKSOrganization> Orgs;
};

#endif	/* __KKSSITOOOM_JKKSORGPACKAGE_H */

