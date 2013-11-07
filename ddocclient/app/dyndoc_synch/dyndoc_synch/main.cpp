//#define TEST_DYNDOC_MAC

#ifndef TEST_DYNDOC_MAC

#include <QApplication>

#include "UserInterface/dyndocForm/dyndocForm.h"
#include "UserInterface/DataStream/DataStream.h"
#include "UserInterface/NetworkStream/NetworkStream.h"
#include "kkssito.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KKSSito *sito = KKSSito::init (true);
    if(!sito)
        return 1;
    if(!sito->db())
        return 1;

    dyndoc_synch_form* userForm = new dyndoc_synch_form();
    dyndocView* viewWidget = new dyndocView();
    dyndocModel* viewModel = new dyndocModel();

    dataStream* dataUnit = new dataStream(sito->db());
    networkStream* netUnit = new networkStream();

    QObject::connect(userForm,SIGNAL(signal_startSyncronization(dyndoc_netThread::synchList*)),netUnit,SLOT(slotStartSyncronization(dyndoc_netThread::synchList*)));
    QObject::connect(userForm,SIGNAL(signal_stopSyncronization()),netUnit,SLOT(slotStopSyncronization()));

    viewWidget->setModel(viewModel);

    userForm->setViewWidget(viewWidget);
    userForm->init();

    userForm->setDbInf(dataUnit->getDbInf());
    userForm->reciveData(dataUnit->getData());
    userForm->exec();

    userForm->setViewWidget(0);

    delete dataUnit;
    delete netUnit;
    delete viewModel;
    delete viewWidget;
    delete userForm;

    return 0;
}

#endif


