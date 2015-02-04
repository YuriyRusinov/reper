#ifndef KKSGISWIDGETBASE
#define KKSGISWIDGETBAS_H

#include "kksqgis_config.h"

//QT Includes
#include <QObject>
#include <QWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QMap>
#include <QMenu>
#include <QToolBar>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QProgressBar>
#include <QDomDocument>

#undef min
#undef max

#include <qgspoint.h>
#include "qgsfeaturestore.h"

#include "dn/azdialcalcroute.h" //az

class QToolButton;

#ifdef WIN32
//Denis Includes (dn)
class DNSpecBath;
#endif

class KKSBadLayerHandler;
class QgsMapLayer;
class QgsRasterLayer;
class QgsMapCanvas;
class QgsMapCanvasLayer;
class QgsMapTool;
class QgsLegend;
class QgsLayerOrder;
class QgsMapLayerRegistry;
class QTableWidget;
class QgsPalLabeling;
class QgsMessageBar;
class QgsScaleComboBox;
class QgsVectorLayer;
class QgsClipboard;
class QgsVectorLayerTools;
class QgsMapTip;

class _QGIS_EXPORT KKSGISWidgetBase : public QWidget
{
  Q_OBJECT
public:
    KKSGISWidgetBase(bool withSubWindows, bool withAddons = true, QWidget* parent = 0, Qt::WFlags fl = 0 );
    virtual ~KKSGISWidgetBase();

    void initQGIS();
    bool initCorrectly() const {return m_bInit;}

//ksa
public:
    void showIOEditor(QWidget * parent, const QString & uid);
    bool featureFromEIO(QWidget * parent, QgsFeature & feature, const QString & geomAsEWKT, const QString & layerTable);
    bool deleteFeaturesAsEIO(QWidget * parent, const QString & tableName, const QList<qint64> & ids);

signals:
    void signalShowIOEditor(QWidget * parent, const QString & uid);

    void signalFeatureFromEIO(QWidget * parent, QgsFeature & feature, const QString & geomAsEWKT, const QString & layerTable);
    void signalDeleteFeaturesAsEIO(QWidget * parent, const QString & tableName, const QList<qint64> & ids);
//ksa

public:

    QStatusBar * statusBar() const;
    const QString & pluginsDir();
    //QTableWidget * tableLegend();
    QWidget * mapLegendWidget();
    QWidget * mapLayerOrderWidget();
    //QToolBar * toolBar() const;
    QMenuBar * menuBar() const;

    const QMap<QString, QMenu *> & menuMap() const;
    const QMap<QString, QToolBar *> & toolBarMap() const;

    QString projectFileName();
    QString readLayerFilePath(const QString & file) const;//возвращает абсолютный путь к файлу сло€, который прочитан из файла проекта QGIS

    AzDialCalcRoute * mAzDialCalcRoute;


    void openProject(const QString & prjFile);
    void closeProject();
    void saveProjectAs(const QString & prjFile);
    bool saveProject();
    void openVectorLayer(const QString & vLayerFile);
    void openRasterLayer(const QString & rLayerFile);
    void openDatabaseLayer();
    QString azCreateName(const int option = 0); // Az
    bool azCreateLayer(QString nameOfLayer, QgsFields pFields, QString pathOfLayer = "C:/temp/",
                       bool addToMap = true, bool useDefaultFields = false, QGis::WkbType pType = QGis::WKBPolygon,
                       const QString encoding = "UTF-8", const long EPSG = 4326); // Az

    bool addVectorLayers( QStringList const & theLayerQStringList, const QString& enc, const QString dataSourceType );
    bool addRasterLayers( QStringList const & theLayerQStringList, bool guiWarning = true );
    void showLayerProperties( QgsMapLayer *ml );

    QgsMapCanvas * mapCanvas() {return mpMapCanvas;}
    QgsPalLabeling * palLabeling();
    QgsMessageBar* messageBar();
    
    QgsLegend * legend();


    /**
     * Access the vector layer tools. This will be an instance of {@see QgsGuiVectorLayerTools}
     * by default.
     * @return  The vector layer tools
     */
    QgsVectorLayerTools* vectorLayerTools() { return mVectorLayerTools; }
    /** Return vector layers in edit mode
     * @param modified whether to return only layers that have been modified
     * @returns list of layers in legend order, or empty list
     * @note added in 1.9 */
    QList<QgsMapLayer *> editableLayers( bool modified = false ) const;

    QAction * actionNewProject() { return mActionNewProject; }

    QAction *actionRemoveLayer() { return mActionRemoveLayer; }
    QAction *actionDuplicateLayer() { return mActionDuplicateLayer; }
    QAction *actionSetLayerCRS() { return mActionSetLayerCRS; }
    QAction *actionSetProjectCRSFromLayer() { return mActionSetProjectCRSFromLayer; }
    QAction *actionLayerProperties() { return mActionLayerProperties; }
    QAction *actionLayerSubsetString() { return mActionLayerSubsetString; }
    QAction *actionToggleEditing() { return mActionToggleEditing; }
    QAction *actionSaveActiveLayerEdits() { return mActionSaveLayerEdits; }
    QAction *actionAllEdits() { return mActionAllEdits; }
    QAction *actionSaveEdits() { return mActionSaveEdits; }
    QAction *actionSaveAllEdits() { return mActionSaveAllEdits; }
    QAction *actionCopyLayerStyle() { return mActionCopyStyle; }
    QAction *actionPasteLayerStyle() { return mActionPasteStyle; }
    QAction *actionOpenTable() { return mActionOpenTable; }
    QAction *actionLayerSaveAs() { return mActionLayerSaveAs; }
    QAction *actionLayerSelectionSaveAs() { return mActionLayerSelectionSaveAs; }
    QAction *actionProjectProperties() { return mActionProjectProperties; }
    
    QAction *actionPanToSelected() { return mActionPanToSelected; }

    QAction *actionSelect() { return mActionSelect; }
    QAction *actionSelectRectangle() { return mActionSelectRectangle; }
    QAction *actionSelectPolygon() { return mActionSelectPolygon; }
    QAction *actionSelectFreehand() { return mActionSelectFreehand; }
    QAction *actionSelectRadius() { return mActionSelectRadius; }

    QAction *actionIdentify() { return mActionIdentify; }

    QAction *actionMeasure() { return mActionMeasure; }
    QAction *actionMeasureArea() { return mActionMeasureArea; }

    QAction *actionZoomFullExtent() { return mActionZoomFullExtent; }
    QAction *actionZoomToLayer() { return mActionZoomToLayer; }
    QAction *actionZoomToSelected() { return mActionZoomToSelected; }
    QAction *actionZoomLast() { return mActionZoomLast; }
    QAction *actionZoomNext() { return mActionZoomNext; }
    QAction *actionZoomActualSize() { return mActionZoomActualSize; }
    QAction *actionDraw() { return mActionDraw; }
    QAction *actionMapTips() { return mActionMapTips; }


#ifdef Q_OS_WIN
    //! ugly hack
    void skipNextContextMenuEvent();
#endif


    //! Returns a pointer to the internal clipboard
    QgsClipboard * clipboard();

    /** Get timeout for timed messages: default of 5 seconds
     * @note added in 1.9 */
    int messageTimeout();


public slots:
    void layerProperties();
    void markDirty();
    void labeling();

    bool toggleEditing( QgsMapLayer *layer, bool allowCancel = true );
    void toggleEditing();

    void saveEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint );
    void saveActiveLayerEdits();
    void saveAllEdits( bool verifyAction = true );
    void saveEdits();

    /** Rollback current edits for selected layer(s) and start new transaction(s)
      * @note added in 1.9 */
    void rollbackEdits();

    /** Rollback edits for all layers and start new transactions
     * @note added in 1.9 */
    void rollbackAllEdits( bool verifyAction = true );

    /** Cancel edits for selected layer(s) and toggle off editing
      * @note added in 1.9 */
    void cancelEdits();
    
    void updateUndoActions();
    
    void reloadLayer(const QString & theLayerId);

    /** Cancel edits for a layer
      * @param leaveEditable leave the layer in editing mode when done
      * @param triggerRepaint send layer signal to repaint canvas when done
      * @note added in 1.9
      */
    void cancelEdits( QgsMapLayer *layer, bool leaveEditable = true, bool triggerRepaint = true );

    /** Cancel all edits for all layers and toggle off editing
     * @note added in 1.9 */
    void cancelAllEdits( bool verifyAction = true );
    //! cuts selected features on the active layer to the clipboard
    /**
       \param layerContainingSelection  The layer that the selection will be taken from
                                        (defaults to the active layer on the legend)
     */
    void editCut( QgsMapLayer * layerContainingSelection = 0 );

    //! copies selected features on the active layer to the clipboard
    /**
       \param layerContainingSelection  The layer that the selection will be taken from
                                        (defaults to the active layer on the legend)
     */
    void editCopy( QgsMapLayer * layerContainingSelection = 0 );
    //! copies features on the clipboard to the active layer
    /**
       \param destinationLayer  The layer that the clipboard will be pasted to
                                (defaults to the active layer on the legend)
     */
    void editPaste( QgsMapLayer * destinationLayer = 0 );

    /**Deletes the selected attributes for the currently selected vector layer*/
    void deleteSelected( QgsMapLayer *layer = 0, QWidget* parent = 0 );

    void slotUpdateMapByNotify(const QString &, const QString &, const QString &);
    void SLOTazChangeColumns(int num); //az
    void SLOTshortestPathCalculateMath(bool CalcIt); //az

signals:
    /** emitted when a project file is successfully read
      @note
      This is useful for plug-ins that store properties with project files.  A
      plug-in can connect to this signal.  When it is emitted, the plug-in
      knows to then check the project properties for any relevant state.
      */
    void projectRead();
    void newProject();
    
    void dataChanged();
    void mapSaved(QDomDocument&);
    void SIGNALchangeWindowTitle();


private slots:
    void zoomInMode();
    void zoomOutMode();
    void panMode();
    void azLoadLayer(QgsMapLayer *theMapLayer, bool isExtent = false);
    void azAddWorkListToMap(QStringList & pList);
    void azRemoveAnnotationItems();
    void azRemoveAllLayers();
    bool azSelectLayer(const QString layerName);
    bool azSelectLayer(const int layerNumber);
    bool azSelectByIntersection(QgsVectorLayer * pMainLayer, QgsVectorLayer * pSelectLayer); //az
    bool azRasterEnhancement(QgsRasterLayer & azRasterLayer);
    bool azRasterCheckBandName(QgsRasterLayer & azRasterLayer, QString strBandName);
    bool azCopyFiles(QString azSource, QString azDestPath, bool bUse = false);

    void azVectorize();
    //bool azMakeLayer(QGis::WkbType azType, QString pDestDir, QString pName);
    bool azAddLayerVector(QFileInfo pFile, bool extent);
    //void addLayerToTOC(QgsMapLayer *mapLayer);

    void clipboardChanged();
    bool verifyEditsActionDialog( const QString& act, const QString& upon );


private slots:
    void SLOTazCoordsCenter();
    void SLOTazThemTaskSpectralBathynometry();


    //void SLOTazShowMouseCoordinate(const QgsPoint & p);
    
    //void SLOTmpActionFileExit();
    void SLOTmpActionFileNew();
    void SLOTmpActionFileOpenProject();
    void SLOTmpActionFileSaveProjectAs();
    void SLOTmpActionFileSaveProject();
    void SLOTmpCloseProject();

    void SLOTmpActionAddVectorLayer();
    void SLOTmpActionAddRasterLayer();
    void SLOTmpActionAddPostGISLayer();

    void addDatabaseLayers( QStringList const & layerPathList, QString const & providerKey );

    void SLOTmpActionVectorize();
    void SLOTsetRenderer();
    void SLOTshortestPathSelectArea(); //az
    void SLOTshortestPathCalculate(); //az
    void SLOTshortestPathGridArea(); //az

    void SLOTtempUse(); //az

    //! Create a new blank project (no template)
    void fileNewBlank();
    //! As above but allows forcing without prompt and forcing blank project
    void fileNew( bool thePromptToSaveFlag, bool forceBlank = false );

    void destinationSrsChanged();
    void hasCrsTransformEnabled( bool theFlag );
    //! Open project properties dialog and show the projections tab
    void projectPropertiesProjections();
    //! Set project properties, including map untis
    void projectProperties();

    //! Slot to show the map coordinate position of the mouse cursor
    void showMouseCoordinate( const QgsPoint & );

    /** Alerts user when commit errors occured
     * @note added in 2.0
     */
    void commitError( QgsVectorLayer* vlayer );

    /** Called when some layer's editing mode was toggled on/off
     * @note added in 1.9 */
    void layerEditStateChanged();

    /* changed from layerWasAdded in 1.8 */
    void layersWereAdded( QList<QgsMapLayer *> );

    /*from QgsApp*/
    void removeLayer(bool promptConfirmation = true);
    /** Duplicate map layer(s) in legend
     * @note added in 1.9 */
    void duplicateLayers( const QList<QgsMapLayer *> lyrList = QList<QgsMapLayer *>() );
    //! Set CRS of a layer
    void setLayerCRS();
    //! Assign layer CRS to project
    void setProjectCRSFromLayer();
    //! Slot to handle user scale input;
    void userScale();
    void showScale( double theScale );
    void updateMouseCoordinatePrecision();
    //! copies style of the active layer to the clipboard
    /**
       \param sourceLayer  The layer where the style will be taken from
                                        (defaults to the active layer on the legend)
     */
    void copyStyle( QgsMapLayer * sourceLayer = 0 );
    //! pastes style on the clipboard to the active layer
    /**
       \param destinatioLayer  The layer that the clipboard will be pasted to
                                (defaults to the active layer on the legend)
     */
    void pasteStyle( QgsMapLayer * destinationLayer = 0 );
    //! show the attribute table for the currently selected layer
    void attributeTable();
    //! save current vector layer
    void saveAsFile();
    void saveSelectionAsVectorFile();

    //! save current raster layer
    void saveAsRasterFile();
    //! change layer subset of current vector layer
    void layerSubsetString();

    //! map tool changed
    void mapToolChanged( QgsMapTool *tool );
    //! Toggle map tips on/off
    void toggleMapTips();
    //! Show the map tip
    void showMapTip();

    void extentsViewToggled( bool theFlag );

    void showProgress( int theProgress, int theTotalSteps );

    //! layer selection changed
    void legendLayerSelectionChanged( void );
    /** Activates or deactivates actions depending on the current maplayer type.
    Is called from the legend when the current legend item has changed*/
    void activateDeactivateLayerRelatedActions( QgsMapLayer* layer );
    //! Add a raster layer to the map (will prompt user for file name using dlg )
    void addRasterLayer();
    void loadOGRSublayers( QString layertype, QString uri, QStringList list );

    /** Update gui actions/menus when layers are modified
     * @note added in 1.9 */
    void updateLayerModifiedActions();

    /** Alerts user when labeling font for layer has not been found on system
     * @note added in 1.9
     */
    void labelingFontNotFound( QgsVectorLayer* vlayer, const QString& fontfamily );
    /** Opens the labeling dialog for a layer when called from labelingFontNotFound alert
     * @note added in 1.9
     */
    void labelingDialogFontNotFound( QAction* act );

    void showExtents();
    void selectionChanged( QgsMapLayer *layer );
    //! catch MapCanvas keyPress event so we can check if selected feature collection must be deleted
    void mapCanvas_keyPressed( QKeyEvent *e );
    void showStatusMessage( QString theMessage );
    /* layer will be removed - changed from removingLayer to removingLayers
       in 1.8.    */
    void removingLayers( QStringList );
    //! copies features to internal clipboard
    void copyFeatures( QgsFeatureStore & featureStore );

    /** open a raster layer for the given file
      @returns false if unable to open a raster layer for rasterFile
      @note
      This is essentially a simplified version of the above
    */
    QgsRasterLayer* addRasterLayer( QString const & rasterFile, QString const & baseName, bool guiWarning = true );

    /** Open a raster layer - this is the generic function which takes all parameters
     * @note added in version 2.0
      */
    QgsRasterLayer* addRasterLayerPrivate( const QString & uri, const QString & baseName,
                                           const QString & providerKey, bool guiWarning,
                                           bool guiUpdate );

    void loadGDALSublayers( QString uri, QStringList list );

    //! activates the selection tool
    void select();
    //! activates the rectangle selection tool
    void selectByRectangle();
    //! activates the polygon selection tool
    void selectByPolygon();
    //! activates the freehand selection tool
    void selectByFreehand();
    //! activates the radius selection tool
    void selectByRadius();
    //! deselect features from all layers
    void deselectAll();
    //! select features by expression
    void selectByExpression();
    //! refresh map canvas
    void refreshMapCanvas();
    //! Identify feature(s) on the currently selected layer
    void identify();
    //! Measure distance
    void measure();
    //! Measure area
    void measureArea();
    //! Measure angle
    void measureAngle();
    //! zoom to actual size of raster layer
    void zoomActualSize();
    //! zoom to extent of layer
    void zoomToLayerExtent();
    //! Zoom to full extent
    void zoomFull();
    //! Zoom to the previous extent
    void zoomToPrevious();
    //! Zoom to the forward extent
    void zoomToNext();
    //! Zoom to selected features
    void zoomToSelected();
    //! Pan map to selected features
    //! @note added in 2.0
    void panToSelected();
    //! activates the add feature tool
    void addFeature();
    //! activates the move feature tool
    void moveFeature();
    //! provides operations with nodes
    void nodeTool();



private:
    void readSettings();
    
    void initStatusBar();
    void initUserSettings();
    void initMapCanvas();
    //void initLegend();
    void createMapTips();
    void initMapLegend();
    void initActions();
    void initConnections();
    void initToolBar();
    void initMenuBar();
    //void initMapTools();
    void initCanvasTools();

    void updateCRSStatusBar();

    void azSetTitleWindow(QWidget & azApp);
    bool azShortestPathWave(QgsVectorLayer *pVectorLayer, QMap<QString, QgsFeatureId> pMap);
    QgsMapLayer *activeLayer();
    bool setActiveLayer( QgsMapLayer * );

    void saveAsVectorFileGeneral( bool saveOnlySelection, QgsVectorLayer* vlayer = 0, bool symbologyOption = true );
    void setTheme( QString theThemeName );

    //! check to see if file is dirty and if so, prompt the user th save it
    bool saveDirty();

    /** This method will open a dialog so the user can select GDAL sublayers to load
     * @returns true if any items were loaded
     * @note added in version 1.9
     */
    bool askUserForZipItemLayers( QString path );
    /** This method will open a dialog so the user can select GDAL sublayers to load
     * @note added in version 1.8
     */
    void askUserForGDALSublayers( QgsRasterLayer *layer );
    bool shouldAskUserForGDALSublayers( QgsRasterLayer *layer );
    
    /** This method will verify if a GDAL layer contains sublayers
     * @note added in version 1.8
     */
    /** This method will open a dialog so the user can select OGR sublayers to load
    */
    void askUserForOGRSublayers( QgsVectorLayer *layer );

    bool addRasterLayer( QgsRasterLayer * theRasterLayer );

protected:

    //! refresh map canvas only
    void refreshMapCanvasOnly();

#ifdef Q_OS_WIN
    //! reimplements context menu event
    virtual void contextMenuEvent( QContextMenuEvent *event );
#endif


private:
    QStringList azWorkList; // working list of anyone
    QgsMapLayer * mpSelectedLayer; //selected map layer

    // Setup's for statusBar
    QLabel * mpCoordsLabel;
    QLineEdit * mpCoordsEdit;
    QValidator * mpCoordsEditValidator;
    QgsPoint mpLastMapPosition;
    QStatusBar * mpStatusBar;

    //! Widget that will live on the statusbar to display "scale 1:"
    QLabel * mScaleLabel;
    //! Widget that will live on the statusbar to display scale value
    QgsScaleComboBox * mScaleEdit;
    //! The validator for the mScaleEdit
    QValidator * mScaleEditValidator;
    //! Widget that will live in the statusbar to show progress of operations
    QProgressBar * mProgressBar;

    KKSBadLayerHandler * m_badLayerHandler; //обработчик слоев, которые по тем или иным причинам не загрузились из файла проекта

    //! Flag to indicate how the project properties dialog was summoned
    bool mShowProjectionTab;

    //QgsRasterLayer *testLayer;
    QgsMapLayerRegistry *mpRegistry;
    QgsMapCanvas * mpMapCanvas;
    QgsLegend * mpMapLegend;
    QWidget * mpMapLegendWidget;
    QgsLayerOrder *mpMapLayerOrder;
    QWidget * mpMapLayerOrderWidget;
    //ksa QList<QgsMapCanvasLayer> mpLayerSet;
    QgsPalLabeling* mLBL;

    QgsClipboard * mInternalClipboard;

    QToolBar * mpMapToolBar;
    QToolBar * mpDataSourceToolBar;
    QToolBar * mpLayerToolBar;
    QToolBar * mpToolsToolBar;
    QToolBar * mpTaskToolBar;
    QToolBar * mpLayerEditsToolBar; //редактирование векторных слоев
    QMap<QString, QToolBar *> mpToolBarMap;

    /*
    QgsMapTool * mpPanTool;
    QgsMapTool * mpZoomInTool;
    QgsMapTool * mpZoomOutTool;
    QgsMapTool * mpSelect;
    */
    class Tools
    {
      public:
        QgsMapTool* mZoomIn;
        QgsMapTool* mZoomOut;
        QgsMapTool* mPan;
#ifdef HAVE_TOUCH
        QgsMapTool* mTouch;
#endif
        QgsMapTool* mIdentify;
        //QgsMapTool* mFeatureAction;
        QgsMapTool* mMeasureDist;
        QgsMapTool* mMeasureArea;
        QgsMapTool* mMeasureAngle;
        QgsMapTool* mAddFeature;
        QgsMapTool* mMoveFeature;
        //QgsMapTool* mOffsetCurve;
        //QgsMapTool* mReshapeFeatures;
        //QgsMapTool* mSplitFeatures;
        //QgsMapTool* mSplitParts;
        QgsMapTool* mSelect;
        QgsMapTool* mSelectRectangle;
        QgsMapTool* mSelectPolygon;
        QgsMapTool* mSelectFreehand;
        QgsMapTool* mSelectRadius;
        //QgsMapTool* mVertexAdd;
        //QgsMapTool* mVertexMove;
        //QgsMapTool* mVertexDelete;
        //QgsMapTool* mAddRing;
        //QgsMapTool* mAddPart;
        //QgsMapTool* mSimplifyFeature;
        //QgsMapTool* mDeleteRing;
        //QgsMapTool* mDeletePart;
        QgsMapTool* mNodeTool;
        //QgsMapTool* mRotatePointSymbolsTool;
        //QgsMapTool* mAnnotation;
        //QgsMapTool* mFormAnnotation;
        //QgsMapTool* mHtmlAnnotation;
        //QgsMapTool* mSvgAnnotation;
        //QgsMapTool* mTextAnnotation;
        //QgsMapTool* mPinLabels;
        //QgsMapTool* mShowHideLabels;
        //QgsMapTool* mMoveLabel;
        //QgsMapTool* mRotateFeature;
        //QgsMapTool* mRotateLabel;
        //QgsMapTool* mChangeLabelProperties;
    } mMapTools;

    QgsMapTool *mNonEditMapTool;


    //! a bar to display warnings in a non-blocker manner
    QgsMessageBar *mInfoBar;

    //! Widget in status bar used to show current project CRS
    QLabel * mOnTheFlyProjectionStatusLabel;
    //! Widget in status bar used to show status of on the fly projection
    QToolButton * mOnTheFlyProjectionStatusButton;
    //! Menu that contains the list of actions of the selected vector layer

    //! A toggle to switch between mouse coords and view extents display
    QToolButton * mToggleExtentsViewButton;

    //строка меню
    QMenuBar * mpMenuBar;
    QMap<QString, QMenu *> mpMenuMap;
    // различные операции
    QAction * mpVectorize;          //
    QAction * mpActionBathymetry;   // Ѕатинометри€
    QAction * mpActionShortestPathAreaSelect; // ѕоиск маршрута военной техники, выбор зоны интереса Az
    QAction * mpActionShortestPathCalc; // ѕоиск маршрута военной техники, расчет Az
    QAction * mpActionShortestPathGrid; // ѕоиск маршрута военной техники, сетка Az
    QAction * mActionMapTips;

    //добавление слоев
    QAction * mpActionAddVectorLayer;
    QAction * mpActionAddRasterLayer;
    QAction * mpActionAddPostGISLayer;
    
    //работа с проектом
    QAction * mActionNewProject;
    QAction * mpActionFileOpenProject;
    QAction * mpActionFileSaveProject;
    QAction * mpActionFileSaveProjectAs;
    QAction * mpActionFileCloseProject;
    QAction * mActionProjectProperties;

    //навигаци€ по карте
    QAction * mpActionZoomIn;
    QAction * mpActionZoomOut;
    QAction * mpActionPan;
    QAction * mActionPanToSelected;
    QAction * mActionZoomFullExtent;
    QAction * mActionZoomToSelected;
    QAction * mActionZoomLast;
    QAction * mActionZoomNext;
    QAction * mActionZoomToLayer;
    QAction * mActionZoomActualSize;
    QAction * mActionDraw;
    QAction * mActionIdentify;
    
    QAction * mActionAllSelect;
    QAction * mActionSelect;
    QAction * mActionSelectRectangle;
    QAction * mActionSelectPolygon;
    QAction * mActionSelectFreehand;
    QAction * mActionSelectRadius;
    QAction * mActionDeselectAll;
    QAction * mActionSelectByExpression;
    
    QAction * mActionAllMeasure;
    QAction * mActionMeasure;
    QAction * mActionMeasureArea;
    QAction * mActionMeasureAngle;


    QAction * mActionRemoveLayer;//
    QAction * mActionDuplicateLayer;//
    QAction * mActionSetLayerCRS;//
    QAction * mActionSetProjectCRSFromLayer;//
    QAction * mActionLayerProperties;//
    QAction * mActionLayerSubsetString;//

    //редактирование векторных слоев
    QAction * mActionAllEdits;//
    QAction * mActionToggleEditing;//
    QAction * mActionSaveLayerEdits;//
    QAction * mActionAddFeature;
    QAction * mActionMoveFeature;
    QAction * mActionNodeTool;
    QAction * mActionDeleteSelected;
    QAction * mActionCutFeatures;
    QAction * mActionCopyFeatures;
    QAction * mActionPasteFeatures;
    //--
    QAction * mActionSaveEdits;//
    QAction * mActionRollbackEdits;
    QAction * mActionCancelEdits;
    QAction * mActionSaveAllEdits;//
    QAction * mActionRollbackAllEdits;
    QAction * mActionCancelAllEdits;
    
    //операции со сло€ми
    QAction * mActionCopyStyle;//
    QAction * mActionPasteStyle;//
    QAction * mActionOpenTable;//
    QAction * mActionLayerSaveAs;
    QAction * mActionLayerSelectionSaveAs;

    /** Flag to indicate an edits save/rollback for active layer is in progress
     * @note added in QGIS 1.9
     */
    bool mSaveRollbackInProgress;
    
    //! How to determine the number of decimal places used to
    //! display the mouse position
    bool mMousePrecisionAutomatic;
    //! The number of decimal places to use if not automatic
    unsigned int mMousePrecisionDecimalPlaces;

    /** String containing supporting vector file formats
      Suitable for a QFileDialog file filter.  Build in ctor.
      */
    QString mVectorFileFilter;
    /** String containing supporting raster file formats
      Suitable for a QFileDialog file filter.  Build in ctor.
      */
    QString mRasterFileFilter;

    QgsVectorLayerTools* mVectorLayerTools;

    /** Timer for map tips
     */
    QTimer *mpMapTipsTimer;

    /** Point of last mouse position in map coordinates (used with MapTips)
     */
    QgsPoint mLastMapPosition;

    /* Maptip object
     */
    QgsMapTip *mpMaptip;

    // Flag to indicate if maptips are on or off
    bool mMapTipsVisible;


    QString mpAppPath;
    QString m_pluginsDir;

    bool m_bWithSubwindows; //надо ли создавать дополнительные окна (меню, тулбар, статусбыр и т.п.)
    bool m_bWithAddons; //надо ли создавать меню дл€ тематической обработки (нет, если работаем в составе DynamicDocs)
    bool m_bInit; //если виджет был инициализирован (т.е. создан канвас, окна и т.п.), то true

#ifdef WIN32
    // dn's
    DNSpecBath *dnThemTaskSpecBath;
#endif

    // functions
    long azGetEPSG(const QString rastrPath);

#ifdef Q_OS_WIN
    int mSkipNextContextMenuEvent; // ugly hack
#endif

};


#endif
