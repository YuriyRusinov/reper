
#include "kksgiswidget.h"

#ifdef __USE_QGIS__
#include "kksgiswidgetqgis.h"
#endif

_GIS_EXPORT KKSGISWidget * KKSGISWidget::smInstance = 0;

KKSGISWidget * KKSGISWidget::initQGISWidget(bool withSubWindows, QWidget* parent, Qt::WFlags fl)
{
#ifndef __USE_QGIS__
    return NULL;
#else
    if(smInstance)
        return smInstance;

    KKSGISWidget * w = new KKSGISWidgetQGIS(withSubWindows, parent, fl);
    smInstance = w;
    
    return w;
#endif
}

KKSGISWidget * KKSGISWidget::instance()
{
    return KKSGISWidget::smInstance;
}

void KKSGISWidget::destroyQGISWidget(KKSGISWidget * w)
{
//#ifndef __USE_QGIS__
//    return;
//#else
    if(w)
        delete w;
    w = 0;
    smInstance = 0;
//#endif
}


KKSGISWidget::KKSGISWidget(QWidget* parent, Qt::WFlags fl ) : QWidget(parent, fl)
{

}

