/***********************************************************************
 * Module:  KKSAttrValue.cpp
 * Author:  sergey
 * Modified: 27 но€бр€ 2008 г. 14:01:33
 * Purpose: Implementation of the class KKSAttrValue
 ***********************************************************************/

#include "KKSValue.h"
#include "KKSCategoryAttr.h"
#include "KKSAttrValue.h"
#include "KKSObject.h"

int m_defId = -2;

void KKSAttrValue::initDefId(){ m_defId = -2;}
int  KKSAttrValue::getDefId(){ return m_defId;}
void KKSAttrValue::decDefId() {m_defId--;}



////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrValue::KKSAttrValue()
// Purpose:    Implementation of KKSAttrValue::KKSAttrValue()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttrValue::KKSAttrValue() : KKSData(),
m_ioSrc(0),
m_ioSrc1(0),
m_isActual(true)
{
   m_attribute = NULL;
   
   m_id = KKSAttrValue::getDefId();
   KKSAttrValue::decDefId();
}

KKSAttrValue::KKSAttrValue(const KKSAttrValue & av) : KKSData(),
m_ioSrc(0),
m_ioSrc1(0),
m_isActual(true)
{
    m_attribute = NULL;

    setId(av.id());

    setMeasDateTime(av.measDateTime());
    setInsertDateTime(av.insertDateTime());
    setStartDateTime(av.startDateTime());
    setStopDateTime(av.stopDateTime());

    setIOSrc(const_cast<KKSObject *>(av.ioSrc()));
    setIOSrc1(const_cast<KKSObject *>(av.ioSrc1()));

    m_value = av.value();

    m_desc = av.desc();

    if(av.attribute())
        setAttribute(const_cast<KKSCategoryAttr*>(av.attribute()));

    verify();
}

KKSAttrValue::KKSAttrValue(const KKSValue & v, KKSCategoryAttr * a) : KKSData(),
m_ioSrc(0),
m_ioSrc1(0),
m_isActual(true)
{
    m_id = KKSAttrValue::getDefId();
    KKSAttrValue::decDefId();
    
    m_attribute = NULL;
    setValue(v);
    setAttribute(a);
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSAttrValue::~KKSAttrValue()
// Purpose:    Implementation of KKSAttrValue::~KKSAttrValue()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSAttrValue::~KKSAttrValue()
{
    if(m_attribute)
        m_attribute->release();

    if(m_ioSrc)
        m_ioSrc->release();

    if(m_ioSrc1)
        m_ioSrc1->release();

}

const KKSValue & KKSAttrValue::value() const
{
    return m_value;
}

KKSValue & KKSAttrValue::value()
{
    return m_value;
}

void KKSAttrValue::setValue(const KKSValue & value)
{
    m_value = value;
}

const QString & KKSAttrValue::desc() const
{
    return m_desc;
}

QString & KKSAttrValue::desc()
{
    return m_desc;
}

void KKSAttrValue::setDesc(const QString & desc)
{
    m_desc = desc;
}

const KKSCategoryAttr * KKSAttrValue::attribute() const
{
    return m_attribute;
}

KKSCategoryAttr * KKSAttrValue::attribute()
{
    return m_attribute;
}

void KKSAttrValue::setAttribute(KKSCategoryAttr * attr)
{
    if(m_attribute)
        m_attribute->release();

    m_attribute = attr;
    if(m_attribute)
        m_attribute->addRef();
}

int KKSAttrValue::id() const
{
    return m_id;
}


void KKSAttrValue::setId(int id)
{
    m_id = id;
}

KKSObject * KKSAttrValue::ioSrc()
{
    return m_ioSrc;
}

const KKSObject * KKSAttrValue::ioSrc() const
{
    return m_ioSrc;
}

void KKSAttrValue::setIOSrc(KKSObject * io)
{
    if (m_ioSrc)
        m_ioSrc->release ();

    m_ioSrc = io;

    if (m_ioSrc)
        m_ioSrc->addRef ();
}

KKSObject * KKSAttrValue::ioSrc1()
{
    return m_ioSrc1;
}

const KKSObject * KKSAttrValue::ioSrc1() const
{
    return m_ioSrc1;
}

void KKSAttrValue::setIOSrc1(KKSObject * io)
{
    if (m_ioSrc1)
        m_ioSrc1->release ();

    m_ioSrc1 = io;

    if (m_ioSrc1)
        m_ioSrc1->addRef ();
}

const QDateTime & KKSAttrValue::measDateTime() const
{
    return m_measDateTime;
}

void KKSAttrValue::setMeasDateTime(const QDateTime & dt)
{
    m_measDateTime = dt;

    verify();
}

const QDateTime & KKSAttrValue::insertDateTime() const
{
    return m_insertDateTime;
}

void KKSAttrValue::setInsertDateTime(const QDateTime & dt)
{
    m_insertDateTime = dt;

    verify();
}

const QDateTime & KKSAttrValue::startDateTime() const
{
    return m_startDateTime;
}

void KKSAttrValue::setStartDateTime(const QDateTime & dt)
{
    m_startDateTime = dt;

    verify();
}

const QDateTime & KKSAttrValue::stopDateTime() const
{
    return m_stopDateTime;
}

void KKSAttrValue::setStopDateTime(const QDateTime & dt)
{
    m_stopDateTime = dt;
    
    verify();
}

void KKSAttrValue::verify()
{
    m_valid = true;
}

bool KKSAttrValue::isValid() const
{
    return m_valid;
}

bool KKSAttrValue::isActual() const
{
    return m_isActual;
}

void KKSAttrValue::setActual(bool a)
{
    m_isActual = a;
}

