#ifndef _ImageGeneratorControl_H
#define _ImageGeneratorControl_H

#include <QObject>
#include <QThread>
#include <QVector>

#include <gologramma.h>

class ImageGeneratorControl : public QObject
{
public:
    ImageGeneratorControl (const generatingDataPlus& gdp, QObject * parent=0);
    virtual ~ImageGeneratorControl (void);

public slots:
    void generateImages (void);

private slots:
    void imageGenerated (void);

signals:
    void imageGenerated (const QVector<returningData>& resD);

private:
    //
    // Variables
    //
    QThread * ImageThread;
    ImageGenerator * generator;
    QVector<returningData> resD;

private:
    Q_OBJECT
};

#endif
