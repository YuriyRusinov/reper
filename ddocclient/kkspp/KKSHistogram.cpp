/* 
 * File:   KKSHistogram.cpp
 * Author: yuriyrusinov
 * 
 * Created on 12 ������� 2013 �., 13:12
 */
#include <QString>
#include <QBuffer>
#include <QDataStream>
#include <QTextStream>
#include <QTextCodec>
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
    outHist.setCodec (QTextCodec::codecForName("UTF-8"));
    outHist << m_xmin << sep << m_xmax << sep << m_num << sep;

    //int i (0);
    outHist << idScenario << sep 
            << idVariant << sep;
    outHist << (c ? c->id() : -1) << sep 
            << (c ? c->name () : QString()) << sep
            << (c ? c->type()->id() : -1) << sep
            << (c ? c->type()->name() : QString()) << sep;

    outHist << (c && c->tableCategory() ? c->tableCategory()->id() : -1) << sep
            << (c && c->tableCategory() ? c->tableCategory()->name() : QString())<< sep
            << (c && c->tableCategory() ? c->tableCategory()->type()->id() : -1) << sep
            << (c && c->tableCategory() ? c->tableCategory()->type()->name() : QString()) << sep;

    outHist << (io ? io->id() : -1) << sep
            << (io ? io->name () : QString()) << sep
            << idSource << sep
            << idReceiver << sep;
    for (QMap<int, double>::const_iterator p=dHist.constBegin();
            p != dHist.constEnd();
            ++p)
    {
        outHist << p.key() << sep << p.value() << sep;
        //resStr += QString("%1%3%2%3").arg (p.key()).arg (p.value()).arg (sep);
        //i++;

    }
    
    //resStr = QString (bh);
    //qDebug () << __PRETTY_FUNCTION__ << bh << resStr;// << outH.data();
    return QString (resStr.toLocal8Bit());
}
//ksa
bool KKSHistogram::fromString(const QString & str)
{
    qDebug () << __PRETTY_FUNCTION__ << str;
    QString sep ("^~^~^");
    QStringList sParList = str.split(sep,QString::KeepEmptyParts,Qt::CaseInsensitive);
    if (sParList.count() <= 1)
        return false;

    QByteArray bStr = str.toLocal8Bit();
    QBuffer hBuffer (&bStr);
    if (!hBuffer.open(QIODevice::ReadOnly))
        return false;
    QTextStream hIn (&hBuffer);
    hIn.setAutoDetectUnicode(true);
    //QString sep1;
    hIn >> m_xmin;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> m_xmax;
    hIn.seek (hIn.pos()+sep.length());// >> sep1;
    hIn >> m_num;
    int mStart = 6;//m_num+26;
//    int nPars = sParList.count();
    hIn.seek (hIn.pos()+sep.length());
    hIn >> idScenario;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> idVariant;
    int idCat;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> idCat;
    hIn.seek (hIn.pos()+sep.length());
    QString cName = sParList[mStart];
    qDebug () << __PRETTY_FUNCTION__ << cName;
    hIn.seek (hIn.pos()+cName.length()+sep.length());// >> cName;
    int idType;
    hIn >> idType;
    hIn.seek (hIn.pos()+sep.length());
    QString cTypeName = sParList[mStart+2];
    qDebug () << __PRETTY_FUNCTION__ << cTypeName;
    hIn.seek (hIn.pos()+cTypeName.length()+sep.length());// >> cName;
    KKSType * cType = new KKSType (idType, cTypeName);
    const KKSCategory * cat = new KKSCategory (idCat, cName, cType);
    int idTCat;
    QString ctName = sParList[mStart+4];
    qDebug () << __PRETTY_FUNCTION__ << ctName << hIn.pos();
    hIn >> idTCat;
    qDebug () << __PRETTY_FUNCTION__ << hIn.pos();
    hIn.seek (hIn.pos()+ctName.length()+2*sep.length());
    //qDebug () << __PRETTY_FUNCTION__ << hIn.pos();
    int idTType;
    hIn >> idTType;
    hIn.seek (hIn.pos()+sep.length());
//    hIn >> ctName;
    QString ctTypeName = sParList[mStart+6];
    hIn.seek (hIn.pos()+ctTypeName.length()+sep.length());
    qDebug () << __PRETTY_FUNCTION__ << ctTypeName;
    KKSType * ctType = new KKSType (idTType, ctTypeName);
    KKSCategory * ct = new KKSCategory (idTCat, ctName, ctType);
    (const_cast<KKSCategory *>(cat))->setTableCategory(ct);
    setCategory (cat);
    int idObj;
    QString objName = sParList[mStart+8];
    hIn >> idObj;
    hIn.seek (hIn.pos()+objName.length()+sep.length());// >> objName >> sep;
    const KKSObject * obj = new KKSObject (idObj, const_cast<KKSCategory *>(cat), objName);
    setSrcObject (obj);
    hIn >> idSource;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> idReceiver;
    hIn.seek (hIn.pos()+sep.length());
    dHist.clear ();
    for (int i=0; i<m_num && !hIn.atEnd(); i++)
    {
        int key;
        double val;
        hIn >> key;
        hIn.seek (hIn.pos()+sep.length());
        hIn >> val;
        if (i < m_num-1)
            hIn.seek (hIn.pos()+sep.length());
        dHist.insert (key, val);
    }
    m_isEmpty = false;
    if(EQUAL_F(m_xmin, 0.0) && EQUAL_F(m_xmax, 0.0) && m_num == -1)
        m_isEmpty = true;
 
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

void KKSHistogram::clear (void)
{
    dHist.clear ();
}

void KKSHistogram::setValue (int key, double val)
{
    dHist.insert (key, val);
}
