/*
 * File:   KKSAttrValuePropsForm.cpp
 * Author: ksa 
 *
 * Created on 27 марта 2012 г., 22:04
 */
#include <QDoubleValidator>
#include <QIntValidator>
#include <QtDebug>
#include <QMessageBox>

#include <KKSAttrValue.h>
#include <KKSObject.h>
#include <attrhistory.h>

#include "KKSAttrValuePropsForm.h"
#include "ui_kksattr_value_props_form.h"

KKSAttrValuePropsForm::KKSAttrValuePropsForm(KKSAttrValue * av, bool mode, bool forRecords, QWidget * parent, Qt::WindowFlags flags)
      : QDialog (parent, flags),
        UI (new Ui::kksattr_value_props_form),
        m_av (NULL),
        m_forRecords(forRecords),
        ioSrc(NULL),
        ioSrc1(NULL)
{
    UI->setupUi (this);
    
    setWindowModality(Qt::WindowModal);

    setAttrValue(av);

    this->initForm (mode);

    UI->pbOk->setVisible (mode);
    UI->pbCancel->setVisible (mode);
    UI->pbApply->setVisible (mode);

    
    
    UI->lEName->setReadOnly (!mode);

    
    connect (UI->tbSourceView, SIGNAL (clicked()), this, SLOT (viewIOSource()) );
    connect (UI->tbSource, SIGNAL (clicked()), this, SLOT (loadIOSource()) );
    connect (UI->tbTransferView, SIGNAL (clicked()), this, SLOT (viewIOTransfer()) );
    connect (UI->tbTransfer, SIGNAL (clicked()), this, SLOT (loadIOTransfer()) );
    connect (UI->chTo, SIGNAL (stateChanged(int)), this, SLOT (chToStateChanged (int)) );

    connect (UI->pbOk, SIGNAL (clicked()), this, SLOT (apply()) );
    connect (UI->pbApply, SIGNAL (clicked()), this, SLOT (saveAttrValue()) );
    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject ()) );
    connect (UI->pbViewHistory, SIGNAL(clicked()), this, SLOT (pbHistoryClicked()));
}

KKSAttrValuePropsForm::~KKSAttrValuePropsForm()
{
    if (m_av)
        m_av->release ();

    if(ioSrc)
        ioSrc->release();

    if(ioSrc1)
        ioSrc1->release();

    delete UI;
}

void KKSAttrValuePropsForm :: setAttrValue(KKSAttrValue * av)
{
    if(m_av)
        m_av->release();

    m_av = av;

    if(m_av)
        m_av->addRef();

    if(ioSrc)
        ioSrc->release();

    if(ioSrc1)
        ioSrc1->release();
    
    if(m_av->ioSrc())
        ioSrc = new KKSObject(*(m_av->ioSrc()));
    else
        ioSrc = NULL;
    
    if(m_av->ioSrc1())
        ioSrc1 = new KKSObject(*(m_av->ioSrc1()));
    else
        ioSrc1 = NULL;
}


KKSAttrValue * KKSAttrValuePropsForm :: attrValue(void) const
{
    return m_av;
}


void KKSAttrValuePropsForm :: viewIOSource (void)
{
    emit viewIOSrc (ioSrc, this);
}

void KKSAttrValuePropsForm :: loadIOSource (void)
{
    KKSObject * io = NULL;
    emit loadIOSrc (&io, this);
    if(!io){
        //UI->lESource->setText(QString::null);
        return;
    }

    if(ioSrc)
        ioSrc->release();

    ioSrc = io;

    if(ioSrc)
        UI->lESource->setText(ioSrc->name());

    //io->addRef() -- делать не надо, поскольку там идет KKSLoader::loadIO
}

void KKSAttrValuePropsForm :: viewIOTransfer (void)
{
    emit viewIOSrc (ioSrc1, this);
}

void KKSAttrValuePropsForm :: loadIOTransfer (void)
{
    KKSObject * io = NULL;
    emit loadIOSrc (&io, this);
    if(!io){
        //UI->lESource->setText(QString::null);
        return;
    }

    if(ioSrc1)
        ioSrc1->release();


    ioSrc1 = io;

    if(ioSrc1)
        UI->lETransfer->setText(ioSrc1->name());

    //io->addRef() -- делать не надо, поскольку там идет KKSLoader::loadIO
}

void KKSAttrValuePropsForm :: saveAttrValue (void)
{
    if(!m_av)
        return;

    if (UI->dTEFrom->dateTime().isValid())
        m_av->setStartDateTime (UI->dTEFrom->dateTime());

    if (UI->dTETo->dateTime().isValid() && UI->dTEFrom->dateTime().isValid() && UI->dTETo->dateTime() >= UI->dTEFrom->dateTime())
        m_av->setStopDateTime (UI->dTETo->dateTime());

    m_av->setIOSrc(ioSrc);
    m_av->setIOSrc1(ioSrc1);

    //if (UI->dTEMeasured->dateTime().isValid())
    //    m_av->setMeasDateTime(UI->dTEMeasured->dateTime());

    m_av->setDesc(UI->tEDescription->toPlainText());

}

void KKSAttrValuePropsForm :: chToStateChanged (int state)
{
    bool isTo (state == Qt::Checked);
    UI->dTETo->setEnabled (isTo);
}

void KKSAttrValuePropsForm :: apply (void)
{
    saveAttrValue ();
    accept ();
}

void KKSAttrValuePropsForm :: initForm (bool mode)
{
    Q_UNUSED(mode);
    if (!m_av || !m_av->attribute())
        return;

    UI->lEName->setText (m_av->attribute()->name());
    UI->lEType->setText (m_av->attribute()->type()->name());
    UI->lEGroup->setText (m_av->attribute()->group()->name());

    //Ќачало актуальности значени€ атрибута есть всегда.
    //ѕри этом если создаетс€ новое значение атрибута (например, при создании »ќ),
    //то оно будет не задано. ¬ этом случае надо его выставить в текущую дату и врем€
    QDateTime dtStart = m_av->startDateTime();
    if(dtStart.isValid()){
        UI->dTEFrom->setDateTime (dtStart);
        UI->dTEFrom->setEnabled(false);
    }
    else{
        UI->dTEFrom->setDateTime (QDateTime::currentDateTime());
        UI->dTEFrom->setEnabled(false);
    }

    //момент утери актуальности дл€ активных значений атрибутов не задан
    //¬ этом случае запрещаем редактировать данный виджет
    QDateTime dtStop = m_av->stopDateTime();
    UI->dTETo->setEnabled (false); //всегда выставл€ем в false, поскольку момекнт утери актуальности значени€ наступает при изменении значени€ или его доп. характеристик
    if(dtStop.isValid())
    {
        UI->dTETo->setDateTime (dtStop);
        UI->chTo->setCheckState (Qt::Checked);
        //UI->dTETo->setEnabled (true);
        UI->swDTTo->setCurrentIndex(0);
    }
    else // if (!mode)
    {
        //UI->dTETo->setEnabled (false);
        UI->chTo->setCheckState (Qt::Unchecked);
        UI->chTo->setEnabled (false);
        //UI->dTETo->setVisible(false);
        UI->swDTTo->setCurrentIndex(1);
        //UI->chTo->setText( UI->chTo->text() + tr("   Not assigned yet"));
    }
    //else
    //{
    //    UI->dTETo->setEnabled (false);
    //    UI->chTo->setCheckState (Qt::Unchecked);
    //}
    
    if (m_av->ioSrc())
        UI->lESource->setText (m_av->ioSrc()->name());
    
    if (m_av->ioSrc1())
        UI->lETransfer->setText (m_av->ioSrc1()->name());
    
    //QDateTime dtMeas = m_av->measDateTime();
    //if(dtMeas.isValid())
    //    UI->dTEMeasured->setDateTime (dtMeas);
    //else
    //    UI->dTEMeasured->setDateTime (QDateTime::currentDateTime());

    QDateTime dtInsert = m_av->insertDateTime();
    if(dtInsert.isValid()){
        UI->dTEInserted->setDateTime (dtInsert);
        UI->swDTInsert->setCurrentIndex(0);
    }
    else{
        //UI->dTEInserted->setVisible(false);
        UI->swDTInsert->setCurrentIndex(1);
        //UI->lInsertDb->setText(UI->lInsertDb->text() + tr("   Not assigned yet"));
    }

    UI->tEDescription->setPlainText (m_av->desc());

    UI->cbActual->setChecked(m_av->isActual());
}


void KKSAttrValuePropsForm::pbHistoryClicked()
{
    emit loadHistory(m_av, m_forRecords);
}

void KKSAttrValuePropsForm::viewHistory(const KKSList<KKSAttrValue*> & history)
{
    if(history.count() == 0)
        return;

    AttrHistory * hForm = new AttrHistory(history, this);
    hForm->exec();
    delete hForm;

}
