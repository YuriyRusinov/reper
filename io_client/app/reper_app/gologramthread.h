#ifndef _ImageGeneratorControl_H
#define _ImageGeneratorControl_H

#include <QObject>
#include <QThread>

#include <gologramma.h>

class ImageGeneratorControl : public QObject
{
public:
    ImageGeneratorControl (const generatingDataPlus& gdp, QObject * parent=0);
    virtual ~ImageGeneratorControl (void);

private slots:
    void imageGenerated (void);

private:
    //
    // Variables
    //
    QThread * ImageThread;
    ImageGenerator * generator;

private:
    Q_OBJECT
};

#endif
