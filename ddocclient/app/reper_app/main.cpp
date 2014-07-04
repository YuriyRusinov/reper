#include "repermainwindow.h"

/* qt */
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <Qt>
#include <QObject>
#include <QMainWindow>
#include <QDir>
#include <QTextCodec>

/* ui */
#include "kkscommandlineopts.h"
#include "kksapplication.h"
#include "kksclient_name.h"

#ifdef __USE_QGIS__ 
#include <qgsapplication.h>
#endif

int main(int argc, char *argv[])
{
    
#ifdef __USE_QGIS__
    QApplication * app = new QgsApplication(argc, argv, true);

    //
    // Set up the QSettings environment must be done after qapp is created
    QCoreApplication::setOrganizationName( ORGANIZATION );
    //QCoreApplication::setOrganizationDomain( "qgis.org" );
    QCoreApplication::setApplicationName( PRODUCT_FULL_NAME );

    //QgsApplication * a = static_cast <QgsApplication *> (app);
    // ---- invoked in KKSSito::loadQGISPlugins ----
    //a->setPluginPath(pluginPath);
    //a->initQgis();
#else
    
    QApplication *app = new QApplication(argc, argv);

#endif

    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp1251"));


    KKSCommandLineOpts * options = KKSApplication::parseCommandLineOptions(argc, argv);
    if(options->showHelp){
        QString msg = options->getHelpMessage();
        fprintf(stdout, "%s", msg.toLocal8Bit().constData());
        KKSCoreApplication::showCommandLineParamsHelp();
        delete options;
        delete app;
        return 0;
    }

    delete options;


    QString pluginsPath = QApplication::applicationDirPath() + QDir::separator() + "qtplugins";
    QCoreApplication::addLibraryPath( pluginsPath );

    KKSApplication *kksApplication = NULL;

    QMainWindow * mainWindow = NULL;


    kksApplication = KKSApplication::init (argc, argv, false);
    if(!kksApplication){
        return 1;
    }

    const KKSCommandLineOpts * opts = kksApplication->commandLineOpts();

    mainWindow = new ReperMainWindow();

    if(!opts->user.isEmpty() && opts->user == QString("admin")){
        mainWindow->setWindowTitle(QObject::tr("Reper ") + QObject::tr("Administrator") + " " + KKS_VERSION);
    }
    else{
        mainWindow->setWindowTitle(QObject::tr("Reper ") + QObject::tr("Operator") + " " + KKS_VERSION);
    }

    mainWindow->show();

    int r = qApp->exec();

    delete mainWindow;
    delete app;
    return r;

}
