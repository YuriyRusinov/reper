/* 
 * File:   kksstateform.cpp
 * Author: yuriyrusinov
 * 
 * Created on 30 ��� 2013 �., 15:24
 */

#include <QTextEdit>

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
    UI->tEDescription->setPlainText (state->desc());
    
    UI->hParentLay->setEnabled (false);
    UI->lParent->setEnabled (false);
    UI->lEParent->setEnabled (false);
    UI->tbSetParent->setEnabled (false);
    UI->tbClearParent->setEnabled (false);

    QIcon addIcon(":/ddoc/accept.png");
    QIcon clearIcon(":/ddoc/remove_icon.png");
    UI->tbSetParent->setIcon(addIcon);
    UI->tbClearParent->setIcon (clearIcon);
    
    connect (UI->tbSetParent, SIGNAL(clicked()), this, SLOT (setParent()) );
    connect (UI->tbClearParent, SIGNAL(clicked()), this, SLOT(clearParent()) );

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
    state->setDesc (UI->tEDescription->toPlainText());
    QDialog::accept();
}

KKSState * kksstateform::getState (void) const
{
    return state;
}

void kksstateform::setParent (void)
{
    
}

void kksstateform::clearParent (void)
{
    state->setParent(0);
    UI->lEParent->clear();
}
