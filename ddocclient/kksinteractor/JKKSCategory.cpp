#include <QDataStream>
#include <QTextStream>
#include <QtDebug>

#include "JKKSLoader.h"
#include "JKKSCategoryAttr.h"
#include "JKKSCategory.h"

JKKSCategory :: JKKSCategory (int idCat, 
                              const QString& cName, 
                              const QString& cCode, 
                              const QString& cDesc, 
                              int type,
                              int id_child, 
                              bool isCMain,
                              const QString & uid,
                              int state,
                              bool isCGlobal)
    : JKKSUID(uid, QString()),
    id_io_category (idCat),
    m_catName (cName),
    m_catCode (cCode),
    m_catDesc (cDesc),
    m_idType (type),
    m_idState(state),
    m_idChild (id_child),
    m_isMain (isCMain),
    m_isGlobal (isCGlobal)
{
}

JKKSCategory :: JKKSCategory (const JKKSCategory& cat)
    : JKKSUID(cat),//.m_uid),
    id_io_category (cat.id_io_category),
    m_catName (cat.m_catName),
    m_catCode (cat.m_catCode),
    m_catDesc (cat.m_catDesc),
    m_idType (cat.m_idType),
    m_idState (cat.m_idState),
    m_idChild (cat.m_idChild),
    m_isMain (cat.m_isMain),
    m_isGlobal (cat.m_isGlobal),
    m_attributes (cat.m_attributes),
    m_rubrics (cat.m_rubrics)
{
}

JKKSCategory :: ~JKKSCategory (void)
{
}

int JKKSCategory :: id (void) const
{
    return id_io_category;
}

void JKKSCategory :: setId (int idCat)
{
    id_io_category = idCat;
}

QString JKKSCategory :: getName (void) const
{
    return m_catName;
}

void JKKSCategory :: setName (const QString& cname)
{
    m_catName = cname;
}

QString JKKSCategory :: getCode (void) const
{
    return m_catCode;
}

void JKKSCategory :: setCode (const QString& code)
{
    m_catCode = code;
}

QString JKKSCategory :: getDescription (void) const
{
    return m_catDesc;
}

void JKKSCategory :: setDescription (const QString& desc)
{
    m_catDesc = desc;
}

int JKKSCategory :: getType (void) const
{
    return m_idType;
}

void JKKSCategory :: setType (int type)
{
    m_idType = type;
}

int JKKSCategory :: getState (void) const
{
    return m_idState;
}

void JKKSCategory :: setState (int state)
{
    m_idState = state;
}

int JKKSCategory :: getIDChild (void) const
{
    return m_idChild;
}

void JKKSCategory :: setIDChild (int idc)
{
    m_idChild = idc;
}

bool JKKSCategory :: isMain (void) const
{
    return m_isMain;
}

void JKKSCategory :: setMain (bool val)
{
    m_isMain = val;
}

bool JKKSCategory :: isGlobal (void) const
{
    return m_isGlobal;
}

void JKKSCategory :: setGlobal (bool val)
{
    m_isGlobal = val;
}

const QMap<int, JKKSCategoryAttr >& JKKSCategory :: attributes (void) const
{
    return m_attributes;
}

QMap<int, JKKSCategoryAttr >& JKKSCategory :: attributes (void)
{
    return m_attributes;
}

void JKKSCategory :: setAttributes (const QMap<int, JKKSCategoryAttr >& attrs)
{
    m_attributes = attrs;
}

const QMap<int, JKKSRubric>& JKKSCategory :: rubrics (void) const
{
    return m_rubrics;
}

QMap<int, JKKSRubric>& JKKSCategory :: rubrics (void)
{
    return m_rubrics;
}

void JKKSCategory :: setRubrics (const QMap<int, JKKSRubric>& rubrs)
{
    m_rubrics = rubrs;
}

QDataStream& operator<< (QDataStream& out, const JKKSCategory& cat)
{
    out << cat.id_io_category;
    out << cat.m_catName;
    out << cat.m_catCode;
    out << cat.m_catDesc;
    out << cat.m_idType;
    out << cat.m_idChild;
    out << cat.m_isMain;
    out << cat.m_attributes;
    out << cat.m_rubrics;
    out << cat.uid();
    out << cat.m_idState;
    out << cat.m_isGlobal;

    return out;
}

QDataStream& operator>> (QDataStream& in, JKKSCategory& cat)
{
    in >> cat.id_io_category;
    in >> cat.m_catName;
    in >> cat.m_catCode;
    in >> cat.m_catDesc;
    in >> cat.m_idType;
    in >> cat.m_idChild;
    in >> cat.m_isMain;
    in >> cat.m_attributes;
    in >> cat.m_rubrics;
    QString uid;
    in >> uid;
    cat.setUid (uid);//m_uid;
    in >> cat.m_idState;
    in >> cat.m_isGlobal;

    return in;
}

JKKSCategory & JKKSCategory::operator =(const JKKSCategory & that)
{
    if (this != &that)
        copy (that);
    return *this;
}

void JKKSCategory::copy(const JKKSCategory & that) throw(  )
{
    id_io_category = that.id_io_category;
    m_catName = that.m_catName;
    m_catCode = that.m_catCode;
    m_catDesc = that.m_catDesc;
    m_idType = that.m_idType;
    m_idChild = that.m_idChild;
    m_isMain = that.m_isMain;
    m_attributes = that.m_attributes;
    m_rubrics = that.m_rubrics;
    m_uid = that.m_uid;
    m_idState = that.m_idState;
    m_isGlobal = that.m_isGlobal;
}


JKKSCategoryPair::JKKSCategoryPair(const JKKSCategory & cMain, const JKKSCategory & cChild)
{
    m_mainCategory = cMain;
    m_childCategory = cChild;
    m_null = false;
    m_alone = false;
}

JKKSCategoryPair::JKKSCategoryPair(const JKKSCategory & cAlone)
{
    m_mainCategory = cAlone;
    m_null = false;
    m_alone = true;
}

JKKSCategoryPair::JKKSCategoryPair()
{
    m_null = true;
    m_alone = true;
}

void JKKSCategoryPair::setMainCategory(const JKKSCategory & cMain)
{
    m_mainCategory = cMain;
    m_null = false;
}

void JKKSCategoryPair::setChildCategory(const JKKSCategory & cChild)
{
    m_childCategory = cChild;
    m_null = false;
    m_alone = false;
}

const JKKSCategory & JKKSCategoryPair::mainCategory() const
{
    return m_mainCategory;
}

const JKKSCategory & JKKSCategoryPair::childCategory() const
{
    return m_childCategory;
}
