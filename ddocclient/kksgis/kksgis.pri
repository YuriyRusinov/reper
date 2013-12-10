HEADERS += \
                kksgiswidget.h \
                kksbadlayerhandler.h \
                kksgis_config.h \
                dn/imagearea.h \
                dn/dnspecbath.h \
                dn/Added/dnvector.h \
                dn/Added/dnpoly.h \
                dn/Added/dnimagelib.h \
                dn/Added/First/structs.h \
                dn/Added/First/dntheam.h \
                dn/Added/First/dnmathadd.h \
                dn/Added/First/dncalcstring.h \
                dn/dnwidgetimage.h

SOURCES += \
                kksgiswidget.cpp \
                kksbadlayerhandler.cpp \
                dn/imagearea.cpp \
                dn/dnspecbath.cpp \
                dn/Added/dnvector.cpp \
                dn/Added/dnpoly.cpp \
                dn/Added/dnimagelib.cpp \
                dn/Added/First/dntheam.cpp \
                dn/Added/First/dnmathadd.cpp \
                dn/Added/First/dncalcstring.cpp \
                dn/dnwidgetimage.cpp

FORMS += \
                dn/dnspecbath.ui \
                dn/DlgShowDeptch.ui

RESOURCES += \
                resources.qrc

qgis{
    HEADERS += \
                kksgiswidgetqgis.h

    SOURCES += \
                kksgiswidgetqgis.cpp \
                kksgiswidgetqgishelper.cpp
}
