/***********************************************************************
 * Module:  KKSAttribute.cpp
 * Author:  sergey
 * Modified: 25 ноября 2008 г. 17:39:56
 * Purpose: Implementation of the class KKSAttribute
 ***********************************************************************/

#include "KKSAttrType.h"
#include "KKSAttribute.h"
#include "KKSSearchTemplate.h"
#include "defines.h"
////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::KKSAttribute()
// Purpose:    Implementation of KKSAttribute::KKSAttribute()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttribute::KKSAttribute() : KKSIndAttr(),
    m_title (QString()),
    m_defWidth (100),
    m_group (0),
    m_st (0),
    m_isSystem (iacIOUserAttr),
    m_attrsLoaded(false)
{
//   m_refType = NULL;
//   m_refColumnType = NULL;
}

KKSAttribute::KKSAttribute(const KKSAttribute & a) : KKSIndAttr(a),
    m_title (a.m_title),
    m_defWidth (a.m_defWidth),
    m_group (a.m_group),
    m_st (a.m_st),
    m_isSystem (a.m_isSystem),
    m_attrs(a.m_attrs),
    m_attrsLoaded(a.m_attrsLoaded)
{
    if (m_group)
        m_group->addRef ();

    if (m_st)
        m_st->addRef ();

}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::~KKSAttribute()
// Purpose:    Implementation of KKSAttribute::~KKSAttribute()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttribute::~KKSAttribute()
{

   if(m_group)
       m_group->release();


   if(m_st)
       m_st->release();

}

const QString & KKSAttribute::title(void) const
{
   return m_title;
}

void KKSAttribute::setTitle(const QString & newTitle)
{
   m_title = newTitle;
}

const KKSAGroup * KKSAttribute::group() const
{
    return m_group;
}

KKSAGroup * KKSAttribute::group()
{
    return m_group;
}

void KKSAttribute::setGroup(KKSAGroup * _group)
{
    if(m_group)
        m_group->release();

    m_group = _group;
    if(m_group)
        m_group->addRef();
}

const KKSSearchTemplate * KKSAttribute::searchTemplate() const
{
    return m_st;
}

KKSSearchTemplate * KKSAttribute::searchTemplate()
{
    return m_st;
}

void KKSAttribute::setSearchTemplate(KKSSearchTemplate* st)
{
    if(m_st)
        m_st->release();

    m_st = st;
    if(m_st)
        m_st->addRef();
}

int KKSAttribute::defWidth() const
{
    return m_defWidth;
}

void KKSAttribute::setDefWidth(int w)
{
    if(w > 0)
        m_defWidth = w;
    else 
        m_defWidth = 100;
}

KKSIndAttr::KKSIndAttrClass KKSAttribute::isSystem() const
{
    return m_isSystem;
}

void KKSAttribute::setAsSystem(KKSIndAttr::KKSIndAttrClass yes)
{
    m_isSystem = yes;
}

bool KKSAttribute::isAttribute (void) const
{
    return true;
}


void KKSAttribute::setAttrs(const KKSMap<int, KKSCategoryAttr *> & attrs)
{
    m_attrs = attrs;
    m_attrsLoaded = true;
}

const KKSMap<int, KKSCategoryAttr*> & KKSAttribute::attrs() const
{
    return m_attrs;
}

KKSMap<int, KKSCategoryAttr*> & KKSAttribute::attrs()
{
    return m_attrs;
}

const KKSCategoryAttr * KKSAttribute::attrAttr(int id) const
{
    KKSCategoryAttr * a = NULL;

     KKSMap<int, KKSCategoryAttr *>::const_iterator i = m_attrs.find(id);
     while (i != m_attrs.end() && i.key() == id) {
         a = i.value();
         //берем всегда первый атрибут
         break;
     }

    return a;
}

KKSCategoryAttr * KKSAttribute::attrAttr(int id)
{
    KKSCategoryAttr * a = NULL;

     QMap<int, KKSCategoryAttr *>::iterator i = m_attrs.find(id);
     while (i != m_attrs.end() && i.key() == id) {
         a = i.value();
         //берем всегда первый атрибут
         break;
     }

    return a;
}


/**************************************
****************KKSAGroup**************
***************************************/

KKSAGroup::KKSAGroup() : KKSRecord()
{
    m_parent = 0;
}

KKSAGroup::KKSAGroup(const KKSAGroup & g) : KKSRecord(g)
{
    setParent(const_cast<KKSAGroup*>(g.parent()));
}

KKSAGroup::KKSAGroup(int id, const QString & name) : KKSRecord(id, name)
{
    m_parent = 0;
}

KKSAGroup::~KKSAGroup()
{
    if(m_parent)
        m_parent->release();
}


// child groups
void KKSAGroup::setChildGroups(const KKSMap<int, KKSAGroup *> & gl)
{
    m_childGroups = gl;
}

const KKSMap<int, KKSAGroup *> & KKSAGroup::childGroups() const
{
    return m_childGroups;
}


int KKSAGroup::addChildGroup(int idGroup, KKSAGroup * g)
{
    int cnt = m_childGroups.insert(idGroup, g);
    if(!cnt)
        return ERROR_CODE;

    return OK_CODE;
}

int KKSAGroup::removeChildGroup(int idGroup)
{
    m_childGroups.remove(idGroup);
    return OK_CODE;
}
// end of child groups

void KKSAGroup::setParent(KKSAGroup * g)
{
    if(m_parent)
        m_parent->release();

    m_parent = g;
    if(m_parent)
        m_parent->addRef();
}

KKSAGroup * KKSAGroup::parent()
{
    return m_parent;
}

const KKSAGroup * KKSAGroup::parent() const 
{
    return m_parent;
}


const KKSAGroup * KKSAGroup :: childGroupForId (int id, bool recursive) const
{
    if (this->id() == id)
        return this;

    KKSMap<int, KKSAGroup *>::const_iterator pg = m_childGroups.constFind (id);
    if (pg != m_childGroups.constEnd())
        return pg.value();
    else if (recursive)
    {
        const KKSAGroup * gr=0;
        for (pg=m_childGroups.constBegin(); pg != m_childGroups.constEnd() && gr==0; pg++)
        {
            gr = pg.value()->childGroupForId (id, recursive);//pg.key());
        }
        return gr;
    }

    return 0;
}

KKSAGroup * KKSAGroup :: childGroupForId (int id, bool recursive)
{
    if (this->id() == id)
        return this;

    KKSMap<int, KKSAGroup *>::const_iterator pg = m_childGroups.constFind (id);
    if (pg != m_childGroups.constEnd())
        return pg.value();
    else if (recursive)
    {
        KKSAGroup * gr=0;
        for (pg=m_childGroups.constBegin(); pg != m_childGroups.constEnd() && gr==0; pg++)
        {
            gr = pg.value()->childGroupForId (id, recursive);
        }
        return gr;
    }

    return 0;
}

QList<int> KKSAGroup :: childKeys (void) const
{
    QList<int> gKeys = m_childGroups.keys();
    for (KKSMap<int, KKSAGroup *>::const_iterator pg=m_childGroups.constBegin(); pg != m_childGroups.constEnd(); pg++)
        gKeys += pg.value()->childKeys();

    return gKeys;
}

void KKSAGroup :: setAttributes (const KKSMap<int, KKSAttribute*>& attrs)
{
    m_Attrs = attrs;
}

const KKSMap<int, KKSAttribute*> & KKSAGroup :: getAttributes (void) const
{
    return m_Attrs;
}

int KKSAGroup :: addAttribute (int idAttr, KKSAttribute * a)
{
    if (a)
        m_Attrs.insert (idAttr, a);
    return OK_CODE;
}

int KKSAGroup :: removeAttribute (int idAttr)
{
    m_Attrs.remove(idAttr);
    return OK_CODE;
}

