
#include <QtGui>
#include <QDockWidget>

#include "ddocinteractorwindow.h"
#include "ddocinteractorbase.h"
#include "timerform.h"
#include <kksdebug.h>
#include <kkssito.h>

DDocInteractorWindow::DDocInteractorWindow(QWidget *parent) : QDialog(parent)
{
    m_base = 0;

    stopButton = new QPushButton(tr("Stop querying"));
    
    manualStartButton = new QPushButton(tr("Single query database"));
    
    QSpacerItem * spacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Expanding);

    QPushButton * pbParams = new QPushButton(tr("Timer params"));
    QPushButton * pbConnInfo = new QPushButton(tr("Connection info"));
    
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    QVBoxLayout * buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(manualStartButton);

    buttonLayout->addItem(spacer);

    buttonLayout->addWidget(pbParams);
    buttonLayout->addWidget(pbConnInfo);
    buttonLayout->addWidget(quitButton);

    buttonLayout->setSizeConstraint(QLayout::SetFixedSize);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QDockWidget * w = (QDockWidget*)kksSito->dbgWidget(true, false);
    mainLayout->addWidget(w->widget());
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("DynamicDocs Server Interactor"));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(closeApp()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopListen()));
    connect(pbParams, SIGNAL(clicked()), this, SLOT(slotSetTimerParams()));
    connect(pbConnInfo, SIGNAL(clicked()), this, SLOT(slotShowConnInfo()));

    m_timerInterval = 10; //10 000 milliseconds
    m_timerUnits = 0; //seconds
    m_startManually = false;
}

void DDocInteractorWindow::setTimerParams(int v, int u, bool m)
{
    m_timerInterval = v; //10 000 milliseconds
    m_timerUnits = u; //seconds
    m_startManually = m;

    if(m_startManually){
        stopButton->setText(tr("Start querying"));
        manualStartButton->setEnabled(true);
    }
    else{
        stopButton->setText(tr("Stop querying"));
        manualStartButton->setEnabled(false);
    }

    QString sUnits;
    switch(m_timerUnits){
        case 0: sUnits = tr("second(s)"); break;
        case 1: sUnits = tr("minute(s)"); break;
        case 2: sUnits = tr("hour(s)"); break;
    }
    kksInfo() << tr("Set timer interval to %1 %2").arg(m_timerInterval).arg(sUnits);
    
    if(m_startManually) 
        kksInfo() << tr("Timer is disabled now. Use <Single query database> button to manually querying database or <Start querying> button to start timer");
    else 
        kksInfo() << tr("Timer is enabled now. Use <Stop querying> button to stop timer");
}


void DDocInteractorWindow::closeApp()
{
    emit exitThreads();

    close();
}

void DDocInteractorWindow::stopListen()
{
    if(!m_base)
        return;

    if(stopButton->text() == tr("Stop querying")){
        stopButton->setText(tr("Start querying"));
        m_startManually = true;
        emit stopReadDatabase();
        manualStartButton->setEnabled(true);
    }
    else{
        stopButton->setText(tr("Stop querying"));
        m_startManually = false;
        emit startReadDatabase();
        manualStartButton->setEnabled(false);
    }

    if(m_startManually)
        kksInfo() << tr("Timer is disabled now. Use <Single query database> button to manually querying database or <Start querying> button to start timer");
    else
        kksInfo() << tr("Timer is enabled now. Use <Stop querying> button to stop timer");

}

void DDocInteractorWindow::setInteractorBase(DDocInteractorBase *b)
{
    if(!b)
        return;
    
    m_base = b;

    connect(m_base, SIGNAL(sendingStarted()), this, SLOT(slotSendingStarted()));
    connect(m_base, SIGNAL(sendingCompleted()), this, SLOT(slotSendingCompleted()));
    connect(manualStartButton, SIGNAL(clicked()), m_base, SIGNAL(startSending()));
    connect(this, SIGNAL(exitThreads()), m_base, SLOT(slotExitThreads()));
    connect(this, SIGNAL(stopReadDatabase()), m_base, SLOT(slotStopClient()));
    connect(this, SIGNAL(startReadDatabase()), m_base, SLOT(slotStartClient()));
    connect(m_base, SIGNAL(hideManualStartButton()), this, SLOT(slotHideManualStartButton()));
    connect(m_base, SIGNAL(showManualStartButton()), this, SLOT(slotShowManualStartButton()));
    connect(this, SIGNAL(refreshTimer(int)), m_base, SIGNAL(refreshTimer(int)));
}

void DDocInteractorWindow::closeEvent(QCloseEvent *event)
{
    if(m_base)
        delete m_base;

    event->accept();
}

void DDocInteractorWindow::slotSendingStarted()
{
    manualStartButton->setEnabled(false);
}

void DDocInteractorWindow::slotSendingCompleted()
{
    manualStartButton->setEnabled(true);
}

void DDocInteractorWindow::slotHideManualStartButton()
{
    manualStartButton->setEnabled(false);
}

void DDocInteractorWindow::slotShowManualStartButton()
{
    manualStartButton->setEnabled(true);
}

void DDocInteractorWindow::slotSetTimerParams()
{
    TimerForm * tf = new TimerForm(this);
    
    tf->init(m_timerInterval, m_timerUnits, m_startManually, false);
    
    if(tf->exec() != QDialog::Accepted){
        delete tf;
        return;
    }
    
    int intervalMs = tf->getTimerMs();
    m_timerInterval = tf->getTimerValue();
    m_timerUnits = tf->getUnits();
    //m_startManually = tf->startManually();

    emit refreshTimer(intervalMs);

    QString sUnits;
    switch(m_timerUnits){
        case 0: sUnits = tr("second(s)"); break;
        case 1: sUnits = tr("minute(s)"); break;
        case 2: sUnits = tr("hour(s)"); break;
    }
    kksInfo() << tr("Set timer interval to %1 %2").arg(m_timerInterval).arg(sUnits);

    delete tf;
}

void DDocInteractorWindow::slotShowConnInfo()
{
    kksSito->showConnectionInfo(m_base->m_loader->getDbRead(), m_base->m_loader->getUserName(), m_base->m_loader->getDlName(), tr("Not used"), this);
}
