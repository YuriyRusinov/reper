/***********************************************************************
 * Module:  KKSValue.cpp
 * Author:  sergey
 * Modified: 1 декабр€ 2008 г. 12:11:30
 * Purpose: Implementation of the class KKSValue
 ***********************************************************************/

#include <QDateTime>
#include <QDate>
#include <QLocale>
#include <QPointF>
#include <QRegExp>
#include <QColor>
#include <QtDebug>

#include "KKSValue.h"
#include "KKSAttrType.h"
#include "defines.h"
#include "KKSHistogram.h"

////////////////////////////////////////////////////////////////////////
// Name:       KKSValue::KKSValue()
// Purpose:    Implementation of KKSValue::KKSValue()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSValue::KKSValue()
{
    m_isValid = false;
    m_isLiteral = false;
    m_isNull = true;

    m_type = (int)KKSAttrType::atInt;
}

KKSValue::KKSValue(const KKSValue & v)
{
    m_isValid = v.isValid();
    m_isLiteral = v.isLiteral();
    m_isNull = v.isNull();
    m_type = v.type();
    m_columnValue = v.columnValue();
    m_value = v.valueVariant();
}

KKSValue::KKSValue(const QString & value, int type)
{
    m_isValid = false;
    m_isLiteral = false;
    m_isNull = true;

    setValue(value, type);
}


////////////////////////////////////////////////////////////////////////
// Name:       KKSValue::~KKSValue()
// Purpose:    Implementation of KKSValue::~KKSValue()
// Return:     
////////////////////////////////////////////////////////////////////////

KKSValue::~KKSValue()
{
}

KKSValue & KKSValue:: operator = (const KKSValue & v)
{
    KKSValue vl;
    m_isValid = v.isValid();
    m_isLiteral = v.isLiteral();
    m_isNull = v.isNull();
    m_type = v.type();
    m_columnValue = v.columnValue();
    m_value = v.valueVariant();

    return *this;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSValue::getIsValid()
// Purpose:    Implementation of KKSValue::getIsValid()
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool KKSValue::isValid(void) const
{
   return m_isValid;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSValue::verify()
// Purpose:    Implementation of KKSValue::verify()
// Return:     bool
////////////////////////////////////////////////////////////////////////

void KKSValue::verify(void) const
{
    m_isValid = false;
    m_isLiteral = false;
    m_isNull = false;


    if(m_value.isNull()){
        m_isValid = true;
        m_isLiteral = false;
        m_isNull = true;
        return;
    }

    KKSAttrType::KKSAttrTypes a_type = (KKSAttrType::KKSAttrTypes) m_type;
    
    if(value().isEmpty()){
        m_isValid = true;
        if (a_type == KKSAttrType::atString ||
            a_type == KKSAttrType::atFixString ||
            a_type == KKSAttrType::atText ||
            a_type == KKSAttrType::atComplex)
        {
            m_isLiteral = true;
            m_isNull = false;
        }
        else if(a_type == KKSAttrType::atHistogram)
        {
            //ksa
            m_isLiteral = true;
            m_isNull = true;
        }
        else if(a_type == KKSAttrType::atVectorLayer)
        {
            //ksa
            m_isLiteral = true;
            m_isNull = true;
        }
        else if(a_type == KKSAttrType::atRasterLayer)
        {
            //ksa
            m_isLiteral = true;
            m_isNull = true;
        }
        else if(a_type == KKSAttrType::atGISMap)
        {
            //ksa
            m_isLiteral = true;
            m_isNull = true;
        }
        else if (a_type == KKSAttrType::atUUID)
        {
            m_isLiteral = true;
            m_isNull = true;
        }
        else if (a_type == KKSAttrType::atJPG ||
                 a_type == KKSAttrType::atSVG ||
                 a_type == KKSAttrType::atVideo ||
                 a_type == KKSAttrType::atXMLDoc ||
                 a_type == KKSAttrType::atPoints)
        {
            m_isLiteral = true;
            m_isNull = true;
        }
        else{
            m_isLiteral = false;
            m_isNull = true;
        }

        return;
    }

    if(a_type == KKSAttrType::atUndef){
        m_isValid = false;
        m_isLiteral = false;
        return;
    }
    
    if(a_type == KKSAttrType::atBool){
        
        if(m_value.canConvert(QVariant::Bool))
            m_isValid = true;

        //true, потому что QVariant возвращает это значение в строковом представлении как 't' или 'f'
        //соответственно, при записи в Ѕƒ такие значени€ понимаютс€ только в литеральном виде
        m_isLiteral = true;
        return;
    }

    //ksa
    if(a_type == KKSAttrType::atHistogram){
        if(m_value.canConvert<KKSHistogram>()){
            m_isValid = true;
        }
        m_isLiteral = true;
        return;
    }
    //ksa
    if(a_type == KKSAttrType::atVectorLayer){
        m_isValid = true;
        m_isLiteral = true;
        return;
    }
    //ksa
    if(a_type == KKSAttrType::atRasterLayer){
        m_isValid = true;
        m_isLiteral = true;
        return;
    }
    //ksa
    if(a_type == KKSAttrType::atGISMap){
        m_isValid = true;
        m_isLiteral = true;
        return;
    }
    
    if(a_type == KKSAttrType::atDate){
        if(m_value.canConvert(QVariant::Date)){
            m_isValid = true;
        }
        m_isLiteral = true;
        return;
    }

    if(a_type == KKSAttrType::atTime){
        if(m_value.canConvert(QVariant::Time)){
            m_isValid = true;
        }
        m_isLiteral = true;
        return;
    }

    if(a_type == KKSAttrType::atDateTime){
        if(m_value.canConvert(QVariant::DateTime)){
            m_isValid = true;
        }
        m_isLiteral = true;
        return;
    }

    if(a_type == KKSAttrType::atInterval){
        QStringList sl = m_value.toStringList();
        
        m_isValid = true;
        if(sl.count() != 2){
            m_isValid = false;
            m_isNull = true;
        }
        else{
            QString amount = sl.at(0);
            bool ok = true;
            int a = amount.toInt(&ok);
            Q_UNUSED(a);
            if(!ok){
                m_isValid = false;
                m_isNull = true;
            }
            else{
                QString unit = sl.at(1);
                bool b = KKSAttrType::isValidUnit(unit);
                if(!b){
                    m_isValid = false;
                    m_isNull = true;
                }
            }
            if(a == 0){
                m_isNull = true;
            }
        }

        m_isLiteral = true;

        return;
    }

    if(a_type == KKSAttrType::atIntervalH){
        QStringList sl = m_value.toStringList();
        
        m_isValid = true;
        m_isLiteral = true;

        if(sl.count() != 3){
            m_isValid = false;
            m_isNull = true;
            return;
        } 
        else{
            QString aHours = sl.at(0);
            bool ok = true;
            int h = aHours.toInt(&ok);
            if(!ok){
                m_isValid = false;
                m_isNull = true;
                return;
            }
            QString aMin = sl.at(1);
            ok = true;
            int m = aMin.toInt(&ok);
            if(!ok){
                m_isValid = false;
                m_isNull = true;
                return;
            }
            QString aSec = sl.at(2);
            ok = true;
            int s = aSec.toInt(&ok);
            
            if(!ok){
                m_isValid = false;
                m_isNull = true;
                return;
            }
            if(h==0 && m==0 && s==0){
                m_isNull = true;
            }
        }

        return;
    }

    if(a_type == KKSAttrType::atDouble){
        if(m_value.canConvert(QVariant::Double)){
            m_isValid = true;
        }
        m_isLiteral = true;
        return;
    }

    if(a_type == KKSAttrType::atPoints){
        //!!!!!!!!!!!!!!!!
        //!!!!!!!!!!!!!!!!
        //!!!!!!!!!!!!!!!!
        m_isValid = true;
        m_isLiteral = true;
        return;
    }

    if( a_type == KKSAttrType::atString ||
        a_type == KKSAttrType::atUrl ||
        a_type == KKSAttrType::atFixString ||
        a_type == KKSAttrType::atText ||
        a_type == KKSAttrType::atJPG ||
        a_type == KKSAttrType::atVideo ||
        a_type == KKSAttrType::atSVG ||
        a_type == KKSAttrType::atXMLDoc ||
        a_type == KKSAttrType::atComplex
       )
    {
        if(m_value.canConvert(QVariant::String)){
            m_isValid = true;
        }
        m_isLiteral = true;
        return;
    }

    //ссылочные типы могут быть не только целочисленными (64)
    if(a_type == KKSAttrType::atList ||
       a_type == KKSAttrType::atParent)
    {
        bool ok (true);
        int nv = m_value.toString().toLongLong (&ok);        
        Q_UNUSED (nv);

        /*if(ok){
            m_isLiteral = false;
        }
        else*/
            m_isLiteral = true;
        
        m_isValid = true;
        return;
    }

    //дл€ типов int64, ссылка на информационный объект, ссылки на цвета
    //значение атрибута ¬—≈√ƒј должно быть целочисленным (64)!
    if( a_type == KKSAttrType::atInt64 ||
        a_type == KKSAttrType::atObjRef ||
        a_type == KKSAttrType::atRecordColorRef ||
        a_type == KKSAttrType::atRecordTextColorRef
       )
    {
        bool ok (true);
        int nv = m_value.toString().toLongLong (&ok);        
        Q_UNUSED (nv);        
        if(ok){
            m_isValid = true;
        }

        m_isLiteral = false;
        return;
    }
    //дл€ типа int
    //значение атрибута ¬—≈√ƒј должно быть целочисленным (32)!
    if( a_type == KKSAttrType::atInt )
    {
        bool ok (true);
        int nv = m_value.toString().toInt (&ok);        
        Q_UNUSED (nv);        
        if(ok){
            m_isValid = true;
        }

        m_isLiteral = false;
        return;
    }
    if (a_type == KKSAttrType::atRecordColor ||
        a_type == KKSAttrType::atRecordTextColor)
    {
        m_isValid = m_value.canConvert (QVariant::Color);
        QColor vCol = m_isValid ? m_value.value<QColor>() : QColor();
        m_isNull = !vCol.isValid();
        //qDebug () << __PRETTY_FUNCTION__ << m_value << m_isNull << vCol;
        m_isLiteral = false;
        return;
    }

    if( a_type == KKSAttrType::atCheckList ||
        a_type == KKSAttrType::atCheckListEx)
    {
        if(m_value.canConvert(QVariant::StringList)){
            m_isValid = true;
        }
        m_isLiteral = true;
        return;
    }

    if( a_type == KKSAttrType::atUUID){
        if(m_value.canConvert(QVariant::String)){
            m_isValid = true;
        }
        m_isLiteral = true;

    }

    return;
}

QDateTime KKSValue::stringToDateTime(const QString & s)
{
    QDateTime dt;

    //проверим, содержит ли значение врем€
	if(!s.contains(" ") && !s.contains("T")){
		//т.е. у нас есть только дата
		QDate d = stringToDate(s);
		if(!d.isValid())
			return dt;

		dt.setDate(d);
		dt.setTime(QTime::fromString("00:00:00"));
		return dt;
	}
    //сначала пробуем вз€ть из указанного формата
	if(s.contains("T"))
		dt = QDateTime::fromString(s, "dd.MM.yyyyTHH:mm:ss");
	else
        dt = QDateTime::fromString(s, "dd.MM.yyyy HH:mm:ss");
    
    if(!dt.isValid()){
        //потом попробуем вз€ть из текущей локали
        QLocale l = QLocale::system();
        QString formatDate = l.dateFormat(QLocale::ShortFormat);
        QString formatTime = l.timeFormat(QLocale::ShortFormat);
        if(s.contains("T"))
		    dt = QDateTime::fromString(s, formatDate + "T" + formatTime);
		else
			dt = QDateTime::fromString(s, formatDate + " " + formatTime);
    }
    else
        return dt;
    
    //потом из ISODate
    if(!dt.isValid()){
        dt = QDateTime::fromString(s, Qt::ISODate);
    }    
    else
        return dt;

    //потом из TextDate
    if(!dt.isValid()){
        dt = QDateTime::fromString(s, Qt::TextDate);
    }
    else
        return dt;

    //
    // потом пробуем вз€ть только врем€ из указанного формата
    // прин€в текущую дату за основу.
    //
    QTime _t;
    _t = QTime::fromString (s, "HH:mm:ss");

    if(!_t.isValid()){
        //потом попробуем вз€ть из текущей локали
        QLocale l = QLocale::system();
        QString formatTime = l.timeFormat(QLocale::ShortFormat);
        _t = QTime::fromString(s, formatTime);
    }

    if (!_t.isValid())
        _t = QTime::fromString(s, Qt::ISODate);

    if (!_t.isValid())
        _t = QTime::fromString(s, Qt::TextDate);

    if (_t.isValid())
    {
        dt = QDateTime::currentDateTime ();
        dt.setTime (_t);
    }

    //if(!dt.isValid())
    //      qWarning() << "Datetime cannot be parsed! String = " << s;

    return dt;
}

QDate KKSValue::stringToDate(const QString & s)
{
    QDate dt;

    //сначала пробуем вз€ть из указанного формата
    dt = QDate::fromString(s, "dd.MM.yyyy");
    
    if(!dt.isValid()){
        //потом попробуем вз€ть из текущей локали
        QLocale l = QLocale::system();
        QString formatDate = l.dateFormat(QLocale::ShortFormat);
        dt = QDate::fromString(s, formatDate);
    }
    else
        return dt;
    
    //потом из ISODate
    if(!dt.isValid()){
        dt = QDate::fromString(s, Qt::ISODate);
    }    
    else
        return dt;

    //потом из TextDate
    if(!dt.isValid()){
        dt = QDate::fromString(s, Qt::TextDate);
    }
    else
        return dt;

    //if(!dt.isValid())
    //      qWarning() << "Datetime cannot be parsed! String = " << s;

    return dt;
}

QTime KKSValue::stringToTime(const QString & s)
{

    QTime _t;
    _t = QTime::fromString (s, "HH:mm:ss");

    if(!_t.isValid()){
        //потом попробуем вз€ть из текущей локали
        QLocale l = QLocale::system();
        QString formatTime = l.timeFormat(QLocale::ShortFormat);
        _t = QTime::fromString(s, formatTime);
    }

    if (!_t.isValid())
        _t = QTime::fromString(s, Qt::ISODate);

    if (!_t.isValid())
        _t = QTime::fromString(s, Qt::TextDate);

    return _t;
}

int KKSValue::setValue(const QString & _value, int _type)
{
    m_type = _type;

    KKSAttrType::KKSAttrTypes a_type = (KKSAttrType::KKSAttrTypes) m_type;
    if(a_type == KKSAttrType::atDateTime &&
       _value != "current_timestamp")
    {
        if(_value.isEmpty()){
            m_value = _value;
            m_columnValue = m_value.toString();
            verify();
            return OK_CODE;
        }
        
        QDateTime dt = stringToDateTime(_value);
        //qDebug () << __PRETTY_FUNCTION__ << dt << _value;
        m_value = dt;
    }
    //ksa
    else if(a_type == KKSAttrType::atHistogram){
        if(_value.isEmpty()){
            m_value = _value;
            m_columnValue = value();
            verify();
            return OK_CODE;
        }

        KKSHistogram h;
        bool ok = h.fromString(_value);
        if(!ok){
            m_isValid = false;
            m_isLiteral = true;
            return ERROR_CODE;
        }
        
        m_value = m_value.fromValue<KKSHistogram>(h);
    }
    //ksa
    else if(a_type == KKSAttrType::atVectorLayer){
        if(_value.isEmpty()){
            m_value = _value;
            m_columnValue = value();
            verify();
            return OK_CODE;
        }

        m_value = _value;
    }
    //ksa
    else if(a_type == KKSAttrType::atRasterLayer){
        if(_value.isEmpty()){
            m_value = _value;
            m_columnValue = value();
            verify();
            return OK_CODE;
        }

        m_value = _value;
    }
    //ksa
    else if(a_type == KKSAttrType::atGISMap){
        if(_value.isEmpty()){
            m_value = _value;
            m_columnValue = value();
            verify();
            return OK_CODE;
        }

        m_value = _value;
    }

    else if(a_type == KKSAttrType::atDate  &&
       _value != "current_timestamp" &&
	   _value != "current_date")
    {
        if(_value.isEmpty()){
            m_value = _value;
            m_columnValue = m_value.toString();
            verify();
            return OK_CODE;
        }
        
        QDate dt = stringToDate(_value);
        //qDebug () << __PRETTY_FUNCTION__ << dt << _value;
        m_value = dt;
    }
    else if(a_type == KKSAttrType::atTime  &&
       _value != "current_timestamp" &&
	   _value != "current_time")
    {
        if(_value.isEmpty()){
            m_value = _value;
            m_columnValue = m_value.toString();
            verify();
            return OK_CODE;
        }
        
        QTime dt = stringToTime(_value);
        //qDebug () << __PRETTY_FUNCTION__ << dt << _value;
        m_value = dt;
    }
    else if(a_type == KKSAttrType::atBool)
    {
        QString s = _value.toLower();
        if(s == "t" || s == "true" || s == "1"){
            m_value = QVariant(true);
        }
        else if(s == "f" || s == "false" || s == "0"){
            m_value = QVariant(false);
        }
        else
            m_value = QVariant(false);
    }
    else if(a_type == KKSAttrType::atCheckList ||
            a_type == KKSAttrType::atCheckListEx)
    {
        QString v(_value);
        if(v.size()>0 && v.at(0) == '{'){
            v.remove(0, 1);
            v.remove(v.length()-1, 1);
        }
        QStringList sl = v.split(",", QString::SkipEmptyParts);
        m_value = sl;
    }
    else if(a_type == KKSAttrType::atInterval){
        QString v(_value);
        if(v.size()>0 && v.at(0) == '{'){
            v.remove(0, 1);
            v.remove(v.length()-1, 1);
        }
        //если данные пришли из Ѕƒ, то разделителем будет зап€та€
        //если из GUI - то пробел
        QStringList sl = v.split(",", QString::SkipEmptyParts);
        if(sl.count() < 2){
            sl = v.split(" ", QString::SkipEmptyParts);
        }
        
        if(sl.count() < 2){
            m_value = QVariant();
        }
        else{
            //если данные пришли из Ѕƒ, то значением what будет число и надо его преобразовать в строку
            //если из GUI, то преобразовывать не надо, там и так будет строка
            QString what = sl.at(1);
            bool b = KKSAttrType::isValidUnit(what);
            if(!b){//в этом случае имеем число, которое надо преобразовать
                int id = what.toInt(&b);
                if(!b){
                    qWarning() << __PRETTY_FUNCTION__ << "VALUE OF TYPE INTERVAL IS INVALID!";
                    m_value = QVariant();
                    m_columnValue = value();
                    verify();
                    return OK_CODE;
                }
                else
                    sl[1] = KKSAttrType::intervalIdToName(id);
            }
            m_value = sl;
        }
    }
    else if(a_type == KKSAttrType::atIntervalH){//hours, mins, secs
        QString v(_value);
        if(v.size()>0 && v.at(0) == '{'){
            v.remove(0, 1);
            v.remove(v.length()-1, 1);
        }
        //если данные пришли из Ѕƒ, то разделителем будет зап€та€
        //если из GUI - то пробел
        QStringList sl = v.split(",", QString::SkipEmptyParts);
        if(sl.count() < 3){
            sl = v.split(" ", QString::SkipEmptyParts);
        }
        
        if(sl.count() < 3){
                m_value = QVariant();
        }
        else{
            m_value = sl;
        }
    }
    else if(a_type == KKSAttrType::atPoints)
    {
        //
        // данные по набору точек €вл€ютс€ массивом QPointF
        //
        //m_value = _value;
        // —ерега не прав
        QString v (_value);
        if (v.size()>0 && v.at(0) == '{')
        {
            v.remove (0, 1);
            v.remove (v.length()-1, 1);
        }
        if (v.startsWith ("\""))
        {
            v.remove (0, 1);
            v.remove (v.length()-1, 1);
        }

        QRegExp rx ("\",\"");
        QStringList sl = v.split (rx, QString::SkipEmptyParts);
        //qDebug () << __PRETTY_FUNCTION__ << sl << v << rx;
        if (sl.isEmpty())
        {
            m_value = QVariant();
            return OK_CODE;
        }
        QList<QPointF> pList;
        QList<QVariant> pvList;
        for (int i=0; i<sl.count(); i++)
        {
            QString v_str = sl[i];
            if (v_str.startsWith ("("))
            {
                v_str.remove (0, 1);
                v_str.remove (v_str.length()-1, 1);
            }
            QStringList wsl = v_str.split (",");
            if (wsl.count() < 2)
            {
                m_value = QVariant();
                return OK_CODE;
            }
            bool ok_x;
            bool ok_y;
            double x = wsl[0].toDouble (&ok_x);
            double y = wsl[1].toDouble (&ok_y);
            if (ok_x && ok_y)
            {
                QPointF p (x, y);
                pList.append (p);
                QVariant vp (p);
                pvList.append (vp);
            }
            else
            {
                m_value = QVariant();
                return OK_CODE;
            }
        }
        m_value = QVariant (pvList);
    }
    else if (a_type == KKSAttrType::atRecordColor ||
             a_type == KKSAttrType::atRecordTextColor)
    {
        bool ok;
        unsigned int vl = _value.toUInt (&ok);
        //qDebug () << __PRETTY_FUNCTION__ << _value << vl << ok;
        QRgb rgb_col = vl;
        QColor vCol;
        if (!_value.isEmpty() && ok)
            vCol = QColor::fromRgba (rgb_col);
        else
            vCol = QColor();
        m_value = QVariant (vCol);
    }
    else{
        m_value = _value;
    }

    m_columnValue = value();

    verify();

    return OK_CODE;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSValue::getValue()
// Purpose:    Implementation of KKSValue::getValue()
// Return:     QString
////////////////////////////////////////////////////////////////////////

QString KKSValue::value(void) const
{
    if(m_type == KKSAttrType::atCheckList ||
       m_type == KKSAttrType::atCheckListEx ||
       m_type == KKSAttrType::atInterval || 
       m_type == KKSAttrType::atIntervalH)
    {
        return m_value.toStringList().join(",");
    }

    //ksa
    if(m_type == KKSAttrType::atHistogram){
        KKSHistogram h = m_value.value<KKSHistogram>();

        return h.toString();
    }
    //ksa
    if(m_type == KKSAttrType::atVectorLayer){
        return m_value.toString();
    }
    //ksa
    if(m_type == KKSAttrType::atRasterLayer){
        return m_value.toString();
    }
    //ksa
    if(m_type == KKSAttrType::atGISMap){
        return m_value.toString();
    }


    if(m_type == KKSAttrType::atDouble){
        if(m_value.isNull())
            return "";

        double v = m_value.toDouble();
        QString s = QString("%1").arg(v);
        return s;
    }

    return m_value.toString();
}

const QVariant & KKSValue::valueVariant(void) const
{
    return m_value;
}

QString KKSValue::valueForInsert() const
{
    KKSAttrType::KKSAttrTypes a_type = (KKSAttrType::KKSAttrTypes) m_type;

    if(isNull()){
        if(a_type == KKSAttrType::atCheckList ||
           a_type == KKSAttrType::atCheckListEx)
        {
            return "'{}'";
        }
        else if (a_type == KKSAttrType::atRecordColor ||
                 a_type == KKSAttrType::atRecordTextColor ||
                 a_type == KKSAttrType::atInt64 || 
                 a_type == KKSAttrType::atList ||
                 a_type == KKSAttrType::atParent ||
                 a_type == KKSAttrType::atRecordColorRef ||
                 a_type == KKSAttrType::atRecordTextColorRef
                 )
        {
            return "NULL::bigint";
        }

        return "NULL";
    }
    
    QString val;


    if(a_type == KKSAttrType::atDateTime ||
       a_type == KKSAttrType::atDate
       )
    {
        QString tVal = value();
        if(tVal == "current_timestamp"){
            return tVal + "::timestamp";
        }
        QDateTime dt = m_value.toDateTime();
        tVal = dt.toString("dd.MM.yyyy hh:mm:ss");
        val = QString("to_timestamp('%1', 'DD.MM.YYYY HH24:MI:SS')::timestamp").arg(tVal);
        return val;
    }
    else if (a_type == KKSAttrType::atTime)
    {
        QString tVal = value();
        if(QString::compare(tVal, "current_timestamp", Qt::CaseInsensitive) == 0 ||
           QString::compare (tVal, "current_time", Qt::CaseInsensitive) == 0)
        {
            return tVal + "::timestamp";
        }
        QTime dt = m_value.toTime();
        tVal = dt.toString("hh:mm:ss");
        val = QString("to_time('%1')").arg(tVal);
        return val;
    }

    if(a_type == KKSAttrType::atInterval){
        if(m_value.toStringList().count() < 2)
            return "NULL";

        if(m_isNull)
            return "NULL";

        int amount = m_value.toStringList().at(0).toInt();
        int what = KKSAttrType::intervalNameToId(m_value.toStringList().at(1));
        val = QString("'{%1,%2}'").arg(amount).arg(what);
        return val;
    }
    
    if(a_type == KKSAttrType::atIntervalH){
        if(m_value.toStringList().count() < 3)
            return "NULL";

        if(m_isNull)
            return "NULL";

        int hours = m_value.toStringList().at(0).toInt();
        int mins = m_value.toStringList().at(1).toInt();
        int secs = m_value.toStringList().at(2).toInt();
        val = QString("'{%1,%2,%3}'").arg(hours).arg(mins).arg(secs);
        return val;
    }

    if(a_type == KKSAttrType::atCheckList ||
       a_type == KKSAttrType::atCheckListEx)
    {
        val = QString("'{%1}'").arg(value());
        return val;
    }

    if (a_type == KKSAttrType::atPoints)
    {
        QList<QVariant> pList = m_value.toList();
        if (pList.isEmpty())
            return "NULL";

        int np = pList.count();
        val = QString ("'{");
        for (int i=0; i<np; i++)
        {
            QPointF p = pList[i].toPointF ();
            if (p.isNull())
                return "NULL";
            val += QString("\"(%1,%2)\"").arg (p.x()).arg(p.y());
            if (i<np-1)
                val+= QString (",");
        }
        val += QString ("}'");
        return val;
    }

    if (a_type == KKSAttrType::atRecordColor ||
        a_type == KKSAttrType::atRecordTextColor)
    {
        QColor vCol (m_value.value<QColor>());
        val = QString::number (vCol.rgba());
        return val;
    }
    
    if (a_type == KKSAttrType::atString ||
        a_type == KKSAttrType::atFixString ||
        a_type == KKSAttrType::atText ||
        a_type == KKSAttrType::atXMLDoc ||
        a_type == KKSAttrType::atComplex)
    {
        QString sVal (value());
        QString escVal (sVal);
        escVal.replace("'", "''");
        escVal.replace("\\", "\\\\");
        escVal.replace("\"", "\\\"");
        escVal.prepend("'");
        escVal.append ("'");
        return escVal;
    }

    if(a_type == KKSAttrType::atInt){
        val += QString("'%1'").arg(value());
        return val;
    }

    if(a_type == KKSAttrType::atInt64){
        val += QString("'%1'").arg(value());
        return val;
    }

    if(a_type == KKSAttrType::atUUID){
        val += QString("'%1'").arg(value());
        return val;
    }

    if(isLiteral())
        val += QString("'%1'").arg(value());
    else
        val = value();

    return val;
}

////////////////////////////////////////////////////////////////////////
// Name:       KKSValue::getType()
// Purpose:    Implementation of KKSValue::getType()
// Return:     int
////////////////////////////////////////////////////////////////////////

int KKSValue::type(void) const
{
   return m_type;
}

void KKSValue::setColumnValue(const QString & _columnValue)
{
    m_columnValue = _columnValue;
}

const QString & KKSValue::columnValue() const
{
    return m_columnValue;
}

bool KKSValue::isLiteral() const
{
    return m_isLiteral;
}

bool KKSValue::isNull() const
{
    return m_isNull;
}

#ifdef Q_WS_WIN
//for web
int KKSValue::setValue_w(const std::string & value, int type)
{
    return setValue(QString::fromUtf8(value.c_str()), type);
}

std::string KKSValue::value_w() const
{
    std::string s;
    s = value().toUtf8();
    return s;
}	

void KKSValue::setColumnValue_w(const std::string & columnValue)
{
    return setColumnValue(QString::fromUtf8(columnValue.c_str()));
}

std::string KKSValue::columnValue_w() const
{
    std::string s;
    s = columnValue().toUtf8();
    return s;
}
#endif
