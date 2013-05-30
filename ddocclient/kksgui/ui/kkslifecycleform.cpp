/* 
 * File:   kkslifecycleform.cpp
 * Author: rusinov
 * 
 * Created on 28 Май 2013 г., 15:24
 */

#include <KKSLifeCycle.h>
#include <KKSState.h>
#include "kkslifecycleform.h"
#include "ui_kks_life_cycle_form.h"

kkslifecycleform::kkslifecycleform(KKSLifeCycleEx * lc, QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    UI (new Ui::kks_life_cycle_form),
    lifeCycle (lc)
{
    UI->setupUi (this);
    if (lc)
        lifeCycle->addRef();
    
    UI->lEId->setText (QString::number (lc->id()));
    UI->lEName->setText (lifeCycle->name());
    UI->lEDescription->setText (lifeCycle->desc());
    if (lifeCycle->startState())
    {
        const KKSState * st = lifeCycle->startState();
        UI->lEStartState->setText (st->name());
    }
    if (lifeCycle->autoStateAttr())
    {
        const KKSState * st = lifeCycle->autoStateAttr();
        UI->lEAutoStateAttribute->setText (st->name());
    }
    if (lifeCycle->autoStateInd())
    {
        const KKSState * st = lifeCycle->autoStateInd();
        UI->lEAutoStateIndicator->setText (st->name());
    }

    connect (UI->tbStartState, SIGNAL(clicked()), this, SLOT (setState()) );
    connect (UI->tbClearStartState, SIGNAL (clicked()), this, SLOT (clearState()) );
    connect (UI->tbAutoStateAttribute, SIGNAL (clicked()), this, SLOT (setStateAttr()) );
    connect (UI->tbClearAutoStateAttribute, SIGNAL (clicked()), this, SLOT (clearStateAttr()) );
    connect (UI->tbStateInd, SIGNAL (clicked()), this, SLOT (setStateInd()) );
    connect (UI->tbClearStateInd, SIGNAL (clicked()), this, SLOT (clearStateInd()) );
    connect (UI->pbOk, SIGNAL(clicked()), this, SLOT (lcAccept()));
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
}

kkslifecycleform::~kkslifecycleform()
{
    delete UI;
    if (lifeCycle)
        lifeCycle->release();
}

void kkslifecycleform::lcAccept (void)
{
    lifeCycle->setName (UI->lEName->text());
    lifeCycle->setDesc (UI->lEDescription->text());
    QDialog::accept();
}

void kkslifecycleform::setState (void)
{
    emit loadStartState (lifeCycle, UI->lEStartState);
}

void kkslifecycleform::clearState (void)
{
    UI->lEStartState->clear();
    lifeCycle->setStartState (0);
}

void kkslifecycleform::setStateAttr (void)
{
    emit loadStateAttribute (lifeCycle, UI->lEStartState);
}

void kkslifecycleform::setStateInd (void)
{
    emit loadStateInd (lifeCycle, UI->lEStartState);
}

void kkslifecycleform::clearStateAttr (void)
{
    UI->lEAutoStateAttribute->clear();
    lifeCycle->setAutoStateAttr(0);
}

void kkslifecycleform::clearStateInd (void)
{
    UI->lEAutoStateIndicator->clear ();
    lifeCycle->setAutoStateInd(0);
}

KKSLifeCycleEx * kkslifecycleform::getLC (void) const
{
    return lifeCycle;
}