#include <QTreeView>
#include <QLineEdit>
#include <QAbstractItemModel>
#include <QLabel>
#include <QToolBar>
#include <QLineEdit>
#include <QGridLayout>
#include <QPixmap>

#include "searchresultsform.h"

SearchResultsForm :: SearchResultsForm (const QImage& im, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    tView (new QTreeView (this)),
    lImage (new QLabel (this)),
    tbActions (new QToolBar (this)),
    lFilter (new QLabel(tr("Filter"), this)),
    filterLE (new QLineEdit (this)),
    sImage (im)
{
    this->init ();
}

SearchResultsForm :: ~SearchResultsForm (void)
{
}

void SearchResultsForm :: setSearchImage (const QImage& sIm)
{
    sImage = sIm;
    lImage->setPixmap(QPixmap::fromImage (sImage));
}

void SearchResultsForm :: setResultsModel (QAbstractItemModel * mod)
{
    QAbstractItemModel * oldMod = tView->model ();
    tView->setModel (mod);
    if (oldMod && oldMod != mod)
        delete oldMod;
}

void SearchResultsForm :: init (void)
{
}
