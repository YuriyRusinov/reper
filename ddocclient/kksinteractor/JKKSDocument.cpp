#include <QBuffer>
#include <QtDebug>
#include <QDataStream>

#include "JKKSCategory.h"
#include "JKKSDocument.h"
#include "JKKSLoader.h"
#include "defines.h"


JKKSDocument :: JKKSDocument (const QString& name, 
                              //const QString& code, 
                              int idCat, 
                              int idAuth, 
                              int idSt, 
                              const QString& tName, 
                              const QString& desc, 
                              const QString& info, 
                              int idmaclabel, 
                              const QString& addr, 
                              const QString& mess_code,
                              const QString& uid,
                              int idsynctype,
                              const QString& sync_name,
                              const QString& sync_desc,
                              int idownerorg,
                              bool isglobal,
                              const QString & orgUID,
                              QColor bkCol,
                              QColor fgCol
                              )
    : JKKSMessage (addr, mess_code), JKKSUID(uid),
    idObject (-1),
    ioName (name),
    //ioCode (code),
    idCategory (idCat),
    idAuthor (idAuth),
    idState (idSt),
    tableName (tName),
    ioDesc (desc),
    ioInfo (info),
    id_maclabel (idmaclabel),
    category (QMap<int, JKKSCategory>()),
    attrValues (QMap<QString, QString>()),
    m_rubrics (QMap<int, JKKSGlobalRubric>()),
    m_urls (QMap<int, JKKSIOUrl>()),
    idCommand (-1),
    realTime (QDateTime::currentDateTime()),
    idJournal (-1),
    idSyncType (idsynctype),
    syncName (sync_name),
    syncDesc (sync_desc),
    idOwnerOrg (idownerorg),
    isGlobal (isglobal),
    ownerOrgUID(orgUID),
    bkColor (bkCol),
    fgColor (fgCol)
{
}

JKKSDocument :: JKKSDocument (const JKKSDocument& io)
    : JKKSMessage (io), JKKSUID(io),
    idObject (io.idObject),
    ioName (io.ioName),
    //ioCode (io.ioCode),
    idCategory (io.idCategory),
    idAuthor (io.idAuthor),
    idState (io.idState),
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
    bkColor (io.bkColor),
    fgColor (io.fgColor),
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

    out << getAddr();
    out << getCode();
    out << ioName;
    //out << ioCode;
    out << idCategory;
    out << idAuthor;
    out << idState;
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
    out << uid();
    out << idSyncType;
    out << syncName;
    out << syncDesc;
    out << idOwnerOrg;
    out << isGlobal;
    out << ownerOrgUID;
    out << bkColor;
    out << fgColor;
    out << m_type;

    return qBuffer.buffer();
}

int JKKSDocument :: unserialize (const QByteArray& mess)
{
    QBuffer buffer;
    buffer.setData(mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);

    QString addr;
    QString code;
    in >> addr;
    in >> code;
    in >> ioName;
    //in >> ioCode;
    in >> idCategory;
    in >> idAuthor;
    in >> idState;
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
    QString uid;
    in >> uid;
    in >> idSyncType;
    in >> syncName;
    in >> syncDesc;
    in >> idOwnerOrg;
    in >> isGlobal;
    in >> ownerOrgUID;
    in >> bkColor;
    in >> fgColor;
    in >> m_type;


    setUid (uid);
    setAddr (addr);
    setCode (code);

    return OK_CODE;
}

const QMap<int, JKKSCategory>& JKKSDocument :: getCategory (void) const
{
    return category;
}

void JKKSDocument :: setCategory (const QMap<int, JKKSCategory>& catMap)
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

int JKKSDocument :: id (void) const
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

//QString JKKSDocument :: getIOCode (void) const
//{
//    return ioCode;
//}

//void JKKSDocument :: setIOCode (const QString& code)
//{
//    ioCode = code;
//}

int JKKSDocument :: getIdIoCat (void) const
{
    return idCategory;
}

void JKKSDocument :: setIdIoCat (int id_io_category)
{
    idCategory = id_io_category;
}

int JKKSDocument :: getIdAuthor (void) const
{
    return idAuthor;
}

void JKKSDocument :: setIdAuthor (int idAuth)
{
    idAuthor = idAuth;
}

int JKKSDocument :: getIdState (void) const
{
    return idState;
}

void JKKSDocument :: setIdState (int idSt)
{
    idState = idSt;
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

int JKKSDocument :: getIdMaclabel (void) const
{
    return id_maclabel;
}

void JKKSDocument :: setIdMaclabel (int idmaclabel)
{
    id_maclabel = idmaclabel;
}

void JKKSDocument :: setId (int id)
{
    idObject = id;
}

int JKKSDocument :: getCommandId (void) const
{
    return idCommand;
}

void JKKSDocument :: setCommandId (int idc)
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

int JKKSDocument :: getJournal (void) const
{
    return idJournal;
}

void JKKSDocument :: setJournal (int idJ)
{
    idJournal = idJ;
}

const QMap<int, JKKSGlobalRubric>& JKKSDocument :: rubrics (void) const
{
    return m_rubrics;
}

QMap<int, JKKSGlobalRubric>& JKKSDocument :: rubrics (void)
{
    return m_rubrics;
}

void JKKSDocument :: setRubrics (const QMap<int, JKKSGlobalRubric>& rubrs)
{
    m_rubrics = rubrs;
}

const QMap<int, JKKSIOUrl>& JKKSDocument :: urls (void) const
{
    return m_urls;
}

QMap<int, JKKSIOUrl>& JKKSDocument :: urls (void)
{
    return m_urls;
}

void JKKSDocument :: setUrls (const QMap<int, JKKSIOUrl>& urls)
{
    m_urls = urls;
}

const QMap<int, JKKSIOTable>& JKKSDocument :: ref_tables (void) const
{
    return m_tables;
}

QMap<int, JKKSIOTable>& JKKSDocument :: ref_tables (void)
{
    return m_tables;
}

void JKKSDocument :: setTables (const QMap<int, JKKSIOTable>& ref_tables)
{
    m_tables = ref_tables;
}

int JKKSDocument :: getSyncType (void) const
{
    return idSyncType;
}

void JKKSDocument :: setSyncType (int id_sync_type)
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

int JKKSDocument :: getOwnerOrg (void) const
{
    return idOwnerOrg;
}

void JKKSDocument :: setOwnerOrg (int id_owner_org)
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

QColor JKKSDocument :: getBkCol (void) const
{
    return bkColor;
}

void JKKSDocument :: setBkColor (QColor bkCol)
{
    bkColor = bkCol;
}

QColor JKKSDocument :: getFgCol (void) const
{
    return fgColor;
}

void JKKSDocument :: setFgCol (QColor fgCol)
{
    fgColor = fgCol;
}

const JKKSType & JKKSDocument :: getType(void) const
{
    return m_type;
}

void JKKSDocument :: setType(const JKKSType & type)
{
    m_type = type;
}
