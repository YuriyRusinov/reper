#ifndef _TIMER_FORM_H
#define _TIMER_FORM_H

#include <QDialog>
class QSettings;

namespace Ui
{
    class timer_form;
};

class dds_TimerForm : public QDialog
{
public:
    dds_TimerForm (QWidget *parent=0, Qt::WFlags f=0);
    virtual ~dds_TimerForm (void);
 
    int getTimer( ){ return timer; }
    
private slots:
  void setTimer (void);

private:
    //
    // Functions
    //
    void init (void);
   
private:
    //
    // Variables
    //
    Ui::timer_form *UI;
    int timer;   
private:
    Q_OBJECT
};
#endif
