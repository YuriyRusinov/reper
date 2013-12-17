
#ifndef KKSGISWIDGET_H
#define KKSGISWIDGET_H


#include "kksgis_config.h"

//QT Includes
#include <QtGui>

class _GIS_EXPORT KKSGISWidget : public QWidget
{
  Q_OBJECT;
public:
    static KKSGISWidget * initQGISWidget(bool withSubWindows, QWidget* parent = 0, Qt::WFlags fl = 0);
    static void destroyQGISWidget(KKSGISWidget *);
    //static KKSGISWidget * instanse();

    virtual QStatusBar * statusBar() const = 0;
    virtual const QString & pluginsDir() = 0;
    //virtual QTableWidget * tableLegend() = 0;
    virtual QWidget * mapLegendWidget() = 0;
    virtual QWidget * mapLayerOrderWidget() = 0;
    virtual QToolBar * toolBar() const = 0;
    virtual QMenuBar * menuBar() const = 0;
    virtual const QMap<QString, QMenu *> & menuMap() const = 0;

    virtual QString projectFileName() = 0;

    virtual void openProject(const QString & prjFile) = 0;
    virtual void closeProject() = 0;
    virtual void saveProjectAs(const QString & prjFile) = 0;
    virtual void saveProject() = 0;
    virtual void openVectorLayer(const QString & vLayerFile) = 0;
    virtual void openRasterLayer(const QString & rLayerFile) = 0;
    virtual void openDatabaseLayer() = 0;

signals:
    void SIGNALchangeWindowTitle();

protected:
    KKSGISWidget(QWidget* parent = 0, Qt::WFlags fl = 0 );
    virtual ~KKSGISWidget(){}
};

#endif

