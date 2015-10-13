#include "searchradioimagefragmentform.h"
#include "searchradioimagecalc.h"

SearchRadioImageCalc :: SearchRadioImageCalc (QObject * parent)
    : QObject (parent)
{
}

SearchRadioImageCalc :: ~SearchRadioImageCalc (void)
{
}

SearchRadioImageFragmentForm * SearchRadioImageCalc :: GUIImageView (const QImage& im, QWidget * parent, Qt::WindowFlags flags)
{
    SearchRadioImageFragmentForm * sForm = new SearchRadioImageFragmentForm (im, parent, flags);

    connect (sForm, SIGNAL (calcParams (const QImage&)), this, SLOT (calculateParameters (const QImage&)) );
    connect (this, SIGNAL (setVals (int, int, double)), sForm, SLOT (setResults(int, int, double)) );

    return sForm;
}

void SearchRadioImageCalc :: calculateParameters (const QImage& im)
{
    Q_UNUSED (im);
}
