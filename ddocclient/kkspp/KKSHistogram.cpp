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

#include "KKSHistogram.h"
#include "KKSType.h"

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
    if(dHist.count() != m_num){
        dHist.clear();
        m_xmin = 0.0;
        m_xmax = 0.0;
        m_num = -1;
    }

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

    if(dHist.isEmpty())
        setSize(m_num);
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
    if(data.isEmpty()){
        //dHist.clear();
        //m_xmin = 0.0;
        //m_xmax = 0.0;
        //m_num = -1;
        setSize(m_num);
    }
    else{
        dHist = data;
        //m_num = dHist.count();
    }


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
    
    if(dHist.isEmpty()){
        //clear ();
        
        for (int i=0; i<m_num; i++)
        {
            //double x = xmin + i*dx;
            setValue (i, QPair<double, double>(0.0, 0.0));
        }
    }

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
    
    //основные параметры гистограммы
    outHist << (int)id() << sep << m_xmin << sep << m_xmax << sep << m_num << sep;

    //значения гистограммы
    for (QMap<int, QPair<double, double> >::const_iterator p=dHist.constBegin();
            p != dHist.constEnd();
            ++p)
    {
        outHist << p.key() << sep << p.value().first << sep << p.value().second << sep;
        //resStr += QString("%1%3%2%3").arg (p.key()).arg (p.value()).arg (sep);
        //i++;

    }

    //далее идет информация о заданных фильтрах
    
    //сценарии
    int nsc = this->scList.count();
    outHist << QString ("scenario begins %1").arg (nsc) << sep;
    for (int i=0; i<nsc; i++)
        outHist << scList[i] << sep;
    outHist << QString ("scenario end") << sep;

    //варианты
    int nvar = this->varList.count();
    outHist << QString ("variant begins %1").arg (nvar) << sep;
    for (int i=0; i<nvar; i++)
        outHist << varList[i] << sep;
    outHist << QString ("variant end") << sep;
    
    //категории
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

    //ИО
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
    
    return QString (resStr.toLocal8Bit());
}

//ksa
bool KKSHistogram::fromString(const QString & str)
{
    qDebug () << __PRETTY_FUNCTION__ << str;
    QString sep ("^~^~^");

    QStringList sParList = str.split(sep,QString::KeepEmptyParts,Qt::CaseInsensitive);
    if (sParList.count() <= 0)
        return false;

    QByteArray bStr = str.toLocal8Bit();
    QBuffer hBuffer (&bStr);
    if (!hBuffer.open(QIODevice::ReadOnly))
        return false;

    QTextStream hIn (&hBuffer);
    hIn.setAutoDetectUnicode(true);
    int idHist;
    hIn >> idHist;
    setId ((qint64)idHist);
    //QString sep1;
    //hIn.seek (hIn.pos()+sep.length());
    //hIn >> m_xmin;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> m_xmin;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> m_xmax;
    hIn.seek (hIn.pos()+sep.length());
    hIn >> m_num;
    hIn.seek (hIn.pos()+sep.length());

    dHist.clear ();
    for (int i=0; i<m_num && !hIn.atEnd(); i++)
    {
        int key;
        QPair<double, double> val;
        hIn >> key;
        hIn.seek (hIn.pos()+sep.length());

        hIn >> val.first;
        hIn.seek (hIn.pos()+sep.length());

        hIn >> val.second;
        hIn.seek (hIn.pos()+sep.length());

        dHist.insert (key, val);
    }
    
    //далее в строке идут данные по фильтрам.
    //Однако их лучше разобрать через QStringList
    QRegExp rScen (QString("scenario\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mScenario = sParList.indexOf (rScen);
    if(mScenario >= 0){
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
    }
    
    QRegExp rVar (QString("variant\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mVar = sParList.indexOf (rVar);
    if(mVar >= 0){
        int numVar = sParList[mVar].lastIndexOf (" ");
        int nvar = sParList[mVar].mid (numVar).toInt();
        this->varList.clear ();
        for (int i=0; i<nvar; i++)
        {
            QString stdIdVar = sParList[mVar+1+i];
            int idVar = stdIdVar.toInt();
            varList.append(idVar);
        }
    }
    
    QRegExp catReg(QString ("categories\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mCat = sParList.indexOf(catReg);
    if(mCat >= 0){
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

void KKSHistogram::clear (void)
{
    dHist.clear ();
}

void KKSHistogram::setValue (int key, QPair<double, double> val)
{
    dHist.insert (key, val);
}
