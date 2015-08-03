/***********************************************************************
 * Module:  KKSLoader.h
 * Author:  sergey
 * Modified: 16 ������� 2008 �. 12:33:15
 * Purpose: Declaration of the class KKSLoader
 ***********************************************************************/

/*!\defgroup FACTORY_GROUP ���������� ���������� �������

�������� ������-�������, ������� �������� ��:
- �������� ������ �� ��, ������������ ��������� ������ ������ �� �������, ������� ������������� ���������� ������ � �� (������ KKSLoader, KKSFileLoader)
- ���������� (��������, ��������, �����������) ������ � �� (������ KKSPPFactory, KKSEIOFactory)
- ������������ ���������� ��������� ���������� ��������������� ��������� (������ KKSAttributesFactory, KKSObjEditorFactory)
- ������������ ���������� ��������� ��� ����������� ��������� ������ (����� KKSViewFactory)
- ������������ ���������� ��������� ��� �������������� ��������� � ��������� (������ KKSCatEditor, KKSTemplateEditorFactory)
- ������������ ���������� ��������� ��� �������������� ������ (����� KKSRubricFactory)
- ���������� ��������������� ��� ������������ ��������� � ������������ (KKSJMonitor)
*/

#if !defined(__KKSSITOOOM_KKSLoader_h)
#define __KKSSITOOOM_KKSLoader_h

#include "kksdatafactory_config.h"

#include <QMap>
#include <QHash>
#include "KKSList.h"
#include "KKSMap.h"
#include "KKSAttribute.h"
#include "KKSAttrType.h"
#include "KKSValue.h"
#include "KKSHistogram.h"
#include "KKSFilter.h"
#include "KKSFile.h"
#include "KKSStatElement.h"
#include "KKSAddTable.h"

class KKSObjectExemplar;
class KKSDatabase;
class KKSCategoryAttr;
class KKSAttrValue;
class KKSCategory;
class KKSObject;
class KKSLifeCycleEx;
class KKSAttribute;
class KKSType;
class KKSSyncType;
class KKSState;
class KKSTemplate;
class KKSEIOData;
class KKSRubricBase;
class KKSRubric;
class KKSRubricItem;
class KKSPrivilege;
class KKSSearchTemplate;
class KKSSearchTemplateType;
class KKSAclTemplate;
class KKSAccessEntity;
class KKSAttrGroup;
class KKSAGroup;
class KKSIndicatorValue;
class KKSIndicatorType;
class KKSIndicator;

//class KKSNotifyReceiver;

/*!\ingroup FACTORY_GROUP
\class KKSLoader
\brief ����� ��� �������� ������ �� ��
*/
class _F_DATA_EXPORT KKSLoader : public QObject
{
    Q_OBJECT
    signals:
        /*!\brief ������ ������������, ����� �� �� �������� ����������� ����������� � ���, ��� ��������� ��������� �������

        ������ ����� ������������� �� ��� �����������. ��������� ����������� ������������� � ���� ���������� ������ (QThread)
        */
        void databaseNotifyReceived(const QString & nName, const QString & tableName, const QString & idRecord);
    public:
        /*!\brief ���������� ����� ��������� �������� �� �����������
        ��� ��������� ���� atList � atParent � atSysChildCategoryRef
        
        
        ������ ������������ � ���� QMap, ��� ������ �������� 
        ������������� (���� id ) ���������������� �������� � ������� ���������������� �����������
        ���� ��������� �������� ����� ��� TRUE - 
        ����� ����������� ���������� �������� �� �������� �� ����������� A->Z.
        
        ��������� �������� ���������� �������� 
        ����, �� ������� � ���������������� ��������� �������
        ������ ������, ����� �������� reference � ������� ���� 
        �� �� ���� id, � �� ��������� ������.
        ����� ��������, ���� ��������� �� ��������������� ��� DynamicDocs
        � ���� ������ ���� id � �������� ������ ����, � ��� ���������
        ������� ��� ���������� �������� � ������ ��������� ������������
        �������� ��� ����������� ��������� refColumnName

        \param a �������
        \param refColumnValues  ���� ������� �������� ��������� ���� ��� ������� �� ��������� ��������
        \param isXml ������� �� � ����������� ����, ��� �������� ����� ��� XML
        \param orderByValue ������� ���������� �������� �� �������� �� ����������� A->Z
        \param tableName ���������������
        \param filters ������� (��������), ������� ������ ������������� ������, ����� ������� � ���������
        */
        QMap<int, QString> loadAttributeValues(const KKSAttribute * a,
                                               QMap<int, QString> & refColumnValues, // = QMap< int, QString > ()
                                               bool isXml = false,
                                               bool orderByValue = true,
                                               QString tableName = QString::null,
                                               const KKSList<const KKSFilterGroup* > filters = KKSList<const KKSFilterGroup* > ()) const;
   
        KKSList<KKSTemplate*> loadCategoryTemplates(int idCategory, bool bWithAllMandatories = false) const;
        KKSList<KKSTemplate*> loadCategoryTemplates(KKSCategory * c, bool bWithAllMandatories = false) const;
        KKSTemplate * loadTemplate(int id) const;

        KKSList<KKSStatElement *> loadIOStatistic(int id) const;
        
        /*!\brief �������� ��������� �� ��
        
        �������� simplify ���������� ����������� ��� �� ����������� ��� �������������� ���������
        ���� ������� true, �� ����������� ������ ��������� �������� ���������.
        � ������ � true ������������� ������������ ��� ���������� �����, 
        � ��������� ��� ��������� �������� ��������� ������� � �.�.

        \param id ������������� ���������
        \param simplify ��������� ������ ��������� ������
        */
        KKSCategory * loadCategory(int id, bool simplify = false) const;

        /*!\brief �������� ��������������� ������� �� ��
        
        �������� simplify ���������� ����������� ��� �� ����������� ��� �������������� ��������������� �������.
        ���� ������� true, �� ����������� ������ ��� ��������� �������� �� � ���������� � ���������.
        � ������ � true ������������� ������������ ��� ���������� �����, 
        � ��������� ��� ��������� �������� ��������� ������� � �.�.

        \param id ������������� ��������������� �������
        \param simplify ��������� ������ ��������� ������
        */
        KKSObject * loadIO(int id, bool simplify = false) const;
        //KKSObject * loadIO_id(int id, bool simplify = false) const;
        
        
        /*!\brief ����� ���������� �������������� ������, ������� ��������� ����������,
        �������� ���� ������ � ������� tableName.

        ���� ������� tableName �� ������������� �� ������ �����������, �� ������������ NULL
        �������� simplify ���������� ����������� ��� �� ����������� ��� �������������� ��������������� �������.
        ���� ������� true, �� ����������� ������ ��� ��������� �������� �� � ���������� � ���������.
        � ������ � true ������������� ������������ ��� ���������� �����, 
        � ��������� ��� ��������� �������� ��������� ������� � �.�.

        \param tableName �������� ������� �����������
        \param simplify ��������� ������ ��������� ������
        */
        KKSObject * loadIO(const QString & tableName, bool simplify = false) const;

        /*!\brief ����� ���������� ������ �����������
        
        \param id ������������� ������ ����������� � �������� �����������
        \param io �������������� ������ ���� ����������, �� �������� ����������� ������
        \param simplify ��������� ������ ��������� ������
        \param c0 ���������������
        \param table ���������������
        */
        KKSObjectExemplar * loadEIO(qint64 id, 
                                    const KKSObject * io, 
                                    bool simplify = true,
                                    const KKSCategory *c0=0, 
                                    const QString& table=QString()) const;
        
        /*!\brief ����� ���������� �������� ���� ��� ������� �� (� ������������ � ���������. 
        
        ������� ��������� ������ � �������, ���������� ���������� ��).
        ������������ KKSMap, ������� � �������� ����� �������� ������������� ���,
        � � �������� �������� - ������ KKSEIOData, ������� �������� KKSMap<QString, QString>
        ��� � �������� ����� ��������� ��� �������� (��. KKSAttribute::code() ), � � �������� �������� - �������� ��������

        \param io �������������� ������ ���� ����������, �� �������� ����������� ������
        \param filters �������

        \sa loadEIOList1
        */
        KKSMap<qint64, KKSEIOData *> loadEIOList(const KKSObject * io, 
                                              const KKSList<const KKSFilterGroup *>& filters = KKSList<const KKSFilterGroup*>()) const;

        KKSMap<qint64, KKSEIOData *> loadEIOList(const KKSCategory * c0,
                                              const QString& tableName,
                                              const KKSList<const KKSFilterGroup *>& filters = KKSList<const KKSFilterGroup*>(),
                                              bool isSys = false) const;//���� true, �� ����������� ������ �� ���������� �����������, � ������ � ��� ��� ����� uuid_t, id_state
        
        /*!\brief ����� ��������� ������ �������������� �������� (��� ������� ������������), �������� � ��������� �������

        \param r �������
        */
        KKSMap<qint64, KKSEIOData *> loadRecList (const KKSRubric * r) const;
        KKSMap<qint64, KKSEIOData *> loadRecList (QList<qint64> ids) const;
        
        KKSEIOData * loadEIOInfo (int idObject, qint64 idRec) const;
        
        /*!\brief ����� ���������� ������ ������� ���������� ����������� � ������������ � ���������
        
        ������ ������ ������������ � ���� QList, � �� QMap
        \param io �������������� ������ ���� ����������, �� �������� ����������� ������
        \param filters �������

        \sa loadEIOList
        */
        KKSList<KKSEIOData *> loadEIOList1(const KKSObject * io, 
                                           const KKSList<const KKSFilterGroup *>& filters = KKSList<const KKSFilterGroup*>()) const;

        KKSList<KKSEIOData *> loadEIOList1(const KKSCategory * c0,
                                           const QString& tableName,
                                           const KKSList<const KKSFilterGroup *>& filters = KKSList<const KKSFilterGroup*>(),
                                           bool isSys = false) const;//���� true, �� ����������� ������ �� ���������� �����������, � ������ � ��� ��� ����� uuid_t, id_state
        
        /*!\brief ��������� ��������� ���� ������, ������� ����� ���������� � �������������� �������� � ������� ������������
        */
        KKSList<KKSFileType *> loadFileTypes() const;
        /*!\brief ��������� ���������� ������, ��������������� � �������� ����� ������
        */
        KKSList<KKSFileExtention*> loadFileExtentions(int idFileType) const;
        /*!\brief ��������� ��� ���������
        */
        KKSType * loadType(int id) const;
        /*!\brief ��������� ��� ��������������� �������
        */
        KKSType * loadIOType(int id) const;
        /*!\brief ��������� ��������� ���� ���������
        */
        KKSMap<int, KKSType *> loadAvailableTypes (void) const;
        /*!\brief ����� ��������� �� �� ������� � ��������� ���������������
        */
        KKSAttribute * loadAttribute(int id) const;
        /*!\brief ����� ��������� �� �� ������� � ��������� �����, ������� �������� ������� �� ��������� ����������.

        ���� ��� �������� � �������� ������� ����������� �������� ����������
        \param code ��� �������� (�������� ������� � ������� �����������)
        \param tableName �������� ������� �����������, �� ������� ��������� �������
        */
        KKSAttribute * loadAttribute(const QString & code, const QString & tableName) const;
        /*!��������� ��� ��������

        \param id ������������� ���� ��������, ������� ���� ���������
        */
        KKSAttrType * loadAttrType(int id) const;
        KKSAttrType * loadAttrType(KKSAttrType::KKSAttrTypes type) const;
        KKSSyncType * loadSyncType(int id) const;

        /*!\brief ��������� ��������� ���� � �������� ���������������
        */
        KKSLifeCycleEx * loadLifeCycle(int idLifeCycle) const;
        /*!\brief ��������� ��������� � �������� ���������������
        */
        KKSState * loadState(int id) const;
        KKSList<KKSState * > loadStates() const;
        /*!\brief ��������� ���������, �������� � ��������� ��������� ����
        */
        KKSList<KKSState * > loadStates(int idLifeCycle) const;

        /*!\brief ��������� �������� �������������� ������ �����������
        */
        KKSMap<int, KKSAddTable *> loadIOTables (KKSObject * io) const;

        void setDb(KKSDatabase * db);
        KKSDatabase * getDb() const;

        /*!\brief ����� ���������� ������������� �������� ������������ ����,
        ��� ������� � ������ ������ ���������� ������ ������������
        */
        int getDlId() const;
        /*!\brief ������������ ������������� �������� ������������
        */
        int getUserId(void) const;
        /*!\brief ������������ ��� ������������ ����, ��� ������� ���������� ������ �������� ������������
        */
        QString getDlName() const;
        /*!\brief ������������ ��� �������� ������������
        */
        QString getUserName() const;
        bool isLocalDl (int idDl) const;
        bool isPublicDl (int idDl) const;

        /*!\brief ������������ �������� ��������� � ���� KKSObjectExemplar
        */
        KKSObjectExemplar * getMessage(int idMsg) const;

        /*!\brief ������������ ������������ � ���� KKSObjectExemplar
        */
        KKSObjectExemplar * getCommand(int idCmd) const;

        /*!\brief ����� ���������� ������������� ��������� �����������

        ��� -1, ���� ��������� ����������� ��� �� �������
        � ��������� ������ ������ ��������� ���������� �����������
        ������������� ������ ������� ������� ��� �����������
        */
        int getLocalOrgId() const; 
        int getOrgId (void) const;
        QString getOrgName (void) const;
        QByteArray getOrgLogo (void) const;
        QString getOrgModeName (void) const;

        /*!\brief ����� ���������� ������� ���� ����������� ������������
        */
        QString getCurrMacLabelName() const;
        int getCurrMacLabelId() const;
        /*!\brief ����� ����������, ����� �� ������ ������ ���� ����������� �������� ���������, 
        �������� � ������� ������ ������� (getCurrMacLabelName())
        */
        bool canChangeMac(int idMaclabel) const;
        /*!\brief ����� ������ ������� ������� ������� ������������, 
        ���� ������������ ����� ����� �� ������ ��������
        */
        int setCurrentMaclabel(int idMaclabel) const;
        
        /*!\brief ����� ���������� ������������� ����, 
        ������� ���������� ���������� �������� ������������ ����
        */
        int getMyBoss() const;
        /*!\brief ����� ���������� ������������� ����, 
        ������� ���������� ������������� �������� ������������ ����
        */
        int getMyUnit() const;
        QList<int> getBossList(int idUser) const;
        QList<int> getUnitList(int idUser) const;

        QMap<int, QString> getBossNamesList (int idUser) const;
        QMap<int, QString> getUnitNamesList (int idUser) const;

        QMap<int, QString> getDls (void) const;
        QMap<int, QString> getUrgencyLevels (void) const;

        bool getPrivilege(int idRole, int idObject, int whatPrivilege, bool withInherit) const;

        /*!\brief ����� ��������� �����������, ��������� �������� ������������.

        ���� �������� ����� ��� TRUE, �� ����������� ������ ���������� "��� ���������" ��� �������� ������������
        */
        KKSRubric * loadRubricators(bool bOnlyMyDocs) const;
        KKSRubricBase * loadCatRubricators(void) const;
        KKSRubric * loadRubric (int idRubr, bool withInherit = false) const;
        KKSRubric * loadRecRubric (qint64 idRubric, bool withInherit = false) const;

        /*!\brief ����� ��������� �������� ���������, ������� ������ (������������) ���� �� ������ ��������������� �������

        �.�. �� ��� ���� ������ � ������� attrs_values
        */
        KKSMap<int, KKSAttribute*> loadIOUsedAttrs() const;

        KKSList<KKSSearchTemplate *> loadSearchTemplates (void) const;
        /*!\brief ����� ���������� ������ ���������� ������� � �������� ���������������
        */
        KKSSearchTemplate * loadSearchTemplate (int idSearchTemplate) const;
        KKSMap<int, KKSSearchTemplateType *> loadSearchTemplateTypes() const;
        KKSMap<qint64, KKSSearchTemplate *> loadSearchTemplatesByType (KKSSearchTemplateType * stt) const;

        QMap<int, QStringList> getSyncOrg (int idObject) const;

        KKSAclTemplate * loadAclTemplate (int idAclTemplate);

        KKSAGroup * loadAttrGroup (int idAGr) const;
        KKSMap<int, KKSAGroup *> loadAttrsGroups (void) const;
        KKSMap<int, KKSAGroup *> loadAvailAttrsGroups (void) const;
        
        /*!\brief ����� ���������� ������� ��������� �������� ��������

        \param av �������
        \param forRecords ���� true, �� ���������, ��� ��������������� �������-���������� ������ �����������
        */
        KKSList<KKSAttrValue *> loadIOAttrValueHistory(const KKSAttrValue * av, bool forRecords = false) const;
        KKSAttrValue * loadIOAttrValue(const KKSAttrValue * av, int idVal, bool forRecords = false) const;
        
        KKSIndicatorValue * loadIOIndicatorValue (KKSObject * io, int idIndicator) const;
        KKSIndicator * loadIndicator (int idIndicator) const;
        KKSMap<int, KKSIndicatorType *> loadIndicatorTypes (void) const;

        /*!\brief ����� ��������� ��������, �������� � ������ ���������� ��������. 
        
        ���������� ����� loadAttribute(), ���� ������� ���������, �� ��������� ������ ���������, �������� � ����. ��� ������� � ����� ��������� ������.
        */
        void loadAttrAttrs(KKSAttribute * a) const;
        /*!\brief ����� ��������� � ���������� ��������, �������� � ������ ���������� ��������, ��������� ����� ���������������. 
        
        */
        KKSMap<int, KKSCategoryAttr*> loadAttrAttrs(int idAttr) const;
        /*!\brief ����� ��������� ��������� ��������� �������� � ������ �������� ���������� ��������

        \param av �������� ���������� ��������
        \param forRecords ���� true, �� ���������, ��� ��������������� �������-���������� ������ �����������
        */
        KKSMap<qint64, KKSAttrValue *> loadAttrAttrValues(KKSAttrValue * av, bool forRec = false) const;
        
        QList<int> getForbiddenTypes (void) const;

        int getRefIO (int idObjectE) const;
        
        qint64 getIdByUID (const QString& tableName, const QString& uid) const;
        /*!\brief ����� ���������� ������������� ��������������� ������� ���� ����������, � ������� ������ ������ � ��������� ���������� ���������������
        */
        qint64 getIdObjectByRecordUID(const QString & uid);
        
        bool isApplicable (KKSSearchTemplate * st, int idCategory) const;
        
        bool isIOCreated (const KKSCategory * cat) const;
        
        bool isIOSynced (const KKSCategory * cat) const;

        QMap<int, QString> getIOInfoForStreams() const;
        QMap<int, QString> getIOInfoForChains() const;
        QMap<int, QString> getCatInfoForStreams() const;
        QMap<int, QString> getCatInfoForChains() const;
        QMap<int, QString> getVariantsInfoForStreams() const;
        QMap<int, QString> getVariantsInfoForChains() const;
        QMap<int, QString> getScenariosInfoForStreams() const;
        QMap<int, QString> getScenariosInfoForChains() const;
        QMap<int, QString> getServicesInfoForChains() const;
        QMap<int, QString> getPartLowInfoForStreams() const;
        QString getHistogramValue(KKSHistogram & h, const QString & tName) const;

        KKSLoader();
        ~KKSLoader();
    protected:
    private:
        friend class KKSCoreApplication;
        friend class KKSJMonitor;
        friend class KKSPPFactory;
        friend class KKSViewFactory;
        friend class KKSCatEditorFactory;
        friend class KKSAttributesFactory;
      // KKSLoader();
       // ~KKSLoader();

        KKSDatabase * db;
        //KKSNotifyReceiver * m_notifyReceiver;//����� � ������� ���������� ��������� � ��������� notify

        mutable QHash<int, KKSObject *> loadedObjects;
        mutable QHash<int, KKSCategory *> loadedCategories;

        void clearCache();//������� ������ loadedObjects 

        void loadPrivileges(KKSObject * io) const;
        void loadPrivileges(KKSRubric * rubr) const;
        KKSMap<int, KKSCategoryAttr *> loadCategoryAttrs(int idCategory) const;
        KKSList<KKSAttrValue *> loadAttrValues(KKSObject * io) const;
        KKSList<KKSAttrValue *> loadIndValues(KKSObjectExemplar * eio) const;
        KKSList<KKSIndicatorValue *> loadIndicatorValues(KKSObject * io) const; //�� ������������!!!
        void loadUserTemplates(KKSObject * io) const;
        void loadRubrics(KKSObject * io) const;
        void loadRubrics(KKSCategory * c) const;
        void loadRecRubrics (KKSObjectExemplar * eio) const;
        QString getRecTable (int idRec) const;

        KKSList<KKSFile *> loadFiles(KKSObject * io) const;
        KKSList<KKSFile *> loadFiles(const KKSObjectExemplar * io) const;


        /*
        QString loadColumnValue(const QString & tName,
                                const QString & cName, 
                                qint64 id, 
                                const QString & parentTable = QString()) const;
        */
        QString loadColumnValue(const KKSIndAttr * a, 
                                qint64 id, 
                                const QString & parentTable = QString()) const;

        KKSMap<int, KKSAttrGroup *> loadTemplateAttrsGroups(int idTemplate) const;

        void addAttributesToGroup(KKSAGroup * ag) const;

        KKSValue constructValue(const QString & value, 
                                const QString & columnValue, //�������� �������, ������� ������ ������������ (��� ���������-������)
                                const KKSAttribute * a, 
                                const QString & parentTable = QString()) const;
        KKSValue constructValue(const QString & value,
                                const QString & columnValue, //�������� �������, ������� ������ ������������ (��� ���������-������)
                                const KKSIndicator * i, 
                                const QString & parentTable = QString()) const;


        QString generateSelectEIOQuery(const KKSCategory * c0,
                                       const QString& tableName,
                                       const KKSList<const KKSFilterGroup *> & filters,
                                       bool isSys) const;
        
        QString generateFilterSQL(const KKSList<const KKSFilterGroup *> & filters, 
                                  const QString & tableName, 
                                  QStringList & exTables) const;
        QString parseGroup(const KKSFilterGroup * g, 
                           const QString & tableName, 
                           QStringList & exTables) const; 
        QString parseFilter(const KKSFilter * f, 
                            const QString & tableName, 
                            QStringList & exTables) const;

        KKSFilterGroup * loadSearchGroup (int idGroup) const;
        KKSList<const KKSFilter *> loadCriteriaFromGroup (int idGroup) const;
        KKSFilter * loadCriterium (int idCriteria) const;
        KKSAccessEntity * loadAccessCat (KKSCategory * cat) const;
        KKSList<const KKSRubricItem *> loadCatRubricItems (const KKSCategory* cat) const;
};

#endif
