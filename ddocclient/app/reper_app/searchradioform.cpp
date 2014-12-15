#include <QValidator>
#include <QDoubleValidator>
#include <QFileDialog>
#include <QDir>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QMessageBox>
#include <QColor>

#include <math.h>
#include "searchradioform.h"
#include "ui_search_radio_form.h"

SearchRadioForm :: SearchRadioForm (QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::Search_Radio_Form)
{
    UI->setupUi (this);

    init ();

    connect (UI->tbLoadImage, SIGNAL (clicked()), this, SLOT (loadImage()) );
    connect (UI->tbClearImage, SIGNAL (clicked()), this, SLOT (clearImage()) );
    connect (UI->tbCalc,SIGNAL (clicked()), this, SLOT (calcPars()) );
    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

SearchRadioForm :: ~SearchRadioForm (void)
{
}

void SearchRadioForm :: loadImage (void)
{
    QString imageFileName = QFileDialog::getOpenFileName (this,
                                                          tr("Open Image File"),
                                                          QDir::currentPath(),
                                                          tr("Images (*.png *.xpm *.jpg *.PNG *.XPM *.JPG)"));
    if (imageFileName.isEmpty())
        return;

    sIm = QImage (imageFileName);
    if (sIm.isNull())
    {
        QMessageBox::information(this, tr("Error"),
                                     tr("Cannot load %1.").arg(imageFileName));
        return;
    }
    lImageP->setPixmap (QPixmap::fromImage (sIm));
}

void SearchRadioForm :: clearImage (void)
{
    sIm = QImage ();
    lImageP->setPixmap (QPixmap());
}

void SearchRadioForm :: init (void)
{
    QValidator * dAzVal = new QDoubleValidator (0.0, 360.0, 16, this);
    UI->lEAzimuth->setValidator (dAzVal);

    QValidator * dElevVal = new QDoubleValidator (-90.0, 90.0, 16, this);
    UI->lEElevationAngle->setValidator (dElevVal);

    QGridLayout * gImageLay = new QGridLayout (UI->wImage);
    lImageP = new QLabel (this);
    gImageLay->addWidget (lImageP, 0, 0, 1, 1);
}

QImage SearchRadioForm :: getImage (void) const
{
    return sIm;
}

void SearchRadioForm :: calcPars (void)
{
    if (sIm.isNull())
        return;

    int xmin (-1);
    int xmax (-1);
    int ymin (-1);
    int ymax (-1);
    int w = sIm.width ();
    int h = sIm.height ();
    for (int i=0; i<w; i++)
        for (int j=0; j<h; j++)
        {
            QRgb col = sIm.pixel (i, j);
            if (QColor (col) != QColor (0, 0, 0))
            {
                if (xmin < 0)
                    xmin = i;
                else
                    xmin = qMin (xmin, i);

                if (xmax < 0)
                    xmax = i;
                else
                    xmax = qMax (xmax, i);

                if (ymin < 0)
                    ymin = j;
                else
                    ymin = qMin (ymin, j);

                if (ymax < 0)
                    ymax = j;
                else
                    ymax = qMax (ymax, j);
            }
        }
    double X = xmax - xmin;
    double Y = ymax - ymin;
    const double pi = 3.14159265358979;
    double az_rad = atan2 (-Y, X);
    if (az_rad < 0.0)
        az_rad += pi;
    double az_dg = az_rad*180/pi;
    UI->lEAzimuth->setText (QString::number (az_dg));
    double sl = sqrt (X*X+Y*Y);
    UI->lELength->setText (QString::number (sl));
}
