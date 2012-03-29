/******************************************************************************
 * Module: JKKSLoader
 * Author: saa & yuriyrusinov
 * Modified: 6 октября 2009 ?. 18:09
 * Purpose: Declaration of class JKKSLoader
 ******************************************************************************/

#if !defined(__KKSSITOOOM_JKKSLoader_h)
#define __KKSSITOOOM_JKKSLoader_h

#include "kksinteractor_config.h"

#include <QList>
#include <QPair>
#include <QMap>

class KKSDatabase;
class JKKSMessage;
class JKKSCategory;
class JKKSCategoryAttr;
class JKKSPMessWithAddr;
class JKKSPMessage;
class JKKSCommand;
class JKKSCmdConfirmation;
class JKKSDocument;
class JKKSMailMessage;
class JKKSMailConfirmation;
class JKKSRubric;
class JKKSGlobalRubric;
class JKKSIOUrl;
class JKKSIOTable;
class JKKSRefRecord;
class JKKSQueueResponse;
class JKKSOrganization;
class JKKSTransport;
class JKKSOrgType;
class JKKSType;
class JKKSWorkMode;
class JKKSWorkModeType;
class JKKSSearchTemplate;
class JKKSSearchGroup;
class JKKSSearchCriterion;
class JKKSCategoryPair;
class JKKSOrgPackage;

typedef QPair<int, int> idTableMap;

class _I_EXPORT JKKSLoader
{
    public:
        //
        // for Jupiter
        //
        JKKSLoader (const QString& host, 
                    const QString& name, 
                    const QString& user, 
                    const QString& pass, 
                    int p, 
                    int idTransport = 1);

        ~JKKSLoader (void);
        int setLocalAddress(const QString & address) const;
	const QString & getLocalAddress() const;

        QList<JKKSPMessWithAddr *> readMessages (void) const;
        int writeMessage(const JKKSPMessage & message) const;

        bool connectToDb (void);

//        void setDb1 (KKSDatabase * _db);
        KKSDatabase * getDbRead (void) const;
        KKSDatabase * getDbWrite (void) const;

        //
        // for messages
        //
        int writeMessage (JKKSCommand *command) const;
        int writeMessage (JKKSCmdConfirmation *cfm) const;
        int writeMessage (JKKSDocument *doc, int syncType=1) const;
        int writeMessage (JKKSMailMessage *mMess) const;
        int writeMessage (JKKSMailConfirmation *cfm) const;
        int setAsSended (int id, int idType) const;
        //int writeMessage (JKKSDocument *document) const;
        int writeMessage (JKKSRefRecord *refRec, const QString& senderUID) const;
        int writeMessage (JKKSQueueResponse *response) const;
        int writeMessage (JKKSOrganization * org, const QString& receiverUID) const;
        int writeMessage (JKKSOrgPackage * OrgPack, const QString& senderUID, const QString& receiverUID) const;
        //
        // Category has to be syncronized without other entities.
        //
        //QMap<int, JKKSCategory> writeCategories (const QMap<int, JKKSCategory> & C) const;
        int writeCategory (JKKSCategory& cat) const;

    public:
        //
        // TODO: for tests
        //
        JKKSMessage * unpackMessage (const JKKSPMessage & pMessage) const;
        QList<JKKSPMessWithAddr *> readCommands (void) const;
        QList<JKKSPMessWithAddr *> readDocuments (void) const;
        QList<JKKSPMessWithAddr *> readMails (void) const;
        QList<JKKSPMessWithAddr *> readCmdConfirmations (void) const;
        QList<JKKSPMessWithAddr *> readMailConfirmations (void) const;
        QList<JKKSPMessWithAddr *> readTableRecords (void) const;
        QList<JKKSPMessWithAddr *> readQueueResults (void) const;

        int getDlId (void) const;
        int getUserId (void) const;
        QString getDlName (void) const;
        QString getUserName (void) const;

        int getIdTransport() const;

        //QString getLocalAddr (void) const;
        //void setLocalAddr (const QString& localAddr);

    private:
        //
        // Functions
        //
        QPair<int,JKKSCategory> readCategory(int) const;
        QMap<int, JKKSCategory> readCategories (int idCat) const;
        QMap<int, JKKSCategory> readPCategories (int idCatChild) const;
        JKKSDocument readDocument (int idObject) const;

        QMap<int, JKKSCategoryAttr> readCategoryAttrs (int idCat) const;
        JKKSCategoryAttr readAttribute (int id) const;
        void writeCategoryAttrs (const JKKSCategory& cat) const;
        QMap<int, JKKSRubric> readCategoryRubrics (int idCat) const;
        void writeCategoryRubrics (const JKKSCategory& cat) const;

        QMap<int, JKKSIOUrl> readDocumentFiles (int idObject) const;
        int writeDocumentFile (JKKSIOUrl& url) const;

        QByteArray getFileData (int idUrl, int blockSize=100000) const;
        int writeFileData (const JKKSIOUrl& url, int blockSize=100000) const;

        QMap<int, JKKSIOTable> readDocumentTables (int idObject) const;
        int writeAddTable (int idObject, JKKSIOTable& table) const;
        JKKSIOTable readIOTable (QString entityuid, int& idObject) const;

        void generateQueueResponse (JKKSQueueResponse & resp) const;

        int writeIOType (JKKSType& ioType) const;

        int writeOrganization (JKKSQueueResponse & resp) const;
        QMap<int, JKKSIOTable> dependencyTables (const JKKSRefRecord& RR) const;
        QPair<int, int> getIDMap (const QString& table_name, const JKKSRefRecord& RR) const;
        int readRecordFromTable (const QString& tableName, JKKSRefRecord& rec) const;
        QMap<int, JKKSOrganization> readOrganizations (int idOrg) const;
        int writeTransport (JKKSTransport& T, const QString& locAddr) const;
        int writeOrgType (JKKSOrgType& OT) const;
        int writeOrgWM (JKKSWorkMode& wm) const;
        int writeWMType (JKKSWorkModeType& wmt) const;
        QMap<int, JKKSWorkMode> readOrgWM (int idOrg) const;
        JKKSWorkMode readWM (int idWM) const;

        int getIdByUID(const QString & tableName, const QString & uid) const; //метод возвращает id записи  по unique_id в заданной таблице
        QString getUIDSbyIDs (int attrId, const QString& tableName, const QList<int>& ids) const;

        int insertDocument(JKKSDocument * doc) const;
        int updateDocument(JKKSDocument * doc) const;
        int deleteDocument(JKKSDocument * doc) const;

        int checkForExist(const QString & docUID) const;

        JKKSSearchTemplate readSearchTemplate (int idSearchTemplate) const;
        void readGroups (int idMainGroup, QMap<int, JKKSSearchGroup>& stGroups) const;
        QMap<int, JKKSSearchCriterion> readCriteriaForGroup (int idGroup) const;

        int writeSearchTemplate (const JKKSSearchTemplate& st) const;
        QMap<int, JKKSSearchGroup> writeGroups (int idParentGr, const QMap<int, JKKSSearchGroup>& stGroups) const;
        int writeCriteriaForGroup (JKKSSearchCriterion& sc) const;

        JKKSCategoryPair parseCategories(const QMap<int, JKKSCategory> & cats) const;
        QMap<int, JKKSCategory> pairToMap(const JKKSCategoryPair & pair) const;

        JKKSOrgPackage readOrgs (int id, const QString& receiverUID) const;
        QMap<int, JKKSTransport> readTransports (const QString& receiverUID) const;

        int writeReceipt (JKKSQueueResponse& response) const;
    private:
        //
        // Variables
        //
        QString dbHost;
        QString dbName;
        QString dbUser;
        QString dbPassword;
        int dbPort;

        //идентификатор текущего транспорта
        int m_idTransport;

        //
        // db1 предназначен для работы с исходящими
        // сообщениями Юпитера
        //
        KKSDatabase * dbRead;
        //
        // db2 предназначен для работы с входящими
        // сообщениями Юпитера
        //
        KKSDatabase * dbWrite;
        mutable int idCurrentDl;//текущее должностное лицо
        mutable int idCurrentUser;//текущий пользователь

        mutable QString local_address;

        //email_prefix организации-отправителя
        mutable QString senderUID;
        //email_prefix организации-получателя
        mutable QString receiverUID;


        mutable QList<JKKSPMessWithAddr *> queueResults;
        mutable QMap<QPair <QString, int>, int> records;
};

#endif
