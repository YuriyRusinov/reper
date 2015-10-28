#include (OpenGl/opengl.pri)

HEADERS += \
           repermainwindow.h \
           searchradioform.h \
           imagewidget.h \
           ImageLabel.h \
           assistant.h \
           searchradioimagefragmentform.h \
           searchradioimagecalc.h \
           searchresultsform.h \
           radMdiArea.h

FORMS += \
         reper_main_window.ui \
         search_radio_form.ui \
         search_radio_image_fragment_form.ui

SOURCES += \
           main.cpp \
           repermainwindow.cpp \
           searchradioform.cpp \
           imagewidget.cpp \
           ImageLabel.cpp \
           assistant.cpp \
           searchradioimagefragmentform.cpp \
           searchradioimagecalc.cpp \
           searchresultsform.cpp \
           radMdiArea.cpp

#RESOURCES = icon_set.qrc
