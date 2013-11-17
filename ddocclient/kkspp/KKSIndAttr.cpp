/* 
 * File:   KKSIndAttr.cpp
 * Author: yuriy
 * 
 * Created on 24 Февраль 2012 г., 17:47
 */

#include "KKSIndAttr.h"
#include "KKSAttrType.h"

KKSIndAttr::KKSIndAttr()
    : KKSRecord(),
    m_tableName (QString()),
    m_columnName (QString()),
    m_refColumnName (QString()),
    m_refColumnType (0),
    m_quotedColumnName (QString()),
    m_type (0),
    m_refType (0)
{
}

KKSIndAttr :: KKSIndAttr(int id, const QString & name, const QString & desc, const QString & code)
    : KKSRecord (id, name, desc, code),
    m_tableName (QString()),
    m_columnName (QString()),
    m_refColumnName (QString()),
    m_refColumnType (0),
    m_quotedColumnName (QString()),
    m_type (0),
    m_refType (0)
{
}

KKSIndAttr::KKSIndAttr(const KKSIndAttr& ia)
    : KKSRecord (ia),
    m_tableName (ia.m_tableName),
    m_columnName (ia.m_columnName),
    m_refColumnName (ia.m_refColumnName),
    m_refColumnType (ia.m_refColumnType),
    m_quotedColumnName (ia.m_quotedColumnName),
    m_type (ia.m_type),
    m_refType (ia.m_refType)
{
    if (m_refColumnType)
        m_refColumnType->addRef();
    if (m_type)
        m_type->addRef ();
    if (m_refType)
        m_refType->addRef ();
}

KKSIndAttr::~KKSIndAttr()
{
    if (m_refColumnType)
        m_refColumnType->release();
    if (m_type)
        m_type->release ();
    if (m_refType)
        m_refType->release ();
}

const QString & KKSIndAttr :: columnName(bool quoted) const
{
    if(quoted){
        if(m_quotedColumnName.isEmpty() && !m_columnName.isEmpty())
            m_quotedColumnName = QString("\"") + m_columnName + QString("\"");

        return m_quotedColumnName;
    }

   return m_columnName;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSIndAttr :: setColumnName(QString newColumnName)
// Purpose:    Implementation of KKSIndAttr :: setColumnName()
// Parameters:
// - newColumnName
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSIndAttr :: setColumnName(const QString & newColumnName)
{
    m_columnName = newColumnName;
    if(!m_columnName.isEmpty())
        m_quotedColumnName = QString("\"") + m_columnName + QString("\"");
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSIndAttr :: getRefColumnName()
// Purpose:    Implementation of KKSIndAttr :: getColumnName()
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSIndAttr :: refColumnName(bool quoted) const
{
    if(quoted){
        if(m_quotedRefColumnName.isEmpty() && !m_refColumnName.isEmpty())
            m_quotedRefColumnName = QString("\"") + m_refColumnName + QString("\"");

        return m_quotedRefColumnName;
    }

   return m_refColumnName;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSIndAttr :: setColumnName(QString newColumnName)
// Purpose:    Implementation of KKSIndAttr :: setColumnName()
// Parameters:
// - newColumnName
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSIndAttr :: setRefColumnName(const QString & newColumnName)
{
    m_refColumnName = newColumnName;
    if(!m_refColumnName.isEmpty())
        m_quotedRefColumnName = QString("\"") + m_refColumnName + QString("\"");
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSIndAttr :: getTableName()
// Purpose:    Implementation of KKSIndAttr :: getTableName()
// Return:     QString
////////////////////////////////////////////////////////////////////////

const QString & KKSIndAttr :: tableName(void) const
{
   return m_tableName;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSIndAttr :: setTableName(QString newTableName)
// Purpose:    Implementation of KKSIndAttr :: setTableName()
// Parameters:
// - newTableName
// Return:     void
////////////////////////////////////////////////////////////////////////

void KKSIndAttr :: setTableName(const QString & newTableName)
{
   m_tableName = newTableName;
}

const KKSAttrType * KKSIndAttr :: type() const
{
    return m_type;
}

KKSAttrType * KKSIndAttr :: type()
{
    return m_type;
}

void KKSIndAttr :: setType(KKSAttrType * _type)
{
    if(m_type)
        m_type->release();

    m_type = _type;
    if(m_type)
        m_type->addRef();
}

const KKSAttrType * KKSIndAttr :: refType() const
{
    return m_refType;
}

KKSAttrType * KKSIndAttr :: refType()
{
    return m_refType;
}

void KKSIndAttr :: setRefType(KKSAttrType * _type)
{
    if(m_refType)
        m_refType->release();

    m_refType = _type;
    if(m_refType)
        m_refType->addRef();
}

const KKSAttrType * KKSIndAttr :: refColumnType() const
{
    return m_refColumnType;
}

KKSAttrType * KKSIndAttr :: refColumnType()
{
    return m_refColumnType;
}

void KKSIndAttr :: setRefColumnType(KKSAttrType * _type)
{
    if(m_refColumnType)
        m_refColumnType->release();

    m_refColumnType = _type;
    if(m_refColumnType)
        m_refColumnType->addRef();
}

KKSIndAttrClass KKSIndAttr :: isSystem() const
{
#ifdef Q_CC_MSVC
    return iacIOUserAttr;
#else
    return iacIOUserAttr;
#endif    
}

