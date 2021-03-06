#if !defined (__KKSSITOOOM_JKKSTransport_H)
#define __KKSSITOOOM_JKKSTransport_H

#include "kksinteractor_config.h"
#include "jkksuid.h"

class QDataStream;
#include "JKKSMessage.h"

class _I_EXPORT JKKSTransport : public JKKSUID
{
public:
    JKKSTransport (qint64 id_transport=-1, 
                   const QString& trName=QString(), 
                   const JKKSAddress & lAddr = JKKSAddress(), 
                   bool isActive=false, 
                   const QString& uid=QString());
    
    JKKSTransport (const JKKSTransport& T);
    ~JKKSTransport (void);

    qint64 id (void) const;
    void setId (qint64 id);

    QString getTransportName (void) const;
    void setTransportName (const QString& name);

    const JKKSAddress & getAddress(void) const;
    void setAddress (const JKKSAddress & addr);

    bool isTransportActive (void) const;
    void setTransportActive (bool isActive);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSTransport& T);
    friend QDataStream& operator>> (QDataStream& in, JKKSTransport& T);
private:
    //
    // Variables
    //
    qint64 idTransport;
    QString transportName;
    JKKSAddress localAddr;
    bool m_isActive;
};

#endif
