#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QtDebug>

#include "JKKSLoader.h"
#include "JKKSOrganization.h"
#include "defines.h"

JKKSOrganization :: JKKSOrganization (int idOrg,
                                      const QString& uidParent,
                                      const QString& uidParent1,
                                      const QString& uidSubstitutor,
                                      const QString& name,
                                      const QString& shortName,
                                      const QString& codeName,
                                      const QString& email_prefix,
                                      double longitude,
                                      double latitude,
                                      const QString& mapSymbol,
                                      const QString& treeSymbol,
                                      const QDateTime& dtPrevMode,
                                      const QDateTime& dtCurrMode,
                                      const QString& addr,
                                      const QString& mess_code,
                                      const QString& uid)
    : JKKSMessage (addr, mess_code),
      JKKSUID (uid),
      m_id (idOrg),
      m_uidParent (uidParent),
      m_uidParent1 (uidParent1),
      m_uidSubstitutor (uidSubstitutor),
      m_name (name),
      m_shortName (shortName),
      m_codeName (codeName),
      m_email_prefix (email_prefix),
      m_longitude (longitude),
      m_latitude (latitude),
      m_mapSymbol (mapSymbol),
      m_treeSymbol (treeSymbol),
      m_dtPrevMode (dtPrevMode),
      m_dtCurrMode (dtCurrMode)
{
}

JKKSOrganization :: JKKSOrganization (const JKKSOrganization& Org)
    : JKKSMessage (Org),
      JKKSUID (Org),
      m_id (Org.m_id),
      orgType (Org.orgType),
      m_uidParent (Org.m_uidParent),
      m_uidParent1 (Org.m_uidParent1),
      m_uidSubstitutor (Org.m_uidSubstitutor),
      currMode (Org.currMode),
      prevMode (Org.prevMode),
      m_name (Org.m_name),
      m_shortName (Org.m_shortName),
      m_codeName (Org.m_codeName),
      m_email_prefix (Org.m_email_prefix),
      m_longitude (Org.m_longitude),
      m_latitude (Org.m_latitude),
      m_mapSymbol (Org.m_mapSymbol),
      m_treeSymbol (Org.m_treeSymbol),
      m_dtPrevMode (Org.m_dtPrevMode),
      m_dtCurrMode (Org.m_dtCurrMode),
      workModes (Org.workModes),
      orgTransport (Org.orgTransport)
{
}

JKKSOrganization :: ~JKKSOrganization (void)
{
}
//
// virtual functions definition
//
QByteArray JKKSOrganization :: serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    QByteArray ba;
    out << (*this);
    ba = qBuffer.buffer();

    return ba;
}

int JKKSOrganization :: unserialize (const QByteArray& mess)
{
    QBuffer qBuffer;
    qBuffer.setData (mess);
    qBuffer.open (QIODevice::ReadOnly);
    QDataStream in (&qBuffer);
    in >> (*this);

    return OK_CODE;
}

int JKKSOrganization :: writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    //qDebug() << __PRETTY_FUNCTION__;
    Q_UNUSED (senderUID);
    int ier = loader->writeMessage (this, receiverUID);
    return ier;
}

int JKKSOrganization :: id (void) const
{
    return m_id;
}

JKKSMessage::JKKSMessageType JKKSOrganization :: getMessageType (void) const
{
    return JKKSMessage::atOrganization;
}

void JKKSOrganization :: setId (int id)
{
    m_id = id;
}

JKKSOrgType JKKSOrganization :: getType (void) const
{
    return orgType;
}

void JKKSOrganization :: setType (const JKKSOrgType& oType)
{
    orgType = oType;
}

const QString& JKKSOrganization :: getParent (void) const
{
    return m_uidParent;
}

void JKKSOrganization :: setParent (const QString& uidPar)
{
    m_uidParent = uidPar;
}

const QString& JKKSOrganization :: getParentPart (void) const
{
    return m_uidParent1;
}

void JKKSOrganization :: setParentPart (const QString& uidPar)
{
    m_uidParent1 = uidPar;
}

const QString& JKKSOrganization :: getSubstitutor (void) const
{
    return m_uidSubstitutor;
}

void JKKSOrganization :: setSubstitutor (const QString& uidSubst)
{
    m_uidSubstitutor = uidSubst;
}

const JKKSWorkMode& JKKSOrganization :: getCurrentMode (void) const
{
    return currMode;
}

void JKKSOrganization :: setCurrentMode (const JKKSWorkMode& c_Mode)
{
    currMode = c_Mode;
}

const JKKSWorkMode& JKKSOrganization :: getPreviousMode (void) const
{
    return prevMode;
}

void JKKSOrganization :: setPreviousMode (const JKKSWorkMode& p_mode)
{
    prevMode = p_mode;
}

const QString& JKKSOrganization :: getName (void) const
{
    return m_name;
}

void JKKSOrganization :: setName (const QString& name)
{
    m_name = name;
}

const QString& JKKSOrganization :: getShortName (void) const
{
    return m_shortName;
}

void JKKSOrganization :: setShortName (const QString& sName)
{
    m_shortName = sName;
}

const QString& JKKSOrganization :: getCode (void) const
{
    return m_codeName;
}

void JKKSOrganization :: setCode (const QString& codeName)
{
    m_codeName = codeName;
}

const QString& JKKSOrganization :: getEMail (void) const
{
    return m_email_prefix;
}

void JKKSOrganization :: setEMail (const QString& mail)
{
    m_email_prefix = mail;
}

void JKKSOrganization :: getCoords (double& longitude, double& latitude) const
{
    longitude = m_longitude;
    latitude = m_latitude;
}

void JKKSOrganization :: setCoords (double longitude, double latitude)
{
    m_longitude = longitude;
    m_latitude = latitude;
}

const QString& JKKSOrganization :: getMapSymbol (void) const
{
    return m_mapSymbol;
}

void JKKSOrganization :: setMapSymbol (const QString& maps)
{
    m_mapSymbol = maps;
}

const QString& JKKSOrganization :: getTreeSymbol (void) const
{
    return m_treeSymbol;
}

void JKKSOrganization :: setTreeSymbol (const QString& trees)
{
    m_treeSymbol = trees;
}

const QDateTime& JKKSOrganization :: getTimeCMode (void) const
{
    return m_dtCurrMode;
}

void JKKSOrganization :: setTimeCMode (const QDateTime& dt)
{
    m_dtCurrMode = dt;
}

const QDateTime& JKKSOrganization :: getTimePMode (void) const
{
    return m_dtPrevMode;
}

void JKKSOrganization :: setTimePMode (const QDateTime& dt)
{
    m_dtPrevMode = dt;
}

const JKKSTransport& JKKSOrganization :: getTransport (void) const
{
    return orgTransport;
}

void JKKSOrganization :: setTransport (const JKKSTransport& T)
{
    orgTransport = T;
}

const QMap<int, JKKSWorkMode>& JKKSOrganization :: getWorkModes (void) const
{
    return workModes;
}

void JKKSOrganization :: setWorkModes (const QMap<int, JKKSWorkMode>& wms)
{
    workModes = wms;
}

QDataStream& operator<< (QDataStream& out, const JKKSOrganization& OO)
{
    out << OO.getAddr();
    out << OO.getCode();

    out << OO.m_id;
    out << OO.orgType;
    out << OO.m_uidParent;
    out << OO.m_uidParent1;
    out << OO.m_uidSubstitutor;
    out << OO.currMode;
    out << OO.prevMode;
    out << OO.m_name;
    out << OO.m_shortName;
    out << OO.m_codeName;
    out << OO.m_email_prefix;
    out << OO.m_latitude;
    out << OO.m_longitude;
    out << OO.m_mapSymbol;
    out << OO.m_treeSymbol;
    out << OO.m_dtPrevMode;
    out << OO.m_dtCurrMode;
    out << OO.orgTransport;
    out << OO.workModes;

    out << OO.uid();

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSOrganization& OO)
{
    QString addr;
    QString code;

    in >> addr;
    in >> code;

    in >> OO.m_id;
    in >> OO.orgType;
    in >> OO.m_uidParent;
    in >> OO.m_uidParent1;
    in >> OO.m_uidSubstitutor;
    in >> OO.currMode;
    in >> OO.prevMode;
    in >> OO.m_name;
    in >> OO.m_shortName;
    in >> OO.m_codeName;
    in >> OO.m_email_prefix;
    in >> OO.m_latitude;
    in >> OO.m_longitude;
    in >> OO.m_mapSymbol;
    in >> OO.m_treeSymbol;
    in >> OO.m_dtPrevMode;
    in >> OO.m_dtCurrMode;
    in >> OO.orgTransport;
    in >> OO.workModes;

    QString uid;
    in >> uid;
    OO.setUid (uid);
    OO.setAddr (addr);// = JKKSRefRecord (avals, uid);
    OO.setCode (code);
    return in;
}
