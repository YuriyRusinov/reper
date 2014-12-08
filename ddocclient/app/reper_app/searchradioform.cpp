#include <QValidator>
#include <QDoubleValidator>
#include "searchradioform.h"
#include "ui_search_radio_form.h"

SearchRadioForm :: SearchRadioForm (QWidget * parent; Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::Search_Radio_Form)
{
    UI->setupUi (this);

    init ();

    connect (UI->tbLoadImage, SIGNAL (clicked()), this, SLOT (loadImage()) );
    connect (UI->tbClearImage, SIGNAL (clicked()), this, SLOT (clearImage()) );
    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

SearchRadioForm :: ~SearchRadioForm (void)
{
}

void SearchRadioForm :: loadImage (void)
{
}

void SearchRadioForm :: clearImage (void)
{
}

void SearchRadioForm :: init (void)
{
    QValidator * dAzVal = new QDoubleValidator (0.0, 360.0, 16, this);
    UI->lEAzimuth->setValidator (dAzVal);

    QValidator * dElevVal = new QDoubleValidator (-90.0, 90.0, 16, this);
    UI->lEElevationAngle->setValidator (dElevVal);
}
