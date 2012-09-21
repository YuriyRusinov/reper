# This is part of KKSSITO. Task db_synchronizer
# author ksa

TEMPLATE = app
TARGET = kaps_manager
include(../../ddocclient.conf)

DESTDIR = ../../build

#QT += xml

INCLUDEPATH += 	. \
		../../dataaccess \
		../../kkscore/base_classes \
		../../kkscore \
                ../../kksfactory \
		../../kksutils \
		../../kkspp 


DEPENDPATH += 	\
		. \
		../../dataaccess \
                ../../kksfactory \
		../../kkscore \
		../../kksutils \
		../../kkspp

INCLUDEPATH += . 

LIBS +=  -L../../build/ \
         -ldataaccess \
         -lkksfactory \
         -lkksutils\
         -lkkscore \
         -lkkspp \
         -lkksgui 

# Input
HEADERS += kaps_manager_form.h \
           edit_task_form.h \
           edit_task_var_form.h \
           edit_file_form.h

SOURCES += main.cpp \
           kaps_manager_form.cpp \
           edit_task_form.cpp \
           edit_task_var_form.cpp \
           edit_file_form.cpp
           

FORMS += kaps_manager_form.ui \
           edit_task_form.ui \
           edit_task_var_form.ui \
           edit_file_form.ui
 

target.files *= $${DISTFILES}
target.path = $$KKS_EXEC_DIR
INSTALLS += target
