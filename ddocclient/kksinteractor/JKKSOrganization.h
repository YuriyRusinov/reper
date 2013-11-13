#ifndef __KKSSITOOOM_JKKSOrganization_H
#define __KKSSITOOOM_JKKSOrganization_H

#include "kksinteractor_config.h"
#include "jkksuid.h"
#include "JKKSMessage.h"
#include "JKKSWorkMode.h"
#include "JKKSOrgType.h"
#include "JKKSTransport.h"

#include <QString>
#include <QDateTime>

class QDataStream;

class JKKSLoader;

class _I_EXPORT JKKSOrganization : public JKKSMessage, public JKKSUID
{
public:
    JKKSOrganization (qint64 idOrg=-1,
                      const QString& uidParent=QString(),
                      const QString& uidParent1=QString(),
                      const QString& uidSubstitutor=QString(),
                      const QString& name=QString(),
                      const QString& shortName=QString(),
                      const QString& codeName=QString(),
                      const QString& email_prefix=QString(),
                      double longitude=0.0,
                      double latitude=0.0,
                      const QString& mapSymbol=QString(),
                      const QString& treeSymbol=QString(),
                      const QDateTime& dtPrevMode=QDateTime(),
                      const QDateTime& dtCurrMode=QDateTime(),
                      const JKKSAddress & addr=JKKSAddress(),
                      const QString& mess_code=QString(),
                      const QString& uid = QString());

    JKKSOrganization (const JKKSOrganization& Org);
    ~JKKSOrganization (void);

    QByteArray serialize (void) const;
    int unserialize (const QByteArray& mess);
    int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
    qint64 id (void) const;
    JKKSMessageType getMessageType (void) const;

    void setId (qint64 id);

    void setIdQueue(qint64 idQueue);
    qint64 idQueue() const;

    JKKSOrgType getType (void) const;
    void setType (const JKKSOrgType& oType);

    const QString& getParent (void) const;
    void setParent (const QString& uidPar);

    const QString& getParentPart (void) const;
    void setParentPart (const QString& idPar);

    const QString& getSubstitutor (void) const;
    void setSubstitutor (const QString& uidSubst);

    const JKKSWorkMode& getCurrentMode (void) const;
    void setCurrentMode (const JKKSWorkMode& cMode);

    const JKKSWorkMode& getPreviousMode (void) const;
    void setPreviousMode (const JKKSWorkMode& pMode);

    const QMap<qint64, JKKSWorkMode>& getWorkModes (void) const;
    void setWorkModes (const QMap<qint64, JKKSWorkMode>& wms);

    const QString& getName (void) const;
    void setName (const QString& name);

    const QString& getShortName (void) const;
    void setShortName (const QString& sName);

    const QString& getCode (void) const;
    void setCode (const QString& codeName);

    const QString& getEMail (void) const;
    void setEMail (const QString& mail);

    void getCoords (double& longitude, double& latitude) const;
    void setCoords (double longitude, double latitude);

    const QString& getMapSymbol (void) const;
    void setMapSymbol (const QString& maps);

    const QString& getTreeSymbol (void) const;
    void setTreeSymbol (const QString& trees);

    const QDateTime& getTimeCMode (void) const;
    void setTimeCMode (const QDateTime& dt);

    const QDateTime& getTimePMode (void) const;
    void setTimePMode (const QDateTime& dt);

    const JKKSTransport& getTransport (void) const;
    void setTransport (const JKKSTransport& T);

private:
    //
    // Functions
    //
    friend QDataStream& operator<< (QDataStream& out, const JKKSOrganization& OO);
    friend QDataStream& operator>> (QDataStream& in, JKKSOrganization& OO);

private:
    //
    // Variables
    //
    qint64 m_id;
    qint64 m_idQueue; //если организация передается посредством чтения таблицы out_sync_queue, то данное поле содержит соответствующий идентификатор в ней. Используется на приемном конце для создания квитанции в queue_results

    JKKSOrgType orgType;

    QString m_uidParent;
    QString m_uidParent1;
    QString m_uidSubstitutor;

    JKKSWorkMode currMode;
    JKKSWorkMode prevMode;

    QString m_name;
    QString m_shortName;
    QString m_codeName;
    QString m_email_prefix;

    double m_longitude;
    double m_latitude;

    QString m_mapSymbol;
    QString m_treeSymbol;

    QDateTime m_dtPrevMode;
    QDateTime m_dtCurrMode;

    QMap<qint64, JKKSWorkMode> workModes;

    JKKSTransport orgTransport;
};

#endif
