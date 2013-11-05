/******************************************************************************
 * Module: JKKSLoader
 * Author: saa & yuriyrusinov
 * Modified: 6 ������� 2009 ?. 18:09
 * Purpose: Declaration of class JKKSLoader
 ******************************************************************************/

#if !defined(__KKSSITOOOM_JKKSLoader_h)
#define __KKSSITOOOM_JKKSLoader_h

#include "kksinteractor_config.h"
#include <defines.h>

#include <QList>
//#include <KKSList.h>
#include <QPair>
#include <QMap>

#include "JKKSMessage.h"
#include "JKKSPing.h"

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
class JKKSFilePart;
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

//typedef QPair<qint64, qint64> idTableMap;

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
                    qint64 idTransport = 1);

        ~JKKSLoader (void);

        int setLocalAddress(const JKKSAddress & address) const;
        const JKKSAddress & getLocalAddress(bool reload = false) const;

        int getLocalOrgId() const;//������������� ��������� �����������

        //������������ ��� ��������� ������ ��� ��������, ������� ���������� ��������� �� �������� ����������
        //receivers - �������� ���������� ��������������� �����������-����������� ���������������
        QList<JKKSPMessWithAddr *> readMessages (QStringList & receivers) const;
        
        //��������, ������������ � ���������� � �� ��, ��� ������ �� �����������-�����������
        //������ ���� ����� ���������� � http_connector'�
        //� �� �������� ��������� ������ ��� ������� ����, ��� �� ��������� ��� ������
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
        int setAsSended (qint64 id, int idType, bool sended = true) const;
        //int writeMessage (JKKSDocument *document) const;
        int writeMessage (JKKSRefRecord *refRec, const QString& senderUID) const;
        int writeMessage (JKKSQueueResponse *response) const;
        int writeMessage (JKKSOrganization * org, const QString& receiverUID) const;
        int writeMessage (JKKSOrgPackage * OrgPack, const QString& senderUID, const QString& receiverUID) const;
        int writeMessage (JKKSFilePart * filePart, const QString& senderUID) const;
        int writeMessage (JKKSPing * ping, const QString & senderUID) const;

        QMap<QString, JKKSPing> createPings() const; //��������� ����� ��� �������� ���� �����������, ���������� � ������� organization
        QMap<QString, JKKSPing> createPings(const QStringList & receivers) const; //��������� ����� ��� �������� ��������� ����������� (�������� ������ email_prefix)
        JKKSPing createPing(const QString &  uidOrg) const;//��������� ����, ������� ����� ��������� � ������� ����������� JKKSPing::m_uidTo
        static QList<JKKSPMessWithAddr *> pingsToPMessWithAddr(const QMap<QString, JKKSPing> pings);

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
        QList<JKKSPMessWithAddr *> readCommands (QStringList & receivers) const;
        QList<JKKSPMessWithAddr *> readDocuments (QStringList & receivers) const;
        QList<JKKSPMessWithAddr *> readMails (QStringList & receivers) const;
        QList<JKKSPMessWithAddr *> readCmdConfirmations (QStringList & receivers) const;
        QList<JKKSPMessWithAddr *> readMailConfirmations (QStringList & receivers) const;
        QList<JKKSPMessWithAddr *> readTableRecords (QStringList & receivers) const;
        QList<JKKSPMessWithAddr *> readQueueResults (QStringList & receivers) const;
        QList<JKKSPMessWithAddr *> readPingResults (QStringList & receivers) const;

        int getDlId (void) const;
        int getUserId (void) const;
        QString getDlName (void) const;
        QString getUserName (void) const;

        qint64 getIdTransport() const;

        QString getReceiverEmailPrefix(qint64 id, qint64 type) const;//�������� email_prefix �����������-���������� ��� ��������� ��������� (� �������� �����)

        QList<JKKSFilePart*> readFileParts(QStringList & receivers) const;
        QByteArray readFilePartData(const QString & absUrl, qint64 blockSize, qint64 position, qint64 * readed) const;
        int writeFilePartData(JKKSFilePart * part) const;
        qint64 getFileDataSize(qint64 idUrl) const;


    private:
        //
        // Functions
        //
        QPair<qint64,JKKSCategory> readCategory(qint64) const;
        QMap<qint64, JKKSCategory> readCategories (qint64 idCat) const;
        QMap<qint64, JKKSCategory> readPCategories (qint64 idCatChild) const;
        JKKSDocument readDocument (qint64 idObject, qint64 idOrganization) const; //������ �������� ������������ � ������ ����� ��������� �������� ������������ ������ �������

        QMap<qint64, JKKSCategoryAttr> readCategoryAttrs (qint64 idCat) const;
        void writeCategoryAttrs (const JKKSCategory& cat) const;

        QMap<qint64, JKKSCategoryAttr> readAttrAttrs(qint64 idAttr) const;
        void writeAttrAttrs (const JKKSCategoryAttr& attr) const;

        JKKSCategoryAttr readAttribute (qint64 id) const;

        QMap<qint64, JKKSRubric> readCategoryRubrics (qint64 idCat) const;
        void writeCategoryRubrics (const JKKSCategory& cat) const;

        QMap<qint64, JKKSIOUrl> readDocumentFiles (qint64 idObject, qint64 idOrganization) const;//������ �������� ������������ � ������ ����� ��������� �������� ������������ ������ �������
        qint64 writeDocumentFile (JKKSIOUrl& url) const;
        qint64 writeRecordFile (JKKSIOUrl& url) const;

        QByteArray getFileData (qint64 idUrl, int blockSize=_MAX_FILE_BLOCK) const;
        int writeFileData (const JKKSIOUrl& url, int blockSize=_MAX_FILE_BLOCK) const;

        QMap<qint64, JKKSIOTable> readDocumentTables (qint64 idObject) const;
        qint64 writeAddTable (qint64 idObject, JKKSIOTable& table) const;
        JKKSIOTable readIOTable (QString entityuid, qint64& idObject) const;

        void generateQueueResponse (JKKSQueueResponse & resp) const;

        qint64 writeIOType (JKKSType& ioType) const;

        QMap<qint64, JKKSIOTable> dependencyTables (const JKKSRefRecord& RR) const;
        
        QPair<qint64, qint64> getIDMap (const QString& table_name, const JKKSRefRecord& RR) const;
        int writeRefRecordSysParams(JKKSRefRecord * rec) const;
        int writeRefRecordIndicators(JKKSRefRecord * rec) const;
        int writeRefRecordRubrics(JKKSRefRecord * rec) const;
        int writeRefRecordFiles(JKKSRefRecord * rec) const;

        int readRecordFromTable (const QString& tableName, JKKSRefRecord& rec, qint64 idOrganization) const;
        
        qint64 writeOrganization (JKKSQueueResponse & resp) const;
        QMap<qint64, JKKSOrganization> readOrganizations (qint64 idOrg) const;
        qint64 writeTransport (JKKSTransport& T) const;
        qint64 writeOrgType (JKKSOrgType& OT) const;
        qint64 writeOrgWM (JKKSWorkMode& wm) const;
        qint64 writeWMType (JKKSWorkModeType& wmt) const;
        QMap<qint64, JKKSWorkMode> readOrgWM (qint64 idOrg) const;
        JKKSWorkMode readWM (qint64 idWM) const;

        qint64 getIdByUID(const QString & tableName, const QString & uid) const; //����� ���������� id ������  �� unique_id � �������� �������
        QString getUIDSbyIDs (qint64 attrId, const QString& tableName, const QList<qint64>& ids) const;

        qint64 insertDocument(JKKSDocument * doc) const;
        qint64 updateDocument(JKKSDocument * doc) const;
        qint64 deleteDocument(JKKSDocument * doc) const;

        qint64 checkForExist(const QString & docUID) const;

        JKKSSearchTemplate readSearchTemplate (qint64 idSearchTemplate) const;
        void readGroups (qint64 idMainGroup, QMap<qint64, JKKSSearchGroup>& stGroups) const;
        QMap<qint64, JKKSSearchCriterion> readCriteriaForGroup (qint64 idGroup) const;

        qint64 writeSearchTemplate (const JKKSSearchTemplate& st) const;
        QMap<qint64, JKKSSearchGroup> writeGroups (qint64 idParentGr, const QMap<qint64, JKKSSearchGroup>& stGroups) const;
        qint64 writeCriteriaForGroup (JKKSSearchCriterion& sc) const;

        JKKSCategoryPair parseCategories(const QMap<qint64, JKKSCategory> & cats) const;
        QMap<qint64, JKKSCategory> pairToMap(const JKKSCategoryPair & pair) const;
        JKKSCategoryPair mapToPair(const QMap<qint64, JKKSCategory> & cats) const;

        JKKSOrgPackage readOrgs (qint64 id, const QString& receiverUID) const;
        QMap<qint64, JKKSTransport> readTransports (const QString& receiverUID) const;

        qint64 writeReceipt (JKKSQueueResponse& response) const;

        int readRefRecordIndicators(JKKSRefRecord & rec) const;
        int readRefRecordTableValues(JKKSRefRecord & rec, qint64 idObject) const;
        int readRefRecordRubrics(JKKSRefRecord & rec) const;
        int readRefRecordFiles(JKKSRefRecord & rec, qint64 idOrganization) const;
    private:
        //
        // Variables
        //
        QString dbHost;
        QString dbName;
        QString dbUser;
        QString dbPassword;
        int dbPort;

        //������������� �������� ����������
        qint64 m_idTransport;

        //
        // db1 ������������ ��� ������ � ����������
        // ����������� �������
        //
        KKSDatabase * dbRead;
        //
        // db2 ������������ ��� ������ � ���������
        // ����������� �������
        //
        KKSDatabase * dbWrite;
        mutable int idCurrentDl;//������� ����������� ����
        mutable int idCurrentUser;//������� ������������

        mutable JKKSAddress local_address;

        mutable int m_localOrgId;//������������� ��������� �����������

        //email_prefix �����������-�����������
        mutable QString senderUID;
        //email_prefix �����������-����������
        mutable QString receiverUID;


        mutable QList<JKKSPMessWithAddr *> queueResults;
        mutable QMap<QPair <QString, qint64>, qint64> records;
};

#endif
