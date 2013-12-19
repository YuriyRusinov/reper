/***********************************************************************
 * Module:  KKSMapWidget.h
 * Author:  sergey
 * Modified: 13 декабря 2013 г. 14:46:21
 * Purpose: Declaration of the class KKSMapWidget
 * виджет для работы с картой
 ***********************************************************************/

#if !defined(__KKSSITOOOM_KKSMapWidget_h)
#define __KKSSITOOOM_KKSMapWidget_h

#include "kksgui_config.h"

#include <KKSAttrWidget.h>
#include <QString>
#include <QList>
#include <QListWidget>
#include <QVariant>
#include <QDockWidget>
#include <QDomDocument>


class KKSAttrValue;
class KKSFile;

#ifdef __USE_QGIS__
class KKSGISWidget;
#endif

class _GUI_EXPORT KKSMapWidget : public QWidget, public KKSAttrWidget
{
    Q_OBJECT
public:
    KKSMapWidget(qint64 idObj, 
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
public slots:
    void slotMapChanged(QDomDocument&);
    void slotDataChanged();
    void slotMapOpened();
    void slotUploadGISFiles(qint64 idObj);
private:
    QDockWidget * m_legendWidget;
    QDockWidget * m_layerOrderWidget;
    QString m_GISHomeDir;
    bool m_dataChanged;
    qint64 m_idObj;

    int openProject();//открываем проект, заданный в качестве значения атрибута
    int downloadLayers(const QString & homeDir, const QString & xmlPrj);//выгрузка на клиент файлов со слоями, если слои представлены файлами (т.е. не PostGIS)
    QString getFileNameForDS(const QString & path, const QString & provider);//берем часть, которая представляет собой название файла, из datasource слоя
    //qint64 getIdUrlForID(const QString & id);//Берем часть, которая представляет собой идентификатор файла в таблице io_urls, из id слоя
    bool removeQGISPrjDir(const QString & path);
    QStringList readGISProjectLayerFiles(const QString & xml) const;
    QString readGISProjectFile();

#ifdef __USE_QGIS__

private:
    void initQGIS(QWidget *parent);

private:
    KKSGISWidget * mpKKSGISWidget;
#endif

};

#endif
