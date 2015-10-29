#include <QLabel>
#include <QGridLayout>
#include <QPixmap>
#include <QPoint>
#include <QColor>
#include <QMessageBox>
#include <QValidator>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QtDebug>

#include "searchradioimagefragmentform.h"
#include "ui_search_radio_image_fragment_form.h"

SearchRadioImageFragmentForm :: SearchRadioImageFragmentForm (const QImage& sImage, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::search_image_fragment_form),
    sourceImage (sImage),
    filteredImage (QImage()),
    lSImage (new QLabel (this)),
    lFImage (new QLabel (this)),
    isFilt (false),
    az (-1)
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
    UI->gbParams->setVisible (true);

    QValidator * azVal = new QDoubleValidator (0, 183, 6, this);
    UI->lEAzimuth->setValidator (azVal);

    QValidator * lVal = new QIntValidator (0, 1000, this);
    UI->lELength->setValidator (lVal);
    QValidator * wVal = new QIntValidator (0, 1000, this);
    UI->lEWidth->setValidator (wVal);

    connect (UI->tbFilt, SIGNAL (clicked()), this, SLOT (brFilt()) );
    //connect (UI->pbCalculate, SIGNAL (clicked()), this, SLOT (pbCalc()) );

    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    this->pbCalc ();
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
    this->pbCalc ();
}

void SearchRadioImageFragmentForm :: pbCalc (void)
{
    if (!isFilt)
        brFilt ();
    UI->gbParams->setVisible (true);
//    int w = sourceImage.width();
//    int h = sourceImage.height();
    double brRel = UI->spBrightess->value()/0.1e3;
    double cVal = (1.0-brRel)*qGray (255, 255, 255);
    emit calcParams (filteredImage, cVal);
//    UI->lELength->setText (QString::number (lf));
//    UI->lEWidth->setText (QString::number (wf));
}

void SearchRadioImageFragmentForm :: setResults (int pix_length, int pix_width, double azimuth)
{
    UI->lELength->setText (QString::number (pix_length));
    UI->lEWidth->setText (QString::number (pix_width));
    UI->lEAzimuth->setText (QString::number (azimuth));
    az = azimuth;
}

double SearchRadioImageFragmentForm :: getAzimuth (void) const
{
    return UI->lEAzimuth->text().toDouble();
}
