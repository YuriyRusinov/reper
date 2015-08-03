/***********************************************************************
 * Module:  KKSMapWidget.cpp
 * Author:  sergey
 * Modified: 13 декабря 2013 г. 14:49:21
 * Purpose: Implementation of the class KKSMapWidget
 ***********************************************************************/

#include "KKSMapWidget.h"

#include <QtDebug>
#include <QDomDocument>
#include "KKSAttribute.h"
#include "KKSAttrValue.h"
#include "KKSObjectExemplar.h"
#include "KKSAttrType.h"
#include "KKSState.h"
#include "KKSFile.h"
#include "defines.h"
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QUrl>

KKSMapWidget::KKSMapWidget(QMap<QString, QString> connectionParams,
                           qint64 idObj, 
                           const QString & gisHomeDir, 
                           const KKSAttrValue* attr, 
                           KKSIndAttrClass isSys, 
                           QWidget *parent)
#ifdef __USE_QGIS__
    : KKSGISWidget(true, false, parent),
#else
    : QWidget(parent), 
#endif
      KKSAttrWidget(attr, isSys),
      m_legendWidget(0),
      m_layerOrderWidget(0),
      m_GISHomeDir(gisHomeDir),
      m_dataChanged(false),
      m_idObj(idObj)
{
    m_connectionParams = connectionParams;
}

KKSMapWidget::~KKSMapWidget()
{
    if(m_legendWidget && m_layerOrderWidget){
#ifdef __USE_QGIS__
        //ksa QWidget * legend = this->mapLegendWidget();
        //ksa legend->setParent(NULL);
#endif
        emit aboutToDestroy(m_legendWidget, m_layerOrderWidget);
    }
    
    m_legendWidget = 0;
    m_layerOrderWidget = 0;
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
    this->initQGIS();
    
    connect(this, SIGNAL(signalFeatureFromEIO(QWidget *, QgsFeature &, const QString &, const QString &)), this, SLOT(slotFeatureFromEIO(QWidget *, QgsFeature &, const QString &, const QString &)));
    connect(this, SIGNAL(signalDeleteFeaturesAsEIO(QWidget *, const QString &, const QList<qint64> &)), this, SLOT(slotDeleteFeaturesAsEIO(QWidget *, const QString &, const QList<qint64> &)));
    
#else
    QMessageBox::warning(this, tr("Warning"), tr("Current IO contains GIS-object attribute!\nBut current build of DynamicDocs Client does not support GIS capabilities!\nWorking with this attribute will be disabled"), QMessageBox::Ok);
#endif
}


#ifdef __USE_QGIS__

void KKSMapWidget::initQGIS()
{
    QString prjName = KKSGISWidget::projectFileName();
    if(!prjName.isEmpty()){
        QMessageBox::critical(this, tr("Error"), tr("Current version of DynamicDocs Client supports only one opened GIS-enabled window!"));
        return;
    }

    KKSGISWidget::initQGIS();

    //bool withSubWindows = true;
    
    //mpKKSGISWidget = KKSGISWidget::initQGISWidget(withSubWindows, parent);
    //if(!mpKKSGISWidget){
    //    QMessageBox::critical(0, "", "");
    //    return;
    //}
    
    //QVBoxLayout * vBoxLayout = new QVBoxLayout();
    //parent->setLayout(vBoxLayout);

    QVBoxLayout * mpMapLayout = new QVBoxLayout(); //main Layout
    QWidget * wParent = (QWidget*)parent();
    wParent->setLayout(mpMapLayout);
    
    QMenuBar * mb = this->menuBar();
    if(mb)
        mpMapLayout->setMenuBar(mb);
    
    QHBoxLayout * mpToolBarLayout = new QHBoxLayout();
    QMap<QString, QToolBar *> mpToolBars = toolBarMap();
    mpToolBarLayout->addWidget(mpToolBars.value("mFileToolBar"));
    mpToolBarLayout->addWidget(mpToolBars.value("mMapNavToolBar"));
    mpToolBarLayout->addWidget(mpToolBars.value("mLayerToolBar"));
    mpToolBarLayout->addWidget(mpToolBars.value("mDigitizeToolBar"));
    mpToolBarLayout->addWidget(mpToolBars.value("mAttributesToolBar"));
    QSpacerItem * sp = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mpToolBarLayout->addSpacerItem(sp);
    
    mpMapLayout->addLayout(mpToolBarLayout);

    mpMapLayout->addWidget(this);

    QWidget * w = this->statusBar();
    if(w){
        w->setFixedHeight(30);
        mpMapLayout->addWidget(w);
    }

    mpMapLayout->setContentsMargins(0, 0, 0, 0);
    mpMapLayout->setMargin(0);


    m_legendWidget = this->mapLegendDockWidget();
    if(m_legendWidget){
        //m_legendWidget->setParent(this);
    }

    //ksa
    /*
    if(legend){
        m_legendWidget = new QDockWidget( tr( "Layers" ), this );
        m_legendWidget->setObjectName( "Legend" );
        m_legendWidget->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
        m_legendWidget->setWidget( legend );
        //addDockWidget( Qt::LeftDockWidgetArea, mLegendDock );
    }
    */
    //ksa
    
    m_layerOrderWidget = this->mapLayerOrderDockWidget();
    if(m_layerOrderWidget){
        //m_layerOrderWidget->setParent(this);
        m_layerOrderWidget->hide();
    }
    
    //ksa
    /*
    if(layerOrder){
        m_layerOrderWidget = new QDockWidget( tr( "Layer order" ), this );
        m_layerOrderWidget->setObjectName( "LayerOrder" );
        m_layerOrderWidget->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
        m_layerOrderWidget->setWidget( layerOrder );
        //addDockWidget( Qt::LeftDockWidgetArea, mLayerOrderDock );
        m_layerOrderWidget->hide();
    }
    */
    //ksa

    //setWindowTitle(this->windowTitle());

    openProject(); //открываем проект, который задан в качестве значения атрибута
    
    connect(this, SIGNAL(projectRead()), this, SLOT(slotDataChanged()));
    connect(this, SIGNAL(mapSaved(QDomDocument &)), this, SLOT(slotMapChanged(QDomDocument&)));
    connect(this, SIGNAL(dataChanged()), this, SLOT(slotDataChanged()));
    
}

#endif


int KKSMapWidget::openProject()//открываем проект, заданный в качестве значения атрибута
{
#ifdef __USE_QGIS__ 
	
    QString xmlPrj; //здесь будет XML-описание проекта (для существующих ИО, ЭИО)

    if(!m_av || m_idObj <= 0){
        ;//return -1;
    }
    else{
        KKSValue v = m_av->value();
        
        xmlPrj = v.value();
        if(xmlPrj.isEmpty())
            return -1;
    }


    QString homeDir = m_GISHomeDir;
    QDir d(homeDir);
    if(!d.exists()){
        if(!d.mkdir(homeDir)){
            QMessageBox::critical(this, tr("Error"), tr("QGIS temporary files home dir does not exist! Cannot create that! Path = %1").arg(homeDir));
            return -1;
        }
    }
    
    //d.cd(homeDir);
    
    qint64 id = m_idObj;
    if(id <= 0)
        id = 0; //для новых ИО (ЭИО)

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
    
    if(id > 0){ //для существующих ИО (ЭИО) - записываем содержимое проекта (т.е. значение KKSValue) в файл
        QFile f(path);
        
        bool ok = f.open(QIODevice::ReadWrite);
        if(!ok){
            QMessageBox::critical(this, tr("Error"), tr("Cannot create file for storing QGIS project file (%1)!").arg(path));
            return -1;
        }

        //перед тем, как записать содержимое проекта, надо обновить datasource для всех postgis-слоев
        xmlPrj = updateProjectXML(xmlPrj, false);

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

        KKSGISWidget::openProject(path);
        KKSGISWidget::refreshMapCanvasOnly();
    }
    else{//для новых ИО (ЭИО) - сохраняем начальный пустой проект в указанный файл
        saveProjectAs(path);
    }

    return 1;
    
#else
	return -1;
#endif
}

//выгрузка на клиент файлов со слоями, если слои представлены файлами (т.е. не PostGIS)
int KKSMapWidget::downloadLayers(const QString & homeDir, const QString & xmlPrj)
{
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


void KKSMapWidget::slotDataChanged()
{
    QString xml = readGISProjectFile();
    if(xml.isEmpty())//Файла проекта нет. Значит проект еще не создан
        return;

    //необходимо заменить пути к файлам слоев в тэге datasource на относительные вида ./filename. 
    //Т.е. полагаем, что все файлы находятся в том же каталоге, что и файл проекта
    xml = updateProjectXML(xml, true);

    KKSValue v(xml, KKSAttrType::atGISMap);
    if(!v.isValid()){
        QMessageBox::critical(this, tr("Error"), tr("Cannot set attribute value for GIS attribute!"));
        return;
    }

    emit valueChanged(m_av->id(), m_isSystem, v.valueVariant());

    m_dataChanged = true;
}

void KKSMapWidget::slotMapChanged(QDomDocument & prjFile)
{
    Q_UNUSED(prjFile); //там данные все равно не корректные. Нет слоев
    
    slotDataChanged();
}

bool KKSMapWidget::removeQGISPrjDir(const QString & path) 
{
    QDir dir(path); 
    QStringList files = dir.entryList(QDir::Files);

    QStringList::Iterator itFile = files.begin(); 
    while (itFile != files.end()) 
    {
        QFile file(path + "/" + *itFile);
        bool ok = file.remove();
        if(!ok){
            int a=0;
        }
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


//для слоев, представленных файлами, получаем список этих самых файлов (полностью отдельные url-ы)
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
        
        mnl = layer.namedItem( "datasource" );
        mne = mnl.toElement();
        dataSource = mne.text();

        if(provider == "spatialite" || provider == "wms"){
            continue;//эти источники представлены ресурсами БД в сети, а не файлами
        }
        else if(provider == "postgres"){
            ;
        }
        else{

            QString fileAbsUrl;

            if(provider == "ogr"){
                QStringList theURIParts = dataSource.split( "|" );
                fileAbsUrl = this->readLayerFilePath( theURIParts[0] );//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
            }
            else if ( provider == "delimitedtext" ){
                //???????????
            }
            else{ //rasters ??
                fileAbsUrl = this->readLayerFilePath( dataSource );//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
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
        }//datasource in files
    }

#endif

    return files;
}


//второй параметр если true, то XML обновляется для записи в БД.
//в этом случае меняем URI postgis-слоев так, чтобы там не было user и password
//а также URI файловых слоев, чтобы там были только относительные ссылки на файлы, причем на текущий каталог (т.е. ./ )
//если параметр = false, XML обновляется для открытия файла проекта в QGIS
//в этом случае меняем URI postgis-слоев так, чтобы datasource содержал параметры соединения к БД, с которой соединен DynamicDocs Client
QString KKSMapWidget::updateProjectXML(const QString & xml, bool bForWriteToDb) const
{
    QString updatedXML;

#ifdef __USE_QGIS__

    QDomDocument d;
    d.setContent(xml);
    QDomNode qgis = d.namedItem("qgis");
    QDomNode oldQgis(qgis);
    QDomNode prjLayers = qgis.namedItem("projectlayers");
    QDomNode oldPrjLayers(prjLayers);

    QDomNodeList mapLayers = prjLayers.childNodes(); //maplayer
    int cnt = mapLayers.length();
    if(cnt <= 0)
        return updatedXML;
    
    for(int index = 0; index<cnt; index++){
        QDomNode layer = mapLayers.item(index);
    
        QDomNode mnl;
        QDomElement mne;

        QString provider;
        QString dataSource;
        
        mnl = layer.namedItem("provider");
        mne = mnl.toElement();
        provider = mne.text();
        
        mnl = layer.namedItem( "datasource" );
        mne = mnl.toElement();
        dataSource = mne.text();

        if(provider == "spatialite" || provider == "wms"){
            continue;//эти источники представлены ресурсами БД в сети, а не файлами
        }
        else if(provider == "postgres"){
        
            QString updatedUri;
            QStringList uriSections = dataSource.split(" ");
            
            for(int i=0; i<uriSections.count(); i++){
                QString & sec = uriSections[i];
                if(sec.startsWith("dbname=")){
                    QString val = m_connectionParams.value("dbname");
                    if(!val.isEmpty())
                        uriSections[i] = QString("dbname=%1").arg(val); 
                }
                else if(sec.startsWith("host=")){
                    QString val = m_connectionParams.value("host");
                    if(!val.isEmpty())
                        uriSections[i] = QString("host=%1").arg(val); 
                }
                else if(sec.startsWith("port=")){
                    QString val = m_connectionParams.value("port");
                    if(!val.isEmpty())
                        uriSections[i] = QString("port=%1").arg(val); 
                }
                else if(sec.startsWith("user=")){
                    QString val;
                    if(!bForWriteToDb)
                        val = m_connectionParams.value("user");//т.е. читаем проект
                    uriSections[i] = QString("user=%1").arg(val); 
                }
                else if(sec.startsWith("password=")){
                    QString val;
                    if(!bForWriteToDb)
                        val = m_connectionParams.value("password");//т.е. читаем проект
                    uriSections[i] = QString("password=%1").arg(val); 
                }
            }
            
            updatedUri = uriSections.join(" ");            
            
            // datasource
            QDomElement dsNode = d.createElement( "datasource" );
            QDomText dsText = d.createTextNode( updatedUri );
            dsNode.appendChild( dsText);
            mapLayers.item(index).replaceChild(dsNode, mnl);
            
            continue;
        }
        else{    

            if(!bForWriteToDb)
                continue;//при чтении проекта не надо обновлять пути к файлам

            QString fileAbsUrl;

            if(provider == "ogr"){
                QStringList theURIParts = dataSource.split( "|" );
                fileAbsUrl = this->readLayerFilePath( theURIParts[0] );//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
                
                QFileInfo fi(fileAbsUrl);
                theURIParts[0] = QString("./") + fi.fileName();
                dataSource = theURIParts.join("|");
            }
            //else if ( provider == "delimitedtext" ){
                //???????????
            //}
            else{ //rasters ??
                fileAbsUrl = this->readLayerFilePath( dataSource );//возвращает абсолютный путь к файлу слоя, который прочитан из файла проекта QGIS
                QFileInfo fi(fileAbsUrl);
                dataSource = QString("./") + fi.fileName();
            }

            // datasource
            QDomElement dsNode = d.createElement( "datasource" );
            QDomText dsText = d.createTextNode( dataSource );
            dsNode.appendChild( dsText);
            mapLayers.item(index).replaceChild(dsNode, mnl);
        }

    }

    //prjLayers.replaceChild(mapLayers, oldMapLayers);
    qgis.replaceChild(prjLayers, oldPrjLayers);
    d.replaceChild(qgis, oldQgis);

    updatedXML = d.toString();
    //qWarning() << updatedXML;

    //QFile f("./1111.xml");
    //f.open(QIODevice::WriteOnly);
    //f.write(updatedXML.toLocal8Bit());
    //f.close();

#endif
    return updatedXML;
}

QString KKSMapWidget::readGISProjectFile()
{
    QString xml;

#ifdef __USE_QGIS__

    QString prjFile = this->projectFileName();
    if(prjFile.isEmpty())// доверяем QGIS'у. Если файл проекта пустой, то это означает, что проект еще пуст
        return xml;

    QFile f(prjFile);
    bool ok = f.open(QIODevice::ReadOnly);
    if(!ok){
        QMessageBox::critical(this, tr("Error"), tr("Cannot open project file! Path = %1").arg(prjFile));
        return xml;
    }

    xml = f.readAll();
    f.close();

#endif

    return xml;
}

void KKSMapWidget::slotSaveGISProject(KKSValue & v)
{
    
#ifdef __USE_QGIS__

    disconnect(this, SIGNAL(mapSaved(QDomDocument &)), this, SLOT(slotMapChanged(QDomDocument&)));
    
    saveProject();

    QString xml = readGISProjectFile();
    if(xml.isEmpty())//Файла проекта нет. Значит проект еще не создан
        return;

    //необходимо заменить пути к файлам слоев в тэге datasource на относительные вида ./filename. 
    //Т.е. полагаем, что все файлы находятся в том же каталоге, что и файл проекта
    xml = updateProjectXML(xml, true);
    
    connect(this, SIGNAL(mapSaved(QDomDocument &)), this, SLOT(slotMapChanged(QDomDocument&)));
    
    v = KKSValue(xml, KKSAttrType::atGISMap);
    if(!v.isValid()){
        QMessageBox::critical(this, tr("Error"), tr("Cannot set attribute value for GIS attribute!"));
        return;
    }
#endif
}

#ifdef __USE_QGIS__
void KKSMapWidget::slotFeatureFromEIO(QWidget * parent, QgsFeature & feature, const QString & geomAsEWKT, const QString & layerTable)
{
    if(!parent)
        return;
    if(layerTable.isEmpty())
        return;
    
    KKSObjectExemplar * eio = NULL;

    emit constructNewEIO(parent, &eio, layerTable, geomAsEWKT);

    if(!eio)
        return;

    const QgsFields * fields = feature.fields();
    int cnt = fields->count();
    for(int i=0; i<cnt; i++){
        QgsField field = fields->at(i);
        QString fName = field.name();
        feature.attribute(fName);
        QVariant avValue;
        
        if(fName == "unique_id")
            avValue = QVariant(eio->uniqueId());
        else if(fName == "last_update")
            avValue = QVariant(eio->lastUpdate());
        else if(fName == "id")
            avValue = QVariant(eio->id());
        else if(fName == "uuid_t")
            avValue = QVariant(eio->uuid());
        else if(fName == "id_io_state")
            avValue = QVariant(eio->state()->id());
        else if(fName == "r_icon")
            avValue = QVariant(eio->iconAsString());
        else if(fName == "record_fill_color")
            avValue = QVariant(eio->recordFillColor());
        else if(fName == "record_text_color")
            avValue = QVariant(eio->recordTextColor());
        else if(fName == "rr_name")
            avValue = QVariant(eio->name());
        else{ //работаем с обычными несистемными атрибутами
            int avIndex = -1;
            int avCnt = eio->attrValues().count();
            
            for(int j=0; j<avCnt; j++){
                KKSAttrValue * av = eio->attrValues().at(j);
                KKSCategoryAttr * ca = av->attribute();
                QString attrCode = ca->code();
                if(attrCode == fName){
                    avIndex = j;
                    break;
                }
            }

            KKSAttrValue * av = eio->attrValue(avIndex);
            if(!av)
                continue;

            avValue = av->value().valueVariant();
        }

        feature.setAttribute(fName, avValue);
    }

    eio->release();

}

void KKSMapWidget::slotDeleteFeaturesAsEIO(QWidget * parent, const QString & tableName, const QList<qint64> & ids)
{
    emit deleteEIOList(parent, tableName, ids);
}

#endif
