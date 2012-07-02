/***********************************************************************
 * Module:  KKSLoader.h
 * Author:  sergey
 * Modified: 16 ������� 2008 �. 12:33:15
 * Purpose: Declaration of the class KKSLoader
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSLoader_h)
#define __KKSSITOOOM_KKSLoader_h

#include "kksfactory_config.h"

#include <QMap>
#include <QHash>
#include "KKSList.h"
#include "KKSMap.h"
#include "KKSAttribute.h"
#include "KKSAttrType.h"
#include "KKSValue.h"
//#include "KKSAttrGroup.h"
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
class KKSLifeCycle;
class KKSAttribute;
class KKSType;
class KKSSyncType;
class KKSState;
class KKSTemplate;
class KKSEIOData;
class KKSRubric;
class KKSPrivilege;
class KKSSearchTemplate;
class KKSAclTemplate;
class KKSAccessEntity;
class KKSAttrGroup;
class KKSAGroup;
class KKSIndicatorValue;
class KKSIndicatorType;
class KKSIndicator;

class _F_EXPORT KKSLoader
{
    public:
        //���������� ����� ��������� �������� �� �����������
        //��� ��������� ���� atList � atParent
        //������������ � ���� QMap, ��� ������ �������� 
        //������������� (���� id ) ���������������� �������� � ������� ���������������� �����������
        //���� ������ �������� ����� ��� TRUE - 
        //����� ����������� ���������� �������� �� �������� �� ����������� A->Z
        QMap<int, QString> loadAttributeValues(const KKSAttribute * a,
                                               QMap<int, QString> & refColumnValues, // = QMap< int, QString > ()
                                               bool isXml = false,
                                               bool orderByValue = true,
                                               QString tableName = QString::null,
                                               const KKSList<const KKSFilterGroup* > filters = KKSList<const KKSFilterGroup* > ()) const;//��������� �������� ���������� �������� 
                                                                                                                  //����, �� ������� � ���������������� ��������� �������
                                                                                                                  //������ ������, ����� �������� reference � ������� ���� 
                                                                                                                  //�� �� ���� id, � �� ��������� ������
                                                                                                                  //����� ��������, ���� ��������� �� ��������������� ��� DynamicDocs
                                                                                                                  //� ���� ������ ���� id � �������� ������ ����, � ��� ���������
                                                                                                                  //������� ��� ���������� �������� � ������ ��������� ������������
                                                                                                                  //�������� ��� ����������� ��������� refColumnName
   
        KKSList<KKSTemplate*> loadCategoryTemplates(int idCategory, bool bWithAllMandatories = false) const;
        KKSList<KKSTemplate*> loadCategoryTemplates(KKSCategory * c, bool bWithAllMandatories = false) const;
        KKSTemplate * loadTemplate(int id) const;

        KKSList<KKSStatElement *> loadIOStatistic(int id) const;
        
        //�������� simplify ���������� ����������� ��� �� ����������� ��� �������������� ��
        //���� ������� true, �� ����������� ������ ��������� �������� �� � ���������� � ���������.
        //� ������ � true ������������� ������������ ��� ���������� �����, 
        //� ��������� ��� ��������� �������� ��������� ������� � �.�.
        KKSCategory * loadCategory(int id, bool simplify = false) const;

        KKSObject * loadIO(int id, bool simplify = false) const;
        KKSObject * loadIO_id(int id, bool simplify = false) const;
        //����� ���������� �������������� ������, ������� ��������� ����������,
        //�������� ���� ������ � ������� tableName
        //���� ������� tableName �� ������������� �� ������ �����������
        //�� ������������ NULL
        KKSObject * loadIO(const QString & tableName, bool simplify = false) const;

        KKSObjectExemplar * loadEIO(qint64 id, 
									KKSObject * io, 
									const KKSCategory *c0=0, 
									const QString& table=QString(),
									bool simplify = true) const;
        //����� ���������� �������� ���� ��� ������� �� (� ������������ � ���������. 
        //������� ��������� ������ � �������, ���������� ���������� ��).
        //������������ KKSMap, ������� � �������� ����� �������� ������������� ���
        //� � �������� �������� - ������ KKSEIOData, ������� �������� KKSMap<QString, QString>
        //��� � �������� ����� ��������� ��� �������� (��. KKSAttribute::code() ), � � �������� �������� - �������� ��������
        KKSMap<qint64, KKSEIOData *> loadEIOList(const KKSObject * io, 
                                              const KKSList<const KKSFilterGroup *> filters = KKSList<const KKSFilterGroup*>()) const;

        KKSMap<qint64, KKSEIOData *> loadEIOList(const KKSCategory * c0,
                                              const QString& tableName,
                                              const KKSList<const KKSFilterGroup *> filters = KKSList<const KKSFilterGroup*>()) const;
        
        
        KKSList<KKSFileType *> loadFileTypes() const;
        KKSList<KKSFileExtention*> loadFileExtentions(int idFileType) const;
        KKSType * loadType(int id) const;
        KKSType * loadIOType(int id) const;
        KKSMap<int, KKSType *> loadAvailableTypes (void) const;
        KKSAttribute * loadAttribute(int id) const;
        KKSAttribute * loadAttribute(const QString & code, const QString & tableName) const;
        KKSAttrType * loadAttrType(int id) const;
        KKSAttrType * loadAttrType(KKSAttrType::KKSAttrTypes type) const;
        KKSState * loadState(int id) const;
        KKSSyncType * loadSyncType(int id) const;

        KKSMap<int, KKSAddTable *> loadIOTables (KKSObject * io) const;

        void setDb(KKSDatabase * db);
        KKSDatabase * getDb() const;

        //����� ���������� ������������� �������� ������������ ����
        //��� ������� � ������ ������ ���������� ������ ������������
        int getDlId() const;
        int getUserId(void) const;
        QString getDlName() const;
        QString getUserName() const;
        bool isLocalDl (int idDl) const;
        bool isPublicDl (int idDl) const;

        //������������ �������� ��������� � ���� KKSObjectExemplar
        KKSObjectExemplar * getMessage(int idMsg) const;

        //������������ ������������ � ���� KKSObjectExemplar
        KKSObjectExemplar * getCommand(int idCmd) const;

        int getLocalOrgId() const; //����� ���������� ������������� ��������� �����������
                                   //��� -1, ���� ��������� ����������� ��� �� �������
                                   //� ��������� ������ ������ ��������� ���������� �����������
                                   //������������� ������ ������� ������� ��� �����������
        int getOrgId (void) const;
        QString getOrgName (void) const;
        QByteArray getOrgLogo (void) const;
        QString getOrgModeName (void) const;

        //����� ���������� ������� ���� ����������� ������������
        QString getCurrMacLabelName() const;
        int getCurrMacLabelId() const;
        //����� ����������, ����� �� ������ ������ ���� ����������� �������� ���������, 
        //�������� � ������� ������ ������� (getCurrMacLabelName())
        bool canChangeMac(int idMaclabel) const;
        //����� ������ ������� ������� ������� ������������, 
        //���� ������������ ����� ����� �� ������ ��������
        int setCurrentMaclabel(int idMaclabel) const;
        
        //������ ���������� �������������� �����, 
        //������� ���������� ���������� � ������������� 
        //�������� ������������ ����
        int getMyBoss() const;
        int getMyUnit() const;
        QList<int> getBossList(int idUser) const;
        QList<int> getUnitList(int idUser) const;

        QMap<int, QString> getBossNamesList (int idUser) const;
        QMap<int, QString> getUnitNamesList (int idUser) const;

        QMap<int, QString> getDls (void) const;
        QMap<int, QString> getUrgencyLevels (void) const;

        bool getPrivilege(int idRole, int idObject, int whatPrivilege, bool withInherit) const;

        //����� ��������� �����������, ��������� �������� ������������.
        //���� �������� ����� ��� TRUE, �� ����������� ������ ���������� "��� ���������" ��� �������� ������������
        KKSRubric * loadRubricators(bool bOnlyMyDocs) const;
        KKSRubric * loadRubric (int idRubr) const;

        //����� ��������� �������� ���������, ������� ������ (������������) ���� �� ������ ��������������� �������
        //�.�. �� ��� ���� ������ � ������� attrs_values
        KKSMap<int, KKSAttribute*> loadIOUsedAttrs() const;

        KKSList<KKSSearchTemplate *> loadSearchTemplates (void) const;
        KKSSearchTemplate * loadSearchTemplate (int idSearchTemplate) const;

        QMap<int, QStringList> getSyncOrg (int idObject) const;

        KKSAclTemplate * loadAclTemplate (int idAclTemplate);

        KKSAGroup * loadAttrGroup (int idAGr) const;
        KKSMap<int, KKSAGroup *> loadAttrsGroups (void) const;
        KKSMap<int, KKSAGroup *> loadAvailAttrsGroups (void) const;
        
        KKSList<KKSAttrValue *> loadIOAttrValueHistory(const KKSAttrValue * av, bool forRecords = false) const;
        
        KKSIndicatorValue * loadIOIndicatorValue (KKSObject * io, int idIndicator) const;
        KKSIndicator * loadIndicator (int idIndicator) const;
        KKSMap<int, KKSIndicatorType *> loadIndicatorTypes (void) const;

        void loadAttrAttrs(KKSAttribute * a) const;
        KKSMap<int, KKSAttrAttr*> loadAttrAttrs(int idAttr) const;
        
        QList<int> getForbiddenTypes (void) const;

        int getRefIO (int idObjectE) const;

        KKSLoader();
        ~KKSLoader();
    protected:
    private:
        friend class KKSSito;
        friend class KKSSitoWeb;
        friend class KKSJMonitor;
        friend class KKSPPFactory;
      // KKSLoader();
       // ~KKSLoader();

        KKSDatabase * db;

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

        KKSList<KKSFile *> loadFiles(KKSObject * io) const;
		KKSList<KKSFile *> loadFiles(const KKSObjectExemplar * io) const;
        
		KKSLifeCycle * loadLifeCycle(int idCategory) const;
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
                                const KKSAttribute * a, 
                                const QString & parentTable = QString()) const;
        KKSValue constructValue(const QString & value, 
                                const KKSIndicator * i, 
                                const QString & parentTable = QString()) const;


        QString generateSelectEIOQuery(const KKSCategory * c0,
                                       const QString& tableName,
                                       const KKSList<const KKSFilterGroup *> & filters) const;
        
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
};

#endif
