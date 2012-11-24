#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QtDebug>

#include "KKSSyncWidget.h"
#include "KKSSyncDialog.h"

KKSSyncDialog :: KKSSyncDialog (QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    syncW (new KKSSyncWidget (this, true)),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this))
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (syncW, 0, 0, 1, 1);

    QHBoxLayout * hButtonsLay = new QHBoxLayout ();
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
    gLay->addLayout (hButtonsLay, 1, 0, 1, 1);

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSSyncDialog :: ~KKSSyncDialog (void)
{
}

KKSSyncWidget * KKSSyncDialog :: getSyncWidget (void) const
{
    return syncW;
}
