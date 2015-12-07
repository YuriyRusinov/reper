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
    UI->gbImageParams->setVisible (true);

    QValidator * azVal = new QDoubleValidator (0, 183, 8, this);
    UI->lEAzimuth->setValidator (azVal);

    QValidator * elVal = new QDoubleValidator (-1, 85, 8, this);
    UI->lEElevation->setValidator (elVal);
    UI->lEElevation->setEnabled (false);

    QValidator * lVal = new QIntValidator (0, 1000, this);
    UI->lELength->setValidator (lVal);
    QValidator * wVal = new QIntValidator (0, 1000, this);
    UI->lEWidth->setValidator (wVal);
    QValidator * dVal = new QIntValidator (-1, 100, this);
    UI->lEDepth->setValidator (dVal);
    UI->lEDepth->setEnabled (false);

    connect (UI->tbFilt, SIGNAL (clicked()), this, SLOT (brFilt()) );
    //connect (UI->pbCalculate, SIGNAL (clicked()), this, SLOT (pbCalc()) );
    connect (UI->cbDepth, SIGNAL (stateChanged(int)), this, SLOT (depthStateChanged (int)) );
    connect (UI->cbElevation, SIGNAL (stateChanged(int)), this, SLOT (elevStateChanged (int)) );

    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (searchBegin()) );
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
    UI->gbImageParams->setVisible (true);
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

void SearchRadioImageFragmentForm :: searchBegin (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    double im_az = getAzimuth();
    double im_elev = getElevation ();
    emit searchByIm (filteredImage, im_az, im_elev);
    accept ();
}

double SearchRadioImageFragmentForm :: getElevation (void) const
{
    if (UI->cbElevation->checkState() == Qt::Checked)
        return UI->lEElevation->text().toDouble ();
    else
        return -1.0;
}

int SearchRadioImageFragmentForm :: getImageWidth (void) const
{
    return UI->lEWidth->text().toInt();
}

int SearchRadioImageFragmentForm :: getImageHeight (void) const
{
    return UI->lELength->text().toInt();
}

int SearchRadioImageFragmentForm :: getImageDepth (void) const
{
    if (UI->cbDepth->checkState() == Qt::Checked)
        return UI->lEDepth->text().toInt();
    else
        return -1;
}

void SearchRadioImageFragmentForm :: elevStateChanged (int state)
{
    UI->lEElevation->setEnabled ((state==Qt::Checked));
}

void SearchRadioImageFragmentForm :: depthStateChanged (int state)
{
    UI->lEDepth->setEnabled ((state==Qt::Checked));
}
