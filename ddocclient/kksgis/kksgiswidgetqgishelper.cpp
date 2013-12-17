#include "kksgiswidgetqgis.h"
#include "kksbadlayerhandler.h"
#include "qgslegend.h"
#include "qgslegendlayer.h"
#include "qgslayerorder.h"
#include "qgsattributetabledialog.h"
#include "qgsclipboard.h"
#include "qgsguivectorlayertools.h"

#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>

#include <qgsmessagebar.h>
#include <qgspallabeling.h>
#include <qgsproviderregistry.h>
#include <qgsmaplayerregistry.h>
#include <qgsmapcanvas.h>
#include <qgsmaptool.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>
#include <qgsscalecombobox.h>
#include <qgsgenericprojectionselector.h>
#include <qgsmessagebaritem.h>
#include <qgsrasterlayersaveasdialog.h>
#include <qgsvectorlayersaveasdialog.h>
#include <qgsrasterfilewriter.h>
#include <qgsvectorfilewriter.h>
#include <qgsdatumtransformdialog.h>
#include <qgsmessageviewer.h>
#include <qgsdataitem.h>
#include <qgsvectordataprovider.h>
#include <qgssublayersdialog.h>
#include <qgserrordialog.h>
#include <qgsquerybuilder.h>
#include <qgseditorwidgetregistry.h>
#include <qgsrelationreferencefactory.h>





KKSGISWidgetQGIS::KKSGISWidgetQGIS(bool withSubWindows, QWidget* parent, Qt::WFlags fl)
    : KKSGISWidget(parent, fl),
    mpSelectedLayer(NULL),
    mpCoordsLabel(NULL),
    mpCoordsEdit(NULL),
    mpCoordsEditValidator(NULL),
    mpStatusBar(NULL),
    m_badLayerHandler(NULL),
    mpMapCanvas(NULL),
    mpMapLegend(NULL),
    mpMapLegendWidget(NULL),
    mpMapLayerOrder(NULL),
    mpMapLayerOrderWidget(NULL),
    mProgressBar(NULL),
    mScaleEditValidator(NULL),
    mScaleLabel(NULL),
    mScaleEdit(NULL),
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
    mActionRemoveLayer(NULL),
    mActionDuplicateLayer(NULL),
    mActionSetLayerCRS(NULL),
    mActionSetProjectCRSFromLayer(NULL),
    mActionLayerProperties(NULL),
    mActionLayerSubsetString(NULL),
    mActionToggleEditing(NULL),
    mActionSaveLayerEdits(NULL),
    mActionAllEdits(NULL),
    mActionSaveEdits(NULL),
    mActionSaveAllEdits(NULL),
    mActionCopyStyle(NULL),
    mActionPasteStyle(NULL),
    mActionOpenTable(NULL),
    mInternalClipboard( 0 ),
    mpRegistry(QgsMapLayerRegistry::instance())
{
    setWindowIcon(QIcon(":/ico/main64x64.png"));
    
    dnThemTaskSpecBath = NULL; //???
    azWorkList.clear();

    m_bWithSubwindows = withSubWindows; //надо ли создавать дополнительные окна (меню, тулбар, статусбыр и т.п.)

    initStatusBar();
    initUserSettings();
    initMapCanvas();

    //initLegend();
    initMapLegend();

    initActions();
    initConnections();
    readSettings();

    initToolBar();
    initMenuBar();

    initMapTools();

    this->azSetTitleWindow(*this);

    m_badLayerHandler = new KKSBadLayerHandler;
    QgsProject::instance()->setBadLayerHandler(m_badLayerHandler);

    // now build vector and raster file filters
    mVectorFileFilter = QgsProviderRegistry::instance()->fileVectorFilters();
    mRasterFileFilter = QgsProviderRegistry::instance()->fileRasterFilters();

    mInternalClipboard = new QgsClipboard; // create clipboard
    connect( mInternalClipboard, SIGNAL( changed() ), this, SLOT( clipboardChanged() ) );

    mVectorLayerTools = new QgsGuiVectorLayerTools();
    ((QgsGuiVectorLayerTools *)mVectorLayerTools)->setWorkingWidget(this);
    // Init the editor widget types
    QgsEditorWidgetRegistry* editorWidgetRegistry = QgsEditorWidgetRegistry::instance();
    QgsAttributeEditorContext context;
    context.setVectorLayerTools( vectorLayerTools() );
    editorWidgetRegistry->registerWidget( "RelationReference", new QgsRelationReferenceFactory( context, tr( "Relation Reference" ) ) );

    if(mpStatusBar)
        mpStatusBar->showMessage("ПК Репер готов к работе.");
}


KKSGISWidgetQGIS::~KKSGISWidgetQGIS()
{
  delete mpZoomInTool;
  delete mpZoomOutTool;
  delete mpPanTool;
  
  if(mpMapToolBar)
      delete mpMapToolBar;
  
  if(mpMenuBar)
    delete mpMenuBar;
  
  if(mpStatusBar)
      delete mpStatusBar;
  
  delete mpMapCanvas;

  delete mInternalClipboard;

}
   
void KKSGISWidgetQGIS::initStatusBar()
{
    if(!m_bWithSubwindows)
        return;

    mpStatusBar = new QStatusBar();
    if(!mpStatusBar)
        return;

    //
    // Add a panel to the status bar for the scale, coords and progress
    // And also rendering suppression checkbox
    //
    mProgressBar = new QProgressBar( mpStatusBar );
    mProgressBar->setObjectName( "mProgressBar" );
    mProgressBar->setMaximumWidth( 100 );
    mProgressBar->hide();
    mProgressBar->setWhatsThis( tr( "Progress bar that displays the status "
                                    "of rendering layers and other time-intensive operations" ) );
    mpStatusBar->addPermanentWidget( mProgressBar, 1 );

  
    QFont myFont("Arial", 9);
    mpStatusBar->setFont(myFont);

    mpCoordsLabel = new QLabel(QString(), mpStatusBar);
    mpCoordsLabel->setObjectName( "mCoordsLabel" );
    mpCoordsLabel->setFont( myFont );
    mpCoordsLabel->setMinimumWidth( 10 );
    mpCoordsLabel->setMaximumHeight( 20 );
    mpCoordsLabel->setMargin( 3 );
    mpCoordsLabel->setAlignment( Qt::AlignCenter );
    mpCoordsLabel->setFrameStyle( QFrame::NoFrame );
    mpCoordsLabel->setText( tr( "Coordinate:" ) );
    mpCoordsLabel->setToolTip( tr( "Current map coordinate" ) );
    mpStatusBar->addPermanentWidget(mpCoordsLabel, 0);

    mpCoordsEdit = new QLineEdit( QString(), mpStatusBar );
    mpCoordsEdit->setObjectName( "mpCoordsEdit" );
    mpCoordsEdit->setFont(myFont);
    mpCoordsEdit->setMinimumWidth(10);
    mpCoordsEdit->setMaximumWidth(300);
    mpCoordsEdit->setMaximumHeight(20);
    mpCoordsEdit->setContentsMargins( 0, 0, 0, 0 );
    mpCoordsEdit->setAlignment( Qt::AlignCenter );
    QRegExp coordValidator( "[+-]?\\d+\\.?\\d*\\s*,\\s*[+-]?\\d+\\.?\\d*" );
    mpCoordsEditValidator = new QRegExpValidator( coordValidator, mpCoordsEdit );

    mpStatusBar->addPermanentWidget( mpCoordsEdit, 0 );

    // add a label to show current scale
    mScaleLabel = new QLabel( QString(), mpStatusBar );
    mScaleLabel->setObjectName( "mScaleLable" );
    mScaleLabel->setFont( myFont );
    mScaleLabel->setMinimumWidth( 10 );
    mScaleLabel->setMaximumHeight( 20 );
    mScaleLabel->setMargin( 3 );
    mScaleLabel->setAlignment( Qt::AlignCenter );
    mScaleLabel->setFrameStyle( QFrame::NoFrame );
    mScaleLabel->setText( tr( "Scale " ) );
    mScaleLabel->setToolTip( tr( "Current map scale" ) );
    statusBar()->addPermanentWidget( mScaleLabel, 0 );

    mScaleEdit = new QgsScaleComboBox( mpStatusBar );
    mScaleEdit->setObjectName( "mScaleEdit" );
    mScaleEdit->setFont( myFont );
    // seems setFont() change font only for popup not for line edit,
    // so we need to set font for it separately
    mScaleEdit->lineEdit()->setFont( myFont );
    mScaleEdit->setMinimumWidth( 10 );
    mScaleEdit->setMaximumWidth( 100 );
    mScaleEdit->setMaximumHeight( 20 );
    mScaleEdit->setContentsMargins( 0, 0, 0, 0 );
    mScaleEdit->setWhatsThis( tr( "Displays the current map scale" ) );
    mScaleEdit->setToolTip( tr( "Current map scale (formatted as x:y)" ) );

    mpStatusBar->addPermanentWidget( mScaleEdit, 0 );
    connect( mScaleEdit, SIGNAL( scaleChanged() ), this, SLOT( userScale() ) );
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
    if(!m_bWithSubwindows)
        return;

    // "theMapLegend" used to find this canonical instance later
    mpMapLegend = new QgsLegend( mpMapCanvas, this, "theMapLegend" );
    mpMapLegend->setWorkingWidget(this);

    mpMapLegend->setWhatsThis( tr( "Map legend that displays all the layers currently on the map canvas. Click on the check box to turn a layer on or off. Double click on a layer in the legend to customize its appearance and set other properties." ) );

    QCheckBox *orderCb = new QCheckBox( tr( "Ручной контроль отрисовки слоев" ) );
    orderCb->setChecked( false );

    connect( orderCb, SIGNAL( toggled( bool ) ), mpMapLegend, SLOT( unsetUpdateDrawingOrder( bool ) ) );
    connect( mpMapLegend, SIGNAL( updateDrawingOrderUnchecked( bool ) ), orderCb, SLOT( setChecked( bool ) ) );
    connect( mpMapLegend, SIGNAL( currentLayerChanged( QgsMapLayer * ) ), this, SLOT( activateDeactivateLayerRelatedActions( QgsMapLayer * ) ) );
    connect( mpMapLegend, SIGNAL( itemSelectionChanged() ), this, SLOT( legendLayerSelectionChanged() ) );
    connect( mpMapLegend, SIGNAL( zOrderChanged() ), this, SLOT( markDirty() ) );

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


    mActionRemoveLayer = new QAction(tr("Remove Layer(s)"), this);
    mActionDuplicateLayer = new QAction(tr("Duplicate Layer(s)"), this);
    mActionSetLayerCRS = new QAction(tr("Set CRS of Layer(s)"), this);
    mActionSetProjectCRSFromLayer = new QAction(tr("Set Project CRS from Layer"), this);
    mActionLayerProperties = new QAction(tr("Properties..."), this);
    mActionLayerSubsetString = new QAction(tr("Query..."), this);
    mActionToggleEditing = new QAction(tr("Toggle Editing"), this);
    mActionSaveLayerEdits = new QAction(tr("Save Layer Edits"), this);
    mActionAllEdits = new QAction(tr("Current Edits"), this);
    /*ksa*/mActionSaveEdits = new QAction(tr("Save Edits"), this);
    /*ksa*/mActionSaveAllEdits = new QAction(tr("Save All Edits"), this);
    mActionCopyStyle = new QAction(tr("Copy style"), this);
    mActionPasteStyle = new QAction(tr("Paste style"), this);
    mActionOpenTable = new QAction(tr("Open Attribute Table"), this);
    mActionLayerSaveAs = new QAction(tr("Save As..."), this);
    mActionLayerSelectionSaveAs = new QAction(tr("Save Selection as Vector File..."), this);

    // Vector edits menu
    QMenu* menuAllEdits = new QMenu( tr( "Current Edits" ), this );
    menuAllEdits->addAction( mActionSaveEdits );
    //menuAllEdits->addAction( mActionRollbackEdits );
    //menuAllEdits->addAction( mActionCancelEdits );
    menuAllEdits->addSeparator();
    menuAllEdits->addAction( mActionSaveAllEdits );
    //menuAllEdits->addAction( mActionRollbackAllEdits );
    //menuAllEdits->addAction( mActionCancelAllEdits );
    mActionAllEdits->setMenu( menuAllEdits );
}


void KKSGISWidgetQGIS::initConnections()
{

  connect( mActionRemoveLayer, SIGNAL( triggered() ), this, SLOT( removeLayer() ) );
  connect( mActionDuplicateLayer, SIGNAL( triggered() ), this, SLOT( duplicateLayers() ) );
  connect( mActionSetLayerCRS, SIGNAL( triggered() ), this, SLOT( setLayerCRS() ) );
  connect( mActionSetProjectCRSFromLayer, SIGNAL( triggered() ), this, SLOT( setProjectCRSFromLayer() ) );
  connect( mActionLayerProperties, SIGNAL( triggered() ), this, SLOT( layerProperties() ) );
  connect( mActionLayerSubsetString, SIGNAL( triggered() ), this, SLOT( layerSubsetString() ) );
  connect( mActionLayerSaveAs, SIGNAL( triggered() ), this, SLOT( saveAsFile() ) );
  connect( mActionLayerSelectionSaveAs, SIGNAL( triggered() ), this, SLOT( saveSelectionAsVectorFile() ) );
  connect( mActionToggleEditing, SIGNAL( triggered() ), this, SLOT( toggleEditing() ) );
  connect( mActionSaveLayerEdits, SIGNAL( triggered() ), this, SLOT( saveActiveLayerEdits() ) );
  connect( mActionSaveEdits, SIGNAL( triggered() ), this, SLOT( saveEdits() ) );
  connect( mActionSaveAllEdits, SIGNAL( triggered() ), this, SLOT( saveAllEdits() ) );
  connect( mActionOpenTable, SIGNAL( triggered() ), this, SLOT( attributeTable() ) );
  connect( mActionCopyStyle, SIGNAL( triggered() ), this, SLOT( copyStyle() ) );
  connect( mActionPasteStyle, SIGNAL( triggered() ), this, SLOT( pasteStyle() ) );

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
    
    if(mpCoordsEdit)
        connect(this->mpCoordsEdit, SIGNAL(editingFinished()), this, SLOT( SLOTazCoordsCenter()));
    
    connect(this->mpActionBathymetry, SIGNAL(triggered()), this, SLOT(SLOTazThemTaskSpectralBathynometry()));

    connect( mpMapCanvas, SIGNAL(xyCoordinates(const QgsPoint &)), this, SLOT(SLOTazShowMouseCoordinate(const QgsPoint &)));
    connect( mpMapCanvas, SIGNAL(xyCoordinates(const QgsPoint &)), this, SLOT(SLOTazShowMouseCoordinate(const QgsPoint &)));
    connect( mpMapCanvas, SIGNAL( extentsChanged() ), this, SLOT( markDirty() ) );
    connect( mpMapCanvas, SIGNAL( layersChanged() ), this, SLOT( markDirty() ) );
    connect( mpMapCanvas->mapRenderer(), SIGNAL( drawingProgress( int, int ) ), this, SLOT( showProgress( int, int ) ) );
    connect( mpMapCanvas, SIGNAL( scaleChanged( double ) ), this, SLOT( showScale( double ) ) );
    connect( mpMapCanvas, SIGNAL( scaleChanged( double ) ), this, SLOT( updateMouseCoordinatePrecision() ) );

    connect(QgsProject::instance(), SIGNAL(readProject(QDomDocument)), mpMapCanvas, SLOT(readProject(QDomDocument)));
    connect( QgsProject::instance(), SIGNAL( layerLoaded( int, int ) ), this, SLOT( showProgress( int, int ) ) );

}


void KKSGISWidgetQGIS::initToolBar()
{
    mpMapToolBar = new QToolBar(tr("Стандартная панель инструментов"));
    if(!m_bWithSubwindows)
        return;

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
    if(!m_bWithSubwindows)
        return;

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

void KKSGISWidgetQGIS::azSetTitleWindow(QWidget & azApp)

{
      QString caption = "ПК Репер";
      QFileInfo projectFileInfo( QgsProject::instance()->fileName() );
      if ( QgsProject::instance()->title().isEmpty() )
      {
        if ( QgsProject::instance()->fileName().isEmpty() )
        {
          // no project title nor file name, so just leave caption with
          // application name and version
        }
        else
        {
            caption += " - " + projectFileInfo.completeBaseName();
        }
      }
      else
      {
          caption += " - " + QgsProject::instance()->title() + " [" + projectFileInfo.fileName() + "]";
      }
      azApp.setWindowTitle( caption );
      emit this->SIGNALchangeWindowTitle();
}

void KKSGISWidgetQGIS::userScale()
{
    if(!mScaleEdit)
        return;

  // Why has MapCanvas the scale inverted?
  mpMapCanvas->zoomScale( 1.0 / mScaleEdit->scale() );
}

void KKSGISWidgetQGIS::showScale( double theScale )
{
    if(!mScaleEdit)
        return;

    // Why has MapCanvas the scale inverted?
    mScaleEdit->setScale( 1.0 / theScale );

    // Not sure if the lines below do anything meaningful /Homann
    if ( mScaleEdit->width() > mScaleEdit->minimumWidth() )
    {
      mScaleEdit->setMinimumWidth( mScaleEdit->width() );
    }
}

void KKSGISWidgetQGIS::updateMouseCoordinatePrecision()
{
  // Work out what mouse display precision to use. This only needs to
  // be when the settings change or the zoom level changes. This
  // function needs to be called every time one of the above happens.

  // Get the display precision from the project settings
  bool automatic = QgsProject::instance()->readBoolEntry( "PositionPrecision", "/Automatic" );
  int dp = 0;

  if ( automatic )
  {
    // Work out a suitable number of decimal places for the mouse
    // coordinates with the aim of always having enough decimal places
    // to show the difference in position between adjacent pixels.
    // Also avoid taking the log of 0.
    if ( mapCanvas()->mapUnitsPerPixel() != 0.0 )
      dp = static_cast<int>( ceil( -1.0 * log10( mapCanvas()->mapUnitsPerPixel() ) ) );
  }
  else
    dp = QgsProject::instance()->readNumEntry( "PositionPrecision", "/DecimalPlaces" );

  // Keep dp sensible
  if ( dp < 0 )
    dp = 0;

  mMousePrecisionDecimalPlaces = dp;
}

void KKSGISWidgetQGIS::removeLayer()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  if ( !mpMapLegend )
  {
    return;
  }

  foreach ( QgsMapLayer * layer, mpMapLegend->selectedLayers() )
  {
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>( layer );
    if ( vlayer && vlayer->isEditable() && !toggleEditing( vlayer, true ) )
      return;
  }

  mpMapLegend->removeSelectedLayers();

  mpMapCanvas->refresh();
}

void KKSGISWidgetQGIS::copyStyle( QgsMapLayer * sourceLayer )
{
  QgsMapLayer *selectionLayer = sourceLayer ? sourceLayer : activeLayer();
  if ( selectionLayer )
  {
    QDomImplementation DomImplementation;
    QDomDocumentType documentType =
      DomImplementation.createDocumentType(
        "qgis", "http://mrcc.com/qgis.dtd", "SYSTEM" );
    QDomDocument doc( documentType );
    QDomElement rootNode = doc.createElement( "qgis" );
    rootNode.setAttribute( "version", QString( "%1" ).arg( QGis::QGIS_VERSION ) );
    doc.appendChild( rootNode );
    QString errorMsg;
    if ( !selectionLayer->writeSymbology( rootNode, doc, errorMsg ) )
    {
      QMessageBox::warning( this,
                            tr( "Error" ),
                            tr( "Cannot copy style: %1" )
                            .arg( errorMsg ),
                            QMessageBox::Ok );
      return;
    }
    // Copies data in text form as well, so the XML can be pasted into a text editor
    clipboard()->setData( QGSCLIPBOARD_STYLE_MIME, doc.toByteArray(), doc.toString() );
    // Enables the paste menu element
    mActionPasteStyle->setEnabled( true );
  }
}

void KKSGISWidgetQGIS::pasteStyle( QgsMapLayer * destinationLayer )
{
  if(!mpMapLegend)
      return;

  QgsMapLayer *selectionLayer = destinationLayer ? destinationLayer : activeLayer();
  if ( selectionLayer )
  {
    if ( clipboard()->hasFormat( QGSCLIPBOARD_STYLE_MIME ) )
    {
      QDomDocument doc( "qgis" );
      QString errorMsg;
      int errorLine, errorColumn;
      if ( !doc.setContent( clipboard()->data( QGSCLIPBOARD_STYLE_MIME ), false, &errorMsg, &errorLine, &errorColumn ) )
      {
        QMessageBox::information( this,
                                  tr( "Error" ),
                                  tr( "Cannot parse style: %1:%2:%3" )
                                  .arg( errorMsg )
                                  .arg( errorLine )
                                  .arg( errorColumn ),
                                  QMessageBox::Ok );
        return;
      }
      QDomElement rootNode = doc.firstChildElement( "qgis" );
      if ( !selectionLayer->readSymbology( rootNode, errorMsg ) )
      {
        QMessageBox::information( this,
                                  tr( "Error" ),
                                  tr( "Cannot read style: %1" )
                                  .arg( errorMsg ),
                                  QMessageBox::Ok );
        return;
      }

      mpMapLegend->refreshLayerSymbology( selectionLayer->id(), false );
      mpMapCanvas->refresh();
    }
  }
}

void KKSGISWidgetQGIS::setLayerCRS()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  if ( !( mpMapLegend && mpMapLegend->currentLayer() ) )
  {
    return;
  }

  QgsGenericProjectionSelector * mySelector = new QgsGenericProjectionSelector( this );
  mySelector->setSelectedCrsId( mpMapLegend->currentLayer()->crs().srsid() );
  mySelector->setMessage();
  if ( mySelector->exec() )
  {
    QgsCoordinateReferenceSystem crs( mySelector->selectedCrsId(), QgsCoordinateReferenceSystem::InternalCrsId );
    mpMapLegend->setCRSForSelectedLayers( crs );
    mpMapCanvas->refresh();
  }
  else
  {
    QApplication::restoreOverrideCursor();
  }

  delete mySelector;
}

void KKSGISWidgetQGIS::setProjectCRSFromLayer()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  if ( !( mpMapLegend && mpMapLegend->currentLayer() ) )
  {
    return;
  }

  QgsCoordinateReferenceSystem crs = mpMapLegend->currentLayer()->crs();
  QgsMapRenderer* myRenderer = mpMapCanvas->mapRenderer();
  mpMapCanvas->freeze();
  myRenderer->setDestinationCrs( crs );
  if ( crs.mapUnits() != QGis::UnknownUnit )
  {
    myRenderer->setMapUnits( crs.mapUnits() );
  }
  mpMapCanvas->freeze( false );
  mpMapCanvas->refresh();
}

void KKSGISWidgetQGIS::duplicateLayers( QList<QgsMapLayer *> lyrList )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  if ( !mpMapLegend )
  {
    return;
  }

  const QList<QgsMapLayer *> selectedLyrs = lyrList.empty() ? mpMapLegend->selectedLayers() : lyrList;
  if ( selectedLyrs.empty() )
  {
    return;
  }

  mpMapCanvas->freeze();
  QgsMapLayer *dupLayer;
  QString layerDupName, unSppType;
  QList<QgsMessageBarItem *> msgBars;

  foreach ( QgsMapLayer * selectedLyr, selectedLyrs )
  {
    dupLayer = 0;
    unSppType = QString( "" );
    layerDupName = selectedLyr->name() + " " + tr( "copy" );

    if ( selectedLyr->type() == QgsMapLayer::PluginLayer )
    {
      unSppType = tr( "Plugin layer" );
    }

    // duplicate the layer's basic parameters

    if ( unSppType.isEmpty() )
    {
      QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>( selectedLyr );
      // TODO: add other layer types that can be duplicated
      // currently memory and plugin layers are skipped
      if ( vlayer && vlayer->storageType() == "Memory storage" )
      {
        unSppType = tr( "Memory layer" );
      }
      else if ( vlayer )
      {
        dupLayer = new QgsVectorLayer( vlayer->source(), layerDupName, vlayer->providerType() );
      }
    }

    if ( unSppType.isEmpty() && !dupLayer )
    {
      QgsRasterLayer *rlayer = qobject_cast<QgsRasterLayer*>( selectedLyr );
      if ( rlayer )
      {
        dupLayer = new QgsRasterLayer( rlayer->source(), layerDupName );
      }
    }

    if ( unSppType.isEmpty() && dupLayer && !dupLayer->isValid() )
    {
      msgBars.append( new QgsMessageBarItem(
                        tr( "Duplicate layer: " ),
                        tr( "%1 (duplication resulted in invalid layer)" ).arg( selectedLyr->name() ) ,
                        QgsMessageBar::WARNING,
                        0,
                        mInfoBar ) );
      continue;
    }

    if ( !unSppType.isEmpty() || !dupLayer )
    {
      msgBars.append( new QgsMessageBarItem(
                        tr( "Duplicate layer: " ),
                        tr( "%1 (%2 type unsupported)" )
                        .arg( selectedLyr->name() )
                        .arg( !unSppType.isEmpty() ? QString( "'" ) + unSppType + "' " : "" ),
                        QgsMessageBar::WARNING,
                        0,
                        mInfoBar ) );
      continue;
    }

    // add layer to layer registry and legend
    QList<QgsMapLayer *> myList;
    myList << dupLayer;
    QgsMapLayerRegistry::instance()->addMapLayers( myList );

    // verify layer has been added to legend
    QgsLegendLayer *duplLayer = 0;
    duplLayer = mpMapLegend->findLegendLayer( dupLayer );
    if ( !duplLayer )
    {
      // some source layers, like items > 4th in a container, have their layer
      // registered but do not show up in the legend, so manually add them
      QgsLegendLayer* llayer = new QgsLegendLayer( dupLayer );
      mpMapLegend->insertTopLevelItem( 0, llayer );
      // double-check, or move of non-existent legend layer will segfault
      duplLayer = mpMapLegend->findLegendLayer( dupLayer );
    }

    QgsLegendLayer *srclLayer = mpMapLegend->findLegendLayer( selectedLyr );
    if ( duplLayer && srclLayer )
    {
      // move layer to just below source layer
      mpMapLegend->moveItem( duplLayer, srclLayer );

      // duplicate the layer style
      copyStyle( selectedLyr );
      pasteStyle( dupLayer );

      QgsVectorLayer* vLayer = dynamic_cast<QgsVectorLayer*>( selectedLyr );
      QgsVectorLayer* vDupLayer = dynamic_cast<QgsVectorLayer*>( dupLayer );
      if ( vLayer && vDupLayer )
      {
        foreach ( const QgsVectorJoinInfo join, vLayer->vectorJoins() )
        {
          vDupLayer->addJoin( join );
        }
      }

      // always set duplicated layers to not visible
      // so layer can be configured before being turned on,
      // and no map canvas refresh needed when doing multiple duplications
      mpMapLegend->setLayerVisible( dupLayer, false );
    }
  }

  dupLayer = 0;

  // update UI
  qApp->processEvents();

  mpMapCanvas->freeze( false );

  // display errors in message bar after duplication of layers
  foreach ( QgsMessageBarItem * msgBar, msgBars )
  {
    mInfoBar->pushItem( msgBar );
  }
}

void KKSGISWidgetQGIS::attributeTable()
{
  if(!mpMapLegend)
    return;
  
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  QgsVectorLayer *myLayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( !myLayer )
  {
    return;
  }

  QgsAttributeTableDialog *mDialog = new QgsAttributeTableDialog( myLayer, this );
  mDialog->show();
  // the dialog will be deleted by itself on close
}


void KKSGISWidgetQGIS::saveAsRasterFile()
{
  QgsRasterLayer* rasterLayer = qobject_cast<QgsRasterLayer *>( activeLayer() );
  if ( !rasterLayer )
  {
    return;
  }

  QgsRasterLayerSaveAsDialog d( rasterLayer, rasterLayer->dataProvider(),
                                mpMapCanvas->extent(), rasterLayer->crs(),
                                mpMapCanvas->mapRenderer()->destinationCrs(),
                                this );
  if ( d.exec() == QDialog::Accepted )
  {
    QgsRasterFileWriter fileWriter( d.outputFileName() );
    if ( d.tileMode() )
    {
      fileWriter.setTiledMode( true );
      fileWriter.setMaxTileWidth( d.maximumTileSizeX() );
      fileWriter.setMaxTileHeight( d.maximumTileSizeY() );
    }

    QProgressDialog pd( 0, tr( "Abort..." ), 0, 0 );
    // Show the dialo immediately because cloning pipe can take some time (WCS)
    pd.setLabelText( tr( "Reading raster" ) );
    pd.show();
    pd.setWindowModality( Qt::WindowModal );

    // TODO: show error dialogs
    // TODO: this code should go somewhere else, but probably not into QgsRasterFileWriter
    // clone pipe/provider is not really necessary, ready for threads
    QgsRasterPipe* pipe = 0;

    if ( d.mode() == QgsRasterLayerSaveAsDialog::RawDataMode )
    {
      QgsDebugMsg( "Writing raw data" );
      //QgsDebugMsg( QString( "Writing raw data" ).arg( pos ) );
      pipe = new QgsRasterPipe();
      if ( !pipe->set( rasterLayer->dataProvider()->clone() ) )
      {
        QgsDebugMsg( "Cannot set pipe provider" );
        return;
      }

      QgsRasterNuller *nuller = new QgsRasterNuller();
      for ( int band = 1; band <= rasterLayer->dataProvider()->bandCount(); band ++ )
      {
        nuller->setNoData( band, d.noData() );
      }
      if ( !pipe->insert( 1, nuller ) )
      {
        QgsDebugMsg( "Cannot set pipe nuller" );
        return;
      }

      // add projector if necessary
      if ( d.outputCrs() != rasterLayer->crs() )
      {
        QgsRasterProjector * projector = new QgsRasterProjector;
        projector->setCRS( rasterLayer->crs(), d.outputCrs() );
        if ( !pipe->insert( 2, projector ) )
        {
          QgsDebugMsg( "Cannot set pipe projector" );
          return;
        }
      }
    }
    else // RenderedImageMode
    {
      // clone the whole pipe
      QgsDebugMsg( "Writing rendered image" );
      pipe = new QgsRasterPipe( *rasterLayer->pipe() );
      QgsRasterProjector *projector = pipe->projector();
      if ( !projector )
      {
        QgsDebugMsg( "Cannot get pipe projector" );
        delete pipe;
        return;
      }
      projector->setCRS( rasterLayer->crs(), d.outputCrs() );
    }

    if ( !pipe->last() )
    {
      delete pipe;
      return;
    }
    fileWriter.setCreateOptions( d.createOptions() );

    fileWriter.setBuildPyramidsFlag( d.buildPyramidsFlag() );
    fileWriter.setPyramidsList( d.pyramidsList() );
    fileWriter.setPyramidsResampling( d.pyramidsResamplingMethod() );
    fileWriter.setPyramidsFormat( d.pyramidsFormat() );
    fileWriter.setPyramidsConfigOptions( d.pyramidsConfigOptions() );

    QgsRasterFileWriter::WriterError err = fileWriter.writeRaster( pipe, d.nColumns(), d.nRows(), d.outputRectangle(), d.outputCrs(), &pd );
    if ( err != QgsRasterFileWriter::NoError )
    {
      QMessageBox::warning( this, tr( "Error" ),
                            tr( "Cannot write raster error code: %1" ).arg( err ),
                            QMessageBox::Ok );

    }
    delete pipe;
  }
}

void KKSGISWidgetQGIS::saveAsFile()
{
  QgsMapLayer* layer = activeLayer();
  if ( !layer )
    return;

  QgsMapLayer::LayerType layerType = layer->type();
  if ( layerType == QgsMapLayer::RasterLayer )
  {
    saveAsRasterFile();
  }
  else if ( layerType == QgsMapLayer::VectorLayer )
  {
    saveAsVectorFileGeneral( false );
  }
}

void KKSGISWidgetQGIS::saveSelectionAsVectorFile()
{
  saveAsVectorFileGeneral( true );
}

void KKSGISWidgetQGIS::saveAsVectorFileGeneral( bool saveOnlySelection, QgsVectorLayer* vlayer, bool symbologyOption )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  if ( !mpMapLegend )
    return;

  if ( !vlayer )
  {
    vlayer = qobject_cast<QgsVectorLayer *>( activeLayer() ); // FIXME: output of multiple layers at once?
  }

  if ( !vlayer )
    return;

  QgsCoordinateReferenceSystem destCRS;

  int options = QgsVectorLayerSaveAsDialog::AllOptions;
  if ( !symbologyOption )
  {
    options &= ~QgsVectorLayerSaveAsDialog::Symbology;
  }

  QgsVectorLayerSaveAsDialog *dialog = new QgsVectorLayerSaveAsDialog( vlayer->crs().srsid(), options, this );

  if ( dialog->exec() == QDialog::Accepted )
  {
    QString encoding = dialog->encoding();
    QString vectorFilename = dialog->filename();
    QString format = dialog->format();
    QStringList datasourceOptions = dialog->datasourceOptions();

    QgsCoordinateTransform* ct = 0;

    switch ( dialog->crs() )
    {
      case -2: // Project CRS
        destCRS = mpMapCanvas->mapRenderer()->destinationCrs();

        break;
      case -1: // Layer CRS
        destCRS = vlayer->crs();
        break;

      default: // Selected CRS
        destCRS = QgsCoordinateReferenceSystem( dialog->crs(), QgsCoordinateReferenceSystem::InternalCrsId );
        break;
    }

    if ( destCRS.isValid() && destCRS != vlayer->crs() )
    {
      ct = new QgsCoordinateTransform( vlayer->crs(), destCRS );

      //ask user about datum transformation
      QList< QList< int > > dt = QgsCoordinateTransform::datumTransformations( vlayer->crs(), destCRS );
      if ( dt.size() > 1 )
      {
        QgsDatumTransformDialog d( vlayer->name(), dt );
        if ( d.exec() == QDialog::Accepted )
        {
          QList< int > sdt = d.selectedDatumTransform();
          if ( sdt.size() > 0 )
          {
            ct->setSourceDatumTransform( sdt.at( 0 ) );
          }
          if ( sdt.size() > 1 )
          {
            ct->setDestinationDatumTransform( sdt.at( 1 ) );
          }
          ct->initialise();
        }
      }
    }

    // ok if the file existed it should be deleted now so we can continue...
    QApplication::setOverrideCursor( Qt::WaitCursor );

    QgsVectorFileWriter::WriterError error;
    QString errorMessage;
    QString newFilename;
    error = QgsVectorFileWriter::writeAsVectorFormat(
              vlayer, vectorFilename, encoding, ct, format,
              saveOnlySelection,
              &errorMessage,
              datasourceOptions, dialog->layerOptions(),
              dialog->skipAttributeCreation(),
              &newFilename,
              ( QgsVectorFileWriter::SymbologyExport )( dialog->symbologyExport() ),
              dialog->scaleDenominator() );

    delete ct;

    QApplication::restoreOverrideCursor();

    if ( error == QgsVectorFileWriter::NoError )
    {
      if ( dialog->addToCanvas() )
      {
        addVectorLayers( QStringList( newFilename ), encoding, "file" );
      }
      messageBar()->pushMessage( tr( "Saving done" ),
                                 tr( "Export to vector file has been completed" ),
                                 QgsMessageBar::INFO, messageTimeout() );
    }
    else
    {
      QgsMessageViewer *m = new QgsMessageViewer( 0 );
      m->setWindowTitle( tr( "Save error" ) );
      m->setMessageAsPlainText( tr( "Export to vector file failed.\nError: %1" ).arg( errorMessage ) );
      m->exec();
    }
  }

  delete dialog;
}

bool KKSGISWidgetQGIS::addVectorLayers( QStringList const & theLayerQStringList, const QString& enc, const QString dataSourceType )
{
  bool wasfrozen = mpMapCanvas->isFrozen();
  QList<QgsMapLayer *> myList;
  foreach ( QString src, theLayerQStringList )
  {
    src = src.trimmed();
    QString base;
    if ( dataSourceType == "file" )
    {
      QFileInfo fi( src );
      base = fi.completeBaseName();

      // if needed prompt for zipitem layers
      QString vsiPrefix = QgsZipItem::vsiPrefix( src );
      if ( ! src.startsWith( "/vsi", Qt::CaseInsensitive ) &&
           ( vsiPrefix == "/vsizip/" || vsiPrefix == "/vsitar/" ) )
      {
        if ( askUserForZipItemLayers( src ) )
          continue;
      }
    }
    else if ( dataSourceType == "database" )
    {
      base = src;
    }
    else //directory //protocol
    {
      QFileInfo fi( src );
      base = fi.completeBaseName();
    }

    QgsDebugMsg( "completeBaseName: " + base );

    // create the layer

    QgsVectorLayer *layer = new QgsVectorLayer( src, base, "ogr" );
    Q_CHECK_PTR( layer );

    if ( ! layer )
    {
      mpMapCanvas->freeze( false );

      // Let render() do its own cursor management
      //      QApplication::restoreOverrideCursor();

      // XXX insert meaningful whine to the user here
      return false;
    }

    if ( layer->isValid() )
    {
      layer->setProviderEncoding( enc );

      QStringList sublayers = layer->dataProvider()->subLayers();
      QgsDebugMsg( QString( "got valid layer with %1 sublayers" ).arg( sublayers.count() ) );

      // If the newly created layer has more than 1 layer of data available, we show the
      // sublayers selection dialog so the user can select the sublayers to actually load.
      if ( sublayers.count() > 1 )
      {
        askUserForOGRSublayers( layer );

        // The first layer loaded is not useful in that case. The user can select it in
        // the list if he wants to load it.
        delete layer;

      }
      else if ( sublayers.count() > 0 ) // there is 1 layer of data available
      {
        //set friendly name for datasources with only one layer
        QStringList sublayers = layer->dataProvider()->subLayers();
        QString ligne = sublayers.at( 0 );
        QStringList elements = ligne.split( ":" );
        layer->setLayerName( elements.at( 1 ) );
        myList << layer;
      }
      else
      {
        QString msg = tr( "%1 doesn't have any layers" ).arg( src );
        messageBar()->pushMessage( tr( "Invalid Data Source" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );
        delete layer;
      }
    }
    else
    {
      QString msg = tr( "%1 is not a valid or recognized data source" ).arg( src );
      messageBar()->pushMessage( tr( "Invalid Data Source" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );

      // since the layer is bad, stomp on it
      delete layer;
    }

  }

  // make sure at least one layer was successfully added
  if ( myList.count() == 0 )
  {
    return false;
  }

  // Register this layer with the layers registry
  QgsMapLayerRegistry::instance()->addMapLayers( myList );

  // update UI
  qApp->processEvents();

  // Only update the map if we frozen in this method
  // Let the caller do it otherwise
  if ( !wasfrozen )
  {
    mpMapCanvas->freeze( false );
    mpMapCanvas->refresh();
  }
// Let render() do its own cursor management
//  QApplication::restoreOverrideCursor();

  // statusBar()->showMessage( mMapCanvas->extent().toString( 2 ) );

  return true;
} 

void KKSGISWidgetQGIS::layerSubsetString()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( !vlayer )
    return;

  // launch the query builder
  QgsQueryBuilder *qb = new QgsQueryBuilder( vlayer, this );
  QString subsetBefore = vlayer->subsetString();

  // Set the sql in the query builder to the same in the prop dialog
  // (in case the user has already changed it)
  qb->setSql( vlayer->subsetString() );
  // Open the query builder
  if ( qb->exec() )
  {
    if ( subsetBefore != qb->sql() )
    {
      mpMapCanvas->refresh();
      if ( mpMapLegend )
      {
        mpMapLegend->refreshLayerSymbology( vlayer->id(), false );
      }
    }
  }

  // delete the query builder object
  delete qb;
}

// restore any application settings stored in QSettings
void KKSGISWidgetQGIS::readSettings()
{
  QSettings settings;
  // get the users theme preference from the settings
  // 'gis' theme is new /themes/default directory (2013-04-15)
  setTheme( settings.value( "/Themes", "default" ).toString() );

  // Add the recently accessed project file paths to the Project menu
  //mRecentProjectPaths = settings.value( "/UI/recentProjectsList" ).toStringList();

  // this is a new session! reset enable macros value to "ask"
  // whether set to "just for this session"
  if ( settings.value( "/qgis/enableMacros", 1 ).toInt() == 2 )
  {
    settings.setValue( "/qgis/enableMacros", 1 );
  }
}

void KKSGISWidgetQGIS::setTheme( QString theThemeName )
{
  /*****************************************************************
  // Init the toolbar icons by setting the icon for each action.
  // All toolbar/menu items must be a QAction in order for this
  // to work.
  //
  // When new toolbar/menu QAction objects are added to the interface,
  // add an entry below to set the icon
  //
  // PNG names must match those defined for the default theme. The
  // default theme is installed in <prefix>/share/qgis/themes/default.
  //
  // New core themes can be added by creating a subdirectory under src/themes
  // and modifying the appropriate CMakeLists.txt files. User contributed themes
  // will be installed directly into <prefix>/share/qgis/themes/<themedir>.
  //
  // Themes can be selected from the preferences dialog. The dialog parses
  // the themes directory and builds a list of themes (ie subdirectories)
  // for the user to choose from.
  //
  */
  QgsApplication::setThemeName( theThemeName );
  //mActionNewProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileNew.svg" ) );
  //mActionOpenProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileOpen.svg" ) );
  //mActionSaveProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileSave.svg" ) );
  //mActionSaveProjectAs->setIcon( QgsApplication::getThemeIcon( "/mActionFileSaveAs.svg" ) );
  
 
  //mActionExit->setIcon( QgsApplication::getThemeIcon( "/mActionFileExit.png" ) );
  //mActionAddOgrLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddOgrLayer.svg" ) );
  //mActionAddRasterLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddRasterLayer.svg" ) );
//#ifdef HAVE_POSTGRESQL
//  mActionAddPgLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddPostgisLayer.svg" ) );
//#endif
  
  mActionRemoveLayer->setIcon( QgsApplication::getThemeIcon( "/mActionRemoveLayer.svg" ) );
  mActionDuplicateLayer->setIcon( QgsApplication::getThemeIcon( "/mActionDuplicateLayer.svg" ) );
  mActionSetLayerCRS->setIcon( QgsApplication::getThemeIcon( "/mActionSetLayerCRS.png" ) );
  mActionSetProjectCRSFromLayer->setIcon( QgsApplication::getThemeIcon( "/mActionSetProjectCRSFromLayer.png" ) );
  //mActionNewVectorLayer->setIcon( QgsApplication::getThemeIcon( "/mActionNewVectorLayer.svg" ) );
  //mActionAddAllToOverview->setIcon( QgsApplication::getThemeIcon( "/mActionAddAllToOverview.svg" ) );
  //mActionHideAllLayers->setIcon( QgsApplication::getThemeIcon( "/mActionHideAllLayers.png" ) );
  //mActionShowAllLayers->setIcon( QgsApplication::getThemeIcon( "/mActionShowAllLayers.png" ) );
  //mActionRemoveAllFromOverview->setIcon( QgsApplication::getThemeIcon( "/mActionRemoveAllFromOverview.svg" ) );
  //mActionToggleFullScreen->setIcon( QgsApplication::getThemeIcon( "/mActionToggleFullScreen.png" ) );
  //mActionProjectProperties->setIcon( QgsApplication::getThemeIcon( "/mActionProjectProperties.png" ) );
  mActionToggleEditing->setIcon( QgsApplication::getThemeIcon( "/mActionToggleEditing.svg" ) );
  mActionSaveLayerEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveAllEdits.svg" ) );
  mActionAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionAllEdits.svg" ) );
  mActionSaveEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveEdits.svg" ) );
  mActionSaveAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveAllEdits.svg" ) );
  //mActionRollbackEdits->setIcon( QgsApplication::getThemeIcon( "/mActionRollbackEdits.svg" ) );
  //mActionRollbackAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionRollbackAllEdits.svg" ) );
  //mActionCancelEdits->setIcon( QgsApplication::getThemeIcon( "/mActionCancelEdits.svg" ) );
  //mActionCancelAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionCancelAllEdits.svg" ) );
  //mActionUndo->setIcon( QgsApplication::getThemeIcon( "/mActionUndo.png" ) );
  //mActionRedo->setIcon( QgsApplication::getThemeIcon( "/mActionRedo.png" ) );
  mActionOpenTable->setIcon( QgsApplication::getThemeIcon( "/mActionOpenTable.png" ) );
  //mActionLabeling->setIcon( QgsApplication::getThemeIcon( "/mActionLabeling.png" ) );

}

// slot to update the progress bar in the status bar
void KKSGISWidgetQGIS::showProgress( int theProgress, int theTotalSteps )
{
  if(!mProgressBar)
      return ;

  if ( theProgress == theTotalSteps )
  {
    mProgressBar->reset();
    mProgressBar->hide();
  }
  else
  {
    //only call show if not already hidden to reduce flicker
    if ( !mProgressBar->isVisible() )
    {
      mProgressBar->show();
    }
    mProgressBar->setMaximum( theTotalSteps );
    mProgressBar->setValue( theProgress );
  }
}

void KKSGISWidgetQGIS::legendLayerSelectionChanged( void )
{
  mActionRemoveLayer->setEnabled( mpMapLegend && mpMapLegend->selectedLayers().size() > 0 );
  mActionDuplicateLayer->setEnabled( mpMapLegend && mpMapLegend->selectedLayers().size() > 0 );
  mActionSetLayerCRS->setEnabled( mpMapLegend && mpMapLegend->selectedLayers().size() > 0 );
  mActionSetProjectCRSFromLayer->setEnabled( mpMapLegend && mpMapLegend->selectedLayers().size() == 1 );

  mActionSaveEdits->setEnabled( mpMapLegend && mpMapLegend->selectedLayersEditable( true ) );
  //mActionRollbackEdits->setEnabled( mMapLegend && mMapLegend->selectedLayersEditable( true ) );
  //mActionCancelEdits->setEnabled( mMapLegend && mMapLegend->selectedLayersEditable() );
}

void KKSGISWidgetQGIS::activateDeactivateLayerRelatedActions( QgsMapLayer* layer )
{
    /*
  bool enableMove = false, enableRotate = false, enablePin = false, enableShowHide = false, enableChange = false;

  QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
  for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); it++ )
  {
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( it.value() );
    if ( !vlayer || !vlayer->isEditable() ||
         ( !vlayer->diagramRenderer() && vlayer->customProperty( "labeling" ).toString() != QString( "pal" ) ) )
      continue;

    int colX, colY, colShow, colAng;
    enablePin =
      enablePin ||
      ( qobject_cast<QgsMapToolPinLabels*>( mMapTools.mPinLabels ) &&
        qobject_cast<QgsMapToolPinLabels*>( mMapTools.mPinLabels )->layerCanPin( vlayer, colX, colY ) );

    enableShowHide =
      enableShowHide ||
      ( qobject_cast<QgsMapToolShowHideLabels*>( mMapTools.mShowHideLabels ) &&
        qobject_cast<QgsMapToolShowHideLabels*>( mMapTools.mShowHideLabels )->layerCanShowHide( vlayer, colShow ) );

    enableMove =
      enableMove ||
      ( qobject_cast<QgsMapToolMoveLabel*>( mMapTools.mMoveLabel ) &&
        ( qobject_cast<QgsMapToolMoveLabel*>( mMapTools.mMoveLabel )->labelMoveable( vlayer, colX, colY )
          || qobject_cast<QgsMapToolMoveLabel*>( mMapTools.mMoveLabel )->diagramMoveable( vlayer, colX, colY ) )
      );

    enableRotate =
      enableRotate ||
      ( qobject_cast<QgsMapToolRotateLabel*>( mMapTools.mRotateLabel ) &&
        qobject_cast<QgsMapToolRotateLabel*>( mMapTools.mRotateLabel )->layerIsRotatable( vlayer, colAng ) );

    enableChange = true;

    if ( enablePin && enableShowHide && enableMove && enableRotate && enableChange )
      break;
  }

  mActionPinLabels->setEnabled( enablePin );
  mActionShowHideLabels->setEnabled( enableShowHide );
  mActionMoveLabel->setEnabled( enableMove );
  mActionRotateLabel->setEnabled( enableRotate );
  mActionChangeLabelProperties->setEnabled( enableChange );
  mMenuPasteAs->setEnabled( clipboard() && !clipboard()->empty() );

  updateLayerModifiedActions();

  if ( !layer )
  {
    mActionSelect->setEnabled( false );
    mActionSelectRectangle->setEnabled( false );
    mActionSelectPolygon->setEnabled( false );
    mActionSelectFreehand->setEnabled( false );
    mActionSelectRadius->setEnabled( false );
    mActionIdentify->setEnabled( QSettings().value( "/Map/identifyMode", 0 ).toInt() != 0 );
    mActionSelectByExpression->setEnabled( false );
    mActionLabeling->setEnabled( false );
    mActionOpenTable->setEnabled( false );
    mActionOpenFieldCalc->setEnabled( false );
    mActionToggleEditing->setEnabled( false );
    mActionToggleEditing->setChecked( false );
    mActionSaveLayerEdits->setEnabled( false );
    mActionLayerSaveAs->setEnabled( false );
    mActionLayerSelectionSaveAs->setEnabled( false );
    mActionLayerProperties->setEnabled( false );
    mActionLayerSubsetString->setEnabled( false );
    mActionAddToOverview->setEnabled( false );
    mActionFeatureAction->setEnabled( false );
    mActionAddFeature->setEnabled( false );
    mActionMoveFeature->setEnabled( false );
    mActionRotateFeature->setEnabled( false );
    mActionOffsetCurve->setEnabled( false );
    mActionNodeTool->setEnabled( false );
    mActionDeleteSelected->setEnabled( false );
    mActionCutFeatures->setEnabled( false );
    mActionCopyFeatures->setEnabled( false );
    mActionPasteFeatures->setEnabled( false );
    mActionCopyStyle->setEnabled( false );
    mActionPasteStyle->setEnabled( false );

    mUndoWidget->dockContents()->setEnabled( false );
    mActionUndo->setEnabled( false );
    mActionRedo->setEnabled( false );
    mActionSimplifyFeature->setEnabled( false );
    mActionAddRing->setEnabled( false );
    mActionAddPart->setEnabled( false );
    mActionDeleteRing->setEnabled( false );
    mActionDeletePart->setEnabled( false );
    mActionReshapeFeatures->setEnabled( false );
    mActionOffsetCurve->setEnabled( false );
    mActionSplitFeatures->setEnabled( false );
    mActionSplitParts->setEnabled( false );
    mActionMergeFeatures->setEnabled( false );
    mActionMergeFeatureAttributes->setEnabled( false );
    mActionRotatePointSymbols->setEnabled( false );

    mActionPinLabels->setEnabled( false );
    mActionShowHideLabels->setEnabled( false );
    mActionMoveLabel->setEnabled( false );
    mActionRotateLabel->setEnabled( false );
    mActionChangeLabelProperties->setEnabled( false );

    mActionLocalHistogramStretch->setEnabled( false );
    mActionFullHistogramStretch->setEnabled( false );
    mActionLocalCumulativeCutStretch->setEnabled( false );
    mActionFullCumulativeCutStretch->setEnabled( false );
    mActionIncreaseBrightness->setEnabled( false );
    mActionDecreaseBrightness->setEnabled( false );
    mActionIncreaseContrast->setEnabled( false );
    mActionDecreaseContrast->setEnabled( false );
    mActionZoomActualSize->setEnabled( false );
    mActionZoomToLayer->setEnabled( false );
    return;
  }

  mActionLayerProperties->setEnabled( QgsProject::instance()->layerIsEmbedded( layer->id() ).isEmpty() );
  mActionAddToOverview->setEnabled( true );
  mActionZoomToLayer->setEnabled( true );

  mActionCopyStyle->setEnabled( true );
  mActionPasteStyle->setEnabled( clipboard()->hasFormat( QGSCLIPBOARD_STYLE_MIME ) );

  // Vector layers
  if ( layer->type() == QgsMapLayer::VectorLayer )
  {
    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
    QgsVectorDataProvider* dprovider = vlayer->dataProvider();

    bool isEditable = vlayer->isEditable();
    bool layerHasSelection = vlayer->selectedFeatureCount() > 0;
    bool layerHasActions = vlayer->actions()->size() > 0;

    bool canChangeAttributes = dprovider->capabilities() & QgsVectorDataProvider::ChangeAttributeValues;
    bool canDeleteFeatures = dprovider->capabilities() & QgsVectorDataProvider::DeleteFeatures;
    bool canAddAttributes = dprovider->capabilities() & QgsVectorDataProvider::AddAttributes;
    bool canAddFeatures = dprovider->capabilities() & QgsVectorDataProvider::AddFeatures;
    bool canSupportEditing = dprovider->capabilities() & QgsVectorDataProvider::EditingCapabilities;
    bool canChangeGeometry = dprovider->capabilities() & QgsVectorDataProvider::ChangeGeometries;

    mActionLocalHistogramStretch->setEnabled( false );
    mActionFullHistogramStretch->setEnabled( false );
    mActionLocalCumulativeCutStretch->setEnabled( false );
    mActionFullCumulativeCutStretch->setEnabled( false );
    mActionIncreaseBrightness->setEnabled( false );
    mActionDecreaseBrightness->setEnabled( false );
    mActionIncreaseContrast->setEnabled( false );
    mActionDecreaseContrast->setEnabled( false );
    mActionZoomActualSize->setEnabled( false );
    mActionLabeling->setEnabled( true );

    mActionSelect->setEnabled( true );
    mActionSelectRectangle->setEnabled( true );
    mActionSelectPolygon->setEnabled( true );
    mActionSelectFreehand->setEnabled( true );
    mActionSelectRadius->setEnabled( true );
    mActionIdentify->setEnabled( true );
    mActionSelectByExpression->setEnabled( true );
    mActionOpenTable->setEnabled( true );
    mActionLayerSaveAs->setEnabled( true );
    mActionLayerSelectionSaveAs->setEnabled( true );
    mActionCopyFeatures->setEnabled( layerHasSelection );
    mActionFeatureAction->setEnabled( layerHasActions );

    if ( !isEditable && mMapCanvas->mapTool()
         && mMapCanvas->mapTool()->isEditTool() && !mSaveRollbackInProgress )
    {
      mMapCanvas->setMapTool( mNonEditMapTool );
    }

    if ( dprovider )
    {
      mActionLayerSubsetString->setEnabled( !isEditable && dprovider->supportsSubsetString() );

      mActionToggleEditing->setEnabled( canSupportEditing && !vlayer->isReadOnly() );
      mActionToggleEditing->setChecked( canSupportEditing && isEditable );
      mActionSaveLayerEdits->setEnabled( canSupportEditing && isEditable && vlayer->isModified() );
      mUndoWidget->dockContents()->setEnabled( canSupportEditing && isEditable );
      mActionUndo->setEnabled( canSupportEditing );
      mActionRedo->setEnabled( canSupportEditing );

      //start editing/stop editing
      if ( canSupportEditing )
      {
        updateUndoActions();
      }

      mActionPasteFeatures->setEnabled( isEditable && canAddFeatures && !clipboard()->empty() );
      mActionAddFeature->setEnabled( isEditable && canAddFeatures );

      //does provider allow deleting of features?
      mActionDeleteSelected->setEnabled( isEditable && canDeleteFeatures && layerHasSelection );
      mActionCutFeatures->setEnabled( isEditable && canDeleteFeatures && layerHasSelection );

      //merge tool needs editable layer and provider with the capability of adding and deleting features
      if ( isEditable && canChangeAttributes )
      {
        mActionMergeFeatures->setEnabled( layerHasSelection && canDeleteFeatures && canAddFeatures );
        mActionMergeFeatureAttributes->setEnabled( layerHasSelection );
      }
      else
      {
        mActionMergeFeatures->setEnabled( false );
        mActionMergeFeatureAttributes->setEnabled( false );
      }

      // moving enabled if geometry changes are supported
      mActionAddPart->setEnabled( isEditable && canChangeGeometry );
      mActionDeletePart->setEnabled( isEditable && canChangeGeometry );
      mActionMoveFeature->setEnabled( isEditable && canChangeGeometry );
      mActionRotateFeature->setEnabled( isEditable && canChangeGeometry );
      mActionNodeTool->setEnabled( isEditable && canChangeGeometry );

      if ( vlayer->geometryType() == QGis::Point )
      {
        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePoint.png" ) );

        mActionAddRing->setEnabled( false );
        mActionReshapeFeatures->setEnabled( false );
        mActionSplitFeatures->setEnabled( false );
        mActionSplitParts->setEnabled( false );
        mActionSimplifyFeature->setEnabled( false );
        mActionDeleteRing->setEnabled( false );
        mActionRotatePointSymbols->setEnabled( false );
        mActionOffsetCurve->setEnabled( false );

        if ( isEditable && canChangeAttributes )
        {
          if ( QgsMapToolRotatePointSymbols::layerIsRotatable( vlayer ) )
          {
            mActionRotatePointSymbols->setEnabled( true );
          }
        }
      }
      else if ( vlayer->geometryType() == QGis::Line )
      {
        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCaptureLine.png" ) );

        mActionReshapeFeatures->setEnabled( isEditable && canAddFeatures );
        mActionSplitFeatures->setEnabled( isEditable && canAddFeatures );
        mActionSplitParts->setEnabled( isEditable && canAddFeatures );
        mActionSimplifyFeature->setEnabled( isEditable && canAddFeatures );
        mActionOffsetCurve->setEnabled( isEditable && canAddFeatures && canChangeAttributes );

        mActionAddRing->setEnabled( false );
        mActionDeleteRing->setEnabled( false );
      }
      else if ( vlayer->geometryType() == QGis::Polygon )
      {
        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePolygon.png" ) );

        mActionAddRing->setEnabled( isEditable && canAddFeatures );
        mActionReshapeFeatures->setEnabled( isEditable && canAddFeatures );
        mActionSplitFeatures->setEnabled( isEditable && canAddFeatures );
        mActionSplitParts->setEnabled( isEditable && canAddFeatures );
        mActionSimplifyFeature->setEnabled( isEditable && canAddFeatures );
        mActionDeleteRing->setEnabled( isEditable && canAddFeatures );
        mActionOffsetCurve->setEnabled( false );
      }

      mActionOpenFieldCalc->setEnabled( isEditable && ( canChangeAttributes || canAddAttributes ) );

      return;
    }
    else
    {
      mUndoWidget->dockContents()->setEnabled( false );
      mActionUndo->setEnabled( false );
      mActionRedo->setEnabled( false );
    }

    mActionLayerSubsetString->setEnabled( false );
  } //end vector layer block
  
  //Raster layers 
  else if ( layer->type() == QgsMapLayer::RasterLayer )
  {
    const QgsRasterLayer *rlayer = qobject_cast<const QgsRasterLayer *>( layer );
    if ( rlayer->dataProvider()->dataType( 1 ) != QGis::ARGB32
         && rlayer->dataProvider()->dataType( 1 ) != QGis::ARGB32_Premultiplied )
    {
      if ( rlayer->dataProvider()->capabilities() & QgsRasterDataProvider::Size )
      {
        mActionFullHistogramStretch->setEnabled( true );
      }
      else
      {
        // it would hang up reading the data for WMS for example
        mActionFullHistogramStretch->setEnabled( false );
      }
      mActionLocalHistogramStretch->setEnabled( true );
    }
    else
    {
      mActionLocalHistogramStretch->setEnabled( false );
      mActionFullHistogramStretch->setEnabled( false );
    }

    mActionLocalCumulativeCutStretch->setEnabled( true );
    mActionFullCumulativeCutStretch->setEnabled( true );
    mActionIncreaseBrightness->setEnabled( true );
    mActionDecreaseBrightness->setEnabled( true );
    mActionIncreaseContrast->setEnabled( true );
    mActionDecreaseContrast->setEnabled( true );

    mActionLayerSubsetString->setEnabled( false );
    mActionFeatureAction->setEnabled( false );
    mActionSelect->setEnabled( false );
    mActionSelectRectangle->setEnabled( false );
    mActionSelectPolygon->setEnabled( false );
    mActionSelectFreehand->setEnabled( false );
    mActionSelectRadius->setEnabled( false );
    mActionZoomActualSize->setEnabled( true );
    mActionOpenTable->setEnabled( false );
    mActionOpenFieldCalc->setEnabled( false );
    mActionToggleEditing->setEnabled( false );
    mActionToggleEditing->setChecked( false );
    mActionSaveLayerEdits->setEnabled( false );
    mUndoWidget->dockContents()->setEnabled( false );
    mActionUndo->setEnabled( false );
    mActionRedo->setEnabled( false );
    mActionLayerSaveAs->setEnabled( true );
    mActionLayerSelectionSaveAs->setEnabled( false );
    mActionAddFeature->setEnabled( false );
    mActionDeleteSelected->setEnabled( false );
    mActionAddRing->setEnabled( false );
    mActionAddPart->setEnabled( false );
    mActionNodeTool->setEnabled( false );
    mActionMoveFeature->setEnabled( false );
    mActionRotateFeature->setEnabled( false );
    mActionOffsetCurve->setEnabled( false );
    mActionCopyFeatures->setEnabled( false );
    mActionCutFeatures->setEnabled( false );
    mActionPasteFeatures->setEnabled( false );
    mActionRotatePointSymbols->setEnabled( false );
    mActionDeletePart->setEnabled( false );
    mActionDeleteRing->setEnabled( false );
    mActionSimplifyFeature->setEnabled( false );
    mActionReshapeFeatures->setEnabled( false );
    mActionSplitFeatures->setEnabled( false );
    mActionSplitParts->setEnabled( false );
    mActionLabeling->setEnabled( false );

    //NOTE: This check does not really add any protection, as it is called on load not on layer select/activate
    //If you load a layer with a provider and idenitfy ability then load another without, the tool would be disabled for both

    //Enable the Identify tool ( GDAL datasets draw without a provider )
    //but turn off if data provider exists and has no Identify capabilities
    mActionIdentify->setEnabled( true );

    QSettings settings;
    int identifyMode = settings.value( "/Map/identifyMode", 0 ).toInt();
    if ( identifyMode == 0 )
    {
      const QgsRasterLayer *rlayer = qobject_cast<const QgsRasterLayer *>( layer );
      const QgsRasterDataProvider* dprovider = rlayer->dataProvider();
      if ( dprovider )
      {
        // does provider allow the identify map tool?
        if ( dprovider->capabilities() & QgsRasterDataProvider::Identify )
        {
          mActionIdentify->setEnabled( true );
        }
        else
        {
          mActionIdentify->setEnabled( false );
        }
      }
    }
  }
  */
}


// present a dialog to choose zipitem layers
bool KKSGISWidgetQGIS::askUserForZipItemLayers( QString path )
{
  bool ok = false;
  QVector<QgsDataItem*> childItems;
  QgsZipItem *zipItem = 0;
  QSettings settings;
  int promptLayers = settings.value( "/qgis/promptForRasterSublayers", 1 ).toInt();

  QgsDebugMsg( "askUserForZipItemLayers( " + path + ")" );

  // if scanZipBrowser == no: skip to the next file
  if ( settings.value( "/qgis/scanZipInBrowser2", "basic" ).toString() == "no" )
  {
    return false;
  }

  zipItem = new QgsZipItem( 0, path, path );
  if ( ! zipItem )
    return false;

  zipItem->populate();
  QgsDebugMsg( QString( "Path= %1 got zipitem with %2 children" ).arg( path ).arg( zipItem->rowCount() ) );

  // if 1 or 0 child found, exit so a normal item is created by gdal or ogr provider
  if ( zipItem->rowCount() <= 1 )
  {
    delete zipItem;
    return false;
  }

  // if promptLayers=Load all, load all layers without prompting
  if ( promptLayers == 3 )
  {
    childItems = zipItem->children();
  }
  // exit if promptLayers=Never
  else if ( promptLayers == 2 )
  {
    delete zipItem;
    return false;
  }
  else
  {
    // We initialize a selection dialog and display it.
    QgsSublayersDialog chooseSublayersDialog( QgsSublayersDialog::Vsifile, "vsi", this );

    QStringList layers;
    for ( int i = 0; i < zipItem->children().size(); i++ )
    {
      QgsDataItem *item = zipItem->children()[i];
      QgsLayerItem *layerItem = dynamic_cast<QgsLayerItem *>( item );
      QgsDebugMsgLevel( QString( "item path=%1 provider=%2" ).arg( item->path() ).arg( layerItem->providerKey() ), 2 );
      if ( layerItem && layerItem->providerKey() == "gdal" )
      {
        layers << QString( "%1|%2|%3" ).arg( i ).arg( item->name() ).arg( "Raster" );
      }
      else if ( layerItem && layerItem->providerKey() == "ogr" )
      {
        layers << QString( "%1|%2|%3" ).arg( i ).arg( item->name() ).arg( tr( "Vector" ) );
      }
    }

    chooseSublayersDialog.populateLayerTable( layers, "|" );

    if ( chooseSublayersDialog.exec() )
    {
      foreach ( int i, chooseSublayersDialog.selectionIndexes() )
      {
        childItems << zipItem->children()[i];
      }
    }
  }

  if ( childItems.isEmpty() )
  {
    // return true so dialog doesn't popup again (#6225) - hopefully this doesn't create other trouble
    ok = true;
  }

  // add childItems
  foreach ( QgsDataItem* item, childItems )
  {
    QgsLayerItem *layerItem = dynamic_cast<QgsLayerItem *>( item );
    QgsDebugMsg( QString( "item path=%1 provider=%2" ).arg( item->path() ).arg( layerItem->providerKey() ) );
    if ( layerItem && layerItem->providerKey() == "gdal" )
    {
      if ( addRasterLayer( item->path(), QFileInfo( item->name() ).completeBaseName() ) )
        ok = true;
    }
    else if ( layerItem && layerItem->providerKey() == "ogr" )
    {
      if ( addVectorLayers( QStringList( item->path() ), "System", "file" ) )
        ok = true;
    }
  }

  delete zipItem;
  return ok;
}

// present a dialog to choose GDAL raster sublayers
void KKSGISWidgetQGIS::askUserForGDALSublayers( QgsRasterLayer *layer )
{
  if ( !layer )
    return;

  QStringList sublayers = layer->subLayers();
  QgsDebugMsg( QString( "raster has %1 sublayers" ).arg( layer->subLayers().size() ) );

  if ( sublayers.size() < 1 )
    return;

  // if promptLayers=Load all, load all sublayers without prompting
  QSettings settings;
  if ( settings.value( "/qgis/promptForRasterSublayers", 1 ).toInt() == 3 )
  {
    loadGDALSublayers( layer->source(), sublayers );
    return;
  }

  // We initialize a selection dialog and display it.
  QgsSublayersDialog chooseSublayersDialog( QgsSublayersDialog::Gdal, "gdal", this );

  QStringList layers;
  QStringList names;
  for ( int i = 0; i < sublayers.size(); i++ )
  {
    // simplify raster sublayer name - should add a function in gdal provider for this?
    // code is copied from QgsGdalLayerItem::createChildren
    QString name = sublayers[i];
    QString path = layer->source();
    // if netcdf/hdf use all text after filename
    // for hdf4 it would be best to get description, because the subdataset_index is not very practical
    if ( name.startsWith( "netcdf", Qt::CaseInsensitive ) ||
         name.startsWith( "hdf", Qt::CaseInsensitive ) )
      name = name.mid( name.indexOf( path ) + path.length() + 1 );
    else
    {
      // remove driver name and file name
      name.replace( name.split( ":" )[0], "" );
      name.replace( path, "" );
    }
    // remove any : or " left over
    if ( name.startsWith( ":" ) )
      name.remove( 0, 1 );

    if ( name.startsWith( "\"" ) )
      name.remove( 0, 1 );

    if ( name.endsWith( ":" ) )
      name.chop( 1 );

    if ( name.endsWith( "\"" ) )
      name.chop( 1 );

    names << name;
    layers << QString( "%1|%2" ).arg( i ).arg( name );
  }

  chooseSublayersDialog.populateLayerTable( layers, "|" );

  if ( chooseSublayersDialog.exec() )
  {
    foreach ( int i, chooseSublayersDialog.selectionIndexes() )
    {
      QgsRasterLayer *rlayer = new QgsRasterLayer( sublayers[i], names[i] );
      if ( rlayer && rlayer->isValid() )
      {
        addRasterLayer( rlayer );
      }
    }
  }
}

// This method is the method that does the real job. If the layer given in
// parameter is NULL, then the method tries to act on the activeLayer.
void KKSGISWidgetQGIS::askUserForOGRSublayers( QgsVectorLayer *layer )
{
  if ( !layer )
  {
    layer = qobject_cast<QgsVectorLayer *>( activeLayer() );
    if ( !layer || layer->dataProvider()->name() != "ogr" )
      return;
  }

  QStringList sublayers = layer->dataProvider()->subLayers();
  QString layertype = layer->dataProvider()->storageType();

  // We initialize a selection dialog and display it.
  QgsSublayersDialog chooseSublayersDialog( QgsSublayersDialog::Ogr, "ogr", this );
  chooseSublayersDialog.populateLayerTable( sublayers );

  if ( chooseSublayersDialog.exec() )
  {
    QString uri = layer->source();
    //the separator char & was changed to | to be compatible
    //with url for protocol drivers
    if ( uri.contains( '|', Qt::CaseSensitive ) )
    {
      // If we get here, there are some options added to the filename.
      // A valid uri is of the form: filename&option1=value1&option2=value2,...
      // We want only the filename here, so we get the first part of the split.
      QStringList theURIParts = uri.split( "|" );
      uri = theURIParts.at( 0 );
    }
    QgsDebugMsg( "Layer type " + layertype );
    // the user has done his choice
    loadOGRSublayers( layertype , uri, chooseSublayersDialog.selectionNames() );
  }
}

// this is a slot for action from GUI to add raster layer
void KKSGISWidgetQGIS::addRasterLayer()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  QString fileFilters;

  QStringList selectedFiles;
  QString e;//only for parameter correctness
  QString title = tr( "Open a GDAL Supported Raster Data Source" );
  QgisGui::openFilesRememberingFilter( "lastRasterFileFilter", mRasterFileFilter, selectedFiles, e,
                                       title );

  if ( selectedFiles.isEmpty() )
  {
    // no files were selected, so just bail
    return;
  }

  addRasterLayers( selectedFiles );

}

//
// This is the method that does the actual work of adding a raster layer - the others
// here simply create a raster layer object and delegate here. It is the responsibility
// of the calling method to manage things such as the frozen state of the mapcanvas and
// using waitcursors etc. - this method won't and SHOULDN'T do it
//
bool KKSGISWidgetQGIS::addRasterLayer( QgsRasterLayer *theRasterLayer )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return false;
  }

  Q_CHECK_PTR( theRasterLayer );

  if ( ! theRasterLayer )
  {
    // XXX insert meaningful whine to the user here; although be
    // XXX mindful that a null layer may mean exhausted memory resources
    return false;
  }

  if ( !theRasterLayer->isValid() )
  {
    delete theRasterLayer;
    return false;
  }

  // register this layer with the central layers registry
  QList<QgsMapLayer *> myList;
  myList << theRasterLayer;
  QgsMapLayerRegistry::instance()->addMapLayers( myList );

  return true;
}

// This method will load with OGR the layers  in parameter.
// This method has been conceived to use the new URI
// format of the ogrprovider so as to give precisions about which
// sublayer to load into QGIS. It is normally triggered by the
// sublayer selection dialog.
void KKSGISWidgetQGIS::loadOGRSublayers( QString layertype, QString uri, QStringList list )
{
  // The uri must contain the actual uri of the vectorLayer from which we are
  // going to load the sublayers.
  QString fileName = QFileInfo( uri ).baseName();
  QList<QgsMapLayer *> myList;
  for ( int i = 0; i < list.size(); i++ )
  {
    QString composedURI;
    QString layerName = list.at( i ).split( ':' ).value( 0 );
    QString layerType = list.at( i ).split( ':' ).value( 1 );

    if ( layertype != "GRASS" )
    {
      composedURI = uri + "|layername=" + layerName;
    }
    else
    {
      composedURI = uri + "|layerindex=" + layerName;
    }

    if ( !layerType.isEmpty() )
    {
      composedURI += "|geometrytype=" + layerType;
    }

    // addVectorLayer( composedURI,  list.at( i ), "ogr" );

    QgsDebugMsg( "Creating new vector layer using " + composedURI );
    QString name = list.at( i );
    name.replace( ":", " " );
    QgsVectorLayer *layer = new QgsVectorLayer( composedURI, name, "ogr" );
    if ( layer && layer->isValid() )
    {
      myList << layer;
    }
    else
    {
      QString msg = tr( "%1 is not a valid or recognized data source" ).arg( composedURI );
      messageBar()->pushMessage( tr( "Invalid Data Source" ), msg, QgsMessageBar::CRITICAL, messageTimeout() );
      if ( layer )
        delete layer;
    }
  }

  if ( ! myList.isEmpty() )
  {
    // Register layer(s) with the layers registry
    QgsMapLayerRegistry::instance()->addMapLayers( myList );
  }
}

// Open a raster layer - this is the generic function which takes all parameters
// this method is a blend of addRasterLayer() functions (with and without provider)
// and addRasterLayers()
QgsRasterLayer* KKSGISWidgetQGIS::addRasterLayerPrivate(const QString & uri, 
                                                        const QString & baseName, 
                                                        const QString & providerKey,
                                                        bool guiWarning, 
                                                        bool guiUpdate )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return NULL;
  }

  if ( guiUpdate )
  {
    // let the user know we're going to possibly be taking a while
    // QApplication::setOverrideCursor( Qt::WaitCursor );
    mpMapCanvas->freeze( true );
  }

  QgsDebugMsg( "Creating new raster layer using " + uri
               + " with baseName of " + baseName );

  QgsRasterLayer *layer = 0;
  // XXX ya know QgsRasterLayer can snip out the basename on its own;
  // XXX why do we have to pass it in for it?
  // ET : we may not be getting "normal" files here, so we still need the baseName argument
  if ( providerKey.isEmpty() )
    layer = new QgsRasterLayer( uri, baseName ); // fi.completeBaseName());
  else
    layer = new QgsRasterLayer( uri, baseName, providerKey );

  QgsDebugMsg( "Constructed new layer" );

  QgsError error;
  QString title;
  bool ok = false;

  if ( !layer->isValid() )
  {
    error = layer->error();
    title = tr( "Invalid Layer" );

    if ( shouldAskUserForGDALSublayers( layer ) )
    {
      askUserForGDALSublayers( layer );
      ok = true;

      // The first layer loaded is not useful in that case. The user can select it in
      // the list if he wants to load it.
      delete layer;
      layer = NULL;
    }
  }
  else
  {
    ok = addRasterLayer( layer );
    if ( !ok )
    {
      error.append( QGS_ERROR_MESSAGE( tr( "Error adding valid layer to map canvas" ),
                                       tr( "Raster layer" ) ) );
      title = tr( "Error" );
    }
  }

  if ( !ok )
  {
    if ( guiUpdate )
      mpMapCanvas->freeze( false );

    // don't show the gui warning if we are loading from command line
    if ( guiWarning )
    {
      QgsErrorDialog::show( error, title );
    }

    if ( layer )
    {
      delete layer;
      layer = NULL;
    }
  }

  if ( guiUpdate )
  {
    // update UI
    qApp->processEvents();
    // draw the map
    mpMapCanvas->freeze( false );
    mpMapCanvas->refresh();
    //update status
    statusBar()->showMessage( mpMapCanvas->extent().toString( 2 ) );
    // Let render() do its own cursor management
    //    QApplication::restoreOverrideCursor();
  }

  return layer;

} // QgisApp::addRasterLayer


//create a raster layer object and delegate to addRasterLayer(QgsRasterLayer *)
QgsRasterLayer* KKSGISWidgetQGIS::addRasterLayer(QString const & rasterFile, 
                                                 QString const & baseName, 
                                                 bool guiWarning )
{
  return addRasterLayerPrivate( rasterFile, baseName, QString(), guiWarning, true );
}

// should the GDAL sublayers dialog should be presented to the user?
bool KKSGISWidgetQGIS::shouldAskUserForGDALSublayers( QgsRasterLayer *layer )
{
  // return false if layer is empty or raster has no sublayers
  if ( !layer || layer->providerType() != "gdal" || layer->subLayers().size() < 1 )
    return false;

  QSettings settings;
  int promptLayers = settings.value( "/qgis/promptForRasterSublayers", 1 ).toInt();
  // 0 = Always -> always ask (if there are existing sublayers)
  // 1 = If needed -> ask if layer has no bands, but has sublayers
  // 2 = Never -> never prompt, will not load anything
  // 3 = Load all -> never prompt, but load all sublayers

  return promptLayers == 0 || promptLayers == 3 || ( promptLayers == 1 && layer->bandCount() == 0 );
}

// This method will load with GDAL the layers in parameter.
// It is normally triggered by the sublayer selection dialog.
void KKSGISWidgetQGIS::loadGDALSublayers( QString uri, QStringList list )
{
  QString path, name;
  QgsRasterLayer *subLayer = NULL;

  //add layers in reverse order so they appear in the right order in the layer dock
  for ( int i = list.size() - 1; i >= 0 ; i-- )
  {
    path = list[i];
    // shorten name by replacing complete path with filename
    name = path;
    name.replace( uri, QFileInfo( uri ).completeBaseName() );
    subLayer = new QgsRasterLayer( path, name );
    if ( subLayer )
    {
      if ( subLayer->isValid() )
        addRasterLayer( subLayer );
      else
        delete subLayer;
    }

  }
}

//create a raster layer object and delegate to addRasterLayer(QgsRasterLayer *)
bool KKSGISWidgetQGIS::addRasterLayers( QStringList const &theFileNameQStringList, bool guiWarning )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return false;
  }

  if ( theFileNameQStringList.empty() )
  {
    // no files selected so bail out, but
    // allow mMapCanvas to handle events
    // first
    mpMapCanvas->freeze( false );
    return false;
  }

  mpMapCanvas->freeze( true );

  // this is messy since some files in the list may be rasters and others may
  // be ogr layers. We'll set returnValue to false if one or more layers fail
  // to load.
  bool returnValue = true;
  for ( QStringList::ConstIterator myIterator = theFileNameQStringList.begin();
        myIterator != theFileNameQStringList.end();
        ++myIterator )
  {
    QString errMsg;
    bool ok = false;

    // if needed prompt for zipitem layers
    QString vsiPrefix = QgsZipItem::vsiPrefix( *myIterator );
    if ( ! myIterator->startsWith( "/vsi", Qt::CaseInsensitive ) &&
         ( vsiPrefix == "/vsizip/" || vsiPrefix == "/vsitar/" ) )
    {
      if ( askUserForZipItemLayers( *myIterator ) )
        continue;
    }

    if ( QgsRasterLayer::isValidRasterFileName( *myIterator, errMsg ) )
    {
      QFileInfo myFileInfo( *myIterator );
      // get the directory the .adf file was in
      QString myDirNameQString = myFileInfo.path();
      //extract basename
      QString myBaseNameQString = myFileInfo.completeBaseName();
      //only allow one copy of a ai grid file to be loaded at a
      //time to prevent the user selecting all adfs in 1 dir which
      //actually represent 1 coverage,

      // try to create the layer
      QgsRasterLayer *layer = addRasterLayerPrivate( *myIterator, 
                                                      myBaseNameQString,
                                                      QString(), 
                                                      guiWarning, 
                                                      true );
      if ( layer && layer->isValid() )
      {
        //only allow one copy of a ai grid file to be loaded at a
        //time to prevent the user selecting all adfs in 1 dir which
        //actually represent 1 coverate,

        if ( myBaseNameQString.toLower().endsWith( ".adf" ) )
        {
          break;
        }
      }
      // if layer is invalid addRasterLayerPrivate() will show the error

    } // valid raster filename
    else
    {
      ok = false;

      // Issue message box warning unless we are loading from cmd line since
      // non-rasters are passed to this function first and then successfully
      // loaded afterwards (see main.cpp)
      if ( guiWarning )
      {
        QgsError error;
        QString msg;

        msg = tr( "%1 is not a supported raster data source" ).arg( *myIterator );
        if ( errMsg.size() > 0 )
          msg += "\n" + errMsg;
        error.append( QGS_ERROR_MESSAGE( msg, tr( "Raster layer" ) ) );

        QgsErrorDialog::show( error, tr( "Unsupported Data Source" ) );
      }
    }
    if ( ! ok )
    {
      returnValue = false;
    }
  }

  statusBar()->showMessage( mpMapCanvas->extent().toString( 2 ) );
  mpMapCanvas->freeze( false );
  mpMapCanvas->refresh();

// Let render() do its own cursor management
//  QApplication::restoreOverrideCursor();

  return returnValue;

}

QgsClipboard * KKSGISWidgetQGIS::clipboard()
{
  return mInternalClipboard;
}

void KKSGISWidgetQGIS::clipboardChanged()
{
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void KKSGISWidgetQGIS::saveActiveLayerEdits()
{
  saveEdits( activeLayer(), true, true );
}

void KKSGISWidgetQGIS::saveEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer || !vlayer->isEditable() || !vlayer->isModified() )
    return;

  //ksa if ( vlayer == activeLayer() )
  //ksa   mSaveRollbackInProgress = true;

  if ( !vlayer->commitChanges() )
  {
    //ksa mSaveRollbackInProgress = false;
    //ksa commitError( vlayer );
  }

  if ( leaveEditable )
  {
    vlayer->startEditing();
  }
  if ( triggerRepaint )
  {
    vlayer->triggerRepaint();
  }
}

void KKSGISWidgetQGIS::saveEdits()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  if(!mpMapLegend)
      return;

  foreach ( QgsMapLayer * layer, mpMapLegend->selectedLayers() )
  {
    saveEdits( layer, true, false );
  }

  mpMapCanvas->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void KKSGISWidgetQGIS::saveAllEdits( bool verifyAction )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  if ( verifyAction )
  {
    if ( !verifyEditsActionDialog( tr( "Save" ), tr( "all" ) ) )
      return;
  }

  foreach ( QgsMapLayer * layer, editableLayers( true ) )
  {
    saveEdits( layer, true, false );
  }
  mpMapCanvas->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void KKSGISWidgetQGIS::editCopy( QgsMapLayer * layerContainingSelection )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  QgsVectorLayer* selectionVectorLayer = qobject_cast<QgsVectorLayer *>( layerContainingSelection ? layerContainingSelection : activeLayer() );
  if ( !selectionVectorLayer )
    return;

  // Test for feature support in this layer
  clipboard()->replaceWithCopyOf( selectionVectorLayer );
}

void KKSGISWidgetQGIS::editPaste( QgsMapLayer *destinationLayer )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  QgsVectorLayer *pasteVectorLayer = qobject_cast<QgsVectorLayer *>( destinationLayer ? destinationLayer : activeLayer() );
  if ( !pasteVectorLayer )
    return;

  pasteVectorLayer->beginEditCommand( tr( "Features pasted" ) );
  QgsFeatureList features;
  if ( mpMapCanvas->mapRenderer()->hasCrsTransformEnabled() )
  {
    features = clipboard()->transformedCopyOf( pasteVectorLayer->crs(), pasteVectorLayer->pendingFields() );
  }
  else
  {
    features = clipboard()->copyOf( pasteVectorLayer->pendingFields() );
  }

  QHash<int, int> remap;
  const QgsFields &fields = clipboard()->fields();
  QgsAttributeList pkAttrList = pasteVectorLayer->pendingPkAttributesList();
  for ( int idx = 0; idx < fields.count(); ++idx )
  {
    int dst = pasteVectorLayer->fieldNameIndex( fields[idx].name() );
    if ( dst < 0 || pkAttrList.contains( dst ) )
    {
      // skip primary key attributes
      continue;
    }
    remap.insert( idx, dst );
  }

  int dstAttrCount = pasteVectorLayer->pendingFields().count();
  for ( int i = 0; i < features.size(); i++ )
  {
    QgsFeature &f = features[i];
    const QgsAttributes &srcAttr = f.attributes();
    QgsAttributes dstAttr( dstAttrCount );

    for ( int src = 0; src < srcAttr.count(); ++src )
    {
      int dst = remap.value( src, -1 );
      if ( dst < 0 )
        continue;

      dstAttr[ dst ] = srcAttr[ src ];
    }

    f.setAttributes( dstAttr );

    //avoid intersection if enabled in digitize settings
    if ( f.geometry() )
    {
      f.geometry()->avoidIntersections();
    }
  }

  pasteVectorLayer->addFeatures( features );
  pasteVectorLayer->endEditCommand();
  mpMapCanvas->refresh();
}

void KKSGISWidgetQGIS::deleteSelected( QgsMapLayer *layer, QWidget* parent )
{
  if(!mpMapLegend)
      return;

  if ( !layer )
  {
    layer = mpMapLegend->currentLayer();
  }

  if ( !parent )
  {
    parent = this;
  }

  if ( !layer )
  {
    messageBar()->pushMessage( tr( "No Layer Selected" ),
                               tr( "To delete features, you must select a vector layer in the legend" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer )
  {
    messageBar()->pushMessage( tr( "No Vector Layer Selected" ),
                               tr( "Deleting features only works on vector layers" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  if ( !( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures ) )
  {
    messageBar()->pushMessage( tr( "Provider does not support deletion" ),
                               tr( "Data provider does not support deleting features" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  if ( !vlayer->isEditable() )
  {
    messageBar()->pushMessage( tr( "Layer not editable" ),
                               tr( "The current layer is not editable. Choose 'Start editing' in the digitizing toolbar." ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }

  //display a warning
  int numberOfDeletedFeatures = vlayer->selectedFeaturesIds().size();
  if ( QMessageBox::warning( parent, tr( "Delete features" ), tr( "Delete %n feature(s)?", "number of features to delete", numberOfDeletedFeatures ), QMessageBox::Ok, QMessageBox::Cancel ) == QMessageBox::Cancel )
  {
    return;
  }

  vlayer->beginEditCommand( tr( "Features deleted" ) );
  if ( !vlayer->deleteSelectedFeatures() )
  {
    messageBar()->pushMessage( tr( "Problem deleting features" ),
                               tr( "A problem occured during deletion of features" ),
                               QgsMessageBar::WARNING );
  }

  vlayer->endEditCommand();
}

bool KKSGISWidgetQGIS::verifyEditsActionDialog( const QString& act, const QString& upon )
{
  bool res = false;
  switch ( QMessageBox::information( 0,
                                     tr( "Current edits" ),
                                     tr( "%1 current changes for %2 layer(s)?" )
                                     .arg( act )
                                     .arg( upon ),
                                     QMessageBox::Cancel | QMessageBox::Ok ) )
  {
    case QMessageBox::Ok:
      res = true;
      break;
    default:
      break;
  }
  return res;
}

QList<QgsMapLayer *> KKSGISWidgetQGIS::editableLayers( bool modified ) const
{
  QList<QgsMapLayer*> editLayers;

  if(!mpMapLegend)
    return editLayers;

  // use legend layers (instead of registry) so QList mirrors its order
  QList<QgsMapLayer*> layers = mpMapLegend->layers();
  if ( layers.count() > 0 )
  {
    foreach ( QgsMapLayer* layer, layers )
    {
      // get layer's editable/modified state from registry since we may be
      // responding to same signal that legend is also responding to
      QgsVectorLayer *vl = qobject_cast<QgsVectorLayer*>(
                             QgsMapLayerRegistry::instance()->mapLayer( layer->id() ) );
      if ( !vl )
      {
        continue;
      }
      if ( vl->isEditable() && ( !modified || ( modified && vl->isModified() ) ) )
      {
        editLayers << vl;
      }
    }
  }
  return editLayers;
}
