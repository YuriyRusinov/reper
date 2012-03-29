#if !defined (__KKSSITOOOM_JKKSTransport_H)
#define __KKSSITOOOM_JKKSTransport_H

#include "kksinteractor_config.h"
#include "jkksuid.h"

class QDataStream;

class _I_EXPORT JKKSTransport : public JKKSUID
{
public:
    JKKSTransport (int id_transport=-1, const QString& trName=QString(), const QString& lAddr=QString(), bool isActive=false, const QString& uid=QString());
    JKKSTransport (const JKKSTransport& T);
    ~JKKSTransport (void);

    int id (void) const;
    void setId (int id);

    QString getTransportName (void) const;
    void setTransportName (const QString& name);

    QString getAddr (void) const;
    void setAddr (const QString& addr);

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
    int idTransport;
    QString transportName;
    QString localAddr;
    bool m_isActive;
};

#endif
