/***********************************************************************
 * Module:  KKSPPFactory.h
 * Author:  sergey
 * Modified: 2 декабря 2008 г. 13:22:57
 * Purpose: Declaration of the class KKSPPFactory
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSPPFactory_h)
#define __KKSSITOOOM_KKSPPFactory_h

#include <QString>
#include "kksfactory_config.h"
#include <QMap>
#include <QStack>
#include "KKSList.h"
#include "KKSMap.h"
#include "KKSAttrType.h"
#include "KKSValue.h"

class KKSObjectExemplar;
class KKSDatabase;
class KKSCategoryAttr;
class KKSAttrValue;
class KKSCategory;
class KKSObject;
class KKSLifeCycle;
class KKSAttribute;
class KKSAttrAttr;
class KKSType;
class KKSState;
class KKSTemplate;
class KKSLoader;
class KKSFileLoader;
class KKSEIOFactory;
class KKSAttrView;
class KKSAttrGroup;
class KKSFile;
class KKSRubric;
class KKSFilterGroup;
class KKSFilter;
class KKSSearchTemplate;
class KKSSearchTemplateType;
class KKSAclTemplate;
class KKSPrivilege;
class KKSAccessEntity;

class _F_EXPORT KKSPPFactory
{
    public:
        int insertIO(KKSObject * io, bool useDefaultTable = true, QWidget * parent = NULL);
        int updateIO(KKSObject * io, QWidget * parent = NULL);
        int deleteIO(KKSObject * io) const;
        int insertAddTable (KKSObject * io, 
                            const QString& tableName, 
                            KKSCategory * c, 
                            const QString& tname, 
                            int idSearchTemplate, 
                            const QString & refTableName) const;
        int setAsExecutedCommand(int idObject, int idJournal, bool draft = false) const;
        int setAsCommandResult(int idObject, int idJournal) const;

        int insertCategory(KKSCategory* c) const;
        int insertType(KKSType * t) const;
        int insertIOType(KKSType * t) const;
        int insertState(KKSState * s) const;
        int insertCategoryAttr(int idCategory, KKSCategoryAttr * a) const;
        int insertLifeCycle(int idCategory, KKSLifeCycle * lc) const;

        int insertAttrAttr(int idParentAttr, KKSAttrAttr * aa) const;
        int insertAttrAttrs(const KKSAttribute * a) const;
    
        int updateCategory(const KKSCategory* c) const;
        int updateCategoryAttr(int idCategory, KKSCategoryAttr * a) const;
        int deleteCategory(KKSCategory* c) const;

        int insertTemplate(KKSTemplate * t) const;
        int updateTemplate(const KKSTemplate * t) const;
        int deleteTemplate(KKSTemplate * t) const;
        int deleteTemplate(int idTemplate) const;

        int insertAclTemplate (KKSAccessEntity * at) const;
        int updateAclTemplate (KKSAccessEntity * at) const;
        int deleteAclTemplate (KKSAccessEntity * at) const;
        int deleteAclTemplate (int idAclTemplate) const;

        void setParams(KKSLoader * _loader, 
                       KKSFileLoader * _fileLoader, 
                       KKSEIOFactory * _eiof, 
                       KKSDatabase * db);
        KKSDatabase * getDb() const;

        //метод создает атрибут по заданным параметрам
        //при этом проверяется наличие атрибута с аналогичным ИД в БД
        //если таковой существует, то он загружается, и возвращается именно он.
        //если атрибут был не создан а именно загружен, то параметр loaded устанавливается в TRUE
        //в противном случае в FALSE
        //если атрибут не загружен и при этом создать его не удалось 
        //(указан неверный тип), то возвращается NULL
        KKSAttribute * createAttribute(int id, 
                                   const QString & code, 
                                   const QString & name, 
                                   KKSAttrType::KKSAttrTypes type, 
                                   bool * loaded);

        
        int createMyDocsRubricator(int idRubricator) const;
        int getMyDocsRubricatorId() const;
        int updateRubricators(KKSRubric * rootRubric, bool bMyDocs = false) const;

        int insertSearchGroup (int idParent, KKSFilterGroup *group) const;
        int deleteSearchGroup (int idGroup) const;
        int clearCriteriaGroup (int idGroup) const;

        int insertSearchTemplate (KKSSearchTemplate * st) const;
        int updateSearchTemplate (KKSSearchTemplate * st) const;
        int deleteSearchTemplate (int idSearchTemplate) const;
        
        int insertSearchTemplateType (KKSSearchTemplateType * t) const;

        int insertSearchCriterion (KKSFilter * f) const;
        int insertSearchCriterionIntoGroup (int idCriterion, int idGroup) const;

    protected:
    private:
        friend class KKSSito;
        friend class KKSSitoWeb;
        friend class KKSObjEditorFactory;//чтобы напрямую пользовательские шаблоны обновить
                                         //впринципе можно просто сделать эту функцию публичной
        KKSPPFactory();
        ~KKSPPFactory();

        KKSDatabase * db;
        KKSLoader * loader;
        KKSFileLoader * fileLoader;
        KKSEIOFactory * eiof;

        //если m_inTransaction.top() задан как TRUE
        //то это означает, что уже выполняется транзакция
        //и начинать новую не надо
        mutable QStack<bool> m_inTransaction;

        bool inTransaction() const;
        void setInTransaction() const;
        void restoreInTransaction() const;
    
        int insertAttrValues(const KKSObject * io) const;
        int deleteAttrValues(const KKSObject * io) const;
        int updateAttrValues(const KKSObject * io) const;

        int updateUserTemplates(const KKSObject * io) const;

        int insertFile(int idObject, KKSFile * f, QWidget * parent = NULL) const;
        int insertFiles(const KKSObject * io, QWidget * parent = NULL) const;
        //int deleteFiles(const KKSObject * io, bool bRemoveFiles = true) const;
        int updateFiles(const KKSObject * io, QWidget * parent = NULL) const;
        int updateFileInfo(KKSFile * f) const;
        int uploadFile(KKSFile * f, QWidget * parent) const;

        int updateIncludes(const KKSObject * io) const;
        int insertIncludes(const KKSObject * io) const;
        int deleteIncludes(int idObject) const;
public:
        int updateRubrics(KKSRubric * parent, int idMyDocsRubricator = -1) const;
private:
        int updateRubric(KKSRubric * r) const;
        int insertRubrics(KKSRubric * parent, int idMyDocsRubricator = -1) const;
        int insertRubric(KKSRubric * r, int idParent, int idObject, bool root = false, int idMyDocsRubricator = -1) const;
        int insertRubricItem(int idRubric, int idObject, bool isAutomated) const;
        void rollbackRubrics(KKSRubric * r, bool forUpdate = false) const;
        void rollbackRubric(KKSRubric * r, bool forUpdate = false) const;
        void commitRubrics(KKSRubric * r) const;
        void commitRubric(KKSRubric * r) const;
        int insertRubricators(KKSRubric * rootRubric, int idMyDocsRubricator, bool bMyDocs = false) const;
        int deleteRubricators(bool bMyDocs = false) const;
        int deleteRubric(int idRubric) const;

        int updateUserPrivileges(const KKSObject * io) const;
        int updateUserPrivileges(const KKSRubric * rubr, int idRubric) const;

        int insertAttrGroup(int idCategory, int idTemplate, KKSAttrGroup * g) const;
        int insertGroup(int idTemplate, KKSAttrGroup * g) const;
        int insertAttrView(int idCategory, int idTemplate, int idGroup, KKSAttrView * av) const;

        int deleteCategoryAttrs(int idCategory) const;
        int deleteLifeCycle(int idCategory) const;
        
        int cDeleteRubrics(int idCategory) const;
        int cInsertRubrics(const KKSCategory * c) const;
        int cUpdateRubrics(const KKSCategory * c) const;
        int cInsertRubrics(KKSRubric * parent) const;
        int cInsertRubric(KKSRubric * r, int idParent, int idCategory, bool root = false) const;

        int writePrivilegies (KKSAccessEntity * at, const KKSMap<int, KKSPrivilege*>& privs, bool isSys=true) const;
        int writeAccessRules (const KKSAccessEntity * acl, int idObject) const;
        int writeCatAccessRules (const KKSAccessEntity * acl, int idCategory) const;
      

private:

        bool categoryInUse(KKSCategory * c) const;

        int setAsLogging(int idOper, bool yes) const;
        int addLogAttrs(int idObject, int idOper) const;
        
        int saveCatTemplates (KKSCategory * c) const;

};

#endif
