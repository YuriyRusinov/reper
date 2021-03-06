#ifndef _SearchRadioImageFragmentForm_H
#define _SearchRadioImageFragmentForm_H

#include <QDialog>
#include <QImage>

#include "seaobjectparameters.h"

class QLabel;

namespace Ui
{
    class search_image_fragment_form;
};

class SearchRadioImageFragmentForm : public QDialog
{
public:
    SearchRadioImageFragmentForm (const QVector<SeaObjectParameters>& sp=QVector<SeaObjectParameters>(), const QImage& sImage=QImage(), QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~SearchRadioImageFragmentForm (void);

    const QImage& getSourceImage (void) const;
    const QImage& getFilteredImage (void) const;
    void setSourceImage (const QImage& im);
    void setFilteredImage (const QImage& im);

    double getAzimuth (void) const;
    double getElevation (void) const;
    int getImageWidth (void) const;
    int getImageHeight (void) const;
    int getImageDepth (void) const;

public slots:
    void pbCalc (void);
    void selObject (int index);

private slots:
    void brFilt (void);
    void setResults (int pix_length, int pix_width, double azimuth);
    void searchBegin (void);
    void elevStateChanged (int state);
    void depthStateChanged (int state);
    void secPropStateChanged (int state);

signals:
    void calcParams (const QImage& im, double cVal);
    void searchByIm (const QImage& im, double az, double elev);
    void searchByParams (const QImage& im, SeaObjectParameters sp);

protected:
    //
    // Overrides
    //
    void showEvent (QShowEvent * event);

private:
    //
    // Variables
    //
    Ui::search_image_fragment_form * UI;
    QVector<SeaObjectParameters> seaPars;
    QImage sourceImage;
    QImage filteredImage;
    QLabel * lSImage;
    QLabel * lFImage;
    bool isFilt;
    double az;

private:
    Q_OBJECT
};

#endif
