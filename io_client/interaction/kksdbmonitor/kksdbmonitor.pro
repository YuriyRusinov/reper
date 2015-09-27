#dyndoc_synch
TEMPLATE = app
TARGET = kksdbmonitor
DESTDIR = ../../build

include(../../io_client.conf)
QT += network

INCLUDEPATH += .\
                ../../corelibs/dataaccess \
                ../../corelibs/kkspp \
                ../../corelibs/kksdatafactory \
                ../../corelibs/kksutils \
                ../../corelibs/kkspluginmanager \
                ../../mgrlibs/kkscoreapplication/base_classes \
                ../../mgrlibs/kkscoreapplication/ui \
                ../../mgrlibs/kkscoreapplication \
                ../kksinteractor \
                ../kkshttpconnector \
                ../sync_queue_viewer 

DEPENDPATH += 	. \
                ../../corelibs/dataaccess \
                ../../corelibs/kkspp \
                ../../corelibs/kksdatafactory \
                ../../corelibs/kksutils \
                ../../corelibs/kkspluginmanager \
                ../../mgrlibs/kkscoreapplication \
                ../kksinteractor \
                ../kkshttpconnector \
                ../sync_queue_viewer 

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkscoreapplication -lkksdatafactory -lkkspp -lkksinteractor -lkkshttpconnector -lsync_queue_viewer -lkkspluginmanager



# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS += -lpq

include(kksdbmonitor.pri)

target.path = $$KKS_EXEC_DIR
INSTALLS += target
