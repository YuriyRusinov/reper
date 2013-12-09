#ifndef KKSBADLAYERHANDLER_H
#define KKSBADLAYERHANDLER_H

#include "config.h"

#include <qgsproject.h>

class KKSBadLayerHandler : public QgsProjectBadLayerHandler
{
  public:
    virtual void handleBadLayers( QList<QDomNode> layers, QDomDocument projectDom );
};



#endif
