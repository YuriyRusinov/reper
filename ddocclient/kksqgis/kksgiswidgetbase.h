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

//Denis Includes (dn)
class DNSpecBath;

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
signals:
    void signalShowIOEditor(QWidget * parent, const QString & uid);
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
    QString readLayerFilePath(const QString & file) const;//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS

    void openProject(const QString & prjFile);
    void closeProject();
    void saveProjectAs(const QString & prjFile);
    bool saveProject();
    void openVectorLayer(const QString & vLayerFile);
    void openRasterLayer(const QString & rLayerFile);
    void openDatabaseLayer();


    bool addVectorLayers( QStringList const & theLayerQStringList, const QString& enc, const QString dataSourceType );
    bool addRasterLayers( QStringList const & theLayerQStringList, bool guiWarning = true );

    void showLayerProperties( QgsMapLayer *ml );

    QgsMapCanvas * mapCanvas() {return mpMapCanvas;}
    QgsPalLabeling * palLabeling();
    QgsMessageBar* messageBar();
    int messageTimeout();

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



    //! Returns a pointer to the internal clipboard
    QgsClipboard * clipboard();

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

    bool azRasterEnhancement(QgsRasterLayer & azRasterLayer);
    bool azRasterCheckBandName(QgsRasterLayer & azRasterLayer, QString strBandName);
    bool azCopyFiles(QString azSource, QString azDestPath, bool bUse = false);

    void azVectorize();
    //bool azMakeLayer(QGis::WkbType azType, QString pDestDir, QString pName);
    bool azAddLayerVector(QFileInfo pFile);
    //void addLayerToTOC(QgsMapLayer *mapLayer);

    void clipboardChanged();
    bool verifyEditsActionDialog( const QString& act, const QString& upon );


private slots:
    void SLOTazContextShowExtent();
    void SLOTazCoordsCenter();
    void SLOTazGetSelectedLegendItem();
    void SLOTazThemTaskSpectralBathynometry();
    void SLOTazShowContextMenuForLegend(const QPoint & pos);
    void SLOTazShowMouseCoordinate(const QgsPoint & p);
    
    //void SLOTmpActionFileExit();
    void SLOTmpActionFileNew();
    void SLOTmpActionFileOpenProject();
    void SLOTmpActionFileSaveProjectAs();
    void SLOTmpActionFileSaveProject();
    void SLOTmpCloseProject();

    void SLOTmpActionAddVectorLayer();
    void SLOTmpActionAddRasterLayer();
    void SLOTmpActionAddPostGISLayer();

    void SLOTmpActionVectorize();
    void SLOTsetRenderer();
    void SLOTtempUse();
    
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


    /*from QgsApp*/
    void removeLayer();
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

    void showProgress( int theProgress, int theTotalSteps );

    //! layer selection changed
    void legendLayerSelectionChanged( void );
    /** Activates or deactivates actions depending on the current maplayer type.
    Is called from the legend when the current legend item has changed*/
    void activateDeactivateLayerRelatedActions( QgsMapLayer* layer );
    //! Add a raster layer to the map (will prompt user for file name using dlg )
    void addRasterLayer();
    void loadOGRSublayers( QString layertype, QString uri, QStringList list );

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


private:
    void readSettings();
    
    void initStatusBar();
    void initUserSettings();
    void initMapCanvas();
    //void initLegend();
    void initMapLegend();
    void initActions();
    void initConnections();
    void initToolBar();
    void initMenuBar();
    //void initMapTools();
    void initCanvasTools();

    void updateCRSStatusBar();

    void azSetTitleWindow(QWidget & azApp);
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
    QList<QgsMapCanvasLayer> mpLayerSet;
    QgsPalLabeling* mLBL;

    QgsClipboard * mInternalClipboard;

    QToolBar * mpMapToolBar;
    QToolBar * mpDataSourceToolBar;
    QToolBar * mpLayerToolBar;
    QToolBar * mpToolsToolBar;
    QToolBar * mpTaskToolBar;
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
        //QgsMapTool* mAddFeature;
        //QgsMapTool* mMoveFeature;
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
        //QgsMapTool* mNodeTool;
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



    //! a bar to display warnings in a non-blocker manner
    QgsMessageBar *mInfoBar;

    //! Widget in status bar used to show current project CRS
    QLabel * mOnTheFlyProjectionStatusLabel;
    //! Widget in status bar used to show status of on the fly projection
    QToolButton * mOnTheFlyProjectionStatusButton;
    //! Menu that contains the list of actions of the selected vector layer

    //строка меню
    QMenuBar * mpMenuBar;
    QMap<QString, QMenu *> mpMenuMap;
    // различные операции
    QAction * mpVectorize;
    QAction * mpActionBathymetry;
    
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

    //навигация по карте
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
    QAction * mActionToggleEditing;//
    QAction * mActionSaveLayerEdits;//
    QAction * mActionAllEdits;//
    QAction * mActionSaveEdits;//
    QAction * mActionSaveAllEdits;//
    QAction * mActionCopyStyle;//
    QAction * mActionPasteStyle;//
    QAction * mActionOpenTable;//
    QAction * mActionLayerSaveAs;
    QAction * mActionLayerSelectionSaveAs;

    //

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

    QString mpAppPath;
    QString m_pluginsDir;

    bool m_bWithSubwindows; //надо ли создавать дополнительные окна (меню, тулбар, статусбыр и т.п.)
    bool m_bWithAddons; //надо ли создавать меню для тематической обработки (нет, если работаем в составе DynamicDocs)
    bool m_bInit; //если виджет был инициализирован (т.е. создан канвас, окна и т.п.), то true


    // dn's
    DNSpecBath *dnThemTaskSpecBath;

    // functions
    long azGetEPSG(const QString rastrPath);
};


#endif
