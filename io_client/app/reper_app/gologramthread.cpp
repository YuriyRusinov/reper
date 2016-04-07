#include <QtDebug>
#include "gologramthread.h"

ImageGeneratorControl :: ImageGeneratorControl (const generatingDataPlus& gdp, QObject * parent)
    : QObject (parent),
    ImageThread (new QThread(this)),
    generator (new ImageGenerator(gdp, 0)),
    resD (QVector<returningData>())
{
    generator->moveToThread (ImageThread);

    connect (ImageThread, SIGNAL (finished()), this, SLOT (imageGenerated()) );
    ImageThread->start ();
}

ImageGeneratorControl :: ~ImageGeneratorControl (void)
{
    ImageThread->quit ();
    ImageThread->wait ();
}

void ImageGeneratorControl :: imageGenerated (void)
{
    deleteLater ();
}

void ImageGeneratorControl :: generateImages (void)
{
    generator->loadModel ();
    resD = generator->generateImages();
    emit imageGenerated (resD);
}

 const QVector<returningData>& ImageGeneratorControl :: getImageResults (void) const
{
    return resD;
}
