#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include <QPoint>
#include <QColor>
#include <QMessageBox>
#include <QtDebug>

#include <math.h>

#include "searchradioimagefragmentform.h"
#include "ui_search_radio_image_fragment_form.h"

SearchRadioImageFragmentForm :: SearchRadioImageFragmentForm (const QImage& sImage, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::search_image_fragment_form),
    sourceImage (sImage),
    filteredImage (QImage()),
    lSImage (new QLabel (this)),
    lFImage (new QLabel (this)),
    isFilt (false)
{
    UI->setupUi (this);

    QGridLayout * gSLay = new QGridLayout (UI->wImageSource);
    gSLay->addWidget (lSImage, 0, 0, 1, 1);
    lSImage->setScaledContents (true);
    this->setSourceImage (sImage);

    QGridLayout * gFLay = new QGridLayout (UI->wImageFiltered);
    lFImage->setScaledContents (true);
    gFLay->addWidget (lFImage, 0, 0, 1, 1);

    UI->tbFilt->setToolTip (tr("Filter by brightness"));
    UI->gbParams->setVisible (false);

    connect (UI->tbFilt, SIGNAL (clicked()), this, SLOT (brFilt()) );
    connect (UI->pbCalculate, SIGNAL (clicked()), this, SLOT (pbCalc()) );

    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
}

SearchRadioImageFragmentForm :: ~SearchRadioImageFragmentForm (void)
{
    delete UI;
}

const QImage& SearchRadioImageFragmentForm :: getSourceImage (void) const
{
    return sourceImage;
}

const QImage& SearchRadioImageFragmentForm :: getFilteredImage (void) const
{
    return filteredImage;
}

void SearchRadioImageFragmentForm :: setSourceImage (const QImage& im)
{
    sourceImage = im;
    lSImage->setPixmap (QPixmap::fromImage (im));
}

void SearchRadioImageFragmentForm :: setFilteredImage (const QImage& im)
{
    filteredImage = im;
    lFImage->setPixmap (QPixmap::fromImage (im));
}

void SearchRadioImageFragmentForm :: brFilt (void)
{
    double brRel = UI->spBrightess->value()/0.1e3;
    //qDebug () << __PRETTY_FUNCTION__ << brRel*qGray (255, 255, 255);//sourceImage.format() << QImage::Format_Mono;
    int w = sourceImage.width();
    int h = sourceImage.height();
    filteredImage = QImage (w, h, sourceImage.format());
    for (int i=0; i<w; i++)
        for (int j=0; j<h; j++)
        {
            QPoint pos = QPoint (i, j);
            int sCol = qGray (sourceImage.pixel (pos));
            QRgb fCol = sCol < (1.0-brRel)*qGray (255, 255, 255) ? QColor(0, 0, 0).rgb() : QColor (255, 255, 255).rgb();
            filteredImage.setPixel (pos, fCol);
        }
    setFilteredImage (filteredImage);
    isFilt = true;
}

void SearchRadioImageFragmentForm :: pbCalc (void)
{
    if (!isFilt)
    {
        QMessageBox::warning (this, tr ("Calculate image parameters"), tr ("Raw image has to be filtered"), QMessageBox::Ok);
        return;
    }
    UI->gbParams->setVisible (true);
    int w = sourceImage.width();
    int h = sourceImage.height();
    double brRel = UI->spBrightess->value()/0.1e3;
    double cVal = (1.0-brRel)*qGray (255, 255, 255);
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
            int sCol = qGray (filteredImage.pixel (cPos));
            int pColX = qGray (filteredImage.pixel (prPosX));
            int pColY = qGray (filteredImage.pixel (prPosY));
            int nColX = qGray (filteredImage.pixel (nPosX));
            int nColY = qGray (filteredImage.pixel (nPosY));
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
    deltax = wp.x()-ep.x();
    deltay = wp.y()-ep.y();
    wf = (int)sqrt (deltax*deltax+deltay*deltay);
    UI->lELength->setText (QString::number (lf));
    UI->lEWidth->setText (QString::number (wf));
}
