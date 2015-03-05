#ifndef KKSGISWIDGETBASE_H
#define KKSGISWIDGETBASE_H

#include "kksqgis_config.h"
#include "qgisapp.h"

#ifdef WIN32
//Denis Includes (dn)
class DNSpecBath;
#endif

class KKSBadLayerHandler;

class _QGIS_EXPORT KKSGISWidgetBase : public QgisApp
{
  Q_OBJECT
public:
    KKSGISWidgetBase(bool withSubWindows, bool withAddons = true, QWidget* parent = 0, Qt::WFlags fl = 0 );
    virtual ~KKSGISWidgetBase();

    //void initQGIS();
    bool initCorrectly() const {return m_bInit;}
    static QSplashScreen * initSplash();

    QDockWidget * mapLegendDockWidget(){ return mLayerTreeDock; }
    QDockWidget * mapLayerOrderDockWidget(){ return mLayerOrderDock; }

    void refreshMapCanvasOnly();
    bool saveProject();
    bool saveProjectAs(const QString & path = QString());

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

    //QWidget * mapLegendWidget();
    //QWidget * mapLayerOrderWidget();
    
    //ksa QToolBar * toolBar() const;
    //ksa QMenuBar * menuBar() const;

    const QMap<QString, QMenu *> & menuMap();
    const QMap<QString, QToolBar *> & toolBarMap();

    QString projectFileName();
    QString readLayerFilePath(const QString & file) const;//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS

    void reloadLayer(const QString & theLayerId);

public slots:
    void slotUpdateMapByNotify(const QString &, const QString &, const QString &);

signals:
    
    void dataChanged();
    void mapSaved(QDomDocument&);
    void SIGNALchangeWindowTitle();


private slots:
    //void azLoadLayer(QgsMapLayer *theMapLayer, bool isExtent = false);
    void azAddWorkListToMap(QStringList & pList);
    //void azRemoveAnnotationItems();
    //void azRemoveAllLayers();

    /**/bool azRasterEnhancement(QgsRasterLayer & azRasterLayer);
    /**/bool azRasterCheckBandName(QgsRasterLayer & azRasterLayer, QString strBandName);
    /**/bool azCopyFiles(QString azSource, QString azDestPath, bool bUse = false);

    /**/void azVectorize();

private slots:
    //void SLOTazContextShowExtent();
    //void SLOTazCoordsCenter();
    //void SLOTazGetSelectedLegendItem();
    void SLOTazThemTaskSpectralBathynometry();
    //void SLOTazShowContextMenuForLegend(const QPoint & pos);
    //void SLOTazShowMouseCoordinate(const QgsPoint & p);
    
    /**/void SLOTmpActionVectorize();
    
private:
    /*!!!*/ QStringList azWorkList; // working list of anyone
    //QgsMapLayer * mpSelectedLayer; //selected map layer

    //Qwidget * mpMapLegendWidget;
    //QWidget * mpMapLayerOrderWidget;


    QToolBar * mpMapToolBar; /**/
    QToolBar * mpDataSourceToolBar; /**/
    QToolBar * mpLayerToolBar; /**/
    QToolBar * mpToolsToolBar; /**/
    QToolBar * mpTaskToolBar; /**/
    QToolBar * mpLayerEditsToolBar; /**/ //редактирование векторных слоев
    QMap<QString, QToolBar *> mpToolBarMap; /**/

    //строка меню
    QMenuBar * mpMenuBar; /**/
    QMap<QString, QMenu *> mpMenuMap; /**/
    // различные операции
    QAction * mpVectorize; /**/
    QAction * mpActionBathymetry; /**/
    
    //QAction * mActionMapTips; /**/

    bool m_bWithSubwindows; /**/ //надо ли создавать дополнительные окна (меню, тулбар, статусбар и т.п.)
    bool m_bWithAddons; /**/ //надо ли создавать меню для тематической обработки (нет, если работаем в составе DynamicDocs)
    bool m_bInit; /**/ //если виджет был инициализирован (т.е. создан канвас, окна и т.п.), то true

#ifdef WIN32
    // dn's
    DNSpecBath *dnThemTaskSpecBath; /**/
#endif

    // functions
    long azGetEPSG(const QString rastrPath); /**/
};


#endif
