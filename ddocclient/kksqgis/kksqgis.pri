# Input
HEADERS += \
                KKSGISWidget.h \
                kksgiswidgetbase.h \
                kksbadlayerhandler.h \
	        kksqgis_config.h

SOURCES += \
                KKSGISWidget.cpp \
                kksbadlayerhandler.cpp \
                kksgiswidgetbase.cpp

RESOURCES += \
                resources.qrc \
                images/images.qrc

headers.files += \
                KKSGISWidget.h \
                kksgiswidgetbase.h \
                kksbadlayerhandler.h \
                kksqgis_config.h 


include (legend/legend.pri)
include (dn/dn.pri)
include (layerprops/layerprops.pri)
include (projectprops/projectprops.pri)
include (maptools/maptools.pri)
