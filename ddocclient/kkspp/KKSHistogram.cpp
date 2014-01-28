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
#include <QTextCodec>
#include <QRegExp>
#include <QtDebug>

#include "KKSCategory.h"
#include "KKSType.h"
#include "KKSObject.h"
#include "KKSHistogram.h"

KKSHistogram::KKSHistogram()
    : KKSRecord(), dHist(QMap<int, QPair<double, double> >()),
    m_xmin(0.0),
    m_xmax(0.0),
    m_num(-1),
    m_isEmpty(true),
    scList (QList<int>()),
    varList (QList<int>()),
    catList(KKSMap<qint64, const KKSCategory *>()),
    ioList (KKSList<const KKSObject *>())
{

}

KKSHistogram::KKSHistogram(const QMap<int, QPair<double, double> >& data, double xmin, double xmax, int n)
    : KKSRecord(), dHist (data),
    m_xmin (xmin),
    m_xmax (xmax),
    m_num (n),
    m_isEmpty(false),
    scList (QList<int>()),
    varList (QList<int>()),
    catList(KKSMap<qint64, const KKSCategory *>()),
    ioList (KKSList<const KKSObject *>())
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
    scList (orig.scList),
    varList (orig.varList),
    catList(orig.catList),
    ioList (orig.ioList)
{
}

KKSHistogram::~KKSHistogram()
{
}

const QMap<int, QPair<double, double> >& KKSHistogram::getVec (void) const
{
    return dHist;
}

void KKSHistogram::setVec (const QMap<int, QPair<double, double> >& data)
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
    outHist << id() << sep << m_xmin << sep << m_xmax << sep << m_num << sep;

    //int i (0);
    //
    int nsc = this->scList.count();
    outHist << QString ("scenario begins %1").arg (nsc) << sep;
    for (int i=0; i<nsc; i++)
        outHist << scList[i] << sep;
    outHist << QString ("scenario end") << sep;
    int nvar = this->varList.count();
    outHist << QString ("variant begins %1").arg (nvar) << sep;
    for (int i=0; i<nvar; i++)
        outHist << varList[i] << sep;
    outHist << QString ("variant end") << sep;
    int nCat = catList.count();
    outHist << QString ("categories begins %1").arg (nCat) << sep;
    for (KKSMap<qint64, const KKSCategory *>::const_iterator pc = catList.constBegin();
         pc != catList.constEnd();
         pc++)
    {
        const KKSCategory * c = pc.value();//catList[i];
        outHist << (c ? c->id() : -1) << sep
                << (c ? c->name () : QString()) << sep
                << (c ? c->type()->id() : -1) << sep
                << (c ? c->type()->name() : QString()) << sep;

        outHist << (c && c->tableCategory() ? c->tableCategory()->id() : -1) << sep
                << (c && c->tableCategory() ? c->tableCategory()->name() : QString())<< sep
                << (c && c->tableCategory() ? c->tableCategory()->type()->id() : -1) << sep
                << (c && c->tableCategory() ? c->tableCategory()->type()->name() : QString()) << sep;
    }
    outHist << QString ("categories end") << sep;

    int nio = ioList.count();
    outHist << QString ("IO begins %1").arg (nio) << sep;
    for (int i=0; i<nio; i++)
    {
        const KKSObject * io = ioList[i];
        outHist << (io ? io->id() : -1) << sep
                << (io ? io->name () : QString()) << sep
                << (io ? io->category()->id() : -1) << sep;
    }
    outHist << QString ("IO end") << sep;
    for (QMap<int, QPair<double, double> >::const_iterator p=dHist.constBegin();
            p != dHist.constEnd();
            ++p)
    {
        outHist << p.key() << sep << p.value().first << sep << p.value().second << sep;
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
    int idHist;
    hIn >> idHist;
    setId (idHist);
    //QString sep1;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> m_xmin;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> m_xmax;
    hIn.seek (hIn.pos()+sep.length());// >> sep1;
    hIn >> m_num;
//    int mStart = 6;//m_num+26;
//    int nPars = sParList.count();
    hIn.seek (hIn.pos()+sep.length());
    QRegExp rScen (QString("scenario\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mScenario = sParList.indexOf (rScen);
    //qDebug () << __PRETTY_FUNCTION__ << mScenario << sParList[mScenario];//idHist;
    int numInd = sParList[mScenario].lastIndexOf(" ");
    int nsc = sParList[mScenario].mid (numInd).toInt();
    scList.clear ();
    for (int i=0; i<nsc; i++)
    {
        QString stdIdSc = sParList[mScenario+1+i];
        int idScenario = stdIdSc.toInt();
        scList.append (idScenario);
    }
    QRegExp rVar (QString("variant\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mVar = sParList.indexOf (rVar);
    int numVar = sParList[mVar].lastIndexOf (" ");
    int nvar = sParList[mVar].mid (numVar).toInt();
    this->varList.clear ();
    for (int i=0; i<nvar; i++)
    {
        QString stdIdVar = sParList[mVar+1+i];
        int idVar = stdIdVar.toInt();
        varList.append(idVar);
    }
    QRegExp catReg(QString ("categories\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mCat = sParList.indexOf(catReg);
    int numCat = sParList[mCat].lastIndexOf (" ");
    int nCat= sParList[mCat].mid (numCat).toInt();
    catList.clear ();
    for (int i=0; i<8*nCat; i+=8)
    {
        int idCat = sParList[mCat+1+i].toInt ();
        QString cName = sParList[mCat+2+i];
        int idType = sParList[mCat+3+i].toInt ();
        QString cTypeName = sParList[mCat+4+i];
        KKSType * cType = new KKSType (idType, cTypeName);
        KKSCategory * c = new KKSCategory (idCat, cName, cType);
        int idTCat = sParList[mCat+5+i].toInt ();
        QString cTName = sParList[mCat+6+i];
        int idTType = sParList[mCat+7+i].toInt ();
        QString cTTypeName = sParList[mCat+8+i];
        KKSType * ctType = new KKSType (idTType, cTTypeName);
        KKSCategory * ct = new KKSCategory (idTCat, cTName, ctType);
        c->setTableCategory(ct);
        catList.insert (idCat, c);
        c->release ();
    }
    QRegExp ioReg(QString("IO\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mIO = sParList.indexOf (ioReg);
    if (mIO > 0)
    {
        int numIO = sParList[mIO].lastIndexOf (" ");
        int nIO = sParList[mIO].mid (numIO).toInt();
        ioList.clear ();
        for (int i=0; i<3*nIO; i+=3)
        {
            int idIO = sParList[mIO+i+1].toInt ();
            QString ioName = sParList[mIO+i+2];
            int idIOCat = sParList[mIO+i+3].toInt();
            KKSCategory * c = new KKSCategory (*catList.value (idIOCat));
            const KKSObject * io = new KKSObject (idIO, c, ioName);
            ioList.append (io);
            io->release ();
        }
    }
/*    hIn >> idScenario;
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
*/    
    dHist.clear ();
    for (int i=0; i<m_num && !hIn.atEnd(); i++)
    {
        int key;
        QPair<double, double> val;
        hIn >> key;
        hIn.seek (hIn.pos()+sep.length());

        hIn >> val.first;
        if (i < m_num-1)
            hIn.seek (hIn.pos()+sep.length());
        hIn >> val.second;
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

const QList<int>& KKSHistogram::getScenario (void) const
{
    return scList;//idScenario;
}

void KKSHistogram::setScenario (const QList<int>& idSc)
{
    scList = idSc;
}

const QList<int>& KKSHistogram::getVariant (void) const
{
    return varList;
}

void KKSHistogram::setVariant (const QList<int>& idv)
{
    varList = idv;
}

const KKSMap<qint64, const KKSCategory *>& KKSHistogram::category (void) const
{
    return catList;
}

void KKSHistogram::setCategory (const KKSMap<qint64, const KKSCategory *>& cat)
{
    catList = cat;
}

const KKSList<const KKSObject *>& KKSHistogram::srcObject (void) const
{
    return ioList;
}

void KKSHistogram::setSrcObject (const KKSList<const KKSObject *>& iosrc)
{
    ioList == iosrc;
}
/*
const QList<int>& KKSHistogram::getSource (void) const
{
    return sourceList;
}

void KKSHistogram::setSource (const QList<int>& ids)
{
    sourceList = ids;
}

const QList<int>& KKSHistogram::getReceiver (void) const
{
    return recvList;//idReceiver;
}

void KKSHistogram::setReceiver (const QList<int>& idr)
{
    recvList = idr;
}
 */

void KKSHistogram::clear (void)
{
    dHist.clear ();
}

void KKSHistogram::setValue (int key, QPair<double, double> val)
{
    dHist.insert (key, val);
}
