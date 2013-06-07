/***********************************************************************
 * Module:  KKSObject.h
 * Author:  sergey
 * Modified: 27 ноября 2008 г. 13:44:52
 * Purpose: Declaration of the class KKSObject
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSObject_h)
#define __KKSSITOOOM_KKSObject_h


#include <KKSRecord.h>
#include <KKSList.h>
#include <KKSMap.h>
#include <QDateTime>
#include <QColor>
#include <string>
#include "kkspp_config.h"
#include "KKSAttrValue.h"
#include "KKSIndicatorValue.h"
#include "KKSFile.h"
#include "KKSPrivilege.h"
#include "KKSCategory.h"

//class KKSCategory;
class KKSTemplate;
//class KKSAttrValue;
class KKSState;
class KKSType;
class KKSSyncType;
class KKSCategoryAttr;
class KKSRubric;
class KKSAccessEntity;
class KKSIndicatorValue;

using namespace std;

class _PP_EXPORT KKSObject : public KKSRecord
{
    public:
        KKSObject();
        KKSObject(int id,
               KKSCategory * c, 
               const QString & name, 
               const QString & desc=QString() );
        KKSObject(const KKSObject & io);
        virtual ~KKSObject();

        const QString & tableName(void) const;
        void setTableName(const QString & newTableName);

        const KKSList<KKSAttrValue *> & attrValues() const;
        KKSList<KKSAttrValue *> & attrValues();
        void setAttrValues(const KKSList<KKSAttrValue *> & _attrValues);
        void addAttrValue(KKSAttrValue * av);
        void addAttrValue(const QString & value,
                          KKSCategoryAttr * attr, 
                          bool * bBadValue = NULL);
   
        //удаляет из списка значений все значения атрибута с указанным id
        int removeAttrValueId(int id);
        //удаляет из списка значений указанное значение
        int removeAttrValue(KKSAttrValue * av);
        //удаляет из списка значений значение с указанным порядковым индексом
        int removeAttrValue(int index);

        KKSAttrValue * attrValue(int index);
        const KKSAttrValue * attrValue(int index) const;
        
        //KKSAttrValue * attrValue(const QString & attrCode);
        //const KKSAttrValue * attrValue(const QString & attrCode) const;

        KKSAttrValue * attrValueId(int attrId);
        const KKSAttrValue * attrValueId(int attrId) const;

        KKSList<KKSAttrValue *>  attrValuesByIdAttr(int attrId);
        const KKSList< const KKSAttrValue *> attrValuesByIdAttr(int attrId) const;


        const KKSList<KKSFile *> & files() const;
        KKSList<KKSFile *> & files();
        void setFiles(const KKSList<KKSFile *> & _files);
        void addFile(KKSFile * f);
   
        //удаляет из списка файлов указанный файл
        int removeFile(KKSFile * f);
        //удаляет из списка файлов файл с указанным порядковым индексом
        int removeFile(int index);

        KKSFile * file(int index);
        const KKSFile * file(int index) const;

        KKSCategory * category();
        const KKSCategory * category() const;
        void setCategory(KKSCategory * _category);

        const QDateTime & insertTime() const;
        void setInsertTime(const QDateTime & datetime);
 
        bool isSystem() const;
        void setIsSystem(bool isSys);

        bool isGlobal() const;
        void setIsGlobal(bool isGl);

        int author() const;
        void setAuthor(int id);

        int ownerOrg() const;
        void setOwnerOrg(int id);

        int idSearchTemplate() const;
        void setIdSearchTemplate(int idSt);
        const QString & refTableName() const;
        void setRefTableName(const QString & rtn);

        int idMaclabel() const;
        void setIdMaclabel(int id);

        //const KKSState * state() const;
        //KKSState * state();
        //void setState(KKSState * state);


        const KKSSyncType * syncType() const;
        KKSSyncType * syncType();
        void setSyncType(KKSSyncType * syncType);

        const KKSType * type() const;
        KKSType * type();
        void setType(KKSType * type);

        const QString & info(void) const;
        void setInfo(const QString & info);

        //данные методы используются для задания/получения шаблонов, 
        //которые используются по умолчанию при визуализации ИО для ДАННОГО пользователя
        void setTableTemplate(KKSTemplate * t);
        KKSTemplate * tableTemplate() const;
        void setAttrTemplate(KKSTemplate * t);
        KKSTemplate * attrTemplate() const;

        //данные методы используются для задания и получения корневой рубрики, 
        //которая содержит вложения данного ИО (особенно полезно для формуляров)
        //корневая рубрика у ИО может быть только одна или не быть вообще.
        void setRootRubric(KKSRubric * r);
        KKSRubric * rootRubric() const;

        //работа с дискреционными правами доступа
/*        const KKSMap<int, KKSPrivilege *> & privileges() const;
        KKSMap<int, KKSPrivilege *> & privileges();
        void setPrivileges(const KKSMap<int, KKSPrivilege *> & _privileges);
        int addPrivilege(int idRole, KKSPrivilege * p);
        int removePrivilege(int idRole);
        int replacePrivilege(int idRole, KKSPrivilege * p);
        const KKSPrivilege * privilege(int idRole) const;
        KKSPrivilege * privilege(int idRole);
        
        KKSMap<int, KKSPrivilege*> & bossPrivileges();
        const KKSMap<int, KKSPrivilege *> & bossPrivileges() const;
        KKSPrivilege * bossPrivilege(int idRole);
        const KKSPrivilege * bossPrivilege(int idRole) const;
        KKSMap<int, KKSPrivilege *> & unitPrivileges();
        const KKSMap<int, KKSPrivilege *> & unitPrivileges() const;
        KKSPrivilege * unitPrivilege(int idRole);
        const KKSPrivilege * unitPrivilege(int idRole) const;
        KKSPrivilege * othersPrivilege();
        KKSPrivilege * othersPrivilege() const;

        int setBossPrivilege(int idRole, KKSPrivilege * p);
        int setUnitPrivilege(int idRole, KKSPrivilege * p);
        int addBossPrivilege(int idRole, KKSPrivilege * p);
        int addUnitPrivilege(int idRole, KKSPrivilege * p);
        void setOthersPrivilege(KKSPrivilege * p);
*/
        const KKSAccessEntity * getAccessRules (void) const;
        KKSAccessEntity * getAccessRules (void);
        void setAccessRules (KKSAccessEntity * acl);

        void addAdditionalTable (QString tableName, KKSCategory * c);
        void clearAdditionalTables (void);
        const KKSMap<QString, KKSCategory*> getAdditionalTables (void) const;

        //перечень организаций, с которыми осуществляется 
        //синхронизация (информационный обмен) данного информационного объекта
        //т.е. перечень организаций, на которые данный ИО 
        //должен быть отправлен при его создании, изменении, удалении из БД
        //Задается набором идентификаторов
        //Обрабатывается как атрибут типа atCheckListEx
        KKSValue replicatedOrgsV() const;
        const QList<int> & replicatedOrgs() const;

        int addReplicatedOrg(int id);
        int removeReplicatedOrg(int id);
        void setReplicatedOrgs(const QList<int> & orgs);
        void setReplicatedOrgsV(const KKSValue & v);

        //
        // работа с индикаторами информационных объектов
        //
        const KKSList<KKSIndicatorValue *> & indicatorValues() const;
        KKSList<KKSIndicatorValue *> & indicatorValues();
        void setIndicatorValues(const KKSList<KKSIndicatorValue *> & _indicatorValues);
        void addIndicatorValue(KKSIndicatorValue * iv);

        //
        // удаляет из списка значений все значения индикатора с указанным id
        //
        int removeIndicatorValueId(int id);
        //
        // удаляет из списка значений указанное значение
        //
        int removeIndicatorValue(KKSIndicatorValue * av);
        //
        // удаляет из списка значений значение с указанным порядковым индексом
        //
        int removeIndicatorValue(int index);

        KKSIndicatorValue * indicatorValue(int index);
        const KKSIndicatorValue * indicatorValue(int index) const;
        
        KKSIndicatorValue * indicatorValueId(int attrId);
        const KKSIndicatorValue * indicatorValueId(int attrId) const;


    protected:
    private:
        friend class KKSPPFactory;
        QDateTime m_insertTime;
        /* Определает происхождение ИО.
         * Если ИО является системным, то он существует в системе изначально 
         * при ее установке, если не является системным, то он создается 
         * пользователями во время их работы.
         * Как правило, системные ИО являются контейнерными, т.е. справочниками.
         * Системные ИО удалить нельзя. Можно только модифицировать 
         * (добавлять, удалять, изменять) их экземпляры ИО. */
        bool m_isSystem;

       
        bool m_isGlobal;//определяет глобальность ИО. В информационном обмене могут участвовать только глобальные ИО
        
        /* Задает название таблицы БД, в которой хранятся ЭИО данного ИО */
        QString m_tableName;
        int m_idUser;  //author
        int m_idOrganization; //организация-владелец данного ИО
        QString m_info;
        int m_idMaclabel;//идентификатор грифа секретности из справочника грифов секретности

        bool m_modified;//флаг используется при определении необходимости вызова сохранения ИО в БД
        bool m_attrsModified;
        bool m_includesModified;
        bool m_filesModified;
        bool m_userTemplatesModified;
        void setModified(bool yes);
        
   
        //KKSState * m_state;
        KKSType* m_type; //тип ИО
        KKSSyncType* m_syncType; //тип синхронизации ИО

        KKSCategory* m_category;
        KKSList<KKSAttrValue *> m_attrValues;
        KKSList<KKSIndicatorValue *> m_indicatorValues;

        KKSList<KKSFile *> m_files;

        KKSTemplate * m_tableTemplate;
        KKSTemplate * m_attrTemplate;

        KKSRubric * m_rootRubric;

        //
        //дискреционные права доступа
        //
        //исключения (применяются всегда последними, поэтому если есть 
        //рассогласование между правами начальника, группы и соответствующими
        //исключениями, если таковые имеются, то применятся права, 
        //заданные для соответствующих исключений)
        //права для boss, unit специально не применяются
        //приложение должно отслеживать, что если пользователь изменил право
        //на boss или unit, то должны автоматически измениться права 
        //для соответствующих ролей в исключениях
        //Специально применяется только служебная роль OTHERS_ROLE
        //KKSMap<int, KKSPrivilege *> m_privileges;
        //KKSMap<int, KKSPrivilege *> m_bossPrivileges;
        //KKSMap<int, KKSPrivilege *> m_unitPrivileges;
        //KKSPrivilege * m_othersPrivilege;

        KKSAccessEntity * m_aclE;

        KKSMap<QString, KKSCategory*> m_addTables;

        QList<int> m_replicatedOrgs;

        int m_idSearchTemplate;
        QString m_refTableName;

        //const QString & code(bool quoted = false) const;
        //void setCode(const QString & newCode);

};

#endif
