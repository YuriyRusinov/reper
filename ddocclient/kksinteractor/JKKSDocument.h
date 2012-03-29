/***********************************************************************
 * Module:  JKKSDocument.h
 * Author:  yuriyrusinov
 * Modified: 13 апреля 2009 г. 11:50
 * Purpose: Declaration of the class JKKSDocument
 ***********************************************************************/

#if !defined(__KKSSITOOOM_JKKSDocument_h)
#define __KKSSITOOOM_JKKSDocument_h

#include "kksinteractor_config.h"

#include <QString>
#include <QMap>
#include <QDateTime>
#include <QColor>

#include "JKKSMessage.h"
#include "JKKSGlobalRubric.h"
#include "JKKSIOUrl.h"
#include "JKKSIOTable.h"
#include "jkksuid.h"
#include "JKKSType.h"

class JKKSCategory;
class JKKSLoader;
class KKSDatabase;

/*
 * Это класс документов для пересылки по Юпитеру
 */

class _I_EXPORT JKKSDocument : public JKKSMessage, public JKKSUID
{
    public:
        JKKSDocument (const QString& name = QString(), 
                      //const QString& code = QString(), 
                      int idCat = -1, 
                      int idAuth = -1, 
                      int idSt = -1, 
                      const QString& tName = QString(), 
                      const QString& desc = QString(), 
                      const QString& info = QString(), 
                      int idmaclabel = 1, 
                      const QString& addr = QString(), 
                      const QString& mess_code = QString(),
                      const QString& uid = QString(),
                      int idsynctype = -1,
                      const QString& sync_name = QString(),
                      const QString& sync_desc = QString(),
                      int idownerorg = -1,
                      bool isglobal = false,
                      const QString & ownerOrgUID = QString(),
                      QColor bkCol=QColor(),
                      QColor fgCol=QColor());

        JKKSDocument (const JKKSDocument& io);
        virtual ~JKKSDocument (void);

        //
        // virtual functions definition
        //
        QByteArray serialize (void) const;
        int unserialize (const QByteArray& mess);
        const QMap<int, JKKSCategory>& getCategory (void) const;
        void setCategory (const QMap<int, JKKSCategory>& catMap);
        int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
        int id (void) const;
        JKKSMessageType getMessageType (void) const;

        void setId (int id);

        void setAttrValues (const QMap<QString, QString>& aVals);
        const QMap<QString, QString>& getAttrValues (void) const;

        QString getName (void) const;
        void setName (const QString& name);

        //QString getIOCode (void) const;
        //void setIOCode (const QString& code);

        int getIdIoCat (void) const;
        void setIdIoCat (int id_io_category);

        int getIdAuthor (void) const;
        void setIdAuthor (int idAuth);

        int getIdState (void) const;
        void setIdState (int idSt);

        QString getTableName (void) const;
        void setTableName (const QString& tName);

        QString getDescription (void) const;
        void setDescription (const QString& desc);

        QString getInfo (void) const;
        void setInfo (const QString& info);

        int getIdMaclabel (void) const;
        void setIdMaclabel (int idmaclabel);

        int getCommandId (void) const;
        void setCommandId (int idc);

        const QDateTime& getRealTime (void) const;
        void setRealTime (const QDateTime& rt);

        int getJournal (void) const;
        void setJournal (int idJ);

        const QMap<int, JKKSGlobalRubric>& rubrics (void) const;
        QMap<int, JKKSGlobalRubric>& rubrics (void);
        void setRubrics (const QMap<int, JKKSGlobalRubric>& rubrs);

        const QMap<int, JKKSIOUrl>& urls (void) const;
        QMap<int, JKKSIOUrl>& urls (void);
        void setUrls (const QMap<int, JKKSIOUrl>& urls);

        const QMap<int, JKKSIOTable>& ref_tables (void) const;
        QMap<int, JKKSIOTable>& ref_tables (void);
        void setTables (const QMap<int, JKKSIOTable>& ref_tables);

        int getSyncType (void) const;
        void setSyncType (int id_sync_type);

        const QString& getSyncName (void) const;
        void setSyncName (const QString& sync_name);

        const QString& getSyncDesc (void) const;
        void setSyncDesc (const QString& sync_desc);

        int getOwnerOrg (void) const;
        void setOwnerOrg (int id_owner_org);

        const QString & getOwnerOrgUID (void) const;
        void setOwnerOrgUID (const QString & orgUid);

        bool getGlobal (void) const;
        void setGlobal (bool is_global);

        QColor getBkCol (void) const;
        void setBkColor (QColor bkCol);

        QColor getFgCol (void) const;
        void setFgCol (QColor fgCol);

        const JKKSType & getType() const;
        void setType(const JKKSType & type);

    private:
        int idObject;
        QString ioName;
        //QString ioCode;
        int idCategory;
        int idAuthor;
        int idState;
        
        QString tableName;
        QString ioDesc;
        QString ioInfo;
        int id_maclabel;

        QMap<int, JKKSCategory> category;
        QMap<QString, QString> attrValues;
        QMap<int, JKKSGlobalRubric> m_rubrics;
        QMap<int, JKKSIOUrl> m_urls;
        QMap<int, JKKSIOTable> m_tables;

        int idCommand;
        QDateTime realTime;
        int idJournal;

        int idSyncType;
        QString syncName;
        QString syncDesc;
        int idOwnerOrg;
        bool isGlobal;
        QString ownerOrgUID;

        QColor bkColor;
        QColor fgColor;

        JKKSType m_type;
};

#endif
