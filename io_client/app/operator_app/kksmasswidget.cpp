#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>

#include "kksmasswidget.h"

MassWidget :: MassWidget (QWidget *parent, Qt::WindowFlags f)
    : QWidget (parent, f),
    chMass (new QCheckBox (tr("Mass creation"))),
    sbNum (new QSpinBox())
{
    QHBoxLayout * hLay = new QHBoxLayout ();
    this->setLayout (hLay);

    hLay->addWidget (chMass);
    hLay->addWidget (sbNum);

    chMass->setCheckState (Qt::Unchecked);
    this->setMass (chMass->checkState());
    sbNum->setRange (1, 1000);
    connect (chMass, SIGNAL (stateChanged (int)), this, SLOT (setMass(int)) );
    connect (sbNum, SIGNAL (valueChanged(int)), this, SLOT (setNumb(int)) );
}

MassWidget :: ~MassWidget (void)
{
    if (sbNum)
        delete sbNum;

    if (chMass)
        delete chMass;
}

bool MassWidget :: isMass (void) const
{
    return (chMass->checkState () == Qt::Checked);
}

int MassWidget :: num (void) const
{
    if (chMass->checkState () == Qt::Checked)
        return sbNum->value ();
    else
        return 1;
}

void MassWidget :: setMass (int state)
{
    bool isEnable (state == Qt::Checked);
    sbNum->setEnabled (isEnable);
    emit setNum (num());
}

void MassWidget :: setNumb (int val)
{
    emit setNum (val);
}
