#include <QAbstractItemModel>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QBuffer>
#include <QMessageBox>
#include <QtDebug>

#include <gsl/gsl_fit.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multifit.h>

#include <math.h>
#include <iostream>
#include <fstream>

#include <KKSObject.h>
#include <KKSCategory.h>
#include <KKSLoader.h>
#include <KKSObjEditorFactory.h>
#include <KKSObjEditor.h>
#include <KKSRecWidget.h>
#include <KKSEIOData.h>
#include <defines.h>

#include "searchradioimagefragmentform.h"
#include "searchresultsform.h"
#include "searchradioimagecalc.h"
#include "imagewidget.h"
#include "seaobjectparameters.h"
#include "qimage_to_cvmat.h"

//#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

using cv::findContours;
using cv::Vec4i;

using std::cout;
using std::ofstream;

SearchRadioImageCalc :: SearchRadioImageCalc (KKSLoader * _l, KKSObjEditorFactory * _oef, QObject * parent)
    : QObject (parent),
    loader (_l),
    oef (_oef)
{
}

SearchRadioImageCalc :: ~SearchRadioImageCalc (void)
{
}

SearchRadioImageFragmentForm * SearchRadioImageCalc :: GUIImageView (const QImage& im, QWidget * parent, Qt::WindowFlags flags)
{
    searchImage = im;
    double brRel = 0.75;
//    double cVal = (1.0-brRel)*qGray (255, 255, 255);
    int w = im.width();
    int h = im.height();
    QImage filteredImage = QImage (w, h, im.format());
    for (int i=0; i<w; i++)
        for (int j=0; j<h; j++)
        {
            QPoint pos = QPoint (i, j);
            int sCol = qGray (im.pixel (pos));
            QRgb fCol = sCol < (1.0-brRel)*qGray (255, 255, 255) ? QColor(0, 0, 0).rgb() : QColor (255, 255, 255).rgb();
            filteredImage.setPixel (pos, fCol);
        }
    QVector<SeaObjectParameters> sp = this->imageAnalyse (filteredImage);
    qDebug () << __PRETTY_FUNCTION__ << sp.count ();
    SearchRadioImageFragmentForm * sForm = new SearchRadioImageFragmentForm (sp, im, parent, flags);

    connect (sForm, SIGNAL (calcParams (const QImage&, double)), this, SLOT (calculateParameters (const QImage&, double)) );
    connect (sForm, SIGNAL (searchByIm (const QImage&, double, double)), this, SLOT (searchIm (const QImage&, double, double)) );
    connect (sForm, SIGNAL (searchByParams (const QImage&, SeaObjectParameters)), this, SLOT (searchParams (const QImage&, SeaObjectParameters)) );
    connect (this, SIGNAL (setVals (int, int, double)), sForm, SLOT (setResults(int, int, double)) );
    sForm->pbCalc ();
    sForm->selObject (0);

    return sForm;
}

ImageWidget * SearchRadioImageCalc :: GUISearchInit (QWidget * parent, Qt::WindowFlags flags)
{
    ImageWidget * imW = new ImageWidget (parent, flags);
    connect (imW, SIGNAL (searchByIm (const QImage&)), this, SLOT (searchInitIm (const QImage&)) );
    return imW;
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
    Q_UNUSED (lf);
    Q_UNUSED (wf);
    QVector<QPoint> r_border;
    im.convertToFormat (QImage::Format_ARGB32);
    cv::Mat rImage = QImageToCvMat (im);
//    ofstream rImStr ("rImageMatr.txt");
//    rImStr << rImage << std::endl;
//              cv::Mat(qimage_to_mat_cpy (im, CV_8UC1));
    //cv::Mat::zeros(im.width(),im.height(), CV_8UC1);
//    qDebug () << __PRETTY_FUNCTION__ << cVal;
    im.convertToFormat (QImage::Format_ARGB32);
    im.save (QString ("object_t.bmp"));
    rImage = cv::imread ("object_t.bmp", CV_LOAD_IMAGE_GRAYSCALE);

    std::vector<std::vector<cv::Point> > contours;
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
/*                rImage.at<int> (cPos.x(), cPos.y()) = 255;
                for (int iii=cPos.x(); iii<=cPos.x()+2; iii++)
                {
                    int jjj=cPos.y(); //jjj<=cPos.y(); jjj++)
                    qDebug () << __PRETTY_FUNCTION__ << QString ("Fill matrix") << iii << jjj;
                    rImage.at<int> (iii, jjj) = 255;
                }
                rImage.at<int> (prPosX.x(), prPosX.y()) = 255;
                rImage.at<int> (prPosY.x(), prPosY.y()) = 255;
                rImage.at<int> (nPosX.x(), nPosX.y()) = 255;
                rImage.at<int> (nPosY.x(), nPosY.y()) = 255;
*/
                r_border.append (cPos);
/*                if (np.x() * np.x() + np.y() * np.y() > 0)
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
*/
            }
        }
    qDebug () << __PRETTY_FUNCTION__ << r_border;//np << ep << sp << wp;
    if (r_border.isEmpty())
    {
        QMessageBox::warning (0, tr("Search"), tr("No objects in selection"), QMessageBox::Ok);
        return;
    }
//    cout << __PRETTY_FUNCTION__ << "Opencv matrix is\n" << rImage;
//    blur( rImage, rImage, cv::Size(3,3) );
    cv::Mat contourOutput = rImage.clone();
    cv::vector<Vec4i> hierarchy;
/*    Vec4i a = {1, -1, -1, -1};
    hierarchy.push_back (a);
    Vec4i b = {2,  0, -1, -1};
    hierarchy.push_back (b);
    hierarchy.push_back (a);
*/
//    hierarchy << array([[[ 1, -1, -1, -1],
//                         [ 2,  0, -1, -1],
//                         [-1,  1, -1, -1]]]);
    findContours( contourOutput, contours, hierarchy, 
            CV_RETR_TREE, CV_CHAIN_APPROX_NONE );
    int nc = contours.size ();
    qDebug () << __PRETTY_FUNCTION__ << nc;//qCont;//contours.size ();
    QVector<QVector<QPoint> > qCont;
    for (int i=0; i<nc; i++)
    {
        double area = contourArea(contours[i], true);
        int ncp = contours[i].size ();
        QVector<QPoint> c;
        for (int ii=0; ii<ncp; ii++)
        {
            QPoint p (contours[i][ii].x, contours[i][ii].y);
            c.append (p);
            //qDebug () << __PRETTY_FUNCTION__ << p;
        }
        qCont.append (c);
        qDebug () << __PRETTY_FUNCTION__ << area << c;
    }
/*    double deltax = sp.x()-np.x();
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
*/
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
    QMap<QString, double> numberTypesP;
    for (int i=0; i<n; i++)
    {
        QString sType = sModel->data (sModel->index (i, m-1), Qt::DisplayRole).toString();//.toInt();
        if (!numberTypesP.contains (sType))
            numberTypesP.insert (sType, 0.0);
    }
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
        QString sType = sModel->data (sModel->index (i, m-1), Qt::DisplayRole).toString();
        int nType = sModel->data (sModel->index (i, m-1), Qt::UserRole+1).toInt();
        QByteArray bscIm = searchImageB (scImage, azimuth, elevation_angle);
        QByteArray bscImStr = getImageStr (bscIm);
        for (int ii=0; ii<bscImStr.size(); ii++)
            if (bscImStr[ii] == imArr[ii])
                np++;
        //qDebug () << __PRETTY_FUNCTION__ << np *100./imArr.size() << wPIndex << QString::compare(QString(bscImStr), QString (imArr), Qt::CaseInsensitive);
        double chi2 = np*1.00/bscImStr.size();///nType;
        /*
         * df\area	.995	.990	.975	.950	.900	.750	.500	.250	.100	.050	.025	.010	.005
1	                0.00004	0.00016	0.00098	0.00393	0.01579	0.10153	0.45494	1.32330	2.70554	3.84146	5.02389	6.63490	7.87944
2	                0.01003	0.02010	0.05064	0.10259	0.21072	0.57536	1.38629	2.77259	4.60517	5.99146	7.37776	9.21034	10.59663
*/
        double prob;
        qDebug () << __PRETTY_FUNCTION__ << chi2;// << sType << nType;
        if (chi2 <= 0.07)
            prob = 0.99;
        else if (chi2 <= 0.1 )//&& nType > 1)
            prob = 0.975;
        else if (chi2 <= 0.575)
            prob = 0.75;
        else
            prob = 0.6;
        numberTypesP[sType] = prob;
    }
    double sp = 0.0;
    for (QMap<QString, double>::const_iterator p = numberTypesP.constBegin();
             p != numberTypesP.constEnd();
             p++)
    {
        qDebug () << __PRETTY_FUNCTION__ << p.key();
        sp += p.value ();
    }
    //qDebug () << __PRETTY_FUNCTION__ << sp;
    sp = qMax (sp, (double)numberTypesP.count());
    for (int i=0; i<n; i++)
    {
        QString sType = sModel->data (sModel->index (i, m-1), Qt::DisplayRole).toString();//.toInt();
        double prob = numberTypesP.value (sType);
        QModelIndex wPIndex = sModel->index (i, m-2);
        sModel->setData (wPIndex, QString::number (prob/sp), Qt::EditRole);
        //qDebug () << __PRETTY_FUNCTION__ << np *100./imArr.size() << wPIndex << QString::compare(QString(bscImStr), QString (imArr), Qt::CaseInsensitive) << isSet;

    }
//    qDebug () << __PRETTY_FUNCTION__ << bImage;
/*    XMatr = gsl_matrix_alloc (5, 5 );//n, nPol+1);
    covMatr = gsl_matrix_alloc (5, 5);//(nPol+1, nPol+1);
    yVec = gsl_vector_alloc (5);
//    int n = sModel->rowCount();
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

void SearchRadioImageCalc :: searchParams (const QImage& sIm, SeaObjectParameters sp)
{
//    qDebug () << __PRETTY_FUNCTION__ << sp.size ();
//    int nsp = sp.size ();
    QString tName = QString ("rli_image_raws");
    KKSObject * io = loader->loadIO (tName, true);
    if (!io)
    {
        QMessageBox::warning (0, tr("Select reference"),
                                 tr ("Not available suitable reference"),
                                 QMessageBox::Ok);
        return;
    }
    KKSList<const KKSFilterGroup *> filterGroups;
    const KKSCategory * c = io->category();
    const KKSCategory * ct = c->tableCategory();
    QByteArray bytes;
    QBuffer buffer(&bytes);

    buffer.open(QIODevice::WriteOnly);
    sIm.save (&buffer, "XPM");
    buffer.close ();
    KKSCategoryAttr * aIm = 0;//loader->loadAttribute ("image_jpg"
    KKSCategoryAttr * aAz = 0;
    KKSCategoryAttr * aElev = 0;
    KKSCategoryAttr * aShipType = 0;
    KKSCategoryAttr * aResolv = 0;
    for (KKSMap<int, KKSCategoryAttr *>::const_iterator p = ct->attributes().constBegin();
            p != ct->attributes().constEnd() ;//&& aIm == 0 && aAz==0;
            ++p)
    {
        if (QString::compare (p.value()->code(), QString("image_jpg"), Qt::CaseInsensitive) == 0)
            aIm = p.value ();
        if (QString::compare (p.value()->code(), QString("azimuth"), Qt::CaseInsensitive) == 0)
            aAz = p.value ();
        if (QString::compare (p.value()->code(), QString("elevation_angle"), Qt::CaseInsensitive) == 0)
            aElev = p.value ();
        if (QString::compare (p.value()->code(), QString ("id_type_ship"), Qt::CaseInsensitive) == 0)
            aShipType = p.value ();
        if (QString::compare (p.value()->code(), QString ("resolution"), Qt::CaseInsensitive) == 0)
             aResolv = p.value ();
    }
    if (!aAz)
    {
        return;
    }

    KKSFilter * filter = ct->createFilter (aIm->id(), bytes, KKSFilter::foEq);
    filter->release ();
    Q_UNUSED (filter);
    KKSFilterGroup * allGroups = new KKSFilterGroup(false);

    KKSFilterGroup * group = new KKSFilterGroup(false);
    SeaObjectParameters sop = sp;
    double az = sop.azimuth;
    KKSFilter * fAzMin = ct->createFilter (aAz->id(), QString::number (az-10), KKSFilter::foGrEq);
    KKSFilter * fAzMax = ct->createFilter (aAz->id(), QString::number (az+10), KKSFilter::foLessEq);
    KKSFilterGroup * azGroup = new KKSFilterGroup (true);
    KKSFilterGroup * azGroupR = new KKSFilterGroup (true);
    azGroup->addFilter (fAzMin);
    fAzMin->release ();
    azGroup->addFilter (fAzMax);
    fAzMax->release ();
    double l = sop.length;
    if (l >= 0)
    {
        QString tableName = QString("type_ship");
        KKSFilterGroup * fTypeGroup = new KKSFilterGroup (false);
        KKSObject * refTypeObj = loader->loadIO (tableName);
        KKSCategory * cType = refTypeObj->category ();
        KKSCategory * cTypeRef = cType->tableCategory ();
        KKSAttribute * lAMin = loader->loadAttribute (1001);
        KKSAttribute * lAMax = loader->loadAttribute (1002);
        KKSFilter * fLenMin = cTypeRef->createFilter (lAMin->id(), QString::number (l), KKSFilter::foLessEq);
        KKSFilter * fLenMax = cTypeRef->createFilter (lAMax->id(), QString::number (l), KKSFilter::foGrEq);
        KKSFilterGroup * fSelType = new KKSFilterGroup (true);
        fSelType->addFilter (fLenMin);
        fLenMin->release ();
        fSelType->addFilter (fLenMax);
        fLenMax->release ();
        KKSList<const KKSFilterGroup *> fGr;
        fGr.append (fSelType);
        lAMin->release ();
        lAMax->release ();
        KKSMap<qint64, KKSEIOData *> fTRecs = loader->loadEIOList (refTypeObj, fGr);
        //QString val = QString ("select * from %2 where %1 >= length_min and %1 <= length_max").arg (l).arg (tableName);
        for (KKSMap<qint64, KKSEIOData *>::const_iterator p=fTRecs.constBegin();
                p != fTRecs.constEnd();
                p++)
        {
            KKSFilter * fType = ct->createFilter (aShipType->id(), QString::number (p.key()), KKSFilter::foEq);
            qDebug () << __PRETTY_FUNCTION__ << p.key();
            fTypeGroup->addFilter (fType);
            fType->release ();
        }
        refTypeObj->release ();
        azGroup->addGroup (fTypeGroup);
        azGroupR->addGroup (fTypeGroup);

        fTypeGroup->release ();
    }
/*    if (l >= 200)
    {
        KKSFilter * fType = ct->createFilter (aShipType->id(), QString::number (1), KKSFilter::foEq);
        azGroup->addFilter (fType);
        azGroupR->addFilter (fType);
        fType->release ();
    }
    else if (l >= 100 && l<200)
    {
        KKSFilter * fType = ct->createFilter (aShipType->id(), QString::number (20), KKSFilter::foEq);
        azGroup->addFilter (fType);
        azGroupR->addFilter (fType);
        fType->release ();
    }
*/
    double resolv = sop.resolution;
    if (resolv >= 0.0)
    {
        KKSFilter * fResol = ct->createFilter (aResolv->id(), QString::number (resolv), KKSFilter::foEq);
        azGroup->addFilter (fResol);
        azGroupR->addFilter (fResol);
        fResol->release ();
    }
    KKSFilter * fAzMinPi = ct->createFilter (aAz->id(), QString::number (az-10+180), KKSFilter::foGrEq);
    KKSFilter * fAzMaxPi = ct->createFilter (aAz->id(), QString::number (az+10+180), KKSFilter::foLessEq);
    azGroupR->addFilter (fAzMinPi);
    fAzMinPi->release ();
    azGroupR->addFilter (fAzMaxPi);
    fAzMaxPi->release ();
    group->addGroup (azGroup);
    double elev = sop.elevation_angle;
    KKSFilter * fElev0 = 0;
    KKSFilter * fElev = 0;
    if (elev >= 0)
    {
        fElev0 = ct->createFilter (aElev->id(), QString::number (elev-10), KKSFilter::foGrEq);
        fElev = ct->createFilter (aElev->id(), QString::number (elev+10), KKSFilter::foLessEq);
        azGroup->addFilter (fElev0);
        azGroup->addFilter (fElev);
        azGroupR->addFilter (fElev0);
        azGroupR->addFilter (fElev);
        fElev0->release ();
        fElev->release ();
    }
//        group->setFilters(filters);
    group->addGroup (azGroup);
    azGroup->release ();
    group->addGroup (azGroupR);
    azGroupR->release ();
    allGroups->addGroup (group);
    group->release ();

    filterGroups.append(allGroups);
    allGroups->release ();
    KKSObjEditor *objEditor = oef->createObjEditor(IO_IO_ID, 
                                                   io->id(), 
                                                   filterGroups, 
                                                   "",
                                                   c,
                                                   false,
                                                   QString(),
                                                   false,
                                                   Qt::NonModal,
                                                   0);
    io->release ();
    SearchResultsForm *sresForm = this->GUIResultsView ();//new SearchResultsForm (sIm);
    KKSRecWidget * rw = objEditor->getRecordsWidget ();
    QAbstractItemModel * sMod = rw->getModel ();
    sresForm->setResultsModel (sMod);

    emit viewWidget (sresForm);
}

void SearchRadioImageCalc :: searchInitIm (const QImage& sIm0)
{
    if (sIm0.isNull())
        return;
    qDebug () << __PRETTY_FUNCTION__;
    SearchRadioImageFragmentForm * srForm = this->GUIImageView (sIm0);//new SearchRadioImageFragmentForm (sIm0);
    //srForm->setImage (sIm0);
    if (srForm->exec() != QDialog::Accepted)
        return;
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

/*
cv::Mat SearchRadioImageCalc :: qimage_to_mat_cpy(const QImage &img, int format)
{
    uchar* b = const_cast<uchar*> (img.bits ());
    int c = img.bytesPerLine();
    cv::Mat mat = cv::Mat(img.height(), img.width(), format, b, c).clone();
    return mat;
}
*/
/*
cv::Mat SearchRadioImageCalc :: QImageToCvMat( const QImage &inImage, bool inCloneImageData )
{
    switch ( inImage.format() )
    {
        // 8-bit, 4 channel
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied:
        {
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC4,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
        }

        // 8-bit, 3 channel
        case QImage::Format_RGB32:
        case QImage::Format_RGB888:
        {
            if ( !inCloneImageData )
            {
               qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
            }

            QImage   swapped;

            if ( inImage.format() == QImage::Format_RGB32 )
               swapped = inImage.convertToFormat( QImage::Format_RGB888 );

            swapped = inImage.rgbSwapped();

            return cv::Mat( swapped.height(), swapped.width(),
                            CV_8UC3,
                            const_cast<uchar*>(swapped.bits()),
                            static_cast<size_t>(swapped.bytesPerLine())
                            ).clone();
        }

        // 8-bit, 1 channel
        case QImage::Format_Indexed8:
        {
            qDebug () << __PRETTY_FUNCTION__ << "Indexed";
            cv::Mat  mat( inImage.height(), inImage.width(),
                          CV_8UC1,
                          const_cast<uchar*>(inImage.bits()),
                          static_cast<size_t>(inImage.bytesPerLine())
                          );

            return (inCloneImageData ? mat.clone() : mat);
        }

        default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }

    return cv::Mat();
}
*/

QVector<SeaObjectParameters> SearchRadioImageCalc :: imageAnalyse (const QImage& inImage)
{
    QImage im (inImage);
    cv::Mat rImage;// = QImageToCvMat (im);
//    rImStr << rImage << std::endl;
//              cv::Mat(qimage_to_mat_cpy (im, CV_8UC1));
    //cv::Mat::zeros(im.width(),im.height(), CV_8UC1);
//    qDebug () << __PRETTY_FUNCTION__ << cVal;
    im.convertToFormat (QImage::Format_ARGB32);
    im.save (QString ("object_t1.bmp"));
    rImage = cv::imread ("object_t1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    ofstream rImStr ("rImageZeroMatr.dat");
    im.convertToFormat (QImage::Format_RGB32);
    cv::Mat rIm1 = QImageToCvMat (im);
    cvtColor (rIm1, rIm1, CV_RGB2GRAY);
    blur (rIm1, rIm1, Size (im.height(), im.width()));
    rImStr << (rImage-rIm1) << std::endl;

    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = rImage.clone();
    cv::vector<Vec4i> hierarchy;
//    Vec4i a = {1, -1, -1, -1};
//    hierarchy.push_back (a);
//    Vec4i b = {2,  0, -1, -1};
//    hierarchy.push_back (b);
//    hierarchy.push_back (a);

//    hierarchy << array([[[ 1, -1, -1, -1],
//                         [ 2,  0, -1, -1],
//                         [-1,  1, -1, -1]]]);
    findContours( contourOutput, contours, hierarchy, 
            CV_RETR_TREE, CV_CHAIN_APPROX_NONE );
    int nc = contours.size ();
    QVector<SeaObjectParameters> objPars;
    bool isProp = false;
    for (int i=0; i<nc; i++)
    {
        int n = contours[i].size ();
        QVector<QPoint> c;
        for (int ii=0; ii<n; ii++)
        {
            QPoint p (contours[i][ii].x, contours[i][ii].y);
            c.append (p);
            //qDebug () << __PRETTY_FUNCTION__ << p;
        }
        QRect r = QPolygon (c).boundingRect ();
        if (r.width() < 6 && r.height() < 6)
        {
            //
            // Image too small
            //
            isProp = true;
            continue;
        }
        double l = qMax (r.width(), r.height());
        double w = qMin (r.width(), r.height());
        double d = -1.0;
        double az = atan2 (w, l)*180/pi;
        double elev = -1.0;
        double resolv = -1.0;
        QString sProp = QString ();
        SeaObjectParameters sp (r, l, w, d, az, elev, resolv, sProp);
        objPars.append (sp);
    }
    int ncr = objPars.size ();
    if (isProp)
        for (int i=0; i<ncr; i++)
            objPars[i].secProp = tr ("Control on the right");

    return objPars;
}
