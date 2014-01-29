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
    vList (QList<int>()),
    cList(QList<int>()),
    ioList (QList<int>()),
    sList(QList<int>()),
    plList(QList<int>())
{

}

KKSHistogram::KKSHistogram(const QMap<int, QPair<double, double> >& data, double xmin, double xmax, int n)
    : KKSRecord(), dHist (data),
    m_xmin (xmin),
    m_xmax (xmax),
    m_num (n),
    m_isEmpty(false),

    scList (QList<int>()),
    vList (QList<int>()),
    cList(QList<int>()),
    ioList (QList<int>()),
    sList(QList<int>()),
    plList(QList<int>())
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
    vList (orig.vList),
    cList(orig.cList),
    ioList (orig.ioList),
    sList(orig.sList),
    plList (orig.plList)
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
    
    //�������� ��������� �����������
    outHist << (int)id() << sep << m_xmin << sep << m_xmax << sep << m_num << sep;

    //�������� �����������
    for (QMap<int, QPair<double, double> >::const_iterator p=dHist.constBegin();
            p != dHist.constEnd();
            ++p)
    {
        outHist << p.key() << sep << p.value().first << sep << p.value().second << sep;
    }

    //����� ���� ���������� � �������� ��������
    
    //��������
    int nsc = this->scList.count();
    outHist << QString ("scenario begins %1").arg (nsc) << sep;
    for (int i=0; i<nsc; i++)
        outHist << scList[i] << sep;
    outHist << QString ("scenario end") << sep;

    //��������
    int nvar = this->vList.count();
    outHist << QString ("variant begins %1").arg (nvar) << sep;
    for (int i=0; i<nvar; i++)
        outHist << vList[i] << sep;
    outHist << QString ("variant end") << sep;
    
    //���������
    int ncat = this->cList.count();
    outHist << QString ("categories begins %1").arg (ncat) << sep;
    for (int i=0; i<ncat; i++)
        outHist << cList[i] << sep;
    outHist << QString ("categories end") << sep;

    //��
    int nio = this->ioList.count();
    outHist << QString ("IO begins %1").arg (nio) << sep;
    for (int i=0; i<nio; i++)
        outHist << ioList[i] << sep;
    outHist << QString ("IO end") << sep;

    //services
    int nserv = this->sList.count();
    outHist << QString ("services begins %1").arg (nserv) << sep;
    for (int i=0; i<nserv; i++)
        outHist << sList[i] << sep;
    outHist << QString ("services end") << sep;

    //partition lows
    int npl = this->plList.count();
    outHist << QString ("part_lows begins %1").arg (npl) << sep;
    for (int i=0; i<npl; i++)
        outHist << plList[i] << sep;
    outHist << QString ("part_lows end") << sep;
    
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
    
    //����� � ������ ���� ������ �� ��������.
    //������ �� ����� ��������� ����� QStringList
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
        this->vList.clear ();
        for (int i=0; i<nvar; i++)
        {
            QString stdIdVar = sParList[mVar+1+i];
            int idVar = stdIdVar.toInt();
            vList.append(idVar);
        }
    }
    
    QRegExp rCat (QString("categories\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mCat = sParList.indexOf (rCat);
    if(mCat >= 0){
        int numCat = sParList[mCat].lastIndexOf (" ");
        int nCat = sParList[mCat].mid (numCat).toInt();
        this->cList.clear ();
        for (int i=0; i<nCat; i++)
        {
            QString stdIdCat = sParList[mCat+1+i];
            int idCat = stdIdCat.toInt();
            cList.append(idCat);
        }
    }

    QRegExp rIO (QString("IO\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mIO = sParList.indexOf (rIO);
    if(mIO >= 0){
        int numIO = sParList[mIO].lastIndexOf (" ");
        int nIO = sParList[mIO].mid (numIO).toInt();
        this->ioList.clear ();
        for (int i=0; i<nIO; i++)
        {
            QString stdIdIO = sParList[mIO+1+i];
            int idIO = stdIdIO.toInt();
            ioList.append(idIO);
        }
    }
    
    QRegExp rServ (QString("services\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mServ = sParList.indexOf (rServ);
    if(mServ >= 0){
        int numServ = sParList[mServ].lastIndexOf (" ");
        int nServ = sParList[mServ].mid (numServ).toInt();
        this->sList.clear ();
        for (int i=0; i<nServ; i++)
        {
            QString stdIdServ = sParList[mServ+1+i];
            int idServ = stdIdServ.toInt();
            sList.append(idServ);
        }
    }

    QRegExp rPL (QString("part_lows\\ begins\\ [0-9]*"), Qt::CaseInsensitive);
    int mPL = sParList.indexOf (rPL);
    if(mPL >= 0){
        int numPL = sParList[mPL].lastIndexOf (" ");
        int nPL = sParList[mPL].mid (numPL).toInt();
        this->plList.clear ();
        for (int i=0; i<nPL; i++)
        {
            QString stdIdPL = sParList[mPL+1+i];
            int idPL = stdIdPL.toInt();
            plList.append(idPL);
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
    return scList;
}

void KKSHistogram::setScenario (const QList<int>& idSc)
{
    scList = idSc;
}

const QList<int>& KKSHistogram::getVariant (void) const
{
    return vList;
}

void KKSHistogram::setVariant (const QList<int>& idv)
{
    vList = idv;
}

const QList<int>& KKSHistogram::getCategories(void) const
{
    return cList;
}

void KKSHistogram::setCategories (const QList<int>& idCat)
{
    cList = idCat;
}

const QList<int>& KKSHistogram::getIO(void) const
{
    return ioList;
}

void KKSHistogram::setIO (const QList<int>& idIO)
{
    ioList = idIO;
}

const QList<int>& KKSHistogram::getServices(void) const
{
    return sList;
}

void KKSHistogram::setServices(const QList<int>& idServ)
{
    sList = idServ;
}

const QList<int>& KKSHistogram::getPartLows(void) const
{
    return plList;
}

void KKSHistogram::setPartLows (const QList<int>& idPartLows)
{
    plList = idPartLows;
}


void KKSHistogram::clear (void)
{
    dHist.clear ();
}

void KKSHistogram::setValue (int key, QPair<double, double> val)
{
    dHist.insert (key, val);
}
