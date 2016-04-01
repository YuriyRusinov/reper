#include <QtDebug>
#include "gologramthread.h"

ImageGeneratorControl :: ImageGeneratorControl (const generatingDataPlus& gdp, QObject * parent)
    : QObject (parent),
    ImageThread (new QThread(this)),
    generator (new ImageGenerator(gdp, this))
{
    generator->moveToThread (ImageThread);

    connect (ImageThread, SIGNAL (finished()), this, SLOT (imageGenerate()) );
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
