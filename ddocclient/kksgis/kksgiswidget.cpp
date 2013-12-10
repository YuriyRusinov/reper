
#include "kksgiswidget.h"
#ifdef __USE_QGIS__
#include "kksgiswidgetqgis.h"
#endif

KKSGISWidget * KKSGISWidget::initQGISWidget(QWidget* parent, Qt::WFlags fl)
{
#ifndef __USE_QGIS__
    return NULL;
#else
    KKSGISWidget * w = new KKSGISWidgetQGIS(parent, fl);
    return w;
#endif
}


KKSGISWidget::KKSGISWidget(QWidget* parent, Qt::WFlags fl ) : QWidget(parent, fl)
{

}

