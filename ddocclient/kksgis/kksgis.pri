HEADERS += \
                kksgiswidget.h \
                kksbadlayerhandler.h \
                kksgis_config.h 

SOURCES += \
                kksgiswidget.cpp \
                kksbadlayerhandler.cpp 

RESOURCES += \
                resources.qrc

qgis{
    HEADERS += \
                kksgiswidgetqgis.h

    SOURCES += \
                kksgiswidgetqgis.cpp \
                kksgiswidgetqgishelper.cpp
}

include(legend/legend.pri)
include(dn/dn.pri)
