#include "kksgiswidgetbase.h"

#include "kksbadlayerhandler.h"
#include <QSplashScreen>
#include <QBitmap>

#include "qgsmaplayerregistry.h"
#include "qgsvectorfilewriter.h"
#include "qgsgeometry.h"
#include "qgsvectordataprovider.h"
#include "qgsmapcanvas.h"


#ifdef WIN32
#include "dn/dnspecbath.h"
#include "dn/Added/dnvector.h"
#endif

KKSGISWidgetBase::KKSGISWidgetBase(bool withSubWindows, bool withAddons, QWidget* parent, Qt::WFlags fl)
    : QgisApp(KKSGISWidgetBase::initSplash(), true, parent, fl)

{
#ifdef WIN32    
    dnThemTaskSpecBath = NULL; //???
#endif

    azWorkList.clear();

    m_bWithSubwindows = withSubWindows; //надо ли создавать дополнительные окна (меню, тулбар, статусбыр и т.п.)
    m_bWithAddons = withAddons;//надо ли создавать меню для тематической обработки (нет, если работаем в составе DynamicDocs)
    m_bInit = false;

}

QSplashScreen * KKSGISWidgetBase::initSplash()
{
    QSettings mySettings;
  
    //ksa QString mySplashPath( QgsCustomization::instance()->splashPath() );
    QString mySplashPath;
    QPixmap myPixmap( mySplashPath + QString( "splash.png" ) );

    QSplashScreen *mypSplash = new QSplashScreen( myPixmap );
    
    bool myHideSplash = false;

    if ( mySettings.value( "/qgis/hideSplash" ).toBool() || myHideSplash )
    {
        //splash screen hidden
    }
    else
    {
        //for win and linux we can just automask and png transparency areas will be used
        mypSplash->setMask( myPixmap.mask() );
        mypSplash->show();
    }

    return mypSplash;
}

/*
void KKSGISWidgetBase::initQGIS()
{
}
*/
KKSGISWidgetBase::~KKSGISWidgetBase()
{
    if(!m_bInit)
        return;//нечего удалять. Виджет не был инициализирован

    //ksa delete mpMapLegendWidget; //в деструкторе класса KKSMapWidget этому виджету делается setParent(NULL), поэтому автоматически он не удаляется. Его надо удалить здесь явным образом

}
   
const QMap<QString, QMenu *> & KKSGISWidgetBase::menuMap() 
{
    return mpMenuMap;
}

const QMap<QString, QToolBar *> & KKSGISWidgetBase::toolBarMap() 
{
    if(mpToolBarMap.isEmpty()){
        mpToolBarMap.insert("mFileToolBar", mFileToolBar);
        mpToolBarMap.insert("mLayerToolBar", mLayerToolBar);
        mpToolBarMap.insert("mDigitizeToolBar", mDigitizeToolBar);
        mpToolBarMap.insert("mMapNavToolBar", mMapNavToolBar);
        mpToolBarMap.insert("mAttributesToolBar", mAttributesToolBar);
    }
    
    return mpToolBarMap;
}




/*************************************/
/*
   Здесь только методы, обеспечивающие работу пользователя с картой
   Конструктор, деструктор, инициализационные и обеспечивающие методы находятся в файле kksgiswidgethelper.cpp
*/
/*
void KKSGISWidgetBase::azRemoveAllLayers()
{
    QgsMapLayerRegistry::instance()->removeAllMapLayers();
    //QgsMapLayerRegistry::instance()->clearAllLayerCaches();
}
*/

/*
bool KKSGISWidgetBase::azSelectLayer(const QString layerName)
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


bool KKSGISWidgetBase::azSelectLayer(const int layerNumber)
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

*/

bool KKSGISWidgetBase::azRasterEnhancement(QgsRasterLayer & azRasterLayer)
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

bool KKSGISWidgetBase::azRasterCheckBandName(QgsRasterLayer &azRasterLayer, QString strBandName)
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

bool KKSGISWidgetBase::azCopyFiles(QString azSource, QString azDestPath, bool bUse)
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
bool KKSGISWidgetBase::azMakeLayer(QGis::WkbType azType, QString pDestDir, QString pName)
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
/*
bool KKSGISWidgetBase::azAddLayerVector(QFileInfo pFile)
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
    connect(mypLayer, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    // Add the Layer to the Layer Set
    //ksa mpLayerSet.append(QgsMapCanvasLayer(mypLayer));
    // set the canvas to the extent of our layer
    mpMapCanvas->setExtent(mypLayer->extent());
    // Set the Map Canvas Layer Set
    //ksa mpMapCanvas->setLayerSet(mpLayerSet);
    
    // update UI
    qApp->processEvents();
    
    mpMapCanvas->refresh();
    
    return true;
}
*/

void KKSGISWidgetBase::SLOTmpActionVectorize()
{
    this->azVectorize();
    this->azAddWorkListToMap(azWorkList);

}


void KKSGISWidgetBase::azAddWorkListToMap(QStringList &pList)
{
    long i(0);
    QString pMessage("");
    foreach (const QString &pString, pList)
    {
        QFileInfo pFile(pString);
        if (pFile.isFile())
        {
            if (openLayer(pString))
            {
                i = i + 1;
                pMessage = "Добавлен слой '" + pFile.baseName() + "'";
                messageBar()->pushMessage( pMessage, pMessage, QgsMessageBar::CRITICAL, messageTimeout() );
            }
        }
    }
    if (i > 1)
    {
        pMessage = "Добавлено " + QString::number(i )+ "слоя(ев)";
        messageBar()->pushMessage( pMessage, pMessage, QgsMessageBar::CRITICAL, messageTimeout() );
    }
}


void KKSGISWidgetBase::azVectorize()
{
#ifdef WIN32
    dnThemTaskSpecBath->close();
    QString pMessage(""); // сообщение в статус баре о результате векторизации
    if (this->dnThemTaskSpecBath->Polygons.count() < 1)
    {
        pMessage = tr("Выбранные объекты не содержат информации для векторизации");
        messageBar()->pushMessage( pMessage, pMessage, QgsMessageBar::CRITICAL, messageTimeout() );
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
        pFeature.initAttributes(2);
        pFeature.setAttribute(1, "deep" );
        pFeature.setAttribute(0,(double)dnVec.Vol);
//        pFeature.setAttribute("comment", "deep" );
//        pFeature.setAttribute("value",(double)dnVec.Vol);

        QgsVectorFileWriter::WriterError mError;
        myWriter.addFeature( pFeature );
        mError = myWriter.hasError();
        if (mError != 0)
        {
            qWarning() << myWriter.errorMessage();
        }
    }
    delete dnThemTaskSpecBath;
    dnThemTaskSpecBath = NULL;
    return;
#endif
}


/*
void KKSGISWidgetBase::azLoadLayer(QgsMapLayer *theMapLayer, bool isExtent)
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

*/

long KKSGISWidgetBase::azGetEPSG(const QString rastrPath)
{
    QFileInfo pFileInfo(rastrPath);
    if (!pFileInfo.isFile()) // проверка существования файла
    {
        return -1;
    }
    QgsRasterLayer * pRLayer = new QgsRasterLayer(pFileInfo.filePath(), pFileInfo.completeBaseName());
    return pRLayer->crs().srsid();
}


/*
void KKSGISWidgetBase::SLOTmpActionAddVectorLayer()
{
    QString fullLayerName = QFileDialog::getOpenFileName(this, "Добавить векторный слой",
                            "", "Все поддерживаемые форматы(*.shp);;Shapefiles (*.shp)");
    
    openVectorLayer(fullLayerName);
}
*/

/*
void KKSGISWidgetBase::SLOTmpActionAddPostGISLayer()
{
    openDatabaseLayer();
}
*/
/*
void KKSGISWidgetBase::SLOTmpActionAddRasterLayer()
{
    // СЛОТ: добавление растра в окно карты
    // сначала вызываем диалог, получаем путь к файлу
    QString fullLayerName = QFileDialog::getOpenFileName(this, "Добавить растровый слой", "",
                             "Все поддерживаемые растровые форматы (*.img *.asc *.tif *tiff *.bmp *.jpg *.jpeg);;Geotiff (*.tif *.tiff)");
    
    openRasterLayer(fullLayerName);

}
*/

void KKSGISWidgetBase::SLOTazThemTaskSpectralBathynometry()
{
#ifdef WIN32
    //ksa -- if (dnThemTaskSpecBath == NULL);
    if (dnThemTaskSpecBath != NULL)
    {
     delete dnThemTaskSpecBath;
    }
    dnThemTaskSpecBath = new DNSpecBath(this);
    connect(this->dnThemTaskSpecBath, SIGNAL(SIGNALcreateVector()), this, SLOT(SLOTmpActionVectorize()));
    dnThemTaskSpecBath->show();
//    QMessageBox::about(0, "a", "a");
//    QFileInfo pFile("D:/!Share/layers/baba.shp");
//    this->azAddLayerVector(pFile);
#endif
}



/********************************************************/
/********************************************************/
/********************************************************/

void KKSGISWidgetBase::showIOEditor(QWidget * parent, const QString & uid)
{
    emit signalShowIOEditor(parent, uid);
}

bool KKSGISWidgetBase::featureFromEIO(QWidget * parent, QgsFeature & feature, const QString & geomAsEWKT, const QString & layerTable)
{
    emit signalFeatureFromEIO(parent, feature, geomAsEWKT, layerTable);
    if(feature.attribute(feature.fieldNameIndex("unique_id")).toString().isEmpty())
        return false;
    
    return true;
}

bool KKSGISWidgetBase::deleteFeaturesAsEIO(QWidget * parent, const QString & tableName, const QList<qint64> & ids)
{
    emit signalDeleteFeaturesAsEIO(parent, tableName, ids);
    return true;
}


void KKSGISWidgetBase::slotUpdateMapByNotify(const QString & nName, const QString & tableName, const QString & idRecord)
{
    const QMap<QString, QgsMapLayer *> layers = QgsMapLayerRegistry::instance()->mapLayers();
    if(layers.count() == 0)
        return;

    QMap<QString, QgsMapLayer *>::const_iterator pa;
    QgsVectorLayer * changedLayer = NULL;

    for (pa = layers.constBegin(); pa != layers.constEnd(); pa++)
    {
        QgsMapLayer * layer = pa.value();
        if(layer->type() != QgsMapLayer::VectorLayer)
            continue;
        QgsVectorLayer * vLayer = (QgsVectorLayer*)layer;

        QString providerName = vLayer->dataProvider()->name();
        if(providerName != "postgres")
            continue;

        QString uri = vLayer->dataProvider()->dataSourceUri();
        QStringList uriSections = uri.split(" ");
        QString layerTable;

        for(int i=0; i<uriSections.count(); i++){
            QString & sec = uriSections[i];
            if(sec.startsWith("table=")){
                QStringList tableSec = sec.split("=");
                if(tableSec.count() != 2){
                    break;
                }
                QString fullTableName = tableSec[1];
                QStringList tableNameSec = fullTableName.split(".");
                if(tableNameSec.count() != 2){
                    break;
                }
                layerTable = tableNameSec[1];
                layerTable.replace("\"", "");
                break;
            }
        }

        if(layerTable != tableName)
            continue;
        
        //vLayer->reload();
        //mpMapCanvas->refresh();
        changedLayer = vLayer;
        break;
    }

    if(!changedLayer)
        return;

    QgsFeature f;
    QgsFeatureRequest fr(QString("unique_id = '%1'").arg(idRecord) );
    QgsFeatureIterator fi = changedLayer->getFeatures(fr);
    bool ok = fi.nextFeature(f);
    //bool ok = changedLayer->updateFeature(f);
    if(!ok){
        int a=0;
        return;
    }
}

void KKSGISWidgetBase::refreshMapCanvasOnly()
{
    mapCanvas()->refresh();
}

bool KKSGISWidgetBase::saveProjectAs(const QString & m_path)
{
    // Retrieve last used project dir from persistent settings
    QSettings settings;
    QString lastUsedDir = settings.value( "/UI/lastProjectDir", "." ).toString();
  
    QString path = m_path;

    if ( path.isEmpty() ){
        path = QFileDialog::getSaveFileName( this,
                 tr( "Choose a file name to save the QGIS project file as" ),
                 lastUsedDir + "/" + QgsProject::instance()->title(),
                 tr( "QGIS files" ) + " (*.qgs *.QGS)" );
    }

    if ( path.isEmpty() )
        return false;

  QFileInfo fullPath( path );

  settings.setValue( "/UI/lastProjectDir", fullPath.path() );

  // make sure the .qgs extension is included in the path name. if not, add it...
  if ( "qgs" != fullPath.suffix().toLower() )
  {
    fullPath.setFile( fullPath.filePath() + ".qgs" );
  }

  QgsProject::instance()->setFileName( fullPath.filePath() );

  if ( QgsProject::instance()->write() )
  {
    //ksa setTitleBarText_( *this ); // update title bar
    statusBar()->showMessage( tr( "Saved project to: %1" ).arg( QgsProject::instance()->fileName() ), 5000 );
    // add this to the list of recently used project files
    saveRecentProjectPath( fullPath.filePath(), settings );
  }
  else
  {
    QMessageBox::critical( this,
                           tr( "Unable to save project %1" ).arg( QgsProject::instance()->fileName() ),
                           QgsProject::instance()->error(),
                           QMessageBox::Ok,
                           Qt::NoButton );

    return false;
  }

  return true;
} 

bool KKSGISWidgetBase::saveProject()
{
    return fileSave();
}

QString KKSGISWidgetBase::projectFileName()
{
    return QgsProject::instance()->fileName();
}

//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
QString KKSGISWidgetBase::readLayerFilePath(const QString & file) const
{
    return QgsProject::instance()->readPath(file);
}

void KKSGISWidgetBase::reloadLayer(const QString & theLayerId)
{
    QgsMapLayerRegistry::instance()->mapLayer(theLayerId)->reload();
    //mpMapCanvas->refresh();
}
