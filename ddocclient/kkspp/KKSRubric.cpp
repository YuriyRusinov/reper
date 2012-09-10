/***********************************************************************
 * Module:  KKSFilter.cpp
 * Author:  sergey
 * Modified: 25 декабря 2008 г. 17:22:35
 * Purpose: Implementation of the class KKSFilter
 * Comment: фильтр по атрибуту
 *    используется при фильтрации справочников, табелей, журналов и прочих  ИО, которые являются контейнерными.
 ***********************************************************************/

#include "KKSRubric.h"
#include "KKSSearchTemplate.h"
#include "KKSPrivilege.h"
#include "KKSCategory.h"
#include "KKSAccessEntity.h"
#include "defines.h"

QPixmap * pxRubric = NULL;
QPixmap * pxRubricItem = NULL;

KKSRubricItem::KKSRubricItem() : KKSData(),
    m_idItem (-1),
    m_isAutomated (false),
    m_isUpdated (false),
    m_rubrItemIcon (QIcon()),
    m_iconData(QString())
{
}

KKSRubricItem::KKSRubricItem(int idItem, const QString & name, bool b) : KKSData(),
    m_idItem (idItem),
    m_name (name),
    m_isAutomated (b),
    m_isUpdated (false),
    m_rubrItemIcon (QIcon()),
    m_iconData(QString())
{
}

KKSRubricItem::KKSRubricItem(const KKSRubricItem & other) : KKSData(),
    m_idItem (other.m_idItem),
    m_name (other.m_name),
    m_isAutomated (other.m_isAutomated),
    m_isUpdated (other.m_isUpdated),
    m_rubrItemIcon (other.m_rubrItemIcon),
    m_iconData(other.m_iconData)
{
}

KKSRubricItem::~KKSRubricItem()
{

}

void KKSRubricItem::setId(int id)
{
    m_idItem = id;
}

int KKSRubricItem::id() const
{
    return m_idItem;
}

void KKSRubricItem::setName(const QString & name)
{
    m_name = name;
}

const QString & KKSRubricItem::name() const
{
    return m_name;
}

void KKSRubricItem::setIcon(const QPixmap & px)
{
    if(pxRubricItem)
        delete pxRubricItem;

    pxRubricItem = new QPixmap(px);
}

QPixmap KKSRubricItem::icon()
{
    QPixmap px;
    
    if(pxRubricItem)
        px = QPixmap(*pxRubricItem);

    return px;
}

QIcon KKSRubricItem::getIcon (void) const
{
    return m_rubrItemIcon;
}

void KKSRubricItem :: setIcon (const QString & s)
{
    QPixmap px;
    px.loadFromData(s.toUtf8());

    m_rubrItemIcon = QIcon(px);
    m_iconData = s;
}

const QString KKSRubricItem :: iconAsString () const
{
    return m_iconData;
}

/*===========================================*/
KKSRubric::KKSRubric() : KKSRecord()
{

}

KKSRubric::KKSRubric(const KKSRubric & other) : KKSRecord(other),
    m_searchTemplate (other.m_searchTemplate),
    m_category (other.m_category),
    m_acl (other.m_acl),
/*    m_privileges(other.m_privileges),
    m_bossPrivileges(other.m_bossPrivileges),
    m_unitPrivileges(other.m_unitPrivileges),
    m_othersPrivilege(NULL),*/
    m_intId(other.m_intId),
    m_rubricIcon (other.m_rubricIcon),
    m_iconData(other.m_iconData),
    m_isCategorized (other.m_isCategorized)
{
    m_items = other.items();
    m_rubrics = other.rubrics();

    if (m_searchTemplate)
        m_searchTemplate->addRef ();

    if (m_category)
        m_category->addRef ();

    if (m_acl)
        m_acl->addRef ();
    //setOthersPrivilege(other.othersPrivilege());
}

KKSRubric::KKSRubric(int id, const QString & name, KKSSearchTemplate * st, KKSCategory * c, KKSAccessEntity * ac) : KKSRecord(id, name),
    m_searchTemplate (st),
    m_category (c),
    m_acl (ac),
/*    m_privileges (KKSMap<int, KKSPrivilege *>()),
    m_bossPrivileges (KKSMap<int, KKSPrivilege *>()),
    m_unitPrivileges (KKSMap<int, KKSPrivilege *>()),
    m_othersPrivilege (0),*/
    m_intId (-1),
    m_rubricIcon (QIcon()),
    m_iconData(QString()),
    m_isCategorized (false)
{
    if (m_searchTemplate)
        m_searchTemplate->addRef ();
    if (m_category)
        m_category->addRef ();
    if (m_acl)
        m_acl->addRef ();
}

KKSRubric::~KKSRubric()
{
    if (m_searchTemplate)
        m_searchTemplate->release ();
    if (m_category)
        m_category->release ();
    if (m_acl)
        m_acl->release ();
}

KKSRubric & KKSRubric::operator = (const KKSRubric & other) 
{
    setId(other.id());
    setName(other.name());
    setCode(other.code());
    setDesc(other.desc());
    setParent(const_cast<KKSRecord*>(other.parent()));

    m_intId = other.m_intId;

    setItems(other.items());
    setRubrics(other.rubrics());
    
    m_searchTemplate = other.m_searchTemplate;
    if (m_searchTemplate)
        m_searchTemplate->addRef ();
    
    m_category = other.m_category;
    if (m_category)
        m_category->addRef ();
    
    //setOthersPrivilege (other.m_othersPrivilege);
    m_acl = other.m_acl;
    if (m_acl)
        m_acl->addRef();
    
    m_rubricIcon = other.m_rubricIcon;
    m_iconData = other.m_iconData;
    m_isCategorized = other.m_isCategorized;
    
    return *this;
}

void KKSRubric::addItem(const KKSRubricItem * item)
{
    if(!item)
        return;
    
    m_items.append(item);
}

void KKSRubric::addItems(const KKSList<const KKSRubricItem*> & items)
{
    for (int i=0; i<items.count(); i++)
    {
        const KKSRubricItem * item = items[i];
        m_items.append(item);
    }
}

void KKSRubric::setItems(const KKSList<const KKSRubricItem *> & items)
{
    m_items.clear();
    m_items = items;
}

void KKSRubric::removeItem(int index)
{
    m_items.removeAt(index);
}

void KKSRubric::clear()
{
    clearItems();
    clearRubrics();
}

void KKSRubric::clearItems()
{
    m_items.clear();
}

void KKSRubric::clearRubrics()
{
    m_rubrics.clear();
}

const KKSRubricItem * KKSRubric::item(int index) const
{
    return m_items.at(index);
}

const KKSList<const KKSRubricItem *> & KKSRubric::items() const
{
    return m_items;
}


void KKSRubric::addRubric(const KKSRubric * r)
{
    if(!r)
        return;
    
    m_rubrics.append(r);
}

void KKSRubric::addRubrics(const KKSList<const KKSRubric *> & rubrics)
{
    for (int i=0; i<rubrics.count(); i++)
    {
        const KKSRubric * r = rubrics[i];
        m_rubrics.append(r);
    }
}

void KKSRubric::setRubrics(const KKSList<const KKSRubric*> & rubrics)
{
    m_rubrics.clear();
    m_rubrics = rubrics;
}

void KKSRubric::removeRubric(int index)
{
    const KKSRubric * rr = m_rubrics.at(index);
    if(!rr)
        return;

    m_deletedRubrics.append(rr);
    m_rubrics.removeAt(index);
}

const KKSRubric * KKSRubric::rubric(int index) const
{
    return m_rubrics.at(index);
}

KKSRubric * KKSRubric::rubric(int index)
{
    if(index<m_rubrics.count() && index >= 0)
        return const_cast<KKSRubric*>(m_rubrics[index]);
    
    return NULL;
}

const KKSList<const KKSRubric*> & KKSRubric::rubrics() const
{
    return m_rubrics;
}

const KKSRubric * KKSRubric::rubricForId(int id, bool recursivelly) const
{
    if(id <= 0)
        return NULL;

    if(this->id() == id)
        return this;

    int cnt = m_rubrics.count();
    for(int i=0; i<cnt; i++){
        const KKSRubric * r = m_rubrics.at(i);
        if(r->id() == id){
            return r;
        }
        else if(recursivelly){
            const KKSRubric * r1 = r->rubricForId(id, recursivelly);
            if(r1)
                return r1;
        }
    }

    return NULL;
}

KKSRubric * KKSRubric::rubricForId(int id, bool recursivelly)
{
    if(id <= 0)
        return NULL;

    if(this->id() == id)
        return this;

    int cnt = m_rubrics.count();
    for(int i=0; i<cnt; i++){
        KKSRubric * r = const_cast<KKSRubric*>(m_rubrics[i]);
        if(r->id() == id){
            return r;
        }
        else if(recursivelly){
            KKSRubric * r1 = r->rubricForId(id, recursivelly);
            if(r1)
                return r1;
        }
    }

    return NULL;
}

const KKSRubricItem * KKSRubric::itemForId(int id) const
{
    if(id <= 0)
        return NULL;

    int cnt = m_items.count();
    for(int i=0; i<cnt; i++){
        const KKSRubricItem * item = m_items.at(i);
        if(item->id() == id){
            return item;
        }
    }

    return NULL;
}

KKSRubricItem * KKSRubric::itemForId(int id)
{
    if(id <= 0)
        return NULL;

    int cnt = m_items.count();
    for(int i=0; i<cnt; i++){
        KKSRubricItem * item = const_cast<KKSRubricItem*>(m_items[i]);
        if(item->id() == id){
            return item;
        }
    }

    return NULL;
}

void KKSRubric::setIcon(const QPixmap & px)
{
    if(pxRubric)
        delete pxRubric;

    pxRubric = new QPixmap(px);
}

QPixmap KKSRubric::icon()
{
    QPixmap px;
    
    if(pxRubric)
        px = QPixmap(*pxRubric);

    return px;
}

KKSRubric * KKSRubric::deepCopy(bool dropIds) const
{
    KKSRubric * copy = new KKSRubric(this->id(), this->name());
    copy->setIcon(this->icon());
    if(dropIds)
        copy->setId(-1);

    KKSList<const KKSRubric *> subRubrics = this->rubrics();
    for(int i=0; i<subRubrics.count(); i++){
        KKSRubric * r = subRubrics.at(i)->deepCopy(dropIds);//deep copy
        copy->addRubric(r);
        r->release();
    }

    KKSList<const KKSRubricItem *> items = this->items();
    for(int i=0; i<items.count(); i++){
        KKSRubricItem * item = new KKSRubricItem(*(items.at(i)));
        copy->addItem(item);
        item->release();
    }

    return copy;
}

KKSSearchTemplate * KKSRubric::getSearchTemplate (void) const
{
    return m_searchTemplate;
}

void KKSRubric::setSearchTemplate (KKSSearchTemplate * st)
{
    if (m_searchTemplate)
        m_searchTemplate->release ();

    m_searchTemplate = st;

    if (m_searchTemplate)
        m_searchTemplate->addRef ();
}

KKSCategory * KKSRubric::getCategory (void) const
{
    return m_category;
}

void KKSRubric::setCategory (KKSCategory *c)
{
    if (m_category)
        m_category->release ();

    m_category = c;

    if (m_category)
        m_category->addRef ();
}

KKSAccessEntity * KKSRubric::getAccessRules (void) const
{
    return m_acl;
}

void KKSRubric::setAccessRules (KKSAccessEntity * _acl)
{
    if (m_acl)
        m_acl->release ();

    m_acl = _acl;

    if (m_acl)
        m_acl->addRef ();
}
/*
const KKSMap<int, KKSPrivilege *> & KKSRubric::privileges() const
{
    return m_privileges;
}

KKSMap<int, KKSPrivilege *> & KKSRubric::privileges()
{
        return m_privileges;
}

void KKSRubric::setPrivileges(const KKSMap<int, KKSPrivilege *> & _privileges)
{
    m_privileges = _privileges;
}

int KKSRubric::addPrivilege(int idRole, KKSPrivilege * p)
{
    int cnt = m_privileges.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSRubric::removePrivilege(int idRole)
{
    m_privileges.remove(idRole);
    return OK_CODE;
}

int KKSRubric::replacePrivilege(int idRole, KKSPrivilege * p)
{
    removePrivilege(idRole);
    int cnt = addPrivilege(idRole, p);

    return cnt == 0 ? ERROR_CODE : OK_CODE;
}

const KKSPrivilege * KKSRubric::privilege(int idRole) const
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

KKSPrivilege * KKSRubric::privilege(int idRole)
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

KKSMap<int, KKSPrivilege*> & KKSRubric::bossPrivileges()
{
    return m_bossPrivileges;
}

const KKSMap<int, KKSPrivilege *> & KKSRubric::bossPrivileges() const
{
    return m_bossPrivileges;
}

KKSPrivilege * KKSRubric::bossPrivilege(int idRole)
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

const KKSPrivilege * KKSRubric::bossPrivilege(int idRole) const
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

KKSMap<int, KKSPrivilege *> & KKSRubric::unitPrivileges()
{
    return m_unitPrivileges;
}

const KKSMap<int, KKSPrivilege *> & KKSRubric::unitPrivileges() const
{
        return m_unitPrivileges;
}

KKSPrivilege * KKSRubric::unitPrivilege(int idRole)
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

const KKSPrivilege * KKSRubric::unitPrivilege(int idRole) const
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

KKSPrivilege * KKSRubric::othersPrivilege()
{
    return m_othersPrivilege;
}

KKSPrivilege * KKSRubric::othersPrivilege() const
{
    return m_othersPrivilege;
}

int KKSRubric::setBossPrivilege(int idRole, KKSPrivilege * p)
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

int KKSRubric::setUnitPrivilege(int idRole, KKSPrivilege * p)
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

int KKSRubric::addBossPrivilege(int idRole, KKSPrivilege * p)
{
    int cnt = m_bossPrivileges.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSRubric::addUnitPrivilege(int idRole, KKSPrivilege * p)
{
    int cnt = m_unitPrivileges.insert(idRole, p);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

void KKSRubric::setOthersPrivilege(KKSPrivilege * p)
{
    if(m_othersPrivilege)
        m_othersPrivilege->release();

    m_othersPrivilege = p;

    if(m_othersPrivilege)
        m_othersPrivilege->addRef();
}
*/
QString KKSRubric::getFullTreeOfIdsString() const
{
    QString ids;
    ids = QString::number(this->id());

    for(int i=0; i<m_rubrics.count(); i++){
        const KKSRubric * r = m_rubrics.at(i); 
        if(r){
            ids += ", " + r->getFullTreeOfIdsString();
        }
    }

    return ids;
}

//рассматривать удаленные подрубрики удаленных рубрик нет необходимости, 
//поскольку серверная функция гарантирует удаление вложенных подрубрик и вложений удаляемой рубрики
QString KKSRubric::getFullTreeOfDeletedIds() const
{
    QString ids;
    //ids = QString::number(this->id());

    for(int i=0; i<m_deletedRubrics.count(); i++){
        const KKSRubric * r = m_deletedRubrics.at(i); 
        if(r){
            if(ids.isEmpty())
                ids += QString::number(r->id());
            else
                ids += ", " + QString::number(r->id());
        }
    }

    for(int i=0; i<m_rubrics.count(); i++){
        const KKSRubric * r = m_rubrics.at(i); 
        if(r){
            QString idsTmp = r->getFullTreeOfDeletedIds();
            if(idsTmp.isEmpty())
                continue;

            if(ids.isEmpty())
                ids += idsTmp;
            else
                ids += ", " + idsTmp;
        }
    }

    return ids;
}

QIcon KKSRubric :: getIcon (void) const
{
    return m_rubricIcon;
}

void KKSRubric :: setIcon (const QString & s)
{
    QPixmap px;
    bool isOk = px.loadFromData(s.toUtf8(), "XPM");
    if (!isOk)
        return;

    m_rubricIcon = QIcon(px);
    m_iconData = s;
}

const QString KKSRubric :: iconAsString () const
{
    return m_iconData;
}

bool KKSRubric :: isCategorized (void) const
{
    return m_isCategorized;
}

void KKSRubric :: setCategorized (bool c)
{
    m_isCategorized = c;
}