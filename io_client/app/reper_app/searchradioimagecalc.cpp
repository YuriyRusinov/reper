#include <QAbstractItemModel>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QBuffer>
#include <QtDebug>

#include <gsl/gsl_fit.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multifit.h>

#include <math.h>

#include "searchradioimagefragmentform.h"
#include "searchresultsform.h"
#include "searchradioimagecalc.h"

SearchRadioImageCalc :: SearchRadioImageCalc (QObject * parent)
    : QObject (parent)
{
}

SearchRadioImageCalc :: ~SearchRadioImageCalc (void)
{
}

SearchRadioImageFragmentForm * SearchRadioImageCalc :: GUIImageView (const QImage& im, QWidget * parent, Qt::WindowFlags flags)
{
    searchImage = im;
    SearchRadioImageFragmentForm * sForm = new SearchRadioImageFragmentForm (im, parent, flags);

    connect (sForm, SIGNAL (calcParams (const QImage&, double)), this, SLOT (calculateParameters (const QImage&, double)) );
    connect (sForm, SIGNAL (searchByIm (const QImage&, double, double)), this, SLOT (searchIm (const QImage&, double, double)) );
    connect (this, SIGNAL (setVals (int, int, double)), sForm, SLOT (setResults(int, int, double)) );
    sForm->pbCalc ();

    return sForm;
}

void SearchRadioImageCalc :: calculateParameters (const QImage& im, double cVal)
{
    if (im.isNull() || qAbs (cVal) <= 0.1e-15)
        return;

    int w = im.width();
    int h = im.height();
    QPoint np (0,0);
    QPoint ep (0,0);
    QPoint sp (0,0);
    QPoint wp (0,0);
    int lf (0);
    int wf (0);
    for (int i=1; i<w-1; i++)
        for (int j=1; j<h-1; j++)
        {
            QPoint cPos = QPoint (i, j);
            QPoint prPosX = QPoint (i-1, j);
            QPoint prPosY = QPoint (i, j-1);
            QPoint nPosX = QPoint (i+1, j);
            QPoint nPosY = QPoint (i, j+1);
            int sCol = qGray (im.pixel (cPos));
            int pColX = qGray (im.pixel (prPosX));
            int pColY = qGray (im.pixel (prPosY));
            int nColX = qGray (im.pixel (nPosX));
            int nColY = qGray (im.pixel (nPosY));
            if (sCol >= cVal && (pColX < cVal || pColY < cVal || nColX < cVal || nColY < cVal))
            {
                //qDebug () << __PRETTY_FUNCTION__ << QString ("Comparison");
                if (np.x() * np.x() + np.y() * np.y() > 0)
                {
                    if (np.y() < cPos.y())
                    {
                        np = cPos;
                    }
                }
                else
                    np = cPos;
                if (sp.x() * sp.x() + sp.y() * sp.y() > 0)
                {
                    if (sp.y() > cPos.y())
                    {
                        sp = cPos;
                    }
                }
                else
                    sp = cPos;
                if (ep.x() * ep.x() + ep.y() * ep.y() > 0)
                {
                    if (ep.x() > cPos.x())
                    {
                        ep = cPos;
                    }
                }
                else
                    ep = cPos;
                if (wp.x() * wp.x() + wp.y() * wp.y() > 0)
                {
                    if (wp.x() < cPos.x())
                    {
                        wp = cPos;
                    }
                }
                else
                    wp = cPos;
            }
        }
    qDebug () << __PRETTY_FUNCTION__ << np << ep << sp << wp;
    double deltax = sp.x()-np.x();
    double deltay = sp.y()-np.y();
    lf = (int)sqrt (deltax*deltax+deltay*deltay);
    double az = atan2 (deltay, deltax);
    const double pi = 3.14159265358979;
    if (az < 0)
        az += pi;
    az *= 180/pi;
    deltax = wp.x()-ep.x();
    deltay = wp.y()-ep.y();
    wf = (int)sqrt (deltax*deltax+deltay*deltay);
    emit setVals (lf, wf, az);
}

QSize SearchRadioImageCalc :: imageShipParameters (const QImage& sIm) const
{
    if (sIm.isNull() )
        return QSize ();

    QVector<QPoint> borderPointsS;
    int nSW = sIm.width ();
    int nSH = sIm.height ();
    int qColGrayMax = qGray (QColor (0, 0, 0).rgb());
    for (int ii=0; ii<nSW; ii++)
        for (int jj=0; jj<nSH; jj++)
        {
            QRgb pCol = sIm.pixel (ii, jj);
            int pCCol = qGray (pCol);
            if (pCCol > qColGrayMax)
                qColGrayMax = pCCol;
        }
    for (int ii=1; ii<nSW-1; ii++)
        for (int jj=1; jj<nSH-1; jj++)
        {
            QRgb pCol = sIm.pixel (ii, jj);
            QRgb pPDiagC = sIm.pixel (ii-1, jj-1);
            QRgb pPDiagC1 = sIm.pixel (ii-1, jj+1);
            QRgb pPRow = sIm.pixel (ii-1, jj);
            QRgb pPCol = sIm.pixel (ii, jj-1);
            QRgb pNDiagC = sIm.pixel (ii+1, jj+1);
            QRgb pNDiagC1 = sIm.pixel (ii-1, jj+1);
            QRgb pNRow = sIm.pixel (ii+1, jj);
            QRgb pNCol = sIm.pixel (ii, jj+1);
            int pColGray = qGray (pCol);
            int pColPDiagC = qGray (pPDiagC);
            int pColPDiagC1 = qGray (pPDiagC1);
            int pColPRow = qGray (pPRow);
            int pColPCol = qGray (pPCol);
            int pColNDiagC = qGray (pNDiagC);
            int pColNDiagC1 = qGray (pNDiagC1);
            int pColNRow = qGray (pNRow);
            int pColNCol = qGray (pNCol);
            if (pColGray >= qColGrayMax/2 &&
                    (pColPDiagC < qColGrayMax/2 || pColPDiagC1 < qColGrayMax/2 || pColPRow < qColGrayMax/2 || pColPCol < qColGrayMax/2 ||
                     pColNDiagC < qColGrayMax/2 || pColNDiagC1 < qColGrayMax/2 || pColNRow  < qColGrayMax/2 || pColNCol < qColGrayMax/2) 
               )
                borderPointsS.append (QPoint (ii, jj));

        }
    int nb = borderPointsS.size ();
    if (nb == 0)
        return QSize ();

    QSize res (0, 0);
    QPoint np, ep, wp, sp;
    np = borderPointsS[0];
    ep = np;
    wp = np;
    sp = np;
    for (int i=0; i<nb; i++)
    {
        QPoint wPoint = borderPointsS[i];
        if (wPoint.y() < np.y())
            np = wPoint;
        if (wPoint.y() > sp.y())
            sp = wPoint;
        if (wPoint.x() < wp.x())
            wp = wPoint;
        if (wPoint.x() > ep.x())
            ep = wPoint;
    }
    double deltax = sp.x()-np.x();
    double deltay = sp.y()-np.y();
    int lf = (int)sqrt (deltax*deltax+deltay*deltay);
    deltax = wp.x()-ep.x();
    deltay = wp.y()-ep.y();
    int wf = (int)sqrt (deltax*deltax+deltay*deltay);
    res = QSize (lf, wf);
    return res;
}

SearchResultsForm * SearchRadioImageCalc :: GUIResultsView (QWidget * parent, Qt::WindowFlags flags)
{
    SearchResultsForm *sresForm = new SearchResultsForm (searchImage, azimuth, elevation_angle, parent, flags);
    qDebug () << __PRETTY_FUNCTION__ << azimuth << elevation_angle;
    connect (sresForm, SIGNAL (calcGoodnessOfFit (QAbstractItemModel *, const QImage&, double, double)), this, SLOT (calcChi2(QAbstractItemModel *, const QImage&, double, double)) );

    return sresForm;
}

QByteArray SearchRadioImageCalc :: searchImageB (const QImage& sIm, double az, double elev) const
{
    if (sIm.isNull())
        return QByteArray ();
    QByteArray bImage;
    QBuffer buffIm (&bImage);
    buffIm.open (QIODevice::WriteOnly);
    QTextStream bIm (&buffIm);
    int nSW = sIm.width ();
    int nSH = sIm.height ();
    bIm << az << " "
        << (elev < 0 ? 90 : elev) << " "
        << nSW << " "
        << nSH << " "
        << 1 << " ";
    int qColGrayMax = qGray (QColor (0, 0, 0).rgb());
    for (int ii=0; ii<nSW; ii++)
        for (int jj=0; jj<nSH; jj++)
        {
            QRgb pCol = sIm.pixel (ii, jj);
            int pCCol = qGray (pCol);
            if (pCCol > qColGrayMax)
                qColGrayMax = pCCol;
        }
    for (int ii=0; ii<nSW; ii++)
        for (int jj=0; jj<nSH; jj++)
        {
            QRgb pCol = sIm.pixel (ii, jj);
            int pColGray = qGray (pCol);
            if (pColGray >= qColGrayMax/2)
                bIm << QString::number (1);
            else
                bIm << QString::number (0);
        }

    buffIm.close ();
    return bImage;
}

QByteArray SearchRadioImageCalc :: getImageStr (const QByteArray& b) const
{
    QTextStream bIm (b);
    unsigned az_dd;
    bIm >> az_dd;
    if (az_dd > 360)
        return QByteArray();
    unsigned elev_dd;
    bIm >> elev_dd;
    unsigned nW, nH, nd;
    bIm >> nW;
    bIm >> nH;
    bIm >> nd;
    QImage pIm (nW, nH, QImage::Format_ARGB32);
    QString imStr;// = bIm.read(nW*nH);
    bIm >> imStr;
    return imStr.toAscii();
}

void SearchRadioImageCalc :: calcChi2 (QAbstractItemModel * sModel, const QImage& sIm, double az, double elev)
{
//    Q_UNUSED (sModel);
//    Q_UNUSED (sIm);
    qDebug () << __PRETTY_FUNCTION__ << sModel << sIm.isNull() << az << elev;
    if (!sModel || sModel->rowCount() == 0 || sModel->columnCount() == 0 || sIm.isNull())
        return;

    gsl_matrix *XMatr, *covMatr;
    gsl_vector * c;
    gsl_vector * yVec;
    Q_UNUSED (XMatr);
    Q_UNUSED (covMatr);
    Q_UNUSED (c);
    Q_UNUSED (yVec);
    int n = sModel->rowCount();
    int m = sModel->columnCount ();
    for (int i=0; i<n; i++)
    {
        QModelIndex wIndex = sModel->index (i, 0);
        QByteArray binaryData = sModel->data (wIndex, Qt::ToolTipRole).toByteArray ();
        if (binaryData.contains("\\x"))
            binaryData = QByteArray::fromHex(binaryData);
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
 /*       QImage pIm (nW, nH, QImage::Format_ARGB32);
        QString imStr;// = bIm.read(nW*nH);
        bIm >> imStr;
*/
        QByteArray imArr = getImageStr (binaryData);//imStr.toAscii();
        uint np (0);
/*        int ncount (0);
        for (uint ii=0; ii<nW; ii++)
        {
            for (uint jj=0; jj<nH; jj++)
            {
                uint c = (uint)imStr.at(ncount++).digitValue();
                c *= 255;
                pIm.setPixel(ii, jj, qRgb(c,c,c));
            }
        }
*/
        QImage scImage = sIm.scaled (nW, nH);
        QByteArray bscIm = searchImageB (scImage, azimuth, elevation_angle);
        QByteArray bscImStr = getImageStr (bscIm);
        for (int ii=0; ii<bscImStr.size(); ii++)
            if (bscImStr[ii] == imArr[ii])
                np++;
        QModelIndex wPIndex = sModel->index (i, m-1);
        qDebug () << __PRETTY_FUNCTION__ << np << imArr.size() << wPIndex << QString::compare(QString(bscImStr), QString (imArr), Qt::CaseInsensitive);
        //sModel->setData (wPIndex, QString::number (np*100./bscImStr.size()), Qt::EditRole);

    }
//    qDebug () << __PRETTY_FUNCTION__ << bImage;
/*    XMatr = gsl_matrix_alloc (5, 5 );//n, nPol+1);
    covMatr = gsl_matrix_alloc (5, 5);//(nPol+1, nPol+1);
    yVec = gsl_vector_alloc (5);
    int n = sModel->rowCount();
    QVector<QPoint> borderPointsS;
    int nSW = sIm.width ();
    int nSH = sIm.height ();
    int qColGrayMax = qGray (QColor (0, 0, 0).rgb());
    for (int ii=0; ii<nSW; ii++)
        for (int jj=0; jj<nSH; jj++)
        {
            QRgb pCol = sIm.pixel (ii, jj);
            int pCCol = qGray (pCol);
            if (pCCol > qColGrayMax)
                qColGrayMax = pCCol;
        }
    for (int ii=1; ii<nSW-1; ii++)
        for (int jj=1; jj<nSH-1; jj++)
        {
            QRgb pCol = sIm.pixel (ii, jj);
            QRgb pPDiagC = sIm.pixel (ii-1, jj-1);
            QRgb pPDiagC1 = sIm.pixel (ii-1, jj+1);
            QRgb pPRow = sIm.pixel (ii-1, jj);
            QRgb pPCol = sIm.pixel (ii, jj-1);
            QRgb pNDiagC = sIm.pixel (ii+1, jj+1);
            QRgb pNDiagC1 = sIm.pixel (ii-1, jj+1);
            QRgb pNRow = sIm.pixel (ii+1, jj);
            QRgb pNCol = sIm.pixel (ii, jj+1);
            int pColGray = qGray (pCol);
            int pColPDiagC = qGray (pPDiagC);
            int pColPDiagC1 = qGray (pPDiagC1);
            int pColPRow = qGray (pPRow);
            int pColPCol = qGray (pPCol);
            int pColNDiagC = qGray (pNDiagC);
            int pColNDiagC1 = qGray (pNDiagC1);
            int pColNRow = qGray (pNRow);
            int pColNCol = qGray (pNCol);
            if (pColGray >= qColGrayMax/2 &&
                    (pColPDiagC < qColGrayMax/2 || pColPDiagC1 < qColGrayMax/2 || pColPRow < qColGrayMax/2 || pColPCol < qColGrayMax/2 ||
                     pColNDiagC < qColGrayMax/2 || pColNDiagC1 < qColGrayMax/2 || pColNRow  < qColGrayMax/2 || pColNCol < qColGrayMax/2) 
               )
                borderPointsS.append (QPoint (ii, jj));

        }
    qDebug () << __PRETTY_FUNCTION__ << borderPointsS;
    for (int i=0; i<n; i++)
    {
        QVector<QPoint> borderPoints0;
        QModelIndex wIndex = sModel->index (i, 0);
        QByteArray binaryData = sModel->data (wIndex, Qt::ToolTipRole).toByteArray ();
        if (binaryData.contains("\\x"))
            binaryData = QByteArray::fromHex(binaryData);

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
        QString imStr;
        bIm >> imStr;
//        int ncount (0);
        for (unsigned int ii=1; ii<nW-1; ii++)
            for (unsigned int jj=1; jj<nH-1; jj++)
            {
                uint c = (uint)imStr.at (ii*nW+jj).digitValue();
                uint cPDiag = (uint)imStr.at ((ii-1)*nW+jj-1).digitValue();
                uint cPRow = (uint)imStr.at ((ii-1)*nW+jj).digitValue();
                uint cPCol = (uint)imStr.at (ii*nW+jj-1).digitValue();
                uint cPDiag1 = (uint)imStr.at ((ii-1)*nW+jj+1).digitValue();
                uint cNDiag = (uint)imStr.at ((ii+1)*nW+jj+1).digitValue();
                uint cNDiag1 = (uint)imStr.at ((ii-1)*nW+jj+1).digitValue();
                uint cNRow = (uint)imStr.at ((ii+1)*nW+jj).digitValue();
                uint cNCol = (uint)imStr.at (ii*nW+jj+1).digitValue();
                if (c != 0 && (cPDiag==0 || cPDiag1==0 || cPRow==0 || cPCol==0 || cNDiag==0 || cNDiag1==0 || cNRow==0 || cNCol==0) )
                {
                    //qDebug () << __PRETTY_FUNCTION__ << "Border point" << ii << jj;
                    borderPoints0.append (QPoint (ii, jj));
                }
            }
    }
    gsl_matrix_free (covMatr);
    gsl_matrix_free (XMatr);
*/
    qDebug () << __PRETTY_FUNCTION__;
}

void SearchRadioImageCalc :: searchIm (const QImage& fImage, double az, double elev)
{
    qDebug () << __PRETTY_FUNCTION__ << fImage.isNull() << az << elev;
    if (fImage.isNull())
        return;

    azimuth = az;
    elevation_angle = elev;
    qDebug () << __PRETTY_FUNCTION__;
}
