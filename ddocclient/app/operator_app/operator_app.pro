TEMPLATE = app

include(../../ddocclient.conf)

TARGET = $${EXECUTABLE_PREFIX}_app

DESTDIR = ../../build

INCLUDEPATH += 	. \
		../../dataaccess \
		../../kkscore/base_classes \
		../../kkscore \
		../../kksutils \
		../../kksfactory \
		../../kkspp \
                ../../kksgui \
		../../kksgui/ui


DEPENDPATH += 	\
		. \
		../../dataaccess \
		../../kkscore \
		../../kksutils \
		../../kkspp \
		../../kksfactory \
                ../../kksgui

INCLUDEPATH *= ../../rptrenderer \
               ../../rptrenderer/src

DEPENDPATH *= ../kkspp \
              ../../rptrenderer

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkscore -lkkspp -lkksfactory -lkksgui -lrptrenderer


# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS += -lpq

RC_FILE = operator.rc


include(operator_app.pri)

target.path = $$KKS_EXEC_DIR
INSTALLS += target
