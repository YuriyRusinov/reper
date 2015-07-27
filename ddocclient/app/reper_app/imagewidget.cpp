#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>
#include <QGridLayout>
#include <QSize>
#include <QRect>
#include "ImageLabel.h"
#include <QSpacerItem>
#include <QScrollArea>
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
    QSize rSize = sizeHint();
    resize (rSize);
}

void ImageWidget :: selectRect (void)
{
    ImageLabel * iL = qobject_cast<ImageLabel *>(lRImage);
    if (!iL)
        return;
//    QRect sRect = iL->getSelection ();
    QImage sImage = getSelectedImage();//rGIm.copy (sRect);
    emit searchByIm (sImage);
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
    QSize imMinSize (200, 200);
    lRImage->setMinimumSize (imMinSize);
    QScrollArea * scImArea = new QScrollArea (this);
    lRImage->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    lRImage->setScaledContents(true);
    scImArea->setWidget (lRImage);
    scImArea->setWidgetResizable (true);

    grLay->addWidget (scImArea, 0, 0, 5, 1);

    tbLoadImage = new QToolButton (this);
    tbLoadImage->setToolTip (tr("Load golographic image from file"));
    tbLoadImage->setText (tr("Load image..."));

    grLay->addWidget (tbLoadImage, 0, 1, 1, 1);

    tbSelect = new QToolButton (this);
    tbSelect->setToolTip (tr("Search by image fragment"));
    tbSelect->setText (tr("Search..."));
    grLay->addWidget (tbSelect, 1, 1, 1, 1);

    tbSaveToDb = new QToolButton (this);
    tbSaveToDb->setToolTip (tr("Save to database"));
    tbSaveToDb->setText (tr("Save to DB"));
    grLay->addWidget (tbSaveToDb, 2, 1, 1, 1);
    tbSaveToDb->setVisible (false);

    tbLoadFromDb = new QToolButton (this);
    tbLoadFromDb->setToolTip (tr("Load image from database"));
    tbLoadFromDb->setText (tr("Load from DB"));
    grLay->addWidget (tbLoadFromDb, 3, 1, 1, 1);
    tbLoadFromDb->setVisible (false);

    QSpacerItem * vSpacer = new QSpacerItem(20, 128, QSizePolicy::Minimum, QSizePolicy::Expanding);
    grLay->addItem (vSpacer, 4, 1, 1, 1);
}

const QImage& ImageWidget :: getImage (void) const
{
    return rGIm;
}

QImage ImageWidget :: getSelectedImage (void) const
{
    if (!qobject_cast<ImageLabel *>(lRImage))
        return QImage ();

    QRect selRect = (qobject_cast<ImageLabel *>(lRImage))->getSelection();
    return rGIm.copy (selRect);
}
