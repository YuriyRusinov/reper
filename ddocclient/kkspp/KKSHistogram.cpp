/* 
 * File:   KKSHistogram.cpp
 * Author: yuriyrusinov
 * 
 * Created on 12 Декабрь 2013 г., 13:12
 */
#include <QString>
#include <QBuffer>
#include <QDataStream>
#include <QTextStream>
#include <QtDebug>

#include "KKSCategory.h"
#include "KKSType.h"
#include "KKSObject.h"
#include "KKSHistogram.h"

KKSHistogram::KKSHistogram()
    : KKSRecord(), dHist(QMap<int, double>()),
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
    : KKSRecord(), dHist (data),
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
    : KKSRecord(orig), dHist (orig.dHist),
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
    //QByteArray bh;
    QTextStream outHist;// (&bh, QIODevice::WriteOnly);
    outHist.setString (&resStr);
    outHist << m_xmin << sep << m_xmax << sep << m_num << sep;

    //int i (0);
    for (QMap<int, double>::const_iterator p=dHist.constBegin();
            p != dHist.constEnd();
            ++p)
    {
        outHist << p.key() << sep << p.value() << sep;
        //resStr += QString("%1%3%2%3").arg (p.key()).arg (p.value()).arg (sep);
        //i++;
        
    }
    outHist << idScenario << sep << idVariant << sep;
    if (!c || !c->tableCategory() || !c->type() || !io)
        return resStr;
    outHist << c->id() << sep << c->name () << sep << c->type()->id() << sep << c->type()->name() << sep;
    outHist << c->tableCategory()->id() << sep << c->tableCategory()->name() << sep << c->tableCategory()->type()->id() << sep << c->tableCategory()->type()->name() << sep;
    outHist << io->id() << sep << io->name () << sep << idSource << sep << idReceiver;
    
    //resStr = QString (bh);
    //qDebug () << __PRETTY_FUNCTION__ << bh << resStr;// << outH.data();
    return resStr;
}
//ksa
bool KKSHistogram::fromString(const QString & str)
{
    qDebug () << __PRETTY_FUNCTION__ << str;
    QString sep ("^~^~^");
    QStringList sParList = str.split(sep,QString::KeepEmptyParts,Qt::CaseInsensitive);
    if (sParList.isEmpty())
        return false;

    QByteArray bStr = str.toLocal8Bit();
    QBuffer hBuffer (&bStr);
    if (!hBuffer.open(QIODevice::ReadOnly))
        return false;
    QTextStream hIn (&hBuffer);
    QString sep1;
    hIn >> m_xmin;
    hIn >> sep1;
    qDebug () << __PRETTY_FUNCTION__ << sep1;
    hIn >> m_xmax;
    hIn >> sep1;
    hIn >> m_num;
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
    int idCat;
    hIn >> idCat >> sep;
    QString cName;
    hIn >> cName >> sep;
    int idType;
    hIn >> idType >> sep;
    QString cTypeName;
    hIn >> cTypeName >> sep;
    KKSType * cType = new KKSType (idType, cTypeName);
    const KKSCategory * cat = new KKSCategory (idCat, cName, cType);
    int idTCat;
    QString ctName;
    hIn >> idTCat >> sep;
    hIn >> ctName >> sep;
    int idTType;
    QString ctTypeName;
    hIn >> idTType >> sep;
    hIn >> ctTypeName >> sep;
    KKSType * ctType = new KKSType (idTType, ctTypeName);
    KKSCategory * ct = new KKSCategory (idTCat, ctName, ctType);
    (const_cast<KKSCategory *>(cat))->setTableCategory(ct);
    setCategory (cat);
    int idObj;
    QString objName;
    hIn >> idObj >> sep;
    hIn >> objName >> sep;
    const KKSObject * obj = new KKSObject (idObj, const_cast<KKSCategory *>(cat), objName);
    setSrcObject (obj);
    hIn >> idSource >> sep;
    hIn >> idReceiver;
 
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
