#include "kksgiswidgetqgis.h"
#include "kksbadlayerhandler.h"
#include "qgslegend.h"
#include "qgslayerorder.h"

#include <qgsmessagebar.h>
#include <qgspallabeling.h>
#include <qgsproviderregistry.h>
#include <qgsmaplayerregistry.h>
#include <qgsmapcanvas.h>
#include <qgsmaptool.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>


KKSGISWidgetQGIS::KKSGISWidgetQGIS(QWidget* parent, Qt::WFlags fl)
    : KKSGISWidget(parent, fl),
    mpSelectedLayer(NULL),
    mpCoordsLabel(NULL),
    mpCoordsEdit(NULL),
    mpCoordsEditValidator(NULL),
    mpStatusBar(NULL),
    m_badLayerHandler(NULL),
    mpMapCanvas(NULL),
    mpMapLegend(NULL),
    mpMapLayerOrder(NULL),
    mpMapLayerOrderWidget(NULL),
    mpMapLegendWidget(NULL),
    mpMapToolBar(NULL),
    mpPanTool(NULL),
    mpZoomInTool(NULL),
    mpZoomOutTool(NULL),
    mpMenuBar(NULL),
    mpVectorize(NULL),
    mpActionAddVectorLayer(NULL),
    mpActionAddRasterLayer(NULL),
    mpActionAddPostGISLayer(NULL),
    mpActionFileOpenProject(NULL),
    mpActionFileSaveProject(NULL),
    mpActionFileSaveProjectAs(NULL),
    mpActionFileCloseProject(NULL),
    mpActionZoomIn(NULL),
    mpActionZoomOut(NULL),
    mpActionPan(NULL),
    //mpTableLegend(NULL),
    mpRegistry(QgsMapLayerRegistry::instance())
{
    setWindowIcon(QIcon(":/ico/main64x64.png"));
    
    dnThemTaskSpecBath = NULL; //???
    azWorkList.clear();

    initStatusBar();
    initUserSettings();
    initMapCanvas();

    //initLegend();
    initMapLegend();

    initActions();
    initConnections();

    initToolBar();
    initMenuBar();

    initMapTools();

    this->azSetTitleWindow(*this);

    mpStatusBar->showMessage("ПК Репер готов к работе.");
    m_badLayerHandler = new KKSBadLayerHandler;
    QgsProject::instance()->setBadLayerHandler(m_badLayerHandler);
}


KKSGISWidgetQGIS::~KKSGISWidgetQGIS()
{
  delete mpZoomInTool;
  delete mpZoomOutTool;
  delete mpPanTool;
  delete mpMapToolBar;
  delete mpMenuBar;
  delete mpStatusBar;
  delete mpMapCanvas;
}
   
void KKSGISWidgetQGIS::initStatusBar()
{
    mpStatusBar = new QStatusBar();
    if(!mpStatusBar)
        return;

    QFont pFont("Courier", 9);
    mpStatusBar->setFont(pFont);

    mpCoordsLabel = new QLabel(QString(), mpStatusBar);
    mpCoordsLabel->setObjectName("mpCoordsLabel");
    mpCoordsLabel->setMinimumWidth(10);
    mpCoordsLabel->setMaximumHeight(20);
    mpCoordsLabel->setFrameStyle(QFrame::NoFrame);
    mpCoordsLabel->setAlignment(Qt::AlignCenter);
    mpStatusBar->addPermanentWidget(mpCoordsLabel, 0);

    mpCoordsEdit = new QLineEdit( QString(), mpStatusBar );
    mpCoordsEdit->setObjectName( "mpCoordsEdit" );
    mpCoordsEdit->setFont(pFont);
    mpCoordsEdit->setMinimumWidth(10);
    mpCoordsEdit->setMaximumWidth(300);
    mpCoordsEdit->setMaximumHeight(20);
    mpCoordsEdit->setContentsMargins( 0, 0, 0, 0 );
    mpCoordsEdit->setAlignment( Qt::AlignCenter );
    QRegExp coordValidator( "[+-]?\\d+\\.?\\d*\\s*,\\s*[+-]?\\d+\\.?\\d*" );
    mpCoordsEditValidator = new QRegExpValidator( coordValidator, mpCoordsEdit );

    mpStatusBar->addPermanentWidget( mpCoordsEdit, 0 );
}

QStatusBar * KKSGISWidgetQGIS::statusBar() const
{
    return mpStatusBar;
}

void KKSGISWidgetQGIS::initUserSettings()
{
    mpAppPath = "D:/!Share/src/reper/ddocclient/app/reper/";
    //QString myPluginsDir        = "C:\\Program Files (x86)\\Quantum GIS 1.7.0\\plugins";
    m_pluginsDir        = "C:\\Program Files (x86)\\DynamicSoft\\DynamicDocs Client 1.2.3\\QGS21\\plugins";

    //QString myPluginsDir        = "D:\\REP_EXT\\QGS21\\plugins";
    QgsApplication::setPluginPath(m_pluginsDir);
    //QgsProviderRegistry::instance(m_pluginsDir);
}

const QString & KKSGISWidgetQGIS::pluginsDir()
{
    return m_pluginsDir;
}

void KKSGISWidgetQGIS::initMapCanvas()
{
    // Create the Map Canvas
    mpMapCanvas= new QgsMapCanvas(0, 0);
    mpMapCanvas->enableAntiAliasing(true);
    mpMapCanvas->useImageToRender(false);
    mpMapCanvas->setCanvasColor(QColor(255, 255, 255));
    mpMapCanvas->freeze(false);
    mpMapCanvas->setVisible(true);
    mpMapCanvas->refresh();

    mLBL = new QgsPalLabeling();
    mpMapCanvas->mapRenderer()->setLabelingEngine( mLBL );

    // a bar to warn the user with non-blocking messages
    mInfoBar = new QgsMessageBar( this );
    mInfoBar->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
    
    mpMapCanvas->show();

    // добавляем фрейм и вставляем в него виджет "Окно карты"(Map Canvas)
    QVBoxLayout * mpMapLayout = new QVBoxLayout(this); //main Layout
    mpMapLayout->addWidget(mpMapCanvas); // adding MapCanvas in Layout
    mpMapLayout->addWidget( mInfoBar);
}

void KKSGISWidgetQGIS::initMapLegend()
{
    // "theMapLegend" used to find this canonical instance later
    mpMapLegend = new QgsLegend( mpMapCanvas, this, "theMapLegend" );
    mpMapLegend->setWorkingWidget(this);

    mpMapLegend->setWhatsThis( tr( "Map legend that displays all the layers currently on the map canvas. Click on the check box to turn a layer on or off. Double click on a layer in the legend to customize its appearance and set other properties." ) );

    QCheckBox *orderCb = new QCheckBox( tr( "Control rendering order" ) );
    orderCb->setChecked( false );

    connect( orderCb, SIGNAL( toggled( bool ) ), mpMapLegend, SLOT( unsetUpdateDrawingOrder( bool ) ) );
    connect( mpMapLegend, SIGNAL( updateDrawingOrderUnchecked( bool ) ), orderCb, SLOT( setChecked( bool ) ) );

    mpMapLegendWidget = new QWidget( this );
    QLayout *l = new QVBoxLayout;
    l->setMargin( 0 );
    l->addWidget( mpMapLegend );
    mpMapLegendWidget->setLayout( l );
    //mLegendDock = new QDockWidget( tr( "Layers" ), this );
    //mLegendDock->setObjectName( "Legend" );
    //mLegendDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    //mLegendDock->setWidget( mpMapLegendWidget );
    //addDockWidget( Qt::LeftDockWidgetArea, mLegendDock );

    // add to the Panel submenu
    //mPanelMenu->addAction( mLegendDock->toggleViewAction() );

    mpMapLayerOrder = new QgsLayerOrder( mpMapLegend, this, "theMapLayerOrder" );
    mpMapLayerOrder->setWhatsThis( tr( "Map layer list that displays all layers in drawing order." ) );

    mpMapLayerOrderWidget = new QWidget( this );
    l = new QVBoxLayout;
    l->setMargin( 0 );
    l->addWidget( mpMapLayerOrder );
    l->addWidget( orderCb );
    mpMapLayerOrderWidget->setLayout( l );
    //mLayerOrderDock = new QDockWidget( tr( "Layer order" ), this );
    //mLayerOrderDock->setObjectName( "LayerOrder" );
    //mLayerOrderDock->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    //mLayerOrderDock->setWidget( mpMapLayerOrderWidget );
    //addDockWidget( Qt::LeftDockWidgetArea, mLayerOrderDock );
    //mLayerOrderDock->hide();

    // add to the Panel submenu
    //mPanelMenu->addAction( mLayerOrderDock->toggleViewAction() );

    return;
}

QWidget * KKSGISWidgetQGIS::mapLegendWidget()
{
    return mpMapLegendWidget;
}

QWidget * KKSGISWidgetQGIS::mapLayerOrderWidget()
{
    return mpMapLayerOrderWidget;
}

/*
void KKSGISWidgetQGIS::initLegend()
{
    mpTableLegend = new QTableWidget(this);
    mpTableLegend->setColumnCount(3);
    
    mpTableLegend->setObjectName(QString::fromUtf8("tableLegend"));
    
    QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(mpTableLegend->sizePolicy().hasHeightForWidth());
    
    mpTableLegend->setSizePolicy(sizePolicy2);
    
    mpTableLegend->setMinimumSize(QSize(200, 200));
    mpTableLegend->setFrameShape(QFrame::StyledPanel);
    mpTableLegend->setFrameShadow(QFrame::Plain);
    mpTableLegend->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mpTableLegend->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mpTableLegend->setSelectionBehavior(QAbstractItemView::SelectRows);
    mpTableLegend->setColumnCount(3);
    mpTableLegend->verticalHeader()->setVisible(false);
    mpTableLegend->verticalHeader()->setHighlightSections(false);

    mpTableLegend->setColumnWidth(0, 25);
    mpTableLegend->setColumnWidth(1, 185);
    mpTableLegend->setColumnWidth(2, 50);
    //tableLegend->setContextMenuPolicy(Qt::CustomContextMenu);
    //tableLegend->setSelectionBehavior(QAbstractItemView::SelectRows);

    //mpContextLegendMenu = new QMenu(tableLegend);
    //mpContextLegendMenu->addAction(mpContextShowExtent);
    //mpContextLegendMenu->addAction(mpContextRemoveLayer);
    //tableLegend->addAction(mpContextShowExtent);
    //tableLegend->addAction(mpContextRemoveLayer);
}
*/
/*
QTableWidget * KKSGISWidgetQGIS::tableLegend()
{
    return mpTableLegend;
}
*/

void KKSGISWidgetQGIS::initActions()
{
    mpActionZoomIn = new QAction(this);
    mpActionZoomIn->setObjectName(QString::fromUtf8("mpActionZoomIn"));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/ico/mActionZoomIn.png"), QSize(), QIcon::Normal, QIcon::Off);
    mpActionZoomIn->setIcon(icon);
    mpActionZoomIn->setText(tr("Приблизить"));

    mpActionZoomOut = new QAction(this);
    mpActionZoomOut->setObjectName(QString::fromUtf8("mpActionZoomOut"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/ico/mActionZoomOut.png"), QSize(), QIcon::Normal, QIcon::Off);
    mpActionZoomOut->setIcon(icon1);
    mpActionZoomOut->setText(tr("Отдалить"));

    mpActionPan = new QAction(this);
    mpActionPan->setObjectName(QString::fromUtf8("mpActionPan"));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/ico/mActionPan.png"), QSize(), QIcon::Normal, QIcon::Off);
    mpActionPan->setIcon(icon2);
    mpActionPan->setText(tr("Перемещение"));

    /*
    mpActionFileExit = new QAction(SimpleMapWindowBase);
    mpActionFileExit->setObjectName(QString::fromUtf8("mpActionFileExit"));
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/ico/mActionFileExit.png"), QSize(), QIcon::Normal, QIcon::On);
    mpActionFileExit->setIcon(icon3);
    */

    mpActionFileOpenProject = new QAction(this);
    mpActionFileOpenProject->setObjectName(QString::fromUtf8("mpActionFileOpenProject"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/ico/mActionFileOpen.png"), QSize(), QIcon::Normal, QIcon::On);
    mpActionFileOpenProject->setIcon(icon4);
    mpActionFileOpenProject->setText(tr("Открыть проект..."));
    
    mpActionFileSaveProject = new QAction(this);
    mpActionFileSaveProject->setObjectName(QString::fromUtf8("mpActionFileSaveProject"));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/ico/mActionFileSave.png"), QSize(), QIcon::Normal, QIcon::On);
    mpActionFileSaveProject->setIcon(icon5);
    mpActionFileSaveProject->setText(tr("Сохранить проект"));
    
    mpActionBathymetry = new QAction(this);
    mpActionBathymetry->setObjectName(QString::fromUtf8("mpActionBathymetry"));
    mpActionBathymetry->setText(tr("Оценка глубин прибрежных территорий"));
    
    /*
    mpActionAddRasterLayer = new QAction(this);
    mpActionAddRasterLayer->setObjectName(QString::fromUtf8("mpActionAddRasterLayer"));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/ico/add_raster.png"), QSize(), QIcon::Normal, QIcon::Off);
    mpActionAddRasterLayer->setIcon(icon5);
    */
    
    /*
    mpActionAddVectorLayer = new QAction(this);
    mpActionAddVectorLayer->setObjectName(QString::fromUtf8("mpActionAddVectorLayer"));
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/ico/add_vector.png"), QSize(), QIcon::Normal, QIcon::Off);
    mpActionAddVectorLayer->setIcon(icon6);
    */

    mpActionFileSaveProjectAs = new QAction(this);
    mpActionFileSaveProjectAs->setObjectName(QString::fromUtf8("mpActionFileSaveProjectAs"));
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/ico/mActionFileSaveAs.png"), QSize(), QIcon::Normal, QIcon::On);
    mpActionFileSaveProjectAs->setIcon(icon6);
    mpActionFileSaveProjectAs->setText(tr("Сохранить проект как..."));

    mpActionFileCloseProject = new QAction(this);
    mpActionFileCloseProject->setObjectName(QString::fromUtf8("mpActionFileCloseProject"));
    mpActionFileCloseProject->setText(tr("Закрыть проект"));


    mpVectorize = new QAction(QIcon(":/ico/vectorize.png"), tr("Векторизация"), this);
    
    mpActionAddVectorLayer = new QAction(QIcon(":/ico/add_vector.png"), tr("Добавить &векторный слой"), this);
    mpActionAddVectorLayer->setStatusTip(tr("Добавить векторный слой в окно карты"));

    mpActionAddRasterLayer = new QAction(QIcon(":/ico/add_raster.png"), tr("Добавить &растровый слой"), this);
    mpActionAddRasterLayer->setStatusTip(tr("Добавить растровый слой в окно карты"));
    
    //mpActionFileExit = new QAction(QIcon(":/ico/mActionFileExit.png"), tr("Выход"), this);
    //mpActionFileExit->setStatusTip(tr("Close Application"));
    //mpActionFileExit->setShortcuts(QKeySequence::Close);
    
    mpActionAddPostGISLayer = new QAction(QIcon(":/ico/add_postgis.png"), tr("Добавить слой из БД"), this);
    mpActionAddPostGISLayer->setStatusTip(tr("Добавить слой из БД"));
}


void KKSGISWidgetQGIS::initConnections()
{
     connect(this->mpActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
     connect(this->mpActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
     connect(this->mpActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));

     connect(this->mpActionAddVectorLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddVectorLayer()));
     connect(this->mpActionAddRasterLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddRasterLayer()));
     connect(this->mpActionAddPostGISLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddPostGISLayer()));

     connect(this->mpVectorize, SIGNAL(triggered()), this, SLOT(SLOTtempUse()));

     connect(this->mpActionFileOpenProject, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileOpenProject()));
     connect(this->mpActionFileSaveProjectAs, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileSaveProjectAs()));
     connect(this->mpActionFileSaveProject, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileSaveProject()));
     connect(this->mpActionFileCloseProject, SIGNAL(triggered()), this, SLOT(SLOTmpCloseProject()));
     //connect(this->mpActionFileExit, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileExit()));

     connect(this->mpRegistry, SIGNAL(layerWasAdded(QgsMapLayer *)), this, SLOT(addLayerToTOC(QgsMapLayer *)));
     connect(this->mpCoordsEdit, SIGNAL(editingFinished()), this, SLOT( SLOTazCoordsCenter()));
     connect(this->mpMapCanvas, SIGNAL(xyCoordinates(const QgsPoint &)), this, SLOT(SLOTazShowMouseCoordinate(const QgsPoint &)));
     connect(this->mpActionBathymetry, SIGNAL(triggered()), this, SLOT(SLOTazThemTaskSpectralBathynometry()));

     connect(QgsProject::instance(), SIGNAL(readProject(QDomDocument)), mpMapCanvas, SLOT(readProject(QDomDocument)));
}


void KKSGISWidgetQGIS::initToolBar()
{
    mpMapToolBar = new QToolBar(tr("File"));
    mpMapToolBar->addAction(mpActionZoomIn);
    mpMapToolBar->addAction(mpActionZoomOut);
    mpMapToolBar->addAction(mpActionPan);

    mpMapToolBar->addSeparator();

    mpMapToolBar->addAction(mpActionAddVectorLayer);
    mpMapToolBar->addAction(mpActionAddRasterLayer);
    mpMapToolBar->addAction(mpActionAddPostGISLayer);

    mpMapToolBar->addSeparator();

    mpMapToolBar->addAction(mpVectorize);
}

QToolBar * KKSGISWidgetQGIS::toolBar() const
{
    return mpMapToolBar;
}


void KKSGISWidgetQGIS::initMenuBar()
{
    mpMenuBar = new QMenuBar();

    QMenu * menuFile = new QMenu(tr("Файл"));
    menuFile->addAction(mpActionFileOpenProject);
    menuFile->addAction(mpActionFileSaveProject);
    menuFile->addAction(mpActionFileSaveProjectAs);
    menuFile->addSeparator();
    menuFile->addAction(mpActionFileCloseProject);

    mpMenuBar->addMenu(menuFile);
    mpMenuMap.insert(tr("File"), menuFile);

    QMenu * menuLayers = new QMenu(tr("Слои"));
    menuLayers->addAction(mpActionAddVectorLayer);
    menuLayers->addAction(mpActionAddRasterLayer);
    menuLayers->addAction(mpActionAddPostGISLayer);

    mpMenuBar->addMenu(menuLayers);
    mpMenuMap.insert(tr("Layers"), menuLayers);

    QMenu * menuNavigation = new QMenu(tr("Навигация по карте"));
    menuNavigation->addAction(mpActionZoomIn);
    menuNavigation->addAction(mpActionZoomOut);
    menuNavigation->addAction(mpActionPan);

    mpMenuBar->addMenu(menuNavigation);
    mpMenuMap.insert(tr("Navigation"), menuNavigation);

    QMenu * menuTasks = new QMenu(tr("Тематический анализ"));
    menuTasks->addAction(mpActionBathymetry);

    mpMenuBar->addMenu(menuTasks);
    mpMenuMap.insert(tr("Thematic Analysis"), menuTasks);
    //menuFile->addSeparator();
    //menuFile->addAction(mpActionFileExit);
}

QMenuBar * KKSGISWidgetQGIS::menuBar() const
{
    return mpMenuBar;
}

const QMap<QString, QMenu *> & KKSGISWidgetQGIS::menuMap() const
{
    return mpMenuMap;
}

void KKSGISWidgetQGIS::initMapTools()
{
    //create the maptools
    mpPanTool = new QgsMapToolPan(mpMapCanvas);
    mpPanTool->setAction(mpActionPan);
    mpZoomInTool = new QgsMapToolZoom(mpMapCanvas, FALSE); // false = in
    mpZoomInTool->setAction(mpActionZoomIn);
    mpZoomOutTool = new QgsMapToolZoom(mpMapCanvas, TRUE ); //true = out
    mpZoomOutTool->setAction(mpActionZoomOut);
}

void KKSGISWidgetQGIS::azSetTitleWindow(QWidget &azApp)
{
      QString caption = "ПК Репер";


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
