
#include "kksgiswidget.h"
#ifdef __USE_QGIS__
#include "kksgiswidgetqgis.h"
#endif

//KKSGISWidget * KKSGISWidget::smInstance = 0;

KKSGISWidget * KKSGISWidget::initQGISWidget(bool withSubWindows, QWidget* parent, Qt::WFlags fl)
{
#ifndef __USE_QGIS__
    return NULL;
#else
    KKSGISWidget * w = new KKSGISWidgetQGIS(withSubWindows, parent, fl);
    return w;
#endif
}

void KKSGISWidget::destroyQGISWidget(KKSGISWidget * w)
{
#ifndef __USE_QGIS__
    return;
#else
    if(w)
        delete w;
#endif
}


KKSGISWidget::KKSGISWidget(QWidget* parent, Qt::WFlags fl ) : QWidget(parent, fl)
{

}

