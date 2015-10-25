#include <QTreeView>
#include <QLineEdit>
#include <QAbstractItemModel>
#include <QLabel>
#include <QToolBar>
#include <QLineEdit>
#include <QGridLayout>
#include <QPixmap>
#include <QGroupBox>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QtDebug>

#include "searchresultsform.h"

SearchResultsForm :: SearchResultsForm (const QImage& im, QWidget * parent, Qt::WindowFlags flags)
    : QWidget (parent, flags),
    sImage (im),
    tView (new QTreeView (this)),
    lImage (new QLabel (this)),
    tbActions (new QToolBar (this)),
    lFilter (0),//new QLabel(tr("Filter"), this)),
    filterLE (new QLineEdit (this)),
    gbFilter (new QGroupBox (this))
{
    setWindowTitle (tr("Search results"));
    lImage->setPixmap(QPixmap::fromImage (sImage));
    this->init ();

    connect (filterLE, SIGNAL (textEdited ( const QString &)), this, SLOT (filterRecs (const QString&)) );
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
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (tbActions, 0, 0, 1, 2);
    gLay->addWidget (tView, 1, 1, 1, 1);
    gLay->addWidget (lImage, 1, 0, 1, 1);

    QSize iconSize (24, 24);
    tbActions->setIconSize (iconSize);
    QGridLayout * gFLay = new QGridLayout (gbFilter);
    lFilter = new QLabel (tr("Filter"), gbFilter);
    gFLay->addWidget (lFilter, 0, 0, 1, 1);
    gFLay->addWidget (filterLE, 0, 1, 1, 1);
    tbActions->addWidget (gbFilter);
}

void SearchResultsForm :: filterRecs (const QString& text)
{
    qDebug () << __PRETTY_FUNCTION__ << text;
    QSortFilterProxyModel * sortMod = qobject_cast<QSortFilterProxyModel *>(tView->model ());
    if (!sortMod)
        return;
    sortMod->setFilterRegExp (text);
    sortMod->setFilterCaseSensitivity (Qt::CaseInsensitive);
    sortMod->setFilterKeyColumn (-1);
}
