//#define TEST_DYNDOC_MAC

#ifndef TEST_DYNDOC_MAC

#include <QApplication>

#include <JKKSLoader.h>

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

    dyndoc_mainStructs::dbInf dbInfForLoader = dataUnit->getDbInf();

    JKKSLoader* idOrganization = new JKKSLoader(dbInfForLoader.hostAddress,dbInfForLoader.dbName,dbInfForLoader.userName,dbInfForLoader.password,dbInfForLoader.port);
    if (idOrganization && idOrganization->connectToDb ())
    {
        ;//OK
    }
    else if (idOrganization)
    {
        delete idOrganization;
        qCritical() << QObject::tr("Unable to connect to the database: %1")
                              .arg(dbInfForLoader.hostAddress);
        QMessageBox::critical(0, QObject::tr("DynamicDocs Interactor"),
                              QObject::tr("Unable to connect to the database: %1")
                              .arg(dbInfForLoader.hostAddress));
        return 0;
    }

    int id = idOrganization->getLocalOrgId();
    delete idOrganization;

    QObject::connect(userForm,SIGNAL(signalStartSyncronization()),netUnit,SLOT(slotStartSyncronization()));
    QObject::connect(userForm,SIGNAL(signalStopSyncronization()),netUnit,SLOT(slotStopSyncronization()));

    viewWidget->setModel(viewModel);

    userForm->setViewWidget(viewWidget);
    userForm->init();

    userForm->setDbInf(dbInfForLoader);
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


