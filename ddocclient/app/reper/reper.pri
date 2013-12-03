HEADERS += \
                simple_map_window.h \
                config.h \
                dn/ui_mainwindow.h \
                dn/ui_dnspecbath.h \
                dn/imagearea.h \
                dn/dnspecbath.h \
    dn/Added/dnwidgetimage.h \
    dn/Added/dnvector.h \
    dn/Added/dnpoly.h \
    dn/Added/dnimagelib.h \
    dn/Added/First/structs.h \
    dn/Added/First/dntheam.h \
    dn/Added/First/dnmathadd.h \
    dn/Added/First/dncalcstring.h \
    dn/dnwidgetimage.h
SOURCES += \
		simple_map_window.cpp \
		junk.cpp \
		dn/imagearea.cpp \
		dn/dnspecbath.cpp \
    main.cpp \
    dn/Added/dnvector.cpp \
    dn/Added/dnpoly.cpp \
    dn/Added/dnimagelib.cpp \
    dn/Added/First/dntheam.cpp \
    dn/Added/First/dnmathadd.cpp \
    dn/Added/First/dncalcstring.cpp \
    dn/dnwidgetimage.cpp
FORMS += \
                simple_map_window_base.ui \
    dn/dnspecbath.ui

RESOURCES += \
                resources.qrc

INCLUDEPATH += C:/OSGeo4W/include \
               dn/

# c:/OSGeo4W/apps/qgis-dev/include \
#LIBPATH += c:/OSGeo4W/lib

#LIBS += -Lc:/OSGeo4W/lib/ -lgdal_i

#LIBPATH += C:/OSGeo4W/lib/

#LIBS += -LC:/OSGeo4W/lib/  -lqgis_core -lproj -lqgis_gui -lgdal_i
LIBS += -LC:/OSGeo4W/lib/  -lproj -lgdal_i

