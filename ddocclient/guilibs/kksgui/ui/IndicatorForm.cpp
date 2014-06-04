/*
 * File:   IndicatorForm.cpp
 * Author: yuriy
 *
 * Created on 28 Февраль 2012 г., 22:04
 */
#include <QDoubleValidator>
#include <QIntValidator>
#include <QtDebug>
#include <KKSIndicatorValue.h>
#include <KKSIndicator.h>
#include <KKSObject.h>

#include "IndicatorForm.h"
#include "ui_indicator_form.h"

IndicatorForm::IndicatorForm(KKSIndicatorValue * av, bool mode, QWidget * parent, Qt::WindowFlags flags)
      : QDialog (parent, flags),
        UI (new Ui::indicator_form),
        indicatorVal (NULL)
{
    UI->setupUi (this);

    setIndValue(av);

    this->initForm (mode);

    //UI->buttonsLayout->setVisible (mode);
    UI->pbOk->setVisible (mode);
    UI->pbCancel->setVisible (mode);
    UI->pbApply->setVisible (mode);
    UI->lEIndicatorName->setReadOnly (!mode);

    connect (UI->tbIndicatorType, SIGNAL (clicked()), this, SLOT (loadIndType()) );
    connect (UI->tbSourceView, SIGNAL (clicked()), this, SLOT (viewIOSource()) );
    connect (UI->tbSource, SIGNAL (clicked()), this, SLOT (loadIOSource()) );
    connect (UI->tbTransferView, SIGNAL (clicked()), this, SLOT (viewIOTransfer()) );
    connect (UI->tbTransfer, SIGNAL (clicked()), this, SLOT (loadIOTransfer()) );
    connect (UI->chTo, SIGNAL (stateChanged(int)), this, SLOT (chToStateChanged (int)) );

    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (applyInd()) );
    connect (UI->pbApply, SIGNAL (clicked()), this, SLOT (saveInd()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject ()) );
}

IndicatorForm::~IndicatorForm()
{
    if (indicatorVal)
        indicatorVal->release ();
    delete UI;
}

void IndicatorForm :: setIndValue(KKSIndicatorValue * av)
{
    if(indicatorVal)
        indicatorVal->release();

    indicatorVal = av;

    if(indicatorVal)
        indicatorVal->addRef();
}

KKSIndicatorValue * IndicatorForm :: indValue (void) const
{
    return indicatorVal;
}

void IndicatorForm :: loadIndType (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit loadIndicatorType (indicatorVal, UI->lEIndicatorType);
}

void IndicatorForm :: viewIOSource (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit viewIOSrc (indicatorVal);
}

void IndicatorForm :: loadIOSource (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit loadIOSrc (indicatorVal, UI->lESource);
}

void IndicatorForm :: viewIOTransfer (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit viewIOSrc1 (indicatorVal);
}

void IndicatorForm :: loadIOTransfer (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    emit loadIOSrc1 (indicatorVal, UI->lETransfer);
}

void IndicatorForm :: saveInd (void)
{
    KKSValue val (UI->lEIndValue->text(), indicatorVal->indicator()->type()->attrType());
    indicatorVal->setValue (val);
    if (UI->dTEFrom->dateTime().isValid())
        indicatorVal->setStartDateTime (UI->dTEFrom->dateTime());
    if (UI->dTETo->dateTime().isValid() && UI->dTEFrom->dateTime().isValid() && UI->dTETo->dateTime() >= UI->dTEFrom->dateTime())
        indicatorVal->setStopDateTime (UI->dTETo->dateTime());
    qDebug () << __PRETTY_FUNCTION__;
}

void IndicatorForm :: chToStateChanged (int state)
{
    bool isTo (state == Qt::Checked);
    UI->dTETo->setEnabled (isTo);
}

void IndicatorForm :: applyInd (void)
{
    qDebug () << __PRETTY_FUNCTION__;
    saveInd ();
    accept ();
}

void IndicatorForm :: initForm (bool mode)
{
    qDebug () << __PRETTY_FUNCTION__;
    if (!indicatorVal || !indicatorVal->indicator())
        return;
    UI->lEIndicatorName->setText (indicatorVal->indicator()->name());
    UI->lEIndicatorType->setText (indicatorVal->indicator()->indType()->name());
    UI->lEIndValue->setText (indicatorVal->value().value());
    UI->dTEFrom->setDateTime (indicatorVal->startDateTime());
    if (indicatorVal->stopDateTime().isValid())
    {
        UI->dTETo->setDateTime (indicatorVal->stopDateTime());
        UI->chTo->setCheckState (Qt::Checked);
        UI->dTETo->setEnabled (true);
    }
    else if (!mode)
    {
        UI->dTETo->setEnabled (false);
        UI->chTo->setCheckState (Qt::Unchecked);
        UI->chTo->setEnabled (false);
    }
    else
    {
        UI->dTETo->setEnabled (false);
        UI->chTo->setCheckState (Qt::Unchecked);
    }
    if (indicatorVal->ioSrc())
        UI->lESource->setText (indicatorVal->ioSrc()->name());
    if (indicatorVal->ioSrc1())
        UI->lETransfer->setText (indicatorVal->ioSrc1()->name());
    //UI->dTEMeasured->setDateTime (indicatorVal->measDateTime());
    UI->dTEInserted->setDateTime (indicatorVal->insertDateTime());
    UI->tEDescription->setPlainText (indicatorVal->indicator()->desc());
}
