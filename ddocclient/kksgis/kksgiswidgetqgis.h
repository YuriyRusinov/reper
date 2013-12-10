#ifndef KKSGISWIDGETQGIS_H
#define KKSGISWIDGETQGIS_H

#include "kksgis_config.h"
#include "kksgiswidget.h"

//QT Includes
#include <QtGui>

#undef min(a,b)
#undef max(a,b)

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
class QgsMapLayerRegistry;
class QTableWidget;

class _GIS_EXPORT KKSGISWidgetQGIS : public KKSGISWidget
{
  Q_OBJECT;
  friend class KKSGISWidget;
public:
    QStatusBar * statusBar() const;
    const QString & pluginsDir();
    QTableWidget * tableLegend();
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

private:
    KKSGISWidgetQGIS(QWidget* parent = 0, Qt::WFlags fl = 0 );
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
    void addLayerToTOC(QgsMapLayer *mapLayer);


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

private:
    void initStatusBar();
    void initUserSettings();
    void initMapCanvas();
    void initLegend();
    void initActions();
    void initConnections();
    void initToolBar();
    void initMenuBar();
    void initMapTools();

    void azSetTitleWindow(QWidget & azApp);


private:
    QStringList azWorkList; // working list of anyone
    QgsMapLayer * mpSelectedLayer; //selected map layer

    // Setup's for statusBar
    QLabel * mpCoordsLabel;
    QLineEdit * mpCoordsEdit;
    QValidator * mpCoordsEditValidator;
    QgsPoint mpLastMapPosition;
    QStatusBar * mpStatusBar;

    KKSBadLayerHandler * m_badLayerHandler; //обработчик слоев, которые по тем или иным причинам не загрузились из файла проекта


    //QgsRasterLayer *testLayer;
    QgsMapLayerRegistry *mpRegistry;
    QgsMapCanvas * mpMapCanvas;
    QgsLegend * mpMapLegend;
    QList<QgsMapCanvasLayer> mpLayerSet;

    QToolBar * mpMapToolBar;

    QgsMapTool * mpPanTool;
    QgsMapTool * mpZoomInTool;
    QgsMapTool * mpZoomOutTool;


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
    
    QTableWidget * mpTableLegend;

    QString mpAppPath;
    QString m_pluginsDir;

    // dn's
    DNSpecBath *dnThemTaskSpecBath;

    // functions
    long azGetEPSG(const QString rastrPath);
};


#endif
