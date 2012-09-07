/***********************************************************************
 * Module:  KKSObject.cpp
 * Author:  sergey
 * Modified: 27 ноября 2008 г. 13:44:52
 * Purpose: Implementation of the class KKSObject
 ***********************************************************************/

#include "KKSCategory.h"
#include "KKSAttrValue.h"
#include "KKSObject.h"
#include "KKSState.h"
#include "KKSType.h"
#include "KKSSyncType.h"
#include "KKSCategoryAttr.h"
#include "KKSAttrType.h"
#include "defines.h"
#include "KKSTemplate.h"
#include "KKSRubric.h"
#include "KKSAccessEntity.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSObject::KKSObject()
// Purpose:    Implementation of KKSObject::KKSObject()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSObject::KKSObject() : KKSRecord(),
    m_isSystem (false),
    m_isGlobal(false),
    m_tableName (QString()),
    m_idUser (-1),
    m_idOrganization(-1),
    m_idMaclabel (1),
    m_info (QString()),
    m_type (NULL),
    m_state (KKSState::defState1()),
    m_syncType (KKSSyncType::defSyncType1()),
    m_category (NULL),
    m_tableTemplate(NULL),
    m_attrTemplate(NULL),
    m_rootRubric(NULL),
    //m_bossPrivilege(NULL),
    //m_unitPrivilege(NULL),
    //m_othersPrivilege(NULL),
    m_aclE (0),
    m_addTables (KKSMap<QString, KKSCategory *>()),
    m_recordFillColor(QColor()),
    m_recordTextColor(QColor()),
    m_idSearchTemplate(-1),
    m_refTableName(QString())
{
    //в будущем наверное требуется изменить параметр на FALSE
    //сейчас же при изменении ИО всегда полагается, 
    //что изменены все его характеристики
    setModified(true);
}

KKSObject::KKSObject(int id, KKSCategory * c, const QString & name, const QString & desc) : KKSRecord(id, name, desc),
    m_isSystem (false),
    m_isGlobal(false),
    m_tableName (QString()),
    m_idUser (-1),
    m_idOrganization(-1),
    m_idMaclabel (1),
    m_info (QString()),
    m_type (NULL),
    m_state (KKSState::defState1()),
    m_syncType (KKSSyncType::defSyncType1()),
    m_category (NULL),
    m_tableTemplate(NULL),
    m_attrTemplate(NULL),
    m_rootRubric(NULL),
    //m_bossPrivilege(NULL),
    //m_unitPrivilege(NULL),
    //m_othersPrivilege(NULL),
    m_aclE (0),
    m_addTables (KKSMap<QString, KKSCategory *>()),
    m_recordFillColor(QColor()),
    m_recordTextColor(QColor()),
    m_idSearchTemplate(-1),
    m_refTableName(QString())
{
    //данный метод в том числе задает рубрики объекту, если у категории таковые были заданы
    //производится копирование рубрик категории в рубрики объекта
    setCategory(c);
    //в будущем наверное требуется изменить параметр на FALSE
    //сейчас же при изменении ИО всегда полагается, 
    //что изменены все его характеристики
    setModified(true);
}

KKSObject::KKSObject(const KKSObject & io) : KKSRecord(io),
    m_isSystem (io.m_isSystem),
    m_isGlobal(io.m_isGlobal),
    m_tableName (io.m_tableName),
    m_idUser (io.m_idUser),
    m_idOrganization(io.m_idOrganization),
    m_idMaclabel (io.m_idMaclabel),
    m_info (io.m_info),
    m_state (NULL),
    m_type (NULL),
    m_syncType(NULL),
    m_category (NULL),
    m_attrValues (io.m_attrValues),
    m_indicatorValues(io.m_indicatorValues),
    m_files (io.m_files),
    m_tableTemplate(NULL),
    m_attrTemplate(NULL),
    m_rootRubric(NULL),
    m_modified(io.m_modified),
    m_attrsModified(io.m_attrsModified),
    m_filesModified(io.m_filesModified),
    m_includesModified(io.m_includesModified),
    m_userTemplatesModified(io.m_userTemplatesModified),
    //m_privileges(io.m_privileges),
    //m_bossPrivileges(io.m_bossPrivileges),
    //m_unitPrivileges(io.m_unitPrivileges),
    //m_othersPrivilege(NULL),
    m_aclE (io.m_aclE),
    m_addTables (KKSMap<QString, KKSCategory *>()),
    m_recordFillColor(io.recordFillColor()),
    m_recordTextColor(io.recordTextColor()),
    m_idSearchTemplate(io.m_idSearchTemplate),
    m_refTableName(io.m_refTableName)
{
    setCategory(const_cast<KKSCategory*>(io.category()));
    
    setTableTemplate(io.tableTemplate());
    setAttrTemplate(io.attrTemplate());

    setRootRubric(io.rootRubric());

    //setBossPrivilege(io.bossPrivilege());
    //setUnitPrivilege(io.unitPrivilege());
    //setOthersPrivilege(io.othersPrivilege());
    if (m_aclE)
        m_aclE->addRef();

    setState(const_cast<KKSState*>(io.state()));
    setType(const_cast<KKSType*>(io.type()));
    setSyncType(const_cast<KKSSyncType*>(io.syncType()));

    for (KKSMap<QString, KKSCategory *>::const_iterator p = io.m_addTables.constBegin(); \
                                                        p != io.m_addTables.constEnd(); \
                                                        p++)
    {
        this->addAdditionalTable (p.key(), p.value());
    }
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSObject::~KKSObject()
// Purpose:    Implementation of KKSObject::~KKSObject()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSObject::~KKSObject()
{
    if(m_category)
        m_category->release();

    if(m_tableTemplate)
        m_tableTemplate->release();

    if(m_attrTemplate)
        m_attrTemplate->release();

    if(m_state)
        m_state->release();

    if(m_type)
        m_type->release();

    if(m_syncType)
        m_syncType->release();

    if(m_rootRubric)
        m_rootRubric->release();


    if (m_aclE)
        m_aclE->release ();
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSObject::getTableName()
// Purpose:    Implementation of KKSObject::getTableName()
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSObject::tableName(void) const 
{
   return m_tableName;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSObject::setTableName(QString newTableName)
// Purpose:    Implementation of KKSObject::setTableName()
// Parameters:
// - newTableName
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSObject::setTableName(const QString & newTableName)
{
   m_tableName = newTableName;
}

const KKSCategory * KKSObject::category() const
{
    return m_category;
}


KKSCategory * KKSObject::category()
{
    return m_category;
}


void KKSObject::setCategory(KKSCategory * _category)
{
    if(m_category)
        m_category->release();

    m_category = _category;
    if(m_category)
        m_category->addRef();

    //if(!m_category)
    //    return;

    //KKSRubric * cRootRubric = m_category->rootRubric();
    //if(!cRootRubric)
    //    return;

    //if(m_rootRubric)
    //    m_rootRubric->release();

    //m_rootRubric = cRootRubric->deepCopy(true);//addref сделано там (в виде new)
}


const QDateTime & KKSObject::insertTime() const
{
    return m_insertTime;
}

void KKSObject::setInsertTime(const QDateTime & datetime)
{
    m_insertTime = datetime;
}

bool KKSObject::isSystem() const
{
    return m_isSystem;
}

void KKSObject::setIsSystem(bool isSys)
{
    m_isSystem = isSys;
}

bool KKSObject::isGlobal() const
{
    return m_isGlobal;
}

void KKSObject::setIsGlobal(bool isGl)
{
    m_isGlobal = isGl;
}

int KKSObject::author() const
{
    return m_idUser;
}

void KKSObject::setAuthor(int id)
{
    m_idUser = id;
}

int KKSObject::ownerOrg() const
{
    return m_idOrganization;
}

void KKSObject::setOwnerOrg(int id)
{
    m_idOrganization = id;
}

int KKSObject::idMaclabel() const
{
    return m_idMaclabel;
}

void KKSObject::setIdMaclabel(int id)
{
    m_idMaclabel = id;
}

const KKSState * KKSObject::state() const
{
    return m_state;
}

KKSState * KKSObject::state()
{
    return m_state;
}

void KKSObject::setState(KKSState * state)
{
    if(m_state)
        m_state->release();

    m_state = state;
    if(m_state)
        m_state->addRef();
}

const KKSType * KKSObject::type() const
{
    return m_type;
}

KKSType * KKSObject::type()
{
    return m_type;
}

void KKSObject::setType(KKSType * type)
{
    if(m_type)
        m_type->release();

    m_type = type;
    if(m_type)
        m_type->addRef();
}

const KKSSyncType * KKSObject::syncType() const
{
    return m_syncType;
}

KKSSyncType* KKSObject::syncType()
{
    return m_syncType;
}

void KKSObject::setSyncType(KKSSyncType * sy)
{
    if(m_syncType)
        m_syncType->release();

    m_syncType = sy;
    if(m_syncType)
        m_syncType->addRef();
}

const QString & KKSObject::info(void) const
{
    return m_info;
}

void KKSObject::setInfo(const QString & info)
{
    m_info = info;
}

const KKSList<KKSAttrValue *> & KKSObject::attrValues() const
{
    return m_attrValues;
}

KKSList<KKSAttrValue *> & KKSObject::attrValues()
{
    return m_attrValues;
}

void KKSObject::setAttrValues(const KKSList<KKSAttrValue *> & _attrValues)
{
    m_attrValues = _attrValues;
    m_attrsModified = true;
}
void KKSObject::addAttrValue(KKSAttrValue * av)
{
    if(!av)
        return;

    m_attrValues.append(av);
    m_attrsModified = true;
}

void KKSObject::addAttrValue(const QString & value, KKSCategoryAttr * attr, bool * bBadValue)
{
    if(!attr){
        if(bBadValue)
            *bBadValue = true;
        return;
    }

    bool badValue = false;
    KKSValue v;
    v.setValue(value, attr->type()->attrType());
    if(!v.isValid())
        badValue = true;
    
    KKSAttrValue * av = new KKSAttrValue(v, attr);
    addAttrValue(av);
    av->release();

    if(bBadValue)
        *bBadValue = badValue;

    m_attrsModified = true;
}

int KKSObject::removeAttrValueId(int id)
{
    
    int count = m_attrValues.count();
    int cnt = 0;
    for(int i=0; i<count; i++){
        KKSCategoryAttr * ca = m_attrValues.at(i)->attribute();
        if(ca->id() == id){
            m_attrValues.removeAt(i);
            i--;
            count--;
            cnt++;
        }
    }

    if(cnt ==  0)
        return ERROR_CODE;

    m_attrsModified = true;
    return OK_CODE;
}

int KKSObject::removeAttrValue(KKSAttrValue * av)
{
    if(!av)
        return ERROR_CODE;
    
    int cnt = m_attrValues.removeAll(av);
    
    if(cnt == 0)
        return ERROR_CODE;
    
    m_attrsModified = true;
    return OK_CODE;
}

int KKSObject::removeAttrValue(int index)
{
    int ok = m_attrValues.removeAt(index);
    if(ok == 0)
        return ERROR_CODE;

    m_attrsModified = true;
    return OK_CODE;
}

KKSAttrValue * KKSObject::attrValue(int index)
{
    KKSAttrValue * av = NULL;
    if(index >= m_attrValues.count())
        return av;

    av = m_attrValues[index];
    return av;
}

const KKSAttrValue * KKSObject::attrValue(int index) const
{
    const KKSAttrValue * av = NULL;
    if(index >= m_attrValues.count())
        return av;

    av = m_attrValues.at(index);
    return av;
}

KKSAttrValue * KKSObject::attrValueId(int attrId)
{
    KKSAttrValue * av = NULL;

    for (int i=0; i<m_attrValues.count(); i++)
    {
        KKSAttrValue * av1 = m_attrValues [i];
        if(av1->attribute()->id() == attrId){
            av = av1;
            break;            
        }
    }

    return av;
}

const KKSAttrValue * KKSObject::attrValueId(int attrId) const
{
    const KKSAttrValue * av = NULL;
    
    for (int i=0; i<m_attrValues.count(); i++)
    {
        KKSAttrValue * av1 = m_attrValues [i];
        if(av1->attribute()->id() == attrId){
            av = av1;
            break;            
        }
    }

    return av;
}

KKSList<KKSAttrValue *>  KKSObject::attrValuesByIdAttr(int attrId)
{
    KKSList<KKSAttrValue *> avs;

    for (int i=0; i<m_attrValues.count(); i++)
    {
        KKSAttrValue * av = m_attrValues [i];
        if(av->attribute()->id() == attrId){
            avs.append(av);
        }
    }

    return avs;
}

const KKSList< const KKSAttrValue *> KKSObject::attrValuesByIdAttr(int attrId) const
{
    KKSList<const KKSAttrValue *> avs;

    for (int i=0; i<m_attrValues.count(); i++)
    {
        const KKSAttrValue * av = m_attrValues.at(i);
        if(av->attribute()->id() == attrId){
            avs.append(av);
        }
    }

    return avs;
}


const KKSList<KKSFile *> & KKSObject::files() const
{
    return m_files;
}

KKSList<KKSFile *> & KKSObject::files()
{
    return m_files;
}

void KKSObject::setFiles(const KKSList<KKSFile *> & _files)
{
    m_files = _files;
    m_filesModified = true;
}

void KKSObject::addFile(KKSFile * f)
{
    if(!f)
        return;

    m_filesModified = true;
    m_files.append(f);
}

//удаляет из списка файлов указанный файл
int KKSObject::removeFile(KKSFile * f)
{
    if(!f)
        return ERROR_CODE;
    
    int cnt = m_files.removeAll(f);
    
    if(cnt == 0)
        return ERROR_CODE;

    m_filesModified = true;
    
    return OK_CODE;
}

//удаляет из списка файлов файл с указанным порядковым индексом
int KKSObject::removeFile(int index)
{
    int ok = m_files.removeAt(index);
    if(ok == 0)
        return ERROR_CODE;

    m_filesModified = true;
    return OK_CODE;
}

KKSFile * KKSObject::file(int index)
{
    KKSFile * f = NULL;
    if(index >= m_files.count())
        return f;

    f = m_files[index];
    return f;
}

const KKSFile * KKSObject::file(int index) const
{
    const KKSFile * f = NULL;
    if(index >= m_files.count())
        return f;

    f = m_files.at(index);

    return f;
}

void KKSObject::setTableTemplate(KKSTemplate * t)
{
    if(m_tableTemplate)
        m_tableTemplate->release();

    m_tableTemplate = t;

    if(m_tableTemplate)
        m_tableTemplate->addRef();

    m_userTemplatesModified = true;
}

void KKSObject::setAttrTemplate(KKSTemplate * t)
{
    if(m_attrTemplate)
        m_attrTemplate->release();

    m_attrTemplate = t;

    if(m_attrTemplate)
        m_attrTemplate->addRef();
    
    m_userTemplatesModified = true;
}

KKSTemplate * KKSObject::tableTemplate() const
{
    return m_tableTemplate;
}

KKSTemplate * KKSObject::attrTemplate() const
{
    return m_attrTemplate;
}

void KKSObject::setRootRubric(KKSRubric * r)
{
    if(m_rootRubric)
        m_rootRubric->release();

    m_rootRubric = r;

    if(m_rootRubric)
        m_rootRubric->addRef();

    m_includesModified = true;
}

KKSRubric * KKSObject::rootRubric() const
{
    return m_rootRubric;
}

void KKSObject::setModified(bool yes)
{
    m_modified = yes;
    m_attrsModified = yes;
    m_filesModified = yes;
    m_includesModified = yes;
    m_userTemplatesModified = yes;
}

//
//работа с дискреционными правами доступа
//
/*
const KKSMap<int, KKSPrivilege *> & KKSObject::privileges() const
{
    return m_privileges;
}

KKSMap<int, KKSPrivilege *> & KKSObject::privileges()
{
    return m_privileges;
}

void KKSObject::setPrivileges(const KKSMap<int, KKSPrivilege *> & _privileges)
{
    m_privileges = _privileges;
}

int KKSObject::addPrivilege(int idRole, KKSPrivilege * p)
{
    int cnt = m_privileges.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSObject::removePrivilege(int idRole)
{
    m_privileges.remove(idRole);
    return OK_CODE;
}

int KKSObject::replacePrivilege(int idRole, KKSPrivilege * p)
{
    removePrivilege(idRole);
    int cnt = addPrivilege(idRole, p);

    return cnt == 0 ? ERROR_CODE : OK_CODE;
}

const KKSPrivilege * KKSObject::privilege(int idRole) const
{
    KKSPrivilege * p = NULL;

     QMap<int, KKSPrivilege*>::const_iterator i = m_privileges.find(idRole);
     while (i != m_privileges.end() && i.key() == idRole) {
         p = i.value();
         //берем всегда первый элемент
         break;
     }

    return p;
}

KKSPrivilege * KKSObject::privilege(int idRole)
{
    KKSPrivilege * p = NULL;

     QMap<int, KKSPrivilege *>::iterator i = m_privileges.find(idRole);
     while (i != m_privileges.end() && i.key() == idRole) {
         p = i.value();
         //берем всегда первый элемент
         break;
     }

    return p;
}

KKSMap<int, KKSPrivilege *> & KKSObject::bossPrivileges()
{
    return m_bossPrivileges;
}

const KKSMap<int, KKSPrivilege *> & KKSObject::bossPrivileges() const
{
    return m_bossPrivileges;
}

KKSMap<int, KKSPrivilege *> & KKSObject::unitPrivileges()
{
    return m_unitPrivileges;
}

const KKSMap<int, KKSPrivilege *> & KKSObject::unitPrivileges() const
{
    return m_unitPrivileges;
}

KKSPrivilege * KKSObject::othersPrivilege()
{
    return m_othersPrivilege;
}

KKSPrivilege * KKSObject::othersPrivilege() const
{
    return m_othersPrivilege;
}

int KKSObject::setBossPrivilege(int idRole, KKSPrivilege * p)
{
    if(!p)
        return ERROR_CODE;

    KKSPrivilege * pr = bossPrivilege(idRole);
    if(!pr)
        return ERROR_CODE;

    m_bossPrivileges.remove(idRole);
    int cnt = m_bossPrivileges.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;


    return OK_CODE;
}

int KKSObject::addBossPrivilege(int idRole, KKSPrivilege * p)
{
    int cnt = m_bossPrivileges.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSObject::setUnitPrivilege(int idRole, KKSPrivilege * p)
{
    if(!p)
        return ERROR_CODE;

    KKSPrivilege * pr = unitPrivilege(idRole);
    if(!pr)
        return ERROR_CODE;

    m_unitPrivileges.remove(idRole);
    int cnt = m_unitPrivileges.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;


    return OK_CODE;
}

int KKSObject::addUnitPrivilege(int idRole, KKSPrivilege * p)
{
    int cnt = m_unitPrivileges.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

void KKSObject::setOthersPrivilege(KKSPrivilege * p)
{
    if(m_othersPrivilege)
        m_othersPrivilege->release();

    m_othersPrivilege = p;

    if(m_othersPrivilege)
        m_othersPrivilege->addRef();
}


const KKSPrivilege * KKSObject::bossPrivilege(int idRole) const
{
    KKSPrivilege * p = NULL;

     QMap<int, KKSPrivilege*>::const_iterator i = m_bossPrivileges.find(idRole);
     while (i != m_bossPrivileges.end() && i.key() == idRole) {
         p = i.value();
         //берем всегда первый элемент
         break;
     }

    return p;
}

KKSPrivilege * KKSObject::bossPrivilege(int idRole)
{
    KKSPrivilege * p = NULL;

     QMap<int, KKSPrivilege *>::iterator i = m_bossPrivileges.find(idRole);
     while (i != m_bossPrivileges.end() && i.key() == idRole) {
         p = i.value();
         //берем всегда первый элемент
         break;
     }

    return p;
}

const KKSPrivilege * KKSObject::unitPrivilege(int idRole) const
{
    KKSPrivilege * p = NULL;

     QMap<int, KKSPrivilege*>::const_iterator i = m_unitPrivileges.find(idRole);
     while (i != m_unitPrivileges.end() && i.key() == idRole) {
         p = i.value();
         //берем всегда первый элемент
         break;
     }

    return p;
}

KKSPrivilege * KKSObject::unitPrivilege(int idRole)
{
    KKSPrivilege * p = NULL;

     QMap<int, KKSPrivilege *>::iterator i = m_unitPrivileges.find(idRole);
     while (i != m_unitPrivileges.end() && i.key() == idRole) {
         p = i.value();
         //берем всегда первый элемент
         break;
     }

    return p;
}
*/
const KKSAccessEntity * KKSObject :: getAccessRules (void) const
{
    return this->m_aclE;
}

KKSAccessEntity * KKSObject :: getAccessRules (void)
{
    return m_aclE;
}

void KKSObject :: setAccessRules (KKSAccessEntity * acl)
{
    if (m_aclE)
        m_aclE->release ();

    m_aclE = acl;

    if (m_aclE)
        m_aclE->addRef ();
}


void KKSObject::addAdditionalTable (QString tableName, KKSCategory * c)
{
    if (!c || m_addTables.contains (tableName))
        return;
    c->addRef ();
    m_addTables.insert (tableName, c);
}

void KKSObject::clearAdditionalTables (void)
{
    for (KKSMap<QString, KKSCategory *>::const_iterator p = m_addTables.constBegin(); \
                                                        p != m_addTables.constEnd(); \
                                                        p++)
    {
        if (p.value())
            p.value()->release ();
    }
    m_addTables.clear ();
}

const KKSMap<QString, KKSCategory*> KKSObject::getAdditionalTables (void) const
{
    return m_addTables;
}

KKSValue KKSObject::replicatedOrgsV() const
{
    QString s = "{";
    int cnt = m_replicatedOrgs.count();
    for (int i=0; i<cnt; i++){
        s += QString::number(m_replicatedOrgs.at(i));
        if(i < cnt-1)
            s += ",";
    }
    s += "}";

    KKSValue v(s, KKSAttrType::atCheckListEx);

    return v;
}

const QList<int> & KKSObject::replicatedOrgs() const
{
    return m_replicatedOrgs;
}

int KKSObject::addReplicatedOrg(int id)
{
    m_replicatedOrgs.append(id);

    return OK_CODE;
}

int KKSObject::removeReplicatedOrg(int id)
{
    int cnt = m_replicatedOrgs.removeAll(id);
    if(cnt == 0)
        return ERROR_CODE;

    return OK_CODE;
}

void KKSObject::setReplicatedOrgs(const QList<int> & orgs)
{
    m_replicatedOrgs = orgs;
}

void KKSObject::setReplicatedOrgsV(const KKSValue & v)
{
    m_replicatedOrgs.clear();
    QStringList sl = v.valueVariant().toStringList();
    int cnt = sl.count();
    for(int i=0; i<cnt; i++){
        int id = sl.at(i).toInt();
        if(id > 0)
            m_replicatedOrgs.append(id);
    }
}

const QColor & KKSObject::recordFillColor() const
{
    return m_recordFillColor;
}

void KKSObject::setRecordFillColor(const QColor & color)
{
    m_recordFillColor = color;
}

const QColor & KKSObject::recordTextColor() const
{
    return m_recordTextColor;
}

void KKSObject::setRecordTextColor(const QColor & color)
{
    m_recordTextColor = color;
}

int KKSObject::idSearchTemplate() const
{
    return m_idSearchTemplate;
}

void KKSObject::setIdSearchTemplate(int idSt)
{
    m_idSearchTemplate = idSt;
}

const QString & KKSObject::refTableName() const
{
    return m_refTableName;
}

void KKSObject::setRefTableName(const QString & rtn)
{
    m_refTableName = rtn;
}
/*
const QString & KKSObject::code(bool quoted) const
{
    Q_UNUSED(quoted);
    return QString::null;
}

void KKSObject::setCode(const QString & newCode)
{
    Q_UNUSED(newCode);
}
*/

/*************/
const KKSList<KKSIndicatorValue *> & KKSObject::indicatorValues() const
{
    return m_indicatorValues;
}

KKSList<KKSIndicatorValue *> & KKSObject::indicatorValues()
{
    return m_indicatorValues;
}

void KKSObject::setIndicatorValues(const KKSList<KKSIndicatorValue *> & _indicatorValues)
{
    m_indicatorValues = _indicatorValues;
    //m_attrsModified = true;
}
void KKSObject::addIndicatorValue(KKSIndicatorValue * iv)
{
    if(!iv)
        return;

    m_indicatorValues.append(iv);
    //m_attrsModified = true;
}

int KKSObject::removeIndicatorValueId(int id)
{
    
    int count = m_indicatorValues.count();
    int cnt = 0;
    for(int i=0; i<count; i++){
        if(m_indicatorValues.at(i)->id() == id){
            m_indicatorValues.removeAt(i);
            i--;
            count--;
            cnt++;
        }
    }

    if(cnt ==  0)
        return ERROR_CODE;

    //m_attrsModified = true;
    return OK_CODE;
}

int KKSObject::removeIndicatorValue(KKSIndicatorValue * iv)
{
    if(!iv)
        return ERROR_CODE;
    
    int cnt = m_indicatorValues.removeAll(iv);
    
    if(cnt == 0)
        return ERROR_CODE;
    
    //m_attrsModified = true;
    return OK_CODE;
}

int KKSObject::removeIndicatorValue(int index)
{
    int ok = m_indicatorValues.removeAt(index);
    if(ok == 0)
        return ERROR_CODE;

    //m_attrsModified = true;
    return OK_CODE;
}

KKSIndicatorValue * KKSObject::indicatorValue(int index)
{
    KKSIndicatorValue * iv = NULL;
    if(index >= m_indicatorValues.count())
        return iv;

    iv = m_indicatorValues[index];
    return iv;
}

const KKSIndicatorValue * KKSObject::indicatorValue(int index) const
{
    const KKSIndicatorValue * av = NULL;
    if(index >= m_indicatorValues.count())
        return av;

    av = m_indicatorValues.at(index);
    return av;
}

KKSIndicatorValue * KKSObject::indicatorValueId(int indId)
{
    KKSIndicatorValue * av = NULL;

    for (int i=0; i<m_indicatorValues.count(); i++)
    {
        KKSIndicatorValue * av1 = m_indicatorValues[i];
        if(av1->id() == indId){
            av = av1;
            break;            
        }
    }

    return av;
}

const KKSIndicatorValue * KKSObject::indicatorValueId(int indId) const
{
    const KKSIndicatorValue * av = NULL;
    
    for (int i=0; i<m_indicatorValues.count(); i++)
    {
        KKSIndicatorValue * av1 = m_indicatorValues[i];
        if(av1->id() == indId){
            av = av1;
            break;            
        }
    }

    return av;
}

QIcon KKSObject :: icon (void) const
{
    return m_icon;
}

void KKSObject :: setIcon (const QString & s)
{
    QPixmap px;
    px.loadFromData(s.toUtf8());

    m_icon = QIcon(px);
    m_iconData = s;
}

const QString & KKSObject :: iconAsString () const
{
    return m_iconData;
}
