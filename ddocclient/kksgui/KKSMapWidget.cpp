/***********************************************************************
 * Module:  KKSMapWidget.cpp
 * Author:  sergey
 * Modified: 13 декабря 2013 г. 14:49:21
 * Purpose: Implementation of the class KKSMapWidget
 ***********************************************************************/

#include "KKSMapWidget.h"

#include <QtDebug>
#include <QDomDocument>
#include "KKSAttrValue.h"
#include "KKSAttrType.h"
#include "KKSFile.h"
#include "defines.h"
#include <QDir>
#include <QMessageBox>
#include <QFile>

#ifdef __USE_QGIS__
#include "kksgiswidget.h"
#endif

KKSMapWidget::KKSMapWidget(const QString & gisHomeDir, const KKSAttrValue* attr, KKSIndAttrClass isSys, QWidget *parent)
    : QWidget(parent), 
      KKSAttrWidget(attr, isSys),
      m_legendWidget(0),
      m_layerOrderWidget(0),
      m_GISHomeDir(gisHomeDir),
      m_dataChanged(false)
{
#ifdef __USE_QGIS__
    mpKKSGISWidget = KKSGISWidget::instance(); //присваивается NULL
    if(mpKKSGISWidget){ //уже открыт редактор ИО с картой
        mpKKSGISWidget = 0;
        QMessageBox::critical(parent, tr("GIS support error"), tr("There are already open an IO Editor with map! Current version of DynamicDocs Client supports only one opened IO Editor with GIS capabilities"));
        return;
    }
#endif
    init(this);
}

KKSMapWidget::~KKSMapWidget()
{
    if(m_legendWidget && m_layerOrderWidget)
        emit aboutToDestroy(m_legendWidget, m_layerOrderWidget);
    
    m_legendWidget = 0;
    m_layerOrderWidget = 0;

#ifdef __USE_QGIS__
    if(mpKKSGISWidget){
        mpKKSGISWidget->setParent(0);
        KKSGISWidget::destroyQGISWidget(mpKKSGISWidget);
        mpKKSGISWidget = 0;
    }
#endif
}

QDockWidget * KKSMapWidget::legendWidget() const
{
    return m_legendWidget;
}

QDockWidget * KKSMapWidget::layerOrderWidget() const
{
    return m_layerOrderWidget;
}

void KKSMapWidget::init(QWidget * parent)
{
#ifdef __USE_QGIS__
    initQGIS(parent);
#endif
}


#ifdef __USE_QGIS__

void KKSMapWidget::initQGIS(QWidget *parent)
{
    bool withSubWindows = true;
    
    mpKKSGISWidget = KKSGISWidget::initQGISWidget(withSubWindows, parent);
    if(!mpKKSGISWidget){
        QMessageBox::critical(0, "", "");
        return;
    }
    
    connect(mpKKSGISWidget, SIGNAL(mapSaved(QDomDocument &)), this, SLOT(slotMapChanged(QDomDocument&)));
    connect(mpKKSGISWidget, SIGNAL(dataChanged()), this, SLOT(slotDataChanged()));

    QVBoxLayout * mpMapLayout = new QVBoxLayout(parent); //main Layout
    QMenuBar * mb = mpKKSGISWidget->menuBar();
    if(mb)
        mpMapLayout->setMenuBar(mb);
    
    QToolBar * tb = mpKKSGISWidget->toolBar();
    if(tb)
        mpMapLayout->addWidget(tb);

    mpMapLayout->addWidget(mpKKSGISWidget);

    QWidget * w = mpKKSGISWidget->statusBar();
    if(w){
        w->setFixedHeight(30);
        mpMapLayout->addWidget(w);
    }

    mpMapLayout->setContentsMargins(0, 0, 0, 0);


    QWidget * legend = mpKKSGISWidget->mapLegendWidget();
    if(legend){
        m_legendWidget = new QDockWidget( tr( "Layers" ), this );
        m_legendWidget->setObjectName( "Legend" );
        m_legendWidget->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
        m_legendWidget->setWidget( legend );
        //addDockWidget( Qt::LeftDockWidgetArea, mLegendDock );
    }
    
    QWidget * layerOrder = mpKKSGISWidget->mapLayerOrderWidget();
    if(layerOrder){
        m_layerOrderWidget = new QDockWidget( tr( "Layer order" ), this );
        m_layerOrderWidget->setObjectName( "LayerOrder" );
        m_layerOrderWidget->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
        m_layerOrderWidget->setWidget( layerOrder );
        //addDockWidget( Qt::LeftDockWidgetArea, mLayerOrderDock );
        m_layerOrderWidget->hide();
    }

    setWindowTitle(mpKKSGISWidget->windowTitle());

    openProject(); //открываем проект, который задан в качестве значения атрибута
    
}

#endif


int KKSMapWidget::openProject()//открываем проект, заданный в качестве значения атрибута
{
#ifdef __USE_QGIS__ 
	if(!m_av)
        return -1;

    KKSValue v = m_av->value();
    
    QString xmlPrj = v.value();
    if(xmlPrj.isEmpty())
        return -1;

    QString homeDir = m_GISHomeDir;

    qint64 id = m_av->id();
    QString path = homeDir + "/qgis_prj_" + QString::number(id) + ".qgs";
    QDir d(homeDir);
    if(!d.exists()){
        if(!d.mkdir(homeDir)){
            QMessageBox::critical(this, tr("Error"), tr("QGIS temporary files home dir does not exist! Cannot create that! Path = %1").arg(homeDir));
            return -1;
        }
    }

    QFile f(path);
    
    bool ok = f.open(QIODevice::ReadWrite);
    if(!ok){
        QMessageBox::critical(this, tr("Error"), tr("Cannot create file for storing QGIS project file (%1)!").arg(path));
        return -1;
    }

    qint64 bytes = f.write(xmlPrj.toLocal8Bit());
    if(bytes <= 0){
        QMessageBox::critical(this, tr("Error"), tr("Cannot write data to QGIS project file (%1)!").arg(path));
        return -1;
    }
    
    f.flush();
    f.close();

    int res = downloadLayers(homeDir, xmlPrj);
    //if(res != 1){
    //    QMessageBox::critical(this, tr("Error"), tr("Some of layer files for this GIS project was downloaded with errors!"));
    //    return -1;
    //}

    mpKKSGISWidget->openProject(path);

    return 1;
#else
	return -1;
#endif
}

//выгрузка на клиент файлов со слоями, если слои представлены файлами (т.е. не PostGIS)
int KKSMapWidget::downloadLayers(const QString & homeDir, const QString & xmlPrj)
{
    QList<qint64> ids;

    bool haveErrors = false;
    QDomDocument d;
    d.setContent(xmlPrj);
    QDomNodeList layers = d.elementsByTagName("maplayer");
    int cnt = layers.length();
    for(int index = 0; index<cnt; index++){
        QDomNode layer = layers.item(index);
        QDomNodeList itemsDS = layer.toElement().elementsByTagName("datasource");
        int cnt = itemsDS.count();
        if(cnt != 1){
            haveErrors = true;
            continue;
        }

        QDomNodeList itemsID = layer.toElement().elementsByTagName("id");
        cnt = itemsID.count();
        if(cnt != 1){
            haveErrors = true;
            continue;
        }

        QDomNodeList itemsProvider = layer.toElement().elementsByTagName("provider");
        cnt = itemsProvider.count();
        if(cnt != 1){
            haveErrors = true;
            continue;
        }

        QDomNodeList itemsLayerName = layer.toElement().elementsByTagName("layername");
        cnt = itemsLayerName.count();
        if(cnt != 1){
            haveErrors = true;
            continue;
        }

        QString path = itemsDS.item(0).toElement().text();
        QString id = itemsID.item(0).toElement().text();
        QString provider = itemsProvider.item(0).toElement().text();
        QString layerName = itemsLayerName.item(0).toElement().text();

        QString fileName = getFileNameForDS(path, provider);
        if(fileName.isEmpty()){
            haveErrors = true;
            continue;
        }

        qint64 idUrl = getIdUrlForID(id);
        
        KKSFile * f = new KKSFile(idUrl, layerName);
        f->setLocalUrl(homeDir + fileName);
        emit downloadFile(f, this);
        if(f->localUrl().isEmpty()){
            QMessageBox::critical(this, tr("Error"), tr("Cannot to download file with requested layer!\nTry to download to %1\nLayer id = %2\nLayer provider = %3").arg(homeDir + fileName).arg(id).arg(provider));
            haveErrors = true;
            continue;
        }
    }

    return haveErrors ? -1 : 1;
}

//берем часть, которая представляет собой название файла, из datasource
QString KKSMapWidget::getFileNameForDS(const QString & path, const QString & provider)
{
#ifdef __USE_QGIS__
    QString fileName;
    if(provider == "ogr"){
        QStringList theURIParts = path.split( "|" );
        fileName = mpKKSGISWidget->readLayerFilePath( theURIParts[0] );
        //fileName = theURIParts.join( "|" );
        QFileInfo fi(fileName);
        fileName = fi.fileName();
    }
    else if ( provider == "delimitedtext" ){
        QUrl urlSource = QUrl::fromEncoded( path.toAscii() );
 
        if ( !path.startsWith( "file:" ) ){
            QUrl file = QUrl::fromLocalFile( path.left( path.indexOf( "?" ) ) );
            urlSource.setScheme( "file" );
            urlSource.setPath( file.path() );
        }
 
        QUrl urlDest = QUrl::fromLocalFile( mpKKSGISWidget->readLayerFilePath( urlSource.toLocalFile() ) );
        urlDest.setQueryItems( urlSource.queryItems() );
        fileName = QString::fromAscii( urlDest.toEncoded() );
    }
    else if(provider == "postgres"){
        //!!!!!!!!!!!!!!!!
    }
    else{
        fileName = mpKKSGISWidget->readLayerFilePath( path );
    }

    return fileName;
#else
	return QString::null;
#endif
}
//Берем часть, которая представляет собой идентификатор файла в таблице io_urls, из id слоя
//Считаем, что идентификатор слоя представлен так: kks_file_<ID_URL>
qint64 KKSMapWidget::getIdUrlForID(const QString & id)
{
    if(id.isEmpty())
        return -1;

    qint64 idUrl = -1;
    if(!id.startsWith("kks_file_"))
        return -1;

    QString s = id;
    s.remove(0, 9); //kks_file_
    
    bool ok = true;
    idUrl = s.toLongLong(&ok, 10);
    if(!ok)
        return -1;

    return idUrl;
}

void KKSMapWidget::slotDataChanged()
{
    m_dataChanged = true;
}

void KKSMapWidget::slotMapChanged(QDomDocument & prjFile)
{
    QString xml = prjFile.toString();
    
    KKSValue v(xml, KKSAttrType::atGISMap);
    if(!v.isValid()){
        QMessageBox::critical(this, tr("Error"), tr("Cannot set attribute value for GIS attribute!"));
        return;
    }

    //m_av->setValue(v);

    emit valueChanged(m_av->id(), m_isSystem, v.valueVariant());

    m_dataChanged = false;
}
