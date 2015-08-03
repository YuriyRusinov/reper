/***********************************************************************
 * Module:  KKSRecord.cpp
 * Author:  sergey
 * Modified: 1 декабря 2008 г. 12:11:43
 * Purpose: Implementation of the class KKSRecord
 ***********************************************************************/

#include "KKSRecord.h"
#include "KKSState.h"

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

const QDateTime & KKSRecord::lastUpdate() const
{
    return m_lastUpdate;
}

void KKSRecord::setLastUpdate(const QDateTime & l)
{
    m_lastUpdate = l;
}

const KKSState * KKSRecord::state () const
{
    return m_state;
}

KKSState * KKSRecord::state ()
{
    return m_state;
}

void KKSRecord::setState(KKSState * s)
{
    if(m_state)
        m_state->release();

    m_state = s;

    if(m_state)
        m_state->addRef();
    //else{
    //    m_state = KKSState::defState1();
    //}

}

const QString & KKSRecord::uuid() const
{
    return m_uuid;
}

void KKSRecord::setUuid(const QString & u)
{
    m_uuid = u;
}

const QString & KKSRecord::uniqueId() const
{
    return m_uniqueId;
}

void KKSRecord::setUniqueId(const QString & u)
{
    m_uniqueId = u;
}

QIcon KKSRecord :: icon (void) const
{
    return m_icon;
}

void KKSRecord :: setIcon (const QString & s)
{
    QPixmap px;
    px.loadFromData(s.toUtf8());

    m_icon = QIcon(px);
    m_iconData = s;
}

const QString & KKSRecord :: iconAsString () const
{
    return m_iconData;
}

const QString KKSRecord :: defIconAsString ()
{
    return QString::null;
}

const QColor & KKSRecord::recordFillColor() const
{
    return m_recordFillColor;
}

void KKSRecord::setRecordFillColor(const QColor & color)
{
    m_recordFillColor = color;
}

const QColor & KKSRecord::recordTextColor() const
{
    return m_recordTextColor;
}

void KKSRecord::setRecordTextColor(const QColor & color)
{
    m_recordTextColor = color;
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

KKSRecord::KKSRecord():
m_recordFillColor(QColor()),
m_recordTextColor(QColor())
{
    m_id = -1;
    m_uuid = QString::null;
    m_uniqueId = QString::null;
    
    m_state = NULL;
   // KKSState * s = dynamic_cast<KKSState *>(this);
   // if(!s)
   //     m_state = KKSState::defState1();
    
    setIcon(KKSRecord::defIconAsString());

    m_name = QString::null;
    m_code = QString::null;
    m_parent = NULL;
    //m_isKKSState = false;
}

KKSRecord::KKSRecord(const KKSRecord & r) :
m_recordFillColor(r.recordFillColor()),
m_recordTextColor(r.recordTextColor())
{
    m_id = r.id();
    m_lastUpdate = r.lastUpdate();
    m_uuid = r.uuid();
    m_uniqueId = r.uniqueId();
    //m_isKKSState = r.m_isKKSState;
    
    m_state = NULL;
    //KKSState * s = dynamic_cast<KKSState *>(this);
    //if(!s)
    setState(const_cast<KKSState *>(r.state()));

    m_icon = r.m_icon;
    m_iconData = r.m_iconData;

    m_name = r.name();
    setCode(r.code());
    //m_code = r.code();

    m_parent = NULL;
    setParent(const_cast<KKSRecord*>(r.parent()));
}

KKSRecord::KKSRecord(qint64 _id, const QString & _name, const QString & _desc, const QString & _code) : //, bool isKKSState) :
m_recordFillColor(QColor()),
m_recordTextColor(QColor())
{
    m_id = _id;
    m_uuid = QString::null;
    m_uniqueId = QString::null;
    
    //m_isKKSState = isKKSState;
    //if(m_isKKSState)

    m_state = NULL;
    //KKSState * s = dynamic_cast<KKSState *>(this);
    //if(!s)
    //    m_state = KKSState::defState1();

    setIcon(KKSRecord::defIconAsString());

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
    if(m_parent){
        m_parent->release();
        m_parent = NULL;
    }
    if(m_state){
        m_state->release();
        m_state = NULL;
    }
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
