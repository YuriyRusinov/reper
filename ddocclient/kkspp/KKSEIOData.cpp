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
    
    m_fields = copy.fields();
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

/*
const QString & KKSEIOData::fieldCode(int index) const
{
    
}
*/

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

