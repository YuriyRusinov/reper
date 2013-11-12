
#include <QtGui>
#include <QDockWidget>

#include "ddocinteractorwindow.h"
#include "ddocinteractorbase.h"
#include <kkssito.h>

DDocInteractorWindow::DDocInteractorWindow(QWidget *parent) : QDialog(parent)
{
    m_base = 0;

    statusLabel = new QLabel(tr("Waiting for data to sent..."));

    stopButton = new QPushButton(tr("Set manual mode querying database"));
    QSpacerItem * spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    manualStartButton = new QPushButton(tr("Query database"));
    manualStartButton->setDefault(true);

    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    QHBoxLayout * buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(stopButton);
    buttonLayout->addItem(spacer);
    buttonLayout->addWidget(manualStartButton);
    buttonLayout->addWidget(quitButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    QDockWidget * w = (QDockWidget*)kksSito->dbgWidget();
    mainLayout->addWidget(w->widget());
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("DynamicDocs Server Interactor"));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(closeApp()));
    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopListen()));
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

    if(stopButton->text() == tr("Set manual mode querying database")){
        stopButton->setText("Set automatic mode querying database");
        emit stopReadDatabase();
        manualStartButton->setEnabled(true);
    }
    else{
        stopButton->setText("Set manual mode querying database");
        emit startReadDatabase();
        manualStartButton->setEnabled(false);
    }


}

void DDocInteractorWindow::setInteractorBase(DDocInteractorBase *b)
{
    if(!b)
        return;
    
    m_base = b;

    connect(m_base, SIGNAL(sendingStarted()), this, SLOT(slotSendingStarted()));
    connect(m_base, SIGNAL(sendingCompleted()), this, SLOT(slotSendingCompleted()));
    connect(manualStartButton, SIGNAL(clicked()), m_base, SIGNAL(startSending()));
    connect(m_base, SIGNAL(showStatusText(QString)), this, SLOT(showStatusText(QString)));
    connect(this, SIGNAL(exitThreads()), m_base, SLOT(slotExitThreads()));
    connect(this, SIGNAL(stopReadDatabase()), m_base, SLOT(slotStopClient()));
    connect(this, SIGNAL(startReadDatabase()), m_base, SLOT(slotStartClient()));
    connect(m_base, SIGNAL(hideManualStartButton()), this, SLOT(slotHideManualStartButton()));
    connect(m_base, SIGNAL(showManualStartButton()), this, SLOT(slotShowManualStartButton()));

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

void DDocInteractorWindow::showStatusText(QString str)
{
    statusLabel->setText(str);
}

void DDocInteractorWindow::slotHideManualStartButton()
{
    manualStartButton->setEnabled(false);
}

void DDocInteractorWindow::slotShowManualStartButton()
{
    manualStartButton->setEnabled(true);
}
