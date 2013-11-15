#ifndef _TIMER_FORM_H
#define _TIMER_FORM_H

#include <QDialog>
class QSettings;

namespace Ui
{
    class timer_form;
};

#include "config_httpconnector.h"


class __HTTP_CONNECTOR_EXPORT TimerForm : public QDialog
{
public:
    TimerForm (QWidget *parent=0, Qt::WFlags f=0);
    virtual ~TimerForm (void);
 
    void init (int t, int u, bool m, bool enableCheckBox);

    int getTimerValue() const;
    int getUnits() const ;
    int getTimerMs() const; //in milliseconds
    bool startManually();
    
private slots:
  void setTimerParams (void);

private:

    Ui::timer_form *UI;
    int m_timer;   
    int m_units;
    bool m_manually;
private:
    Q_OBJECT
};


#endif
