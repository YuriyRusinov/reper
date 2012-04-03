/***********************************************************************
 * Module:  KKSAttrAttrValue.cpp
 * Author:  sergey
 * Modified: 02 04 2012 ã. 14:01:33
 * Purpose: Implementation of the class KKSAttrAttrValue
 ***********************************************************************/

#include "KKSValue.h"
#include "KKSAttrAttr.h"
#include "KKSAttrAttrValue.h"
//#include "KKSObject.h"

/*
int m_defId = -2;

void KKSAttrValue::initDefId(){ m_defId = -2;}
int  KKSAttrValue::getDefId(){ return m_defId;}
void KKSAttrValue::decDefId() {m_defId--;}
*/


KKSAttrAttrValue::KKSAttrAttrValue() : KKSData()                
{
    m_attribute = NULL;
    m_id = -1;
   
}

KKSAttrAttrValue::KKSAttrAttrValue(const KKSAttrAttrValue & av) : KKSData()
{
    m_attribute = NULL;

    setId(av.id());

/*
    setMeasDateTime(av.measDateTime());
    setInsertDateTime(av.insertDateTime());
    setStartDateTime(av.startDateTime());
    setStopDateTime(av.stopDateTime());

    setIOSrc(const_cast<KKSObject *>(av.ioSrc()));
    setIOSrc1(const_cast<KKSObject *>(av.ioSrc1()));

    m_desc = av.desc();
*/
    m_value = av.value();


    if(av.attribute())
        setAttribute(const_cast<KKSAttrAttr*>(av.attribute()));

    verify();
}

KKSAttrAttrValue::KKSAttrAttrValue(const KKSValue & v, KKSAttrAttr * a) : KKSData()
{
    m_id = -1;
//    m_id = KKSAttrValue::getDefId();
//    KKSAttrValue::decDefId();
    
    m_attribute = NULL;
    setValue(v);
    setAttribute(a);
}


KKSAttrAttrValue::~KKSAttrAttrValue()
{
    if(m_attribute)
        m_attribute->release();

/*
    if(m_ioSrc)
        m_ioSrc->release();

    if(m_ioSrc1)
        m_ioSrc1->release();
*/
}

const KKSValue & KKSAttrAttrValue::value() const
{
    return m_value;
}

KKSValue & KKSAttrAttrValue::value()
{
    return m_value;
}

void KKSAttrAttrValue::setValue(const KKSValue & value)
{
    m_value = value;
}

/*
const QString & KKSAttrAttrValue::desc() const
{
    return m_desc;
}

QString & KKSAttrAttrValue::desc()
{
    return m_desc;
}

void KKSAttrValue::setDesc(const QString & desc)
{
    m_desc = desc;
}
*/

const KKSAttrAttr * KKSAttrAttrValue::attribute() const
{
    return m_attribute;
}

KKSAttrAttr * KKSAttrAttrValue::attribute()
{
    return m_attribute;
}

void KKSAttrAttrValue::setAttribute(KKSAttrAttr * attr)
{
    if(m_attribute)
        m_attribute->release();

    m_attribute = attr;
    if(m_attribute)
        m_attribute->addRef();
}

int KKSAttrAttrValue::id() const
{
    return m_id;
}


void KKSAttrAttrValue::setId(int id)
{
    m_id = id;
}

/*
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
   */

void KKSAttrAttrValue::verify()
{
    m_valid = true;
}

bool KKSAttrAttrValue::isValid() const
{
    return m_valid;
}

/*
bool KKSAttrValue::isActual() const
{
    return m_isActual;
}

void KKSAttrValue::setActual(bool a)
{
    m_isActual = a;
}
*/
