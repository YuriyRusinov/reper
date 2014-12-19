#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>

#include "imagewidget.h"
#include "ui_image_widget.h"

ImageWidget :: ImageWidget (QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    UI (new Ui::Image_widget),
    rGIm (QImage())
{
    UI->setupUi (this);

    connect (UI->tbLoadImage, SIGNAL (clicked()), this, SLOT (loadImageFile()) );
    connect (UI->tbSelect, SIGNAL (pressed()), this, SLOT (selectRect()) );
    connect (UI->tbSaveToDb, SIGNAL (clicked()), this, SLOT (saveImageToDb()) );
    connect (UI->tbLoadFromDb, SIGNAL (clicked()), this, SLOT (loadImageFromDb()) );
}

ImageWidget :: ~ImageWidget (void)
{
    delete UI;
}

void ImageWidget :: setImage (const QImage& im)
{
    rGIm = im;
    if (!rGIm.isNull())
        UI->lRImage->setPixmap (QPixmap::fromImage (rGIm));
    else
        UI->lRImage->setPixmap (QPixmap());
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

