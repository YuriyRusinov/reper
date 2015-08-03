/***********************************************************************
 * Module:  KKSType.cpp
 * Author:  sergey
 * Modified: 25 ноября 2008 г. 17:46:06
 * Purpose: Implementation of the class KKSType
 ***********************************************************************/

#include "KKSType.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSType::KKSType()
// Purpose:    Implementation of KKSType::KKSType()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSType::KKSType() : KKSRecord()
{
    m_isQualifier = false;
}

KKSType::KKSType(const KKSType & t) : KKSRecord(t)
{
    m_isQualifier = t.m_isQualifier;
}

KKSType::KKSType(int id, const QString & name, const QString & desc) : KKSRecord(id, name, desc)
{
    m_isQualifier = false;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSType::~KKSType()
// Purpose:    Implementation of KKSType::~KKSType()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSType::~KKSType()
{
   // TODO : implement
}

KKSType * KKSType::defType1()
{
    KKSType * type = new KKSType(1, QString::fromLocal8Bit("Общие документы"));
    type->setAsQualifier(false);

    return type;
}
KKSType * KKSType::createType10()
{
    KKSType * type = new KKSType(10,  QString::fromLocal8Bit("Подчиненная категория"));
    type->setAsQualifier(true);

    return type;
}

bool KKSType::isQualifier() const
{
    return m_isQualifier;
}

void KKSType::setAsQualifier(bool yes)
{
    m_isQualifier = yes;
}

void KKSType::setRName(const QString & rName)
{
    m_rName = rName;
}

const QString & KKSType::rName() const
{
    return m_rName;
}
