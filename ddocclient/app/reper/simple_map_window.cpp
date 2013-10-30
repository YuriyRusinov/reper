/***************************************************************************
 *   Copyright (C) 2006 by Tim Sutton   *
 *   tim@linfiniti.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "simple_map_window.h"
//#include <gdal.h>
//#include <gdal_priv.h>
#include <qgsproviderregistry.h>

MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
{
    //required by Qt4 to initialise the ui
    setupUi(this);
    this->setWindowIcon(QIcon(":/ico/main64x64.png"));
    needPaint = false;
//    this->setCentralWidget(frameMap);
//    mpLeftDock = new QDockWidget("Content", this);
//    mpBottomDock = new QDockWidget("Jornal", this);
//    this->addDockWidget(Qt::LeftDockWidgetArea, mpLeftDock, Qt::Horizontal);
//    this->addDockWidget(Qt::BottomDockWidgetArea, mpBottomDock, Qt::Horizontal);
//    mpLeftDock->setWidget(mpMapLegend);
    QString myPluginsDir        = "C:\\Program Files (x86)\\Quantum GIS 1.7.0\\plugins";
    QgsProviderRegistry::instance(myPluginsDir);

    // Get the registry singleton
    mpRegistry = QgsMapLayerRegistry::instance();

    // Create the Map Canvas
    mpMapCanvas= new QgsMapCanvas(0, 0);
    mpMapCanvas->enableAntiAliasing(true);
    mpMapCanvas->useImageToRender(false);
    mpMapCanvas->setCanvasColor(QColor(255, 255, 255));
    mpMapCanvas->freeze(false);
    mpMapCanvas->setVisible(true);
    mpMapCanvas->refresh();
    mpMapCanvas->show();

    this->azSetTitleWindow(*this);

    //Creating map TOC
    //    QgsLegend * mpMapLegend = new QgsLegend(mpMapCanvas, 0, 0);

    mpMapLayout = new QVBoxLayout(frameMap); //main Layout
    mpMapLayout->addWidget(mpMapCanvas); // adding MapCanvas in Layout
    mpLegendLayout = new QVBoxLayout(frameLegend); //new Layout2

//    mpLegendLayout->addWidget(mpMapLegend); //adding TOC to Layout2

    //create actions
    mpVectorize = new QAction(QIcon(":/ico/vectorize.png"), tr("Vectorize"), this);
    mpActionAddVectorLayer = new QAction(QIcon(":/ico/add_vector.png"), tr("Add &Vector Layer"), this);
    mpActionAddRasterLayer = new QAction(QIcon(":/ico/add_raster.png"), tr("Add &Raster Layer"), this);
    mpActionAddVectorLayer->setStatusTip(tr("Add a vector layer to the map window"));
    mpActionAddRasterLayer->setStatusTip(tr("Add a raster layer to the map window"));
    mpContextShowExtent = new QAction("Show extent", tableLegend);
    mpContextRemoveLayer = new QAction("Remove", tableLegend);


    //settings of table
    tableLegend->setHorizontalHeaderLabels(QStringList() << tr("V") << tr("Layer's name") << tr("Style") << ("Path"));
    tableLegend->setColumnCount(tableLegend->horizontalHeader()->count());
    tableLegend->setColumnWidth(0, 25);
    tableLegend->setColumnWidth(1, 185);
    tableLegend->setColumnWidth(2, 50);
    tableLegend->setContextMenuPolicy(Qt::CustomContextMenu);
    mpContextLegendMenu = new QMenu(tableLegend);
    mpContextLegendMenu->addAction(mpContextShowExtent);
    mpContextLegendMenu->addAction(mpContextRemoveLayer);
//    tableLegend->addAction(mpContextShowExtent);
//    tableLegend->addAction(mpContextRemoveLayer);


    //create the action behaviours
     connect(this->mpActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
     connect(this->mpActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
     connect(this->mpActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));
     connect(this->mpActionAddVectorLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddVectorLayer()));
     connect(this->mpActionAddRasterLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddRasterLayer()));
     connect(this->mpActionFileExit, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileExit()));
     connect(this->mpVectorize, SIGNAL(triggered()), this, SLOT(SLOTmpActionVectorize()));
     connect(this->mpActionFileOpenProject, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileOpenProject()));
     connect(mpRegistry, SIGNAL(layerWasAdded(QgsMapLayer *)), this, SLOT(addLayerToTOC(QgsMapLayer *)));
     connect(tableLegend, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SLOTazShowContextMenuForLegend(const QPoint &)));
//     connect(QgsProject::instance(), SIGNAL(readProject(QDomDocument)), mpMapCanvas, SLOT(readProject(QDomDocument)));

    //create a little toolbar
    mpMapToolBar = addToolBar(tr("File"));
    mpMapToolBar->addAction(mpActionAddLayer);
    mpMapToolBar->addAction(mpActionZoomIn);
    mpMapToolBar->addAction(mpActionZoomOut);
    mpMapToolBar->addAction(mpActionPan);
    mpMapToolBar->addAction(mpActionAddVectorLayer);
    mpMapToolBar->addAction(mpActionAddRasterLayer);
    mpMapToolBar->addAction(mpVectorize);


    //create the maptools
    mpPanTool = new QgsMapToolPan(mpMapCanvas);
    mpPanTool->setAction(mpActionPan);
    mpZoomInTool = new QgsMapToolZoom(mpMapCanvas, FALSE); // false = in
    mpZoomInTool->setAction(mpActionZoomIn);
    mpZoomOutTool = new QgsMapToolZoom(mpMapCanvas, TRUE ); //true = out
    mpZoomOutTool->setAction(mpActionZoomOut);

    statusBar()->showMessage("Reper is ready.");
}

MainWindow::~MainWindow()
{
  delete mpZoomInTool;
  delete mpZoomOutTool;
  delete mpPanTool;
  delete mpMapToolBar;
  delete mpMapCanvas;
  delete mpMapLayout;
  delete mpLegendLayout;
  // deleteAll
}


void MainWindow::addLayerToTOC(QgsMapLayer* mapLayer)
{
//    mpLayerSet
//    mpRegistry
    int nRow(0);
    int nCol(0);
    nRow = tableLegend->rowCount();
    nCol = tableLegend->columnCount();
    tableLegend->insertRow(nRow);
    QTableWidgetItem *pItem = new QTableWidgetItem(mapLayer->name());
    tableLegend->setItem(nRow, 1, pItem);
    pItem = new QTableWidgetItem(mapLayer->id());
    tableLegend->setItem(nRow, 2, pItem);
    pItem = new QTableWidgetItem();
    //QVariant *ok;
    Qt::CheckState pState;
    if (mapLayer->hasScaleBasedVisibility())
    {
        pState = Qt::Checked;
    }
    else
    {
        pState = Qt::Unchecked;
    }

    pItem->setData(Qt::CheckStateRole, pState);
    tableLegend->setItem(nRow, 0, pItem);
    //mpMapLegend->addLayer(mapLayer);
//    QStandardItemModel l1;

}

void MainWindow::azRemoveAnnotationItems()
{
    if ( !mpMapCanvas )
    {
      return;
    }
    QGraphicsScene* scene = mpMapCanvas->scene();
    if ( !scene )
    {
      return;
    }
//    QList<QgsAnnotationItem*> itemList = annotationItems();
//    QList<QgsAnnotationItem*>::iterator itemIt = itemList.begin();
//    for ( ; itemIt != itemList.end(); ++itemIt )
//    {
//      if ( *itemIt )
//      {
//        scene->removeItem( *itemIt );
//        delete *itemIt;
//      }
//    }
}

void MainWindow::azRemoveAllLayers()
{
    QgsMapLayerRegistry::instance()->removeAllMapLayers();
}

void MainWindow::azSetTitleWindow(QWidget & azApp)
{
      QString caption = "Reper";

      if ( QgsProject::instance()->title().isEmpty() )
      {
        if ( QgsProject::instance()->fileName().isEmpty() )
        {
          // no project title nor file name, so just leave caption with
          // application name and version
        }
        else
        {
          QFileInfo projectFileInfo( QgsProject::instance()->fileName() );
          caption += " - " + projectFileInfo.completeBaseName();
        }
      }
      else
      {
        caption += " - " + QgsProject::instance()->title();
      }

      azApp.setWindowTitle( caption );
}

void MainWindow::panMode()
{
  mpMapCanvas->setMapTool(mpPanTool);
}
void MainWindow::zoomInMode()
{
  mpMapCanvas->setMapTool(mpZoomInTool);
}
void MainWindow::zoomOutMode()
{
  mpMapCanvas->setMapTool(mpZoomOutTool);
}

void MainWindow::azLoadLayer(QgsMapLayer *theMapLayer, bool isExtent)
{
    QgsMapLayerRegistry::instance()->addMapLayer(theMapLayer, TRUE);
    //create a layerset
    QList<QgsMapCanvasLayer> myList;
    QString l;

    for (int i = 0; i < mpMapCanvas->layers().size(); i++)
    {
        myList.append(QgsMapCanvasLayer(mpMapCanvas->layers().at(i)));
    }

    // Add the layers to the Layer Set
    myList.append(QgsMapCanvasLayer(theMapLayer, TRUE));//bool visibility
//    l.setNum(myList.size());
//    QMessageBox::information(this,"a", l);

    // set the canvas to the extent of our layer if it need
    if (isExtent)
    {
        mpMapCanvas->setExtent(theMapLayer->extent());
    }
    // Set the Map Canvas Layer Set
    mpMapCanvas->setLayerSet(myList);
}


void MainWindow::SLOTmpActionFileExit() // Exit from Application
{
    bool ProjectChange(false); // changing of Project NEEDED TO CONNECT
    QString pMessageText("");
    uint pButtonsPack(0);
    int pButton(0);
    ProjectChange = true;
    if (ProjectChange)
    {
        pButtonsPack = 0x00410800;
        pMessageText = "The Document has changed,"
                "\n Do you want to save changes before close application?";
    }
    else
    {
        pButtonsPack = 0x00404000;
        pMessageText = "Do you want to close application?";
    }

    QMessageBox * pExitApp = new QMessageBox (QMessageBox::Question,
                                              "Exit from Application",
                                        pMessageText,
                           (QMessageBox::StandardButton)pButtonsPack);
    pButton = pExitApp->exec();
    delete pExitApp;
    if (pButton == QMessageBox::Yes || pButton == QMessageBox::No)
    {
        this->close();
    }
    else if (pButton == QMessageBox::Save)
    {
        // save NEEDED TO CONNECT
        this->close();
    }
}

void MainWindow::SLOTmpActionFileOpenProject() // Open QGIS Project in MapDock
{
    QString projectFileName = QFileDialog::getOpenFileName(this, "Open map project *.qgs", "", "*.qgs");
    if (projectFileName.isNull())
    {
        return;
    }
    //    deletePrintComposers();
    this->azRemoveAnnotationItems();
    this->azRemoveAllLayers();
    mpLayerSet.clear();

    QgsProject::instance()->setFileName(projectFileName);
    if ( !QgsProject::instance()->read() )
    {
        QApplication::restoreOverrideCursor();
        this->statusBar()->showMessage("Failed to open project!"
                                       "\n " + projectFileName + "'");
        qDebug("Failed to open project!" + projectFileName.toAscii());
        mpMapCanvas->freeze(false);
        mpMapCanvas->refresh();
        return;
    }
    this->azSetTitleWindow( *this );
    //    mpMapCanvas->setCanvasColor( myColor );
    // k // QgsMapLayer *layer;
    QMapIterator < QString, QgsMapLayer * > i(QgsMapLayerRegistry::instance()->mapLayers());
    i.toBack();
    while (i.hasPrevious())
    {
        i.previous();
//        QMessageBox::about(this, "hell", i.value()->name());
        if (i.value()->type() == QgsMapLayer::VectorLayer)
                {
                    if (i.value()->isValid())
                    {
                        mpRegistry->addMapLayer(i.value(), TRUE);
                        mpLayerSet.append(QgsMapCanvasLayer(i.value()));
                    }

                }
        else if (i.value()->type() == QgsMapLayer::RasterLayer)
        {
            //add to raster file
        }

    }
    // TODO add last Open Project
    QApplication::restoreOverrideCursor();
    mpMapCanvas->setLayerSet(mpLayerSet);
    mpMapCanvas->freeze(false);
    mpMapCanvas->refresh();
}

void MainWindow::SLOTsetRenderer()
{
//    QgsMapLayer* layer = mpMapCanvas->layer(0);
//    Q_ASSERT(layer);
//    Q_ASSERT(layer->type() == QgsMapLayer::VectorLayer);
//    QgsVectorLayer* vlayer = static_cast<QgsVectorLayer*>(layer);
//    QgsRendererV2PropertiesDialog dlg(vlayer, QgsStyleV2::defaultStyle());
//    dlg.exec();
    mpMapCanvas->refresh();
}


void MainWindow::SLOTmpActionAddVectorLayer()
{
    QString fullLayerName = QFileDialog::getOpenFileName(this, "Add layer", "", "All supported (*.shp *.tif *tiff);;Shapefiles (*.shp);;Geotiff (*.tif *.tiff)");
    QFileInfo azFileInfo(fullLayerName);
    if (!azFileInfo.isFile())
    {
        return;
    }
    QString myProviderName      = "ogr";
    QgsVectorLayer * mypLayer = new QgsVectorLayer(azFileInfo.filePath(), azFileInfo.completeBaseName(), myProviderName);
    QgsSymbolV2 * s = QgsSymbolV2::defaultSymbol(mypLayer->geometryType());
    QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(s);//(mypLayer->geometryType());
    //        QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->geometryType());
    mypLayer->setRendererV2(mypRenderer);
    if (!mypLayer->isValid())
    {
        qDebug("Shp layer is not valid. Layer was not added to map." + fullLayerName.toAscii());
        this->statusBar()->showMessage("Layer is not valid. Layer was not added to map."
                                       "\n " + fullLayerName + "'");
        return;
    }
    // Add the Vector Layer to the Layer Registry
    mpRegistry->addMapLayer(mypLayer, TRUE);
    // Add the Layer to the Layer Set
    mpLayerSet.append(QgsMapCanvasLayer(mypLayer));
    // set the canvas to the extent of our layer
    mpMapCanvas->setExtent(mypLayer->extent());
    // Set the Map Canvas Layer Set
    mpMapCanvas->setLayerSet(mpLayerSet);
    mpMapCanvas->refresh();
}

/*void MainWindow::paintEvent(QPaintEvent *pPaint)
{
    if (!needPaint)
    {
        return;
    }
    QPainter pPainter(this);
    QgsRasterViewPort pRastPort;
    pRastPort.bottomRightPoint.setX(30);
    pRastPort.bottomRightPoint.setY(30);
    pRastPort.topLeftPoint.setX(200);
    pRastPort.topLeftPoint.setY(200);
    testLayer->draw(pPainter, pRastPort);
}*/

void MainWindow::SLOTmpActionAddRasterLayer()
{
    QString fullLayerName = QFileDialog::getOpenFileName(this, "Add raster layer", "",
                         "Raster files (*.img *.asc *.tif *tiff *.bmp *.jpg *.jpeg);;Geotiff (*.tif *.tiff)");
    QFileInfo azFileInfo(fullLayerName);
    if (!azFileInfo.isFile())
    {
        return;
    }

//     it's work
    QString errMsg;
    if (QgsRasterLayer::isValidRasterFileName(fullLayerName, errMsg ))
    {
        this->statusBar()->showMessage("All ok");
        //        return;
    }
    else
    {
         this->statusBar()->showMessage("Bad thing :(");
    }

    /*

 GDALRasterBand  *poBand;
 GDALAllRegister();
 GDALDataset *GdalData = NULL;
 GdalData=(GDALDataset *) GDALOpen(fullLayerName.toAscii(),GA_ReadOnly);
 int W=GdalData->GetRasterXSize();
 int H=GdalData->GetRasterYSize();
 int Ch=GdalData->GetRasterCount();
 QString prj = QString(GdalData->GetProjectionRef());
 //GdalData->getGe
*/
	QgsRasterLayer::registerGdalDrivers();


     QgsProviderRegistry * pReg = QgsProviderRegistry::instance();
	 QString myLibPath = pReg->library("gdal");
    QgsRasterLayer * mypLayer = new QgsRasterLayer(azFileInfo.filePath(), azFileInfo.completeBaseName());
    QString a;
    QString t;
    a = "Numbers Of Bands: " + t.setNum(mypLayer->bandCount()) + "\n";
    a = a + "Wide x Height: " + t.setNum(mypLayer->width());
    a = a + " x " + t.setNum(mypLayer->height());
	int epsgId = mypLayer->crs().epsg();
	QString wkt = mypLayer->crs().toWkt();
	
    //QgsCoordinateReferenceSystem crs;
	//crs.createFromWkt(prj);
	//mypLayer->setCrs(crs);
    
    
    //-- QMessageBox::about(this, "info", a);
	
    //QString wkt = mypLayer->projectionWkt();

//  needPaint = true;
//  mypLayer->draw(QPainter *pPainter, QgsRasterViewPort *myRasterViewPort);
    if (!mypLayer->isValid())
    {
        qDebug("Raster layer is not valid or not supported by GDAL. Layer was not added to map." + fullLayerName.toAscii());
        this->statusBar()->showMessage("Raster layer is not valid or not supported by GDAL. Layer was not added to map."
                                       "\n " + fullLayerName + "'");
        return;
    }
    
    QgsRasterLayer::LayerType lType = mypLayer->rasterType();
    if(lType == QgsRasterLayer::Multiband){
        //mypLayer->setDrawingStyle( QgsRasterLayer::MultiBandSingleBandPseudoColor ); 
        mypLayer->setDrawingStyle( QgsRasterLayer::MultiBandColor);
        mypLayer->setColorShadingAlgorithm( QgsRasterLayer::ColorRampShader );
    }
    else if(lType == QgsRasterLayer::Palette){
        mypLayer->setDrawingStyle(QgsRasterLayer::PalettedColor);
        mypLayer->setColorShadingAlgorithm( QgsRasterLayer::ColorRampShader );
    }
    else if(lType == QgsRasterLayer::GrayOrUndefined){
        //mypLayer->setDrawingStyle(QgsRasterLayer::SingleBandPseudoColor);
        mypLayer->setDrawingStyle(QgsRasterLayer::SingleBandGray);
        mypLayer->setColorShadingAlgorithm( QgsRasterLayer::PseudoColorShader );
    }
    else{
        mypLayer->setDrawingStyle(QgsRasterLayer::SingleBandColorDataStyle);
        mypLayer->setColorShadingAlgorithm( QgsRasterLayer::ColorRampShader );
    }
    
    
    
    mypLayer->setContrastEnhancementAlgorithm(QgsContrastEnhancement::StretchToMinimumMaximum, false );
    
    
    //connect(mypLayer, SIGNAL(repaintRequested()), mpMapCanvas, SLOT(refresh()) );
//    mypLayer->setMinimumValue( mypLayer->grayBandName(), 0.0, false );
//    mypLayer->setMaximumValue( mypLayer->grayBandName(), 10.0 );
    
    
    // Add the Vector Layer to the Layer Registry
    mpRegistry->addMapLayer(mypLayer, TRUE);
    
    
    
    mpMapCanvas->setExtent(mypLayer->extent());
    
    // Add the Layer to the Layer Set
    mpLayerSet.append(QgsMapCanvasLayer(mypLayer));
    mpMapCanvas->setLayerSet(mpLayerSet);
    
    
    mpMapCanvas->freeze(false);
    mpMapCanvas->refresh();
}

void MainWindow::SLOTmpActionVectorize()
{
//    QString msg, t1;
//    msg = "mpRegestry: " + t1.number(mpRegistry->count()) + "\n";
//    msg = msg + "mpLayerSet: " + t1.number(mpLayerSet.count()) + "\n";
//    QMessageBox::about(this, "Test", msg);
    QString pFile;
    QGis::WkbType pGeom;
    pGeom = QGis::WKBPolygon;
    QgsCoordinateReferenceSystem pSRS;
    pSRS.createFromOgcWmsCrs( GEO_EPSG_CRS_AUTHID );
    pSRS.validate();
    QgsProviderRegistry * pReg = QgsProviderRegistry::instance();
    QString ogrlib = pReg->library( "ogr" );



    pFile = "shapeTestAzura";
//    QgsVectorLayer *pLayer = new QgsVectorLayer( src, pFile, "ogr" );

//    pFile =

//    QgsVectorFileWriter::WriterError error;
//    error = QgsVectorFileWriter.writeAsShapefile(pLayer, "newShape19");
//    error = QgsVectorFileWriter.writeAsVectorFormat(pLayer, "my_shapes.shp", \
//      "CP1250", None, "ESRI Shapefile")

//    if error == QgsVectorFileWriter.NoError:
//      print "success!"

}

void MainWindow::SLOTazShowContextMenuForLegend(const QPoint & pos)
{
    mpContextLegendMenu->exec(tableLegend->viewport()->mapToGlobal(pos));
//    QgsMapLayer.;


}

