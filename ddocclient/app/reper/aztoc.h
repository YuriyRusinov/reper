#ifndef AZTOC_H
#define AZTOC_H

#include "config.h"

#include <ogr_api.h>
#include <gdal_priv.h>


#include <QTreeWidget>
#include <QPair>
#include <QMenu>
#include <QMouseEvent>


//QGis Includes

#include <qgsapplication.h>
#include <qgsfield.h>
#include <qgsmapcanvas.h>
#include <qgsmaplayerregistry.h>
#include <qgsproviderregistry.h>
#include <qgsrasterlayer.h>
#include <qgsproject.h>
//#include <qgssinglesymbolrenderer.h>
#include <qgsvectorlayer.h>
#include <qgsvectorfilewriter.h>
#include <qgsapplication.h>
#include <QMessageBox>


//#include "qgsapplication.h"
//#include "qgisapp.h"
//#include <qgslogger.h>
//#include "qgslegend.h"
//#include "qgslegendgroup.h"
#include <qgslegendlayer.h>
//#include "qgslegendpropertygroup.h"
//#include "qgslegendsymbologyitem.h"
//#include <qgsmapcanvas.h>
//#include <qgsmapcanvasmap.h>
//#include <qgsmaplayer.h>
//#include <qgsmaplayerregistry.h>
//#include <qgsmaprenderer.h>
//#include <qgsproject.h>
//#include <qgsrasterlayer.h>
//#include <qgsvectorlayer.h>
//#include <qgsgenericprojectionselector.h>
//#include <qgsclipboard.h>




class QTreeWidgetItem;
class QgsMapLayer;
class QgsMapCanvas;
class QgsCoordinateReferenceSystem;


class AzTOC : public QTreeWidget

{
    Q_OBJECT

public:
    AzTOC(QgsMapCanvas *canvas, QWidget *parent = 0);
    QgsMapCanvas *canvas() { return mpMapCanvas; }
    ~AzTOC();

private:
  // Moved here to match access of declaration later in file.
  // Previous location raised a warning in msvc as the forward
  // declaration was public while the definition was private
  class QgsLegendPixmaps;
  QgsMapCanvas * mpMapCanvas;


public slots:
  void SLOTlegendLayerZoom();

private slots:
  void handleRightClickEvent( QTreeWidgetItem* item, const QPoint& position );
  void SLOThandleItemChange( QTreeWidgetItem* item, int row );
  void SLOThandleCurrentItemChanged( QTreeWidgetItem* current, QTreeWidgetItem* previous );
  void SLOTaddLayers (QList<QgsMapLayer *> azLayerList);

protected:
  bool mpMousePressed;
  QPoint mpLastPressPos;
  enum { BEFORE, INSERT, AFTER } mpDropAction;

  void mouseMoveEvent( QMouseEvent * tEvent );
  void mousePressEvent( QMouseEvent * tEvent );
  void mouseReleaseEvent( QMouseEvent * tEvent );
  void mouseDoubleClickEvent( QMouseEvent* tEvent );
};

#endif // AZTOC_H
