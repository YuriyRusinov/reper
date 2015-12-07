/* 
 * File:   KKSBinaryWidget.cpp
 * Author: yuriyrusinov
 * 
 * Created on 26 Ќо€брь 2014 г., 18:47
 * содержит бинарные данные по изображени€м.
 */
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QtDebug>

#include <QDataStream>
#include <QTextStream>

#include <KKSValue.h>
#include "KKSBinaryWidget.h"
#include "KKSAttrValue.h"

KKSBinaryWidget::KKSBinaryWidget(const KKSAttrValue*attr, KKSIndAttrClass isSys, QWidget * parent, Qt::WindowFlags flags)
    : QLabel (parent, flags),
    KKSAttrWidget(attr, isSys),
    binaryData (QByteArray())
{
    KKSValue val = attr->value();
    QVariant V = val.valueVariant();
    binaryData = V.toByteArray();
    setImage ();
}

KKSBinaryWidget::~KKSBinaryWidget()
{
}

void KKSBinaryWidget :: openFile (void)
{
    //qDebug () << __PRETTY_FUNCTION__;
    QString filter (tr("All files (*.*)"));
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Data File"),
                                                    QDir::currentPath(),
                                                    filter);
    if (fileName.isEmpty())
        return;

    QFile fData (fileName);
    fData.open (QIODevice::ReadOnly);
    qint64 nBytes = fData.bytesAvailable();
    Q_UNUSED (nBytes)
    binaryData.clear();
    uint us = sizeof(unsigned int);
    char * azrev = new char [us];
    qint64 naz = fData.read (azrev, us);
    QByteArray az;// = new char [us];
    for ( int i=0; i<naz; i++)
    {
        az += QByteArray::number ((unsigned)azrev[naz-i-1], 16);
        //qDebug () << __PRETTY_FUNCTION__ << (unsigned)azref[naz-i-1] << az;
        //QString::fromRawData (QChar(az));
        //qDebug () << __PRETTY_FUNCTION__ << az ;//<< QByteArray::fromHex(QByteArray::fromRawData(az, us));
    }
    //QDataStream fDD (az);//QByteArray::fromHex (QByteArray::az));//&fData);
    bool ok;
    unsigned int azimuth_dd = az.toUInt (&ok, 16);
    delete [] azrev;
    binaryData.append (QByteArray::number(azimuth_dd));
    
    char * elev_rev = new char [us];
    qint64 nelev = fData.read (elev_rev, us);
    QByteArray elev;
    for (int i=nelev-1; i>=0; i--)
        elev += QByteArray::number ((unsigned)elev_rev[i], 16);
    unsigned int elevation_dd = elev.toUInt (&ok, 16);
    delete [] elev_rev;
    binaryData.append  (" ");
    binaryData.append (QByteArray::number(elevation_dd));
    
    char * rows_rev = new char [us];
    qint64 nr = fData.read (rows_rev, us);
    QByteArray rows;
    for (int i=nr-1; i>=0; i--)
        rows += QByteArray::number ((unsigned)rows_rev[i], 16);
    unsigned int nWidth = rows.toUInt (&ok, 16);
    delete [] rows_rev;
    binaryData.append  (" ");
    binaryData.append (QByteArray::number (nWidth));

    char * cols_rev = new char [us];
    qint64 nc = fData.read (cols_rev, us);
    QByteArray cols;
    for (int i=nc-1; i>=0; i--)
        cols += QByteArray::number ((unsigned)cols_rev[i], 16);
    unsigned int nHeight = rows.toUInt (&ok, 16);
    delete [] cols_rev;
    binaryData.append  (" ");
    binaryData.append (QByteArray::number (nHeight));
    
    char * bits_rev = new char [us];
    qint64 nbits = fData.read (bits_rev, us);
    QByteArray bits;
    for (int i=nbits-1; i>=0; i--)
        bits += QByteArray::number ((unsigned)bits_rev[i], 16);
    unsigned int nBitsPerLine = bits.toUInt (&ok, 16);
    delete [] bits_rev;
    binaryData.append  (" ");
    binaryData.append (QByteArray::number (nBitsPerLine));
    
    char * imData = new char[nWidth*nHeight];
    qint64 nim = fData.read (imData, nWidth*nHeight);
    Q_UNUSED (nim);
    QByteArray bIm;// (imData);
    //QFile debIm ("ddd.dat");
    //QDataStream debImage (&bIm, QIODevice::WriteOnly);
    //debIm.open (QIODevice::WriteOnly);
    int ncount (0);
    for (uint i=0; i<nWidth; i++)
    {
        for (uint ii=0; ii<nHeight; ii++)
        {
            bIm += QByteArray::number ((uchar)imData[ncount++]);
        }
        //debImage << QString("\r\n");
    }
    qDebug () << __PRETTY_FUNCTION__ << bIm;
    //for (int i=0; i<nim; i++)
    //    bIm += (unsigned char)imData[i];
    QVector<QRgb> grayscale;

    for (int i = 0; i < 256; ++i) {
        grayscale.append(qRgb(i, i, i));
    }
    
    QImage pIm (/*(uchar *)(bIm.constData()),*/ nWidth, nHeight, /*nBitsPerLine,*/ QImage::Format_ARGB32);
    ncount = 0;
    for (uint i=0; i<nWidth; i++)
    {
        for (uint j=0; j<nHeight; j++)
        {
            uint c = (uchar)imData[ncount++];
            c *= 255;
            pIm.setPixel(i, j, qRgb(c,c,c));
        }
    }
    //pIm.setColorTable(grayscale);
    //qDebug () << __PRETTY_FUNCTION__ << bIm << pIm.isNull();
    //pIm = QImage::fromData (bIm);
    binaryData.append  (" ");
    binaryData.append (bIm);
    delete [] imData;
    this->setPixmap(QPixmap::fromImage (pIm));
    //fDD >> elevation_dd;
    //char * w = 0;//new char [nBytes+1];
    //fDD.readBytes (w, l);//RawData(w, nBytes);
//    int nr = fread (w, sizeof(char), nBytes, fBin);
//    w[nBytes] = '\0';
//    binaryData.clear();
    //binaryData.append (QByteArray::fromHex(w));
    //binaryData = fName.readAll();
 
    setText (binaryData.constData());
    //qDebug () << __PRETTY_FUNCTION__ << binaryData << binaryData.length() << nBytes << nim;
    //delete [] w;
    
    QVariant v = QVariant(binaryData);
    emit valueChanged (m_av->id(), m_isSystem, v);
}

void KKSBinaryWidget :: setData (const QByteArray& ba)
{
    binaryData = ba;
    setText (binaryData.constData());
    setImage ();
}

void KKSBinaryWidget :: setImage (void)
{
    if (binaryData.isEmpty())
        return;
    if (binaryData.contains("\\x"))
        binaryData = QByteArray::fromHex(binaryData);
    qDebug () << __PRETTY_FUNCTION__ << binaryData;
    setText (binaryData.constData());
    QTextStream bIm (binaryData);
    unsigned az_dd;
    bIm >> az_dd;
    if (az_dd > 360)
        return;
    unsigned elev_dd;
    bIm >> elev_dd;
    unsigned nW, nH, nd;
    bIm >> nW;
    bIm >> nH;
    bIm >> nd;
    QImage pIm (nW, nH, QImage::Format_ARGB32);
    //bIm.readRawData(imData, nW*nH);
    QString imStr;// = bIm.read(nW*nH);
    bIm >> imStr;
    QByteArray imArr = imStr.toAscii();
    //qDebug () << __PRETTY_FUNCTION__ << imArr;
    if (imStr.isEmpty())
        return;
    int ncount (0);
    for (uint i=0; i<nW; i++)
    {
        for (uint j=0; j<nH; j++)
        {
            // = new char;// = (uchar)imData[ncount++];
            //bIm >> imData;
            //QByteArray s = QByteArray (imArr.at(ncount++));
            uint c = (uint)imStr.at(ncount++).digitValue();//s.toUInt();
            c *= 255;
            pIm.setPixel(i, j, qRgb(c,c,c));
        }
    }
    this->setPixmap(QPixmap::fromImage (pIm));

}
