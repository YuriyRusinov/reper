//#define TEST_DYNDOC_MAC

#ifndef TEST_DYNDOC_MAC

#include <QApplication>

#include <JKKSLoader.h>

#include "UserInterface/dyndocForm/dyndocsyncform.h"
#include "UserInterface/DataStream/DataStream.h"
#include "kkscoreapplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KKSCoreApplication *sito = KKSCoreApplication::init (argc, argv, true);
    if(!sito)
        return 1;
    if(!sito->db())
        return 1;

    DyndocSyncForm* userForm = new DyndocSyncForm(sito->db());
    dyndocView* viewWidget = new dyndocView();
    dyndocModel* viewModel = new dyndocModel();

    dataStream* dataUnit = new dataStream(sito->db());
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

    idOrganization->connectToDb();
    int id = idOrganization->getLocalOrgId();      
    delete idOrganization;

    viewWidget->setModel(viewModel);

    userForm->setViewWidget(viewWidget);
    userForm->init();

    userForm->setDbInf(dbInfForLoader);
    userForm->setLocalId(id);
    userForm->reciveData(dataUnit->getData());
    userForm->exec();

    userForm->setViewWidget(0);

    delete dataUnit;
    delete viewModel;
    delete viewWidget;
    delete userForm;

    return 0;
}

#endif


