/***********************************************************************
 * Module:  KKSMapWidget.h
 * Author:  sergey karin
 * Modified: 07 декабря 2013 г. 14:46:21
 * Purpose: Declaration of the class KKSMapWidget
 * виджет для работы с картой
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSMapWidget_h)
#define __KKSSITOOOM_KKSMapWidget_h

#include "kksgui_config.h"

#include <QObject>
#include <KKSAttrWidget.h>
#include <QString>
#include <QList>
#include <QListWidget>
#include <QVariant>
#include <QDockWidget>
#include <QDomDocument>


class KKSAttrValue;
class KKSFile;
class KKSObjectExemplar;

#include <KKSValue.h>

#ifdef __USE_QGIS__
#include "KKSGISWidget.h"
#include <qgsfeature.h>
#include <qgsgeometry.h>
//#include "kksgiswidgetqgis.h"
//class KKSGISWidget;
#endif

#ifdef __USE_QGIS__
class _GUI_EXPORT KKSMapWidget : public KKSGISWidget, public KKSAttrWidget
#else
class _GUI_EXPORT KKSMapWidget : public QWidget, public KKSAttrWidget
#endif
{
    Q_OBJECT
public:
    KKSMapWidget(QMap<QString, QString> connectionParams,
                 qint64 idObj, 
                 const QString & gisHomeDir, 
                 const KKSAttrValue* attr = 0, 
                 KKSIndAttrClass isSys = iacIOUserAttr, 
                 QWidget *parent=NULL);

    virtual ~KKSMapWidget(void);

    QDockWidget * legendWidget() const;
    QDockWidget * layerOrderWidget() const;
    void init();

signals:
    void valueChanged (qint64 id, KKSIndAttrClass sys, QVariant val);
    void aboutToDestroy(QDockWidget *, QDockWidget *);
    void downloadGISFiles(bool bForRec, const QString & homeDir, qint64 idObj, QWidget * parent);//запрос на выгрузку файла со слоем из БД. файл задается идентификатором из таблицы io_urls
    void uploadGISFiles(bool bForRec, const QStringList & files, qint64 idObj, QWidget * parent);//загрузка файлов ГИС-проекта на сервер. Файлы задаются абсолютными путями
    void constructNewEIO(QWidget * parent, KKSObjectExemplar ** eio, const QString & layerTable, const QString & geomAsEWKT); //запрос на создание нового EИО в справочнике, который задается названием своей таблицы в БД. При этом создавать ИО в бД не надо
    void deleteEIOList(QWidget * parent, const QString & tableName, const QList<qint64> & ids);
    
public slots:
    void slotMapChanged(QDomDocument&);
    void slotDataChanged();
    void slotUploadGISFiles(qint64 idObj);
    void slotSaveGISProject(KKSValue & v);
#ifdef __USE_QGIS__
    void slotFeatureFromEIO(QWidget * parent, QgsFeature & feature, const QString & geomAsEWKT, const QString & layerTable);
    void slotDeleteFeaturesAsEIO(QWidget * parent, const QString & tableName, const QList<qint64> & ids);
#endif
private:
    QDockWidget * m_legendWidget;
    QDockWidget * m_layerOrderWidget;
    QString m_GISHomeDir;
    bool m_dataChanged;
    qint64 m_idObj;

    int openProject();//открываем проект, заданный в качестве значения атрибута
    int downloadLayers(const QString & homeDir, const QString & xmlPrj);//выгрузка на клиент файлов со слоями, если слои представлены файлами (т.е. не PostGIS)
    bool removeQGISPrjDir(const QString & path);
    QStringList readGISProjectLayerFiles(const QString & xml) const;
    QString updateProjectXML(const QString & xml, 
                             bool bForWriteToDb) const; //второй параметр если true, то XML обновляется для записи в БД.
                                                        //в этом случае меняем URI postgis-слоев так, чтобы там не было user и password
                                                        //а также URI файловых слоев, чтобы там были только относительные ссылки на файлы, причем на текущий каталог (т.е. ./ )
                                                        //если параметр = false, XML обновляется для открытия файла проекта в QGIS
                                                        //в этом случае меняем URI postgis-слоев так, чтобы datasource содержал параметры соединения к БД, с которой соединен DynamicDocs Client
    QString readGISProjectFile();

    QMap<QString, QString> m_connectionParams;//используется для подмены URI postgis-слоев

private:
    void initQGIS();

};

#endif
