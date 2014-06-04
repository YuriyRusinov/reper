/* 
 * File:   KKSStuffDialog.cpp
 * Author: rusinov
 * 
 * Created on 5 Декабрь 2012 г., 12:25
 */

#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>

#include "kksstuffform.h"
#include "KKSStuffDialog.h"

KKSStuffDialog::KKSStuffDialog(KKSStuffForm * stForm, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    stuffWidget (stForm),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this))
{
    QGridLayout * gLay = new QGridLayout (this);
    gLay->addWidget (stuffWidget, 0, 0, 1, 1);

    QHBoxLayout * hButtonsLay = new QHBoxLayout ();
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
    gLay->addLayout (hButtonsLay, 1, 0, 1, 1);

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSStuffDialog::~KKSStuffDialog()
{
    if (stuffWidget)
        delete stuffWidget;
}

KKSStuffForm * KKSStuffDialog :: getStuffForm (void) const
{
    return stuffWidget;
}

