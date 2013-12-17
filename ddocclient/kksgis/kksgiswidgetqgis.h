#ifndef KKSGISWIDGETQGIS_H
#define KKSGISWIDGETQGIS_H

#include "kksgis_config.h"
#include "kksgiswidget.h"

//QT Includes
#include <QtGui>

#undef min
#undef max

#include <qgsapplication.h>
#include <qgspoint.h>

//Denis Includes (dn)
#include "dn/dnspecbath.h"
#include "dn/Added/dnvector.h"

class KKSBadLayerHandler;
class QgsMapLayer;
class QgsRasterLayer;
class QgsPoint;
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

class _GIS_EXPORT KKSGISWidgetQGIS : public KKSGISWidget
{
  Q_OBJECT;
  friend class KKSGISWidget;
public:
    QStatusBar * statusBar() const;
    const QString & pluginsDir();
    //QTableWidget * tableLegend();
    QWidget * mapLegendWidget();
    QWidget * mapLayerOrderWidget();
    QToolBar * toolBar() const;
    QMenuBar * menuBar() const;

    const QMap<QString, QMenu *> & menuMap() const;

    QString projectFileName();

    void openProject(const QString & prjFile);
    void closeProject();
    void saveProjectAs(const QString & prjFile);
    void saveProject();
    void openVectorLayer(const QString & vLayerFile);
    void openRasterLayer(const QString & rLayerFile);
    void openDatabaseLayer();

    bool addVectorLayers( QStringList const & theLayerQStringList, const QString& enc, const QString dataSourceType );
    bool addRasterLayers( QStringList const & theLayerQStringList, bool guiWarning = true );


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

private:
    KKSGISWidgetQGIS(bool withSubWindows, QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~KKSGISWidgetQGIS();

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

    static bool azRasterEnhancement(QgsRasterLayer & azRasterLayer);
    static bool azRasterCheckBandName(QgsRasterLayer & azRasterLayer, QString strBandName);
    static bool azCopyFiles(QString azSource, QString azDestPath, bool bUse = false);

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
    void initMapTools();

    void azSetTitleWindow(QWidget & azApp);
    QgsMapLayer *activeLayer();
    bool setActiveLayer( QgsMapLayer * );
    void showLayerProperties( QgsMapLayer *ml );

    void saveAsVectorFileGeneral( bool saveOnlySelection, QgsVectorLayer* vlayer = 0, bool symbologyOption = true );
    void setTheme( QString theThemeName );

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

    QgsMapTool * mpPanTool;
    QgsMapTool * mpZoomInTool;
    QgsMapTool * mpZoomOutTool;

    //! a bar to display warnings in a non-blocker manner
    QgsMessageBar *mInfoBar;


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
    QAction * mpActionFileOpenProject;
    QAction * mpActionFileSaveProject;
    QAction * mpActionFileSaveProjectAs;
    QAction * mpActionFileCloseProject;

    //навигация по карте
    QAction * mpActionZoomIn;
    QAction * mpActionZoomOut;
    QAction * mpActionPan;

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

    bool m_bWithSubwindows;

    // dn's
    DNSpecBath *dnThemTaskSpecBath;

    // functions
    long azGetEPSG(const QString rastrPath);
};


#endif
