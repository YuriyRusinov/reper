#ifndef _SearchRadioImageCalc_H
#define _SearchRadioImageCalc_H

#include <QObject>
#include <QImage>
#include <QSize>
#include <QVector>

#include <cv.h>

#include "seaobjectparameters.h"

using cv::Mat;

class QAbstractItemModel;
class QWidget;

class SearchRadioImageFragmentForm;
class SearchResultsForm;
class ImageWidget;

class KKSLoader;
class KKSObjEditorFactory;

class SearchRadioImageCalc : public QObject
{
public:
    SearchRadioImageCalc (KKSLoader * _l, KKSObjEditorFactory * _oef, QObject * parent=0);
    virtual ~SearchRadioImageCalc (void);

    ImageWidget * GUISearchInit (QWidget * parent=0, Qt::WindowFlags flags=0);
    SearchRadioImageFragmentForm * GUIImageView (const QImage& im=QImage(), QWidget * parent=0, Qt::WindowFlags flags=0);
    SearchResultsForm * GUIResultsView (QWidget * parent=0, Qt::WindowFlags flags=0);
    
private slots:
    void calculateParameters (const QImage& im, double cVal);
    void calcChi2 (QAbstractItemModel * sModel, const QImage& sIm, double az, double elev);
    void searchInitIm (const QImage& im);
    void searchIm (const QImage& fImage, double az, double elev);
    void searchParams (const QImage& sIm, SeaObjectParameters sp);

signals:
    void setVals (int pl, int pw, double az);
    void viewWidget (QWidget *);

private:
    //
    // Functions
    //
    QSize imageShipParameters (const QImage& filteredIm) const;
    QByteArray searchImageB (const QImage& sIm, double az, double elev) const;
    QByteArray getImageStr (const QByteArray& b) const;

    //cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );
    QVector<SeaObjectParameters> imageAnalyse (const QImage& inImage);
private:
    //
    // Variables
    //
    QImage searchImage;
    double azimuth;
    double elevation_angle;
    KKSLoader * loader;
    KKSObjEditorFactory * oef;

private:
    Q_OBJECT
};

#endif
