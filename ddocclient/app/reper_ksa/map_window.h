#ifndef MAPWINDOW_H
#define MAPWINDOW_H


//QT Includes
#include <QtGui>
#include <QMessageBox>
#include <QFile>
#include <QPainter>
#include <QApplication>

//Local Includes
#include <ui_map_window_base.h>
#include "kksgiswidget.h"

class MapWindow : public QMainWindow, private Ui::MapWindowBase
{
  Q_OBJECT;
public:

    MapWindow(QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~MapWindow();

public slots:
    void SLOTmpActionFileExit(); // Exit from Application

private:
    KKSGISWidget * mpKKSGISWidget;
};




#endif
