#dyndoc_synch
TEMPLATE = app
TARGET = kksdbmonitor
DESTDIR = ../../build

include(../../ddocclient.conf)
QT += network

INCLUDEPATH += .\
                ../../dataaccess \
                ../../kkscore/base_classes \
                ../../kkscore \
                ../../kkspp \
                ../../kksfactory \
                ../../kksutils \
                ../../kksinteractor \
                ../../kkshttpconnector \
                ../../sync_queue_viewer \
                ../../kksqwt/src 

DEPENDPATH += 	. \
                ../../dataaccess \
                ../../kkscore \
                ../../kkscore/base_classes \
                ../../kkspp \
                ../../kksfactory \
                ../../kksutils \
                ../../kksinteractor \
                ../../kkshttpconnector \
                ../../sync_queue_viewer \
                ../../kksqwt

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkscore -lkksfactory -lkkspp -lkksgui -lkksinteractor -lkkshttpconnector -lsync_queue_viewer -lkksqwt -lkkspluginmanager



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
