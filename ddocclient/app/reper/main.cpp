
//#include <QtGui>
//#include <QApplication>

#include <simple_map_window.h>
//
// QGIS Includes
//
#include <qgsapplication.h>
#include <qgslegendmodel.h>
#include <qgsproviderregistry.h>


int main(int argc, char ** argv) 
{
  // Start the Application
  QgsApplication * app = new QgsApplication(argc, argv, true);

  QString myPluginsDir        = "C:\\Program Files (x86)\\Quantum GIS 1.7.0\\plugins";
  app->setPluginPath(myPluginsDir);
  app->initQgis();

  QString s = app->showSettings();
  qWarning() << s;

  MainWindow * mypMainWindow = new MainWindow();
  mypMainWindow->show();
  
  // Start the Application Event Loop
  int i = app->exec();
  //int i = a.exec();
  delete app;
  return i;
}

/*
int main (int argc, char ** argv)
{
    QgsApplication app(argc, argv, true);

    QString myPluginsDir = "C:\\OSGeo4W\\apps\\qgis-dev\\plugins";
    QString myLayerPath = "./data";
    QString myLayerBaseName = "test";
    QString myProviderName = "ogr";

    QgsProviderRegistry::instance(myPluginsDir);

    QgsVectorLayer * mypLayer = new QgsVectorLayer(myLayerPath, myLayerBaseName, myProviderName);


    QgsSymbolV2 * s = QgsSymbolV2::defaultSymbol(mypLayer->geometryType());
    QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(s);
    QList <QgsMapCanvasLayer> myLayerSet;
    mypLayer->setRendererV2(mypRenderer);
    if (mypLayer->isValid())
    {
        qDebug("Layer is valid");
    }
    else
    {
        qDebug("Layer is NOT valid");
    }
    
    // Add the Vector Layer to the Layer Registry
    QgsMapLayerRegistry::instance()->addMapLayer(mypLayer, TRUE);
    // Add the Layer to the Layer Set
    myLayerSet.append(QgsMapCanvasLayer(mypLayer, TRUE));

    // Create the Map Canvas
    QgsMapCanvas * mypMapCanvas = new QgsMapCanvas(0, 0);
    mypMapCanvas->setExtent(mypLayer->extent());
    mypMapCanvas->enableAntiAliasing(true);
    mypMapCanvas->setCanvasColor(QColor(255, 255, 255));
    mypMapCanvas->freeze(false);

    // Set the Map Canvas Layer Set
    mypMapCanvas->setLayerSet(myLayerSet);
    mypMapCanvas->setVisible(true);
    mypMapCanvas->refresh();

    return app.exec();
}
*/
