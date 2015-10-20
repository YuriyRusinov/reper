#ifndef _SearchRadioImageFragmentForm_H
#define _SearchRadioImageFragmentForm_H

#include <QDialog>
#include <QImage>

class QLabel;

namespace Ui
{
    class search_image_fragment_form;
};

class SearchRadioImageFragmentForm : public QDialog
{
public:
    SearchRadioImageFragmentForm (const QImage& sImage=QImage(), QWidget * parent=0, Qt::WindowFlags flags=0);
    virtual ~SearchRadioImageFragmentForm (void);

    const QImage& getSourceImage (void) const;
    const QImage& getFilteredImage (void) const;
    void setSourceImage (const QImage& im);
    void setFilteredImage (const QImage& im);

public slots:
    void pbCalc (void);

private slots:
    void brFilt (void);
    void setResults (int pix_length, int pix_width, double azimuth);

signals:
    void calcParams (const QImage& im, double cVal);

private:
    //
    // Variables
    //
    Ui::search_image_fragment_form * UI;
    QImage sourceImage;
    QImage filteredImage;
    QLabel * lSImage;
    QLabel * lFImage;
    bool isFilt;

private:
    Q_OBJECT
};

#endif
