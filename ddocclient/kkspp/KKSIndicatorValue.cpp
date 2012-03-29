#include "KKSIndicatorValue.h"
#include "KKSIndicator.h"
#include "KKSObject.h"

KKSIndicatorValue :: KKSIndicatorValue () : KKSData(),
m_io(0),
m_ioSrc(0),
m_ioSrc1(0),
m_indicator(0)
{
    m_id = -1;

    m_valid = false;
}

KKSIndicatorValue :: KKSIndicatorValue (int id, KKSIndicator * indicator, const KKSValue & value) : KKSData(),
m_io(0),
m_ioSrc(0),
m_ioSrc1(0),
m_indicator(0)
{
    m_id = id;

    setIndicator(indicator);
    setValue(value);

    verify();
}

KKSIndicatorValue :: KKSIndicatorValue (const KKSIndicatorValue & Ind) : KKSData(),
m_io(0),
m_ioSrc(0),
m_ioSrc1(0),
m_indicator(0)
{
    setId(Ind.id());

    setMeasDateTime(Ind.measDateTime());
    setInsertDateTime(Ind.insertDateTime());
    setStartDateTime(Ind.startDateTime());
    setStopDateTime(Ind.stopDateTime());

    setIO(const_cast<KKSObject *>(Ind.io()));
    setIOSrc(const_cast<KKSObject *>(Ind.ioSrc()));
    setIOSrc1(const_cast<KKSObject *>(Ind.ioSrc1()));
    setIndicator(const_cast<KKSIndicator *>(Ind.indicator()));

    setValue(Ind.value());
}

KKSIndicatorValue :: ~KKSIndicatorValue ()
{
    if(m_io)
        m_io->release();

    if(m_indicator)
        m_indicator->release();

    if(m_ioSrc)
        m_ioSrc->release();

    if(m_ioSrc1)
        m_ioSrc1->release();

}


int KKSIndicatorValue::id() const
{
    return m_id;
}


void KKSIndicatorValue::setId(int id)
{
    m_id = id;
}


KKSObject * KKSIndicatorValue::io()
{
    return m_io;
}

const KKSObject * KKSIndicatorValue::io() const
{
    return m_io;
}

void KKSIndicatorValue::setIO(KKSObject * io)
{
    if (m_io)
        m_io->release ();

    m_io = io;

    if (m_io)
        m_io->addRef ();
}

const KKSIndicator * KKSIndicatorValue::indicator() const
{
    return m_indicator;
}

KKSIndicator * KKSIndicatorValue::indicator()
{
    return m_indicator;
}

void KKSIndicatorValue::setIndicator(KKSIndicator * indicator)
{
    if (m_indicator)
        m_indicator->release ();

    m_indicator = indicator;

    if (m_indicator)
        m_indicator->addRef ();
}

KKSObject * KKSIndicatorValue::ioSrc()
{
    return m_ioSrc;
}

const KKSObject * KKSIndicatorValue::ioSrc() const
{
    return m_ioSrc;
}

void KKSIndicatorValue::setIOSrc(KKSObject * io)
{
    if (m_ioSrc)
        m_ioSrc->release ();

    m_ioSrc = io;

    if (m_ioSrc)
        m_ioSrc->addRef ();
}

KKSObject * KKSIndicatorValue::ioSrc1()
{
    return m_ioSrc1;
}

const KKSObject * KKSIndicatorValue::ioSrc1() const
{
    return m_ioSrc1;
}

void KKSIndicatorValue::setIOSrc1(KKSObject * io)
{
    if (m_ioSrc1)
        m_ioSrc1->release ();

    m_ioSrc1 = io;

    if (m_ioSrc1)
        m_ioSrc1->addRef ();
}

const QDateTime & KKSIndicatorValue::measDateTime() const
{
    return m_measDateTime;
}

void KKSIndicatorValue::setMeasDateTime(const QDateTime & dt)
{
    m_measDateTime = dt;

    verify();
}

const QDateTime & KKSIndicatorValue::insertDateTime() const
{
    return m_insertDateTime;
}

void KKSIndicatorValue::setInsertDateTime(const QDateTime & dt)
{
    m_insertDateTime = dt;

    verify();
}

const QDateTime & KKSIndicatorValue::startDateTime() const
{
    return m_startDateTime;
}

void KKSIndicatorValue::setStartDateTime(const QDateTime & dt)
{
    m_startDateTime = dt;

    verify();
}

const QDateTime & KKSIndicatorValue::stopDateTime() const
{
    return m_stopDateTime;
}

void KKSIndicatorValue::setStopDateTime(const QDateTime & dt)
{
    m_stopDateTime = dt;
    
    verify();
}

const KKSValue & KKSIndicatorValue::value() const
{
    return m_value;
}

KKSValue & KKSIndicatorValue::value()
{
    return m_value;
}

void KKSIndicatorValue::setValue(const KKSValue & value)
{
    m_value = value;
}

void KKSIndicatorValue::verify()
{
    m_valid = true;
}

