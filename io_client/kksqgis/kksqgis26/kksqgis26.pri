# Input
HEADERS += \
                KKSGISWidget.h \
                kksgiswidgetbase.h \
                kksbadlayerhandler.h \
                qgshandlebadlayers.h \
                qgisapp.h \
                qgisappinterface.h \
	        kksqgis_config.h

FORMS += \
                qgshandlebadlayersbase.ui \

SOURCES += \
                KKSGISWidget.cpp \
                kksbadlayerhandler.cpp \
                qgshandlebadlayers.cpp \
                qgisapp.cpp \
                qgisappinterface.cpp \
                kksgiswidgetbase.cpp

RESOURCES += \
                resources.qrc \
                images/images.qrc

headers.files += \
                KKSGISWidget.h \
                kksgiswidgetbase.h \
                kksbadlayerhandler.h \
                qgshandlebadlayers.h \
                qgisapp.h \
                qgisappinterface.h \
                kksqgis_config.h 


include (legend/legend.pri)

win32{
    include (dn/dn.pri)
}

include (layerprops/layerprops.pri)
include (projectprops/projectprops.pri)
include (maptools/maptools.pri)
include (pluginmanager/pluginmanager.pri)
