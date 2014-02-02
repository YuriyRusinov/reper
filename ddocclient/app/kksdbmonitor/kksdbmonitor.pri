# dyndoc_synch
HEADERS += \
        UserInterface/dyndocForm/dyndocsyncform.h \
        UserInterface/DataStream/DataStream.h \
        UserInterface/DataElements/viewElement.h \
        UserInterface/DataElements/dyndocModel.h \
        UserInterface/DataElements/dyndocView.h \
        UserInterface/DataElements/dyndoc_delegate.h \
        UserInterface/dyndocForm/optionsdialog.h \
        Namespaces/netStruct.h 

SOURCES += \
        main.cpp \
        UserInterface/dyndocForm/dyndocsyncform.cpp \
        UserInterface/DataStream/DataStream.cpp \
        UserInterface/DataElements/viewElement.cpp \
        UserInterface/DataElements/dyndocModel.cpp \
        UserInterface/DataElements/dyndocView.cpp \
        UserInterface/DataElements/dyndoc_delegate.cpp \
        UserInterface/dyndocForm/optionsdialog.cpp \
        Namespaces/netStruct.cpp

FORMS   += \
        UserInterface/dyndocForm/dyndoc_sync_form.ui \
        UserInterface/dyndocForm/options_dialog.ui

