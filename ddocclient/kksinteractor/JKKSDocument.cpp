#include <QBuffer>
#include <QtDebug>
#include <QDataStream>

#include "JKKSCategory.h"
#include "JKKSDocument.h"
#include "JKKSLoader.h"
#include "defines.h"


JKKSDocument :: JKKSDocument (const QString& name, 
                              qint64 idCat, 
                              qint64 idAuth, 
                              const QString& tName, 
                              const QString& desc, 
                              const QString& info, 
                              qint64 idmaclabel, 
                              qint64 idsynctype,
                              const QString& sync_name,
                              const QString& sync_desc,
                              qint64 idownerorg,
                              bool isglobal,
                              const QString & orgUID,

                              const JKKSAddress & addr, 
                              const QString& mess_code,

                              const QString & uid,
                              const QString & uuid,
                              qint64 idState, 
                              const QColor bkCol,
                              const QColor fgCol,
                              const QString& rIcon
                              )
    : JKKSMessage (addr, mess_code), JKKSUID(uid, uuid, idState, bkCol, fgCol, rIcon),
    idObject (-1),
    ioName (name),
    idCategory (idCat),
    idAuthor (idAuth),
    tableName (tName),
    ioDesc (desc),
    ioInfo (info),
    id_maclabel (idmaclabel),
    category (QMap<qint64, JKKSCategory>()),
    attrValues (QMap<QString, QString>()),
    m_rubrics (QMap<qint64, JKKSGlobalRubric>()),
    m_urls (QMap<qint64, JKKSIOUrl>()),
    idCommand (-1),
    realTime (QDateTime::currentDateTime()),
    idJournal (-1),
    idSyncType (idsynctype),
    syncName (sync_name),
    syncDesc (sync_desc),
    idOwnerOrg (idownerorg),
    isGlobal (isglobal),
    ownerOrgUID(orgUID)
{
}

JKKSDocument :: JKKSDocument (const JKKSDocument& io)
    : JKKSMessage (io), JKKSUID(io),
    idObject (io.idObject),
    ioName (io.ioName),
    idCategory (io.idCategory),
    idAuthor (io.idAuthor),
    tableName (io.tableName),
    ioDesc (io.ioDesc),
    ioInfo (io.ioInfo),
    id_maclabel (io.id_maclabel),
    category (io.category),
    attrValues (io.attrValues),
    m_rubrics (io.m_rubrics),
    m_urls (io.m_urls),
    m_tables (io.m_tables),
    idCommand (io.idCommand),
    realTime (io.realTime),
    idJournal (io.idJournal),
    idSyncType (io.idSyncType),
    syncName (io.syncName),
    syncDesc (io.syncDesc),
    idOwnerOrg (io.idOwnerOrg),
    isGlobal (io.isGlobal),
    ownerOrgUID(io.ownerOrgUID),
    m_type(io.m_type)
{
}

JKKSDocument :: ~JKKSDocument (void)
{
}

QByteArray JKKSDocument :: serialize (void) const
{
    QBuffer qBuffer;
    qBuffer.open (QIODevice::WriteOnly);
    QDataStream out (&qBuffer);

    out << getAddr(); //from jkksmessage
    out << getCode(); //from jkksmessage

    out << ioName;
    out << idCategory;
    out << idAuthor;
    out << tableName;
    out << ioDesc;
    out << ioInfo;
    out << id_maclabel;
    out << category;
    out << attrValues;
    out << m_rubrics;
    out << m_urls;
    out << m_tables;
    out << idCommand;
    out << realTime;
    out << idJournal;
    out << idSyncType;
    out << syncName;
    out << syncDesc;
    out << idOwnerOrg;
    out << isGlobal;
    out << ownerOrgUID;
    out << m_type;

    //from jkksuid
    out << uid();
    out << uuid();
    out << idState();
    out << bgColor();
    out << fgColor();
    out << rIcon();

    return qBuffer.buffer();
}

int JKKSDocument :: unserialize (const QByteArray& mess)
{
    QBuffer buffer;
    buffer.setData(mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    JKKSAddress addr;
    QString code;
    
    in >> addr; //from jkksmessage
    in >> code; //from jkksmessage
    setAddr (addr);
    setCode (code);
    
    in >> ioName;
    in >> idCategory;
    in >> idAuthor;
    in >> tableName;
    in >> ioDesc;
    in >> ioInfo;
    in >> id_maclabel;
    in >> category;
    in >> attrValues;
    in >> m_rubrics;
    in >> m_urls;
    in >> m_tables;
    in >> idCommand;
    in >> realTime;
    in >> idJournal;
    
    in >> idSyncType;
    in >> syncName;
    in >> syncDesc;
    in >> idOwnerOrg;
    in >> isGlobal;
    in >> ownerOrgUID;
    in >> m_type;

    //from jkksuid

    QString uid;
    QString uuid;
    qint64 idSt;
    QColor bgCol;
    QColor fgCol;
    QString rIc;

    in >> uid;
    in >> uuid;
    in >> idSt;
    in >> bgCol;
    in >> fgCol;
    in >> rIc;


    setUid (uid);
    setUuid(uuid);
    setIdState(idSt);
    setBgColor(bgCol);
    setFgColor(fgCol);
    setRIcon(rIc);

    return OK_CODE;
}

const QMap<qint64, JKKSCategory>& JKKSDocument :: getCategory (void) const
{
    return category;
}

void JKKSDocument :: setCategory (const QMap<qint64, JKKSCategory>& catMap)
{
    category = catMap;
}

int JKKSDocument :: writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID)
{
    //qDebug() << __PRETTY_FUNCTION__;
    Q_UNUSED (senderUID);
    Q_UNUSED (receiverUID);
    int ier = loader->writeMessage (this);
    return ier;
}

qint64 JKKSDocument :: id (void) const
{
    return idObject;
}

JKKSMessage::JKKSMessageType JKKSDocument :: getMessageType (void) const
{
    return JKKSMessage::atDocument;
}

const QMap<QString, QString>& JKKSDocument :: getAttrValues (void) const
{
    return attrValues;
}

void JKKSDocument :: setAttrValues (const QMap<QString, QString>& aVals)
{
    attrValues = aVals;
}

QString JKKSDocument :: getName (void) const
{
    return ioName;
}

void JKKSDocument :: setName (const QString& name)
{
    ioName = name;
}

qint64 JKKSDocument :: getIdIoCat (void) const
{
    return idCategory;
}

void JKKSDocument :: setIdIoCat (qint64 id_io_category)
{
    idCategory = id_io_category;
}

qint64 JKKSDocument :: getIdAuthor (void) const
{
    return idAuthor;
}

void JKKSDocument :: setIdAuthor (qint64 idAuth)
{
    idAuthor = idAuth;
}

QString JKKSDocument :: getTableName (void) const
{
    return tableName;
}

void JKKSDocument :: setTableName (const QString& tName)
{
    tableName = tName;
}

QString JKKSDocument :: getDescription (void) const
{
    return ioDesc;
}

void JKKSDocument :: setDescription (const QString& desc)
{
    ioDesc = desc;
}

QString JKKSDocument :: getInfo (void) const
{
    return ioInfo;
}

void JKKSDocument :: setInfo (const QString& info)
{
    ioInfo = info;
}

qint64 JKKSDocument :: getIdMaclabel (void) const
{
    return id_maclabel;
}

void JKKSDocument :: setIdMaclabel (qint64 idmaclabel)
{
    id_maclabel = idmaclabel;
}

void JKKSDocument :: setId (qint64 id)
{
    idObject = id;
}

qint64 JKKSDocument :: getCommandId (void) const
{
    return idCommand;
}

void JKKSDocument :: setCommandId (qint64 idc)
{
    idCommand = idc;
}

const QDateTime& JKKSDocument :: getRealTime (void) const
{
    return realTime;
}

void JKKSDocument :: setRealTime (const QDateTime& rt)
{
    realTime = rt;
}

qint64 JKKSDocument :: getJournal (void) const
{
    return idJournal;
}

void JKKSDocument :: setJournal (qint64 idJ)
{
    idJournal = idJ;
}

const QMap<qint64, JKKSGlobalRubric>& JKKSDocument :: rubrics (void) const
{
    return m_rubrics;
}

QMap<qint64, JKKSGlobalRubric>& JKKSDocument :: rubrics (void)
{
    return m_rubrics;
}

void JKKSDocument :: setRubrics (const QMap<qint64, JKKSGlobalRubric>& rubrs)
{
    m_rubrics = rubrs;
}

const QMap<qint64, JKKSIOUrl>& JKKSDocument :: urls (void) const
{
    return m_urls;
}

QMap<qint64, JKKSIOUrl>& JKKSDocument :: urls (void)
{
    return m_urls;
}

void JKKSDocument :: setUrls (const QMap<qint64, JKKSIOUrl>& urls)
{
    m_urls = urls;
}

const QMap<qint64, JKKSIOTable>& JKKSDocument :: ref_tables (void) const
{
    return m_tables;
}

QMap<qint64, JKKSIOTable>& JKKSDocument :: ref_tables (void)
{
    return m_tables;
}

void JKKSDocument :: setTables (const QMap<qint64, JKKSIOTable>& ref_tables)
{
    m_tables = ref_tables;
}

qint64 JKKSDocument :: getSyncType (void) const
{
    return idSyncType;
}

void JKKSDocument :: setSyncType (qint64 id_sync_type)
{
    idSyncType = id_sync_type;
}

const QString& JKKSDocument :: getSyncName (void) const
{
    return syncName;
}

void JKKSDocument :: setSyncName (const QString& sync_name)
{
    syncName = sync_name;
}

const QString& JKKSDocument :: getSyncDesc (void) const
{
    return syncDesc;
}

void JKKSDocument :: setSyncDesc (const QString& sync_desc)
{
    syncDesc = sync_desc;
}

qint64 JKKSDocument :: getOwnerOrg (void) const
{
    return idOwnerOrg;
}

void JKKSDocument :: setOwnerOrg (qint64 id_owner_org)
{
    idOwnerOrg = id_owner_org;
}

const QString & JKKSDocument :: getOwnerOrgUID (void) const
{
    return ownerOrgUID;
}

void JKKSDocument :: setOwnerOrgUID (const QString & orgUID)
{
    ownerOrgUID = orgUID;
}

bool JKKSDocument :: getGlobal (void) const
{
    return isGlobal;
}

void JKKSDocument :: setGlobal (bool is_global)
{
    isGlobal = is_global;
}

const JKKSType & JKKSDocument :: getType(void) const
{
    return m_type;
}

void JKKSDocument :: setType(const JKKSType & type)
{
    m_type = type;
}
