/***********************************************************************
 * Module:  KKSCategory.h
 * Author:  sergey
 * Modified: 27 ноября 2008 г. 12:03:27
 * Purpose: Declaration of the class KKSCategory
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSCategory_h)
#define __KKSSITOOOM_KKSCategory_h

class KKSType;
class KKSState;
class KKSLifeCycle;
class KKSRubric;
class KKSAttrType;
class KKSAccessEntity;

#include <KKSRecord.h>
#include "KKSMap.h"
#include "KKSCategoryAttr.h"
#include "KKSTemplate.h"
#include "KKSFilter.h"
#include "KKSAttrType.h"
#include "kkspp_config.h"

class _PP_EXPORT KKSCategory : public KKSRecord
{
    public:
        KKSCategory();
        KKSCategory(const KKSCategory & c);
        KKSCategory(int id, const QString & name, KKSType * type);
        virtual ~KKSCategory();

        const KKSCategory * tableCategory() const;
        KKSCategory * tableCategory();
        const KKSCategory * recAttrCategory() const;
        KKSCategory * recAttrCategory();

        const KKSType * type() const;
        KKSType * type();

        bool isMain() const;
        void setMain(bool isMain);
   
        const KKSMap<int, KKSCategoryAttr *> & attributes() const;
        KKSMap<int, KKSCategoryAttr *> & attributes();
        const KKSCategoryAttr * attribute(int id) const;
        KKSCategoryAttr * attribute(int id);

        //метод создает фильтр по заданному атрибуту категории.
        //метод особенно полезен для дочерних категорий,
        //когда необходимо получить ЭИО заданного контейнерного ИО 
        //с учетом ограничений по значениям тех или иных атрибутов (полей) подчиненной таблицы
        /*KKSFilter * createFilter(const QString & attrCode, 
                                 const QString & value, 
                                 KKSFilter::FilterOper operation);
   
        KKSFilter * createFilter(const QString & attrCode, 
                                 const QStringList & values, 
                                 KKSFilter::FilterOper operation);*/

        KKSFilter * createFilter(int attrId, 
                                 const QString & value, 
                                 KKSFilter::FilterOper operation);
   
        KKSFilter * createFilter(int attrId, 
                                 const QStringList & values, 
                                 KKSFilter::FilterOper operation);

        void setAttributes(const KKSMap<int, KKSCategoryAttr *> & _attributes);
        //добавить более одного атрибута с одинаковым ключом (ИД) нельзя
        //поэтому при создании новых категорий и добавлении в них новых, 
        //еще не сохраненных в БД атрибутов, 
        //рекомендуется использовать отрицательные идентификаторы
        //другой рекомендацией является не добавлять не сохраненные в БД атрибуты
        //а при создании нового - сначала сохранять его в БД
        int addAttribute(int idAttribute, KKSCategoryAttr * a); //НЕ idCategoryAttr !!
        int removeAttribute(int idAttribute);//НЕ idCategoryAttr !!
        //////int removeAttribute(const QString & code);
        int replaceAttribute(int idAttribute, KKSCategoryAttr * a); //НЕ idCategoryAttr !!

        const KKSLifeCycle * lifeCycle() const;
        KKSLifeCycle * lifeCycle();

        void setTableCategory(KKSCategory * _tableCategory);
        void setRecAttrCategory(KKSCategory * _recAttrCategory);
        void setType(KKSType * _type);
        void setLifeCycle(KKSLifeCycle * _lifeCycle);

        //возвращает шаблон по умолчанию для данной категории
        //т.е. когда категория не имеет ни одного шаблона
        //В этом случае шаблон имеет одну общую группу 
        //и набор атрибутов в этой группе идентичен набору атрибутов в категории
        //(включая параметры атрибутов)
        const KKSTemplate & defTemplate() const;

        //Шаблоны для категории
        //!!!Не являются собственностью данной категории, поэтому методов для создания шаблонов нет
        //только методы для ведения списка шаблонов, 
        //за исключением случая удаления атрибута из категории, 
        //когда требуется удалить также атрибут из всех соответмствующих шаблонов
        
        //ВАЖНО!! Если шаблон еще не создан в БД следует использовать отрицательную нумерацию для идентификаторов шаблонов
        
        //ВАЖНО!! Добавить шаблон в категорию можно только в том случае, 
        //если метод шаблона category() указывает на категорию, в которую добавляется шаблон. 
        const KKSMap<int, KKSTemplate *> & getTemplates() const;
        KKSMap<int, KKSTemplate *> & getTemplates();
        const KKSTemplate * getTemplate(int idTemplate) const;
        KKSTemplate * getTemplate(int idTemplate);

        int addTemplate(KKSTemplate * t);
        int removeTemplate(int idTemplate);
        int clearTemplates();


        bool isSystem() const;
        void setAsSystem(bool yes = true);

        //определяетс глобальность категории.
        //только ИО глобальных категорий могут участвовать в информационном обмене
        bool isGlobal() const;
        void setAsGlobal(bool yes = true);

        //данные методы используются здя задания и получения корневой рубрики, 
        //которая содержит вложения данной категории
        //корневая рубрика у категории может быть только одна или не быть вообще.
        //В последнем случае это означает, что категории не заданы рубрики
        void setRootRubric(KKSRubric * r);
        KKSRubric * rootRubric() const;

        //перечень организаций, с которыми осуществляется 
        //синхронизация (информационный обмен) данной категории
        //т.е. перечень организаций, на которые данная категория 
        //должна быть отправлена при ее создании, изменении, удалении из БД
        //Задается набором идентификаторов
        //Обрабатывается как атрибут типа atCheckListEx
        //KKSValue replicatedOrgsV() const;
        //const QList<int> & replicatedOrgs() const;

        //int addReplicatedOrg(int id);
        //int removeReplicatedOrg(int id);
        //void setReplicatedOrgs(const QList<int> & orgs);
        //void setReplicatedOrgsV(const KKSValue & v);


        const KKSState * state() const;
        KKSState * state();
        void setState(KKSState * state);

        QList<int> searchAttributesByType (int aType) const;
        bool isAttrTypeContains (KKSAttrType::KKSAttrTypes type) const;

        const KKSAccessEntity * getAccessRules (void) const;
        KKSAccessEntity * getAccessRules (void);
        void setAccessRules (KKSAccessEntity * acl);

    protected:
    private:
        bool m_isMain;

        bool m_attrsModified;
        void setModified(bool yes);

        KKSType* m_type;
        KKSCategory* m_tableCategory; //категория описывает набор атрибутов, которые соответствуют колонкам в подчиненной таблице
        KKSCategory* m_recAttrCategory;//категория описывает набор пользовательских атрибутов (показателей), которыми могут обладать записи справочников
        KKSMap<int, KKSCategoryAttr *> m_attributes;//атрибуты категории. ВАЖНО!! В качестве ключа используется idAttribute, не idCategoryAttr !!!
        KKSLifeCycle* m_lifeCycle;

        KKSRubric * m_rootRubric;

        mutable KKSTemplate m_defTemplate;
        KKSMap<int, KKSTemplate *> m_templates;//шаблоны категории.

        bool m_isSystem;
        bool m_isGlobal;

        KKSState * m_state;

        KKSAccessEntity * m_acl;
        //QList<int> m_replicatedOrgs;

        //код атрибута не является уникальным, поэтому данные функции являются приватными
        //их можно использовать только когда категория описывает таблицу 
        //(в этом случае гарантированно все коды атрибутов будут уникальными)
        //используется при заполнении класса KKSEIOData в KKSLoader
        friend class KKSLoader;
        friend class KKSViewFactory;
        friend class KKSRubricFactory;
        KKSCategoryAttr * attribute(const QString & code);
        const KKSCategoryAttr * attribute(const QString & code) const;

};

#endif
