/***********************************************************************
 * Module:  KKSStatElement.cpp
 * Author:  sergey
 * Modified: 25 но€бр€ 2008 г. 17:46:06
 * Purpose: Implementation of the class KKSStatElement
 ***********************************************************************/

#include "KKSStatElement.h"
#include <QObject>

KKSStatElement::KKSStatElement() 
: KKSData(),
m_idUser(-1),
m_operType(KKSStatElement::soUndefined),
m_operTime(QDateTime::currentDateTime())
{
}

KKSStatElement::KKSStatElement(const KKSStatElement & t) : KKSData()
{
    m_idUser = t.m_idUser;
    m_userName = t.m_userName;
    m_operTime = t.m_operTime;
    m_operType = t.m_operType;
}

KKSStatElement::KKSStatElement(int idUser, 
                               const QString & userName, 
                               KKSStatOperation operType, 
                               QDateTime operTime) 
: KKSData(),
m_idUser(idUser),
m_userName(userName),
m_operType(operType),
m_operTime(operTime)
{
    
}

KKSStatElement::~KKSStatElement()
{
}

const QString & KKSStatElement::userName() const
{
    return m_userName;
}

KKSStatElement::KKSStatOperation KKSStatElement::operType() const
{
    return m_operType;
}

QString KKSStatElement::operTypeName() const
{
    switch(m_operType){
        case soUndefined: 
            return QObject::tr("Undefined");
        case soInsertIO:
            return QObject::tr("Object created");
        case soUpdateIO:
            return QObject::tr("Object params changed");
        case soInsertAttr:
            return QObject::tr("Attribute added");
        case soRemoveAttr:
            return QObject::tr("Attribute removed");
        case soUpdateAttrList:
            return QObject::tr("Attribute list changed");
    }

    return QObject::tr("Undefined");
}

const QDateTime & KKSStatElement::operTime() const
{
    return m_operTime;
}
