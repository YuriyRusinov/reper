
// --------------------------- test VectorWriter ----------------------------

//    pProvider->select(pProvider->attributeIndexes());
//    while (pProvider->nextFeature(pFeat));
//    {
//        pGeom = pFeat.geometry();
//        QMessageBox::about(this, "hell", QString::number(pFeat.id()));
//    }
//    pFeat.setGeometry(pGeom);
//    pGeom
//    pLayer->addFeature(pFeat);

//    QString msg, t1;
//    msg = "mpRegestry: " + t1.number(mpRegistry->count()) + "\n";
//    msg = msg + "mpLayerSet: " + t1.number(mpLayerSet.count()) + "\n";
//    QMessageBox::about(this, "info", this->mpAppPath+"/shps/ex_poly.shp");

//    if (!azCopyFiles(this->mpAppPath+"shps/ex_poly.shp", "D:/!Share/layers/test4.shp"))
//    {
//        QMessageBox::about(this, "Test", "bad");
//    }

//    this->azMakeLayer(QGis::WKBPolygon, "D:/!Share/layers/", "test2");
    // QgsVectorLayer *pLayer = new QgsVectorLayer("D:/!Share/layers/", "test2", "ogr");
    // QgsVectorFileWriter::WriterError pError;
//    pError = QgsVectorFileWriter::writeAsVectorFormat(pLayer, pFile, "utf-8", pSRS, "ESRI Shapefile");
//    pError =

//    QgsVectorLayer *pLayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
//    if ( !pLayer )
//    {
//        QMessageBox::critical(this, "Error", "pLayer is nothing...");
//        return;
//    }

//    QString pFile;
//    pFile = "shapeTestAzura.shp";

//    QGis::WkbType pGeom;
//    pGeom = QGis::WKBPolygon;

//	QString path = QgsApplication::srsDbFilePath();

//    QgsCoordinateReferenceSystem pSRS;


//    pSRS.createFromOgcWmsCrs("EPSG:4326"); // use WGS 84

//    pSRS.validate();

//    QString enc("utf-8");

//    QgsProviderRegistry * pReg = QgsProviderRegistry::instance();
//    QString ogrlib = pReg->library( "ogr" );
//    QLibrary* myLib = new QLibrary( ogrlib );
//    bool loaded = myLib->load();
//    if (!loaded )
//    {
//        QMessageBox::critical(this, "Error", "myLib not load...");
//    }
//    QgsVectorLayer *pLayer = new QgsVectorLayer("D:\\!Share\\layers", "testAzura", "ogr");

//    QgsVectorFileWriter::WriterError pError;
//    pError = QgsVectorFileWriter::writeAsVectorFormat(pLayer, pFile, "utf-8", pSRS, "ESRI Shapefile");

//    QgsVectorLayer *pLayer = qobject_cast<QgsVectorLayer *>( activeLayer() );
//    if ( !pLayer )
//    {
//        QMessageBox::critical(this, "Error", "pLayer is nothing...");
//      return;
//    }


//    QgsVectorFileWriter::WriterError pError;
//    pError = QgsVectorFileWriter::writeAsVectorFormat(pLayer, pFile, "utf-8", pSRS, "ESRI Shapefile");

//    error = QgsVectorFileWriter::writeAsVectorFormat(
//              vlayer,
//              vectorFilename,
//              encoding,
//              &destCRS,
//              format,
//              saveOnlySelection,
//              &errorMessage,
//              datasourceOptions, dialog->layerOptions(),
//              dialog->skipAttributeCreation(),
//              &newFilename );


//    QString ogrlib = pReg->library( "ogr" );
//    QLibrary* myLib = new QLibrary( ogrlib );
//    bool loaded = myLib->load();
//    if ( loaded )
//    {
//     typedef bool ( *createEmptyDataSourceProc )( const QString&, const QString&, const QString&, QGis::WkbType,
//          const std::list<std::pair<QString, QString> >&, const QgsCoordinateReferenceSystem * );
//      createEmptyDataSourceProc createEmptyDataSource = ( createEmptyDataSourceProc ) cast_to_fptr( myLib->resolve( "createEmptyDataSource" ) );
//      if ( createEmptyDataSource )
//      {
//        if ( geometrytype != QGis::WKBUnknown )
//        {
//          QgsCoordinateReferenceSystem srs( crsId, QgsCoordinateReferenceSystem::InternalCrsId );
//          if ( !createEmptyDataSource( fileName, fileformat, enc, geometrytype, attributes, &srs ) )
//          {
//            return QString();
//          }
//        }
//        else
//        {
//          QgsDebugMsg( "geometry type not recognised" );
//          return QString();
//        }
//      }
//      else
//      {
//        QgsDebugMsg( "Resolving newEmptyDataSource(...) failed" );
//        return QString();
//      }
//    }


//    QgsVectorLayer *pLayer = new QgsVectorLayer( src, pFile, "ogr" );

//    pFile =

//    QgsVectorFileWriter::WriterError error;
//    error = QgsVectorFileWriter.writeAsShapefile(pLayer, "newShape19");
//    error = QgsVectorFileWriter.writeAsVectorFormat(pLayer, "my_shapes.shp", \
//      "CP1250", None, "ESRI Shapefile")

//    if error == QgsVectorFileWriter.NoError:
//      print "success!"

/*
void SLOTmpActionVectorize()
{
//    mpLayerSet.at(n);

    if (this->azSelectLayer("test2"))
    {
//        QMessageBox::about(this, "hell", mpSelectedLayer->name());
    }
    if (mpSelectedLayer == NULL)
    {
        QMessageBox::about(this, "hell", "Bye-bye!");
        return;
    }
    QgsVectorLayer * pLayer ;
    pLayer = dynamic_cast<QgsVectorLayer*> (mpSelectedLayer);
    QMessageBox::about(this, "hell", QString::number(pLayer->featureCount()));

//    QgsFeature pFeat;
//    QgsGeometry pGeom;
//    QgsVectorDataProvider *pProvider;
//    pProvider = pLayer->dataProvider();
//    QgsPolygon pPoly;


    // --azura
    QString mEncoding;
    QgsFieldMap mFields;
    QgsPoint mPoint1;
    QgsPoint mPoint2;
    QgsPoint mPoint3;



    mEncoding = "UTF-8";
    QgsField myField2("areaDouble", QVariant::Double, "Double", 0, 0, "Azura's comment");
    QgsField myField1( "Field1", QVariant::String, "String", 10, 0, "Field 1 comment" );
    mFields.insert( 0, myField1 );
    mFields.insert(1, myField2);
    QgsCoordinateReferenceSystem pSRS;
    pSRS.createFromOgcWmsCrs("EPSG:4326"); // use WGS 84
    pSRS.validate();
    mPoint1 = QgsPoint( 10.0, 10.0 );
    mPoint2 = QgsPoint( 15.0, 10.0 );
    mPoint3 = QgsPoint( 15.0, 12.0 );


    QString myFileName ("/testply.shp");
    myFileName = "D:/!Share/src/reper/ddocclient/build/layers" + myFileName;
    QgsVectorFileWriter myWriter( myFileName, mEncoding, mFields, QGis::WKBPolygon, &pSRS);

    QgsPolyline myPolyline;
    myPolyline << mPoint1 << mPoint2 << mPoint3 << mPoint1;
    QgsPolygon myPolygon;
    myPolygon << myPolyline;
    //polygon: first item of the list is outer ring,
    // inner rings (if any) start from second item
    //
    // NOTE: don't delete this pointer again -
    // ownership is passed to the feature which will
    // delete it in its dtor!
    QgsGeometry * mypPolygonGeometry = QgsGeometry::fromPolygon( myPolygon );
    QgsFeature myFeature;
    myFeature.setTypeName( "WKBPolygon" );
    myFeature.setGeometry( mypPolygonGeometry );
    myFeature.addAttribute( 0, "HelloWorld" );
    myFeature.addAttribute(1, 334);

    QgsVectorFileWriter::WriterError mError;
    myWriter.addFeature( myFeature );
    mError = myWriter.hasError();

    return;

}
*/
// -------------------------------- Vectorize
//    void vectorize()
//{
//    QString pMessage(""); // сообщение в статус баре о результате векторизации
//    if (this->dnThemTaskSpecBath->Polygons.count() < 1)
//    {

//    }

//    QString mEncoding; // кодировка
//    mEncoding = "UTF-8";

//    //ksa -- QgsFieldMap mFields;
//    QgsFields mFields;
//    QgsField myField1("value", QVariant::Double, "Double", 0, 0);
//    QgsField myField2( "comment", QVariant::String, "String", 10, 0, "Comment" );
//    mFields.append(myField1);
//    mFields.append(myField2);
//    //ksa -- mFields.insert( 0, myField1 );
//    //ksa -- mFields.insert( 1, myField2 );
//    QgsCoordinateReferenceSystem pSRS;

//    // создаем систему координат идентичную растру
//    pSRS.createFromOgcWmsCrs("EPSG:" +
//                             QString::number(azGetEPSG(dnThemTaskSpecBath->Polygons.at(0).EPSG)));
//    pSRS.validate();
//    QString myFileName ("/" + dnThemTaskSpecBath->Polygons.at(0).NameLayer +
//                        QString::number(QTime::currentTime().hour())  + "-" +
//                        QString::number(QTime::currentTime().minute())  + "-" +
//                        QString::number(QTime::currentTime().second()) + "-" +
//                        QString::number(QTime::currentTime().msec()) + ".shp");
//    myFileName = "D:/!Share/layers/" + myFileName;

//    QgsVectorFileWriter myWriter( myFileName, mEncoding, mFields, QGis::WKBPolygon, &pSRS);

//    QMessageBox::about(this->dnThemTaskSpecBath, "test", "Hello!");
//    for (int i = 0; i < dnThemTaskSpecBath->Polygons.size(); i++)
//    {
//        DNVector dnVec;
//        dnVec = dnThemTaskSpecBath->Polygons.at(i);
//        QgsPolyline pPolyLine;
//		QgsPoint pFirstPoint (dnVec.GPt.at(0).x, dnVec.GPt.at(0).y);
//        for (int j = 0; j < dnVec.GPt.size(); j++)
//        {
//            QgsPoint p(dnVec.GPt.at(j).x, dnVec.GPt.at(j).y);
//            pPolyLine << p;
//        }
//		pPolyLine << pFirstPoint;
//        QgsPolygon pPolygon;
//        pPolygon << pPolyLine;
//        QgsGeometry * pPolygonGeometry = QgsGeometry::fromPolygon( pPolygon );
//        QgsFeature pFeature;
//        //ksa -- pFeature.setTypeName( "WKBPolygon" );
//        pFeature.setGeometry( pPolygonGeometry );
//        pFeature.setAttribute( "comment", "val" );
//        pFeature.setAttribute("value", (double)dnVec.Vol);

//        QgsVectorFileWriter::WriterError mError;
//        myWriter.addFeature( pFeature );
//        mError = myWriter.hasError();
//		//QMessageBox::about(0, "", );
//		qWarning() << myWriter.errorMessage();


////    QgsPoint mPoint1;
////    QgsPoint mPoint2;
////    QgsPoint mPoint3;


////    mPoint1 = QgsPoint( 10.0, 10.0 );
////    mPoint2 = QgsPoint( 15.0, 10.0 );
////    mPoint3 = QgsPoint( 15.0, 12.0 );


////    QString myFileName ("/testply.shp");
////    myFileName = "D:/!Share/src/reper/ddocclient/build/layers" + myFileName;
////    QgsVectorFileWriter myWriter( myFileName, mEncoding, mFields, QGis::WKBPolygon, &pSRS);

////    QgsPolyline myPolyline;
////    myPolyline << mPoint1 << mPoint2 << mPoint3 << mPoint1;
////    QgsPolygon myPolygon;
////    myPolygon << myPolyline;
////    //polygon: first item of the list is outer ring,
////    // inner rings (if any) start from second item
////    //
////    // NOTE: don't delete this pointer again -
////    // ownership is passed to the feature which will
////    // delete it in its dtor!
////    QgsGeometry * mypPolygonGeometry = QgsGeometry::fromPolygon( myPolygon );
////    QgsFeature myFeature;
////    myFeature.setTypeName( "WKBPolygon" );
////    myFeature.setGeometry( mypPolygonGeometry );
////    myFeature.addAttribute( 0, "HelloWorld" );
////    myFeature.addAttribute(1, 334);

////    QgsVectorFileWriter::WriterError mError;
////    myWriter.addFeature( myFeature );
////    mError = myWriter.hasError();

//    return;

//    }
