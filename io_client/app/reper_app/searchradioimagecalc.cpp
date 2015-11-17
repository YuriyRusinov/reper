#include <QAbstractItemModel>
#include <QVector>
#include <QPoint>
#include <QtDebug>

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

SearchResultsForm * SearchRadioImageCalc :: GUIResultsView (QWidget * parent, Qt::WindowFlags flags)
{
    SearchResultsForm *sresForm = new SearchResultsForm (searchImage, parent, flags);
    connect (sresForm, SIGNAL (calcGoodnessOfFit (QAbstractItemModel *, const QImage&)), this, SLOT (calcChi2(QAbstractItemModel *, const QImage&)) );

    return sresForm;
}

void SearchRadioImageCalc :: calcChi2 (QAbstractItemModel * sModel, const QImage& sIm)
{
//    Q_UNUSED (sModel);
//    Q_UNUSED (sIm);
    qDebug () << __PRETTY_FUNCTION__ << sModel << sIm.isNull();
    if (!sModel || sModel->rowCount() == 0 || sModel->columnCount() == 0 || sIm.isNull())
        return;

    int n = sModel->rowCount();
    QVector<QPoint> borderPointsS;
    int nSW = sIm.width ();
    int nSH = sIm.height ();
    for (int ii=1; ii<nSW-1; ii++)
        for (int jj=1; jj<nSH-1; jj++)
        {
        }
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
                    qDebug () << __PRETTY_FUNCTION__ << "Border point" << ii << jj;
                    borderPoints0.append (QPoint (ii, jj));
                }
            }
    }
}
