/* 
 * File:   kksstateform.cpp
 * Author: yuriyrusinov
 * 
 * Created on 30 Май 2013 г., 15:24
 */

#include <KKSState.h>
#include "kksstateform.h"
#include "ui_kks_state_form.h"

kksstateform::kksstateform(KKSState * st, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::kks_state_form),
    state (st)
{
    UI->setupUi (this);
    if (st)
        state->addRef();
    
    UI->lEID->setText (QString::number (st->id()));
    UI->lEName->setText (st->name());
    UI->lEDescription->setText (state->desc());
    connect (UI->pbOk, SIGNAL(clicked()), this, SLOT (stAccept()));
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

kksstateform::~kksstateform()
{
    delete UI;
    if (state)
        state->release();
}

void kksstateform::stAccept (void)
{
    state->setName (UI->lEName->text());
    state->setDesc (UI->lEDescription->text());
    state->setIsSystem (UI->chSystem->checkState() == Qt::Checked);
    QDialog::accept();
}

KKSState * kksstateform::getState (void) const
{
    return state;
}