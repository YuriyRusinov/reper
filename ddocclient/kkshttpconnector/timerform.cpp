#include <QIntValidator>


#include "ui_timer_form.h"
#include "timerform.h"

TimerForm :: TimerForm (QWidget *parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::timer_form)
{
    UI->setupUi (this);


    //автоматический режим работы. ќпрос каждые 10 секунд
    this->init (10, 0, false, true);

    connect (UI->pbCancel, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (UI->pbOK, SIGNAL (clicked()), this, SLOT (setTimerParams()) );
}

TimerForm :: ~TimerForm (void)
{
    delete UI;
}

void TimerForm :: setTimerParams (void)
{
   m_timer = UI->sbInterval->value();
   m_units = UI->cbUnits->currentIndex();
   m_manually = UI->cbManually->isChecked();

   accept ();

}

void TimerForm :: init (int t, int u, bool m, bool enableCheckbox)
{
    m_timer = t;
    m_units = u;
    m_manually = m;

    UI->sbInterval->setValue(m_timer);
    UI->cbUnits->setCurrentIndex(m_units);
    UI->cbManually->setChecked(m_manually);

    UI->cbManually->setEnabled(enableCheckbox);
}

int TimerForm :: getTimerValue() const 
{ 
    return m_timer; 
}

int TimerForm :: getUnits() const 
{ 
    return m_units; 
}

int TimerForm :: getTimerMs() const //in milliseconds
{
    int val = 10;
    switch(m_units){
        case 0: val = m_timer * 1000; break;
        case 1: val = m_timer * 1000 * 60; break;
        case 2: val = m_timer * 1000 * 60 * 60; break;
    }

    return val;
}

bool TimerForm :: startManually()
{
    return m_manually;
}

