/* 
 * File:   KKSIncludesDialog.cpp
 * Author: rusinov
 * 
 * Created on 5 Декабрь 2012 г., 12:25
 */

#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>

#include "kksincludeswidget.h"
#include "KKSIncludesDialog.h"

KKSIncludesDialog::KKSIncludesDialog(KKSIncludesWidget * iW, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    incWidget (iW),
    pbOk (new QPushButton (tr("&OK"), this)),
    pbCancel (new QPushButton (tr("&Cancel"), this))
{
    QGridLayout * gLay = new QGridLayout (this);
    incWidget->setParent(this);
    gLay->addWidget (incWidget, 0, 0, 1, 1);

    QHBoxLayout * hButtonsLay = new QHBoxLayout ();
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbOk);
    hButtonsLay->addWidget (pbCancel);
    gLay->addLayout (hButtonsLay, 1, 0, 1, 1);

    connect (pbOk, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

KKSIncludesDialog::~KKSIncludesDialog()
{
    if (incWidget)
    {
        incWidget->setParent (0);
        delete incWidget;
    }
}

KKSIncludesWidget * KKSIncludesDialog :: getIncWidget (void) const
{
    return incWidget;
}

