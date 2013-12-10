#include <map_window.h>
//
// QGIS Includes
//
#include <qgsapplication.h>
#include <qgsproviderregistry.h>

// Qt Includes
#include <QTranslator>
#include <QMessageBox>

int main(int argc, char ** argv) 
{
  // Start the Application
    QTextCodec::setCodecForTr(QTextCodec::codecForName("cp1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp1251"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp1251"));

    QgsApplication * app = new QgsApplication(argc, argv, true);

//    QTranslator translator;
//    translator.load("reper_ru.qm", ".");
//    if (translator.isEmpty())
//    {
//        QMessageBox::about(0, "empty", "");
//    }
//    app->installTranslator(&translator);

    QString myPluginsDir        = "C:\\Program Files (x86)\\DynamicSoft\\DynamicDocs Client 1.2.3\\QGS21\\plugins";
    //QString myPluginsDir        = "D:\\REP_EXT\\QGS21\\plugins";
    app->setPluginPath(myPluginsDir);
    
    app->initQgis();

    QString s = app->showSettings();
    qWarning() << s;

    MapWindow * mypMapWindow = new MapWindow();
    mypMapWindow->showMaximized();

    // Start the Application Event Loop
    int i = app->exec();
    //int i = a.exec();
    delete app;
    return i;
}
