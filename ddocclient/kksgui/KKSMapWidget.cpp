/***********************************************************************
 * Module:  KKSMapWidget.cpp
 * Author:  sergey
 * Modified: 13 декабря 2013 г. 14:49:21
 * Purpose: Implementation of the class KKSMapWidget
 ***********************************************************************/

#include "KKSMapWidget.h"

#include <QtDebug>
#include "KKSAttrValue.h"
#include "defines.h"

#ifdef __USE_QGIS__
#include "kksgiswidget.h"
#endif

KKSMapWidget::KKSMapWidget(const KKSAttrValue* attr, KKSIndAttrClass isSys, QWidget *parent)
    : QWidget(parent), 
      KKSAttrWidget(attr, isSys),
      m_legendWidget(0),
      m_layerOrderWidget(0),
      mpKKSGISWidget(0)
{
#ifdef __USE_QGIS__
    init(parent);
#endif
}

KKSMapWidget::~KKSMapWidget()
{
    if(m_legendWidget && m_layerOrderWidget)
        emit aboutToDestroy(m_legendWidget, m_layerOrderWidget);
    
    m_legendWidget = 0;
    m_layerOrderWidget = 0;

#ifdef __USE_QGIS__
    if(mpKKSGISWidget)
        KKSGISWidget::destroyQGISWidget(mpKKSGISWidget);
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


#ifdef __USE_QGIS__

void KKSMapWidget::init(QWidget *parent)
{
    bool withSubWindows = false;

    mpKKSGISWidget = KKSGISWidget::initQGISWidget(withSubWindows, parent);
    if(!mpKKSGISWidget){
        QMessageBox::critical(0, "", "");
        return;
    }

    QVBoxLayout * mpMapLayout = new QVBoxLayout(this); //main Layout
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
        /**/ //addDockWidget( Qt::LeftDockWidgetArea, mLegendDock );
    }
    
    QWidget * layerOrder = mpKKSGISWidget->mapLayerOrderWidget();
    if(layerOrder){
        m_layerOrderWidget = new QDockWidget( tr( "Layer order" ), this );
        m_layerOrderWidget->setObjectName( "LayerOrder" );
        m_layerOrderWidget->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
        m_layerOrderWidget->setWidget( layerOrder );
        /**/ //addDockWidget( Qt::LeftDockWidgetArea, mLayerOrderDock );
        m_layerOrderWidget->hide();
    }

    setWindowTitle(mpKKSGISWidget->windowTitle());
}

#endif