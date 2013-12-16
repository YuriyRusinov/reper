
// OSGEO includes
#include <ogr_api.h>
#include <gdal_priv.h>

#include "kksgiswidgetqgis.h"
#include "qgslegend.h"

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
#include <qgspallabeling.h>
#include <qgslabelinggui.h>
#include <qgsmessagebar.h>
#include <qgsscalecombobox.h>   
#include <qgsmessagebaritem.h>
#include <qgsvectorlayerproperties.h>
#include <qgsrasterlayerproperties.h>
#include <qgspluginlayer.h>
#include <qgspluginlayerregistry.h>


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

/*
   Здесь только методы, обеспечивающие работу пользователя с картой
   Конструктор, деструктор, инициализационные и обеспечивающие методы находятся в файле kksgiswidgethelper.cpp
*/

/*
void KKSGISWidgetQGIS::addLayerToTOC(QgsMapLayer* mapLayer)
{
//    mpLayerSet
//    mpRegistry
    int nRow(0);
    int nCol(0);
    nRow = mpTableLegend->rowCount();
    nCol = mpTableLegend->columnCount();
    mpTableLegend->insertRow(nRow);
    QTableWidgetItem *pItem = new QTableWidgetItem(mapLayer->name());
    mpTableLegend->setItem(nRow, 1, pItem);
    pItem = new QTableWidgetItem(mapLayer->id());
    mpTableLegend->setItem(nRow, 2, pItem);
    //QVariant *ok;
    Qt::CheckState pState;
    if (mapLayer->hasScaleBasedVisibility())
    {
        pState = Qt::Checked;
    }
    else
    {
        pState = Qt::Unchecked;
    }

    pItem->setData(Qt::CheckStateRole, pState);
    mpTableLegend->setItem(nRow, 0, pItem);
    //mpMapLegend->addLayer(mapLayer);
//    QStandardItemModel l1;

}
*/

void KKSGISWidgetQGIS::azRemoveAnnotationItems()
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

void KKSGISWidgetQGIS::azRemoveAllLayers()
{
    QgsMapLayerRegistry::instance()->removeAllMapLayers();
}

bool KKSGISWidgetQGIS::azSelectLayer(const QString layerName)
{
    bool bComplete(false);
    // создадим резервную копию рабочего слоя
    QgsMapLayer * pRezLayer;
    pRezLayer = NULL;
    if (mpSelectedLayer == NULL) // если он есть
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
        if (!mpSelectedLayer->isValid() && pRezLayer->isValid()) // если mpSelectedLayer был замещен некорректным слоем
        {
            mpSelectedLayer = pRezLayer; // возвращаем значение слоя, который был
            bComplete = false; // сообщаем, что выбор слоя неудачен

        }
    }
    return bComplete;
}


bool KKSGISWidgetQGIS::azSelectLayer(const int layerNumber)
{
    bool bComplete(false);
    // создадим резервную копию рабочего слоя
    if (layerNumber > mpRegistry->count())
    {
        return bComplete;
    }

    QgsMapLayer * pRezLayer;
    pRezLayer = NULL;
    if (mpSelectedLayer == NULL) // если он есть
    {
        pRezLayer = mpSelectedLayer;
    }

    int n(0); // инициируем счетчик
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
        if (!mpSelectedLayer->isValid() && pRezLayer->isValid()) // если mpSelectedLayer был замещен некорректным слоем
        {
            mpSelectedLayer = pRezLayer; // возвращаем значение слоя, который был
            bComplete = false; // сообщаем, что выбор слоя неудачен

        }
    }
    return bComplete;
}

bool KKSGISWidgetQGIS::azRasterEnhancement(QgsRasterLayer & azRasterLayer)
{
    // функция улучшения изображения
    // цель: улучшить вид отображения снимка для человеческого восприятия
    // (по идее должна определять тип снимка автоматически и
    // подстраивать соответветствующие параметры)
    bool bComplete(false); // инициализируем пременную для сигнализации об
                          //    успешном улучшении

    /*
      --------------------
// способы отображения снимка:
    pLayer->setDrawingStyle:
UndefinedDrawingStyle
SingleBandGray
SingleBandPseudoColor
PalettedColor
PalettedSingleBandGray
PalettedSingleBandPseudoColor
PalettedMultiBandColor          - три цвета
MultiBandSingleGandGray
MultiBandSingleBandGray
MultiBandSingleBandPseudoColor
MultiBandColor
SingleBandColorDataStyle
---------------------------
// алгоритм цветопередачи:
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
//        azRasterLayer.setDrawingStyle(QgsRaster::MultiBandColor); // устанавливаем "3-х цветное изображение"

        // пока делаю так: 3-х цветное изображение и каналы:
        // 4 - красный; 3 - зеленый; 2 - синий.
        if (azRasterLayer.bandCount() < 3)
        {
            // меньше 3-х каналов не улучшаем
        }
        else if (azRasterLayer.bandCount() == 3)
        {
            // по умолчанию если три то пусть идут в обратном

            //ksa -- azRasterLayer.setRedBandName(azRasterLayer.bandName(3));
            //ksa -- azRasterLayer.setGreenBandName(azRasterLayer.bandName(2));
            //ksa -- azRasterLayer.setBlueBandName(azRasterLayer.bandName(1));
            bComplete = true;
        }
        else
        {
//            pRast.BlueBand
                // проверяем название каждого канала
            if (azRasterCheckBandName(azRasterLayer, "Band 4"))
            {

//                azRasterLayer.set("Band 4"); // есть - добавляем
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

//        azRasterLayer.setStandardDeviations(2.5); // для рястяжки по гистограмме яркости
                                                           // исп. среднее квадратичное отклонение 2.5
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

bool KKSGISWidgetQGIS::azRasterCheckBandName(QgsRasterLayer &azRasterLayer, QString strBandName)
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

bool KKSGISWidgetQGIS::azCopyFiles(QString azSource, QString azDestPath, bool bUse)
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
bool KKSGISWidgetQGIS::azMakeLayer(QGis::WkbType azType, QString pDestDir, QString pName)
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

//            QDebug("Файл не скопирован в" + pDestDir.toAscii() + pName.toAscii() + pExp.toAscii());
            bComplete = false;
        }
    }
    return bComplete;
}
*/

bool KKSGISWidgetQGIS::azAddLayerVector(QFileInfo pFile)
{
    QString myProviderName      = "ogr";
    QgsVectorLayer * mypLayer = new QgsVectorLayer(pFile.filePath(), pFile.completeBaseName(), myProviderName);
    QgsSymbolV2 * s = QgsSymbolV2::defaultSymbol(mypLayer->geometryType());
    QgsSingleSymbolRendererV2 *mypRenderer = new QgsSingleSymbolRendererV2(s);//(mypLayer->geometryType());
    //        QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(mypLayer->geometryType());
    mypLayer->setRendererV2(mypRenderer);
    if (!mypLayer->isValid())
    {

//        QDebug("Слой некорректный '" + pFile.filePath().toAscii() + "'");
        return false;
    }

    // Add the Vector Layer to the Layer Registry
    mpRegistry->addMapLayer(mypLayer, TRUE);
    // Add the Layer to the Layer Set
    mpLayerSet.append(QgsMapCanvasLayer(mypLayer));
    // set the canvas to the extent of our layer
    mpMapCanvas->setExtent(mypLayer->extent());
    // Set the Map Canvas Layer Set
    mpMapCanvas->setLayerSet(mpLayerSet);
    mpMapCanvas->refresh();
    return true;
}

void KKSGISWidgetQGIS::SLOTmpActionVectorize()
{
    this->azVectorize();
    this->azAddWorkListToMap(azWorkList);

}

void KKSGISWidgetQGIS::azAddWorkListToMap(QStringList &pList)
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
                pMessage = "Добавлен слой '" + pFile.baseName() + "'";
                mpStatusBar->showMessage(pMessage);
            }
        }
    }
    if (i > 1)
    {
        pMessage = "Добавлено " + QString::number(i )+ "слоя(ев)";
        mpStatusBar->showMessage(pMessage);
    }
}

void KKSGISWidgetQGIS::azVectorize()
{
    dnThemTaskSpecBath->close();
    QString pMessage(""); // сообщение в статус баре о результате векторизации
    if (this->dnThemTaskSpecBath->Polygons.count() < 1)
    {
        pMessage = tr("Выбранные объекты не содержат информации для векторизации");
        mpStatusBar->showMessage(pMessage);
        return;
    }

    QString mEncoding; // кодировка
    mEncoding = "UTF-8";

    QgsFields mFields; // набор полей
    QgsField myField1("value", QVariant::Double, "Double", 0, 0);
    QgsField myField2( "comment", QVariant::String, "String", 10, 0, "Comment" );
    mFields.append( myField1 );
    mFields.append( myField2 );
    QgsCoordinateReferenceSystem pSRS;

    // создаем систему координат идентичную растру
    pSRS.createFromOgcWmsCrs("EPSG:" +
                             QString::number(azGetEPSG(dnThemTaskSpecBath->Polygons.at(0).EPSG)));
    pSRS.validate();
    QString myFileName (dnThemTaskSpecBath->Polygons.at(0).NameLayer +
                        QString::number(QTime::currentTime().hour())  + "-" +
                        QString::number(QTime::currentTime().minute())  + "-" +
                        QString::number(QTime::currentTime().second()) + "-" +
                        QString::number(QTime::currentTime().msec()) + ".shp");
    QgsVectorFileWriter myWriter( myFileName, mEncoding, mFields, QGis::WKBPolygon, &pSRS);
    azWorkList.clear();
    azWorkList.append(myFileName);

    for (int i = 0; i < dnThemTaskSpecBath->Polygons.size(); i++)
    {
        DNVector dnVec;
        dnVec = dnThemTaskSpecBath->Polygons.at(i);
        QgsPolyline pPolyLine;
        QgsPoint pFirstPoint (dnVec.GPt.at(0).x, dnVec.GPt.at(0).y);
        for (int j = 0; j < dnVec.GPt.size(); j++)
        {
            QgsPoint p(dnVec.GPt.at(j).x, dnVec.GPt.at(j).y);
            pPolyLine << p;
        }
        pPolyLine << pFirstPoint;
        QgsPolygon pPolygon;
        pPolygon << pPolyLine;
        QgsGeometry * pPolygonGeometry = QgsGeometry::fromPolygon( pPolygon );
        QgsFeature pFeature;
//        pFeature.setTypeName( "WKBPolygon" );
        pFeature.setGeometry( pPolygonGeometry );
        pFeature.setAttribute("comment", "deep" );
        pFeature.setAttribute("val",(double)dnVec.Vol);

        QgsVectorFileWriter::WriterError mError;
        myWriter.addFeature( pFeature );
        mError = myWriter.hasError();
        if (mError != 0)
        {
            qWarning() << myWriter.errorMessage();
        }
    }
    return;
}


void KKSGISWidgetQGIS::panMode()
{
  mpMapCanvas->setMapTool(mpPanTool);
}

void KKSGISWidgetQGIS::zoomInMode()
{
  mpMapCanvas->setMapTool(mpZoomInTool);
}

void KKSGISWidgetQGIS::zoomOutMode()
{
  mpMapCanvas->setMapTool(mpZoomOutTool);
}

void KKSGISWidgetQGIS::azLoadLayer(QgsMapLayer *theMapLayer, bool isExtent)
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
void KKSGISWidgetQGIS::SLOTmpActionFileExit() // Exit from Application
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
                                              "Выход из приложения",
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

void KKSGISWidgetQGIS::SLOTmpCloseProject()
{
    // unload the project macros before changing anything
    //if ( mTrustedMacros ){
    //    QgsPythonRunner::run( "qgis.utils.unloadProjectMacros();" );
    //}

    // remove any message widgets from the message bar
    //mInfoBar->clearWidgets();

    //mTrustedMacros = false;

    //deletePrintComposers();
    this->azRemoveAnnotationItems();
    // clear out any stuff from project
    mpMapCanvas->freeze( true );
    azRemoveAllLayers();
    mpLayerSet.clear();
    mpMapCanvas->freeze( false);
    mpMapCanvas->refresh();
    mpMapCanvas->freeze( true );
}

void KKSGISWidgetQGIS::SLOTmpActionFileOpenProject() // Open QGIS Project in MapDock
{
    QString projectFileName = QFileDialog::getOpenFileName(this, "Открыть файл проекта карты *.qgs", "", "*.qgs");
    if (projectFileName.isNull())
        return;
    
    openProject(projectFileName);
}

void KKSGISWidgetQGIS::SLOTsetRenderer()
{
//    QgsMapLayer* layer = mpMapCanvas->layer(0);
//    Q_ASSERT(layer);
//    Q_ASSERT(layer->type() == QgsMapLayer::VectorLayer);
//    QgsVectorLayer* vlayer = static_cast<QgsVectorLayer*>(layer);
//    QgsRendererV2PropertiesDialog dlg(vlayer, QgsStyleV2::defaultStyle());
//    dlg.exec();
    mpMapCanvas->refresh();
}

void KKSGISWidgetQGIS::SLOTtempUse()
{
//    QString pFilePath("D:/!Share/rastrs/union_den.tif");
//    QMessageBox::about(this, "hello world!", QString::number(azGetEPSG(pFilePath)));
}

long KKSGISWidgetQGIS::azGetEPSG(const QString rastrPath)
{
    QFileInfo pFileInfo(rastrPath);
    if (!pFileInfo.isFile()) // проверка существования файла
    {
        return -1;
    }
    QgsRasterLayer * pRLayer = new QgsRasterLayer(pFileInfo.filePath(), pFileInfo.completeBaseName());
    return pRLayer->crs().srsid();
}


void KKSGISWidgetQGIS::SLOTmpActionAddVectorLayer()
{
    QString fullLayerName = QFileDialog::getOpenFileName(this, "Добавить векторный слой",
                            "", "Все поддерживаемые форматы(*.shp);;Shapefiles (*.shp)");
    
    openVectorLayer(fullLayerName);
}

void KKSGISWidgetQGIS::SLOTmpActionAddPostGISLayer()
{
    openDatabaseLayer();
}

void KKSGISWidgetQGIS::SLOTmpActionAddRasterLayer()
{
    // СЛОТ: добавление растра в окно карты
    // сначала вызываем диалог, получаем путь к файлу
    QString fullLayerName = QFileDialog::getOpenFileName(this, "Добавить растровый слой", "",
                             "Все поддерживаемые растровые форматы (*.img *.asc *.tif *tiff *.bmp *.jpg *.jpeg);;Geotiff (*.tif *.tiff)");
    
    openRasterLayer(fullLayerName);

}

void KKSGISWidgetQGIS::SLOTazShowMouseCoordinate(const QgsPoint & p )
{
      if ( mpLayerSet.count() < 1 ) return;
      if (!mpMapCanvas->underMouse()) return;
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


void KKSGISWidgetQGIS::SLOTazShowContextMenuForLegend(const QPoint & pos)
{
//    mpContextLegendMenu->exec(mpTOC->viewport()->mapToGlobal(pos));
//    QMessageBox::about(mpTOC, "", "OK!");
}

void KKSGISWidgetQGIS::SLOTazContextShowExtent()
{
//    QgsMapLayer * pLayer = mpMapCanvas->layer(0);
//    mpMapCanvas->setExtent(pLayer->extent());
//    mpMapCanvas->refresh();
}

void KKSGISWidgetQGIS::SLOTazGetSelectedLegendItem()
{
//    QModelIndexList pList;
//    QModelIndex pIndex;
//    QTableWidgetItem *pItem = new QTableWidgetItem();
////    pList = tableLegend->selectedIndexes();
//    if (pList.count()<1) return;
    //    qDebug("all ok");
}

void KKSGISWidgetQGIS::SLOTazThemTaskSpectralBathynometry()
{
    //ksa -- if (dnThemTaskSpecBath == NULL);
    if (dnThemTaskSpecBath == NULL)
    {
        dnThemTaskSpecBath = new DNSpecBath(this);
        connect(this->dnThemTaskSpecBath, SIGNAL(SIGNALcreateVector()), this, SLOT(SLOTmpActionVectorize()));
    }

    dnThemTaskSpecBath->show();
//    QMessageBox::about(0, "a", "a");
//    QFileInfo pFile("D:/!Share/layers/baba.shp");
//    this->azAddLayerVector(pFile);

}

void KKSGISWidgetQGIS::SLOTazCoordsCenter()
{
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

void KKSGISWidgetQGIS::SLOTmpActionFileSaveProjectAs()
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

void KKSGISWidgetQGIS::SLOTmpActionFileSaveProject()
{
    saveProject();
}


void KKSGISWidgetQGIS::openProject(const QString & prjFile)
{
    SLOTmpCloseProject();

    QgsProject::instance()->setFileName(prjFile);
    if ( !QgsProject::instance()->read() )
    {
        QApplication::restoreOverrideCursor();
        mpStatusBar->showMessage("Ошибка открытия файла проекта!"
                                       "\n " + prjFile + "'");
        qDebug("Ошибка открытия файла проекта!" + prjFile.toAscii());

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
        mScaleEdit->updateScales( QgsProject::instance()->readListEntry( "Scales", "/ScalesList" ) );
    }

    mpMapCanvas->updateScale();

  // k // QgsMapLayer *layer;
    QMapIterator < QString, QgsMapLayer * > i(QgsMapLayerRegistry::instance()->mapLayers());
    i.toBack();
    while (i.hasPrevious())
    {
        i.previous();
        if (i.value()->type() == QgsMapLayer::VectorLayer)
                {
                    if (i.value()->isValid())
                    {
                        mpRegistry->addMapLayer(i.value(), TRUE);
                        mpLayerSet.append(QgsMapCanvasLayer(i.value()));
                    }

                }
        else if (i.value()->type() == QgsMapLayer::RasterLayer)
        {
            mpRegistry->addMapLayer(i.value(), TRUE);
            mpLayerSet.append(QgsMapCanvasLayer(i.value()));
        }

    }
    
    QSettings settings;
    // load PAL engine settings
    mLBL->loadEngineSettings();

    emit projectRead(); // let plug-ins know that we've read in a new
    // project so that they can check any project
    // specific plug-in state
    
    // TODO add last Open Project
    QApplication::restoreOverrideCursor();
    mpMapCanvas->setLayerSet(mpLayerSet);
    mpMapCanvas->freeze(false);
    mpMapCanvas->refresh();
}

void KKSGISWidgetQGIS::closeProject()
{

}

void KKSGISWidgetQGIS::saveProjectAs(const QString & path)
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

void KKSGISWidgetQGIS::saveProject()
{
    if ( mpMapCanvas && mpMapCanvas->isDrawing() ){
        return;
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
            return;

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
    }
}

void KKSGISWidgetQGIS::openVectorLayer(const QString & vLayerFile)
{
    QFileInfo azFileInfo(vLayerFile);
    if (!azFileInfo.isFile())
        return;

    this->azAddLayerVector(azFileInfo);
}

void KKSGISWidgetQGIS::openRasterLayer(const QString & rLayerFile)
{
    QFileInfo azFileInfo(rLayerFile);
    if (!azFileInfo.isFile()) // проверка существования файла
        return;

    // создаем экземпляр класса QgsRasterLayer
    // он содержит всю инфу о растре
    QgsRasterLayer * mypLayer = new QgsRasterLayer(azFileInfo.filePath(), azFileInfo.completeBaseName());
    if (!mypLayer->isValid()) // проверяем его корректность
    {
        qDebug("Raster layer is not valid or not supported by GDAL. Layer was not added to map." + rLayerFile.toAscii());
        mpStatusBar->showMessage("Файл растра не явялется корректным или не поддерживается библиотекой GDAL. Слой не был добавлен на карту."
                                       "\n " + rLayerFile + "'");
        return;
    }

    if (!this->azRasterEnhancement(*mypLayer)) // применяем улучшение снимка
        qDebug("Улучшение растра не использовано. Нераспознаный формат растра." + rLayerFile.toAscii());

    // применяем улучшение контраста (т.е. цвета отображаются от минимума до максимума)
    mypLayer->setContrastEnhancement(QgsContrastEnhancement::StretchToMinimumMaximum);

    // самообновление
    connect(mypLayer, SIGNAL(repaintRequested()), mpMapCanvas, SLOT(refresh()) );

    // Add the Vector Layer to the Layer Registry
    mpRegistry->addMapLayer(mypLayer, TRUE);
    // Add the Layer to the Layer Set
    mpLayerSet.append(QgsMapCanvasLayer(mypLayer));
    mpMapCanvas->setExtent(mypLayer->extent());
    mpMapCanvas->setLayerSet(mpLayerSet);
    mpMapCanvas->refresh();
/*    QgsMapLayer *pLayer;
    pLayer = mypLayer;
    QgsCoordinateReferenceSystem pSRS;
    pSRS = mypLayer->*/
}

void KKSGISWidgetQGIS::openDatabaseLayer()
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
  
    dbs->exec();
    delete dbs;
}

QString KKSGISWidgetQGIS::projectFileName()
{
    return QgsProject::instance()->fileName();
}

void KKSGISWidgetQGIS::layerProperties()
{
  if ( mpMapCanvas && mpMapCanvas->isDrawing() )
    return;

  showLayerProperties( activeLayer() );
}

/** Get a pointer to the currently selected map layer */
QgsMapLayer *KKSGISWidgetQGIS::activeLayer()
{
  return mpMapLegend ? mpMapLegend->currentLayer() : 0;
}

/** set the current layer */
bool KKSGISWidgetQGIS::setActiveLayer( QgsMapLayer *layer )
{
  if ( !layer )
    return false;

  return mpMapLegend->setCurrentLayer( layer );
}

void KKSGISWidgetQGIS::showLayerProperties( QgsMapLayer *ml )
{
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
    }

    rlp->exec();
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
    }

    if ( vlp->exec() )
    {
      activateDeactivateLayerRelatedActions( ml );
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

QgsPalLabeling *KKSGISWidgetQGIS::palLabeling()
{
  Q_ASSERT( mLBL );
  return mLBL;
}

QgsMessageBar* KKSGISWidgetQGIS::messageBar()
{
  Q_ASSERT( mInfoBar );
  return mInfoBar;
}

int KKSGISWidgetQGIS::messageTimeout()
{
  QSettings settings;
  return settings.value( "/qgis/messageTimeout", 5 ).toInt();
}

void KKSGISWidgetQGIS::labeling()
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

void KKSGISWidgetQGIS::markDirty()
{
  // notify the project that there was a change
  QgsProject::instance()->dirty( true );
}


void KKSGISWidgetQGIS::toggleEditing()
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

bool KKSGISWidgetQGIS::toggleEditing( QgsMapLayer *layer, bool allowCancel )
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
                                     QgsMessageBar::CRITICAL );
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
