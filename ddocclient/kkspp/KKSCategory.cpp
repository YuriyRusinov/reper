/***********************************************************************
 * Module:  KKSCategory.cpp
 * Author:  sergey
 * Modified: 27 ноября 2008 г. 12:03:27
 * Purpose: Implementation of the class KKSCategory
 ***********************************************************************/

#include "KKSType.h"
#include "KKSCategoryAttr.h"
#include "KKSLifeCycle.h"
#include "KKSCategory.h"
#include "KKSAttrType.h"
#include "KKSRubric.h"
#include "KKSState.h"
#include "KKSAccessEntity.h"

#include <QtDebug>
#include <QStringList>

#include "defines.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSCategory::KKSCategory()
// Purpose:    Implementation of KKSCategory::KKSCategory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSCategory::KKSCategory() : KKSRecord(),
    m_isMain (true),
    m_type (NULL),
    m_tableCategory (NULL),
    m_recAttrCategory(NULL),
    m_lifeCycle (KKSLifeCycle::defLifeCycle()),
    m_isSystem(false),
    m_isGlobal(false),
    m_rootRubric(NULL),
    m_state (KKSState::defState1()),
    m_acl (new KKSAccessEntity())
{
    //в будущем наверное требуется изменить параметр на FALSE
    //сейчас же при изменении категории всегда полагается, 
    //что изменены все ее характеристики
    setModified(false);
}

KKSCategory::KKSCategory(const KKSCategory & c) : KKSRecord(c),
    m_isMain (c.m_isMain),
    m_attributes (c.m_attributes),
    m_attrsModified(c.m_attrsModified),
    m_isSystem(c.isSystem()),
    m_isGlobal(c.isGlobal()),
    m_rootRubric(NULL),
    m_state (NULL),
    m_acl (c.m_acl)
{
    m_type = NULL;
    m_tableCategory = NULL;
    m_recAttrCategory = NULL;
    m_lifeCycle = NULL;
    if (m_acl)
        m_acl->addRef ();

    setType(const_cast<KKSType *>(c.type()));

    setState(const_cast<KKSState*>(c.state()));
    
    setTableCategory(const_cast<KKSCategory *>(c.tableCategory()));
    setRecAttrCategory(const_cast<KKSCategory *>(c.recAttrCategory()));

    setLifeCycle(const_cast<KKSLifeCycle*>(c.lifeCycle()));

    setRootRubric(c.rootRubric());
}

KKSCategory::KKSCategory(int id, const QString & name, KKSType * type) : KKSRecord(id, name),
    m_isMain (true),
    m_type (NULL),
    m_tableCategory (NULL),
    m_recAttrCategory(NULL),
    m_lifeCycle (KKSLifeCycle::defLifeCycle()),
    m_isSystem(false),
    m_isGlobal(false),
    m_rootRubric(NULL),
    m_state (KKSState::defState1()),
    m_acl ((type->id() != 10 ? new KKSAccessEntity() : 0))
{
    setType(type);

    //в будущем наверное требуется изменить параметр на FALSE
    //сейчас же при изменении категории всегда полагается, 
    //что изменены все ее характеристики
    setModified(false);
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSCategory::~KKSCategory()
// Purpose:    Implementation of KKSCategory::~KKSCategory()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSCategory::~KKSCategory()
{
    if(m_type)
        m_type->release();

    if(m_tableCategory)
        m_tableCategory->release();

    if(m_recAttrCategory)
        m_recAttrCategory->release();

    if(m_lifeCycle)
        m_lifeCycle->release();

    if(m_rootRubric)
        m_rootRubric->release();

    if(m_state)
        m_state->release();

    if (m_acl)
        m_acl->release ();

}

const KKSState * KKSCategory::state() const
{
    return m_state;
}

KKSState * KKSCategory::state()
{
    return m_state;
}

void KKSCategory::setState(KKSState * state)
{
    if(m_state)
        m_state->release();

    m_state = state;
    if(m_state)
        m_state->addRef();
}

const KKSCategory * KKSCategory::tableCategory() const
{
    return m_tableCategory;
}

KKSCategory * KKSCategory::tableCategory()
{
    return m_tableCategory;
}

const KKSCategory * KKSCategory::recAttrCategory() const
{
    return m_recAttrCategory;
}

KKSCategory * KKSCategory::recAttrCategory()
{
    return m_recAttrCategory;
}

const KKSType * KKSCategory::type() const
{
    return m_type;
}

KKSType * KKSCategory::type()
{
    return m_type;
}

const KKSMap<int, KKSCategoryAttr *> & KKSCategory::attributes() const
{
    return m_attributes;
}

KKSMap<int, KKSCategoryAttr *> & KKSCategory::attributes()
{
    return m_attributes;
}

const KKSLifeCycle * KKSCategory::lifeCycle() const
{
    return m_lifeCycle;
}

KKSLifeCycle * KKSCategory::lifeCycle()
{
    return m_lifeCycle;
}

void KKSCategory::setTableCategory(KKSCategory * _tableCategory)
{
    if(m_tableCategory && m_tableCategory != _tableCategory )
        m_tableCategory->release();

    m_tableCategory = _tableCategory;
    if(m_tableCategory)
        m_tableCategory->addRef();
}

void KKSCategory::setRecAttrCategory(KKSCategory * _recAttrCategory)
{
    if(m_recAttrCategory && m_recAttrCategory != _recAttrCategory)
        m_recAttrCategory->release();

    m_recAttrCategory = _recAttrCategory;
    
    if(m_recAttrCategory)
        m_recAttrCategory->addRef();
}

void KKSCategory::setType(KKSType * _type)
{
    if(m_type)
        m_type->release();

    m_type = _type;
    if(m_type)
        m_type->addRef();
}

void KKSCategory::setAttributes(const KKSMap<int, KKSCategoryAttr *> & _attributes)
{
    m_attributes = _attributes;
    m_attrsModified = true;
}

void KKSCategory::setLifeCycle(KKSLifeCycle * _lifeCycle)
{
    if(m_lifeCycle)
        m_lifeCycle->release();
    
    m_lifeCycle = _lifeCycle;
    if(m_lifeCycle)
        m_lifeCycle->addRef();
}

const KKSCategoryAttr * KKSCategory::attribute(int id) const
{
    KKSCategoryAttr * a = NULL;

     QMap<int, KKSCategoryAttr *>::const_iterator i = m_attributes.find(id);
     while (i != m_attributes.end() && i.key() == id) {
         a = i.value();
         //берем всегда первый атрибут
         break;
     }

    return a;
}

KKSCategoryAttr * KKSCategory::attribute(int id)
{
    KKSCategoryAttr * a = NULL;

     QMap<int, KKSCategoryAttr *>::iterator i = m_attributes.find(id);
     while (i != m_attributes.end() && i.key() == id) {
         a = i.value();
         //берем всегда первый атрибут
         break;
     }

    return a;
}

KKSCategoryAttr * KKSCategory::attribute(const QString & code)
{
    KKSCategoryAttr * a = NULL;

    bool bFound = false;
    KKSMap<int, KKSCategoryAttr *>::iterator pca;
    for (pca = m_attributes.begin(); pca != m_attributes.end(); pca++)
    {
        a = pca.value();
        if(a && a->code() == code){
            bFound = true;
            break;
        }
    }
    
    if(!bFound)
        return NULL;

    return a;
}

const KKSCategoryAttr * KKSCategory::attribute(const QString & code) const
{
    KKSCategoryAttr * a = NULL;

    bool bFound = false;
    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca = m_attributes.constBegin(); pca != m_attributes.constEnd(); pca++)
    {
        a = pca.value();
        if(a && a->code() == code){
            bFound = true;
            break;
        }
    }
    
    if(!bFound)
        return NULL;

    return a;
}

int KKSCategory::addAttribute(int id, KKSCategoryAttr * a)
{
    int cnt = m_attributes.insert(id, a);
    if(!cnt)
        return ERROR_CODE;

    m_attrsModified = true;
    return OK_CODE;
}

int KKSCategory::removeAttribute(int id)
{
    m_attributes.remove(id);
    m_attrsModified = true;
    return OK_CODE;
}
/*
int KKSCategory::removeAttribute(const QString & code)
{
    KKSAttribute * a = attribute(code);
    if(!a)
        return ERROR_CODE;

    m_attributes.remove(a->id());
    
    m_attrsModified = true;
    return OK_CODE;
}
*/

int KKSCategory::replaceAttribute(int id, KKSCategoryAttr * a)
{
    removeAttribute(id);
    int cnt = addAttribute(id, a);

    m_attrsModified = true;
    return cnt == 0 ? ERROR_CODE : OK_CODE;
}

bool KKSCategory::isMain() const
{
    return m_isMain;
}

void KKSCategory::setMain(bool isMain)
{
    m_isMain = isMain;
}

const KKSTemplate & KKSCategory::defTemplate() const
{

    m_defTemplate.setCategory(NULL);
    QString tName = QObject::tr("Base template for category %1").arg(name());
    m_defTemplate = KKSTemplate(-1, tName, const_cast<KKSCategory *>(this));
    KKSAttrGroup * g = new KKSAttrGroup();
    g->setOrder(0);
    g->setName("default group");
    g->setId(-1);
    
    KKSMap<int, KKSAttrView *> a;
    int order = 0;
    KKSMap<int, KKSCategoryAttr *>::const_iterator pca;
    for (pca = m_attributes.constBegin(); pca != m_attributes.constEnd(); pca++)
    {
        KKSCategoryAttr * ca = pca.value();
        KKSAttrView * av = new KKSAttrView(*ca);
        av->setOrder(order++);
        a.insert(ca->id(), av);
        av->release();
    }

    g->setAttrViews(a);
    m_defTemplate.addGroup(g);
    g->release();

    return m_defTemplate;
}

KKSFilter * KKSCategory::createFilter(int attrId, 
                                      const QString & value, 
                                      KKSFilter::FilterOper operation)
{
    KKSFilter * f = NULL;
    KKSAttribute * a = attribute(attrId);
    if(!a){
        qWarning() << __PRETTY_FUNCTION__ << "attribute does not exist!";
        return f;
    }

    KKSValue * v = NULL;
    //для указанных операций значение не требуется. 
    //В этом случае просто опускаем его
    if(operation != KKSFilter::foIsNotNull &&
       operation != KKSFilter::foIsNull )
    {
        if(operation != KKSFilter::foInSQL)
            v = new KKSValue(value, a->type()->attrType());
        else
            v = new KKSValue(value, KKSAttrType::atString);
        
        if(!v->isValid()){
            v->release();
            qWarning() << __PRETTY_FUNCTION__ << "KKSValue is not valid!";
            return f;
        }
    }

    f = new KKSFilter(a, v, operation);
    if(!f->isCorrect()){
        f->release();
        qWarning() << __PRETTY_FUNCTION__ << "KKSFilter is not correct!";
        return NULL;
    }

    return f;
}

KKSFilter * KKSCategory::createFilter(int attrId, 
                                      const QStringList & values, 
                                      KKSFilter::FilterOper operation)
{
    KKSFilter * f = NULL;
    KKSAttribute * a = attribute(attrId);
    if(!a){
        qWarning() << __PRETTY_FUNCTION__ << "attribute does not exist!";   
        return f;
    }

    f = new KKSFilter();

    int cnt = values.count();
    for(int i=0; i<cnt; i++)
    {
        QString value = values.at(i);
        KKSValue * v = new KKSValue(value, a->type()->attrType());
        if(!v->isValid()){
            v->release();
            f->release();
            qWarning() << __PRETTY_FUNCTION__ << "KKSValue is not valid!";
            return NULL;
        }
        f->addValue(v);
    }

    
    f->setAttribute(a);
    f->setOperation( operation);

    if(!f->isCorrect()){
        f->release();
        qWarning() << __PRETTY_FUNCTION__ << "KKSFilter is not correct!";
        return NULL;
    }

    return f;
}

/*
KKSFilter * KKSCategory::createFilter(const QString & attrCode, 
                                      const QString & value, 
                                      KKSFilter::FilterOper operation)
{
    KKSFilter * f = NULL;
    KKSAttribute * a = attribute(attrCode);
    if(!a){
        qWarning() << __PRETTY_FUNCTION__ << "attribute does not exist!";
        return f;
    }

    KKSValue * v = NULL;
    //для указанных операций значение не требуется. 
    //В этом случае просто опускаем его
    if(operation != KKSFilter::foIsNotNull &&
        operation != KKSFilter::foIsNull)
    {
        v = new KKSValue(value, a->type()->attrType());
        if(!v->isValid()){
            v->release();
            qWarning() << __PRETTY_FUNCTION__ << "KKSValue is not valid!";
            return f;
        }
    }

    f = new KKSFilter(a, v, operation);
    if(!f->isCorrect()){
        f->release();
        qWarning() << __PRETTY_FUNCTION__ << "KKSFilter is not correct!";
        return NULL;
    }

    return f;
}

KKSFilter * KKSCategory::createFilter(const QString & attrCode, 
                                      const QStringList & values, 
                                      KKSFilter::FilterOper operation)
{
    KKSFilter * f = NULL;
    KKSAttribute * a = attribute(attrCode);
    if(!a){
        qWarning() << __PRETTY_FUNCTION__ << "attribute does not exist!";   
        return f;
    }

    f = new KKSFilter();

    int cnt = values.count();
    for(int i=0; i<cnt; i++)
    {
        QString value = values.at(i);
        KKSValue * v = new KKSValue(value, a->type()->attrType());
        if(!v->isValid()){
            v->release();
            f->release();
            qWarning() << __PRETTY_FUNCTION__ << "KKSValue is not valid!";
            return NULL;
        }
        f->addValue(v);
    }

    
    f->setAttribute(a);
    f->setOperation( operation);

    if(!f->isCorrect()){
        f->release();
        qWarning() << __PRETTY_FUNCTION__ << "KKSFilter is not correct!";
        return NULL;
    }

    return f;
}
*/


void KKSCategory::setModified(bool yes)
{
    m_attrsModified = yes;
}

bool KKSCategory::isSystem() const
{
    return m_isSystem;
}

void KKSCategory::setAsSystem(bool yes)
{
    m_isSystem = yes;
}

bool KKSCategory::isGlobal() const
{
    return m_isGlobal;
}

void KKSCategory::setAsGlobal(bool yes)
{
    m_isGlobal = yes;
}

void KKSCategory::setRootRubric(KKSRubric * r)
{
    if(m_rootRubric)
        m_rootRubric->release();

    m_rootRubric = r;

    if(m_rootRubric)
        m_rootRubric->addRef();
}

KKSRubric * KKSCategory::rootRubric() const
{
    return m_rootRubric;
}

/*
KKSValue KKSCategory::replicatedOrgsV() const
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

const QList<int> & KKSCategory::replicatedOrgs() const
{
    return m_replicatedOrgs;
}

int KKSCategory::addReplicatedOrg(int id)
{
    m_replicatedOrgs.append(id);

    return OK_CODE;
}

int KKSCategory::removeReplicatedOrg(int id)
{
    int cnt = m_replicatedOrgs.removeAll(id);
    if(cnt == 0)
        return ERROR_CODE;

    return OK_CODE;
}

void KKSCategory::setReplicatedOrgs(const QList<int> & orgs)
{
    m_replicatedOrgs = orgs;
}

void KKSCategory::setReplicatedOrgsV(const KKSValue & v)
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
*/

QList<int> KKSCategory :: searchAttributesByType (int aType) const
{
    QList<int> results;
    KKSMap<int, KKSCategoryAttr *>::const_iterator pa = m_attributes.constBegin();
    for (; pa != m_attributes.constEnd(); pa++)
        if (pa.value()->type()->attrType() == (KKSAttrType::KKSAttrTypes)aType)
            results.append (pa.key());

    return results;
}

bool KKSCategory :: isAttrTypeContains (KKSAttrType::KKSAttrTypes type) const
{
    bool isC = false;
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator pa = m_attributes.constBegin();
         pa != m_attributes.constEnd() && !isC;
         pa++)
        isC = (pa.value()->type()->attrType() == type);

    return isC;
}

const KKSAccessEntity * KKSCategory :: getAccessRules (void) const
{
    return m_acl;
}

KKSAccessEntity * KKSCategory :: getAccessRules (void)
{
    return m_acl;
}

void KKSCategory :: setAccessRules (KKSAccessEntity * acl)
{
    if (m_acl)
        m_acl->release ();

    m_acl = acl;

    if (m_acl)
        m_acl->addRef ();
}
