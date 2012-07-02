/***********************************************************************
 * Module:  KKSLoader.h
 * Author:  sergey
 * Modified: 16 декабря 2008 Ј. 12:33:15
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
        //возвращает набор возможных значений из справочника
        //для атрибутов типа atList и atParent
        //возвращаются в виде QMap, где ключом является 
        //идентификатор (поле id ) соответствующего значения в таблице соответствующего справочника
        //если второй параметр задан как TRUE - 
        //будет произведена сортировка значений по алфавиту по возрастанию A->Z
        QMap<int, QString> loadAttributeValues(const KKSAttribute * a,
                                               QMap<int, QString> & refColumnValues, // = QMap< int, QString > ()
                                               bool isXml = false,
                                               bool orderByValue = true,
                                               QString tableName = QString::null,
                                               const KKSList<const KKSFilterGroup* > filters = KKSList<const KKSFilterGroup* > ()) const;//последний параметр возврашает значения 
                                                                                                                  //поля, на которое в действительности ссылается атрибут
                                                                                                                  //Бывают случаи, когда реальный reference в таблице идет 
                                                                                                                  //не на поле id, а на некоторое другое
                                                                                                                  //Такое возможно, если структура БД разрабатывалась вне DynamicDocs
                                                                                                                  //В этом случае поле id в таблицах всегда есть, и оно уникально
                                                                                                                  //Наличие или отсутствие значения в данном параметре определяется
                                                                                                                  //наличием или отсутствием параметра refColumnName
   
        KKSList<KKSTemplate*> loadCategoryTemplates(int idCategory, bool bWithAllMandatories = false) const;
        KKSList<KKSTemplate*> loadCategoryTemplates(KKSCategory * c, bool bWithAllMandatories = false) const;
        KKSTemplate * loadTemplate(int id) const;

        KKSList<KKSStatElement *> loadIOStatistic(int id) const;
        
        //параметр simplify определяет загружаются или не загружаются все характеристики ИО
        //Если указано true, то загружаются только системные атрибуты ИО и информация о категории.
        //в случае с true целесообразно использовать для внутренних целей, 
        //в частности для ускорения загрузки различных списков и т.п.
        KKSCategory * loadCategory(int id, bool simplify = false) const;

        KKSObject * loadIO(int id, bool simplify = false) const;
        KKSObject * loadIO_id(int id, bool simplify = false) const;
        //метод возвращает информационный объект, который описывает справочник,
        //хранящий свои записи в таблице tableName
        //Если таблица tableName не соответствует ни одному справочнику
        //то возвращается NULL
        KKSObject * loadIO(const QString & tableName, bool simplify = false) const;

        KKSObjectExemplar * loadEIO(qint64 id, 
									KKSObject * io, 
									const KKSCategory *c0=0, 
									const QString& table=QString(),
									bool simplify = true) const;
        //метод производит загрузку всех ЭИО данного ИО (в соответствии с фильтрами. 
        //Фильтры применимы только к таблице, содержащей экземпляры ИО).
        //Возвращается KKSMap, который в качестве ключа содержит идентификатор ЭИО
        //а в качестве значения - объект KKSEIOData, который содержит KKSMap<QString, QString>
        //где в качестве ключа выступает код атрибута (см. KKSAttribute::code() ), а в качестве значения - значение атрибута
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

        //метод возвращает идентификатор текущего должностного лица
        //под которым в данный момент происходит работа пользователя
        int getDlId() const;
        int getUserId(void) const;
        QString getDlName() const;
        QString getUserName() const;
        bool isLocalDl (int idDl) const;
        bool isPublicDl (int idDl) const;

        //Возвращается почтовое сообщение в виде KKSObjectExemplar
        KKSObjectExemplar * getMessage(int idMsg) const;

        //Возвращается распоряжение в виде KKSObjectExemplar
        KKSObjectExemplar * getCommand(int idCmd) const;

        int getLocalOrgId() const; //метод возвращает идентификатор локальной организации
                                   //или -1, если локальная организация еще не создана
                                   //в последнем случае работа оператора становится невозможной
                                   //администратор должен сначала создать ОШС организации
        int getOrgId (void) const;
        QString getOrgName (void) const;
        QByteArray getOrgLogo (void) const;
        QString getOrgModeName (void) const;

        //метод возвращает текущий гриф секретности пользователя
        QString getCurrMacLabelName() const;
        int getCurrMacLabelId() const;
        //метод определяет, можно ли задать данный гриф секретности текущему документу, 
        //находясь в текущем уровне доступа (getCurrMacLabelName())
        bool canChangeMac(int idMaclabel) const;
        //метод меняет текущий уровень доступа пользователя, 
        //если пользователь имеет право на такуцю операцию
        int setCurrentMaclabel(int idMaclabel) const;
        
        //методы возвращают идентификаторы ролей, 
        //которые определяют начальника и подразделение 
        //текущего должностного лица
        int getMyBoss() const;
        int getMyUnit() const;
        QList<int> getBossList(int idUser) const;
        QList<int> getUnitList(int idUser) const;

        QMap<int, QString> getBossNamesList (int idUser) const;
        QMap<int, QString> getUnitNamesList (int idUser) const;

        QMap<int, QString> getDls (void) const;
        QMap<int, QString> getUrgencyLevels (void) const;

        bool getPrivilege(int idRole, int idObject, int whatPrivilege, bool withInherit) const;

        //метод загружает рубрикаторы, доступные текущему пользователю.
        //Если параметр задан как TRUE, то озвращается только рубрикатор "Мои документы" для текущего пользователя
        KKSRubric * loadRubricators(bool bOnlyMyDocs) const;
        KKSRubric * loadRubric (int idRubr) const;

        //метод загружает перечень атрибутов, которые заданы (используются) хотя бы одному информационному объекту
        //т.е. на них есть ссылки в таблице attrs_values
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

        void clearCache();//очищает список loadedObjects 

        void loadPrivileges(KKSObject * io) const;
        void loadPrivileges(KKSRubric * rubr) const;
        KKSMap<int, KKSCategoryAttr *> loadCategoryAttrs(int idCategory) const;
        KKSList<KKSAttrValue *> loadAttrValues(KKSObject * io) const;
        KKSList<KKSAttrValue *> loadIndValues(KKSObjectExemplar * eio) const;
        KKSList<KKSIndicatorValue *> loadIndicatorValues(KKSObject * io) const; //не используется!!!
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
