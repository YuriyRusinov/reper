/* 
 * File:   kkslifecycleform.cpp
 * Author: rusinov
 * 
 * Created on 28 Май 2013 г., 15:24
 */

#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QtDebug>

#include <KKSLifeCycle.h>
#include <KKSState.h>
#include <KKSMap.h>
#include <KKSStateCross.h>
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
    this->initStates();

    connect (UI->tbStartState, SIGNAL(clicked()), this, SLOT (setState()) );
    connect (UI->tbClearStartState, SIGNAL (clicked()), this, SLOT (clearState()) );

    connect (UI->tbAutoStateAttribute, SIGNAL (clicked()), this, SLOT (setStateAttr()) );
    connect (UI->tbClearAutoStateAttribute, SIGNAL (clicked()), this, SLOT (clearStateAttr()) );

    connect (UI->tbStateInd, SIGNAL (clicked()), this, SLOT (setStateInd()) );
    connect (UI->tbClearStateInd, SIGNAL (clicked()), this, SLOT (clearStateInd()) );

    connect (UI->tbAddState, SIGNAL (clicked()), this, SLOT (addState()) );
    connect (UI->tbDelState, SIGNAL (clicked()), this, SLOT (delState()) );

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
    emit loadState (lifeCycle, UI->lEStartState, lcStart);
}

void kkslifecycleform::clearState (void)
{
    UI->lEStartState->clear();
    lifeCycle->setStartState (0);
}

void kkslifecycleform::setStateAttr (void)
{
    emit loadState (lifeCycle, UI->lEAutoStateAttribute, lcAttrChanged);
}

void kkslifecycleform::setStateInd (void)
{
    emit loadState (lifeCycle, UI->lEAutoStateIndicator, lcIndChanged);
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

void kkslifecycleform::addState (void)
{
    emit addLCState (lifeCycle, UI->tvStates->model());
}

void kkslifecycleform::delState (void)
{
    QItemSelectionModel * selModel = UI->tvStates->selectionModel();
    QModelIndex selInd = selModel->currentIndex();
    int idState = selInd.data(Qt::UserRole).toInt();
    lifeCycle->removeState (idState);
    QAbstractItemModel * stMod = UI->tvStates->model ();
    int iRow = selInd.row();
    stMod->removeRows (iRow, 1);
}

void kkslifecycleform::initStates (void)
{
    int nStates (lifeCycle->states().size());
    QAbstractItemModel * stateModel = new QStandardItemModel (nStates, 1);
    stateModel->setHeaderData(0, Qt::Horizontal, tr("Available states"), Qt::DisplayRole);

    int i=0;
    for (KKSMap<int, KKSState*>::const_iterator ps=lifeCycle->states().constBegin();
            ps != lifeCycle->states().constEnd();
            ps++)
    {
        QModelIndex stInd = stateModel->index (i, 0);
        KKSState * st = ps.value();
        stateModel->setData(stInd, st->id(), Qt::UserRole);
        stateModel->setData(stInd, st->name(), Qt::DisplayRole);
        i++;
    }
    UI->tvStates->setModel (stateModel);
}