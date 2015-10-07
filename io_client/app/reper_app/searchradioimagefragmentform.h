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

private slots:
    void brFilt (void);
    void pbCalc (void);

private:
    //
    // Variables
    //
    Ui::search_image_fragment_form * UI;
    QImage sourceImage;
    QImage filteredImage;
    QLabel * lSImage;
    QLabel * lFImage;

private:
    Q_OBJECT
};

#endif
