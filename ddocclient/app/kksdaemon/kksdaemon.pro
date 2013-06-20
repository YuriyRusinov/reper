TEMPLATE = app

include(../../ddocclient.conf)
QT += network

TARGET = kksdaemon

DESTDIR = ../../build

INCLUDEPATH +=  . \
    	        ../../dataaccess \
                ../../kksutils \
                ../../kksrunservice


DEPENDPATH += 	\
                . \
                ../../dataaccess \
                ../../kksutils \
                ../../kksrunservice

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkksrunservice 

# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS += -lpq

RC_FILE = kksdaemon.rc


include(kksdaemon.pri)

target.path = $$KKS_EXEC_DIR
INSTALLS += target
