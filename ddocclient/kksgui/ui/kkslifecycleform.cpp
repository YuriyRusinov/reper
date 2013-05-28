/* 
 * File:   kkslifecycleform.cpp
 * Author: rusinov
 * 
 * Created on 28 Май 2013 г., 15:24
 */

#include <KKSLifeCycle.h>
#include "kkslifecycleform.h"
#include "ui_kks_life_cycle_form.h"

kkslifecycleform::kkslifecycleform(KKSLifeCycleEx * lc, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::kks_life_cycle_form),
    lifeCycle (lc)
{
    UI->setupUi (this);

    connect (UI->tbStartState, SIGNAL(clicked()), this, SLOT (setState()) );
    connect (UI->tbClearStartState, SIGNAL (clicked()), this, SLOT (clearState()) );
    connect (UI->pbOk, SIGNAL(clicked()), this, SLOT (lcAccept()));
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

kkslifecycleform::~kkslifecycleform()
{
}

void kkslifecycleform::lcAccept (void)
{
    QDialog::accept();
}

void kkslifecycleform::setState (void)
{

}

void kkslifecycleform::clearState (void)
{
}