#include "aztoc.h"

AzTOC::AzTOC(QgsMapCanvas *canvas, QWidget *parent) : QTreeWidget(parent)
{
    mpMousePressed = false;
    mpMapCanvas = canvas;
    this->setHeaderLabels(QStringList() << tr("") << tr("Layer's name") << tr("Style") << tr("Uniq") << tr("Image"));
//    this->setColumnCount(this->count());
    this->setColumnWidth(0, 25);
    this->setColumnWidth(1, 185);
    this->setColumnWidth(2, 50);

    //connections
    connect( QgsMapLayerRegistry::instance(),
             SIGNAL( layersAdded( QList<QgsMapLayer*> ) ),
             this, SLOT(SLOTaddLayers(QList<QgsMapLayer*>)));

    connect( this, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ),
             this, SLOT( handleItemChange( QTreeWidgetItem*, int ) ) );

    connect( this, SIGNAL( currentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ),
             this, SLOT( handleCurrentItemChanged( QTreeWidgetItem*, QTreeWidgetItem* ) ) );


    //    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
//                SLOT(showContextMenu(const QPoint&)));
}

AzTOC::~AzTOC()
{
    delete mpMapCanvas;
}

void AzTOC::SLOTlegendLayerZoom()
{
//    if ( !mpMapCanvas )
//    {
//      return;
//    }

//    QgsRectangle extent;

//    QgsLegendItem* li = dynamic_cast<QgsLegendItem *>( currentItem() );

//    if ( !li )
//      return;

//    if ( li->type() == QgsLegendItem::LEGEND_LAYER )
//    {
//      QgsLegendLayer* currentLayer = dynamic_cast<QgsLegendLayer *>( currentItem() );
//      if ( !currentLayer )
//        return;

//      QgsMapLayer* theLayer = currentLayer->layer();
//      extent = theLayer->extent();

      //transform extent if otf-projection is on
//      if ( mMapCanvas->hasCrsTransformEnabled() )
//      {
//        QgsMapRenderer* renderer = mMapCanvas->mapRenderer();
//        if ( renderer )
//        {
//          extent = renderer->layerExtentToOutputExtent( theLayer, extent );
//        }
//      }
//    }
//    else if ( li->type() == QgsLegendItem::LEGEND_GROUP )
//    {
//      QgsLegendGroup* currentGroup = dynamic_cast<QgsLegendGroup *>( currentItem() );

//      QgsRectangle layerExtent;

//      QList<QgsLegendLayer*> layers = currentGroup->legendLayers();
//      for ( int i = 0; i < layers.size(); ++i )
//      {
//        QgsMapLayer* theLayer = layers.at( i )->layer();
//        layerExtent = theLayer->extent();

//        //transform extent if otf-projection is on
//        if ( mMapCanvas->hasCrsTransformEnabled() )
//        {
//          QgsMapRenderer* renderer = mMapCanvas->mapRenderer();
//          if ( renderer )
//          {
//            layerExtent = renderer->layerExtentToOutputExtent( theLayer, layerExtent );
//          }
//        }

//        if ( i == 0 )
//        {
//          extent = layerExtent;
//        }
//        else
//        {
//          extent.combineExtentWith( &layerExtent );
//        }
//      }
//    }

//    if ( extent.isEmpty() )
//    {
//      return;
//    }

//    // Increase bounding box with 5%, so that layer is a bit inside the borders
//    extent.scale( 1.05 );

//    //zoom to bounding box
//    mpMapCanvas->setExtent( extent );
    mpMapCanvas->refresh();
}

void AzTOC::handleRightClickEvent(QTreeWidgetItem *item, const QPoint &position)
{
    if ( !mpMapCanvas || mpMapCanvas->isDrawing() )
    {
      return;
    }




    QMenu theMenu( tr( "Legend context" ), this );
    theMenu.addAction(tr( "Show extent" ), this, SLOT(SLOTlegendLayerZoom()) );
//    QgsLegendItem* li = dynamic_cast<QgsLegendItem *>( item );
//    if ( li )
//    {
//      if ( li->type() == QgsLegendItem::LEGEND_LAYER )
//      {
//        qobject_cast<QgsLegendLayer*>( li )->addToPopupMenu( theMenu );

//        if ( li->parent() && !parentGroupEmbedded( li ) )
//        {
//          theMenu.addAction( tr( "&Make to Toplevel Item" ), this, SLOT( makeToTopLevelItem() ) );
//        }
//      }
//      else if ( li->type() == QgsLegendItem::LEGEND_GROUP )
//      {
//        theMenu.addAction( QgisApp::getThemeIcon( "/mActionZoomToLayer.png" ),
//                           tr( "Zoom to Group" ), this, SLOT( legendLayerZoom() ) );

//        // use QGisApp::removeLayer() to remove all selected layers+groups
//        theMenu.addAction( QgisApp::getThemeIcon( "/mActionRemoveLayer.png" ), tr( "&Remove" ), QgisApp::instance(), SLOT( removeLayer() ) );

//        theMenu.addAction( QgisApp::getThemeIcon( "/mActionSetCRS.png" ),
//                           tr( "&Set Group CRS" ), this, SLOT( legendGroupSetCRS() ) );
//      }

//      if (( li->type() == QgsLegendItem::LEGEND_LAYER || li->type() == QgsLegendItem::LEGEND_GROUP ) && !groupEmbedded( li ) && !parentGroupEmbedded( li ) )
//      {
//        theMenu.addAction( tr( "Re&name" ), this, SLOT( openEditor() ) );
//      }

//      //
//      // Option to group layers, if the selection is more than one
//      //
//      if ( selectedLayers().length() > 1 )
//      {
//        theMenu.addAction( tr( "&Group Selected" ), this, SLOT( groupSelectedLayers() ) );
//      }
//      // ends here
//    }

//    if ( selectedLayers().length() == 1 )
//    {
//      QgisApp* app = QgisApp::instance();
//      theMenu.addAction( tr( "Copy Style" ), app, SLOT( copyStyle() ) );
//      if ( app->clipboard()->hasFormat( QGSCLIPBOARD_STYLE_MIME ) )
//      {
//        theMenu.addAction( tr( "Paste Style" ), app, SLOT( pasteStyle() ) );
//      }
//    }

//    theMenu.addAction( QgisApp::getThemeIcon( "/folder_new.png" ), tr( "&Add New Group" ), this, SLOT( addGroupToCurrentItem() ) );
//    theMenu.addAction( QgisApp::getThemeIcon( "/mActionExpandTree.png" ), tr( "&Expand All" ), this, SLOT( expandAll() ) );
//    theMenu.addAction( QgisApp::getThemeIcon( "/mActionCollapseTree.png" ), tr( "&Collapse All" ), this, SLOT( collapseAll() ) );

//    QAction *updateDrawingOrderAction = theMenu.addAction( QgisApp::getThemeIcon( "/mUpdateDrawingOrder.png" ), tr( "&Update Drawing Order" ), this, SLOT( toggleDrawingOrderUpdate() ) );
//    updateDrawingOrderAction->setCheckable( true );
//    updateDrawingOrderAction->setChecked( mUpdateDrawingOrder );

    theMenu.exec( position );
    QMessageBox::about(this, "", "yes!");
}

void AzTOC::SLOThandleItemChange(QTreeWidgetItem *item, int row)
{

}

void AzTOC::SLOThandleCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{

}
void AzTOC::SLOTaddLayers(QList<QgsMapLayer *> azLayerList)
{
    if ( !mpMapCanvas || mpMapCanvas->isDrawing() )
    {
      return;
    }

    QSettings settings;

    //Note if the canvas was previously blank so we can
    //zoom to all layers at the end if neeeded
//    bool myFirstLayerFlag = false;
//    if ( layers().count() < 1 )
//    {
//        myFirstLayerFlag = true;
//    }

    //iteratively add the layers to the canvas
//    for ( int i = 0; i < azLayerList.size(); ++i )
//    {
//        QgsMapLayer * layer = azLayerList.at( i );
//        QgsLegendLayer* llayer = new QgsLegendLayer( layer );
//        if ( !QgsProject::instance()->layerIsEmbedded( layer->id() ).isEmpty() )
//        {
//            QFont itemFont;
//            itemFont.setItalic( true );
//            llayer->setFont( 0, itemFont );
//        }

//      //set the correct check states
//      blockSignals( true );
//      llayer->setCheckState( 0, llayer->isVisible() ? Qt::Checked : Qt::Unchecked );
//      blockSignals( false );

//      QgsLegendGroup *lg = dynamic_cast<QgsLegendGroup *>( currentItem() );
//      if ( !lg && currentItem() )
//      {
//        lg = dynamic_cast<QgsLegendGroup *>( currentItem()->parent() );
//      }

//      int index = 0;
//      if ( lg )
//      {
//        index = lg->indexOfChild( currentItem() );
//      }
//      else
//      {
//        index = indexOfTopLevelItem( currentItem() );
//      }

//      if ( index < 0 )
//      {
//        index = 0;
//      }

//      if ( lg && settings.value( "/qgis/addNewLayersToCurrentGroup", false ).toBool() )
//      {
//        lg->insertChild( index, llayer );
//      }
//      else
//      {
//        insertTopLevelItem( index, llayer );
//        setCurrentItem( llayer );
//      }

//      setItemExpanded( llayer, true );
//      //don't expand raster items by default, there could be too many
//      refreshLayerSymbology( layer->id(), layer->type() != QgsMapLayer::RasterLayer );

//      updateMapCanvasLayerSet();
//      emit itemAdded( indexFromItem( llayer ) );
//    }
//    // first layer?
//    if ( myFirstLayerFlag )
//    {
//      QgsMapLayer * myFirstLayer = theLayerList.at( 0 );
//      if ( !mMapCanvas->mapRenderer()->hasCrsTransformEnabled() )
//      {
//        mMapCanvas->mapRenderer()->setDestinationCrs( myFirstLayer->crs() );
//        mMapCanvas->mapRenderer()->setMapUnits( myFirstLayer->crs().mapUnits() );
//      }
//      mMapCanvas->zoomToFullExtent();
//      mMapCanvas->clearExtentHistory();
//    }
//    //make the QTreeWidget item up-to-date
//    doItemsLayout();
}


void AzTOC::mouseMoveEvent(QMouseEvent *tEvent)
{
}

void AzTOC::mousePressEvent(QMouseEvent *tEvent)
{
}

void AzTOC::mouseReleaseEvent(QMouseEvent *tEvent)
{
}

void AzTOC::mouseDoubleClickEvent(QMouseEvent *tEvent)
{
}
