# http-client KKSSITO. 

TEMPLATE = app
TARGET = sync_queue_viwer

DESTDIR = ../../build
include(../../ddocclient.conf)

INCLUDEPATH += 	. \
		../../dataaccess \
		../../kkscore/base_classes \
		../../kkscore \
                ../../kkspp \
                ../../kksfactory \
		../../kksutils 
#                ./include
#                /usr/local/kkssito/include\
#                /usr/local/pgsql/include

DEPENDPATH += 	. \
		../../dataaccess \
		../../kkscore \
		../../kkscore/base_classes \
                ../../kkspp \
                ../../kksfactory \
		../../kksutils 

LIBS += -L$$DESTDIR -ldataaccess -lkksutils -lkkscore -lkksfactory -lkkspp -lkksgui

# libpq support
LIBS += -L$${PSQL_HOME}/lib
INCLUDEPATH *= $${PSQL_HOME}/include
INCLUDEPATH *= $${PSQL_INCLUDE_DIR}

win32 {
!win32-g++:LIBS *= libpqdll.lib
win32-g++:LIBS += $${PSQL_HOME}/lib/libpq.dll.a
}
unix: LIBS *= -lpq

# Input
HEADERS += \
           ./syncqueueviewerform.h \
           ./syncqueueviewform.h \
           ./syncqueueview.h \
           ./syncqueueitemmodel.h \
		   ./filtersform.h

SOURCES += \
           ./syncqueueviewerform.cpp \
           ./syncqueueviewform.cpp \
           ./syncqueueview.cpp \
           ./syncqueueitemmodel.cpp \
           ./main.cpp \
		   ./filtersform.cpp

FORMS += \
           ./sync_queue_view_form.ui \
           ./sync_queue_viewer_form.ui 

target.files *= $${DISTFILES}
target.path = $$KKS_EXEC_DIR
INSTALLS += target

