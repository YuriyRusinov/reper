#include <QIntValidator>


#include "ui_timer_form.h"
#include "timerform.h"

TimerForm :: TimerForm (QWidget *parent, Qt::WFlags f)
    : QDialog (parent, f),
    UI (new Ui::timer_form)
{
    UI->setupUi (this);


    //QIntValidator *pDbVal = new QIntValidator (0, 100000, this);
    //UI->sbTimer->setValidator (pDbVal);
  

    this->init ();

    connect (UI->pbMan, SIGNAL (clicked()), this, SLOT (reject()) );
    connect (UI->pbYes, SIGNAL (clicked()), this, SLOT (setTimer()) );
}

TimerForm :: ~TimerForm (void)
{
    delete UI;
}



void TimerForm :: setTimer (void)
{
   timer = UI->sbTimer->value();
   accept ();

}

void TimerForm :: init (void)
{
  timer = 36000;
}
