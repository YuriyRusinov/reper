/* 
 * File:   KKSRubricMessageBox.cpp
 * Author: yuriy
 * 
 * Created on 20 Ноябрь 2012 г., 17:11
 */
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "KKSRubricMessageBox.h"

KKSRubricMessageBox :: KKSRubricMessageBox(QWidget * parent, Qt::WindowFlags flags)
    : QDialog (parent, flags),
    vLay (new QVBoxLayout (this)),
    hLabelLay (new QHBoxLayout),
    hButtonsLay (new QHBoxLayout),
    lText (new QLabel (tr("Do you really want to delete documents from rubric ?"), this)),
    pbYes (new QPushButton (tr("&Yes"), this)),
    pbNo (new QPushButton (tr("&No"), this)),
    chDb (new QCheckBox (tr("Delete from DB"), this))
{
    init ();
}

KKSRubricMessageBox :: ~KKSRubricMessageBox()
{
}

Qt::CheckState KKSRubricMessageBox :: getDbState (void) const
{
    return chDb->checkState();
}

void KKSRubricMessageBox :: init (void)
{
    this->setWindowTitle (tr("Delete documents from rubric"));
    hLabelLay->addWidget (lText, 0, Qt::AlignJustify | Qt::AlignVCenter);
    vLay->addLayout (hLabelLay);
    vLay->addWidget (chDb);
    
    hButtonsLay->addStretch ();
    hButtonsLay->addWidget (pbYes);
    hButtonsLay->addWidget (pbNo);
    vLay->addLayout (hButtonsLay);
    
    connect (pbYes, SIGNAL (clicked()), this, SLOT (accept()) );
    connect (pbNo, SIGNAL (clicked()), this, SLOT (reject()) );
}