#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>
#include <QGridLayout>
#include "ImageLabel.h"
#include <QSpacerItem>
#include <QtDebug>

#include "imagewidget.h"

ImageWidget :: ImageWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    rGIm (QImage())
{
    this->init ();

    connect (tbLoadImage, SIGNAL (clicked()), this, SLOT (loadImageFile()) );
    connect (tbSelect, SIGNAL (pressed()), this, SLOT (selectRect()) );
    connect (tbSaveToDb, SIGNAL (clicked()), this, SLOT (saveImageToDb()) );
    connect (tbLoadFromDb, SIGNAL (clicked()), this, SLOT (loadImageFromDb()) );
}

ImageWidget :: ~ImageWidget (void)
{
}

void ImageWidget :: setImage (const QImage& im)
{
    rGIm = im;
    if (!rGIm.isNull())
        lRImage->setPixmap (QPixmap::fromImage (rGIm));
    else
        lRImage->setPixmap (QPixmap());
}

void ImageWidget :: loadImageFile (void)
{
    QString imFileName = QFileDialog::getOpenFileName (this, tr("Open File"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg *.PNG *.XPM *.JPG);;All files (*)"));
    if (imFileName.isEmpty())
        return;
    QImage image(imFileName);
    if (image.isNull()) {
        qWarning() << tr("Cannot load %1.").arg(imFileName);
        QMessageBox::information(this, tr("Error"),
                                 tr("Cannot load %1.").arg(imFileName));
        return;
    }
    setImage (image);
}

void ImageWidget :: selectRect (void)
{
}

void ImageWidget :: saveImageToDb (void)
{
    emit saveImage (rGIm);
}

void ImageWidget :: loadImageFromDb (void)
{
    emit loadImage();
}

void ImageWidget :: init (void)
{
    QGridLayout * grLay = new QGridLayout (this);
    lRImage = new ImageLabel (this);

    grLay->addWidget (lRImage, 0, 0, 5, 1);

    tbLoadImage = new QToolButton (this);
    tbLoadImage->setToolTip (tr("Load golographic image from file"));
    tbLoadImage->setText (tr("..."));

    grLay->addWidget (tbLoadImage, 0, 1, 1, 1);

    tbSelect = new QToolButton (this);
    tbSelect->setToolTip (tr("Select fragment"));
    tbSelect->setCheckable (true);
    tbSelect->setText (tr("..."));
    grLay->addWidget (tbSelect, 1, 1, 1, 1);

    tbSaveToDb = new QToolButton (this);
    tbSaveToDb->setToolTip (tr("Save to database"));
    tbSaveToDb->setText (tr("..."));
    grLay->addWidget (tbSaveToDb, 2, 1, 1, 1);

    tbLoadFromDb = new QToolButton (this);
    tbLoadFromDb->setToolTip (tr("Load image from database"));
    tbLoadFromDb->setText (tr("..."));
    grLay->addWidget (tbLoadFromDb, 3, 1, 1, 1);

    QSpacerItem * vSpacer = new QSpacerItem(20, 128, QSizePolicy::Minimum, QSizePolicy::Expanding);
    grLay->addItem (vSpacer, 4, 1, 1, 1);
}
