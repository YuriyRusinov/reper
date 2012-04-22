/***********************************************************************
 * Module:  KKSRecord.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:11:43
 * Purpose: Implementation of the class KKSRecord
 ***********************************************************************/

#include "KKSRecord.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::getName()
// Purpose:    Implementation of KKSRecord::getName()
// Comment:    Возвращает название записи (ИО или экз. ИО)
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSRecord::name(void) const
{
   return m_name;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::setName(QString newName)
// Purpose:    Implementation of KKSRecord::setName()
// Parameters:
// - newName
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSRecord::setName(const QString & newName)
{
   m_name = newName;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::getId()
// Purpose:    Implementation of KKSRecord::getId()
// Return:     int
////////////////////////////////////////////////////////////////////////

qint64 KKSRecord::id(void) const
{
   return m_id;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::setId(int newId)
// Purpose:    Implementation of KKSRecord::setId()
// Parameters:
// - newId
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSRecord::setId(qint64 newId)
{
   m_id = newId;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::getDesc()
// Purpose:    Implementation of KKSRecord::getDesc()
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSRecord::desc(void) const
{
   return m_desc;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::setDesc(QString newDesc)
// Purpose:    Implementation of KKSRecord::setDesc()
// Parameters:
// - newDesc
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSRecord::setDesc(const QString & newDesc)
{
   m_desc = newDesc;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::getCode()
// Purpose:    Implementation of KKSRecord::getCode()
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSRecord::code(bool quoted) const
{
    if(quoted)
        return m_quotedCode;

    return m_code;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::setCode(QString newCode)
// Purpose:    Implementation of KKSRecord::setCode()
// Parameters:
// - newCode
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSRecord::setCode(const QString & newCode)
{
    m_code = newCode;
    m_quotedCode = QString("\"") + m_code + QString("\"");
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::KKSRecord()
// Purpose:    Implementation of KKSRecord::KKSRecord()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSRecord::KKSRecord()
{
   m_id = -1;
   m_name = QString::null;
   m_code = QString::null;
   m_parent = NULL;
}

KKSRecord::KKSRecord(const KKSRecord & r)
{
    m_id = r.id();
    m_name = r.name();
    setCode(r.code());
    //m_code = r.code();

    m_parent = NULL;
    setParent(const_cast<KKSRecord*>(r.parent()));
}

KKSRecord::KKSRecord(qint64 _id, const QString & _name, const QString & _desc, const QString & _code)
{
    m_id = _id;
    m_name = _name;
    setCode(_code);
    //m_code = _code;
    m_desc = _desc;

    m_parent = NULL;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSRecord::~KKSRecord()
// Purpose:    Implementation of KKSRecord::~KKSRecord()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSRecord::~KKSRecord()
{
    if(m_parent)
        m_parent->release();
}

void KKSRecord::setParent(KKSRecord * p)
{
    if(m_parent)
        m_parent->release();

    m_parent = p;
    if(m_parent)
        m_parent->addRef();
}

const KKSRecord * KKSRecord::parent() const
{
    return m_parent;
}

KKSRecord * KKSRecord::parent()
{
    return m_parent;
}
