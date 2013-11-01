#include <QIntValidator>


#include "ui_timer_form.h"

#include "timerform.h"

dds_TimerForm :: dds_TimerForm (QWidget *parent, Qt::WFlags f)
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

dds_TimerForm :: ~dds_TimerForm (void)
{
    delete UI;
}



void dds_TimerForm :: setTimer (void)
{
   timer = UI->sbTimer->value();
   accept ();

}

void dds_TimerForm :: init (void)
{
  timer = 36000;
}
