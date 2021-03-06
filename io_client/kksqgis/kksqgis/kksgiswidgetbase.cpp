#include "kksgiswidgetbase.h"
#include "kksbadlayerhandler.h"

#include "qgslegend.h"
#include "qgslegendlayer.h"
#include "qgslayerorder.h"
#include "qgsattributetabledialog.h"
#include "qgsclipboard.h"
#include "qgsguivectorlayertools.h"
#include "qgsprojectproperties.h"
#include "qgsdatasourceuri.h"

#include <qgslegendmodel.h>
#include <qgslegendinterface.h>
#include <qgsscalecombobox.h>   

#include <qgsapplication.h>
#include <qgsproject.h>
#include "qgsmaptip.h"

#include <qgsproviderregistry.h>
#include <qgsmaplayerregistry.h>
#include <qgspluginlayer.h>
#include <qgspluginlayerregistry.h>
#include <qgsvectordataprovider.h>

#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>

#include <qgsmessagebar.h>
#include <qgsmessagebaritem.h>
#include <qgsmessageviewer.h>

#include <qgspallabeling.h>
#include <qgsmapcanvas.h>

#include <qgsmaptool.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>

#include "qgsmaptooladdfeature.h"
#include "qgsmaptoolmovefeature.h"
#include "nodetool/qgsmaptoolnodetool.h"

#include "qgsmaptoolselectutils.h"
#include "qgsmaptoolidentifyaction.h"
#include "qgsmaptoolmeasureangle.h"
#include "qgsmaptoolselect.h"
#include "qgsmaptoolselectrectangle.h"
#include "qgsmaptoolselectfreehand.h"
#include "qgsmaptoolselectpolygon.h"
#include "qgsmaptoolselectradius.h"
#include "qgsmeasuretool.h"
#include "qgsexpressionselectiondialog.h"


#include <qgsscalecombobox.h>

#include <qgsgenericprojectionselector.h>
#include <qgsrasterlayersaveasdialog.h>
#include <qgsvectorlayersaveasdialog.h>

#include <qgsrasterfilewriter.h>
#include <qgsvectorfilewriter.h>
#include <qgscoordinatereferencesystem.h>

#include <qgsdatumtransformdialog.h>
#include <qgsdataitem.h>
#include <qgsvectordataprovider.h>
#include <qgssublayersdialog.h>
#include <qgserrordialog.h>
#include <qgsquerybuilder.h>
#include <qgseditorwidgetregistry.h>
#include <qgsrelationreferencefactory.h>

#include <qgsfield.h>
#include <qgsfeature.h>
#include <qgsgeometry.h>

#include <qgsmapcanvas.h>

#include <qgsmaptool.h>

#include <qgslabel.h>
#include <qgslabelattributes.h>
#include <qgspallabeling.h>
#include <qgslabelinggui.h>

#include <qgsvectorlayerproperties.h>
#include <qgsrasterlayerproperties.h>

#include <qgssinglesymbolrendererv2.h>
#include <qgsstylev2.h>

#include <qgsspatialindex.h> //az

#ifdef WIN32
#include "dn/dnspecbath.h"
#include "dn/Added/dnvector.h"
#include "dn/azdialgeneral.h"
#endif

#include <QProgressDialog>
#include <QMessageBox>
#include <QFileDialog>


KKSGISWidgetBase::KKSGISWidgetBase(bool withSubWindows, bool withAddons, QWidget* parent, Qt::WFlags fl)
    : QWidget(parent, fl),
    mpSelectedLayer(NULL),
    mpCoordsLabel(NULL),
    mpCoordsEdit(NULL),
    mpCoordsEditValidator(NULL),
    mpStatusBar(NULL),
    mToggleExtentsViewButton (NULL),
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
    mpDataSourceToolBar(NULL),
    mpLayerToolBar(NULL),
    mpLayerEditsToolBar(NULL),
    mpToolsToolBar(NULL),
    mpTaskToolBar(NULL),
    mInfoBar(NULL),
    //mpPanTool(NULL),
    //mpSelect(NULL),

    //mpZoomInTool(NULL),
    //mpZoomOutTool(NULL),
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
    
    //�������������� ��������� �����
    mActionAllEdits(NULL),
    mActionToggleEditing(NULL),
    mActionSaveLayerEdits(NULL),
    mActionAddFeature(NULL),
    mActionMoveFeature(NULL),
    mActionNodeTool(NULL),
    mActionDeleteSelected(NULL),
    mActionCutFeatures(NULL),
    mActionCopyFeatures(NULL),
    mActionPasteFeatures(NULL),
    //--
    mActionSaveEdits(NULL),
    mActionRollbackEdits(NULL),
    mActionCancelEdits(NULL),
    mActionSaveAllEdits(NULL),
    mActionRollbackAllEdits(NULL),
    mActionCancelAllEdits(NULL),
    
    mActionCopyStyle(NULL),
    mActionPasteStyle(NULL),
    mActionOpenTable(NULL),
    mActionPanToSelected(NULL),
    mActionZoomFullExtent(NULL),
    mActionZoomToSelected(NULL),
    mActionZoomLast(NULL),
    mActionZoomNext(NULL),
    mActionZoomToLayer(NULL),
    mActionZoomActualSize(NULL),
    mActionDraw(NULL),
    mActionIdentify(NULL),
    mActionMapTips(NULL),
    mActionSelect(NULL),
    mActionSelectRectangle(NULL),
    mActionSelectPolygon(NULL),
    mActionSelectFreehand(NULL),
    mActionSelectRadius(NULL),
    mActionDeselectAll(NULL),
    mActionSelectByExpression(NULL),
    mActionMeasure(NULL),
    mActionMeasureArea(NULL),
    mActionMeasureAngle(NULL),
    mActionAllMeasure(NULL),
    mActionAllSelect(NULL),
    mInternalClipboard( 0 ),
    mpRegistry(QgsMapLayerRegistry::instance()),
    mShowProjectionTab( false )
{
#ifdef WIN32    
    dnThemTaskSpecBath = NULL; //???
#endif
    azSettings = new QSettings ("VKA", "Reper"); // ������: ����������� + ��� //az


#ifdef Q_OS_WIN
    mSkipNextContextMenuEvent = 0;
#endif

    azWorkList.clear();

    m_bWithSubwindows = withSubWindows; //���� �� ��������� �������������� ���� (����, ������, ��������� � �.�.)
    m_bWithAddons = withAddons;//���� �� ��������� ���� ��� ������������ ��������� (���, ���� �������� � ������� DynamicDocs)
    m_bInit = false;
    mSaveRollbackInProgress = false;
  
    mMapTipsVisible = false;

}


void KKSGISWidgetBase::initQGIS()
{
    QIcon icon(":/ico/main64x64.png");
    //setWindowIcon(icon);

    initUserSettings();
    initMapCanvas();
    initStatusBar();

    //initLegend();
    initMapLegend();
    createMapTips();

    initActions();
    initConnections();
    readSettings();

    initToolBar();
    initMenuBar();

    initCanvasTools();

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

    if(mpStatusBar && m_bWithAddons)
        mpStatusBar->showMessage("��� �����-�� ����� � ������.");

    m_bInit = true;
}

KKSGISWidgetBase::~KKSGISWidgetBase()
{
    if(!m_bInit)
        return;//������ �������. ������ �� ��� ���������������


    SLOTmpCloseProject();

  delete mMapTools.mZoomIn;
  delete mMapTools.mZoomOut;
  delete mMapTools.mPan;
#ifdef HAVE_TOUCH
  //delete mMapTools.mTouch;
#endif
  delete mMapTools.mAddFeature;
  //delete mMapTools.mAddPart;
  //delete mMapTools.mAddRing;
  //delete mMapTools.mAnnotation;
  //delete mMapTools.mChangeLabelProperties;
  //delete mMapTools.mDeletePart;
  //delete mMapTools.mDeleteRing;
  //delete mMapTools.mFeatureAction;
  //delete mMapTools.mFormAnnotation;
  //delete mMapTools.mHtmlAnnotation;
  delete mMapTools.mIdentify;
  delete mMapTools.mMeasureAngle;
  delete mMapTools.mMeasureArea;
  delete mMapTools.mMeasureDist;
  delete mMapTools.mMoveFeature;
  //delete mMapTools.mMoveLabel;
  delete mMapTools.mNodeTool;
  //delete mMapTools.mOffsetCurve;
  //delete mMapTools.mPinLabels;
  //delete mMapTools.mReshapeFeatures;
  //delete mMapTools.mRotateFeature;
  //delete mMapTools.mRotateLabel;
  //delete mMapTools.mRotatePointSymbolsTool;
  delete mMapTools.mSelect;
  delete mMapTools.mSelectFreehand;
  delete mMapTools.mSelectPolygon;
  delete mMapTools.mSelectRadius;
  delete mMapTools.mSelectRectangle;
  //delete mMapTools.mShowHideLabels;
  //delete mMapTools.mSimplifyFeature;
  //delete mMapTools.mSplitFeatures;
  //delete mMapTools.mSplitParts;
  //delete mMapTools.mSvgAnnotation;
  //delete mMapTools.mTextAnnotation;

    if(mpMapToolBar)
        delete mpMapToolBar;

    if(mpToolsToolBar)
        delete mpToolsToolBar;

    if(mpDataSourceToolBar)
        delete mpDataSourceToolBar;

    if(mpLayerToolBar)
        delete mpLayerToolBar;

    if(mpLayerEditsToolBar)
        delete mpLayerEditsToolBar;

    if(mpTaskToolBar)
        delete mpTaskToolBar;

    if(mpMenuBar)
        delete mpMenuBar;

    if(mpStatusBar)
        delete mpStatusBar;

    if(mInfoBar)
        delete mInfoBar;

    delete mpMapLegendWidget; //� ����������� ������ KKSMapWidget ����� ������� �������� setParent(NULL), ������� ������������� �� �� ���������. ��� ���� ������� ����� ����� �������

    delete mpMapCanvas;

    delete mInternalClipboard;

}
   
void KKSGISWidgetBase::initStatusBar()
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
    //toggle to switch between mouse pos and extents display in status bar widget
    mToggleExtentsViewButton = new QToolButton( statusBar() );
    mToggleExtentsViewButton->setObjectName( "mToggleExtentsViewButton" );
    mToggleExtentsViewButton->setMaximumWidth( 20 );
    mToggleExtentsViewButton->setMaximumHeight( 20 );
    mToggleExtentsViewButton->setIcon( QgsApplication::getThemeIcon( "tracking.png" ) );
    mToggleExtentsViewButton->setToolTip( tr( "Toggle extents and mouse position display" ) );
    mToggleExtentsViewButton->setCheckable( true );
    connect( mToggleExtentsViewButton, SIGNAL( toggled( bool ) ), this, SLOT( extentsViewToggled( bool ) ) );
    statusBar()->addPermanentWidget( mToggleExtentsViewButton, 0 );


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

    mOnTheFlyProjectionStatusLabel = new QLabel( QString(), statusBar() );
    mOnTheFlyProjectionStatusLabel->setObjectName( "mOnTheFlyProjectionStatusLabel" );
    mOnTheFlyProjectionStatusLabel->setFont( myFont );
    mOnTheFlyProjectionStatusLabel->setMinimumWidth( 10 );
    mOnTheFlyProjectionStatusLabel->setMaximumHeight( 20 );
    mOnTheFlyProjectionStatusLabel->setMargin( 3 );
    mOnTheFlyProjectionStatusLabel->setAlignment( Qt::AlignCenter );
    mOnTheFlyProjectionStatusLabel->setFrameStyle( QFrame::NoFrame );
    QString myCrs = mpMapCanvas->mapRenderer()->destinationCrs().authid();
    statusBar()->addPermanentWidget( mOnTheFlyProjectionStatusLabel, 0 );

    // On the fly projection status bar icon
    // Changed this to a tool button since a QPushButton is
    // sculpted on OS X and the icon is never displayed [gsherman]
    mOnTheFlyProjectionStatusButton = new QToolButton( statusBar() );
    mOnTheFlyProjectionStatusButton->setObjectName( "mOntheFlyProjectionStatusButton" );
    mOnTheFlyProjectionStatusButton->setMaximumWidth( 20 );
    // Maintain uniform widget height in status bar by setting button height same as labels
    // For Qt/Mac 3.3, the default toolbutton height is 30 and labels were expanding to match
    mOnTheFlyProjectionStatusButton->setMaximumHeight( mScaleLabel->height() );
    mOnTheFlyProjectionStatusButton->setIcon( QgsApplication::getThemeIcon( "mIconProjectionEnabled.png" ) );
    mOnTheFlyProjectionStatusButton->setWhatsThis( tr( "This icon shows whether "
      "on the fly coordinate reference system transformation is enabled or not. "
      "Click the icon to bring up "
      "the project properties dialog to alter this behaviour." ) );
    mOnTheFlyProjectionStatusButton->setToolTip( tr( "CRS status - Click "
      "to open coordinate reference system dialog" ) );
    connect( mOnTheFlyProjectionStatusButton, SIGNAL( clicked() ),
           this, SLOT( projectPropertiesProjections() ) );//bring up the project props dialog when clicked

    statusBar()->addPermanentWidget( mOnTheFlyProjectionStatusButton, 0 );

}

QStatusBar * KKSGISWidgetBase::statusBar() const
{
    return mpStatusBar;
}

void KKSGISWidgetBase::initUserSettings()
{
    mpAppPath = "D:/!Share/src/reper/ddocclient/app/reper/"; // �����, ���� �� ������������?
    m_pluginsDir = "C:\\Program Files (x86)\\DynamicSoft\\DynamicDocs Client 1.2.3\\QGS21\\plugins";
    QgsApplication::setPluginPath(m_pluginsDir);
    //QgsProviderRegistry::instance(m_pluginsDir);
#ifdef WIN32
    m_pluginsDir = azSettings->value("MainSettings/PluginsDirPath", "C:/REP_EXT/QGS22/plugins").toString();   // ������: ������� ��������
    QgsApplication::setPluginPath(m_pluginsDir);
#endif
}

const QString & KKSGISWidgetBase::pluginsDir()
{
    return m_pluginsDir;
}

void KKSGISWidgetBase::initMapCanvas()
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
    
    // ��������� ����� � ��������� � ���� ������ "���� �����"(Map Canvas)
    QVBoxLayout * mpMapLayout = new QVBoxLayout(this); //main Layout
    mpMapLayout->addWidget(mpMapCanvas); // adding MapCanvas in Layout
    mpMapLayout->addWidget( mInfoBar);

    mpMapCanvas->show();
}

void KKSGISWidgetBase::initMapLegend()
{
    if(!m_bWithSubwindows)
        return;

    // "theMapLegend" used to find this canonical instance later
    mpMapLegend = new QgsLegend( mpMapCanvas, this, "theMapLegend" );
    mpMapLegend->setWorkingWidget(this);

    mpMapLegend->setWhatsThis( tr( "Map legend that displays all the layers currently on the map canvas. Click on the check box to turn a layer on or off. Double click on a layer in the legend to customize its appearance and set other properties." ) );

    QCheckBox *orderCb = new QCheckBox( tr( "������ �������� ��������� �����" ) );
    orderCb->setChecked( false );

    connect( orderCb, SIGNAL( toggled( bool ) ), mpMapLegend, SLOT( unsetUpdateDrawingOrder( bool ) ) );
    connect( orderCb, SIGNAL( toggled( bool ) ), this, SIGNAL( dataChanged( ) ) );
    connect( mpMapLegend, SIGNAL( updateDrawingOrderUnchecked( bool ) ), orderCb, SLOT( setChecked( bool ) ) );
    connect( mpMapLegend, SIGNAL( currentLayerChanged( QgsMapLayer * ) ), this, SLOT( activateDeactivateLayerRelatedActions( QgsMapLayer * ) ) );
    connect( mpMapLegend, SIGNAL( itemSelectionChanged() ), this, SLOT( legendLayerSelectionChanged() ) );
    connect( mpMapLegend, SIGNAL( zOrderChanged() ), this, SLOT( markDirty() ) );
    connect( mpMapLegend, SIGNAL( zOrderChanged() ), this, SIGNAL( dataChanged() ) );

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

QWidget * KKSGISWidgetBase::mapLegendWidget()
{
    return mpMapLegendWidget;
}

QWidget * KKSGISWidgetBase::mapLayerOrderWidget()
{
    return mpMapLayerOrderWidget;
}

void KKSGISWidgetBase::initActions()
{
    //��������� �� �����
    mpActionZoomIn = new QAction(this);
    mpActionZoomIn->setObjectName(QString::fromUtf8("mpActionZoomIn"));
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/ico/mActionZoomIn.png"), QSize(), QIcon::Normal, QIcon::Off);
    mpActionZoomIn->setIcon(icon);
    mpActionZoomIn->setText(tr("����������"));

    mpActionZoomOut = new QAction(this);
    mpActionZoomOut->setObjectName(QString::fromUtf8("mpActionZoomOut"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/ico/mActionZoomOut.png"), QSize(), QIcon::Normal, QIcon::Off);
    mpActionZoomOut->setIcon(icon1);
    mpActionZoomOut->setText(tr("��������"));

    mpActionPan = new QAction(this);
    mpActionPan->setObjectName(QString::fromUtf8("mpActionPan"));
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/ico/mActionPan.png"), QSize(), QIcon::Normal, QIcon::Off);
    mpActionPan->setIcon(icon2);
    mpActionPan->setText(tr("�����������"));

    mActionPanToSelected = new QAction(tr("Pan Map to Selection"), this);
    mActionZoomFullExtent = new QAction(tr("Zoom Full"), this);
    mActionZoomToLayer = new QAction(tr("Zoom to Layer"), this);
    mActionZoomToSelected = new QAction(tr("Zoom to Selection"), this);
    mActionZoomLast = new QAction(tr("Zoom Last"), this);
    mActionZoomNext = new QAction(tr("Zoom Next"), this);
    mActionZoomActualSize = new QAction(tr("Zoom Actual Size"), this);
    mActionDraw = new QAction(tr("Refresh"), this);
    
    mActionSelect = new QAction(tr("Select Single Feature"), this);
    mActionSelectRectangle = new QAction(tr("Select Features by Rectangle"), this);
    mActionSelectPolygon = new QAction(tr("Select Features by Polygon"), this);
    mActionSelectFreehand = new QAction(tr("Select Features by Freehand"), this);
    mActionSelectRadius = new QAction(tr("Select Features by Radius"), this);
    mActionSelectByExpression = new QAction(tr("Select By Expression"), this);
    mActionDeselectAll = new QAction(tr("Deselect Features from All Layers"), this);

    mActionAllSelect = new QAction(tr("Select"), this);
    QMenu* menuAllSelect = new QMenu( tr( "Select" ), this );
    menuAllSelect->addAction( mActionSelect );
    menuAllSelect->addAction( mActionSelectRectangle );
    menuAllSelect->addAction( mActionSelectPolygon );
    menuAllSelect->addAction( mActionSelectFreehand );
    menuAllSelect->addAction( mActionSelectRadius );
    menuAllSelect->addAction( mActionSelectByExpression );
    menuAllSelect->addAction( mActionDeselectAll );
    mActionAllSelect->setMenu( menuAllSelect );

    mActionIdentify = new QAction(tr("Identify Features"), this);
    mActionMapTips = new QAction(tr("Map Tips"), this);
    
    mActionAllMeasure = new QAction(tr("Measure"), this);
    QMenu* menuAllMeasure = new QMenu( tr( "Measure" ), this );
    mActionMeasure = new QAction(tr("Measure Line"), this);
    mActionMeasureArea = new QAction(tr("Measure Area"), this);
    mActionMeasureAngle = new QAction(tr("Measure Angle"), this);
    menuAllMeasure->addAction( mActionMeasure );
    menuAllMeasure->addAction( mActionMeasureArea );
    menuAllMeasure->addAction( mActionMeasureAngle );
    mActionAllMeasure->setMenu( menuAllMeasure );



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
    mpActionFileOpenProject->setText(tr("������� ������..."));
    
    mpActionFileSaveProject = new QAction(this);
    mpActionFileSaveProject->setObjectName(QString::fromUtf8("mpActionFileSaveProject"));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/ico/mActionFileSave.png"), QSize(), QIcon::Normal, QIcon::On);
    mpActionFileSaveProject->setIcon(icon5);
    mpActionFileSaveProject->setText(tr("��������� ������"));

    mpActionBathymetry = new QAction(this);
    mpActionBathymetry->setObjectName(QString::fromUtf8("mpActionBathymetry"));
    mpActionBathymetry->setText(tr("������������ ��������� ��������� ������"));

    mpVectorize = new QAction(QIcon(":/ico/vectorize.png"), tr("����� ��������"), this); // Az
    mpActionShortestPathAreaSelect = new QAction(QIcon(":/ico/vectorize.png"), tr("������� ���� ��� ������"), this); // Az
    mpActionShortestPathCalc = new QAction(QIcon(":/ico/vectorize.png"), tr("��������� �������"), this); // Az
    mpActionShortestPathGrid = new QAction(QIcon(":/ico/vectorize.png"), tr("������������ ���������� ����"), this); // Az
    mpActionDirSettings = new QAction(QIcon(":/ico/vectorize.png"), tr("��������� ������� ���������"), this); // Az


//    mpActionBathymetry = new QAction(this);
//    mpActionBathymetry->setObjectName(QString::fromUtf8("mpActionBathymetry"));
//    mpActionBathymetry->setText(tr("���������� ��������� ������������ ������� �������"));
//    QIcon icon5;
//    icon5.addFile(QString::fromUtf8(":/ico/mActionFileSave.png"), QSize(), QIcon::Normal, QIcon::On);
//    mpActionFileSaveProject->setIcon(icon5);
//    mpActionFileSaveProject->setText(tr("��������� ������"));
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
    mpActionFileSaveProjectAs->setText(tr("��������� ������ ���..."));

    mpActionFileCloseProject = new QAction(this);
    mpActionFileCloseProject->setObjectName(QString::fromUtf8("mpActionFileCloseProject"));
    mpActionFileCloseProject->setText(tr("������� ������"));

    mpActionAddVectorLayer = new QAction(QIcon(":/ico/add_vector.png"), tr("�������� &��������� ����"), this);
    mpActionAddVectorLayer->setStatusTip(tr("�������� ��������� ���� � ���� �����"));

    mpActionAddRasterLayer = new QAction(QIcon(":/ico/add_raster.png"), tr("�������� &��������� ����"), this);
    mpActionAddRasterLayer->setStatusTip(tr("�������� ��������� ���� � ���� �����"));
    
    //mpActionFileExit = new QAction(QIcon(":/ico/mActionFileExit.png"), tr("�����"), this);
    //mpActionFileExit->setStatusTip(tr("Close Application"));
    //mpActionFileExit->setShortcuts(QKeySequence::Close);
    
    mpActionAddPostGISLayer = new QAction(QIcon(":/ico/add_postgis.png"), tr("�������� ���� �� ��"), this);
    mpActionAddPostGISLayer->setStatusTip(tr("�������� ���� �� ��"));

    mActionProjectProperties = new QAction(tr("Project Properties..."), this);
    mActionNewProject = new QAction(tr("&New"), this);

    mActionRemoveLayer = new QAction(tr("Remove Layer(s)"), this);
    mActionDuplicateLayer = new QAction(tr("Duplicate Layer(s)"), this);
    mActionSetLayerCRS = new QAction(tr("Set CRS of Layer(s)"), this);
    mActionSetProjectCRSFromLayer = new QAction(tr("Set Project CRS from Layer"), this);
    mActionLayerProperties = new QAction(tr("Properties..."), this);
    mActionLayerSubsetString = new QAction(tr("Query..."), this);
    
    mActionCopyStyle = new QAction(tr("Copy style"), this);
    mActionPasteStyle = new QAction(tr("Paste style"), this);
    mActionOpenTable = new QAction(tr("Open Attribute Table"), this);
    mActionLayerSaveAs = new QAction(tr("Save As..."), this);
    mActionLayerSelectionSaveAs = new QAction(tr("Save Selection as Vector File..."), this);


    mActionAllEdits = new QAction(tr("Current Edits"), this);
    mActionToggleEditing = new QAction(tr("Toggle Editing"), this);
    mActionSaveLayerEdits = new QAction(tr("Save Layer Edits"), this);
    mActionAddFeature = new QAction(tr("Add Feature"), this);
    mActionMoveFeature = new QAction(tr("Move Feature(s)"), this);
    mActionNodeTool = new QAction(tr("Node Tool"), this);
    mActionDeleteSelected = new QAction(tr("Delete Selected"), this);
    mActionCutFeatures = new QAction(tr("Cut Features"), this);
    mActionCopyFeatures = new QAction(tr("Copy Features"), this);
    mActionPasteFeatures = new QAction(tr("Paste Features"), this);
    //--
    mActionSaveEdits = new QAction(tr("Save Edits"), this);
    mActionRollbackEdits = new QAction(tr("Rollback Edits"), this);
    mActionCancelEdits = new QAction(tr("Cancel Edits"), this);
    mActionSaveAllEdits = new QAction(tr("Save All Edits"), this);
    mActionRollbackAllEdits = new QAction(tr("Rollback All Edits"), this);
    mActionCancelAllEdits = new QAction(tr("Cancel All Edits"), this);

    // Vector edits menu
    QMenu* menuAllEdits = new QMenu( tr( "Current Edits" ), this );
    menuAllEdits->addAction( mActionSaveEdits );
    menuAllEdits->addAction( mActionRollbackEdits );
    menuAllEdits->addAction( mActionCancelEdits );
    menuAllEdits->addSeparator();
    menuAllEdits->addAction( mActionSaveAllEdits );
    menuAllEdits->addAction( mActionRollbackAllEdits );
    menuAllEdits->addAction( mActionCancelAllEdits );
    mActionAllEdits->setMenu( menuAllEdits );
}


void KKSGISWidgetBase::initConnections()
{

    connect( mActionRemoveLayer, SIGNAL( triggered() ), this, SLOT( removeLayer() ) );
    connect( mActionDuplicateLayer, SIGNAL( triggered() ), this, SLOT( duplicateLayers() ) );
    connect( mActionSetLayerCRS, SIGNAL( triggered() ), this, SLOT( setLayerCRS() ) );
    connect( mActionSetProjectCRSFromLayer, SIGNAL( triggered() ), this, SLOT( setProjectCRSFromLayer() ) );
    connect( mActionLayerProperties, SIGNAL( triggered() ), this, SLOT( layerProperties() ) );
    connect( mActionLayerSubsetString, SIGNAL( triggered() ), this, SLOT( layerSubsetString() ) );
    connect( mActionLayerSaveAs, SIGNAL( triggered() ), this, SLOT( saveAsFile() ) );
    connect( mActionLayerSelectionSaveAs, SIGNAL( triggered() ), this, SLOT( saveSelectionAsVectorFile() ) );
    
    //�������������� ��������� �����
    connect( mActionToggleEditing, SIGNAL( triggered() ), this, SLOT( toggleEditing() ) );
    connect( mActionSaveLayerEdits, SIGNAL( triggered() ), this, SLOT( saveActiveLayerEdits() ) );
    connect( mActionAddFeature, SIGNAL( triggered() ), this, SLOT( addFeature() ) );
    connect( mActionMoveFeature, SIGNAL( triggered() ), this, SLOT( moveFeature() ) );
    connect( mActionNodeTool, SIGNAL( triggered() ), this, SLOT( nodeTool() ) );
    connect( mActionDeleteSelected, SIGNAL( triggered() ), this, SLOT( deleteSelected() ) );
    connect( mActionCutFeatures, SIGNAL( triggered() ), this, SLOT( editCut() ) );
    connect( mActionCopyFeatures, SIGNAL( triggered() ), this, SLOT( editCopy() ) );
    connect( mActionPasteFeatures, SIGNAL( triggered() ), this, SLOT( editPaste() ) );
    //--    
    connect( mActionSaveEdits, SIGNAL( triggered() ), this, SLOT( saveEdits() ) );
    connect( mActionRollbackEdits, SIGNAL( triggered() ), this, SLOT( rollbackEdits() ) );
    connect( mActionCancelEdits, SIGNAL( triggered() ), this, SLOT( cancelEdits() ) );
    connect( mActionSaveAllEdits, SIGNAL( triggered() ), this, SLOT( saveAllEdits() ) );
    connect( mActionRollbackAllEdits, SIGNAL( triggered() ), this, SLOT( rollbackAllEdits() ) );
    connect( mActionCancelAllEdits, SIGNAL( triggered() ), this, SLOT( cancelAllEdits() ) );
    
    connect( mActionOpenTable, SIGNAL( triggered() ), this, SLOT( attributeTable() ) );
    connect( mActionCopyStyle, SIGNAL( triggered() ), this, SLOT( copyStyle() ) );
    connect( mActionPasteStyle, SIGNAL( triggered() ), this, SLOT( pasteStyle() ) );

    connect( mActionPanToSelected, SIGNAL( triggered() ), this, SLOT( panToSelected() ) );
    connect( mActionZoomFullExtent, SIGNAL( triggered() ), this, SLOT( zoomFull() ) );
    connect( mActionZoomToSelected, SIGNAL( triggered() ), this, SLOT( zoomToSelected() ) );
    connect( mActionZoomLast, SIGNAL( triggered() ), this, SLOT( zoomToPrevious() ) );
    connect( mActionZoomNext, SIGNAL( triggered() ), this, SLOT( zoomToNext() ) );
    connect( mActionZoomToLayer, SIGNAL( triggered() ), this, SLOT( zoomToLayerExtent() ) );
    connect( mActionZoomActualSize, SIGNAL( triggered() ), this, SLOT( zoomActualSize() ) );

    connect( mActionDraw, SIGNAL( triggered() ), this, SLOT( refreshMapCanvas() ) );

    connect( mActionIdentify, SIGNAL( triggered() ), this, SLOT( identify() ) );
    connect( mActionMapTips, SIGNAL( triggered() ), this, SLOT( toggleMapTips() ) );

    connect( mActionSelect, SIGNAL( triggered() ), this, SLOT( select() ) );
    connect( mActionSelectRectangle, SIGNAL( triggered() ), this, SLOT( selectByRectangle() ) );
    connect( mActionSelectPolygon, SIGNAL( triggered() ), this, SLOT( selectByPolygon() ) );
    connect( mActionSelectFreehand, SIGNAL( triggered() ), this, SLOT( selectByFreehand() ) );
    connect( mActionSelectRadius, SIGNAL( triggered() ), this, SLOT( selectByRadius() ) );
    connect( mActionDeselectAll, SIGNAL( triggered() ), this, SLOT( deselectAll() ) );
    connect( mActionSelectByExpression, SIGNAL( triggered() ), this, SLOT( selectByExpression() ) );

    connect( mActionMeasure, SIGNAL( triggered() ), this, SLOT( measure() ) );
    connect( mActionMeasureArea, SIGNAL( triggered() ), this, SLOT( measureArea() ) );
    connect( mActionMeasureAngle, SIGNAL( triggered() ), this, SLOT( measureAngle() ) );


    connect(this->mpActionPan, SIGNAL(triggered()), this, SLOT(panMode()));
    connect(this->mpActionZoomIn, SIGNAL(triggered()), this, SLOT(zoomInMode()));
    connect(this->mpActionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOutMode()));

    connect(this->mpActionAddVectorLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddVectorLayer()));
    connect(this->mpActionAddRasterLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddRasterLayer()));
    connect(this->mpActionAddPostGISLayer, SIGNAL(triggered()), this, SLOT(SLOTmpActionAddPostGISLayer()));


    connect(this->mActionProjectProperties, SIGNAL( triggered() ), this, SLOT( projectProperties() ) );
    connect(this->mActionNewProject, SIGNAL( triggered() ), this, SLOT( SLOTmpActionFileNew() ) );
    connect(this->mpActionFileOpenProject, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileOpenProject()));
    connect(this->mpActionFileSaveProjectAs, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileSaveProjectAs()));
    connect(this->mpActionFileSaveProject, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileSaveProject()));
    connect(this->mpActionFileCloseProject, SIGNAL(triggered()), this, SLOT(SLOTmpCloseProject()));
    //connect(this->mpActionFileExit, SIGNAL(triggered()), this, SLOT(SLOTmpActionFileExit()));

    //connect(this->mpRegistry, SIGNAL(layerWasAdded(QgsMapLayer *)), this, SLOT(addLayerToTOC(QgsMapLayer *)));
    
    if(mpCoordsEdit)
        connect(this->mpCoordsEdit, SIGNAL(editingFinished()), this, SLOT( SLOTazCoordsCenter()));
    
    // task connections
//    connect(this->mpVectorize, SIGNAL(triggered()), this, SLOT(SLOTtempUse()));
    connect(this->mpActionBathymetry, SIGNAL(triggered()), this, SLOT(SLOTazThemTaskSpectralBathynometry()));
    connect(this->mpActionShortestPathAreaSelect, SIGNAL(triggered()), this, SLOT(SLOTshortestPathSelectArea())); // Az
    connect(this->mpActionShortestPathCalc, SIGNAL(triggered()), this, SLOT(SLOTshortestPathCalculate())); // Az
    connect(this->mpActionShortestPathGrid, SIGNAL(triggered()), this, SLOT(SLOTshortestPathGridArea())); // Az
    connect(this->mpActionDirSettings, SIGNAL(triggered()), this, SLOT(SLOTazActionDirSettings())); // Az

    connect( mpMapCanvas, SIGNAL(xyCoordinates(const QgsPoint &)), this, SLOT(showMouseCoordinate(const QgsPoint &)));
    connect( mpMapCanvas, SIGNAL( extentsChanged() ), this, SLOT( markDirty() ) );
    connect( mpMapCanvas, SIGNAL( extentsChanged() ), this, SLOT( showExtents() ) );
    connect( mpMapCanvas, SIGNAL( scaleChanged( double ) ), this, SLOT( showScale( double ) ) );
    connect( mpMapCanvas, SIGNAL( scaleChanged( double ) ), this, SLOT( updateMouseCoordinatePrecision() ) );
    connect( mpMapCanvas, SIGNAL( mapToolSet( QgsMapTool * ) ), this, SLOT( mapToolChanged( QgsMapTool * ) ) );
    connect( mpMapCanvas, SIGNAL( selectionChanged( QgsMapLayer * ) ), this, SLOT( selectionChanged( QgsMapLayer * ) ) );
    connect( mpMapCanvas, SIGNAL( layersChanged() ), this, SLOT( markDirty() ) );
    connect( mpMapCanvas, SIGNAL( zoomLastStatusChanged( bool ) ), mActionZoomLast, SLOT( setEnabled( bool ) ) );
    connect( mpMapCanvas, SIGNAL( zoomNextStatusChanged( bool ) ), mActionZoomNext, SLOT( setEnabled( bool ) ) );
    // connect MapCanvas keyPress event so we can check if selected feature collection must be deleted
    connect( mpMapCanvas, SIGNAL( keyPressed( QKeyEvent * ) ), this, SLOT( mapCanvas_keyPressed( QKeyEvent * ) ) );

    //connect( mRenderSuppressionCBox, SIGNAL( toggled( bool ) ), mMapCanvas, SLOT( setRenderFlag( bool ) ) );

    connect( mpMapCanvas->mapRenderer(), SIGNAL( drawingProgress( int, int ) ), this, SLOT( showProgress( int, int ) ) );
    connect( mpMapCanvas->mapRenderer(), SIGNAL( hasCrsTransformEnabledChanged( bool ) ), this, SLOT( hasCrsTransformEnabled( bool ) ) );
    connect( mpMapCanvas->mapRenderer(), SIGNAL( destinationSrsChanged() ), this, SLOT( destinationSrsChanged() ) );

    connect(QgsProject::instance(), SIGNAL(readProject(QDomDocument)), mpMapCanvas, SLOT(readProject(QDomDocument)));
    connect( QgsProject::instance(), SIGNAL( layerLoaded( int, int ) ), this, SLOT( showProgress( int, int ) ) );
    connect( QgsProject::instance(), SIGNAL( writeProject(QDomDocument &) ), this, SIGNAL( mapSaved( QDomDocument& ) ) );
    //connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument& ) ), this, SLOT( writeAnnotationItemsToProject( QDomDocument& ) ) );
    connect( QgsProject::instance(), SIGNAL( loadingLayer( QString ) ), this, SLOT( showStatusMessage( QString ) ) );

    // connect map layer registry
    connect( this->mpRegistry, SIGNAL( layersRemoved(QStringList)), this, SIGNAL(dataChanged()));
    connect( this->mpRegistry, SIGNAL( layersWillBeRemoved( QStringList ) ), this, SLOT( removingLayers( QStringList ) ) );
    connect( this->mpRegistry, SIGNAL( layersAdded(QList<QgsMapLayer *>)), this, SIGNAL(dataChanged()));
    connect( this->mpRegistry, SIGNAL( layersAdded( QList<QgsMapLayer *> ) ), this, SLOT( layersWereAdded( QList<QgsMapLayer *> ) ) );

    // handle deprecated labels in project for QGIS 2.0
    //connect( this, SIGNAL( newProject() ), this, SLOT( checkForDeprecatedLabelsInProject() ) );
    //connect( this, SIGNAL( projectRead() ), this, SLOT( checkForDeprecatedLabelsInProject() ) );
}


void KKSGISWidgetBase::initToolBar()
{
    if(!m_bWithSubwindows)
        return;

    //������
    mpMapToolBar = new QToolBar(tr("������ � ��������"));
    mpMapToolBar->setObjectName("mpMapToolBar");

    mpMapToolBar->addAction(mActionNewProject);
    mpMapToolBar->addAction(mpActionFileOpenProject);
    mpMapToolBar->addAction(mpActionFileSaveProject);
    //mpMapToolBar->addAction(mpActionFileSaveProjectAs);
    mpMapToolBar->addSeparator();
    mpMapToolBar->addAction(mActionProjectProperties);
    //mpMapToolBar->addAction(mpActionFileCloseProject);

    mpToolBarMap.insert(mpMapToolBar->objectName(), mpMapToolBar);

    //�����������
    mpToolsToolBar = new QToolBar(tr("�����������"));
    mpToolsToolBar->setObjectName("mpToolsToolBar");

    mpToolsToolBar->addAction(mpActionPan);
    mpToolsToolBar->addAction(mActionPanToSelected);
    mpToolsToolBar->addAction(mpActionZoomIn);
    mpToolsToolBar->addAction(mpActionZoomOut);
    mpToolsToolBar->addSeparator();

    mpToolsToolBar->addAction(mActionZoomFullExtent);
    mpToolsToolBar->addAction(mActionZoomToLayer);
    mpToolsToolBar->addAction(mActionZoomToSelected);
    mpToolsToolBar->addAction(mActionZoomLast);
    mpToolsToolBar->addAction(mActionZoomNext);
    mpToolsToolBar->addAction(mActionZoomActualSize);
    mpToolsToolBar->addSeparator();

    mpToolsToolBar->addAction(mActionAllSelect);
    mpToolsToolBar->addAction(mActionIdentify);
    mpToolsToolBar->addAction(mActionMapTips);
    mpToolsToolBar->addAction(mActionAllMeasure);
    mpToolsToolBar->addSeparator();

    mpToolsToolBar->addAction(mActionDraw);


    mpToolBarMap.insert(mpToolsToolBar->objectName(), mpToolsToolBar);

    //��������� ������
    mpDataSourceToolBar = new QToolBar(tr("��������� ������"));
    mpDataSourceToolBar->setObjectName("mpDataSourceToolBar");

    mpDataSourceToolBar->addAction(mpActionAddVectorLayer);
    mpDataSourceToolBar->addAction(mpActionAddRasterLayer);
    mpDataSourceToolBar->addAction(mpActionAddPostGISLayer);

    mpToolBarMap.insert(mpDataSourceToolBar->objectName(), mpDataSourceToolBar);

    //if(m_bWithAddons){
        //������
        mpTaskToolBar = new QToolBar(tr("������"));
        mpTaskToolBar->setObjectName("mpTaskToolBar");

//        mpTaskToolBar->addAction(mpVectorize);
        mpTaskToolBar->addAction(mpActionBathymetry);

        mpToolBarMap.insert(mpTaskToolBar->objectName(), mpTaskToolBar);
    //}

    //����
    mpLayerToolBar = new QToolBar(tr("����"));
    mpLayerToolBar->setObjectName("mpLayerToolBar");

    mpLayerToolBar->addAction(mActionCopyStyle);
    mpLayerToolBar->addAction(mActionPasteStyle);
    mpLayerToolBar->addSeparator();
    
    mpLayerToolBar->addAction(mActionOpenTable);
    //mpLayerToolBar->addAction(mActionToggleEditing);
    //mpLayerToolBar->addAction(mActionSaveLayerEdits);
    //mpLayerToolBar->addAction(mActionAllEdits);
    mpLayerToolBar->addSeparator();

    //mpLayerToolBar->addAction(mActionLayerSaveAs);
    //mpLayerToolBar->addAction(mActionLayerSelectionSaveAs);

    mpLayerToolBar->addAction(mActionRemoveLayer);
    mpLayerToolBar->addAction(mActionDuplicateLayer);
    //mpLayerToolBar->addAction(mActionSetLayerCRS);
    //mpLayerToolBar->addAction(mActionSetProjectCRSFromLayer);
    mpLayerToolBar->addAction(mActionLayerProperties);
    //mpLayerToolBar->addAction(mActionLayerSubsetString);

    mpToolBarMap.insert(mpLayerToolBar->objectName(), mpLayerToolBar);
    
    //�������������� ��������� �����
    mpLayerEditsToolBar = new QToolBar(tr("�������������� ��������� �����"));
    mpLayerEditsToolBar->setObjectName("mpLayerEditsToolBar");
    mpLayerEditsToolBar->addAction(mActionAllEdits);
    mpLayerEditsToolBar->addAction(mActionToggleEditing);
    mpLayerEditsToolBar->addAction(mActionSaveLayerEdits);
    mpLayerEditsToolBar->addAction(mActionAddFeature);
    mpLayerEditsToolBar->addAction(mActionMoveFeature);
    mpLayerEditsToolBar->addAction(mActionNodeTool);
    mpLayerEditsToolBar->addAction(mActionDeleteSelected);
    mpLayerEditsToolBar->addAction(mActionCutFeatures);
    mpLayerEditsToolBar->addAction(mActionCopyFeatures);
    mpLayerEditsToolBar->addAction(mActionPasteFeatures);

    mpToolBarMap.insert(mpLayerEditsToolBar->objectName(), mpLayerEditsToolBar);

}

/*
QToolBar * KKSGISWidgetBase::toolBar() const
{
    return mpMapToolBar;
}
*/

void KKSGISWidgetBase::initMenuBar()
{
    if(!m_bWithSubwindows)
        return;

    mpMenuBar = new QMenuBar();

    QMenu * menuFile = new QMenu(tr("������"));
    menuFile->addAction(mActionNewProject);
    menuFile->addAction(mpActionFileOpenProject);
    menuFile->addAction(mpActionFileSaveProject);
    menuFile->addAction(mpActionFileSaveProjectAs);
    menuFile->addSeparator();
    menuFile->addAction(mpActionDirSettings);
    menuFile->addAction(mActionProjectProperties);
    menuFile->addSeparator();
    menuFile->addAction(mpActionFileCloseProject);

    mpMenuBar->addMenu(menuFile);
    mpMenuMap.insert(tr("File"), menuFile);

    QMenu * menuDataSources = new QMenu(tr("��������� ������"));
    menuDataSources->addAction(mpActionAddVectorLayer);
    menuDataSources->addAction(mpActionAddRasterLayer);
    menuDataSources->addAction(mpActionAddPostGISLayer);
    
    mpMenuBar->addMenu(menuDataSources);
    mpMenuMap.insert(tr("DataSources"), menuDataSources);

    QMenu * menuLayer = new QMenu(tr("����"));
    menuLayer->addAction(mActionCopyStyle);
    menuLayer->addAction(mActionPasteStyle);
    menuLayer->addSeparator();
    
    menuLayer->addAction(mActionOpenTable);
    menuLayer->addAction(mActionToggleEditing);
    menuLayer->addAction(mActionSaveLayerEdits);
    menuLayer->addAction(mActionAllEdits);
    menuLayer->addSeparator();

    menuLayer->addAction(mActionLayerSaveAs);
    menuLayer->addAction(mActionLayerSelectionSaveAs);

    menuLayer->addAction(mActionRemoveLayer);
    menuLayer->addAction(mActionDuplicateLayer);
    menuLayer->addAction(mActionSetLayerCRS);
    menuLayer->addAction(mActionSetProjectCRSFromLayer);
    menuLayer->addAction(mActionLayerProperties);
    menuLayer->addAction(mActionLayerSubsetString);

    mpMenuBar->addMenu(menuLayer);
    mpMenuMap.insert(tr("Layer"), menuLayer);

    QMenu * menuLayerEdits = new QMenu(tr("��������������"));
    menuLayerEdits->addAction(mActionAllEdits);
    menuLayerEdits->addAction(mActionToggleEditing);
    menuLayerEdits->addAction(mActionSaveLayerEdits);
    menuLayerEdits->addSeparator();
    menuLayerEdits->addAction(mActionAddFeature);
    menuLayerEdits->addAction(mActionMoveFeature);
    menuLayerEdits->addAction(mActionNodeTool);
    menuLayerEdits->addSeparator();
    menuLayerEdits->addAction(mActionDeleteSelected);
    menuLayerEdits->addAction(mActionCutFeatures);
    menuLayerEdits->addAction(mActionCopyFeatures);
    menuLayerEdits->addAction(mActionPasteFeatures);

    mpMenuBar->addMenu(menuLayerEdits);
    mpMenuMap.insert(tr("Edit"), menuLayerEdits);


    QMenu * menuTools = new QMenu(tr("�����������"));
    menuTools->addAction(mpActionPan);
    menuTools->addAction(mActionPanToSelected);
    menuTools->addAction(mpActionZoomIn);
    menuTools->addAction(mpActionZoomOut);
    menuTools->addSeparator();

    menuTools->addAction(mActionZoomFullExtent);
    menuTools->addAction(mActionZoomToLayer);
    menuTools->addAction(mActionZoomToSelected);
    menuTools->addAction(mActionZoomLast);
    menuTools->addAction(mActionZoomNext);
    menuTools->addAction(mActionZoomActualSize);
    menuTools->addSeparator();

    menuTools->addAction(mActionAllSelect);
    menuTools->addAction(mActionIdentify);
    menuTools->addAction(mActionMapTips);
    menuTools->addAction(mActionAllMeasure);
    menuTools->addSeparator();

    menuTools->addAction(mActionDraw);

    mpMenuBar->addMenu(menuTools);

    mpMenuMap.insert(tr("Tools"), menuTools);

    if(m_bWithAddons){
        QMenu * menuTasks = new QMenu(tr("������"));
//        menuTasks->addAction(mpVectorize);
        menuTasks->addAction(mpActionBathymetry);

        QMenu * menuTaskShortestPath = new QMenu(tr("����� ��������� ������������ �������� ������������")); // Az
        menuTaskShortestPath->addAction(mpActionShortestPathAreaSelect); // Az
        menuTaskShortestPath->addAction(mpActionShortestPathGrid); // Az
        menuTaskShortestPath->addAction(mpActionShortestPathCalc); // Az
        menuTasks->addMenu(menuTaskShortestPath);  // Az

        mpMenuBar->addMenu(menuTasks);
        mpMenuMap.insert(tr("Tasks"), menuTasks);
    }
}

QMenuBar * KKSGISWidgetBase::menuBar() const
{
    return mpMenuBar;
}

const QMap<QString, QMenu *> & KKSGISWidgetBase::menuMap() const
{
    return mpMenuMap;
}

const QMap<QString, QToolBar *> & KKSGISWidgetBase::toolBarMap() const
{
    return mpToolBarMap;
}

void KKSGISWidgetBase::initCanvasTools()
{
  // create tools
  mMapTools.mZoomIn = new QgsMapToolZoom( mpMapCanvas, false /* zoomIn */ );
  mMapTools.mZoomIn->setAction( mpActionZoomIn );
  
  mMapTools.mZoomOut = new QgsMapToolZoom( mpMapCanvas, true /* zoomOut */ );
  mMapTools.mZoomOut->setAction( mpActionZoomOut );
  
  mMapTools.mPan = new QgsMapToolPan( mpMapCanvas );
  mMapTools.mPan->setAction( mpActionPan );

#ifdef HAVE_TOUCH
  mMapTools.mTouch = new QgsMapToolTouch( mMapCanvas );
  mMapTools.mTouch->setAction( mActionTouch );
#endif

  mMapTools.mIdentify = new QgsMapToolIdentifyAction( mpMapCanvas );
  ((QgsMapToolIdentifyAction*)mMapTools.mIdentify)->setWorkingWidget(this);
  mMapTools.mIdentify->setAction( mActionIdentify );
  connect( mMapTools.mIdentify, SIGNAL( copyToClipboard( QgsFeatureStore & ) ),
           this, SLOT( copyFeatures( QgsFeatureStore & ) ) );

  //mMapTools.mFeatureAction = new QgsMapToolFeatureAction( mMapCanvas );
  //mMapTools.mFeatureAction->setAction( mActionFeatureAction );
  mMapTools.mMeasureDist = new QgsMeasureTool( mpMapCanvas, false /* area */ );
  mMapTools.mMeasureDist->setAction( mActionMeasure );
  mMapTools.mMeasureArea = new QgsMeasureTool( mpMapCanvas, true /* area */ );
  mMapTools.mMeasureArea->setAction( mActionMeasureArea );
  mMapTools.mMeasureAngle = new QgsMapToolMeasureAngle( mpMapCanvas );
  mMapTools.mMeasureAngle->setAction( mActionMeasureAngle );
  
  //mMapTools.mTextAnnotation = new QgsMapToolTextAnnotation( mpMapCanvas );
  //mMapTools.mTextAnnotation->setAction( mActionTextAnnotation );
  //mMapTools.mFormAnnotation = new QgsMapToolFormAnnotation( mMapCanvas );
  //mMapTools.mFormAnnotation->setAction( mActionFormAnnotation );
  //mMapTools.mHtmlAnnotation = new QgsMapToolHtmlAnnotation( mMapCanvas );
  //mMapTools.mHtmlAnnotation->setAction( mActionHtmlAnnotation );
  //mMapTools.mSvgAnnotation = new QgsMapToolSvgAnnotation( mMapCanvas );
  //mMapTools.mSvgAnnotation->setAction( mActionSvgAnnotation );
  //mMapTools.mAnnotation = new QgsMapToolAnnotation( mMapCanvas );
  //mMapTools.mAnnotation->setAction( mActionAnnotation );
  mMapTools.mAddFeature = new QgsMapToolAddFeature( this,  mpMapCanvas );
  mMapTools.mAddFeature->setAction( mActionAddFeature );
  mMapTools.mMoveFeature = new QgsMapToolMoveFeature( this, mpMapCanvas );
  mMapTools.mMoveFeature->setAction( mActionMoveFeature );
  //mMapTools.mRotateFeature = new QgsMapToolRotateFeature( mMapCanvas );
  //mMapTools.mRotateFeature->setAction( mActionRotateFeature );
  //need at least geos 3.3 for OffsetCurve tool
//#if defined(GEOS_VERSION_MAJOR) && defined(GEOS_VERSION_MINOR) && \
//  ((GEOS_VERSION_MAJOR>3) || ((GEOS_VERSION_MAJOR==3) && (GEOS_VERSION_MINOR>=3)))
//  mMapTools.mOffsetCurve = new QgsMapToolOffsetCurve( mMapCanvas );
//  mMapTools.mOffsetCurve->setAction( mActionOffsetCurve );
//#else
//  mAdvancedDigitizeToolBar->removeAction( mActionOffsetCurve );
//  mEditMenu->removeAction( mActionOffsetCurve );
//  mMapTools.mOffsetCurve = 0;
//#endif //GEOS_VERSION
  //mMapTools.mReshapeFeatures = new QgsMapToolReshape( mMapCanvas );
  //mMapTools.mReshapeFeatures->setAction( mActionReshapeFeatures );
  //mMapTools.mSplitFeatures = new QgsMapToolSplitFeatures( mMapCanvas );
  //mMapTools.mSplitFeatures->setAction( mActionSplitFeatures );
  //mMapTools.mSplitParts = new QgsMapToolSplitParts( mMapCanvas );
  //mMapTools.mSplitParts->setAction( mActionSplitParts );
  
  QgsMapToolSelectUtils::setWorkingWidget(this);

  mMapTools.mSelect = new QgsMapToolSelect( mpMapCanvas );
  mMapTools.mSelect->setAction( mActionSelect );

  mMapTools.mSelectRectangle = new QgsMapToolSelectRectangle( mpMapCanvas );
  mMapTools.mSelectRectangle->setAction( mActionSelectRectangle );

  mMapTools.mSelectPolygon = new QgsMapToolSelectPolygon( mpMapCanvas );
  mMapTools.mSelectPolygon->setAction( mActionSelectPolygon );

  mMapTools.mSelectFreehand = new QgsMapToolSelectFreehand( mpMapCanvas );
  mMapTools.mSelectFreehand->setAction( mActionSelectFreehand );

  mMapTools.mSelectRadius = new QgsMapToolSelectRadius( mpMapCanvas );
  mMapTools.mSelectRadius->setAction( mActionSelectRadius );

  //mMapTools.mAddRing = new QgsMapToolAddRing( mpMapCanvas );
  //mMapTools.mAddRing->setAction( mActionAddRing );
  //mMapTools.mAddPart = new QgsMapToolAddPart( mMapCanvas );
  //mMapTools.mSimplifyFeature = new QgsMapToolSimplify( mMapCanvas );
  //mMapTools.mSimplifyFeature->setAction( mActionSimplifyFeature );
  //mMapTools.mDeleteRing = new QgsMapToolDeleteRing( mMapCanvas );
  //mMapTools.mDeleteRing->setAction( mActionDeleteRing );
  //mMapTools.mDeletePart = new QgsMapToolDeletePart( mMapCanvas );
  //mMapTools.mDeletePart->setAction( mActionDeletePart );
  mMapTools.mNodeTool = new QgsMapToolNodeTool( this, mpMapCanvas );
  mMapTools.mNodeTool->setAction( mActionNodeTool );
  //mMapTools.mRotatePointSymbolsTool = new QgsMapToolRotatePointSymbols( mMapCanvas );
  //mMapTools.mRotatePointSymbolsTool->setAction( mActionRotatePointSymbols );
  //mMapTools.mPinLabels = new QgsMapToolPinLabels( mMapCanvas );
  //mMapTools.mPinLabels->setAction( mActionPinLabels );
  //mMapTools.mShowHideLabels = new QgsMapToolShowHideLabels( mMapCanvas );
  //mMapTools.mShowHideLabels->setAction( mActionShowHideLabels );
  //mMapTools.mMoveLabel = new QgsMapToolMoveLabel( mMapCanvas );
  //mMapTools.mMoveLabel->setAction( mActionMoveLabel );

  //mMapTools.mRotateLabel = new QgsMapToolRotateLabel( mMapCanvas );
  //mMapTools.mRotateLabel->setAction( mActionRotateLabel );
  //mMapTools.mChangeLabelProperties = new QgsMapToolChangeLabelProperties( mMapCanvas );
  //mMapTools.mChangeLabelProperties->setAction( mActionChangeLabelProperties );

  //ensure that non edit tool is initialised or we will get crashes in some situations
  mNonEditMapTool = mMapTools.mPan;
}

/*
void KKSGISWidgetBase::initMapTools()
{
    //create the maptools
    mpPanTool = new QgsMapToolPan(mpMapCanvas);
    mpPanTool->setAction(mpActionPan);

    mpZoomInTool = new QgsMapToolZoom(mpMapCanvas, FALSE); // false = in
    mpZoomInTool->setAction(mpActionZoomIn);
    
    mpZoomOutTool = new QgsMapToolZoom(mpMapCanvas, TRUE ); //true = out
    mpZoomOutTool->setAction(mpActionZoomOut);

    mpSelect = new QgsMapToolS
}
*/

void KKSGISWidgetBase::azSetTitleWindow(QWidget & azApp)

{
      QString caption = "��� �����-��";
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


void KKSGISWidgetBase::userScale()
{
    if(!mScaleEdit)
        return;

  // Why has MapCanvas the scale inverted?
  mpMapCanvas->zoomScale( 1.0 / mScaleEdit->scale() );
}

void KKSGISWidgetBase::showScale( double theScale )
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

void KKSGISWidgetBase::updateMouseCoordinatePrecision()
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

void KKSGISWidgetBase::removeLayer(bool promptConfirmation )
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

  //validate selection
  int numberOfRemovedItems = mpMapLegend->selectedItems().size();
  if ( numberOfRemovedItems == 0 )
  {
    messageBar()->pushMessage( tr( "No Object Selected" ),
                               tr( "To remove objects, you must select them in the legend" ),
                               QgsMessageBar::INFO, messageTimeout() );
    return;
  }
  //display a warning
  if ( promptConfirmation && QMessageBox::warning( this, tr( "Remove objects" ), tr( "Remove %n object(s)?", "number of objects to remove", numberOfRemovedItems ), QMessageBox::Ok | QMessageBox::Cancel ) == QMessageBox::Cancel )
  {
    return;
  }

  mpMapLegend->removeSelectedLayers();

  showStatusMessage( tr( "%n object(s) removed.", "number of objects removed", numberOfRemovedItems ) );

  mpMapCanvas->refresh();
}

void KKSGISWidgetBase::copyStyle( QgsMapLayer * sourceLayer )
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

void KKSGISWidgetBase::pasteStyle( QgsMapLayer * destinationLayer )
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
      emit dataChanged();
      mpMapCanvas->refresh();
    }
  }
}

void KKSGISWidgetBase::setLayerCRS()
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

void KKSGISWidgetBase::setProjectCRSFromLayer()
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

void KKSGISWidgetBase::duplicateLayers( QList<QgsMapLayer *> lyrList )
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

void KKSGISWidgetBase::attributeTable()
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

  QgsAttributeTableDialog *mDialog = new QgsAttributeTableDialog( this, myLayer, this );
  mDialog->show();
  // the dialog will be deleted by itself on close
}


void KKSGISWidgetBase::saveAsRasterFile()
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

void KKSGISWidgetBase::saveAsFile()
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

void KKSGISWidgetBase::saveSelectionAsVectorFile()
{
  saveAsVectorFileGeneral( true );
}

void KKSGISWidgetBase::saveAsVectorFileGeneral( bool saveOnlySelection, QgsVectorLayer* vlayer, bool symbologyOption )
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

void KKSGISWidgetBase::addDatabaseLayers( QStringList const & layerPathList, QString const & providerKey )
{
  QList<QgsMapLayer *> myList;
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  if ( layerPathList.empty() )
  {
    // no layers to add so bail out, but
    // allow mMapCanvas to handle events
    // first
    mpMapCanvas->freeze( false );
    return;
  }

  mpMapCanvas->freeze( true );

  QApplication::setOverrideCursor( Qt::WaitCursor );

  foreach ( QString layerPath, layerPathList )
  {
    // create the layer
    QgsDataSourceURI uri( layerPath );

    QgsVectorLayer *layer = new QgsVectorLayer( uri.uri(), uri.table(), providerKey );
    Q_CHECK_PTR( layer );

    if ( ! layer )
    {
      mpMapCanvas->freeze( false );
      QApplication::restoreOverrideCursor();

      // XXX insert meaningful whine to the user here
      return;
    }

    if ( layer->isValid() )
    {
      // add to list of layers to register
      //with the central layers registry
      myList << layer;
    }
    else
    {
      qWarning() << tr( "%1 is an invalid layer - not loaded" ).arg( layerPath );
      //QLabel *msgLabel = new QLabel( tr( "%1 is an invalid layer and cannot be loaded. Please check the <a href=\"#messageLog\">message log</a> for further info." ).arg( layerPath ), messageBar() );
      //msgLabel->setWordWrap( true );
      //connect( msgLabel, SIGNAL( linkActivated( QString ) ), mLogDock, SLOT( show() ) );
      //QgsMessageBarItem *item = new QgsMessageBarItem( msgLabel, QgsMessageBar::WARNING );
      //messageBar()->pushItem( item );
      delete layer;
    }
    //qWarning("incrementing iterator");
  }

  QgsMapLayerRegistry::instance()->addMapLayers( myList );
  statusBar()->showMessage( mpMapCanvas->extent().toString( 2 ) );

  // update UI
  qApp->processEvents();

  // draw the map
  mpMapCanvas->freeze( false );
  mpMapCanvas->refresh();

  QApplication::restoreOverrideCursor();
}

bool KKSGISWidgetBase::addVectorLayers( QStringList const & theLayerQStringList, const QString& enc, const QString dataSourceType )
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

void KKSGISWidgetBase::layerSubsetString()
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
        emit dataChanged();
      }
    }
  }

  // delete the query builder object
  delete qb;
}

// restore any application settings stored in QSettings
void KKSGISWidgetBase::readSettings()
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

void KKSGISWidgetBase::setTheme( QString theThemeName )
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
  mActionNewProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileNew.svg" ) );
  mActionProjectProperties->setIcon( QgsApplication::getThemeIcon( "/mActionProjectProperties.png" ) );
  //mActionOpenProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileOpen.svg" ) );
  //mActionSaveProject->setIcon( QgsApplication::getThemeIcon( "/mActionFileSave.svg" ) );
  //mActionSaveProjectAs->setIcon( QgsApplication::getThemeIcon( "/mActionFileSaveAs.svg" ) );
  
 
  //mActionExit->setIcon( QgsApplication::getThemeIcon( "/mActionFileExit.png" ) );
  //mActionAddOgrLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddOgrLayer.svg" ) );
  //mActionAddRasterLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddRasterLayer.svg" ) );
//#ifdef HAVE_POSTGRESQL
//  mActionAddPgLayer->setIcon( QgsApplication::getThemeIcon( "/mActionAddPostgisLayer.svg" ) );
//#endif
  
  mActionPasteStyle->setIcon( QgsApplication::getThemeIcon( "/mActionEditPaste.png" ) );
  mActionCopyStyle->setIcon( QgsApplication::getThemeIcon( "/mActionEditCopy.png" ) );
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
  
  mActionAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionAllEdits.svg" ) );
  mActionToggleEditing->setIcon( QgsApplication::getThemeIcon( "/mActionToggleEditing.svg" ) );
  mActionSaveLayerEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveAllEdits.svg" ) );
  mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePoint.png" ) );
  mActionMoveFeature->setIcon( QgsApplication::getThemeIcon( "/mActionMoveFeature.png" ) );
  mActionNodeTool->setIcon( QgsApplication::getThemeIcon( "/mActionNodeTool.png" ) );
  mActionDeleteSelected->setIcon( QgsApplication::getThemeIcon( "/mActionDeleteSelected.svg" ) );
  mActionCutFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionEditCut.png" ) );
  mActionCopyFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionEditCopy.png" ) );
  mActionPasteFeatures->setIcon( QgsApplication::getThemeIcon( "/mActionEditPaste.png" ) );
  //--
  mActionSaveEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveEdits.svg" ) );
  mActionRollbackEdits->setIcon( QgsApplication::getThemeIcon( "/mActionRollbackEdits.svg" ) );
  mActionCancelEdits->setIcon( QgsApplication::getThemeIcon( "/mActionCancelEdits.svg" ) );
  mActionSaveAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionSaveAllEdits.svg" ) );
  mActionRollbackAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionRollbackAllEdits.svg" ) );
  mActionCancelAllEdits->setIcon( QgsApplication::getThemeIcon( "/mActionCancelAllEdits.svg" ) );
  
  
  //mActionUndo->setIcon( QgsApplication::getThemeIcon( "/mActionUndo.png" ) );
  //mActionRedo->setIcon( QgsApplication::getThemeIcon( "/mActionRedo.png" ) );
  
  mActionOpenTable->setIcon( QgsApplication::getThemeIcon( "/mActionOpenTable.png" ) );
  mActionLayerProperties->setIcon( QgsApplication::getThemeIcon( "/mActionProjectProperties.png" ) );
  //mActionLabeling->setIcon( QgsApplication::getThemeIcon( "/mActionLabeling.png" ) );


  mActionPanToSelected->setIcon( QgsApplication::getThemeIcon( "/mActionPanToSelected.svg" ) );

  mActionZoomFullExtent->setIcon( QgsApplication::getThemeIcon( "/mActionZoomFullExtent.svg" ) );
  mActionZoomToSelected->setIcon( QgsApplication::getThemeIcon( "/mActionZoomToSelected.svg" ) );
  mActionZoomLast->setIcon( QgsApplication::getThemeIcon( "/mActionZoomLast.svg" ) );
  mActionZoomNext->setIcon( QgsApplication::getThemeIcon( "/mActionZoomNext.svg" ) );
  mActionZoomToLayer->setIcon( QgsApplication::getThemeIcon( "/mActionZoomToLayer.svg" ) );
  mActionZoomActualSize->setIcon( QgsApplication::getThemeIcon( "/mActionZoomActual.svg" ) );

  mActionDraw->setIcon( QgsApplication::getThemeIcon( "/mActionDraw.svg" ) );

  mActionIdentify->setIcon( QgsApplication::getThemeIcon( "/mActionIdentify.svg" ) );
  mActionMapTips->setIcon( QgsApplication::getThemeIcon( "/mActionMapTips.png" ) );

  mActionSelect->setIcon( QgsApplication::getThemeIcon( "/mActionSelect.svg" ) );
  mActionSelectRectangle->setIcon( QgsApplication::getThemeIcon( "/mActionSelectRectangle.svg" ) );
  mActionSelectPolygon->setIcon( QgsApplication::getThemeIcon( "/mActionSelectPolygon.svg" ) );
  mActionSelectFreehand->setIcon( QgsApplication::getThemeIcon( "/mActionSelectFreehand.svg" ) );
  mActionSelectRadius->setIcon( QgsApplication::getThemeIcon( "/mActionSelectRadius.svg" ) );
  mActionDeselectAll->setIcon( QgsApplication::getThemeIcon( "/mActionDeselectAll.svg" ) );
  mActionSelectByExpression->setIcon( QgsApplication::getThemeIcon( "/mIconExpressionSelect.svg" ) );

  mActionMeasure->setIcon( QgsApplication::getThemeIcon( "/mActionMeasure.png" ) );
  mActionMeasureArea->setIcon( QgsApplication::getThemeIcon( "/mActionMeasureArea.png" ) );
  mActionMeasureAngle->setIcon( QgsApplication::getThemeIcon( "/mActionMeasureAngle.png" ) );



  mpActionBathymetry->setIcon( QgsApplication::getThemeIcon( "/histogram.png" ) );

}

// slot to update the progress bar in the status bar
void KKSGISWidgetBase::showProgress( int theProgress, int theTotalSteps )
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

void KKSGISWidgetBase::legendLayerSelectionChanged( void )
{
  mActionRemoveLayer->setEnabled( mpMapLegend && mpMapLegend->selectedLayers().size() > 0 );
  mActionDuplicateLayer->setEnabled( mpMapLegend && mpMapLegend->selectedLayers().size() > 0 );
  mActionSetLayerCRS->setEnabled( mpMapLegend && mpMapLegend->selectedLayers().size() > 0 );
  mActionSetProjectCRSFromLayer->setEnabled( mpMapLegend && mpMapLegend->selectedLayers().size() == 1 );

  mActionSaveEdits->setEnabled( mpMapLegend && mpMapLegend->selectedLayersEditable( true ) );
  mActionRollbackEdits->setEnabled( mpMapLegend && mpMapLegend->selectedLayersEditable( true ) );
  mActionCancelEdits->setEnabled( mpMapLegend && mpMapLegend->selectedLayersEditable() );
}

void KKSGISWidgetBase::activateDeactivateLayerRelatedActions( QgsMapLayer* layer )
{
  bool enableMove = false, enableRotate = false, enablePin = false, enableShowHide = false, enableChange = false;

  QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
  for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); ++it )
  {
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( it.value() );
    if ( !vlayer || !vlayer->isEditable() ||
         ( !vlayer->diagramRenderer() && vlayer->customProperty( "labeling" ).toString() != QString( "pal" ) ) )
      continue;

    int colX, colY, colShow, colAng;
    /*
    enablePin =
      enablePin ||
      ( qobject_cast<QgsMapToolPinLabels*>( mpMapTools.mPinLabels ) &&
        qobject_cast<QgsMapToolPinLabels*>( mpMapTools.mPinLabels )->layerCanPin( vlayer, colX, colY ) );
    */
    /*
    enableShowHide =
      enableShowHide ||
      ( qobject_cast<QgsMapToolShowHideLabels*>( mpMapTools.mShowHideLabels ) &&
        qobject_cast<QgsMapToolShowHideLabels*>( mpMapTools.mShowHideLabels )->layerCanShowHide( vlayer, colShow ) );
    */
    /*
    enableMove =
      enableMove ||
      ( qobject_cast<QgsMapToolMoveLabel*>( mpMapTools.mMoveLabel ) &&
        ( qobject_cast<QgsMapToolMoveLabel*>( mpMapTools.mMoveLabel )->labelMoveable( vlayer, colX, colY )
          || qobject_cast<QgsMapToolMoveLabel*>( mpMapTools.mMoveLabel )->diagramMoveable( vlayer, colX, colY ) )
      );
    */
    /*
    enableRotate =
      enableRotate ||
      ( qobject_cast<QgsMapToolRotateLabel*>( mpMapTools.mRotateLabel ) &&
        qobject_cast<QgsMapToolRotateLabel*>( mpMapTools.mRotateLabel )->layerIsRotatable( vlayer, colAng ) );
    */

    enableChange = true;

    if ( enablePin && enableShowHide && enableMove && enableRotate && enableChange )
      break;
  }

  /*
  mActionPinLabels->setEnabled( enablePin );
  mActionShowHideLabels->setEnabled( enableShowHide );
  mActionMoveLabel->setEnabled( enableMove );
  mActionRotateLabel->setEnabled( enableRotate );
  mActionChangeLabelProperties->setEnabled( enableChange );
  */
    /*
  mMenuPasteAs->setEnabled( clipboard() && !clipboard()->empty() );
  mActionPasteAsNewVector->setEnabled( clipboard() && !clipboard()->empty() );
  mActionPasteAsNewMemoryVector->setEnabled( clipboard() && !clipboard()->empty() );
  */

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
    //mActionLabeling->setEnabled( false );
    mActionOpenTable->setEnabled( false );
    //mActionOpenFieldCalc->setEnabled( false );
    mActionToggleEditing->setEnabled( false );
    mActionToggleEditing->setChecked( false );
    mActionSaveLayerEdits->setEnabled( false );
    mActionLayerSaveAs->setEnabled( false );
    mActionLayerSelectionSaveAs->setEnabled( false );
    mActionLayerProperties->setEnabled( false );
    mActionLayerSubsetString->setEnabled( false );
    //mActionAddToOverview->setEnabled( false );
    //mActionFeatureAction->setEnabled( false );
    mActionAddFeature->setEnabled( false );
    mActionMoveFeature->setEnabled( false );
    //mActionRotateFeature->setEnabled( false );
    //mActionOffsetCurve->setEnabled( false );
    mActionNodeTool->setEnabled( false );
    mActionDeleteSelected->setEnabled( false );
    mActionCutFeatures->setEnabled( false );
    mActionCopyFeatures->setEnabled( false );
    mActionPasteFeatures->setEnabled( false );
    mActionCopyStyle->setEnabled( false );
    mActionPasteStyle->setEnabled( false );

    //mUndoWidget->dockContents()->setEnabled( false );
    //mActionUndo->setEnabled( false );
    //mActionRedo->setEnabled( false );
    //mActionSimplifyFeature->setEnabled( false );
    //mActionAddRing->setEnabled( false );
    //mActionFillRing->setEnabled( false );
    //mActionAddPart->setEnabled( false );
    //mActionDeleteRing->setEnabled( false );
    //mActionDeletePart->setEnabled( false );
    //mActionReshapeFeatures->setEnabled( false );
    //mActionOffsetCurve->setEnabled( false );
    //mActionSplitFeatures->setEnabled( false );
    //mActionSplitParts->setEnabled( false );
    //mActionMergeFeatures->setEnabled( false );
    //mActionMergeFeatureAttributes->setEnabled( false );
    //mActionRotatePointSymbols->setEnabled( false );

    //mActionPinLabels->setEnabled( false );
    //mActionShowHideLabels->setEnabled( false );
    //mActionMoveLabel->setEnabled( false );
    //mActionRotateLabel->setEnabled( false );
    //mActionChangeLabelProperties->setEnabled( false );

    //mActionLocalHistogramStretch->setEnabled( false );
    //mActionFullHistogramStretch->setEnabled( false );
    //mActionLocalCumulativeCutStretch->setEnabled( false );
    //mActionFullCumulativeCutStretch->setEnabled( false );
    //mActionIncreaseBrightness->setEnabled( false );
    //mActionDecreaseBrightness->setEnabled( false );
    //mActionIncreaseContrast->setEnabled( false );
    //mActionDecreaseContrast->setEnabled( false );
    mActionZoomActualSize->setEnabled( false );
    mActionZoomToLayer->setEnabled( false );
    return;
  }

  mActionLayerProperties->setEnabled( QgsProject::instance()->layerIsEmbedded( layer->id() ).isEmpty() );
  //mActionAddToOverview->setEnabled( true );
  mActionZoomToLayer->setEnabled( true );

  mActionCopyStyle->setEnabled( true );
  mActionPasteStyle->setEnabled( clipboard()->hasFormat( QGSCLIPBOARD_STYLE_MIME ) );

  //***********Vector layers****************
  if ( layer->type() == QgsMapLayer::VectorLayer )
  {
    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
    QgsVectorDataProvider* dprovider = vlayer->dataProvider();

    bool isEditable = vlayer->isEditable();
    bool layerHasSelection = vlayer->selectedFeatureCount() > 0;
    //bool layerHasActions = vlayer->actions()->size() + QgsMapLayerActionRegistry::instance()->mapLayerActions( vlayer ).size() > 0;

    bool canChangeAttributes = dprovider->capabilities() & QgsVectorDataProvider::ChangeAttributeValues;
    bool canDeleteFeatures = dprovider->capabilities() & QgsVectorDataProvider::DeleteFeatures;
    bool canAddAttributes = dprovider->capabilities() & QgsVectorDataProvider::AddAttributes;
    bool canAddFeatures = dprovider->capabilities() & QgsVectorDataProvider::AddFeatures;
    bool canSupportEditing = dprovider->capabilities() & QgsVectorDataProvider::EditingCapabilities;
    bool canChangeGeometry = dprovider->capabilities() & QgsVectorDataProvider::ChangeGeometries;

    //mActionLocalHistogramStretch->setEnabled( false );
    //mActionFullHistogramStretch->setEnabled( false );
    //mActionLocalCumulativeCutStretch->setEnabled( false );
    //mActionFullCumulativeCutStretch->setEnabled( false );
    //mActionIncreaseBrightness->setEnabled( false );
    //mActionDecreaseBrightness->setEnabled( false );
    //mActionIncreaseContrast->setEnabled( false );
    //mActionDecreaseContrast->setEnabled( false );
    mActionZoomActualSize->setEnabled( false );
    //mActionLabeling->setEnabled( true );

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
    //mActionFeatureAction->setEnabled( layerHasActions );

    if ( !isEditable && mpMapCanvas->mapTool()
         && mpMapCanvas->mapTool()->isEditTool() && !mSaveRollbackInProgress )
    {
      mpMapCanvas->setMapTool( mNonEditMapTool );
    }

    if ( dprovider )
    {
      mActionLayerSubsetString->setEnabled( !isEditable && dprovider->supportsSubsetString() );

      mActionToggleEditing->setEnabled( canSupportEditing && !vlayer->isReadOnly() );
      mActionToggleEditing->setChecked( canSupportEditing && isEditable );
      mActionSaveLayerEdits->setEnabled( canSupportEditing && isEditable && vlayer->isModified() );
      //mUndoWidget->dockContents()->setEnabled( canSupportEditing && isEditable );
      //mActionUndo->setEnabled( canSupportEditing );
      //mActionRedo->setEnabled( canSupportEditing );

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
        //mActionMergeFeatures->setEnabled( layerHasSelection && canDeleteFeatures && canAddFeatures );
        //mActionMergeFeatureAttributes->setEnabled( layerHasSelection );
      }
      else
      {
        //mActionMergeFeatures->setEnabled( false );
        //mActionMergeFeatureAttributes->setEnabled( false );
      }

      // moving enabled if geometry changes are supported
      //mActionAddPart->setEnabled( isEditable && canChangeGeometry );
      //mActionDeletePart->setEnabled( isEditable && canChangeGeometry );
      mActionMoveFeature->setEnabled( isEditable && canChangeGeometry );
      //mActionRotateFeature->setEnabled( isEditable && canChangeGeometry );
      mActionNodeTool->setEnabled( isEditable && canChangeGeometry );

      if ( vlayer->geometryType() == QGis::Point )
      {
        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePoint.png" ) );

        //mActionAddRing->setEnabled( false );
        //mActionFillRing->setEnabled( false );
        //mActionReshapeFeatures->setEnabled( false );
        //mActionSplitFeatures->setEnabled( false );
        //mActionSplitParts->setEnabled( false );
        //mActionSimplifyFeature->setEnabled( false );
        //mActionDeleteRing->setEnabled( false );
        //mActionRotatePointSymbols->setEnabled( false );
        //mActionOffsetCurve->setEnabled( false );
        /*
        if ( isEditable && canChangeAttributes )
        {
          if ( QgsMapToolRotatePointSymbols::layerIsRotatable( vlayer ) )
          {
            mActionRotatePointSymbols->setEnabled( true );
          }
        }
        */
      }
      else if ( vlayer->geometryType() == QGis::Line )
      {
        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCaptureLine.png" ) );

        //mActionReshapeFeatures->setEnabled( isEditable && canAddFeatures );
        //mActionSplitFeatures->setEnabled( isEditable && canAddFeatures );
        //mActionSplitParts->setEnabled( isEditable && canAddFeatures );
        //mActionSimplifyFeature->setEnabled( isEditable && canAddFeatures );
        //mActionOffsetCurve->setEnabled( isEditable && canAddFeatures && canChangeAttributes );

        //mActionAddRing->setEnabled( false );
        //mActionFillRing->setEnabled( false );
        //mActionDeleteRing->setEnabled( false );
      }
      else if ( vlayer->geometryType() == QGis::Polygon )
      {
        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionCapturePolygon.png" ) );

        //mActionAddRing->setEnabled( isEditable && canChangeGeometry );
        //mActionFillRing->setEnabled( isEditable && canChangeGeometry );
        //mActionReshapeFeatures->setEnabled( isEditable && canChangeGeometry );
        //mActionSplitFeatures->setEnabled( isEditable && canAddFeatures );
        //mActionSplitParts->setEnabled( isEditable && canChangeGeometry );
        //mActionSimplifyFeature->setEnabled( isEditable && canChangeGeometry );
        //mActionDeleteRing->setEnabled( isEditable && canChangeGeometry );
        //mActionOffsetCurve->setEnabled( false );
      }
      else if ( vlayer->geometryType() == QGis::NoGeometry )
      {
        mActionAddFeature->setIcon( QgsApplication::getThemeIcon( "/mActionNewTableRow.png" ) );
      }

      //mActionOpenFieldCalc->setEnabled( isEditable && ( canChangeAttributes || canAddAttributes ) );

      return;
    }
    else
    {
      //mUndoWidget->dockContents()->setEnabled( false );
      //mActionUndo->setEnabled( false );
      //mActionRedo->setEnabled( false );
    }

    mActionLayerSubsetString->setEnabled( false );
  } //end vector layer block
  //*************Raster layers*************
  else if ( layer->type() == QgsMapLayer::RasterLayer )
  {
    const QgsRasterLayer *rlayer = qobject_cast<const QgsRasterLayer *>( layer );
    if ( rlayer->dataProvider()->dataType( 1 ) != QGis::ARGB32
         && rlayer->dataProvider()->dataType( 1 ) != QGis::ARGB32_Premultiplied )
    {
      if ( rlayer->dataProvider()->capabilities() & QgsRasterDataProvider::Size )
      {
        //mActionFullHistogramStretch->setEnabled( true );
      }
      else
      {
        // it would hang up reading the data for WMS for example
        //mActionFullHistogramStretch->setEnabled( false );
      }
      //mActionLocalHistogramStretch->setEnabled( true );
    }
    else
    {
      //mActionLocalHistogramStretch->setEnabled( false );
      //mActionFullHistogramStretch->setEnabled( false );
    }

    //mActionLocalCumulativeCutStretch->setEnabled( true );
    //mActionFullCumulativeCutStretch->setEnabled( true );
    //mActionIncreaseBrightness->setEnabled( true );
    //mActionDecreaseBrightness->setEnabled( true );
    //mActionIncreaseContrast->setEnabled( true );
    //mActionDecreaseContrast->setEnabled( true );

    mActionLayerSubsetString->setEnabled( false );
    //mActionFeatureAction->setEnabled( false );
    mActionSelect->setEnabled( false );
    mActionSelectRectangle->setEnabled( false );
    mActionSelectPolygon->setEnabled( false );
    mActionSelectFreehand->setEnabled( false );
    mActionSelectRadius->setEnabled( false );
    mActionZoomActualSize->setEnabled( true );
    mActionOpenTable->setEnabled( false );
    //mActionOpenFieldCalc->setEnabled( false );
    mActionToggleEditing->setEnabled( false );
    mActionToggleEditing->setChecked( false );
    mActionSaveLayerEdits->setEnabled( false );
    //mUndoWidget->dockContents()->setEnabled( false );
    //mActionUndo->setEnabled( false );
    //mActionRedo->setEnabled( false );
    mActionLayerSaveAs->setEnabled( true );
    mActionLayerSelectionSaveAs->setEnabled( false );
    mActionAddFeature->setEnabled( false );
    mActionDeleteSelected->setEnabled( false );
    //mActionAddRing->setEnabled( false );
    //mActionFillRing->setEnabled( false );
    //mActionAddPart->setEnabled( false );
    mActionNodeTool->setEnabled( false );
    mActionMoveFeature->setEnabled( false );
    //mActionRotateFeature->setEnabled( false );
    //mActionOffsetCurve->setEnabled( false );
    mActionCopyFeatures->setEnabled( false );
    mActionCutFeatures->setEnabled( false );
    mActionPasteFeatures->setEnabled( false );
    //mActionRotatePointSymbols->setEnabled( false );
    //mActionDeletePart->setEnabled( false );
    //mActionDeleteRing->setEnabled( false );
    //mActionSimplifyFeature->setEnabled( false );
    //mActionReshapeFeatures->setEnabled( false );
    //mActionSplitFeatures->setEnabled( false );
    //mActionSplitParts->setEnabled( false );
    //mActionLabeling->setEnabled( false );

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
  
}


// present a dialog to choose zipitem layers
bool KKSGISWidgetBase::askUserForZipItemLayers( QString path )
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
void KKSGISWidgetBase::askUserForGDALSublayers( QgsRasterLayer *layer )
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
void KKSGISWidgetBase::askUserForOGRSublayers( QgsVectorLayer *layer )
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
void KKSGISWidgetBase::addRasterLayer()
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
bool KKSGISWidgetBase::addRasterLayer( QgsRasterLayer *theRasterLayer )
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
void KKSGISWidgetBase::loadOGRSublayers( QString layertype, QString uri, QStringList list )
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
QgsRasterLayer* KKSGISWidgetBase::addRasterLayerPrivate(const QString & uri, 
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
QgsRasterLayer* KKSGISWidgetBase::addRasterLayer(QString const & rasterFile, 
                                                 QString const & baseName, 
                                                 bool guiWarning )
{
  return addRasterLayerPrivate( rasterFile, baseName, QString(), guiWarning, true );
}

// should the GDAL sublayers dialog should be presented to the user?
bool KKSGISWidgetBase::shouldAskUserForGDALSublayers( QgsRasterLayer *layer )
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
void KKSGISWidgetBase::loadGDALSublayers( QString uri, QStringList list )
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
bool KKSGISWidgetBase::addRasterLayers( QStringList const &theFileNameQStringList, bool guiWarning )
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


QgsClipboard * KKSGISWidgetBase::clipboard()
{
  return mInternalClipboard;
}

void KKSGISWidgetBase::clipboardChanged()
{
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void KKSGISWidgetBase::saveActiveLayerEdits()
{
  saveEdits( activeLayer(), true, true );
}

void KKSGISWidgetBase::saveEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer || !vlayer->isEditable() || !vlayer->isModified() )
    return;

  if ( vlayer == activeLayer() )
    mSaveRollbackInProgress = true;

  if ( !vlayer->commitChanges() )
  {
    mSaveRollbackInProgress = false;
    commitError( vlayer );
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

void KKSGISWidgetBase::saveEdits()
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

void KKSGISWidgetBase::saveAllEdits( bool verifyAction )
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

void KKSGISWidgetBase::editCopy( QgsMapLayer * layerContainingSelection )
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

void KKSGISWidgetBase::editPaste( QgsMapLayer *destinationLayer )
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

void KKSGISWidgetBase::deleteSelected( QgsMapLayer *layer, QWidget* parent )
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

  QString providerName = vlayer->dataProvider()->name();
  QString tableName = "";
  int fIndex = vlayer->fieldNameIndex("unique_id");
  
  /*  �������� ����������� �������� ����������. 
      ������ ���������� postgresprovider

  //ksa
  //���� ���� �� �� DynamicDocs - ������� ������� ���� ������ �������
  if(providerName == "postgres" && fIndex >= 0){ //DynamicDocs layer!
      QString uri = vlayer->dataProvider()->dataSourceUri();
      QStringList uriSections = uri.split(" ");
      QString layerTable;

      for(int i=0; i<uriSections.count(); i++){
          QString & sec = uriSections[i];
          if(sec.startsWith("table=")){
              QStringList tableSec = sec.split("=");
              if(tableSec.count() != 2){
                  return;
              }
              QString fullTableName = tableSec[1];
              QStringList tableNameSec = fullTableName.split(".");
              if(tableNameSec.count() != 2){
                  return;
              }
              tableName = tableNameSec[1];
              tableName.replace("\"", "");

              break;
          }
      }

      QgsFeatureList features = vlayer->selectedFeatures();
      //QgsFeatureIds  fids = vlayer->selectedFeaturesIds();
      QList<qint64> ids;// = fids.values();
      for(int i=0; i<features.count(); i++){
          QgsFeature f = features.at(i);
          int fIndex = f.fieldNameIndex("id");
          if(fIndex < 0)
              return;
          
          QVariant v = f.attribute("id");
          if(v.type() == QVariant::Invalid){
              return;
          }
          ids.append(v.toLongLong());
      }

      deleteFeaturesAsEIO(this, tableName, ids);
      vlayer->updateExtents();
  }
  else{
*/
    if ( !vlayer->deleteSelectedFeatures() )
    {
      messageBar()->pushMessage( tr( "Problem deleting features" ),
                                 tr( "A problem occured during deletion of features" ),
                                 QgsMessageBar::WARNING, messageTimeout() );
    }
    else
    {
      showStatusMessage( tr( "%n feature(s) deleted.", "number of features deleted", numberOfDeletedFeatures ) );
    }
/*
  } //NOT DynamicDocs layer
  */

  vlayer->endEditCommand();
}

bool KKSGISWidgetBase::verifyEditsActionDialog( const QString& act, const QString& upon )
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

QList<QgsMapLayer *> KKSGISWidgetBase::editableLayers( bool modified ) const
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



/*************************************/
/*
   ����� ������ ������, �������������� ������ ������������ � ������
   �����������, ����������, ����������������� � �������������� ������ ��������� � ����� kksgiswidgethelper.cpp
*/

void KKSGISWidgetBase::azRemoveAnnotationItems()
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

void KKSGISWidgetBase::azRemoveAllLayers()
{
    QgsMapLayerRegistry::instance()->removeAllMapLayers();
    //QgsMapLayerRegistry::instance()->clearAllLayerCaches();
}

bool KKSGISWidgetBase::azSelectLayer(const QString layerName)
{
    bool bComplete(false);
    // �������� ��������� ����� �������� ����
    QgsMapLayer * pRezLayer;
    pRezLayer = NULL;
    if (mpSelectedLayer == NULL) // ���� �� ����
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

            break;
        }

    }
    if (pRezLayer != NULL)
    {
        if (!mpSelectedLayer->isValid() && pRezLayer->isValid()) // ���� mpSelectedLayer ��� ������� ������������ �����
        {
            mpSelectedLayer = pRezLayer; // ���������� �������� ����, ������� ���
            bComplete = false; // ��������, ��� ����� ���� ��������

        }
    }
    return bComplete;
}


bool KKSGISWidgetBase::azSelectLayer(const int layerNumber)
{
    bool bComplete(false);
    // �������� ��������� ����� �������� ����
    if (layerNumber > mpRegistry->count())
    {
        return bComplete;
    }

    QgsMapLayer * pRezLayer;
    pRezLayer = NULL;
    if (mpSelectedLayer == NULL) // ���� �� ����
    {
        pRezLayer = mpSelectedLayer;
    }

    int n(0); // ���������� �������
    QMapIterator < QString, QgsMapLayer * > i(this->mpRegistry->mapLayers());
    while (i.hasNext())
    {
        i.next();
        n = n+1;
        if (n == layerNumber)
        {
            mpSelectedLayer = i.value();
            bComplete = true;

            break;
        }

    }
    if (pRezLayer != NULL)
    {
        if (!mpSelectedLayer->isValid() && pRezLayer->isValid()) // ���� mpSelectedLayer ��� ������� ������������ �����
        {
            mpSelectedLayer = pRezLayer; // ���������� �������� ����, ������� ���
            bComplete = false; // ��������, ��� ����� ���� ��������

        }
    }
    return bComplete;
}

bool KKSGISWidgetBase::azSelectByIntersection(QgsVectorLayer * pMainLayer, QgsVectorLayer * pSelectLayer)
{
    mAzDialCalcRoute->mProgressBarText->setText("������� ��������: ��������� ���� '" + pSelectLayer->name() + "'");
    QgsVectorDataProvider *pInputProvider;
    QgsVectorDataProvider *pSelectProvider;
    pInputProvider = pMainLayer->dataProvider();
    pSelectProvider = pSelectLayer->dataProvider();

    int pNumValColumn(-1); // ������� ���������� ��������
    QgsFeature pInFeat; // �������������� ������.������
    QgsFeature pSelectFeat; // ������. ������, ���������� �������� ���������� pInFeat
    QgsSpatialIndex pSpIndex;
    QgsGeometry * pGeom;
    QList<QgsFeatureId> pListIntersects;
    pListIntersects.clear();
    QgsFeatureIterator pFeatInIterator = pInputProvider->getFeatures();
    QgsFeatureIterator pFeatSelectIterator = pSelectProvider->getFeatures();
    quint64 pCount(0); // �������

    // ��������� ������� �� ��������� ���� � ����� SpatialIndex
    while ( pFeatInIterator.nextFeature( pInFeat ))
    {
        pSpIndex.insertFeature(pInFeat);
    }

    pNumValColumn = pMainLayer->fieldNameIndex("val"); // ������ ����� ������� 'val'
    if (pNumValColumn < 0)
    {
        return false; // ��� ����� � ����� ������
    }

    // ����� ��������������
    if ( !( pMainLayer->dataProvider()->capabilities() &
            QgsVectorDataProvider::EditingCapabilities ) )
    {
        return false;
    }
    pMainLayer->startEditing(); // �������� ��������������

    mAzDialCalcRoute->mProgressBar->setMaximum((int)pSelectProvider->featureCount());
    mAzDialCalcRoute->mProgressBar->setValue(0);

    // ������������ ���������� ��� ������� � "����-����������"
    while ( pFeatSelectIterator.nextFeature( pSelectFeat ) )
    {
        pGeom = pSelectFeat.geometry();
        pListIntersects = pSpIndex.intersects(pGeom->boundingBox());
        foreach (QgsFeatureId pFeatureId, pListIntersects)
        {
            pInputProvider->getFeatures(QgsFeatureRequest().setFilterFid(pFeatureId)).nextFeature(pInFeat);
            QgsGeometry * pTempGeom;
            pTempGeom = pInFeat.geometry() ;
            if (pGeom->intersects(pTempGeom))
            {
                // ������ �� ��������, ������� ������������ �������� val = -1
                if (!pMainLayer->changeAttributeValue(pFeatureId, pNumValColumn, QVariant(-1.0), QVariant(0.0)))
                {
                    return false;
                }
            }
        }
        pCount++;
        mAzDialCalcRoute->mProgressBar->setValue(pCount);
    }
    if (!pMainLayer->commitChanges()) // ��������� ���������
    {
        mAzDialCalcRoute->mProgressBarText->setText("���������� �������� ������ � ����");
        return false;
    }
    // �������� ������ ����
    else
    {

//        QgsFeatureIterator pitt = pInputProvider->getFeatures();
//        QgsFeature ff;
//        while (pitt.nextFeature(ff))
//        {
//            QString pss;
//            pss = "id = " + QString::number(ff.id()) + "\n" + "val = '" + ff.attribute(ff.fieldNameIndex("val")).toString() + "'";
//            qDebug(pss.toAscii());
//        }
    }
    pMainLayer->updateExtents();
    return true;
}

bool KKSGISWidgetBase::azRasterEnhancement(QgsRasterLayer & azRasterLayer)
{
    // ������� ��������� �����������
    // ����: �������� ��� ����������� ������ ��� ������������� ����������
    // (�� ���� ������ ���������� ��� ������ ������������� �
    // ������������ ������������������ ���������)
    bool bComplete(false); // �������������� ��������� ��� ������������ ��
                          //    �������� ���������

    /*
      --------------------
// ������� ����������� ������:
    pLayer->setDrawingStyle:
UndefinedDrawingStyle
SingleBandGray
SingleBandPseudoColor
PalettedColor
PalettedSingleBandGray
PalettedSingleBandPseudoColor
PalettedMultiBandColor          - ��� �����
MultiBandSingleGandGray
MultiBandSingleBandGray
MultiBandSingleBandPseudoColor
MultiBandColor
SingleBandColorDataStyle
---------------------------
// �������� �������������:
    pLayer->setColorShadingAlgorithm:
UndefinedShader
PseudoColorShader
FreakOutShader
ColorRampShader
UserDefinedShader
----------------------
    */
    if (azRasterLayer.rasterType() == QgsRasterLayer::Multiband)
    {
//        azRasterLayer.setDrawingStyle(QgsRaster::MultiBandColor); // ������������� "3-� ������� �����������"

        // ���� ����� ���: 3-� ������� ����������� � ������:
        // 4 - �������; 3 - �������; 2 - �����.
        if (azRasterLayer.bandCount() < 3)
        {
            // ������ 3-� ������� �� ��������
        }
        else if (azRasterLayer.bandCount() == 3)
        {
            // �� ��������� ���� ��� �� ����� ���� � ��������

            //ksa -- azRasterLayer.setRedBandName(azRasterLayer.bandName(3));
            //ksa -- azRasterLayer.setGreenBandName(azRasterLayer.bandName(2));
            //ksa -- azRasterLayer.setBlueBandName(azRasterLayer.bandName(1));
            bComplete = true;
        }
        else
        {
//            pRast.BlueBand
                // ��������� �������� ������� ������
            if (azRasterCheckBandName(azRasterLayer, "Band 4"))
            {

//                azRasterLayer.set("Band 4"); // ���� - ���������
            }
            if (azRasterCheckBandName(azRasterLayer, "Band 3"))
            {
//                azRasterLayer.setRedBandName("Band 3");
            }
            if (azRasterCheckBandName(azRasterLayer, "Band 2"))
            {
//                azRasterLayer.setRedBandName("Band 2");
            }
            //ksa -- azRasterLayer.setBlueBandName("Band 2");
            bComplete = true;
        }

//        azRasterLayer.setStandardDeviations(2.5); // ��� �������� �� ����������� �������
                                                           // ���. ������� ������������ ���������� 2.5
    }
    else if (azRasterLayer.rasterType() == QgsRasterLayer::Palette)
    {
        return bComplete;
    }
    else // QgsRasterLayer::GrayOrUndefined
    {
        return bComplete;
    }
    return false;
}

bool KKSGISWidgetBase::azRasterCheckBandName(QgsRasterLayer &azRasterLayer, QString strBandName)
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

bool KKSGISWidgetBase::azCopyFiles(QString azSource, QString azDestPath, bool bUse)
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

/*
bool KKSGISWidgetBase::azMakeLayer(QGis::WkbType azType, QString pDestDir, QString pName)
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

//            QDebug("���� �� ���������� �" + pDestDir.toAscii() + pName.toAscii() + pExp.toAscii());
            bComplete = false;
        }
    }
    return bComplete;
}
*/

bool KKSGISWidgetBase::azAddLayerVector(QFileInfo pFile, bool extent = false)
{
    QString myProviderName      = "ogr";
    QgsVectorLayer * mypLayer = new QgsVectorLayer(pFile.filePath(), pFile.completeBaseName(), myProviderName);
    QgsSymbolV2 * s = QgsSymbolV2::defaultSymbol(mypLayer->geometryType());
    QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(s);//(mypLayer->geometryType());
    //        QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->geometryType());
    mypLayer->setRendererV2(mypRenderer);
    if (!mypLayer->isValid())
    {

//        QDebug("���� ������������ '" + pFile.filePath().toAscii() + "'");
        return false;
    }

    // Add the Vector Layer to the Layer Registry
    mpRegistry->addMapLayer(mypLayer, TRUE);
    connect(mypLayer, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    // Add the Layer to the Layer Set
    //ksa mpLayerSet.append(QgsMapCanvasLayer(mypLayer));
    // set the canvas to the extent of our layer
    if (extent)
    {
        mpMapCanvas->setExtent(mypLayer->extent());
    }
    // Set the Map Canvas Layer Set
    //ksa mpMapCanvas->setLayerSet(mpLayerSet);
    
    // update UI
    qApp->processEvents();
    
    mpMapCanvas->refresh();
    
    return true;
}

void KKSGISWidgetBase::SLOTmpActionVectorize()
{
    this->azVectorize();
    this->azAddWorkListToMap(azWorkList);

}

void KKSGISWidgetBase::azAddWorkListToMap(QStringList &pList)
{
    long i(0);
    QString pMessage("");
    foreach (const QString &pString, pList)
    {
        QFileInfo pFile(pString);
        if (pFile.isFile())
        {
            if (azAddLayerVector(pFile))
            {
                i = i + 1;
                pMessage = "�������� ���� '" + pFile.baseName() + "'";
                mpStatusBar->showMessage(pMessage);
            }
        }
    }
    if (i > 1)
    {
        pMessage = "��������� " + QString::number(i )+ "����(��)";
        mpStatusBar->showMessage(pMessage);
    }
}

void KKSGISWidgetBase::azVectorize()
{
#ifdef WIN32
    dnThemTaskSpecBath->close();
    QString pMessage(""); // ��������� � ������ ���� � ���������� ������������
    if (this->dnThemTaskSpecBath->Polygons.count() < 1)
    {
        pMessage = tr("��������� ������� �� �������� ���������� ��� ������������");
        mpStatusBar->showMessage(pMessage);
        return;
    }

    QString mEncoding; // ���������
    mEncoding = "UTF-8";
    QgsFields pFields; // ����� �����
//    DNVector dnVecForFields;
//    dnVecForFields = dnThemTaskSpecBath->Polygons.at(0);
    QgsField myField1( "value", QVariant::Double, "Double", 0, 0);
    QgsField myField2( "comment", QVariant::String, "String", 10, 0, "Comment" );
    pFields.append( myField1 );
    pFields.append( myField2 );

    QString myFileName (dnThemTaskSpecBath->Polygons.at(0).NameLayer + "_" + this->azCreateName(2) + ".shp");

    QgsCoordinateReferenceSystem pSRS;
    // ������� ������� ��������� ���������� ������
    pSRS.createFromOgcWmsCrs("EPSG:" +
                             QString::number(azGetEPSG(dnThemTaskSpecBath->Polygons.at(0).EPSG)));
    pSRS.validate();

    QgsVectorFileWriter myWriter( myFileName, mEncoding, pFields, QGis::WKBPolygon, &pSRS);

    azWorkList.clear();
    azWorkList.append(myFileName);

    for (long i = 0; i < dnThemTaskSpecBath->Polygons.size(); i++)
    {
        DNVector dnVec;
        dnVec = dnThemTaskSpecBath->Polygons.at(i);
        QgsPolyline pPolyLine;
        QgsPoint pFirstPoint (dnVec.GPt.at(0).x, dnVec.GPt.at(0).y);

        for (long j = 0; j < dnVec.GPt.size(); j++)
        {
            QgsPoint p(dnVec.GPt.at(j).x, dnVec.GPt.at(j).y);
            pPolyLine << p;
        }
        pPolyLine << pFirstPoint;
        QgsPolygon pPolygon;
        pPolygon << pPolyLine;
        QgsGeometry * pPolygonGeometry = QgsGeometry::fromPolygon( pPolygon );
        QgsFeature pFeature;
        pFeature.setGeometry( pPolygonGeometry );
        pFeature.initAttributes(2);
        pFeature.setAttribute(0, (double)dnVec.Vol);
        pFeature.setAttribute(1, "deep");

        QgsVectorFileWriter::WriterError mError;
        myWriter.addFeature ( pFeature );
        mError = myWriter.hasError();
        if (mError != 0)
        {
            qWarning() << myWriter.errorMessage();
        }
    }
    delete dnThemTaskSpecBath;
    dnThemTaskSpecBath = NULL;
    return;
#endif
}


void KKSGISWidgetBase::panMode()
{
    mpMapCanvas->setMapTool(mMapTools.mPan);
}

void KKSGISWidgetBase::zoomInMode()
{
    mpMapCanvas->setMapTool(mMapTools.mZoomIn);
}

void KKSGISWidgetBase::zoomOutMode()
{
    mpMapCanvas->setMapTool(mMapTools.mZoomOut);
}

void KKSGISWidgetBase::azLoadLayer(QgsMapLayer *theMapLayer, bool isExtent)
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

/*
void KKSGISWidgetBase::SLOTmpActionFileExit() // Exit from Application
{
    bool ProjectChange(false); // changing of Project NEEDED TO CONNECT
    QString pMessageText("");
    uint pButtonsPack(0);
    int pButton(0);
 
    if (ProjectChange)
    {
        pButtonsPack = 0x00410800;

    }
    else
    {
        pButtonsPack = 0x00404000;

    }

    QMessageBox * pExitApp = new QMessageBox (QMessageBox::Question,
                                              "����� �� ����������",
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
    
    SLOTmpCloseProject();
}
*/

void KKSGISWidgetBase::SLOTmpCloseProject()
{
    // unload the project macros before changing anything
    //if ( mTrustedMacros ){
    //    QgsPythonRunner::run( "qgis.utils.unloadProjectMacros();" );
    //}

    // remove any message widgets from the message bar
    if(mInfoBar)
        mInfoBar->clearWidgets();

    //mTrustedMacros = false;

    //deletePrintComposers();
    this->azRemoveAnnotationItems();
    // clear out any stuff from project
    mpMapCanvas->freeze( true );
    azRemoveAllLayers();
    
    //ksa mpLayerSet.clear();
    mpMapCanvas->freeze( false);
    mpMapCanvas->refresh();
    mpMapCanvas->freeze( true );

    QgsProject::instance()->setFileName(QString(""));
}

void KKSGISWidgetBase::SLOTmpActionFileOpenProject() // Open QGIS Project in MapDock
{
    QString projectFileName = QFileDialog::getOpenFileName(this, "������� ���� ������� ����� *.qgs", "", "*.qgs");
    if (projectFileName.isNull())
        return;
    
    //TODO ��������, ����� ���� ���� �������� URL ��� postgis-�����...

    openProject(projectFileName);
}

void KKSGISWidgetBase::SLOTsetRenderer()
{
//    QgsMapLayer* layer = mpMapCanvas->layer(0);
//    Q_ASSERT(layer);
//    Q_ASSERT(layer->type() == QgsMapLayer::VectorLayer);
//    QgsVectorLayer* vlayer = static_cast<QgsVectorLayer*>(layer);
//    QgsRendererV2PropertiesDialog dlg(vlayer, QgsStyleV2::defaultStyle());
//    dlg.exec();
    mpMapCanvas->refresh();
}

void KKSGISWidgetBase::SLOTshortestPathSelectArea() // Az
{

    bool bOk (false); // ������ ������ ��� ������� � �������
    QString str = QInputDialog::getText( this,"��������� ���� ��������",
                                         "�������� ����:", QLineEdit::Normal,
                                         "route_army_area_" + this->azCreateName(1) + "_a",
                                         &bOk );
    if (!bOk)
    {
        return; // ������ ������
    }
    QgsFields pFlds;
    if (!azCreateLayer(str + ".shp", pFlds, azSettings->value("MainSettings/SaveDirPath", "C:/").toString(), true))
    {
        return; // ���� �� ������
    }
    this->azAddWorkListToMap(azWorkList);
    activateDeactivateLayerRelatedActions (this->activeLayer());
    this->toggleEditing();
    if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    {
        return;
    }
    mpMapCanvas->setMapTool( mMapTools.mAddFeature );
}

void KKSGISWidgetBase::SLOTshortestPathCalculate()
{
    mAzDialCalcRoute = new AzDialCalcRoute(azSettings, this);
    QMapIterator < QString, QgsMapLayer * > i(QgsMapLayerRegistry::instance()->mapLayers());
    i.toBack();
    QObject::connect(mAzDialCalcRoute->mComboLayerRelief,
                     SIGNAL(currentIndexChanged(int)),
                     this, SLOT(SLOTazChangeColumns(int)));
    QObject::connect(mAzDialCalcRoute, SIGNAL(SIGNALbuttonsClick(bool)),
                     this, SLOT(SLOTshortestPathCalculateMath(bool)));
    while (i.hasPrevious()) // add all layers to dialog's combobox
    {
        i.previous();

        QgsMapLayer * currLayer = i.value();
        if (currLayer->type() == QgsMapLayer::VectorLayer)
        {
            QgsVectorLayer * pVLayer = qobject_cast<QgsVectorLayer *> ( currLayer );
            if (currLayer->isValid() && pVLayer->geometryType() == QGis::Polygon)
            {
                mAzDialCalcRoute->mComboLayerAoI->addItem(currLayer->name());
                mAzDialCalcRoute->mComboLayerForest->addItem(currLayer->name());
                mAzDialCalcRoute->mComboLayerWater->addItem(currLayer->name());
                mAzDialCalcRoute->mComboLayerRelief->addItem(currLayer->name());
            }
        }
    }
    mAzDialCalcRoute->mCoordsRoute.xStart = 1;
    mAzDialCalcRoute->mCoordsRoute.yStart = 1;
    mAzDialCalcRoute->mCoordsRoute.xFinish = 60;
    mAzDialCalcRoute->mCoordsRoute.yFinish = 60;
    mAzDialCalcRoute->setWindowTitle("������ ��������");
    mAzDialCalcRoute->exec();
}

void KKSGISWidgetBase::SLOTshortestPathGridArea()
{
    AzDialGeneral pDialog(this, 1); // dialog
    QMapIterator < QString, QgsMapLayer * > i(QgsMapLayerRegistry::instance()->mapLayers());
    i.toBack();
    while (i.hasPrevious()) // add all layers to dialog's combobox
    {
        i.previous();

        QgsMapLayer * currLayer = i.value();
        if (currLayer->type() == QgsMapLayer::VectorLayer)
        {
            QgsVectorLayer * pVLayer = qobject_cast<QgsVectorLayer *> ( currLayer );
            if (currLayer->isValid() && pVLayer->geometryType() == QGis::Polygon)
            {
                pDialog.mComboBoxOne->addItem(currLayer->name());
            }
        }
    }
    if (pDialog.mComboBoxOne->count() < 1) // no layers
    {
        QMessageBox::information(this, "������������ ������", "���������� ��������� ����, ���������� ������������� �������.", QMessageBox::Ok);
        return;
    }
    pDialog.exec();
    if (!pDialog.mOkClick)
    {
        return; // click "Cancel" or Close
    }
    if(!azSelectLayer(pDialog.mComboBoxOne->currentText()))
    {
        return; // there is no layer select
    }
    double offset = pDialog.mDoubleSpinBox->value();
    if (offset <= 0.00000001)
    {
        QMessageBox::information(this, "������������ ������", "������ �� ��������� �������� ������ ����������� ���� ����������", QMessageBox::Ok);
        return;
    }

    QgsRectangle pRect(mpSelectedLayer->extent());
    QString pStr(azSettings->value("MainSettings/SaveDirPath", "C:/").toString() + "grid_" + azCreateName(1) + ".shp");
    QgsCoordinateReferenceSystem pSRS;
    pSRS.createFromSrsId(mpSelectedLayer->crs().srsid());
    pSRS.validate();
    QgsFields pFields;
    {
        QgsField pField1("id_num", QVariant::Int, "Int");
        QgsField pField2("xMin", QVariant::Double, "Double");
        QgsField pField3("xMax", QVariant::Double, "Double");
        QgsField pField4("yMin", QVariant::Double, "Double");
        QgsField pField5("yMax", QVariant::Double, "Double");
        QgsField pField6("cell_coord", QVariant::String, "String");
        QgsField pField7("cell_x", QVariant::Int, "Int");
        QgsField pField8("cell_y", QVariant::Int, "Int");
        QgsField pField9("dir", QVariant::Double, "Double");
        QgsField pField10("slope", QVariant::Double, "Double");
        QgsField pField11("val", QVariant::Double, "Double");
        QgsField pField12("settings", QVariant::String, "String");


        pFields.append( pField1 );
        pFields.append( pField2 );
        pFields.append( pField3 );
        pFields.append( pField4 );
        pFields.append( pField5 );
        pFields.append( pField6 );
        pFields.append( pField7 );
        pFields.append( pField8 );
        pFields.append( pField9 );
        pFields.append( pField10 );
        pFields.append( pField11 );
        pFields.append( pField12 );
    }
    QgsVectorFileWriter::WriterError pError;
    QgsVectorFileWriter myWriter( pStr, "UTF-8", pFields, QGis::WKBPolygon, &pSRS);
    pError = myWriter.hasError();
    if (pError != 0)
    {
        QMessageBox::information(this, "Error", myWriter.errorMessage(), QMessageBox::Ok);
    }

    quint32 idFeat(0);
    quint64 countY = 1; // x - �������
    quint64 countX = 1; // y - �������
    double y, x;
    y = pRect.yMaximum();
    while (y >= pRect.yMinimum())
    {
        countX = 1;
        x = pRect.xMinimum();
        while (x <= pRect.xMaximum())
        {
            QgsPoint pt1(x, y);
            QgsPoint pt2(x + offset, y);
            QgsPoint pt3(x + offset, y - offset);
            QgsPoint pt4(x, y - offset);
            QgsPoint pt5(x, y);

            QgsPolyline pPolyLine;
            pPolyLine << pt1;
            pPolyLine << pt2;
            pPolyLine << pt3;
            pPolyLine << pt4;
            pPolyLine << pt5;

            QgsPolygon pPolygon;
            pPolygon << pPolyLine;
            QgsGeometry * pPolygonGeometry = QgsGeometry::fromPolygon( pPolygon );
            QgsFeature pFeature;
            pFeature.setGeometry( pPolygonGeometry );
            pFeature.initAttributes(8);
            pFeature.setAttribute(0, (long)idFeat);
            pFeature.setAttribute(1, (double)x);
            pFeature.setAttribute(2, (double)(x + offset));
            pFeature.setAttribute(3, (double)(y - offset));
            pFeature.setAttribute(4, (double)y);
            pFeature.setAttribute(5, QString::number(countX) + ";" + QString::number(countY));
            pFeature.setAttribute(6, (long)countX);
            pFeature.setAttribute(7, (long)countY);
            myWriter.addFeature(pFeature);
            pError = myWriter.hasError();
            if (pError != 0)
            {
                QMessageBox::information(this, "Error", myWriter.errorMessage(), QMessageBox::Ok);
            }
            idFeat++;
            countX++;
            x = x + offset;
        }
        y = y - offset;
        countY++;
    }

    QFileInfo pFile(pStr);
    QString myProviderName = "ogr";
    QgsVectorLayer * pVLayer = new QgsVectorLayer(pFile.filePath(), pFile.completeBaseName(), myProviderName);
    pVLayer->updateExtents();
    this->azAddLayerVector(pFile, true);
}

void KKSGISWidgetBase::SLOTshortestPathCalculateMath(bool CalcIt)
{
    if (!CalcIt) // ������ "������"
    {
        mAzDialCalcRoute->close();
        return;
    }

    bool bSuccess(true); // ��������� ������
    azSelectLayer(mAzDialCalcRoute->mComboLayerAoI->currentText());
    QgsVectorLayer * pAoILayer = qobject_cast<QgsVectorLayer *> ( mpSelectedLayer );
    QStringList pList;
    QMap<QString, QgsFeatureId> pMapPath; // ����� ����
    QgsVectorDataProvider *pVDProvider;
    pVDProvider = pAoILayer->dataProvider();

    // �������� � ���������
    //    QString pStr( "\"cell_x\"=11 AND \"cell_y\"=59" ); \\ ��������� ����� � ���� ��������
    //    QString pStr( "\"cell_coord\" = \'11;59\'" ); \\ ��������� ������ � �������

    // �������� ��������� � �������� �����
    QgsFeature pFeatFinish;
    QgsFeature pFeat;
    if (!pAoILayer->getFeatures(QgsFeatureRequest().setFilterExpression( "\"cell_coord\" = \'" + mAzDialCalcRoute->mStrStart + "\'" )).nextFeature(pFeat)) bSuccess = false;
    if (!pAoILayer->getFeatures(QgsFeatureRequest().setFilterExpression( "\"cell_coord\" = \'" + mAzDialCalcRoute->mStrFinish + "\'" )).nextFeature(pFeatFinish)) bSuccess = false;
    if (!bSuccess)
    {
        QMessageBox::information(this, "Error", "��������� ������ ������/����� �������� �� ����������", QMessageBox::Ok);
        mAzDialCalcRoute->setFinish(true);
        return;
    }
    else
    {
        azSettings->setValue("ShortestPath/StartXY", mAzDialCalcRoute->mStrStart);
        azSettings->setValue("ShortestPath/FinishXY", mAzDialCalcRoute->mStrFinish);
        pMapPath["start"] = pFeat.id();
        pMapPath["finish"] = pFeatFinish.id();
    }

    // ��������� ����� "������������ ���"
    pList.clear();
    pList << mAzDialCalcRoute->mComboLayerWater->currentText()
          << mAzDialCalcRoute->mComboLayerForest->currentText();
//          << mAzDialCalcRoute->mComboLayerRelief->currentText();
    foreach (QString strLayer, pList)
    {
        if (azSelectLayer(strLayer))
        {
            QgsVectorLayer * pCurrLayer = qobject_cast<QgsVectorLayer *> ( mpSelectedLayer );
            if (!azSelectByIntersection(pAoILayer, pCurrLayer))
            {
                QMessageBox::information(this, "Error", "������ � ��������� ���� '" + strLayer + "'", QMessageBox::Ok);
            }
        }
    }

    // ���������� �����
    mAzDialCalcRoute->mProgressBarText->setText("������� ��������: ������������� �����");
    if (!azShortestPathWave(pAoILayer, pMapPath))
    {
        QMessageBox::information(this, "Error", "������ ������������ ����������� ����", QMessageBox::Ok);
        mAzDialCalcRoute->setFinish(false);
        return;
    }

    // ��������� ��������
    mAzDialCalcRoute->mProgressBarText->setText("������� ��������: ������������ ��������� ��������");
    if (!azShortestPathRoute(pAoILayer, pMapPath))
    {
        QMessageBox::information(this, "Error", "������ ��������� ��������", QMessageBox::Ok);
        mAzDialCalcRoute->setFinish(false);
        return;
    }

    mAzDialCalcRoute->close();
    pAoILayer->triggerRepaint();
}


bool KKSGISWidgetBase::azShortestPathWave(QgsVectorLayer *pVectorLayer, QMap<QString, QgsFeatureId> pMap)
{
    bool bSuccess(false);
    bool bWaveMoved(true);
    int pCount(1);
    int pNumValColumn(-1);
    int pNumCellColumn(-1);
    int pNumSettingsColumn(-1);

    pNumValColumn = pVectorLayer->fieldNameIndex("val"); // ������ ����� ������� 'val'
    pNumCellColumn = pVectorLayer->fieldNameIndex("cell_coord");
    pNumSettingsColumn = pVectorLayer->fieldNameIndex("settings");
    if (pNumValColumn < 0 || pNumCellColumn < 0 || pNumSettingsColumn < 0) return false; // ��� ����� � ����� ������

    QgsVectorDataProvider *pProvider;
    pProvider = pVectorLayer->dataProvider();
    if ( !( pVectorLayer->dataProvider()->capabilities() &
            QgsVectorDataProvider::EditingCapabilities ) )
    {
        mAzDialCalcRoute->mProgressBarText->setText("�������������� ����� ����������");
        return false;
    }


    pVectorLayer->startEditing(); // �������� ��������������

    mAzDialCalcRoute->mProgressBar->setMinimum(0);
    mAzDialCalcRoute->mProgressBar->setMaximum(150);
    mAzDialCalcRoute->mProgressBar->setValue(0);

    QList<QgsFeatureId> pQue; // �������� �������������� QgsFeatureId
    QList<QString> pListChecked;

    // �������� ��������� ������

    {
        QgsFeature f;
        pVectorLayer->changeAttributeValue(pMap.value("start"), pNumValColumn, QVariant(1.0), QVariant::String );
        pVectorLayer->changeAttributeValue(pMap.value("start"), pNumSettingsColumn, QVariant("start"), QVariant::String );
        if (!pVectorLayer->getFeatures(QgsFeatureRequest().setFilterFid(pMap.value("start"))).nextFeature(f))
        {
            mAzDialCalcRoute->mProgressBarText->setText("������ ������ � ��������� ������ ����������");
            return false;
        }
        pListChecked << f.attribute(pNumCellColumn).toString();
    }
    pQue << pMap.value("start");
    if (!pQue.isEmpty()) bWaveMoved = true;

    // ���� ��������������� �����
    while (bWaveMoved) // �� ��� ���, ���� "�����" ��������
    {
        // ������� - ����� FeatureId, ������� ���� �������� "������"
        // ����� ����� - ����� ����� ����� �������� � ������� �� �������

        bWaveMoved = false; // �� ��������� ����� �� ���������
        if (pQue.isEmpty()) break; // ������� �����
        QList<QgsFeatureId> pNewQue; // ������� ����� ����� (���������� ����� ���� ��� ������ ����� ����������)
        pCount++; // ������� ����
        mAzDialCalcRoute->mProgressBar->setValue(mAzDialCalcRoute->mProgressBar->value()+1);

        qDebug( "============== wave #" + QString::number(pCount).toAscii());
        mAzDialCalcRoute->mProgressBarText->setText("������� ��������: ��������������� ����� #" + QString::number(pCount));

        // ��������� �������
        while (!pQue.isEmpty())
        {
            QgsFeature pFeature;
            if (!pVectorLayer->getFeatures(QgsFeatureRequest().setFilterFid( pQue.at(0))).nextFeature(pFeature))
            {
                // �� �������� ������
                pQue.removeAt(0);
                continue;
            }

            QStringList pCell = (pFeature.attribute(pNumCellColumn).toString().split(";"));
            int cX(pCell.at(0).toInt()); //x
            int cY(pCell.at(1).toInt()); //y

            // �������� ����� ����� �����
            QStringList pNewCells = azGetNeighbors((long)cX, (long)cY, 1); // ����� ����� ��������� �����

            // ����������� ���������� �����
            for (int i = 0; i < pNewCells.count(); i++)
            {
                if (pListChecked.contains(pNewCells.at(i)))
                {
                    continue;
                }

                QgsFeature pFeat;
                QString pExpression( "\"cell_coord\" = \'" + pNewCells.at(i) + "\'");

                // ��������� ���� �� ����� ������ � ����� ����
                if (pVectorLayer->getFeatures(QgsFeatureRequest().setFilterExpression( pExpression )).nextFeature(pFeat))
                {
                    double valFeat(0.0);
                    valFeat = pFeat.attribute(pNumValColumn).toDouble();
                    // ��� ���� ����������� �������� ������
                    if (valFeat < 1.0 && valFeat > -1.0 && !pNewQue.contains(pFeat.id())) // ������ �� �������� � ���������
                    {
                        if (pVectorLayer->changeAttributeValue(pFeat.id(), pNumValColumn, QVariant((double)pCount), QVariant::String)) // �������� ����������
                        {
                            pListChecked << pNewCells.at(i);
                            bWaveMoved = true; // ����� ����������
                            pNewQue << pFeat.id();
                        }
                    }
                }
                else // ������ ���
                {
                    pListChecked << pNewCells.at(i); // ��������� �� ����������, ����� � ��������� ������ �� ������� �� ��� �����
                }
            }// ����� ����� ������� ������ �����
            pQue.removeAt(0);

        }// ����� ����� ��������� �������
        pQue << pNewQue; // �������� ����� ��������
    } // ����� ����� �����

    //    �������� �������� ������
    if (!pVectorLayer->changeAttributeValue(pMap.value("finish"), pNumSettingsColumn, QVariant("finish"), QVariant::String ))
    {
        bSuccess = false;
        mAzDialCalcRoute->mProgressBarText->setText("������ ������ � �������� ������ ����������");
    }
    else
    {
        bSuccess = true;
    }

    if (bSuccess)
    {
        if (pVectorLayer->commitChanges())
        {
            return bSuccess; // ��������� ���������
        }
        else
        {
            mAzDialCalcRoute->mProgressBarText->setText("������ ������ ������");
            return false;
        }
    }
    else
    {
        pVectorLayer->rollBack(); // ���������� ���������
        mAzDialCalcRoute->mProgressBarText->setText("������ ������ ������");
        return false;
    }
}

bool KKSGISWidgetBase::azShortestPathRoute(QgsVectorLayer *pVectorLayer, QMap<QString, QgsFeatureId> pMap)
{
    QList<QgsPoint> pPoints;
    QgsFeature pCurrentFeature;
    double pCurrentValue(0.0);

    if (!pVectorLayer->getFeatures(QgsFeatureRequest().setFilterFid(pMap.value("finish"))).nextFeature(pCurrentFeature))
    {
        // �� ������� �������� ������
        mAzDialCalcRoute->mProgressBarText->setText("����������� �������� ������ ��� ������ � ���");
        return false;
    }
    pPoints << azGetCentroid(pCurrentFeature); // �������� ����� - ������
    int pNumValColumn(-1);                     // ������� ��������
    int pNumCellColumn(-1);                    // ������� ���������
    int pNumXcolumn(-1);                       // ������� "x"
    int pNumYcolumn(-1);                       // ������� "y"
    pNumXcolumn = pVectorLayer->fieldNameIndex("cell_x"); // ������ ����� ������� 'cell_x'
    pNumYcolumn = pVectorLayer->fieldNameIndex("cell_y"); // ������ ����� ������� 'cell_y'
    pNumValColumn = pVectorLayer->fieldNameIndex("val");
    pNumCellColumn = pVectorLayer->fieldNameIndex("cell_coord");
    if (pNumXcolumn < 0 || pNumYcolumn < 0 ||
        pNumValColumn < 0 || pNumCellColumn < 0)
    {
        mAzDialCalcRoute->mProgressBarText->setText("����������� ����������� ���� � ����");
        return false; // ��� ����������� ����� � �������
    }

    int pMaxX(-1); // �������� �� "�"
    int pMaxY(-1); // �������� �� "�"
    pMaxX = pVectorLayer->maximumValue(pNumXcolumn).toInt();
    pMaxY = pVectorLayer->maximumValue(pNumYcolumn).toInt();

    if (pMaxX < 2 || pMaxY < 2)
    {
        mAzDialCalcRoute->mProgressBarText->setText("����������� ����������� ���� ����������� ����");
        return false;
    }
    pCurrentValue = pCurrentFeature.attribute(pNumValColumn).toDouble(); // ������� �������� ����������� ����
    // ��������� �������
    {
        mAzDialCalcRoute->mProgressBar->setMaximum((int)(pCurrentValue+1));
        mAzDialCalcRoute->mProgressBar->setMinimum(0);
        mAzDialCalcRoute->mProgressBar->setValue(1);
    }

    QString pMsg = "/" + QString::number(pCurrentValue) + ")";
    int pIter(0);                                // ����� ��������
    bool bPathIsMoved(true);                     // ���� �������� ��������
    while (pCurrentValue > 1.1)  // ���� �� ������ ��������� ����� ��� �� ������� � �����
    {
        int cX(pCurrentFeature.attribute(pNumXcolumn).toInt()); //x
        int cY(pCurrentFeature.attribute(pNumYcolumn).toInt()); //y
        bPathIsMoved = false;                                   // ���������� �������� �� ����
        QStringList pSosedi = azGetNeighbors( (long)cX, (long)cY, 2);
        for (int i = 0; i < pSosedi.count(); i++)
        {
            QgsFeature pNewFeat;
            QString pExpression( "\"cell_coord\" = \'" + pSosedi.at(i) + "\'");
            // ��������� ���� �� ����� ������ � ����� ����
            if (pVectorLayer->getFeatures(QgsFeatureRequest().setFilterExpression( pExpression )).nextFeature(pNewFeat))
            {
                double testVal(0.0);
                testVal = pNewFeat.attribute(pNumValColumn).toDouble(); // �������� �������� ������
                if (testVal > 0.0 && testVal < pCurrentValue)           // �� �������� "������������" � ������ �������� ��������
                {
                    // ������
                    {
                        pIter++;
                        mAzDialCalcRoute->mProgressBarText->setText("������� ��������: ��������� �������� (�������� " + QString::number(pIter) + pMsg);
                        mAzDialCalcRoute->mProgressBar->setValue(mAzDialCalcRoute->mProgressBar->value() + 1);
                    }
                    bPathIsMoved = true;        // ���� ��������
                    pCurrentFeature = pNewFeat; // � ��� ����� �������
                    pCurrentValue = pCurrentFeature.attribute(pNumValColumn).toDouble();
                    pPoints << azGetCentroid(pCurrentFeature); // ��������� ����� �������� ��� ����� ��������� ����
                    break;
                } // �������� ������
            } // ��������� ������
        } // ������ �������
        if (!bPathIsMoved) break; // �������� �� ����, ���� ����������
    }

    if (!bPathIsMoved)
    {
        mAzDialCalcRoute->mProgressBarText->setText("���� ����� ��������� ������� �� ����������");
        return false;
    }

    QgsFields pFields;
    QgsField myField( "comment", QVariant::String, "String", 10, 0, "Comment" );
    pFields.append( myField );
    QString pStrLayerName = azSettings->value("MainSettings/SaveDirPath", "C:/").toString() + "finalRoute_" + azCreateName(1) + ".shp";
    QgsVectorFileWriter pWriter(pStrLayerName, "UTF-8", pFields, QGis::WKBLineString, &pVectorLayer->crs());
    QgsFeature pFeatLine;
    QgsPolyline pPolyLine;
    for (int j = pPoints.size() - 1; j >= 0; j--)
    {
        pPolyLine << pPoints.at(j);
    }
    QgsGeometry * pLineGeometry = QgsGeometry::fromPolyline(pPolyLine);
    pFeatLine.setGeometry(pLineGeometry);
    pWriter.addFeature(pFeatLine);
    QgsVectorFileWriter::WriterError pError;
    pError = pWriter.hasError();
    if (pError != 0)
    {
        QMessageBox::information(this, "Error", pWriter.errorMessage(), QMessageBox::Ok);
        qDebug() << pWriter.errorMessage().toAscii();
    }
    QFileInfo pFile(pStrLayerName);
    if (pFile.exists())
    {
        azAddLayerVector(pFile);
        return true;
    }
    return false;

}

void KKSGISWidgetBase::SLOTtempUse()
{
//    QString pFilePath("D:/!Share/rastrs/union_den.tif");
//    QMessageBox::about(this, "hello world!", QString::number(azGetEPSG(pFilePath)));
}

long KKSGISWidgetBase::azGetEPSG(const QString rastrPath)
{
    QFileInfo pFileInfo(rastrPath);
    if (!pFileInfo.isFile()) // �������� ������������� �����
    {
        return -1;
    }
    QgsRasterLayer * pRLayer = new QgsRasterLayer(pFileInfo.filePath(), pFileInfo.completeBaseName());
    return pRLayer->crs().srsid();
}


void KKSGISWidgetBase::SLOTmpActionAddVectorLayer()
{
    QString fullLayerName = QFileDialog::getOpenFileName(this, "�������� ��������� ����",
                            "", "��� �������������� �������(*.shp);;Shapefiles (*.shp)");
    
    openVectorLayer(fullLayerName);
}

void KKSGISWidgetBase::SLOTmpActionAddPostGISLayer()
{
    openDatabaseLayer();
}

void KKSGISWidgetBase::SLOTmpActionAddRasterLayer()
{
    // ����: ���������� ������ � ���� �����
    // ������� �������� ������, �������� ���� � �����
    QString fullLayerName = QFileDialog::getOpenFileName(this, "�������� ��������� ����", "",
                             "��� �������������� ��������� ������� (*.img *.int *.asc *.tif *tiff *.bmp *.jpg *.jpeg);;Geotiff (*.tif *.tiff)");
    
    openRasterLayer(fullLayerName);

}

/*
void KKSGISWidgetBase::SLOTazShowMouseCoordinate(const QgsPoint & p )
{
    if(!mpCoordsEdit)
        return;

    if ( mpLayerSet.count() < 1 ) 
        return;
    if (!mpMapCanvas->underMouse()) 
        return;
    
    mpLastMapPosition = p;
     
    if ( mpMapCanvas->mapUnits() == QGis::Degrees )
    {
      QString format = QgsProject::instance()->readEntry( "PositionPrecision", "/DegreeFormat", "D" );

      if ( format == "DM" )
        mpCoordsEdit->setText( p.toDegreesMinutes( mMousePrecisionDecimalPlaces ) );
      else if ( format == "DMS" )
        mpCoordsEdit->setText( p.toDegreesMinutesSeconds( mMousePrecisionDecimalPlaces ) );
      else
        mpCoordsEdit->setText( p.toString( mMousePrecisionDecimalPlaces ) );
    }
    else
    {
      mpCoordsEdit->setText( p.toString( mMousePrecisionDecimalPlaces ) );
    }

    if ( mpCoordsEdit->width() > mpCoordsEdit->minimumWidth() )
    {
      mpCoordsEdit->setMinimumWidth( mpCoordsEdit->width() );
    }
 
}
*/

void KKSGISWidgetBase::SLOTazThemTaskSpectralBathynometry()
{
#ifdef WIN32
    //ksa -- if (dnThemTaskSpecBath == NULL);
    if (dnThemTaskSpecBath != NULL)
    {
     delete dnThemTaskSpecBath;
    }
    dnThemTaskSpecBath = new DNSpecBath(this);
    connect(this->dnThemTaskSpecBath, SIGNAL(SIGNALcreateVector()), this, SLOT(SLOTmpActionVectorize()));
    dnThemTaskSpecBath->show();
#endif
}

void KKSGISWidgetBase::SLOTazActionDirSettings()
{
    mAzDialDirSettings = new AzDialDirSettings(this->azSettings, this);
    mAzDialDirSettings->exec();
}


void KKSGISWidgetBase::SLOTazCoordsCenter()
{
    if(!mpCoordsEdit)
        return;

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

void KKSGISWidgetBase::SLOTmpActionFileSaveProjectAs()
{
  

    // Retrieve last used project dir from persistent settings
    QSettings settings;
    QString lastUsedDir = settings.value( "/UI/lastProjectDir", "." ).toString();

    QString path = QFileDialog::getSaveFileName( this,
                                                 tr( "Choose a file name to save the QGIS project file as" ),
                                                 lastUsedDir + "/" + QgsProject::instance()->title(),
                                                 tr( "QGis files" ) + " (*.qgs *.QGS)" );
    if ( path.isEmpty() )
        return;

    saveProjectAs(path);
}

void KKSGISWidgetBase::SLOTmpActionFileSaveProject()
{
    saveProject();
}


void KKSGISWidgetBase::openProject(const QString & prjFile)
{
    SLOTmpCloseProject();

    QgsProject::instance()->setFileName(prjFile);
    if ( !QgsProject::instance()->read() )
    {
        QApplication::restoreOverrideCursor();
        mpStatusBar->showMessage("������ �������� ����� �������!"
                                       "\n " + prjFile + "'");
        qDebug("������ �������� ����� �������!" + prjFile.toAscii());

        mpMapCanvas->freeze(false);
        mpMapCanvas->refresh();
        return;
    }

    this->azSetTitleWindow(*this);

    int  myRedInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorRedPart", 255 );
    int  myGreenInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorGreenPart", 255 );
    int  myBlueInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorBluePart", 255 );
    QColor myColor = QColor( myRedInt, myGreenInt, myBlueInt );
    mpMapCanvas->setCanvasColor( myColor ); //this is fill color before rendering starts

    bool projectScales = QgsProject::instance()->readBoolEntry( "Scales", "/useProjectScales" );
    if ( projectScales )
    {
        if(mScaleEdit)
            mScaleEdit->updateScales( QgsProject::instance()->readListEntry( "Scales", "/ScalesList" ) );
    }

    mpMapCanvas->updateScale();

    /*ksa
    QMapIterator < QString, QgsMapLayer * > i(QgsMapLayerRegistry::instance()->mapLayers());
    i.toBack();
    while (i.hasPrevious())
    {
        i.previous();
        
        QgsMapLayer * currLayer = i.value();
        
        if (currLayer->type() == QgsMapLayer::VectorLayer){
            if (currLayer->isValid()){
                mpRegistry->addMapLayer(currLayer, TRUE);
                connect(currLayer, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
                mpLayerSet.append(QgsMapCanvasLayer(currLayer));
            }

        }
        else if (currLayer->type() == QgsMapLayer::RasterLayer)
        {
            mpRegistry->addMapLayer(currLayer, TRUE);
            connect(currLayer, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
            mpLayerSet.append(QgsMapCanvasLayer(currLayer));
        }
    }
    */
    
    QSettings settings;
    // load PAL engine settings
    mLBL->loadEngineSettings();

    emit projectRead(); // let plug-ins know that we've read in a new
    // project so that they can check any project
    // specific plug-in state
    
    // TODO add last Open Project
    QApplication::restoreOverrideCursor();
    //ksa mpMapCanvas->setLayerSet(mpLayerSet);
    
    mpMapCanvas->freeze(false);
    mpMapCanvas->refresh();
}

void KKSGISWidgetBase::closeProject()
{

}

void KKSGISWidgetBase::saveProjectAs(const QString & path)
{
    if ( mpMapCanvas && mpMapCanvas->isDrawing() )
        return;

    QFileInfo fullPath( path );

    QSettings settings;
    settings.setValue( "/UI/lastProjectDir", fullPath.path() );

    // make sure the .qgs extension is included in the path name. if not, add it...
    if ( "qgs" != fullPath.suffix().toLower() ){
        fullPath.setFile( fullPath.filePath() + ".qgs" );
    }

    QgsProject::instance()->setFileName( fullPath.filePath() );

    if ( QgsProject::instance()->write() ){
        //setTitleBarText_( *this ); // update title bar
        mpStatusBar->showMessage( tr( "Saved project to: %1" ).arg( QgsProject::instance()->fileName() ), 5000 );
        // add this to the list of recently used project files
        //saveRecentProjectPath( fullPath.filePath(), settings );
    }
    else{
        QMessageBox::critical( this,
                               tr( "Unable to save project %1" ).arg( QgsProject::instance()->fileName() ),
                               QgsProject::instance()->error(),
                               QMessageBox::Ok,
                               Qt::NoButton );
    }
}

bool KKSGISWidgetBase::saveProject()
{
    if ( mpMapCanvas && mpMapCanvas->isDrawing() ){
        return false;
    }

    // if we don't have a file name, then obviously we need to get one; note
    // that the project file name is reset to null in fileNew()
    QFileInfo fullPath;

    // we need to remember if this is a new project so that we know to later
    // update the "last project dir" settings; we know it's a new project if
    // the current project file name is empty
    bool isNewProject = false;

    if ( QgsProject::instance()->fileName().isNull() ){
        isNewProject = true;

        // Retrieve last used project dir from persistent settings
        QSettings settings;
        QString lastUsedDir = settings.value( "/UI/lastProjectDir", "." ).toString();

        QString path = QFileDialog::getSaveFileName(
                                                     this,
                                                     tr( "Choose a QGIS project file" ),
                                                     lastUsedDir + "/" + QgsProject::instance()->title(),
                                                     tr( "QGis files" ) + " (*.qgs *.QGS)" 
                                                   );
        if ( path.isEmpty() )
            return false;

        fullPath.setFile( path );

        // make sure we have the .qgs extension in the file name
        if ( "qgs" != fullPath.suffix().toLower() ){
            fullPath.setFile( fullPath.filePath() + ".qgs" );
        }

        QgsProject::instance()->setFileName( fullPath.filePath() );
    }

    if ( QgsProject::instance()->write() ){
        //setTitleBarText_( *this ); // update title bar
        mpStatusBar->showMessage( tr( "Saved project to: %1" ).arg( QgsProject::instance()->fileName() ), 5000 );

        if ( isNewProject ){
            // add this to the list of recently used project files
            //QSettings settings;
            //saveRecentProjectPath( fullPath.filePath(), settings );
        }
    }
    else{
        QMessageBox::critical( this,
                               tr( "Unable to save project %1" ).arg( QgsProject::instance()->fileName() ),
                               QgsProject::instance()->error() 
                             );
        return false;
    }

    return true;
}

void KKSGISWidgetBase::openVectorLayer(const QString & vLayerFile)
{
    QFileInfo azFileInfo(vLayerFile);
    if (!azFileInfo.isFile())
        return;

    this->azAddLayerVector(azFileInfo);
}

void KKSGISWidgetBase::openRasterLayer(const QString & rLayerFile)
{
    QFileInfo azFileInfo(rLayerFile);
    if (!azFileInfo.isFile()) // �������� ������������� �����
        return;

    // ������� ��������� ������ QgsRasterLayer
    // �� �������� ��� ���� � ������
    QgsRasterLayer * mypLayer = new QgsRasterLayer(azFileInfo.filePath(), azFileInfo.completeBaseName());
    if (!mypLayer->isValid()) // ��������� ��� ������������
    {
        qDebug("Raster layer is not valid or not supported by GDAL. Layer was not added to map." + rLayerFile.toAscii());
        mpStatusBar->showMessage("���� ������ �� �������� ���������� ��� �� �������������� ����������� GDAL. ���� �� ��� �������� �� �����."
                                       "\n " + rLayerFile + "'");
        return;
    }

//    if (!this->azRasterEnhancement(*mypLayer)) // ��������� ��������� ������
        qDebug("��������� ������ �� ������������. ������������� ������ ������." + rLayerFile.toAscii());

    // ��������� ��������� ��������� (�.�. ����� ������������ �� �������� �� ���������)
//    mypLayer->setContrastEnhancement(QgsContrastEnhancement::StretchToMinimumMaximum);

    // ��������������
    connect(mypLayer, SIGNAL(repaintRequested()), mpMapCanvas, SLOT(refresh()) );

    // Add the Vector Layer to the Layer Registry
    mpRegistry->addMapLayer(mypLayer, TRUE);
    connect(mypLayer, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    // Add the Layer to the Layer Set
    //ksa mpLayerSet.append(QgsMapCanvasLayer(mypLayer));
    mpMapCanvas->setExtent(mypLayer->extent());
    //ksa mpMapCanvas->setLayerSet(mpLayerSet);
    mpMapCanvas->refresh();
/*    QgsMapLayer *pLayer;
    pLayer = mypLayer;
    QgsCoordinateReferenceSystem pSRS;
    pSRS = mypLayer->*/
}

void KKSGISWidgetBase::openDatabaseLayer()
{
    if ( mpMapCanvas && mpMapCanvas->isDrawing() )
        return;

    // TODO: QDialog for now, switch to QWidget in future
    QDialog *dbs = dynamic_cast<QDialog*>( QgsProviderRegistry::instance()->selectWidget( "postgres", this ) );
    if ( !dbs )
    {
        QMessageBox::warning( this, tr( "PostgreSQL" ), tr( "Cannot get PostgreSQL select dialog from provider." ) );
        return;
    }
  
    connect( dbs, SIGNAL( addDatabaseLayers( QStringList const &, QString const & ) ),
             this, SLOT( addDatabaseLayers( QStringList const &, QString const & ) ) );
    connect( dbs, SIGNAL( progress( int, int ) ),
             this, SLOT( showProgress( int, int ) ) );
    connect( dbs, SIGNAL( progressMessage( QString ) ),
             this, SLOT( showStatusMessage( QString ) ) );
    /*!!!!*/
    //connect(mypLayer, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    dbs->exec();
    delete dbs;
}

QString KKSGISWidgetBase::azCreateName(const int option)
{
    QString pStr = "";
    QDate pDate(QDate::currentDate());
    QTime pTime(QTime::currentTime());
    switch (option)
    {
    case 0:
        // name like "yyyy-mm-dd--hh-mm-ss-ms"
        pStr = QString::number(pDate.year()) + "-" +
                QString::number(pDate.month()) + "-" +
                QString::number(pDate.day()) + "--" +
                QString::number(pTime.hour()) + "-" +
                QString::number(pTime.minute()) + "-" +
                QString::number(pTime.second()) + "-" +
                QString::number(pTime.msec());
        break;
    case 1:
        // name like "hh-mm-ss"
        pStr = QString::number(pTime.hour()) + "-" +
                QString::number(pTime.minute()) + "-" +
                QString::number(pTime.second());
        break;
    case 2:
        // name like "hh-mm-ss-ms"
        pStr =  QString::number(pTime.hour()) + "-" +
                QString::number(pTime.minute()) + "-" +
                QString::number(pTime.second()) + "-" +
                QString::number(pTime.msec());
        break;
    }
    return pStr;
}

bool KKSGISWidgetBase::azCreateLayer(QString nameOfLayer, QgsFields pFields, QString pathOfLayer, bool addToMap, bool useDefaultFields, QGis::WkbType pType, const QString encoding, const long EPSG)
{
    QString pStr = pathOfLayer + nameOfLayer;
    QgsCoordinateReferenceSystem pSRS;
    pSRS.createFromOgcWmsCrs("EPSG:" + EPSG);
    pSRS.validate();
    if (pFields.count() < 1 || useDefaultFields)
    {
        QgsField myField( "comment", QVariant::String, "String", 10, 0, "Comment" );
        pFields.append( myField );
    }
    QgsVectorFileWriter::WriterError mError;
    QgsVectorFileWriter myWriter( pStr, encoding, pFields, pType, &pSRS);

    mError = myWriter.hasError();
    if (mError != 0)
    {
        qWarning() << myWriter.errorMessage();
        return false;
    }
    if (addToMap)
    {
        azWorkList.clear();
        azWorkList.append(pStr);
    }
    return true;
}



QString KKSGISWidgetBase::projectFileName()
{
    return QgsProject::instance()->fileName();
}

void KKSGISWidgetBase::layerProperties()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  showLayerProperties( activeLayer() );
}

/** Get a pointer to the currently selected map layer */
QgsMapLayer *KKSGISWidgetBase::activeLayer()
{
  return mpMapLegend ? mpMapLegend->currentLayer() : 0;
}

/** set the current layer */
bool KKSGISWidgetBase::setActiveLayer( QgsMapLayer *layer )
{
  if ( !layer )
    return false;

  if(!mpMapLegend)
      return false;

  return mpMapLegend->setCurrentLayer( layer );
}

void KKSGISWidgetBase::showLayerProperties( QgsMapLayer *ml )
{
    if(!mpMapLegend)
        return;
  /*
  TODO: Consider reusing the property dialogs again.
  Sometimes around mid 2005, the property dialogs were saved for later reuse;
  this resulted in a time savings when reopening the dialog. The code below
  cannot be used as is, however, simply by saving the dialog pointer here.
  Either the map layer needs to be passed as an argument to sync or else
  a separate copy of the dialog pointer needs to be stored with each layer.
  */

  if ( !ml )
    return;

  if ( !QgsProject::instance()->layerIsEmbedded( ml->id() ).isEmpty() )
  {
    return; //don't show properties of embedded layers
  }

  if ( ml->type() == QgsMapLayer::RasterLayer )
  {
    QgsRasterLayerProperties *rlp = NULL; // See note above about reusing this
    if ( rlp )
    {
      rlp->sync();
    }
    else
    {
      rlp = new QgsRasterLayerProperties( ml, mpMapCanvas, this );
      rlp->setWorkingWidget(this);
      connect( rlp, SIGNAL( refreshLegend( QString, bool ) ), mpMapLegend, SLOT( refreshLayerSymbology( QString, bool ) ) );
      connect( rlp, SIGNAL( refreshLegend( QString, bool ) ), this, SIGNAL (dataChanged() ) );
    }

    if(rlp->exec())
        emit dataChanged();

    delete rlp; // delete since dialog cannot be reused without updating code
  }
  else if ( ml->type() == QgsMapLayer::VectorLayer ) // VECTOR
  {
    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( ml );

    QgsVectorLayerProperties *vlp = NULL; // See note above about reusing this
    if ( vlp )
    {
      vlp->syncToLayer();
    }
    else
    {
      vlp = new QgsVectorLayerProperties( vlayer, this );
      vlp->setWorkingWidget(this);
      connect( vlp, SIGNAL( refreshLegend( QString, QgsLegendItem::Expansion ) ), mpMapLegend, SLOT( refreshLayerSymbology( QString, QgsLegendItem::Expansion ) ) );
      connect( vlp, SIGNAL( refreshLegend( QString, QgsLegendItem::Expansion ) ), this, SIGNAL (dataChanged() ) );
    }

    if ( vlp->exec() )
    {
      activateDeactivateLayerRelatedActions( ml );
      emit dataChanged();
    }
    delete vlp; // delete since dialog cannot be reused without updating code
  }
  else if ( ml->type() == QgsMapLayer::PluginLayer )
  {
    QgsPluginLayer* pl = qobject_cast<QgsPluginLayer *>( ml );
    if ( !pl )
      return;

    QgsPluginLayerType* plt = QgsPluginLayerRegistry::instance()->pluginLayerType( pl->pluginLayerType() );
    if ( !plt )
      return;

    if ( !plt->showLayerProperties( pl ) )
    {
      messageBar()->pushMessage( tr( "Warning" ),
                                 tr( "This layer doesn't have a properties dialog." ),
                                 QgsMessageBar::INFO, messageTimeout() );
    }
  }
  
}

QgsPalLabeling *KKSGISWidgetBase::palLabeling()
{
  Q_ASSERT( mLBL );
  return mLBL;
}

QgsMessageBar* KKSGISWidgetBase::messageBar()
{
  Q_ASSERT( mInfoBar );
  return mInfoBar;
}

int KKSGISWidgetBase::messageTimeout()
{
  QSettings settings;
  return settings.value( "/qgis/messageTimeout", 5 ).toInt();
}

void KKSGISWidgetBase::labeling()
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>( activeLayer() );
  if ( !vlayer )
  {
    messageBar()->pushMessage( tr( "Labeling Options" ),
                               tr( "Please select a vector layer first" ),
                               QgsMessageBar::INFO,
                               messageTimeout() );
    return;
  }


  QDialog *dlg = new QDialog( this );
  dlg->setWindowTitle( tr( "Layer labeling settings" ) );
  QgsLabelingGui *labelingGui = new QgsLabelingGui( mLBL, vlayer, mpMapCanvas, dlg );
  labelingGui->setWorkingWidget(this); //ksa
  labelingGui->init(); // load QgsPalLayerSettings for layer
  labelingGui->layout()->setContentsMargins( 0, 0, 0, 0 );
  QVBoxLayout *layout = new QVBoxLayout( dlg );
  layout->addWidget( labelingGui );

  QDialogButtonBox *buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply, Qt::Horizontal, dlg );
  layout->addWidget( buttonBox );

  dlg->setLayout( layout );

  QSettings settings;
  dlg->restoreGeometry( settings.value( "/Windows/Labeling/geometry" ).toByteArray() );

  connect( buttonBox->button( QDialogButtonBox::Ok ), SIGNAL( clicked() ), dlg, SLOT( accept() ) );
  connect( buttonBox->button( QDialogButtonBox::Cancel ), SIGNAL( clicked() ), dlg, SLOT( reject() ) );
  connect( buttonBox->button( QDialogButtonBox::Apply ), SIGNAL( clicked() ), labelingGui, SLOT( apply() ) );

  if ( dlg->exec() )
  {
    labelingGui->apply();

    settings.setValue( "/Windows/Labeling/geometry", dlg->saveGeometry() );

    // alter labeling - save the changes
    labelingGui->layerSettings().writeToLayer( vlayer );

    // trigger refresh
    if ( mpMapCanvas )
    {
      mpMapCanvas->refresh();
    }
  }

  delete dlg;

  activateDeactivateLayerRelatedActions( vlayer );
}

void KKSGISWidgetBase::markDirty()
{
  // notify the project that there was a change
  QgsProject::instance()->dirty( true );
}


void KKSGISWidgetBase::toggleEditing()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  QgsVectorLayer *currentLayer = qobject_cast<QgsVectorLayer*>( activeLayer() );
  if ( currentLayer )
  {
    toggleEditing( currentLayer, true );
  }
  else
  {
    // active although there's no layer active!?
    mActionToggleEditing->setChecked( false );
  }
}

bool KKSGISWidgetBase::toggleEditing( QgsMapLayer *layer, bool allowCancel )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer )
  {
    return false;
  }

  bool res = true;

  if ( !vlayer->isEditable() && !vlayer->isReadOnly() )
  {
    if ( !( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::EditingCapabilities ) )
    {
      mActionToggleEditing->setChecked( false );
      mActionToggleEditing->setEnabled( false );
      messageBar()->pushMessage( tr( "Start editing failed" ),
                                 tr( "Provider cannot be opened for editing" ),
                                 QgsMessageBar::INFO, messageTimeout() );
      return false;
    }

    vlayer->startEditing();

    QSettings settings;
    QString markerType = settings.value( "/qgis/digitizing/marker_style", "Cross" ).toString();
    bool markSelectedOnly = settings.value( "/qgis/digitizing/marker_only_for_selected", false ).toBool();

    // redraw only if markers will be drawn
    if (( !markSelectedOnly || vlayer->selectedFeatureCount() > 0 ) &&
        ( markerType == "Cross" || markerType == "SemiTransparentCircle" ) )
    {
      vlayer->triggerRepaint();
    }
  }
  else if ( vlayer->isModified() )
  {
    QMessageBox::StandardButtons buttons = QMessageBox::Save | QMessageBox::Discard;
    if ( allowCancel )
      buttons |= QMessageBox::Cancel;

    switch ( QMessageBox::information( 0,
                                       tr( "Stop editing" ),
                                       tr( "Do you want to save the changes to layer %1?" ).arg( vlayer->name() ),
                                       buttons ) )
    {
      case QMessageBox::Cancel:
        res = false;
        break;

      case QMessageBox::Save:
        if ( !vlayer->commitChanges() )
        {
          //ksa commitError( vlayer );
          // Leave the in-memory editing state alone,
          // to give the user a chance to enter different values
          // and try the commit again later
          res = false;
        }

        vlayer->triggerRepaint();
        break;

      case QMessageBox::Discard:
        mpMapCanvas->freeze( true );
        if ( !vlayer->rollBack() )
        {
          messageBar()->pushMessage( tr( "Error" ),
                                     tr( "Problems during roll back" ),
                                     QgsMessageBar::CRITICAL, messageTimeout() );
          res = false;
        }
        mpMapCanvas->freeze( false );

        vlayer->triggerRepaint();
        break;

      default:
        break;
    }
  }
  else //layer not modified
  {
    mpMapCanvas->freeze( true );
    vlayer->rollBack();
    mpMapCanvas->freeze( false );
    res = true;
    vlayer->triggerRepaint();
  }

  if ( !res && layer == activeLayer() )
  {
    // while also called when layer sends editingStarted/editingStopped signals,
    // this ensures correct restoring of gui state if toggling was canceled
    // or layer commit/rollback functions failed
    
    activateDeactivateLayerRelatedActions( layer );
  }

  return res;
}

//���������� ���������� ���� � ����� ����, ������� �������� �� ����� ������� QGIS
QString KKSGISWidgetBase::readLayerFilePath(const QString & file) const
{
    return QgsProject::instance()->readPath(file);
}

QStringList KKSGISWidgetBase::azGetNeighbors(long xPoint, long yPoint, int pStyleOfNeighbors)
{
    QStringList azGetNeighbors1;
    switch (pStyleOfNeighbors)
    {
    case 1:
        /* ����� ����� �����
    // 1 | 2 | 3
    // 4 |x,y| 5
    // 6 | 7 | 8
    // ��� �������, ��� X � Y ����� ���� ������ ������ 0*/

        if (xPoint-1>=1 && yPoint-1>=1) azGetNeighbors1 << QString::number(xPoint-1) + ";" +  QString::number(yPoint-1);  // 1
        if (yPoint-1>=1)                azGetNeighbors1 << QString::number(xPoint)   + ";" +  QString::number(yPoint-1);  // 2
        if (yPoint-1>=1)                azGetNeighbors1 << QString::number(xPoint+1) + ";" +  QString::number(yPoint-1);  // 3
        if (xPoint-1>=1)                azGetNeighbors1 << QString::number(xPoint-1) + ";" +  QString::number(yPoint);    // 4
                                        azGetNeighbors1 << QString::number(xPoint+1) + ";" +  QString::number(yPoint);    // 5
        if (xPoint-1>=1)                azGetNeighbors1 << QString::number(xPoint-1) + ";" +  QString::number(yPoint+1);  // 6
                                        azGetNeighbors1 << QString::number(xPoint)   + ";" +  QString::number(yPoint+1);  // 7
                                        azGetNeighbors1 << QString::number(xPoint+1) + ";" +  QString::number(yPoint+1);  // 8
        break;
    case 2:
        if (yPoint-1>=1)                azGetNeighbors1 << QString::number(xPoint)   + ";" +  QString::number(yPoint-1);  // 2
                                        azGetNeighbors1 << QString::number(xPoint+1) + ";" +  QString::number(yPoint);    // 5
                                        azGetNeighbors1 << QString::number(xPoint)   + ";" +  QString::number(yPoint+1);  // 7
        if (xPoint-1>=1)                azGetNeighbors1 << QString::number(xPoint-1) + ";" +  QString::number(yPoint);    // 4
        if (xPoint-1>=1 && yPoint-1>=1) azGetNeighbors1 << QString::number(xPoint-1) + ";" +  QString::number(yPoint-1);  // 1
        if (yPoint-1>=1)                azGetNeighbors1 << QString::number(xPoint+1) + ";" +  QString::number(yPoint-1);  // 3
                                        azGetNeighbors1 << QString::number(xPoint+1) + ";" +  QString::number(yPoint+1);  // 8
        if (xPoint-1>=1)                azGetNeighbors1 << QString::number(xPoint-1) + ";" +  QString::number(yPoint+1);  // 6
        break;
    }
    return azGetNeighbors1;
}

QgsPoint KKSGISWidgetBase::azGetCentroid(QgsFeature &pFeature)
{
    QgsGeometry *pGeom;
    pGeom = pFeature.geometry();
    double x(pGeom->centroid()->asPoint().x());
    double y(pGeom->centroid()->asPoint().y());
    QgsPoint pPoint(x, y);
    return pPoint;
}

void KKSGISWidgetBase::SLOTmpActionFileNew()
{
  fileNew( true ); // prompts whether to save project
} // fileNew()

void KKSGISWidgetBase::fileNewBlank()
{
  fileNew( true, true );
}


void KKSGISWidgetBase::fileNew( bool thePromptToSaveFlag, bool forceBlank )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  if ( thePromptToSaveFlag )
  {
    if ( !saveDirty() )
    {
      return; //cancel pressed
    }
  }

  QSettings settings;

  closeProject();
  mpMapCanvas->clear();

  QgsProject* prj = QgsProject::instance();
  prj->title( QString::null );
  prj->setFileName( QString::null );
  prj->clearProperties(); // why carry over properties from previous projects?

  //set the color for selections
  //the default can be set in qgisoptions
  //use project properties to override the color on a per project basis
  int myRed = settings.value( "/qgis/default_selection_color_red", 255 ).toInt();
  int myGreen = settings.value( "/qgis/default_selection_color_green", 255 ).toInt();
  int myBlue = settings.value( "/qgis/default_selection_color_blue", 0 ).toInt();
  int myAlpha = settings.value( "/qgis/default_selection_color_alpha", 255 ).toInt();
  prj->writeEntry( "Gui", "/SelectionColorRedPart", myRed );
  prj->writeEntry( "Gui", "/SelectionColorGreenPart", myGreen );
  prj->writeEntry( "Gui", "/SelectionColorBluePart", myBlue );
  prj->writeEntry( "Gui", "/SelectionColorAlphaPart", myAlpha );

  //set the canvas to the default background color
  //the default can be set in qgisoptions
  //use project properties to override the color on a per project basis
  myRed = settings.value( "/qgis/default_canvas_color_red", 255 ).toInt();
  myGreen = settings.value( "/qgis/default_canvas_color_green", 255 ).toInt();
  myBlue = settings.value( "/qgis/default_canvas_color_blue", 255 ).toInt();
  prj->writeEntry( "Gui", "/CanvasColorRedPart", myRed );
  prj->writeEntry( "Gui", "/CanvasColorGreenPart", myGreen );
  prj->writeEntry( "Gui", "/CanvasColorBluePart", myBlue );
  mpMapCanvas->setCanvasColor( QColor( myRed, myGreen, myBlue ) );

  prj->dirty( false );

  azSetTitleWindow( *this );

  //QgsDebugMsg("emiting new project signal");

  //emit signal so QgsComposer knows we have a new project
  emit newProject();

  mpMapCanvas->freeze( false );
  mpMapCanvas->refresh();
  mpMapCanvas->clearExtentHistory();
  mScaleEdit->updateScales();

  // set project CRS
  QgsMapRenderer* myRenderer = mpMapCanvas->mapRenderer();
  QString defCrs = settings.value( "/Projections/projectDefaultCrs", GEO_EPSG_CRS_AUTHID ).toString();
  QgsCoordinateReferenceSystem srs;
  srs.createFromOgcWmsCrs( defCrs );
  myRenderer->setDestinationCrs( srs );
  // write the projections _proj string_ to project settings
  prj->writeEntry( "SpatialRefSys", "/ProjectCrs", defCrs );
  prj->dirty( false );
  if ( srs.mapUnits() != QGis::UnknownUnit )
  {
    myRenderer->setMapUnits( srs.mapUnits() );
  }

  // enable OTF CRS transformation if necessary
  myRenderer->setProjectionsEnabled( settings.value( "/Projections/otfTransformEnabled", 0 ).toBool() );

  updateCRSStatusBar();

  /** New Empty Project Created
      (before attempting to load custom project templates/filepaths) */

  // load default template
  /* NOTE: don't open default template on launch until after initialization,
           in case a project was defined via command line */

  // don't open template if last auto-opening of a project failed
  if ( ! forceBlank )
  {
    forceBlank = ! settings.value( "/qgis/projOpenedOKAtLaunch", QVariant( true ) ).toBool();
  }

  //if ( ! forceBlank && settings.value( "/qgis/newProjectDefault", QVariant( false ) ).toBool() )
  //{
  //  fileNewFromDefaultTemplate();
  //}

  // set the initial map tool
#ifndef HAVE_TOUCH
  mpMapCanvas->setMapTool( mMapTools.mPan);
  mNonEditMapTool = mMapTools.mPan;  // signals are not yet setup to catch this
#else
  mpMapCanvas->setMapTool( mMapTools.mTouch );
  mNonEditMapTool = mMapTools.mTouch;  // signals are not yet setup to catch this
#endif

} // QgisApp::fileNew(bool thePromptToSaveFlag)

void KKSGISWidgetBase::updateCRSStatusBar()
{
  mOnTheFlyProjectionStatusLabel->setText( mpMapCanvas->mapRenderer()->destinationCrs().authid() );

  if ( mpMapCanvas->mapRenderer()->hasCrsTransformEnabled() )
  {
    mOnTheFlyProjectionStatusLabel->setEnabled( true );
    mOnTheFlyProjectionStatusLabel->setToolTip(
      tr( "Current CRS: %1 (OTFR enabled)" ).arg( mpMapCanvas->mapRenderer()->destinationCrs().description() ) );
    mOnTheFlyProjectionStatusButton->setIcon( QgsApplication::getThemeIcon( "mIconProjectionEnabled.png" ) );
  }
  else
  {
    mOnTheFlyProjectionStatusLabel->setEnabled( false );
    mOnTheFlyProjectionStatusLabel->setToolTip(
      tr( "Current CRS: %1 (OTFR disabled)" ).arg( mpMapCanvas->mapRenderer()->destinationCrs().description() ) );
    mOnTheFlyProjectionStatusButton->setIcon( QgsApplication::getThemeIcon( "mIconProjectionDisabled.png" ) );
  }
}

void KKSGISWidgetBase::destinationSrsChanged()
{
  // save this information to project
  long srsid = mpMapCanvas->mapRenderer()->destinationCrs().srsid();
  QgsProject::instance()->writeEntry( "SpatialRefSys", "/ProjectCRSID", ( int )srsid );
  updateCRSStatusBar();
}


void KKSGISWidgetBase::hasCrsTransformEnabled( bool theFlag )
{
  // save this information to project
  QgsProject::instance()->writeEntry( "SpatialRefSys", "/ProjectionsEnabled", ( theFlag ? 1 : 0 ) );
  updateCRSStatusBar();
}

void KKSGISWidgetBase::projectPropertiesProjections()
{
  // Driver to display the project props dialog and switch to the
  // projections tab
  mShowProjectionTab = true;
  projectProperties();
}

void KKSGISWidgetBase::projectProperties()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  /* Display the property sheet for the Project */
  // set wait cursor since construction of the project properties
  // dialog results in the construction of the spatial reference
  // system QMap
  QApplication::setOverrideCursor( Qt::WaitCursor );
  QgsProjectProperties *pp = new QgsProjectProperties( mpMapCanvas, this );
  // if called from the status bar, show the projection tab
  if ( mShowProjectionTab )
  {
    pp->showProjectionsTab();
    mShowProjectionTab = false;
  }
  qApp->processEvents();
  // Be told if the mouse display precision may have changed by the user
  // changing things in the project properties dialog box
  connect( pp, SIGNAL( displayPrecisionChanged() ), this, SLOT( updateMouseCoordinatePrecision() ) );

  connect( pp, SIGNAL( scalesChanged( const QStringList & ) ), mScaleEdit, SLOT( updateScales( const QStringList & ) ) );
  QApplication::restoreOverrideCursor();

  //pass any refresh signals off to canvases
  // Line below was commented out by wonder three years ago (r4949).
  // It is needed to refresh scale bar after changing display units.
  connect( pp, SIGNAL( refresh() ), mpMapCanvas, SLOT( refresh() ) );

  QgsMapRenderer* myRender = mpMapCanvas->mapRenderer();
  bool wasProjected = myRender->hasCrsTransformEnabled();
  long oldCRSID = myRender->destinationCrs().srsid();

  // Display the modal dialog box.
  pp->exec();

  long newCRSID = myRender->destinationCrs().srsid();
  bool isProjected = myRender->hasCrsTransformEnabled();

  // projections have been turned on/off or dest CRS has changed while projections are on
  if ( wasProjected != isProjected || ( isProjected && oldCRSID != newCRSID ) )
  {
    // TODO: would be better to try to reproject current extent to the new one
    mpMapCanvas->updateFullExtent();
  }

  int  myRedInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorRedPart", 255 );
  int  myGreenInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorGreenPart", 255 );
  int  myBlueInt = QgsProject::instance()->readNumEntry( "Gui", "/CanvasColorBluePart", 255 );
  QColor myColor = QColor( myRedInt, myGreenInt, myBlueInt );
  mpMapCanvas->setCanvasColor( myColor ); // this is fill color before rendering onto canvas

  //qobject_cast<QgsMeasureTool*>( mMapTools.mMeasureDist )->updateSettings();
  //qobject_cast<QgsMeasureTool*>( mMapTools.mMeasureArea )->updateSettings();
  //qobject_cast<QgsMapToolMeasureAngle*>( mMapTools.mMeasureAngle )->updateSettings();

  // Set the window title.
  azSetTitleWindow( *this );

  // delete the property sheet object
  delete pp;
} // QgisApp::projectProperties

/**
  Prompt and save if project has been modified.
  @return true if saved or discarded, false if cancelled
 */
bool KKSGISWidgetBase::saveDirty()
{
  QString whyDirty = "";
  bool hasUnsavedEdits = false;
  // extra check to see if there are any vector layers with unsaved provider edits
  // to ensure user has opportunity to save any editing
  if ( QgsMapLayerRegistry::instance()->count() > 0 )
  {
    QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
    for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); it++ )
    {
      QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( it.value() );
      if ( !vl )
      {
        continue;
      }

      hasUnsavedEdits = ( vl->isEditable() && vl->isModified() );
      if ( hasUnsavedEdits )
      {
        break;
      }
    }

    if ( hasUnsavedEdits )
    {
      markDirty();
      whyDirty = "<p style='color:darkred;'>";
      whyDirty += tr( "Project has layer(s) in edit mode with unsaved edits, which will NOT be saved!" );
      whyDirty += "</p>";
    }
  }

  QMessageBox::StandardButton answer( QMessageBox::Discard );
  mpMapCanvas->freeze( true );

  //QgsDebugMsg(QString("Layer count is %1").arg(mMapCanvas->layerCount()));
  //QgsDebugMsg(QString("Project is %1dirty").arg( QgsProject::instance()->isDirty() ? "" : "not "));
  //QgsDebugMsg(QString("Map canvas is %1dirty").arg(mMapCanvas->isDirty() ? "" : "not "));

  QSettings settings;
  bool askThem = settings.value( "qgis/askToSaveProjectChanges", true ).toBool();

  if ( askThem && ( QgsProject::instance()->isDirty() || mpMapCanvas->isDirty() ) && QgsMapLayerRegistry::instance()->count() > 0 )
  {
    // flag project as dirty since dirty state of canvas is reset if "dirty"
    // is based on a zoom or pan
    markDirty();

    // old code: mProjectIsDirtyFlag = true;

    // prompt user to save
    answer = QMessageBox::information( this, tr( "Save?" ),
                                       tr( "Do you want to save the current project? %1" )
                                       .arg( whyDirty ),
                                       QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard,
                                       hasUnsavedEdits ? QMessageBox::Cancel : QMessageBox::Save );
    if ( QMessageBox::Save == answer )
    {
        if ( !saveProject() )
            answer = QMessageBox::Cancel;
    }
  }

  mpMapCanvas->freeze( false );

  return answer != QMessageBox::Cancel;
} // QgisApp::saveDirty()


void KKSGISWidgetBase::select()
{
    mpMapCanvas->setMapTool( mMapTools.mSelect );
}

void KKSGISWidgetBase::selectByRectangle()
{
  mpMapCanvas->setMapTool( mMapTools.mSelectRectangle );
}

void KKSGISWidgetBase::selectByPolygon()
{
  mpMapCanvas->setMapTool( mMapTools.mSelectPolygon );
}

void KKSGISWidgetBase::selectByFreehand()
{
  mpMapCanvas->setMapTool( mMapTools.mSelectFreehand );
}

void KKSGISWidgetBase::selectByRadius()
{
  mpMapCanvas->setMapTool( mMapTools.mSelectRadius );
}

void KKSGISWidgetBase::deselectAll()
{
  if ( !mpMapCanvas || mpMapCanvas->isDrawing() )
  {
    return;
  }

  // Turn off rendering to improve speed.
  bool renderFlagState = mpMapCanvas->renderFlag();
  if ( renderFlagState )
    mpMapCanvas->setRenderFlag( false );

  QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
  for ( QMap<QString, QgsMapLayer*>::iterator it = layers.begin(); it != layers.end(); it++ )
  {
    QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( it.value() );
    if ( !vl )
      continue;

    vl->removeSelection();
  }

  // Turn on rendering (if it was on previously)
  if ( renderFlagState )
    mpMapCanvas->setRenderFlag( true );
}

void KKSGISWidgetBase::selectByExpression()
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( mpMapCanvas->currentLayer() );
  if ( !vlayer )
  {
    messageBar()->pushMessage(
      tr( "No active vector layer" ),
      tr( "To select features, choose a vector layer in the legend" ),
      QgsMessageBar::INFO,
      messageTimeout() );
    return;
  }

  QgsExpressionSelectionDialog* dlg = new QgsExpressionSelectionDialog( vlayer );
  dlg->setAttribute( Qt::WA_DeleteOnClose );
  dlg->show();
}

void KKSGISWidgetBase::refreshMapCanvas()
{
  //clear all caches first
  QgsMapLayerRegistry::instance()->clearAllLayerCaches();
  //reload cached provider data
  QgsMapLayerRegistry::instance()->reloadAllLayers();
  //then refresh
  mpMapCanvas->refresh();
}

void KKSGISWidgetBase::refreshMapCanvasOnly()
{
  mpMapCanvas->refresh();
}

void KKSGISWidgetBase::identify()
{
  mpMapCanvas->setMapTool( mMapTools.mIdentify );
}

void KKSGISWidgetBase::measure()
{
  mpMapCanvas->setMapTool( mMapTools.mMeasureDist );
}

void KKSGISWidgetBase::measureArea()
{
  mpMapCanvas->setMapTool( mMapTools.mMeasureArea );
}

void KKSGISWidgetBase::measureAngle()
{
  mpMapCanvas->setMapTool( mMapTools.mMeasureAngle );
}

void KKSGISWidgetBase::zoomFull()
{
  mpMapCanvas->zoomToFullExtent();
}

void KKSGISWidgetBase::zoomToPrevious()
{
  mpMapCanvas->zoomToPreviousExtent();
}

void KKSGISWidgetBase::zoomToNext()
{
  mpMapCanvas->zoomToNextExtent();
}

void KKSGISWidgetBase::zoomActualSize()
{
  mpMapLegend->legendLayerZoomNative();
}

void KKSGISWidgetBase::zoomToSelected()
{
  mpMapCanvas->zoomToSelected();
}

void KKSGISWidgetBase::zoomToLayerExtent()
{
  mpMapLegend->legendLayerZoom();
}

void KKSGISWidgetBase::panToSelected()
{
  mpMapCanvas->panToSelected();
}

void KKSGISWidgetBase::showIOEditor(QWidget * parent, const QString & uid)
{
    emit signalShowIOEditor(parent, uid);
}

bool KKSGISWidgetBase::featureFromEIO(QWidget * parent, QgsFeature & feature, const QString & geomAsEWKT, const QString & layerTable)
{
    emit signalFeatureFromEIO(parent, feature, geomAsEWKT, layerTable);
    if(feature.attribute(feature.fieldNameIndex("unique_id")).toString().isEmpty())
        return false;
    
    return true;
}

bool KKSGISWidgetBase::deleteFeaturesAsEIO(QWidget * parent, const QString & tableName, const QList<qint64> & ids)
{
    emit signalDeleteFeaturesAsEIO(parent, tableName, ids);
    return true;
}

void KKSGISWidgetBase::editCut( QgsMapLayer * layerContainingSelection )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }

  // Test for feature support in this layer
  QgsVectorLayer* selectionVectorLayer = qobject_cast<QgsVectorLayer *>( layerContainingSelection ? layerContainingSelection : activeLayer() );
  if ( !selectionVectorLayer )
    return;

  clipboard()->replaceWithCopyOf( selectionVectorLayer );

  selectionVectorLayer->beginEditCommand( tr( "Features cut" ) );
  selectionVectorLayer->deleteSelectedFeatures();
  selectionVectorLayer->endEditCommand();
}

void KKSGISWidgetBase::addFeature()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
  {
    return;
  }
  mpMapCanvas->setMapTool( mMapTools.mAddFeature );
}

void KKSGISWidgetBase::moveFeature()
{
  mpMapCanvas->setMapTool( mMapTools.mMoveFeature );
}

void KKSGISWidgetBase::nodeTool()
{
  mpMapCanvas->setMapTool( mMapTools.mNodeTool );
}

void KKSGISWidgetBase::rollbackEdits()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  foreach ( QgsMapLayer * layer, mpMapLegend->selectedLayers() )
  {
    cancelEdits( layer, true, false );
  }
  mpMapCanvas->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void KKSGISWidgetBase::rollbackAllEdits( bool verifyAction )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  if ( verifyAction )
  {
    if ( !verifyEditsActionDialog( tr( "Rollback" ), tr( "all" ) ) )
      return;
  }

  foreach ( QgsMapLayer * layer, editableLayers( true ) )
  {
    cancelEdits( layer, true, false );
  }
  mpMapCanvas->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void KKSGISWidgetBase::cancelEdits()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  foreach ( QgsMapLayer * layer, mpMapLegend->selectedLayers() )
  {
    cancelEdits( layer, false, false );
  }
  mpMapCanvas->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void KKSGISWidgetBase::cancelAllEdits( bool verifyAction )
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  if ( verifyAction )
  {
    if ( !verifyEditsActionDialog( tr( "Cancel" ), tr( "all" ) ) )
      return;
  }

  foreach ( QgsMapLayer * layer, editableLayers() )
  {
    cancelEdits( layer, false, false );
  }
  mpMapCanvas->refresh();
  activateDeactivateLayerRelatedActions( activeLayer() );
}

void KKSGISWidgetBase::cancelEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint )
{
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
  if ( !vlayer || !vlayer->isEditable() )
    return;

  if ( vlayer == activeLayer() && leaveEditable )
    mSaveRollbackInProgress = true;

  mpMapCanvas->freeze( true );
  if ( !vlayer->rollBack( !leaveEditable ) )
  {
    mSaveRollbackInProgress = false;
    QMessageBox::information( 0,
                              tr( "Error" ),
                              tr( "Could not %1 changes to layer %2\n\nErrors: %3\n" )
                              .arg( leaveEditable ? tr( "rollback" ) : tr( "cancel" ) )
                              .arg( vlayer->name() )
                              .arg( vlayer->commitErrors().join( "\n  " ) ) );
  }
  mpMapCanvas->freeze( false );

  if ( leaveEditable )
  {
    vlayer->startEditing();
  }
  if ( triggerRepaint )
  {
    vlayer->triggerRepaint();
  }
}

void KKSGISWidgetBase::commitError( QgsVectorLayer* vlayer )
{
  QgsMessageViewer *mv = new QgsMessageViewer();
  mv->setWindowTitle( tr( "Commit errors" ) );
  mv->setMessageAsPlainText( tr( "Could not commit changes to layer %1" ).arg( vlayer->name() )
                             + "\n\n"
                             + tr( "Errors: %1\n" ).arg( vlayer->commitErrors().join( "\n  " ) )
                           );

  QToolButton *showMore = new QToolButton();
  // store pointer to vlayer in data of QAction
  QAction *act = new QAction( showMore );
  act->setData( QVariant( QMetaType::QObjectStar, &vlayer ) );
  act->setText( tr( "Show more" ) );
  showMore->setStyleSheet( "background-color: rgba(255, 255, 255, 0); color: black; text-decoration: underline;" );
  showMore->setCursor( Qt::PointingHandCursor );
  showMore->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred );
  showMore->addAction( act );
  showMore->setDefaultAction( act );
  connect( showMore, SIGNAL( triggered( QAction* ) ), mv, SLOT( exec() ) );
  connect( showMore, SIGNAL( triggered( QAction* ) ), showMore, SLOT( deleteLater() ) );

  // no timeout set, since notice needs attention and is only shown first time layer is labeled
  QgsMessageBarItem *errorMsg = new QgsMessageBarItem(
    tr( "Commit errors" ),
    tr( "Could not commit changes to layer %1" ).arg( vlayer->name() ),
    showMore,
    QgsMessageBar::WARNING,
    0,
    messageBar() );
  messageBar()->pushItem( errorMsg );
}

void KKSGISWidgetBase::layerEditStateChanged()
{
  QgsMapLayer* layer = qobject_cast<QgsMapLayer *>( sender() );
  if ( layer && layer == activeLayer() )
  {
    activateDeactivateLayerRelatedActions( layer );
    mSaveRollbackInProgress = false;
  }
}

//changed from layerWasAdded to layersWereAdded in 1.8
void KKSGISWidgetBase::layersWereAdded( QList<QgsMapLayer *> theLayers )
{
  for ( int i = 0; i < theLayers.size(); ++i )
  {
    QgsMapLayer * layer = theLayers.at( i );
    QgsDataProvider *provider = 0;

    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
    if ( vlayer )
    {
      // notify user about any font family substitution, but only when rendering labels (i.e. not when opening settings dialog)
      connect( vlayer, SIGNAL( labelingFontNotFound( QgsVectorLayer*, QString ) ), this, SLOT( labelingFontNotFound( QgsVectorLayer*, QString ) ) );

      QgsVectorDataProvider* vProvider = vlayer->dataProvider();
      if ( vProvider && vProvider->capabilities() & QgsVectorDataProvider::EditingCapabilities )
      {
        connect( vlayer, SIGNAL( layerModified() ), this, SLOT( updateLayerModifiedActions() ) );
        connect( vlayer, SIGNAL( editingStarted() ), this, SLOT( layerEditStateChanged() ) );
        connect( vlayer, SIGNAL( editingStopped() ), this, SLOT( layerEditStateChanged() ) );
      }
      provider = vProvider;
    }

    QgsRasterLayer *rlayer = qobject_cast<QgsRasterLayer *>( layer );
    if ( rlayer )
    {
      // connect up any request the raster may make to update the app progress
      connect( rlayer, SIGNAL( drawingProgress( int, int ) ), this, SLOT( showProgress( int, int ) ) );

      // connect up any request the raster may make to update the statusbar message
      connect( rlayer, SIGNAL( statusChanged( QString ) ), this, SLOT( showStatusMessage( QString ) ) );

      provider = rlayer->dataProvider();
    }

    if ( provider )
    {
      connect( provider, SIGNAL( dataChanged() ), layer, SLOT( clearCacheImage() ) );
      connect( provider, SIGNAL( dataChanged() ), mpMapCanvas, SLOT( refresh() ) );
    }
  }
}

QgsLegend *KKSGISWidgetBase::legend()
{
  Q_ASSERT( mpMapLegend );
  return mpMapLegend;
}

#ifdef Q_OS_WIN
// hope your wearing your peril sensitive sunglasses.
void KKSGISWidgetBase::contextMenuEvent( QContextMenuEvent *e )
{
  if ( mSkipNextContextMenuEvent )
  {
    mSkipNextContextMenuEvent--;
    e->ignore();
    return;
  }

  QWidget::contextMenuEvent( e );
}

void KKSGISWidgetBase::skipNextContextMenuEvent()
{
  mSkipNextContextMenuEvent++;
}
#endif

void KKSGISWidgetBase::updateUndoActions()
{
  bool canUndo = false, canRedo = false;
  QgsMapLayer* layer = activeLayer();
  if ( layer )
  {
    QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( layer );
    if ( vlayer && vlayer->isEditable() )
    {
      canUndo = vlayer->undoStack()->canUndo();
      canRedo = vlayer->undoStack()->canRedo();
    }
  }
  //mActionUndo->setEnabled( canUndo );
  //mActionRedo->setEnabled( canRedo );
}

void KKSGISWidgetBase::mapToolChanged( QgsMapTool *tool )
{
  if ( tool && !tool->isEditTool() )
  {
    mNonEditMapTool = tool;
  }
}

void KKSGISWidgetBase::updateLayerModifiedActions()
{
  bool enableSaveLayerEdits = false;
  QgsVectorLayer* vlayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
  if ( vlayer )
  {
    QgsVectorDataProvider* dprovider = vlayer->dataProvider();
    if ( dprovider )
    {
      enableSaveLayerEdits = ( dprovider->capabilities() & QgsVectorDataProvider::ChangeAttributeValues
                               && vlayer->isEditable()
                               && vlayer->isModified() );
    }
  }
  mActionSaveLayerEdits->setEnabled( enableSaveLayerEdits );

  mActionSaveEdits->setEnabled( mpMapLegend && mpMapLegend->selectedLayersEditable( true ) );
  mActionRollbackEdits->setEnabled( mpMapLegend && mpMapLegend->selectedLayersEditable( true ) );
  mActionCancelEdits->setEnabled( mpMapLegend && mpMapLegend->selectedLayersEditable() );

  bool hasEditLayers = ( editableLayers().count() > 0 );
  mActionAllEdits->setEnabled( hasEditLayers );
  mActionCancelAllEdits->setEnabled( hasEditLayers );

  bool hasModifiedLayers = ( editableLayers( true ).count() > 0 );
  mActionSaveAllEdits->setEnabled( hasModifiedLayers );
  mActionRollbackAllEdits->setEnabled( hasModifiedLayers );
}

void KKSGISWidgetBase::showExtents()
{
  if ( !mToggleExtentsViewButton->isChecked() )
  {
    return;
  }

  // update the statusbar with the current extents.
  QgsRectangle myExtents = mpMapCanvas->extent();
  mpCoordsLabel->setText( tr( "Extents:" ) );
  mpCoordsEdit->setText( myExtents.toString( true ) );
  //ensure the label is big enough
  if ( mpCoordsEdit->width() > mpCoordsEdit->minimumWidth() )
  {
    mpCoordsEdit->setMinimumWidth( mpCoordsEdit->width() );
  }
} 

void KKSGISWidgetBase::selectionChanged( QgsMapLayer *layer )
{
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
    if ( vlayer ){
        showStatusMessage( tr( "%n feature(s) selected on layer %1.", "number of selected features", vlayer->selectedFeatureCount() ).arg( vlayer->name() ) );
    }

    activateDeactivateLayerRelatedActions( layer );
}

void KKSGISWidgetBase::mapCanvas_keyPressed( QKeyEvent *e )
{
  // Delete selected features when it is possible and KeyEvent was not managed by current MapTool
  if (( e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete ) && e->isAccepted() )
  {
    deleteSelected();
  }
}

void KKSGISWidgetBase::showStatusMessage( QString theMessage )
{
  statusBar()->showMessage( theMessage );
}

void KKSGISWidgetBase::removingLayers( QStringList theLayers )
{
  foreach ( const QString &layerId, theLayers )
  {
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer*>(
                               QgsMapLayerRegistry::instance()->mapLayer( layerId ) );
    if ( !vlayer || !vlayer->isEditable() )
      return;

    toggleEditing( vlayer, false );
  }
}

void KKSGISWidgetBase::copyFeatures( QgsFeatureStore & featureStore )
{
  clipboard()->replaceWithCopyOf( featureStore );
}

void KKSGISWidgetBase::labelingFontNotFound( QgsVectorLayer* vlayer, const QString& fontfamily )
{
  // TODO: update when pref for how to resolve missing family (use matching algorithm or just default font) is implemented
  QString substitute = tr( "Default system font substituted." );

  QToolButton* btnOpenPrefs = new QToolButton();
  btnOpenPrefs->setStyleSheet( "QToolButton{ background-color: rgba(255, 255, 255, 0); color: black; text-decoration: underline; }" );
  btnOpenPrefs->setCursor( Qt::PointingHandCursor );
  btnOpenPrefs->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred );
  btnOpenPrefs->setToolButtonStyle( Qt::ToolButtonTextOnly );

  // store pointer to vlayer in data of QAction
  QAction* act = new QAction( btnOpenPrefs );
  act->setData( QVariant( QMetaType::QObjectStar, &vlayer ) );
  act->setText( tr( "Open labeling dialog" ) );
  btnOpenPrefs->addAction( act );
  btnOpenPrefs->setDefaultAction( act );
  btnOpenPrefs->setToolTip( "" );
  connect( btnOpenPrefs, SIGNAL( triggered( QAction* ) ), this, SLOT( labelingDialogFontNotFound( QAction* ) ) );

  // no timeout set, since notice needs attention and is only shown first time layer is labeled
  QgsMessageBarItem* fontMsg = new QgsMessageBarItem(
    tr( "Labeling" ),
    tr( "Font for layer <b><u>%1</u></b> was not found (<i>%2</i>). %3" ).arg( vlayer->name() ).arg( fontfamily ).arg( substitute ),
    btnOpenPrefs,
    QgsMessageBar::WARNING,
    0,
    messageBar() );
  messageBar()->pushItem( fontMsg );
}

void KKSGISWidgetBase::labelingDialogFontNotFound( QAction* act )
{
  if ( !act )
  {
    return;
  }

  // get base pointer to layer
  QObject* obj = qvariant_cast<QObject*>( act->data() );

  // remove calling messagebar widget
  messageBar()->popWidget();

  if ( !obj )
  {
    return;
  }

  QgsMapLayer* layer = qobject_cast<QgsMapLayer*>( obj );
  if ( layer && setActiveLayer( layer ) )
  {
    labeling();
  }
}


void KKSGISWidgetBase::showMouseCoordinate( const QgsPoint & p )
{
  if ( mMapTipsVisible )
  {
    // store the point, we need it for when the maptips timer fires
    mLastMapPosition = p;

    // we use this slot to control the timer for maptips since it is fired each time
    // the mouse moves.
    if ( mpMapCanvas->underMouse() )
    {
      // Clear the maptip (this is done conditionally)
      mpMaptip->clear( mpMapCanvas );
      // don't start the timer if the mouse is not over the map canvas
      mpMapTipsTimer->start();
      //QgsDebugMsg("Started maptips timer");
    }
  }
  if ( mToggleExtentsViewButton->isChecked() )
  {
    //we are in show extents mode so no need to do anything
    return;
  }
  else
  {
    if ( mpMapCanvas->mapUnits() == QGis::Degrees )
    {
      QString format = QgsProject::instance()->readEntry( "PositionPrecision", "/DegreeFormat", "D" );

      if ( format == "DM" )
        mpCoordsEdit->setText( p.toDegreesMinutes( mMousePrecisionDecimalPlaces ) );
      else if ( format == "DMS" )
        mpCoordsEdit->setText( p.toDegreesMinutesSeconds( mMousePrecisionDecimalPlaces ) );
      else
        mpCoordsEdit->setText( p.toString( mMousePrecisionDecimalPlaces ) );
    }
    else
    {
      mpCoordsEdit->setText( p.toString( mMousePrecisionDecimalPlaces ) );
    }

    if ( mpCoordsEdit->width() > mpCoordsEdit->minimumWidth() )
    {
      mpCoordsEdit->setMinimumWidth( mpCoordsEdit->width() );
    }
  }
}

void KKSGISWidgetBase::toggleMapTips()
{
  mMapTipsVisible = !mMapTipsVisible;
  // if off, stop the timer
  if ( !mMapTipsVisible )
  {
    mpMapTipsTimer->stop();
  }
}

// Show the maptip using tooltip
void KKSGISWidgetBase::showMapTip()
{
  // Stop the timer while we look for a maptip
  mpMapTipsTimer->stop();

  // Only show tooltip if the mouse is over the canvas
  if ( mpMapCanvas->underMouse() )
  {
    QPoint myPointerPos = mpMapCanvas->mouseLastXY();

    //  Make sure there is an active layer before proceeding
    QgsMapLayer* mypLayer = mpMapCanvas->currentLayer();
    if ( mypLayer )
    {
      //QgsDebugMsg("Current layer for maptip display is: " + mypLayer->source());
      // only process vector layers
      if ( mypLayer->type() == QgsMapLayer::VectorLayer )
      {
        // Show the maptip if the maptips button is depressed
        if ( mMapTipsVisible )
        {
          mpMaptip->showMapTip( mypLayer, mLastMapPosition, myPointerPos, mpMapCanvas );
        }
      }
    }
    else
    {
      showStatusMessage( tr( "Maptips require an active layer" ) );
    }
  }
}

void KKSGISWidgetBase::createMapTips()
{
  // Set up the timer for maptips. The timer is reset everytime the mouse is moved
  mpMapTipsTimer = new QTimer( mpMapCanvas );
  // connect the timer to the maptips slot
  connect( mpMapTipsTimer, SIGNAL( timeout() ), this, SLOT( showMapTip() ) );
  // set the interval to 0.850 seconds - timer will be started next time the mouse moves
  mpMapTipsTimer->setInterval( 850 );
  // Create the maptips object
  mpMaptip = new QgsMapTip();
}

void KKSGISWidgetBase::extentsViewToggled( bool theFlag )
{
  if ( theFlag )
  {
    //extents view mode!
    mToggleExtentsViewButton->setIcon( QgsApplication::getThemeIcon( "extents.png" ) );
    mpCoordsEdit->setToolTip( tr( "Map coordinates for the current view extents" ) );
    mpCoordsEdit->setReadOnly( true );
    showExtents();
  }
  else
  {
    //mouse cursor pos view mode!
    mToggleExtentsViewButton->setIcon( QgsApplication::getThemeIcon( "tracking.png" ) );
    mpCoordsEdit->setToolTip( tr( "Map coordinates at mouse cursor position" ) );
    mpCoordsEdit->setReadOnly( false );
    mpCoordsLabel->setText( tr( "Coordinate:" ) );
  }
}

void KKSGISWidgetBase::reloadLayer(const QString & theLayerId)
{
    QgsMapLayerRegistry::instance()->mapLayer(theLayerId)->reload();
    //mpMapCanvas->refresh();
}

void KKSGISWidgetBase::slotUpdateMapByNotify(const QString & nName, const QString & tableName, const QString & idRecord)
{
    const QMap<QString, QgsMapLayer *> layers = QgsMapLayerRegistry::instance()->mapLayers();
    if(layers.count() == 0)
        return;

    QMap<QString, QgsMapLayer *>::const_iterator pa;
    QgsVectorLayer * changedLayer = NULL;

    for (pa = layers.constBegin(); pa != layers.constEnd(); pa++)
    {
        QgsMapLayer * layer = pa.value();
        if(layer->type() != QgsMapLayer::VectorLayer)
            continue;
        QgsVectorLayer * vLayer = (QgsVectorLayer*)layer;

        QString providerName = vLayer->dataProvider()->name();
        if(providerName != "postgres")
            continue;

        QString uri = vLayer->dataProvider()->dataSourceUri();
        QStringList uriSections = uri.split(" ");
        QString layerTable;

        for(int i=0; i<uriSections.count(); i++){
            QString & sec = uriSections[i];
            if(sec.startsWith("table=")){
                QStringList tableSec = sec.split("=");
                if(tableSec.count() != 2){
                    break;
                }
                QString fullTableName = tableSec[1];
                QStringList tableNameSec = fullTableName.split(".");
                if(tableNameSec.count() != 2){
                    break;
                }
                layerTable = tableNameSec[1];
                layerTable.replace("\"", "");
                break;
            }
        }

        if(layerTable != tableName)
            continue;
        
        //vLayer->reload();
        //mpMapCanvas->refresh();
        changedLayer = vLayer;
        break;
    }

    if(!changedLayer)
        return;

    changedLayer->setCacheImage(NULL);
    

    QgsFeature f;
    QgsFeatureRequest fr(QString("unique_id = '%1'").arg(idRecord) );
    QgsFeatureIterator fi = changedLayer->getFeatures(fr);
    bool ok = fi.nextFeature(f);
    //bool ok = changedLayer->updateFeature(f);
    if(!ok){
        int a = 0;
        return;
    }
}

void KKSGISWidgetBase::SLOTazChangeColumns(int num)
{
    mAzDialCalcRoute->mComboColDirectionSlope->clear();
    mAzDialCalcRoute->mComboColSlope->clear();
    if (!azSelectLayer(mAzDialCalcRoute->mComboLayerRelief->itemText(num)))
    {
        QMessageBox::information(this, "������", "���������� ������� ����.", QMessageBox::Ok);
        return;
    }
    if (mpSelectedLayer->type() != QgsMapLayer::VectorLayer)
    {
        QMessageBox::information(this, "Error", "Not a vector layer.", QMessageBox::Ok);
        return;
    }
    QgsVectorLayer * pLayer = (QgsVectorLayer*) mpSelectedLayer;
    QgsFields pFields;
    pFields = pLayer->pendingFields();
    for (int i = 0; i < pFields.count(); i++)
    {
        QgsField pField = pFields.at(i);
        if (pField.type() == QVariant::Double ||
            pField.type() == QVariant::Int ||
            pField.type() == QVariant::LongLong)
        {
                mAzDialCalcRoute->mComboColDirectionSlope->addItem(pField.name());
                mAzDialCalcRoute->mComboColSlope->addItem(pField.name());
        }
    }
}
