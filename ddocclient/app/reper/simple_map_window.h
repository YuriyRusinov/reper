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


#include <ogr_api.h>
#include <gdal_priv.h>
//QGis Includes

#include <qgsapplication.h>
#include <qgsfield.h>
#include <qgsmapcanvas.h>
#include <qgsmaptool.h>
#include <qgslabel.h>
#include <qgslabelattributes.h>
#include <qgslegendmodel.h>
#include <qgslegendinterface.h>
#include <qgsmaplayerregistry.h>
#include <qgsproject.h>
#include <qgsproviderregistry.h>
#include <qgsrasterlayer.h>
#include <qgssinglesymbolrendererv2.h>
//#include <qgssinglesymbolrenderer.h>
#include <qgsstylev2.h>
#include <qgsvectorlayer.h>
#include <qgsvectorfilewriter.h>


#include "qgsmaptoolpan.h"
#include "qgsmaptoolzoom.h"

//#include <qgslegend.h>

//#include <qgsrasterlayer.h>
//#include <qgsfield.h>
//#include <qgslabel.h>
//#include <qgslabelattributes.h>

// GDAL / ORG Includes


//QT Includes
#include <QtGui>
#include <QMessageBox>
#include <QPainter>


//Local Includes
#include <ui_simple_map_window_base.h>

/**
@author Tim Sutton
*/
class MainWindow : public QMainWindow, private Ui::SimpleMapWindowBase
{
  Q_OBJECT;
public:

    MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~MainWindow();
public slots:
    void zoomInMode();
    void zoomOutMode();
    void panMode();
    void azLoadLayer(QgsMapLayer *theMapLayer, bool isExtent = false);
    void addLayerToTOC(QgsMapLayer *mapLayer);
    void azRemoveAnnotationItems();
    void azRemoveAllLayers();
    static void azSetTitleWindow(QWidget & azApp);

private slots:
    void SLOTmpActionFileExit();
    void SLOTmpActionFileOpenProject();
    void SLOTsetRenderer();
    void SLOTmpActionAddVectorLayer();
    void SLOTmpActionAddRasterLayer();
    void SLOTmpActionVectorize();
    void SLOTazShowContextMenuForLegend(const QPoint & pos);

private:
    bool needPaint;
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

protected:
//    void paintEvent(QPaintEvent *event);

};

#endif
