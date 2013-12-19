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

KKSMapWidget::KKSMapWidget(qint64 idObj, const QString & gisHomeDir, const KKSAttrValue* attr, KKSIndAttrClass isSys, QWidget *parent)
    : QWidget(parent), 
      KKSAttrWidget(attr, isSys),
      m_legendWidget(0),
      m_layerOrderWidget(0),
      m_GISHomeDir(gisHomeDir),
      m_dataChanged(false),
      m_idObj(idObj)
{
#ifdef __USE_QGIS__
    mpKKSGISWidget = KKSGISWidget::instance(); //присваивается NULL
    if(mpKKSGISWidget){ //уже открыт редактор ИО с картой
        mpKKSGISWidget = 0;
        QMessageBox::critical(parent, tr("GIS support error"), tr("There are already open an IO Editor with map! Current version of DynamicDocs Client supports only one opened IO Editor with GIS capabilities"));
        return;
    }
#endif
    //init(this); //вызывается отдельно!!!
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

void KKSMapWidget::init()
{
#ifdef __USE_QGIS__
    initQGIS(this);
#else
    QMessageBox::warning(this, tr("Warning"), tr("Current IO contains GIS-object attribute!\nBut current build of DynamicDocs Client does not support GIS capabilities!\nWorking with this attribute will be disabled"), QMessageBox::Ok);
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
    
    connect(mpKKSGISWidget, SIGNAL(projectRead()), this, SLOT(slotMapOpened()));
}

#endif


int KKSMapWidget::openProject()//открываем проект, заданный в качестве значения атрибута
{
#ifdef __USE_QGIS__ 
	if(!m_av || m_idObj <= 0)
        return -1;

    KKSValue v = m_av->value();
    
    QString xmlPrj = v.value();
    if(xmlPrj.isEmpty())
        return -1;

    QString homeDir = m_GISHomeDir;
    QDir d(homeDir);
    if(!d.exists()){
        if(!d.mkdir(homeDir)){
            QMessageBox::critical(this, tr("Error"), tr("QGIS temporary files home dir does not exist! Cannot create that! Path = %1").arg(homeDir));
            return -1;
        }
    }
    
    d.cd(homeDir);
    
    qint64 id = m_idObj;

    QString prjHomeDir = QString("qgis_prj_") + QString::number(id);
    QDir dPrj(homeDir + "/" + prjHomeDir);
    if(dPrj.exists()){
        int answer = QMessageBox::question(this, tr("Open QGIS project"), tr("directory with name\n%1\nexist in QGIS projects home dir!\nIt will be removed and all data in it will be lost. Do you want to continue?").arg(homeDir + "/" + prjHomeDir), QMessageBox::Yes|QMessageBox::No);
        if(answer == QMessageBox::No)
            return -1;

        bool ok = this->removeQGISPrjDir(homeDir + "/" + prjHomeDir);
        if(!ok){
            QMessageBox::critical(this, tr("Error"), tr("Cannot remove directory!\nPath = %1").arg(homeDir + "/" + prjHomeDir));
            return -1;
        }
    }

    if(!dPrj.mkdir(homeDir + "/" + prjHomeDir)){
        QMessageBox::critical(this, tr("Error"), tr("Cannot create QGIS project home directory!\nPath = %1").arg(homeDir + "/" + prjHomeDir));
        return -1;
    }

    QString path = homeDir + "/" + prjHomeDir + "/qgis_prj_" + QString::number(id) + ".qgs";

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

    int res = downloadLayers(homeDir + "/" + prjHomeDir, xmlPrj);
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
    /*QList<qint64> ids;

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
    */

    bool bForRec = false;
    switch (m_isSystem)
    {
    case iacEIOUserAttr:
    case iacTableAttr:
        bForRec = true;
        break;
    case iacIOUserAttr:
        bForRec = false;
        break;
    case iacEIOSysAttr:
        QMessageBox::critical(this, tr("Error"), tr("Records system attributes cannot contains GIS-object!"));
        return -1;
        break;
    case iacAttrAttr:
        QMessageBox::critical(this, tr("Error"), tr("Complex attributes cannot contains GIS-object!"));
        return -1;
        break;
    }
    
    emit downloadGISFiles(bForRec, homeDir, m_idObj, this);

    return 1;
}

//берем часть, которая представляет собой название файла, из datasource
QString KKSMapWidget::getFileNameForDS(const QString & path, const QString & provider)
{
#ifdef __USE_QGIS__
    QString fileName;
    if(provider == "ogr"){
        QStringList theURIParts = path.split( "|" );
        fileName = mpKKSGISWidget->readLayerFilePath( theURIParts[0] );//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
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
 
        QUrl urlDest = QUrl::fromLocalFile( mpKKSGISWidget->readLayerFilePath( urlSource.toLocalFile() ) ); //возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
        urlDest.setQueryItems( urlSource.queryItems() );
        fileName = QString::fromAscii( urlDest.toEncoded() );
    }
    else if(provider == "postgres"){
        //!!!!!!!!!!!!!!!!
    }
    else{ //rasters ??
        fileName = mpKKSGISWidget->readLayerFilePath( path );//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
    }

    return fileName;
#else
	return QString::null;
#endif
}
//Берем часть, которая представляет собой идентификатор файла в таблице io_urls, из id слоя
//Считаем, что идентификатор слоя представлен так: kks_file_<ID_URL>
/*
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
*/

void KKSMapWidget::slotDataChanged()
{
    //m_dataChanged = true;
}

void KKSMapWidget::slotMapChanged(QDomDocument & prjFile)
{
    Q_UNUSED(prjFile); //там данные все равно не корректные. Нет слоев
    
    slotMapOpened();
}

void KKSMapWidget::slotMapOpened()
{

    QString xml = readGISProjectFile();

    KKSValue v(xml, KKSAttrType::atGISMap);
    if(!v.isValid()){
        QMessageBox::critical(this, tr("Error"), tr("Cannot set attribute value for GIS attribute!"));
        return;
    }

    //m_av->setValue(v);//потом будем использовать при загрузке файлов на сервер

    emit valueChanged(m_av->id(), m_isSystem, v.valueVariant());

    m_dataChanged = true;
}

bool KKSMapWidget::removeQGISPrjDir(const QString & path) 
{
    QDir dir(path); 
    QStringList files = dir.entryList(QDir::Files);

    QStringList::Iterator itFile = files.begin(); 
    while (itFile != files.end()) 
    {
        QFile file(path + "/" + *itFile);
        file.remove();
        ++itFile;
    } 

    QStringList dirs = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot); 
    QStringList::Iterator itDir = dirs.begin(); 
    while (itDir != dirs.end())
    { 
        bool ok = this->removeQGISPrjDir(path + "/" + *itDir);
        if(!ok)
            return false;

        ++itDir; 
    } 

    return dir.rmdir(path);
}

void KKSMapWidget::slotUploadGISFiles(qint64 idObj)
{
    if(!m_dataChanged)
        return;

    if(!m_av)
        return;

    bool bForRec = false;
    switch (m_isSystem)
    {
    case iacEIOUserAttr:
    case iacTableAttr:
        bForRec = true;
        break;
    case iacIOUserAttr:
        bForRec = false;
        break;
    case iacEIOSysAttr:
        QMessageBox::critical(this, tr("Error"), tr("Records system attributes cannot contains GIS-object!"));
        return;
        break;
    case iacAttrAttr:
        QMessageBox::critical(this, tr("Error"), tr("Complex attributes cannot contains GIS-object!"));
        return;
        break;
    }
    
    QStringList files;
    
    QString xml = readGISProjectFile();
    if(xml.isEmpty())
        return;

    files = readGISProjectLayerFiles(xml);

    m_idObj = idObj;
    emit uploadGISFiles(bForRec, files, m_idObj, this);//загрузка файлов ГИС-проекта на сервер. Файлы задаются абсолютными путями
}

QStringList KKSMapWidget::readGISProjectLayerFiles(const QString & xml) const
{
    QStringList files;

#ifdef __USE_QGIS__

    QDomDocument d;
    d.setContent(xml);
    QDomNodeList layers = d.elementsByTagName("maplayer");
    int cnt = layers.length();
    
    for(int index = 0; index<cnt; index++){
        QDomNode layer = layers.item(index);
    
        QDomNode mnl;
        QDomElement mne;

        QString provider;
        QString dataSource;
        
        mnl = layer.namedItem("provider");
        mne = mnl.toElement();
        provider = mne.text();
        
        if(provider == "postgres" || provider == "spatialite" || provider == "wms"){
            continue;//эти источники представлены ресурсами БД в сети, а не файлами
        }

        mnl = layer.namedItem( "datasource" );
        mne = mnl.toElement();
        dataSource = mne.text();

        QString fileAbsUrl;

        if(provider == "ogr"){
            QStringList theURIParts = dataSource.split( "|" );
            fileAbsUrl = mpKKSGISWidget->readLayerFilePath( theURIParts[0] );//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
        }
        else if ( provider == "delimitedtext" ){
            //???????????
        }
        else{ //rasters ??
            fileAbsUrl = mpKKSGISWidget->readLayerFilePath( dataSource );//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
        }
        
        QFileInfo fi(fileAbsUrl);
        if(fi.exists()){
            QStringList filter;
            filter << fi.baseName() + ".*";
            QDir dir = fi.absoluteDir();
            dir.setNameFilters(filter);
            QFileInfoList fiList = dir.entryInfoList(filter, QDir::Files);
            for(int j=0; j<fiList.count(); j++){
                QFileInfo f = fiList.at(j);
                QString absFilePath = f.absoluteFilePath();
                files.append(absFilePath);
            }
            
        }
    }

#endif

    return files;
}

QString KKSMapWidget::readGISProjectFile()
{
    QString xml;

#ifdef __USE_QGIS__
    QString prjFile = mpKKSGISWidget->projectFileName();
    QFile f(prjFile);
    bool ok = f.open(QIODevice::ReadOnly);
    if(!ok){
        QMessageBox::critical(this, tr("Error"), tr("Cannot open project file! Path = %1").arg(prjFile));
        return xml;
    }

    xml = f.readAll();

#endif

    return xml;
}
