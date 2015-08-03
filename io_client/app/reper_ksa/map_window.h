#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <kksqgis_config.h>


//QT Includes
#include <QtGui>
#include <QMessageBox>
#include <QFile>
#include <QPainter>
#include <QApplication>

//Local Includes
#include <ui_map_window_base.h>
#include "kksgiswidget.h"

//class KKSGISWidget;

class MapWindow : public QMainWindow, private Ui::MapWindowBase
{
  Q_OBJECT
public:

    MapWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~MapWindow();

public slots:
    void SLOTmpActionFileExit(); // Exit from Application
    void SLOTmpActionChangeTitle(); // Change window title to add name of current project

private:
    KKSGISWidget * mpKKSGISWidget;
};




#endif
