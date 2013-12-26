#if !defined(__KKSSITOOOM_KKSGISWidget_h)
#define __KKSSITOOOM_KKSGISWidget_h

#include "kksqgis_config.h"
#include "kksgiswidgetbase.h"

#include <QObject>
#include <QWidget>
#include <QString>
#include <QList>
#include <QListWidget>
#include <QVariant>
#include <QDockWidget>
#include <QDomDocument>

class _QGIS_EXPORT KKSGISWidget : public KKSGISWidgetBase
{
    Q_OBJECT
public:
    KKSGISWidget(bool withSubWindows, bool withAddons = true, QWidget* parent = 0, Qt::WFlags fl = 0 );

    virtual ~KKSGISWidget(void);

};

#endif
