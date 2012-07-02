/***********************************************************************
 * Module:  KKSEIOData.cpp
 * Author:  ksa
 * Modified: 19 декабря 2008 г. 16:57:28
 * Purpose: Implementation of the class KKSEIOData
 ***********************************************************************/

#include "KKSEIOData.h"
#include "defines.h"

KKSEIOData::KKSEIOData() : KKSData()
{
    m_null = QString::null;
}

KKSEIOData::KKSEIOData(const KKSEIOData & copy) : KKSData()
{
    m_null = QString::null;
    
    m_fields.clear();
    m_sysFields.clear();
    
    m_sysFields = copy.sysFields();
}

KKSEIOData::~KKSEIOData()
{
}

const QMap<QString, QString> & KKSEIOData::fields() const
{
    return m_fields;
}

QMap<QString, QString> & KKSEIOData::fields()
{
    return m_fields;
}

void KKSEIOData::clear()
{
    m_fields.clear();
    m_sysFields.clear();
}

int KKSEIOData::fieldsCount() const
{
    return m_fields.count();
}

QString KKSEIOData::fieldValue(const QString & code) const
{
    if(!m_fields.contains(code))
        return QString::null;

    return m_fields[code];
}

QString & KKSEIOData::fieldValue(const QString & code) 
{
    if(!m_fields.contains(code))
        return m_null;

    return m_fields[code];
}


int KKSEIOData::addField(const QString & code, const QString & value)
{
    if(code.isEmpty())
        return ERROR_CODE;
    
    if(m_fields.contains(code))
        return ERROR_CODE;

    m_fields.insert(code, value);
    return OK_CODE;
}

void KKSEIOData::setFields(const QMap<QString, QString> & fields)
{
    m_fields = fields;
}

int KKSEIOData::removeField(const QString & code)
{
    int cnt = m_fields.remove(code);

    return cnt;
}

int KKSEIOData::updateField(const QString & code, const QString & value)
{
    int cnt = removeField(code);
    if(cnt == 0)
        return ERROR_CODE;

    cnt = addField(code, value);

    return cnt;
}

/************************************/
const QMap<QString, QString> & KKSEIOData::sysFields() const
{
    return m_sysFields;
}

QMap<QString, QString> & KKSEIOData::sysFields()
{
    return m_sysFields;
}


int KKSEIOData::sysFieldsCount() const
{
    return m_sysFields.count();
}

QString KKSEIOData::sysFieldValue(const QString & code) const
{
    if(!m_sysFields.contains(code))
        return QString::null;

    return m_sysFields[code];
}

QString & KKSEIOData::sysFieldValue(const QString & code) 
{
    if(!m_sysFields.contains(code))
        return m_null;

    return m_sysFields[code];
}


int KKSEIOData::addSysField(const QString & code, const QString & value)
{
    if(code.isEmpty())
        return ERROR_CODE;
    
    if(m_sysFields.contains(code))
        return ERROR_CODE;

    m_sysFields.insert(code, value);
    return OK_CODE;
}

void KKSEIOData::setSysFields(const QMap<QString, QString> & fields)
{
    m_sysFields = fields;
}

int KKSEIOData::removeSysField(const QString & code)
{
    int cnt = m_sysFields.remove(code);

    return cnt;
}

int KKSEIOData::updateSysField(const QString & code, const QString & value)
{
    int cnt = removeSysField(code);
    if(cnt == 0)
        return ERROR_CODE;

    cnt = addSysField(code, value);

    return cnt;
}
