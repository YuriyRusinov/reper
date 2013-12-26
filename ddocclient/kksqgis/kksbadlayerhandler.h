#ifndef KKSBADLAYERHANDLER_H
#define KKSBADLAYERHANDLER_H

#include "kksqgis_config.h"

#include <qgsproject.h>

class _QGIS_EXPORT KKSBadLayerHandler : public QgsProjectBadLayerHandler
{
  public:
    virtual void handleBadLayers( QList<QDomNode> layers, QDomDocument projectDom );
};



#endif
