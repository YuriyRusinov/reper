#include <QtDebug>

#include <math.h>

#include "searchradioimagefragmentform.h"
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
