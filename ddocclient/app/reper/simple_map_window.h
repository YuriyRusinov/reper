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
#ifndef OMGMAINWINDOW_H
#define OMGMAINWINDOW_H


#include "config.h"

// OSGEO includes
#include <ogr_api.h>
#include <gdal_priv.h>

//QGis includes
#include <qgsapplication.h>
#include <qgsfield.h>
#include <qgsfeature.h>
#include <qgsgeometry.h>
#include <qgsmapcanvas.h>
#include <qgsmaptool.h>
#include <qgslabel.h>
#include <qgslabelattributes.h>
#include <qgslegendmodel.h>
#include <qgslegendinterface.h>
#include <qgsmaplayerregistry.h>

#include <qgsproject.h>
#include <qgsproviderregistry.h>
#include <qgsvectordataprovider.h>
#include <qgsrasterlayer.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgsstylev2.h>
#include <qgsvectorlayer.h>
#include <qgsvectorfilewriter.h>
#include <qgsmaptoolpan.h>
#include <qgsmaptoolzoom.h>
#include <qgscoordinatereferencesystem.h>

//#include "aztoc.h"


//QT Includes
#include <QtGui>
#include <QMessageBox>
#include <QFile>
#include <QPainter>
#include <QApplication>

//Local Includes
#include <ui_simple_map_window_base.h>

//Denis Includes (dn)
#include "dn/dnspecbath.h"
#include "dn/Added/dnvector.h"

class MainWindow : public QMainWindow, private Ui::SimpleMapWindowBase
{
  Q_OBJECT;
public:

    MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~MainWindow();


    QgsMapLayer * mpSelectedLayer; //selected map layer

public slots:
    void zoomInMode();
    void zoomOutMode();
    void panMode();
    void azLoadLayer(QgsMapLayer *theMapLayer, bool isExtent = false);
    void addLayerToTOC(QgsMapLayer *mapLayer);
    void azRemoveAnnotationItems();
    void azRemoveAllLayers();
    bool azSelectLayer(const QString layerName);
    bool azSelectLayer(const int layerNumber);
    static void azSetTitleWindow(QWidget & azApp);
    static bool azRasterEnhancement(QgsRasterLayer & azRasterLayer);
    static bool azRasterCheckBandName(QgsRasterLayer & azRasterLayer, QString strBandName);
    static bool azCopyFiles(QString azSource, QString azDestPath, bool bUse = false);
    bool azMakeLayer(QGis::WkbType azType, QString pDestDir, QString pName);
    bool azAddLayerVector(QFileInfo pFile);



private slots:
    void SLOTazContextShowExtent();
    void SLOTazCoordsCenter();
    void SLOTazGetSelectedLegendItem();
    void SLOTazThemTaskSpectralBathynometry();
    void SLOTazShowContextMenuForLegend(const QPoint & pos);
    void SLOTazShowMouseCoordinate(const QgsPoint & p);
    void SLOTmpActionFileExit();
    void SLOTmpActionFileOpenProject();
    void SLOTmpActionAddVectorLayer();
    void SLOTmpActionAddRasterLayer();
    void SLOTmpActionVectorize();
    void SLOTsetRenderer();
    void SLOTtempUse();


private:
    // Setup's for statusBar
    QLabel * mpCoordsLabel;
    QLineEdit * mpCoordsEdit;
    QValidator * mpCoordsEditValidator;
    QgsPoint mpLastMapPosition;


    QgsRasterLayer *testLayer;
    QgsMapCanvas * mpMapCanvas;
//    QVBoxLayout  * mpLayout;
    QVBoxLayout  * mpMapLayout;
    QVBoxLayout  * mpLegendLayout;
    QToolBar * mpMapToolBar;
    QDockWidget * mpLeftDock;
    QDockWidget * mpBottomDock;
    QgsMapTool * mpPanTool;
    QgsMapTool * mpZoomInTool;
    QgsMapTool * mpZoomOutTool;

//    QgsLegendModel * mpLegendModel;
//    AzTOC * mpTOC;

    QgsLegend * mpMapLegend;
    QgsMapLayerRegistry *mpRegistry;
    QList<QgsMapCanvasLayer> mpLayerSet;
    QMenu *mpContextLegendMenu;
    // Actions
    QAction *mpVectorize;
    QAction *mpActionAddVectorLayer;
    QAction *mpActionAddRasterLayer;
    QAction *mpContextShowExtent;
    QAction *mpContextRemoveLayer;
    QAction *mpActionFileExit;
    QString mpAppPath;

    // dn's
    DNSpecBath *dnThemTaskSpecBath;

    // functions
    long azGetEPSG(const QString rastrPath);

protected:
//    void paintEvent(QPaintEvent *event);

};

#endif
