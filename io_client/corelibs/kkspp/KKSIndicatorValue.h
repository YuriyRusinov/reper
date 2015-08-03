/* 
 * File:   KKSIndicatorValue.h
 * Author: ksa
 *
 * Created on 17 Февраль 2012 г., 13:39
 */

#ifndef _KKSINDICATOR_VALUE_H
#define	_KKSINDICATOR_VALUE_H

#include "kkspp_config.h"
#include <QDateTime>
#include "KKSRecord.h"
#include "KKSMap.h"
#include "KKSValue.h"

class KKSObject;
class KKSIndicator;



class _PP_EXPORT KKSIndicatorValue : public KKSData
{
public:
    KKSIndicatorValue ();
    KKSIndicatorValue (int id, KKSIndicator * indicator, const KKSValue & value);
    KKSIndicatorValue (const KKSIndicatorValue & Ind);
    virtual ~KKSIndicatorValue ();

    int id() const;
    void setId(int _id);

    KKSObject * io();
    const KKSObject * io() const;
    void setIO(KKSObject * io);

    const KKSIndicator * indicator() const;
    KKSIndicator * indicator();
    void setIndicator(KKSIndicator * indicator);

    KKSObject * ioSrc();
    const KKSObject * ioSrc() const;
    void setIOSrc(KKSObject * io);

    KKSObject * ioSrc1();
    const KKSObject * ioSrc1() const;
    void setIOSrc1(KKSObject * io);

    //const QDateTime & measDateTime() const;
    //void setMeasDateTime(const QDateTime & dt);

    const QDateTime & insertDateTime() const;
    void setInsertDateTime(const QDateTime & dt);

    const QDateTime & startDateTime() const;
    void setStartDateTime(const QDateTime & dt);
    const QDateTime & stopDateTime() const;
    void setStopDateTime(const QDateTime & dt);

    const KKSValue & value() const;
    KKSValue & value();
    void setValue(const KKSValue & value);

    bool isValid() const;


private:

    int m_id;
    KKSIndicator * m_indicator;
    KKSObject * m_io;
    KKSObject * m_ioSrc;
    KKSObject * m_ioSrc1;
    //QDateTime m_measDateTime;
    QDateTime m_insertDateTime;
    QDateTime m_startDateTime;
    QDateTime m_stopDateTime;
    
    KKSValue m_value;

    bool m_valid;

    void verify();

};

#endif	/* _KKSINDICATOR_VALUE_H */

