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
                      qint64 idCat = -1, 
                      qint64 idAuth = -1, 
                      const QString& tName = QString(), 
                      const QString& desc = QString(), 
                      const QString& info = QString(), 
                      qint64 idmaclabel = 1, 
                      qint64 idsynctype = -1,
                      const QString& sync_name = QString(),
                      const QString& sync_desc = QString(),
                      qint64 idownerorg = -1,
                      bool isglobal = false,
                      const QString & ownerOrgUID = QString(),

                      const JKKSAddress & addr = JKKSAddress(), 
                      const QString& mess_code = QString(),

                      const QString& uid = QString(),
                      const QString & uuid = QString(),
                      qint64 idState = 1, 
                      const QColor bkCol = QColor(),
                      const QColor fgCol = QColor(),
                      const QString & rIcon = QString());

        JKKSDocument (const JKKSDocument& io);
        virtual ~JKKSDocument (void);

        //
        // virtual functions definition
        //
        QByteArray serialize (void) const;
        int unserialize (const QByteArray& mess);
        
        const QMap<qint64, JKKSCategory>& getCategory (void) const;
        void setCategory (const QMap<qint64, JKKSCategory>& catMap);
        
        int writeToDB (const JKKSLoader * loader, const QString& senderUID, const QString& receiverUID);
        
        JKKSMessageType getMessageType (void) const;

        qint64 id (void) const;
        void setId (qint64 id);

        void setAttrValues (const QMap<QString, QString>& aVals);
        const QMap<QString, QString>& getAttrValues (void) const;

        QString getName (void) const;
        void setName (const QString& name);

        qint64 getIdIoCat (void) const;
        void setIdIoCat (qint64 id_io_category);

        qint64 getIdAuthor (void) const;
        void setIdAuthor (qint64 idAuth);

        QString getTableName (void) const;
        void setTableName (const QString& tName);

        QString getDescription (void) const;
        void setDescription (const QString& desc);

        QString getInfo (void) const;
        void setInfo (const QString& info);

        qint64 getIdMaclabel (void) const;
        void setIdMaclabel (qint64 idmaclabel);

        qint64 getCommandId (void) const;
        void setCommandId (qint64 idc);

        const QDateTime& getRealTime (void) const;
        void setRealTime (const QDateTime& rt);

        qint64 getJournal (void) const;
        void setJournal (qint64 idJ);

        const QMap<qint64, JKKSGlobalRubric>& rubrics (void) const;
        QMap<qint64, JKKSGlobalRubric>& rubrics (void);
        void setRubrics (const QMap<qint64, JKKSGlobalRubric>& rubrs);

        const QMap<qint64, JKKSIOUrl>& urls (void) const;
        QMap<qint64, JKKSIOUrl>& urls (void);
        void setUrls (const QMap<qint64, JKKSIOUrl>& urls);

        const QMap<qint64, JKKSIOTable>& ref_tables (void) const;
        QMap<qint64, JKKSIOTable>& ref_tables (void);
        void setTables (const QMap<qint64, JKKSIOTable>& ref_tables);

        qint64 getSyncType (void) const;
        void setSyncType (qint64 id_sync_type);

        const QString& getSyncName (void) const;
        void setSyncName (const QString& sync_name);

        const QString& getSyncDesc (void) const;
        void setSyncDesc (const QString& sync_desc);

        qint64 getOwnerOrg (void) const;
        void setOwnerOrg (qint64 id_owner_org);

        const QString & getOwnerOrgUID (void) const;
        void setOwnerOrgUID (const QString & orgUid);

        bool getGlobal (void) const;
        void setGlobal (bool is_global);

        //QColor getBkCol (void) const;
        //void setBkColor (QColor bkCol);

        //QColor getFgCol (void) const;
        //void setFgCol (QColor fgCol);

        const JKKSType & getType() const;
        void setType(const JKKSType & type);

    private:
        qint64 idObject;
        QString ioName;
        //QString ioCode;
        qint64 idCategory;
        qint64 idAuthor;
        //int idState;
        
        QString tableName;
        QString ioDesc;
        QString ioInfo;
        qint64 id_maclabel;

        QMap<qint64, JKKSCategory> category;
        QMap<QString, QString> attrValues;
        QMap<qint64, JKKSGlobalRubric> m_rubrics;
        QMap<qint64, JKKSIOUrl> m_urls;
        QMap<qint64, JKKSIOTable> m_tables;

        qint64 idCommand;
        QDateTime realTime;
        qint64 idJournal;

        qint64 idSyncType;
        QString syncName;
        QString syncDesc;
        qint64 idOwnerOrg;
        bool isGlobal;
        QString ownerOrgUID;

        //QColor bkColor;
        //QColor fgColor;

        JKKSType m_type;
};

#endif
