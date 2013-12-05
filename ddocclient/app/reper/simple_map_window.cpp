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


MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
    : QMainWindow(parent,fl)
{
    //required by Qt4 to initialise the ui

    //init's
    setupUi(this);
    this->setWindowIcon(QIcon(":/ico/main64x64.png"));
    mpAppPath = "D:/!Share/src/reper/ddocclient/app/reper/";
	mpSelectedLayer = NULL;
    dnThemTaskSpecBath = NULL;


    // setup StatusBar
    {
    QFont pFont("Courier", 9);
    statusBar()->setFont(pFont);
    mpCoordsLabel = new QLabel(QString(), this->statusBar());
    mpCoordsLabel->setObjectName("mpCoordsLabel");
    mpCoordsLabel->setMinimumWidth(10);
    mpCoordsLabel->setMaximumHeight(20);
    mpCoordsLabel->setFrameStyle(QFrame::NoFrame);
    mpCoordsLabel->setAlignment(Qt::AlignCenter);
    mpCoordsLabel->setText(tr("Координаты:"));
    mpCoordsLabel->setToolTip(tr("Текущие координаты карты") );
    statusBar()->addPermanentWidget(mpCoordsLabel, 0);

    mpCoordsEdit = new QLineEdit( QString(), statusBar() );
    mpCoordsEdit->setObjectName( "mpCoordsEdit" );
    mpCoordsEdit->setFont(pFont);
    mpCoordsEdit->setMinimumWidth(10);
    mpCoordsEdit->setMaximumWidth(300);
    mpCoordsEdit->setMaximumHeight(20);
    mpCoordsEdit->setContentsMargins( 0, 0, 0, 0 );
    mpCoordsEdit->setAlignment( Qt::AlignCenter );
    QRegExp coordValidator( "[+-]?\\d+\\.?\\d*\\s*,\\s*[+-]?\\d+\\.?\\d*" );
    mpCoordsEditValidator = new QRegExpValidator( coordValidator, mpCoordsEdit );
    mpCoordsEdit->setWhatsThis( tr( "Показывает координаты карты "
                                   "в позиции курсора." ) );
    mpCoordsEdit->setToolTip( tr( "Текущие координаты карты (в формате x, y)" ) );
    statusBar()->addPermanentWidget( mpCoordsEdit, 0 );


    }

    {
        // use dock wigets
//    this->setCentralWidget(frameMap);
//    mpLeftDock = new QDockWidget("Content", this);
//    mpBottomDock = new QDockWidget("Jornal", this);
//    this->addDockWidget(Qt::LeftDockWidgetArea, mpLeftDock, Qt::Horizontal);
//    this->addDockWidget(Qt::BottomDockWidgetArea, mpBottomDock, Qt::Horizontal);
//    mpLeftDock->setWidget(mpMapLegend);
    }
    //QString myPluginsDir        = "C:\\Program Files (x86)\\Quantum GIS 1.7.0\\plugins";
	QString myPluginsDir        = "D:\\REP_EXT\\QGS17\\plugins";
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

    // добавляем фрейм и вставляем в него виджет "Окно карты"(Map Canvas)
    mpMapLayout = new QVBoxLayout(frameMap); //main Layout
    mpMapLayout->addWidget(mpMapCanvas); // adding MapCanvas in Layout


    //settings of TOC-treeWidget
//    mpTOC = new AzTOC(mpMapCanvas, this);
//    mpLegendLayout = new QVBoxLayout(frameLegend); //new Layout2
//    mpLegendLayout->addWidget(mpTOC);
// connect(this->mpTOC, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(SLOTazShowContextMenuForLegend(const QPoint &)));
    //    QgsLegend * mpMapLegend = new QgsLegend(mpMapCanvas, 0, 0);


//    mpLegendLayout->addWidget(mpMapLegend); //adding TOC to Layout2

    //create actions
    mpVectorize = new QAction(QIcon(":/ico/vectorize.png"), tr("Vectorize"), this);
    mpActionAddVectorLayer = new QAction(QIcon(":/ico/add_vector.png"), tr("Добавить &векторный слой"), this);
    mpActionAddRasterLayer = new QAction(QIcon(":/ico/add_raster.png"), tr("Добавить &растровый слой"), this);
    mpActionAddVectorLayer->setStatusTip(tr("Добавить векторный слой в окно карты"));
    mpActionAddRasterLayer->setStatusTip(tr("Добавить растровый слой в окно карты"));
    mpContextShowExtent = new QAction("Показать экстент", tableLegend);
    mpContextRemoveLayer = new QAction("Удалить", tableLegend);
    mpActionFileExit = new QAction(QIcon(":/ico/mActionFileExit.png"), tr("Выход"), this);
    mpActionFileExit->setStatusTip(tr("Close Application"));
    mpActionFileExit->setShortcuts(QKeySequence::Close);

    //settings of TOC-tableWiget
    {
    tableLegend->setHorizontalHeaderLabels(QStringList() << tr("") << tr("Имя слоя") << tr("Стиль") << ("Путь"));
    tableLegend->setColumnCount(tableLegend->horizontalHeader()->count());
    tableLegend->setColumnWidth(0, 25);
    tableLegend->setColumnWidth(1, 185);
    tableLegend->setColumnWidth(2, 50);
    tableLegend->setContextMenuPolicy(Qt::CustomContextMenu);
    tableLegend->setSelectionBehavior(QAbstractItemView::SelectRows);

    mpContextLegendMenu = new QMenu(tableLegend);
    mpContextLegendMenu->addAction(mpContextShowExtent);
    mpContextLegendMenu->addAction(mpContextRemoveLayer);
    tableLegend->addAction(mpContextShowExtent);
    tableLegend->addAction(mpContextRemoveLayer);
    }


//    Connections
    {
     connect(this->mpActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
     connect(this->mpActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
     connect(this->mpActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));
     connect(this->mpActionAddVectorLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddVectorLayer()));
     connect(this->mpActionAddRasterLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddRasterLayer()));
     connect(this->mpActionFileExit, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileExit()));
     connect(this->mpVectorize, SIGNAL(triggered()), this, SLOT(SLOTtempUse()));
     connect(this->mpActionFileOpenProject, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileOpenProject()));
     connect(this->mpRegistry, SIGNAL(layerWasAdded(QgsMapLayer *)), this, SLOT(addLayerToTOC(QgsMapLayer *)));
     connect(this->mpContextShowExtent, SIGNAL(triggered()), this, SLOT(SLOTazContextShowExtent()));
     connect(this->mpCoordsEdit, SIGNAL(editingFinished()), this, SLOT( SLOTazCoordsCenter()));
     connect(this->mpMapCanvas, SIGNAL(xyCoordinates(const QgsPoint &)), this, SLOT(SLOTazShowMouseCoordinate(const QgsPoint &)));
     connect(this->mpActionBathymetry, SIGNAL(triggered()), this, SLOT(SLOTazThemTaskSpectralBathynometry()));

//     connect(this->mpTOC, SIGNAL(itemSelectionChanged()), this, SLOT(SLOTazGetSelectedLegendItem()));
    }
//     connect(QgsProject::instance(), SIGNAL(readProject(QDomDocument)), mpMapCanvas, SLOT(readProject(QDomDocument)));

//create a little toolbar
    {
    mpMapToolBar = addToolBar(tr("File"));
//    mpMapToolBar->addAction(mpActionAddLayer);
    mpMapToolBar->addAction(mpActionZoomIn);
    mpMapToolBar->addAction(mpActionZoomOut);
    mpMapToolBar->addAction(mpActionPan);
    mpMapToolBar->addAction(mpActionAddVectorLayer);
    mpMapToolBar->addAction(mpActionAddRasterLayer);
    mpMapToolBar->addAction(mpVectorize);
    }

// menu Bar
    {
        this->menuFile->addAction(mpActionAddVectorLayer);
        this->menuFile->addAction(mpActionAddRasterLayer);
        this->menuFile->addSeparator();
        this->menuFile->addAction(mpActionFileExit);
    }

    //create the maptools
    mpPanTool = new QgsMapToolPan(mpMapCanvas);
    mpPanTool->setAction(mpActionPan);
    mpZoomInTool = new QgsMapToolZoom(mpMapCanvas, FALSE); // false = in
    mpZoomInTool->setAction(mpActionZoomIn);
    mpZoomOutTool = new QgsMapToolZoom(mpMapCanvas, TRUE ); //true = out
    mpZoomOutTool->setAction(mpActionZoomOut);

    statusBar()->showMessage("Репер готов к работе.");
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

bool MainWindow::azSelectLayer(const QString layerName)
{
    bool bComplete(false);
    // создадим резервную копию рабочего слоя
    QgsMapLayer * pRezLayer;
    pRezLayer = NULL;
    if (mpSelectedLayer == NULL) // если он есть
    {
        pRezLayer = mpSelectedLayer;
    }

    QMapIterator < QString, QgsMapLayer * > i(this->mpRegistry->mapLayers());
    while (i.hasNext())
    {
        i.next();
        if (i.value()->name() == layerName)
        {
            mpSelectedLayer = i.value();
            bComplete = true;
            this->statusBar()->showMessage("Выбранный слой: '" + mpSelectedLayer->name() + "'.");
            break;
        }

    }
    if (pRezLayer != NULL)
    {
        if (!mpSelectedLayer->isValid() && pRezLayer->isValid()) // если mpSelectedLayer был замещен некорректным слоем
        {
            mpSelectedLayer = pRezLayer; // возвращаем значение слоя, который был
            bComplete = false; // сообщаем, что выбор слоя неудачен
            this->statusBar()->showMessage("Слой: '" + mpSelectedLayer->name() + "' не может быть выбран.");
        }
    }
    return bComplete;
}


bool MainWindow::azSelectLayer(const int layerNumber)
{
    bool bComplete(false);
    // создадим резервную копию рабочего слоя
    if (layerNumber > mpRegistry->count())
    {
        return bComplete;
    }

    QgsMapLayer * pRezLayer;
    pRezLayer = NULL;
    if (mpSelectedLayer == NULL) // если он есть
    {
        pRezLayer = mpSelectedLayer;
    }

    int n(0); // инициируем счетчик
    QMapIterator < QString, QgsMapLayer * > i(this->mpRegistry->mapLayers());
    while (i.hasNext())
    {
        i.next();
        n = n+1;
        if (n == layerNumber)
        {
            mpSelectedLayer = i.value();
            bComplete = true;
            this->statusBar()->showMessage("Выбранный слой: '" + mpSelectedLayer->name() + "'.");
            break;
        }

    }
    if (pRezLayer != NULL)
    {
        if (!mpSelectedLayer->isValid() && pRezLayer->isValid()) // если mpSelectedLayer был замещен некорректным слоем
        {
            mpSelectedLayer = pRezLayer; // возвращаем значение слоя, который был
            bComplete = false; // сообщаем, что выбор слоя неудачен
            this->statusBar()->showMessage("Слой: '" + mpSelectedLayer->name() + "' не может быть выбран.");
        }
    }
    return bComplete;
}

void MainWindow::azSetTitleWindow(QWidget &azApp)
{
      QString caption = "Репер";

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

bool MainWindow::azRasterEnhancement(QgsRasterLayer & azRasterLayer)
// функция улучшения изображения
// цель: улучшить вид отображения снимка для человеческого восприятия
// (по идее должна определять тип снимка автоматически и
// подстраивать соответветствующие параметры)

{
    bool bComplete(false); // инициализируем пременную для сигнализации об
                          //    успешном улучшении

    /*
      --------------------
// способы отображения снимка:
    pLayer->setDrawingStyle:
UndefinedDrawingStyle
SingleBandGray
SingleBandPseudoColor
PalettedColor
PalettedSingleBandGray
PalettedSingleBandPseudoColor
PalettedMultiBandColor          - три цвета
MultiBandSingleGandGray
MultiBandSingleBandGray
MultiBandSingleBandPseudoColor
MultiBandColor
SingleBandColorDataStyle
---------------------------
// алгоритм цветопередачи:
    pLayer->setColorShadingAlgorithm:
UndefinedShader
PseudoColorShader
FreakOutShader
ColorRampShader
UserDefinedShader
----------------------
    */


    // пока делаю так: 3-х цветное изображение и каналы:
    // 4 - красный; 3 - зеленый; 2 - синий.
    if (azRasterLayer.bandCount() < 3)
    {
        // меньше 3-х каналов не улучшаем
    }
    else if (azRasterLayer.bandCount() == 3)
    {
        azRasterLayer.setDrawingStyle( QgsRasterLayer::MultiBandColor); // устанавливаем "3-х цветное изображение"
        // по умолчанию если три то пусть идут в обратном
        azRasterLayer.setRedBandName(azRasterLayer.bandName(3));
        azRasterLayer.setGreenBandName(azRasterLayer.bandName(2));
        azRasterLayer.setBlueBandName(azRasterLayer.bandName(1));
        bComplete = true;
    }
    else
    {
        azRasterLayer.setDrawingStyle( QgsRasterLayer::MultiBandColor); // устанавливаем "3-х цветное изображение"
            // проверяем название каждого канала
        if (azRasterCheckBandName(azRasterLayer, "Band 4"))
        {
            azRasterLayer.setRedBandName("Band 4"); // есть - добавляем
        }
        if (azRasterCheckBandName(azRasterLayer, "Band 3"))
        {
            azRasterLayer.setGreenBandName("Band 3");
        }
        if (azRasterCheckBandName(azRasterLayer, "Band 2"))
        {
            azRasterLayer.setBlueBandName("Band 2");
        }
        bComplete = true;
    }

    azRasterLayer.setStandardDeviations(2.5); // для рястяжки по гистограмме яркости
                                              // исп. среднее квадратичное отклонение 2.5
    return bComplete;
}

bool MainWindow::azRasterCheckBandName(QgsRasterLayer &azRasterLayer, QString strBandName)
{
    bool bComplete(false);
    for (int i = 0; azRasterLayer.bandCount(); i++)
    {
        if (azRasterLayer.bandName(i) == strBandName)
        {
            bComplete = true;
            break;
        }
    }
    return bComplete;
}

bool MainWindow::azCopyFiles(QString azSource, QString azDestPath, bool bUse)
{
    bool bComplete(false);
    QFileInfo pFileInfo(azSource);
    if (!pFileInfo.isFile())
    {
        return bComplete;
    }
    if (azDestPath.length()<2) return bComplete;
    bComplete = QFile::copy(azSource, azDestPath);
    return bComplete;
}

bool MainWindow::azMakeLayer(QGis::WkbType azType, QString pDestDir, QString pName)
{
    bool bComplete(false);
    QString pAddName("");
    QStringList pExpList;
    pExpList << ".dbf" << ".prj" << ".qpj" << ".shp" << ".shx";
    if (azType == QGis::WKBPoint)
    {
        pAddName = "shps/ex_point";
    }
    else if (azType == QGis::WKBLineString)
    {
        pAddName ="shps/ex_line";
    }
    else if (azType == QGis::WKBPolygon)
    {
        pAddName = "shps/ex_poly";
    }
    else
    {
        return false;
    }
    bComplete = true;
    foreach (const QString &pExp, pExpList)
    {
        if (!azCopyFiles(mpAppPath + pAddName + pExp, pDestDir + pName + pExp))
        {
            QMessageBox::about(this, "Не скопирован", "из" + mpAppPath + pAddName + pExp +
                               "\n в" + pDestDir + pName + pExp);
            bComplete = false;
        }
    }
    return bComplete;
}

bool MainWindow::azAddLayerVector(QFileInfo pFile)
{
    QString myProviderName      = "ogr";
    QgsVectorLayer * mypLayer = new QgsVectorLayer(pFile.filePath(), pFile.completeBaseName(), myProviderName);
    QgsSymbolV2 * s = QgsSymbolV2::defaultSymbol(mypLayer->geometryType());
    QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(s);//(mypLayer->geometryType());
    //        QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->geometryType());
    mypLayer->setRendererV2(mypRenderer);
    if (!mypLayer->isValid())
    {
        qDebug("Shp слой некорректный. Слой не добавлен на карту." + pFile.filePath().toAscii());
        this->statusBar()->showMessage("Shp слой некорректный. Слой не добавлен на карту."
                                       "\n " + pFile.filePath() + "'");
        return false;
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
    return true;
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
    ProjectChange = false;
    if (ProjectChange)
    {
        pButtonsPack = 0x00410800;
        pMessageText = "Проект был изменен,"
                "\n вы хотите сохранить изменения перед выходом из приложения?";
    }
    else
    {
        pButtonsPack = 0x00404000;
        pMessageText = "Вы уверены, что хотите закрыть приложение?";
    }

    QMessageBox * pExitApp = new QMessageBox (QMessageBox::Question,
                                              "Выход из приложения",
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
    QString projectFileName = QFileDialog::getOpenFileName(this, "Открыть файл проекта карты *.qgs", "", "*.qgs");
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
        this->statusBar()->showMessage("Ошибка открытия файла проекта!"
                                       "\n " + projectFileName + "'");
        qDebug("Ошибка открытия файла проекта!" + projectFileName.toAscii());
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
            mpRegistry->addMapLayer(i.value(), TRUE);
            mpLayerSet.append(QgsMapCanvasLayer(i.value()));
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

void MainWindow::SLOTtempUse()
{
    QFileInfo pFile("D:/!Share/layers/baba.shp");
    this->azAddLayerVector(pFile);
}

long MainWindow::azGetEPSG(const QString rastrPath)
{
    QFileInfo pFileInfo(rastrPath);
    if (!pFileInfo.isFile()) // проверка существования файла
    {
        return -1;
    }
    QgsRasterLayer * pRLayer = new QgsRasterLayer(pFileInfo.filePath(), pFileInfo.completeBaseName());
    return pRLayer->crs().epsg();
}


void MainWindow::SLOTmpActionAddVectorLayer()
{
    QString fullLayerName = QFileDialog::getOpenFileName(this, "Добавить векторный слой", "", "Все поддерживаемые форматы(*.shp);;Shapefiles (*.shp)");
    QFileInfo azFileInfo(fullLayerName);
    if (!azFileInfo.isFile())
    {
        return;
    }
    this->azAddLayerVector(azFileInfo);
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
    // СЛОТ: добавление растра в окно карты
    // сначала вызываем диалог, получаем путь к файлу
    QString fullLayerName = QFileDialog::getOpenFileName(this, "Добавить растровый слой", "",
                         "Все поддерживаемые растровые форматы (*.img *.asc *.tif *tiff *.bmp *.jpg *.jpeg);;Geotiff (*.tif *.tiff)");
    QFileInfo azFileInfo(fullLayerName);
    if (!azFileInfo.isFile()) // проверка существования файла
    {
        return;
    }


    // создаем экземпляр класса QgsRasterLayer
    // он содержит всю инфу о растре
    QgsRasterLayer * mypLayer = new QgsRasterLayer(azFileInfo.filePath(), azFileInfo.completeBaseName());
    if (!mypLayer->isValid()) // проверяем его корректность
    {
        qDebug("Raster layer is not valid or not supported by GDAL. Layer was not added to map." + fullLayerName.toAscii());
        this->statusBar()->showMessage("Файл растра не явялется корректным или не поддерживается библиотекой GDAL. Слой не был добавлен на карту."
                                       "\n " + fullLayerName + "'");
        return;
    }
    if (!this->azRasterEnhancement(*mypLayer)) // применяем улучшение снимка
    {
        qDebug("Улучшение растра не использовано. Нераспознаный формат растра." + fullLayerName.toAscii());
    }
    // применяем улучшение контраста (т.е. цвета отображаются от минимума до максимума)
    mypLayer->setContrastEnhancementAlgorithm(QgsContrastEnhancement::StretchToMinimumMaximum, false );

    // самообновление
    connect(mypLayer, SIGNAL(repaintRequested()), mpMapCanvas, SLOT(refresh()) );

    // Add the Vector Layer to the Layer Registry
    mpRegistry->addMapLayer(mypLayer, TRUE);
    // Add the Layer to the Layer Set
    mpLayerSet.append(QgsMapCanvasLayer(mypLayer));
    mpMapCanvas->setExtent(mypLayer->extent());
    mpMapCanvas->setLayerSet(mpLayerSet);
    mpMapCanvas->refresh();
/*    QgsMapLayer *pLayer;
    pLayer = mypLayer;
    QgsCoordinateReferenceSystem pSRS;
    pSRS = mypLayer->*/
}

void MainWindow::SLOTmpActionVectorize()
{
    dnThemTaskSpecBath->close();
    QString pMessage(""); // сообщение в статус баре о результате векторизации
    if (this->dnThemTaskSpecBath->Polygons.count() < 1)
    {
        pMessage = tr("Выбранные объекты не содержат информации для векторизации");
        this->statusBar()->showMessage(pMessage);
        return;
    }

    QString mEncoding; // кодировка
    mEncoding = "UTF-8";

    QgsFieldMap mFields;
    QgsField myField1("value", QVariant::Double, "Double", 0, 0);
    QgsField myField2( "comment", QVariant::String, "String", 10, 0, "Comment" );
    mFields.insert( 0, myField1 );
    mFields.insert( 1, myField2 );
    QgsCoordinateReferenceSystem pSRS;

    // создаем систему координат идентичную растру
    pSRS.createFromOgcWmsCrs("EPSG:" +
                             QString::number(azGetEPSG(dnThemTaskSpecBath->Polygons.at(0).EPSG)));
    pSRS.validate();
    //QString myFileName (dnThemTaskSpecBath->Polygons.at(0).NameLayer +
    //                    QString::number(QTime::currentTime().hour())  + "-" +
    //                    QString::number(QTime::currentTime().minute())  + "-" +
    //                    QString::number(QTime::currentTime().second()) + "-" +
    //                    QString::number(QTime::currentTime().msec()) + ".shp");
    QString myFileName ("baba.shp");
    myFileName = "D:/!Share/layers/" + myFileName;
    QgsVectorFileWriter myWriter( myFileName, mEncoding, mFields, QGis::WKBPolygon, &pSRS);

//    QMessageBox::about(this->dnThemTaskSpecBath, "test", "Hello!");
    for (int i = 0; i < dnThemTaskSpecBath->Polygons.size(); i++)
    {
        DNVector dnVec;
        dnVec = dnThemTaskSpecBath->Polygons.at(i);
        QgsPolyline pPolyLine;
		QgsPoint pFirstPoint (dnVec.GPt.at(0).x, dnVec.GPt.at(0).y);
        for (int j = 0; j < dnVec.GPt.size(); j++)
        {
            QgsPoint p(dnVec.GPt.at(j).x, dnVec.GPt.at(j).y);
            pPolyLine << p;
        }
		pPolyLine << pFirstPoint;
        QgsPolygon pPolygon;
        pPolygon << pPolyLine;
        QgsGeometry * pPolygonGeometry = QgsGeometry::fromPolygon( pPolygon );
        QgsFeature pFeature;
        pFeature.setTypeName( "WKBPolygon" );
        pFeature.setGeometry( pPolygonGeometry );
        pFeature.addAttribute( 1, "val" );
        pFeature.addAttribute(0, (double)dnVec.Vol);

        QgsVectorFileWriter::WriterError mError;
        myWriter.addFeature( pFeature );
        mError = myWriter.hasError();
        if (mError != 0)
        {
            qWarning() << myWriter.errorMessage();
        }
    }
	QFileInfo pFile("D:/!Share/layers/baba.shp");
	this->azAddLayerVector(pFile);
	return;
    

    /*QFileInfo pFile(myFileName);
    if (pFile.isFile())
    {
	    if (azAddLayerVector(pFile))
		{
			pMessage = "Добавлен слой '" + myFileName + "'";
			this->statusBar()->showMessage(pMessage);
		}
    }*/

}

void MainWindow::SLOTazShowMouseCoordinate(const QgsPoint & p )
{
      if ( mpLayerSet.count() < 1 ) return;
      if (!mpMapCanvas->underMouse()) return;
      mpLastMapPosition = p;
     
/*      if ( mMapTipsVisible )
//      {
          // store the point, we need it for when the maptips timer fires
          mpLastMapPosition = p;
    // we use this slot to control the timer for maptips since it is fired each time
    // the mouse moves.
    if ( mMapCanvas->underMouse() )
    {
      // Clear the maptip (this is done conditionally)
      mpMaptip->clear( mMapCanvas );
      // don't start the timer if the mouse is not over the map canvas
      mpMapTipsTimer->start();
      //QgsDebugMsg("Started maptips timer");
    }
     }*/

      if ( mpMapCanvas->mapUnits() == QGis::DegreesMinutesSeconds )
      {
          mpCoordsEdit->setText( p.toDegreesMinutesSeconds(0) );
      }
      else
      {
          mpCoordsEdit->setText( p.toString(/* mpMousePrecisionDecimalPlaces */) );
      }
      if ( mpCoordsEdit->width() > mpCoordsEdit->minimumWidth() )
      {
          mpCoordsEdit->setMinimumWidth( mpCoordsEdit->width() );
      }

}


void MainWindow::SLOTazShowContextMenuForLegend(const QPoint & pos)
{
//    mpContextLegendMenu->exec(mpTOC->viewport()->mapToGlobal(pos));
//    QMessageBox::about(mpTOC, "", "OK!");
}

void MainWindow::SLOTazContextShowExtent()
{
//    QgsMapLayer * pLayer = mpMapCanvas->layer(0);
//    mpMapCanvas->setExtent(pLayer->extent());
//    mpMapCanvas->refresh();
}

void MainWindow::SLOTazGetSelectedLegendItem()
{
//    QModelIndexList pList;
//    QModelIndex pIndex;
//    QTableWidgetItem *pItem = new QTableWidgetItem();
////    pList = tableLegend->selectedIndexes();
//    if (pList.count()<1) return;
    //    qDebug("all ok");
}

void MainWindow::SLOTazThemTaskSpectralBathynometry()
{
    if (dnThemTaskSpecBath == NULL);
    {
        dnThemTaskSpecBath = new DNSpecBath(this);
        connect(this->dnThemTaskSpecBath, SIGNAL(SIGNALcreateVector()), this, SLOT(SLOTmpActionVectorize()));
    }

    dnThemTaskSpecBath->setWindowTitle(tr("Оценка глубин прибрежных территорий"));
//    dnThemTaskSpecBath->setWindowModality(Qt::WindowModal);
    dnThemTaskSpecBath->show();
}

void MainWindow::SLOTazCoordsCenter()
{
    QStringList parts = mpCoordsEdit->text().split( ',' );
    if ( parts.size() != 2 )
      return;

    bool xOk;
    double x = parts.at( 0 ).toDouble( &xOk );
    if ( !xOk ) return;

    bool yOk;
    double y = parts.at( 1 ).toDouble( &yOk );
    if ( !yOk ) return;

    QgsRectangle r = mpMapCanvas->extent();

    mpMapCanvas->setExtent(QgsRectangle (
        x - r.width() / 2.0,  y - r.height() / 2.0,
        x + r.width() / 2.0, y + r.height() / 2.0  ));
    mpMapCanvas->refresh();
}

