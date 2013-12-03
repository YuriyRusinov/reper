
//#include <QtGui>
//#include <QApplication>

#include <simple_map_window.h>
//
// QGIS Includes
//
#include <qgsapplication.h>
#include <qgsproviderregistry.h>


int main(int argc, char ** argv) 
{
  // Start the Application
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    QgsApplication * app = new QgsApplication(argc, argv, true);

    //QString myPluginsDir        = "C:\\Program Files (x86)\\Quantum GIS 1.7.0\\plugins";
	QString myPluginsDir        = "D:\\REP_EXT\\QGS17\\plugins";
    app->setPluginPath(myPluginsDir);
    app->initQgis();

    QString s = app->showSettings();
    qWarning() << s;

    MainWindow * mypMainWindow = new MainWindow();
    mypMainWindow->showMaximized();

    // Start the Application Event Loop
    int i = app->exec();
    //int i = a.exec();
    delete app;
    return i;
}
