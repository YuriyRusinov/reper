/***********************************************************************
 * Module:  KKSAttribute.cpp
 * Author:  sergey
 * Modified: 25 но€бр€ 2008 г. 17:39:56
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
    m_isSystem (false)
{
//   m_refType = NULL;
//   m_refColumnType = NULL;
}

KKSAttribute::KKSAttribute(const KKSAttribute & a) : KKSIndAttr(a),
    m_title (a.m_title),
    m_defWidth (a.m_defWidth),
    m_group (a.m_group),
    m_st (a.m_st),
    m_isSystem (a.m_isSystem)
{
//    m_refType = NULL;
//    m_refColumnType = NULL;
//    m_st = NULL;
//    setType(const_cast<KKSAttrType*>(a.type()));
//    setGroup(const_cast<KKSAGroup*>(a.group()));
    if (m_group)
        m_group->addRef ();
//    setRefType(const_cast<KKSAttrType*>(a.refType()));
//    setRefColumnType(const_cast<KKSAttrType*>(a.refColumnType()));
//    setSearchTemplate(const_cast<KKSSearchTemplate*>(a.searchTemplate()));
    if (m_st)
        m_st->addRef ();

//    m_title = a.title();
//    m_tableName = a.tableName();
//    setColumnName(a.columnName());
//    setRefColumnName(a.refColumnName());
//    //m_columnName = a.columnName();
//    //m_quotedColumnName = a.columnName(true);
//    m_defWidth = a.defWidth();
//    m_isSystem = a.isSystem();
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::~KKSAttribute()
// Purpose:    Implementation of KKSAttribute::~KKSAttribute()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttribute::~KKSAttribute()
{
//   if(m_type)
//       m_type->release();

   if(m_group)
       m_group->release();

//   if(m_refType)
//       m_refType->release();

//   if(m_refColumnType)
//       m_refColumnType->release();

   if(m_st)
       m_st->release();

}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::getColumnName()
// Purpose:    Implementation of KKSAttribute::getColumnName()
// Return:     QString
////////////////////////////////////////////////////////////////////////
/*
const QString & KKSAttribute::columnName(bool quoted) const
{
    if(quoted){
        if(m_quotedColumnName.isEmpty() && !m_columnName.isEmpty())
            m_quotedColumnName = QString("\"") + m_columnName + QString("\"");

        return m_quotedColumnName;
    }
   
   return m_columnName;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::setColumnName(QString newColumnName)
// Purpose:    Implementation of KKSAttribute::setColumnName()
// Parameters:
// - newColumnName
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSAttribute::setColumnName(const QString & newColumnName)
{
    m_columnName = newColumnName;
    if(!m_columnName.isEmpty())
        m_quotedColumnName = QString("\"") + m_columnName + QString("\"");
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::getRefColumnName()
// Purpose:    Implementation of KKSAttribute::getColumnName()
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSAttribute::refColumnName(bool quoted) const
{
    if(quoted){
        if(m_quotedRefColumnName.isEmpty() && !m_refColumnName.isEmpty())
            m_quotedRefColumnName = QString("\"") + m_refColumnName + QString("\"");

        return m_quotedRefColumnName;
    }
   
   return m_refColumnName;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::setColumnName(QString newColumnName)
// Purpose:    Implementation of KKSAttribute::setColumnName()
// Parameters:
// - newColumnName
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSAttribute::setRefColumnName(const QString & newColumnName)
{
    m_refColumnName = newColumnName;
    if(!m_refColumnName.isEmpty())
        m_quotedRefColumnName = QString("\"") + m_refColumnName + QString("\"");
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::getTableName()
// Purpose:    Implementation of KKSAttribute::getTableName()
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSAttribute::tableName(void) const
{
   return m_tableName;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::setTableName(QString newTableName)
// Purpose:    Implementation of KKSAttribute::setTableName()
// Parameters:
// - newTableName
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSAttribute::setTableName(const QString & newTableName)
{
   m_tableName = newTableName;
}
*/
////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::getTitle()
// Purpose:    Implementation of KKSAttribute::getTitle()
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSAttribute::title(void) const
{
   return m_title;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttribute::setTitle(QString newTitle)
// Purpose:    Implementation of KKSAttribute::setTitle()
// Parameters:
// - newTitle
// Return:     void
////////////////////////////////////////////////////////////////////////

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

bool KKSAttribute::isSystem() const
{
    return m_isSystem;
}

void KKSAttribute::setAsSystem(bool yes)
{
    m_isSystem = yes;
}

bool KKSAttribute::isAttribute (void) const
{
    return true;
}

/*
#ifdef Q_WS_WIN
		//for web 
std::string KKSAttribute::columnName_w(void) const
{
	std::string s;
    s = columnName().toUtf8();
    return s;
}
void KKSAttribute::setColumnName_w(std::string & newColumnName)
{
	return setColumnName(QString::fromUtf8(newColumnName.c_str()));
}
std::string KKSAttribute::tableName_w(void) const
{
	std::string s;
    s = tableName().toUtf8();
    return s;
}
void KKSAttribute::setTableName(std::string & newTableName)
{
	return setTableName(QString::fromUtf8(newTableName.c_str()));
}
std::string KKSAttribute::title_w(void) const
{
	std::string s;
    s = title().toUtf8();
    return s;
}
void KKSAttribute::setTitle_w(std::string & newTitle)
{
	return setTitle(QString::fromUtf8(newTitle.c_str()));
}
#endif
*/

/*----------------------------*/

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

