/* 
 * File:   KKSHistogram.cpp
 * Author: rusinov
 * 
 * Created on 12 Декабрь 2013 г., 13:12
 */
#include <QString>
#include <QBuffer>
#include <QDataStream>

#include "KKSCategory.h"
#include "KKSType.h"
#include "KKSObject.h"
#include "KKSHistogram.h"

KKSHistogram::KKSHistogram()
    : dHist(QMap<int, double>()),
    m_xmin(0.0),
    m_xmax(0.0),
    m_num(-1),
    m_isEmpty(true),
    idScenario (-1),
    idVariant (-1),
    c (0),
    io (0),
    idSource (-1),
    idReceiver (-1)
{

}

KKSHistogram::KKSHistogram(const QMap<int, double>& data, double xmin, double xmax, int n)
    : dHist (data),
    m_xmin (xmin),
    m_xmax (xmax),
    m_num (n),
    m_isEmpty(false),
    idScenario (-1),
    idVariant (-1),
    c (0),
    io (0),
    idSource (-1),
    idReceiver (-1)
{
    if(EQUAL_F(m_xmin, 0.0) && EQUAL_F(m_xmax, 0.0) && m_num == -1)
        m_isEmpty = true;
}

KKSHistogram::KKSHistogram(const KKSHistogram& orig)
    : dHist (orig.dHist),
    m_xmin (orig.m_xmin),
    m_xmax (orig.m_xmax),
    m_num (orig.m_num),
    m_isEmpty(orig.m_isEmpty),
    idScenario (orig.idScenario),
    idVariant (orig.idVariant),
    c (orig.c),
    io (orig.io),
    idSource (orig.idSource),
    idReceiver (orig.idReceiver)
{
    if (c)
        c->addRef();
    if (io)
        io->addRef();
}

KKSHistogram::~KKSHistogram()
{
    if (c)
        c->release();
    if (io)
        io->release();
}

const QMap<int, double>& KKSHistogram::getVec (void) const
{
    return dHist;
}

void KKSHistogram::setVec (const QMap<int, double>& data)
{
    dHist = data;

    m_isEmpty = false;
    if(EQUAL_F(m_xmin, 0.0) && EQUAL_F(m_xmax, 0.0) && m_num == -1)
        m_isEmpty = true;

}

void KKSHistogram::setRange (double xmin, double xmax)
{
    m_xmin = xmin;
    m_xmax = xmax;

    m_isEmpty = false;
    if(EQUAL_F(m_xmin, 0.0) && EQUAL_F(m_xmax, 0.0) && m_num == -1)
        m_isEmpty = true;

}

double KKSHistogram::getXMin (void) const
{
    return m_xmin;
}

double KKSHistogram::getXMax (void) const
{
    return m_xmax;
}

int KKSHistogram::size (void) const
{
    return m_num;
}

void KKSHistogram::setSize (int n)
{
    m_num = n;
    
    m_isEmpty = false;
    if(EQUAL_F(m_xmin, 0.0) && EQUAL_F(m_xmax, 0.0) && m_num == -1)
        m_isEmpty = true;

}

//ksa
QString KKSHistogram::toString() const
{
    QString resStr;
    if (m_isEmpty)
        return resStr;
    QString sep ("^~^~^");
    resStr = QString ("%1%4%2%4%3")
              .arg (m_xmin)
              .arg (m_xmax)
              .arg (m_num)
              .arg (sep);
    int i (0);
    for (QMap<int, double>::const_iterator p=dHist.constBegin();
            p != dHist.constEnd();
            ++p)
    {
        resStr += QString("%1%3%2%3").arg (p.key()).arg (p.value()).arg (sep);
        i++;
        
    }
    resStr += QString("%1%7%2%7%3%7%8%7%9%7%10%7%4%7%5%7%6")
            .arg (idScenario)
            .arg (idVariant)
            .arg (c ? QString::number (c->id()) : QString::number (-1))
            .arg (io ? QString::number (io->id()) : QString::number (-1))
            .arg (idSource)
            .arg (idReceiver)
            .arg (sep)
            .arg (c ? c->name() : QString())
            .arg (c ? c->type()->id() : -1)
            .arg (c ? c->type()->name() : QString());
    return resStr;
}
//ksa
bool KKSHistogram::fromString(const QString & str)
{
    QString sep ("^~^~^");
    QStringList sParList = str.split(sep,QString::KeepEmptyParts,Qt::CaseInsensitive);
    if (sParList.isEmpty())
        return false;
    bool ok;
    m_xmin = sParList[0].toDouble(&ok);
    if (!ok)
        return false;
    m_xmax = sParList[1].toDouble (&ok);
    if (!ok)
    {
        m_xmin = 0.0;
        return false;
    }
    m_num = sParList[2].toInt(&ok);
    if (!ok)
    {
        m_xmin = 0.0;
        m_xmax = 0.0;
        return false;
    }
    for (int i=0; i<m_num; i++)
    {
        int key = sParList[2*i+3].toInt (&ok);
        if (!ok)
        {
            m_xmin = 0.0;
            m_xmax = 0.0;
            m_num = -1;
            return false;
        }
        double dVal = sParList[2*i+4].toDouble (&ok);
        if (!ok)
        {
            m_xmin = 0.0;
            m_xmax = 0.0;
            m_num = -1;
            return false;
        }
        dHist.insert(key, dVal);
    }
/*    QByteArray bStr = str.toAscii();
    QBuffer hBuffer (&bStr);
    if (!hBuffer.open(QIODevice::ReadOnly))
        return false;
    QDataStream hIn (&hBuffer);
    QString sep, sep1;
    hIn >> m_xmin >> sep >> m_xmax >> sep1 >> m_num;
    dHist.clear ();
    for (int i=0; i<m_num; i++)
    {
        int key;
        double val;
        hIn >> key >> sep >> val >> sep1;
        dHist.insert (key, val);
    }
    hIn >> idScenario >> sep;
    hIn >> idVariant >> sep;
 */
    return true;
}

bool KKSHistogram::isEmpty() const 
{
    return m_isEmpty;
}

int KKSHistogram::getScenario (void) const
{
    return idScenario;
}

void KKSHistogram::setScenario (int idSc)
{
    idScenario = idSc;
}

int KKSHistogram::getVariant (void) const
{
    return idVariant;
}

void KKSHistogram::setVariant (int idv)
{
    idVariant = idv;
}

const KKSCategory * KKSHistogram::category (void) const
{
    return c;
}

void KKSHistogram::setCategory (const KKSCategory * cat)
{
    if (c == cat)
        return;

    if (c)
        c->release ();
    
    c = cat;
    
    if (c)
        c->addRef();
}

const KKSObject * KKSHistogram::srcObject (void) const
{
    return io;
}

void KKSHistogram::setSrcObject (const KKSObject * iosrc)
{
    if (io == iosrc)
        return;
    
    if (io)
        io->release ();
    
    io = iosrc;
    
    if (io)
        io->addRef ();
}

int KKSHistogram::getSource (void) const
{
    return idSource;
}

void KKSHistogram::setSource (int ids)
{
    idSource = ids;
}

int KKSHistogram::getReceiver (void) const
{
    return idReceiver;
}

void KKSHistogram::setReceiver (int idr)
{
    idReceiver = idr;
}
