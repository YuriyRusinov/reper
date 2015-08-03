/***********************************************************************
 * Module:  KKSLoader.h
 * Author:  sergey
 * Modified: 16 декабря 2008 Ј. 12:33:15
 * Purpose: Declaration of the class KKSLoader
 ***********************************************************************/

/*!\defgroup FACTORY_GROUP Подсистема управления данными

Содержит классы-фабрики, которые отвечают за:
- загрузку данных из ДЗ, формирование объектной модели данных на клиенте, которая соответствует структурам данных в БД (классы KKSLoader, KKSFileLoader)
- сохранение (создание, удаление, модификацию) данных в БД (классы KKSPPFactory, KKSEIOFactory)
- формирование визуальных элементов управления информационными ресурсами (классы KKSAttributesFactory, KKSObjEditorFactory)
- формирование визуальных элементов для отображения табличных данных (класс KKSViewFactory)
- формирование визуальных элементов для редактирования атрибутов и категорий (классы KKSCatEditor, KKSTemplateEditorFactory)
- формирование визуальных элементов для редактирования рубрик (класс KKSRubricFactory)
- мониторинг предназначенных для пользователя сообщений и распоряжений (KKSJMonitor)
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
\brief Класс для загрузки данных из БД
*/
class _F_DATA_EXPORT KKSLoader : public QObject
{
    Q_OBJECT
    signals:
        /*!\brief Сигнал генерируется, когда от БД получено асинхронное уведомление о том, что произолшо некоторое событие

        Модули могут подписываться на это уведомление. Получение уведомлений организровано в виде отдельного потока (QThread)
        */
        void databaseNotifyReceived(const QString & nName, const QString & tableName, const QString & idRecord);
    public:
        /*!\brief Возвращает набор возможных значений из справочника
        для атрибутов типа atList и atParent и atSysChildCategoryRef
        
        
        Данные возвращаются в виде QMap, где ключом является 
        идентификатор (поле id ) соответствующего значения в таблице соответствующего справочника
        если четвертый параметр задан как TRUE - 
        будет произведена сортировка значений по алфавиту по возрастанию A->Z.
        
        Последний параметр возврашает значения 
        поля, на которое в действительности ссылается атрибут
        Бывают случаи, когда реальный reference в таблице идет 
        не на поле id, а на некоторое другое.
        Такое возможно, если структура БД разрабатывалась вне DynamicDocs
        В этом случае поле id в таблицах всегда есть, и оно уникально
        Наличие или отсутствие значения в данном параметре определяется
        наличием или отсутствием параметра refColumnName

        \param a атрибут
        \param refColumnValues  сюда попадут значения ключевого поля для каждого из возможных значений
        \param isXml имеется ли в справочнике поле, для которого задан тип XML
        \param orderByValue признак сортировки значений по алфавиту по возрастанию A->Z
        \param tableName зарезервировано
        \param filters фильтры (критерии), которым должны удовлетворять данные, чтобы попасть в результат
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
        
        /*!\brief Загрузка категории из БД
        
        Параметр simplify определяет загружаются или не загружаются все характеристики категории
        Если указано true, то загружаются только системные атрибуты категории.
        в случае с true целесообразно использовать для внутренних целей, 
        в частности для ускорения загрузки различных списков и т.п.

        \param id идентификатор категории
        \param simplify загрузить только системные данные
        */
        KKSCategory * loadCategory(int id, bool simplify = false) const;

        /*!\brief Загрузка информационного ресурса из БД
        
        Параметр simplify определяет загружаются или не загружаются все характеристики информационного ресурса.
        Если указано true, то загружаются только его системные атрибуты ИО и информация о категории.
        в случае с true целесообразно использовать для внутренних целей, 
        в частности для ускорения загрузки различных списков и т.п.

        \param id идентификатор информационного ресурса
        \param simplify загрузить только системные данные
        */
        KKSObject * loadIO(int id, bool simplify = false) const;
        //KKSObject * loadIO_id(int id, bool simplify = false) const;
        
        
        /*!\brief Метод возвращает информационный объект, который описывает справочник,
        хранящий свои записи в таблице tableName.

        Если таблица tableName не соответствует ни одному справочнику, то возвращается NULL
        Параметр simplify определяет загружаются или не загружаются все характеристики информационного ресурса.
        Если указано true, то загружаются только его системные атрибуты ИО и информация о категории.
        в случае с true целесообразно использовать для внутренних целей, 
        в частности для ускорения загрузки различных списков и т.п.

        \param tableName название таблицы справочника
        \param simplify загрузить только системные данные
        */
        KKSObject * loadIO(const QString & tableName, bool simplify = false) const;

        /*!\brief Метод возвращает запись справочника
        
        \param id идентификатор записи справочника в заданном справочнике
        \param io информационный ресурс типа справочник, из которого загружается запись
        \param simplify загрузить только системные данные
        \param c0 зарезервировано
        \param table зарезервировано
        */
        KKSObjectExemplar * loadEIO(qint64 id, 
                                    const KKSObject * io, 
                                    bool simplify = true,
                                    const KKSCategory *c0=0, 
                                    const QString& table=QString()) const;
        
        /*!\brief Метод производит загрузку всех ЭИО данного ИО (в соответствии с фильтрами. 
        
        Фильтры применимы только к таблице, содержащей экземпляры ИО).
        Возвращается KKSMap, который в качестве ключа содержит идентификатор ЭИО,
        а в качестве значения - объект KKSEIOData, который содержит KKSMap<QString, QString>
        где в качестве ключа выступает код атрибута (см. KKSAttribute::code() ), а в качестве значения - значение атрибута

        \param io информационный ресурс типа справочник, из которого загружаются записи
        \param filters фильтры

        \sa loadEIOList1
        */
        KKSMap<qint64, KKSEIOData *> loadEIOList(const KKSObject * io, 
                                              const KKSList<const KKSFilterGroup *>& filters = KKSList<const KKSFilterGroup*>()) const;

        KKSMap<qint64, KKSEIOData *> loadEIOList(const KKSCategory * c0,
                                              const QString& tableName,
                                              const KKSList<const KKSFilterGroup *>& filters = KKSList<const KKSFilterGroup*>(),
                                              bool isSys = false) const;//если true, то загружаются данные из системного справочника, а значит у них нет полей uuid_t, id_state
        
        /*!\brief Метод загружает список информационных ресурсов (или записей справочников), входящих в указанную рубрику

        \param r рубрика
        */
        KKSMap<qint64, KKSEIOData *> loadRecList (const KKSRubric * r) const;
        KKSMap<qint64, KKSEIOData *> loadRecList (QList<qint64> ids) const;
        
        KKSEIOData * loadEIOInfo (int idObject, qint64 idRec) const;
        
        /*!\brief Метод возвращает список записей указанного справочника в соответствии с фильтрами
        
        Просто данные возвращаются в виде QList, а не QMap
        \param io информационный ресурс типа справочник, из которого загружаются записи
        \param filters фильтры

        \sa loadEIOList
        */
        KKSList<KKSEIOData *> loadEIOList1(const KKSObject * io, 
                                           const KKSList<const KKSFilterGroup *>& filters = KKSList<const KKSFilterGroup*>()) const;

        KKSList<KKSEIOData *> loadEIOList1(const KKSCategory * c0,
                                           const QString& tableName,
                                           const KKSList<const KKSFilterGroup *>& filters = KKSList<const KKSFilterGroup*>(),
                                           bool isSys = false) const;//если true, то загружаются данные из системного справлчника, а значит у них нет полей uuid_t, id_state
        
        /*!\brief Загрузить доступные типы файлов, которые можно прикрепить к информационным ресурсам и записям справочников
        */
        KKSList<KKSFileType *> loadFileTypes() const;
        /*!\brief Загрузить расширения файлов, ассоциированные с заданным типом файлов
        */
        KKSList<KKSFileExtention*> loadFileExtentions(int idFileType) const;
        /*!\brief Загрузить тип категории
        */
        KKSType * loadType(int id) const;
        /*!\brief Загрузить тип информационного ресурса
        */
        KKSType * loadIOType(int id) const;
        /*!\brief Загрузить доступные типы категорий
        */
        KKSMap<int, KKSType *> loadAvailableTypes (void) const;
        /*!\brief Метод загружает из БД атрибут с указанным идентификатором
        */
        KKSAttribute * loadAttribute(int id) const;
        /*!\brief Метод загружает из БД атрибут с указанным кодом, который является ссылкой на указанный справочник.

        Пара код атрибута и название таблицы справочника является уникальной
        \param code код атрибута (название колонки в таблице справочника)
        \param tableName название таблицы справочника, на который ссылается атрибут
        */
        KKSAttribute * loadAttribute(const QString & code, const QString & tableName) const;
        /*!Загрузить тип атрибута

        \param id идентификатор типа атрибута, который надо загрузить
        */
        KKSAttrType * loadAttrType(int id) const;
        KKSAttrType * loadAttrType(KKSAttrType::KKSAttrTypes type) const;
        KKSSyncType * loadSyncType(int id) const;

        /*!\brief Загрузить жизненный цикл с заданным идентификатором
        */
        KKSLifeCycleEx * loadLifeCycle(int idLifeCycle) const;
        /*!\brief Загрузить состояние с заданным идентификатором
        */
        KKSState * loadState(int id) const;
        KKSList<KKSState * > loadStates() const;
        /*!\brief Загрузить состояния, входящие в указанный жизненный цикл
        */
        KKSList<KKSState * > loadStates(int idLifeCycle) const;

        /*!\brief Загрузить описания дополнительных таблиц справочника
        */
        KKSMap<int, KKSAddTable *> loadIOTables (KKSObject * io) const;

        void setDb(KKSDatabase * db);
        KKSDatabase * getDb() const;

        /*!\brief Метод возвращает идентификатор текущего должностного лица,
        под которым в данный момент происходит работа пользователя
        */
        int getDlId() const;
        /*!\brief Возвращается идентификатор текущего пользователя
        */
        int getUserId(void) const;
        /*!\brief Возвращается имя должностного лица, под которым происходит работа текущего пользователя
        */
        QString getDlName() const;
        /*!\brief Возвращается имя текущего пользователя
        */
        QString getUserName() const;
        bool isLocalDl (int idDl) const;
        bool isPublicDl (int idDl) const;

        /*!\brief Возвращается почтовое сообщение в виде KKSObjectExemplar
        */
        KKSObjectExemplar * getMessage(int idMsg) const;

        /*!\brief Возвращается распоряжение в виде KKSObjectExemplar
        */
        KKSObjectExemplar * getCommand(int idCmd) const;

        /*!\brief Метод возвращает идентификатор локальной организации

        или -1, если локальная организация еще не создана
        в последнем случае работа оператора становится невозможной
        администратор должен сначала создать ОШС организации
        */
        int getLocalOrgId() const; 
        int getOrgId (void) const;
        QString getOrgName (void) const;
        QByteArray getOrgLogo (void) const;
        QString getOrgModeName (void) const;

        /*!\brief метод возвращает текущий гриф секретности пользователя
        */
        QString getCurrMacLabelName() const;
        int getCurrMacLabelId() const;
        /*!\brief метод определяет, можно ли задать данный гриф секретности текущему документу, 
        находясь в текущем уровне доступа (getCurrMacLabelName())
        */
        bool canChangeMac(int idMaclabel) const;
        /*!\brief метод меняет текущий уровень доступа пользователя, 
        если пользователь имеет право на такуцю операцию
        */
        int setCurrentMaclabel(int idMaclabel) const;
        
        /*!\brief метод возвращает идентификатор роли, 
        которая определяет начальника текущего должностного лица
        */
        int getMyBoss() const;
        /*!\brief метод возвращает идентификатор роли, 
        которая определяет подразделение текущего должностного лица
        */
        int getMyUnit() const;
        QList<int> getBossList(int idUser) const;
        QList<int> getUnitList(int idUser) const;

        QMap<int, QString> getBossNamesList (int idUser) const;
        QMap<int, QString> getUnitNamesList (int idUser) const;

        QMap<int, QString> getDls (void) const;
        QMap<int, QString> getUrgencyLevels (void) const;

        bool getPrivilege(int idRole, int idObject, int whatPrivilege, bool withInherit) const;

        /*!\brief метод загружает рубрикаторы, доступные текущему пользователю.

        Если параметр задан как TRUE, то озвращается только рубрикатор "Мои документы" для текущего пользователя
        */
        KKSRubric * loadRubricators(bool bOnlyMyDocs) const;
        KKSRubricBase * loadCatRubricators(void) const;
        KKSRubric * loadRubric (int idRubr, bool withInherit = false) const;
        KKSRubric * loadRecRubric (qint64 idRubric, bool withInherit = false) const;

        /*!\brief Метод загружает перечень атрибутов, которые заданы (используются) хотя бы одному информационному объекту

        т.е. на них есть ссылки в таблице attrs_values
        */
        KKSMap<int, KKSAttribute*> loadIOUsedAttrs() const;

        KKSList<KKSSearchTemplate *> loadSearchTemplates (void) const;
        /*!\brief Метод возвращает шаблон поискового запроса с заданным идентификатором
        */
        KKSSearchTemplate * loadSearchTemplate (int idSearchTemplate) const;
        KKSMap<int, KKSSearchTemplateType *> loadSearchTemplateTypes() const;
        KKSMap<qint64, KKSSearchTemplate *> loadSearchTemplatesByType (KKSSearchTemplateType * stt) const;

        QMap<int, QStringList> getSyncOrg (int idObject) const;

        KKSAclTemplate * loadAclTemplate (int idAclTemplate);

        KKSAGroup * loadAttrGroup (int idAGr) const;
        KKSMap<int, KKSAGroup *> loadAttrsGroups (void) const;
        KKSMap<int, KKSAGroup *> loadAvailAttrsGroups (void) const;
        
        /*!\brief Метод возвращает историю изменения значений атрибута

        \param av атрибут
        \param forRecords если true, то считается, что рассматривается атрибут-показатель записи справочника
        */
        KKSList<KKSAttrValue *> loadIOAttrValueHistory(const KKSAttrValue * av, bool forRecords = false) const;
        KKSAttrValue * loadIOAttrValue(const KKSAttrValue * av, int idVal, bool forRecords = false) const;
        
        KKSIndicatorValue * loadIOIndicatorValue (KKSObject * io, int idIndicator) const;
        KKSIndicator * loadIndicator (int idIndicator) const;
        KKSMap<int, KKSIndicatorType *> loadIndicatorTypes (void) const;

        /*!\brief Метод загружает атрибуты, входящие в состав составного атрибута. 
        
        Изначально метод loadAttribute(), если атрибут составной, не загружает список атрибутов, входящих в него. Это сделано с целью ускорения работы.
        */
        void loadAttrAttrs(KKSAttribute * a) const;
        /*!\brief Метод загружает и возвращает атрибуты, входящие в состав составного атрибута, заданного своим идентификатором. 
        
        */
        KKSMap<int, KKSCategoryAttr*> loadAttrAttrs(int idAttr) const;
        /*!\brief Метод загружает знаечения атрибутов входящие в состав значения составного атрибута

        \param av значение составного атрибута
        \param forRecords если true, то считается, что рассматривается атрибут-показатель записи справочника
        */
        KKSMap<qint64, KKSAttrValue *> loadAttrAttrValues(KKSAttrValue * av, bool forRec = false) const;
        
        QList<int> getForbiddenTypes (void) const;

        int getRefIO (int idObjectE) const;
        
        qint64 getIdByUID (const QString& tableName, const QString& uid) const;
        /*!\brief Метод возвращает идентификатор информационного ресурса типа справочник, в который входит запись с указанным уникальным идентификатором
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
        //KKSNotifyReceiver * m_notifyReceiver;//поток в котором происходит получение и обработка notify

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
                                const QString & columnValue, //значение колонки, которая должна отображаться (для атрибутов-ссылок)
                                const KKSAttribute * a, 
                                const QString & parentTable = QString()) const;
        KKSValue constructValue(const QString & value,
                                const QString & columnValue, //значение колонки, которая должна отображаться (для атрибутов-ссылок)
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
