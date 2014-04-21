#include "kksmainwindow.h"

/* qt */
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <Qt>
#include <QObject>
#include <QMainWindow>

/* ui */
#include "kkssito.h"
#include "kksclient_name.h"

#ifdef __USE_QGIS__ 
#include <QgsApplication.h>
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


    QString pluginsPath = QApplication::applicationDirPath() + QDir::separator() + "qtplugins";
    QCoreApplication::addLibraryPath( pluginsPath );

    KKSSito *sito = NULL;

    QMainWindow * mainWindow = NULL;

    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp1251"));

    if(argc == 2 && strcmp(argv[1], "admin") == 0){
        //sito = KKSSito::init (false);//, QString("admin"));
        sito = KKSSito::init (argc, argv, false, QString("admin"), true);
        mainWindow = new KKSMainWindow();
        mainWindow->setWindowTitle(QObject::tr("DynamicDocs ") + QObject::tr("Administrator") + " " + KKS_VERSION);
        //mainWindow->setWindowTitle(QObject::tr("PK IR ") + QObject::tr("Administrator") + " " + KKS_VERSION);
    }
    else{
        sito = KKSSito::init (argc, argv, false, QString(), true);
        mainWindow = new KKSMainWindow();
        mainWindow->setWindowTitle(QObject::tr("DynamicDocs ") + QObject::tr("Operator") + " " + KKS_VERSION);
        //mainWindow->setWindowTitle(QObject::tr("PK IR ") + QObject::tr("Operator") + " " + KKS_VERSION);
    }

    if(!sito){
        delete mainWindow;
        return 1;
    }

    mainWindow->show();

    int r = qApp->exec();

    delete mainWindow;
    delete app;
    return r;
}
