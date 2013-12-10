#ifndef KKSBADLAYERHANDLER_H
#define KKSBADLAYERHANDLER_H

#include "kksgis_config.h"

#include <qgsproject.h>

class _GIS_EXPORT KKSBadLayerHandler : public QgsProjectBadLayerHandler
{
  public:
    virtual void handleBadLayers( QList<QDomNode> layers, QDomDocument projectDom );
};



#endif
