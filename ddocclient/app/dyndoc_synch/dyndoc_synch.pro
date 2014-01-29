#dyndoc_synch

TEMPLATE = app
TARGET = dyndoc_synch

DESTDIR = ../../build
include(../../ddocclient.conf)

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

LIBS += -L/usr/local/kkssito/lib\
        -L$$DESTDIR -ldataaccess -lkksutils -lkkscore -lkksfactory -lkkspp -lkksgui -lkksinteractor -lkkshttpconnector -lsync_queue_viewer -lkksqwt

# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS *= -lpq

CONFIG += warn_on dll

DEFINES += DYNDOC_SYNCH_EXPORT
include(dyndoc_synch.pri)

# install targets
# headers
headers.path = $$KKS_INCLUDE_DIR

# import lib
win32 {
  implib.path = $$KKS_LIB_DIR
  implib.files = $$sprintf("%1/%2.lib", $$DESTDIR, $$TARGET)

# lib
  target.path = $$KKS_BIN_DIR
  debug {
    target.files = \
    $$sprintf("%1/%2.dll", $$DESTDIR, $$TARGET) \
    $$sprintf("%1/%2.pdb", $$DESTDIR, $$TARGET) \
    $$sprintf("%1/%2.ilk", $$DESTDIR, $$TARGET) \
    $$sprintf("%1/%2.exp", $$DESTDIR, $$TARGET)
  }
  release {
    target.files = \
    $$sprintf("%1/%2.dll", $$DESTDIR, $$TARGET)
  }
}
unix {
  target.path = $$KKS_LIB_DIR 
}

INSTALLS += target headers
win32{
  INSTALLS += implib
}
