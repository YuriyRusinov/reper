# dyndoc_synch
HEADERS += \
        config_dyndoc_synch.h \
        UserInterface/dyndocForm/dyndocsyncform.h \
        UserInterface/DataStream/DataStream.h \
        UserInterface/NetworkStream/netThread.h \
        UserInterface/NetworkStream/NetworkStream.h \
        UserInterface/DataElements/viewElement.h \
        UserInterface/DataElements/dyndocModel.h \
        UserInterface/DataElements/dyndocView.h \
        UserInterface/DataElements/dyndoc_delegate.h \
        Namespaces/netStruct.h 

SOURCES += \
        main.cpp \
        UserInterface/dyndocForm/dyndocsyncform.cpp \
        UserInterface/DataStream/DataStream.cpp \
        UserInterface/NetworkStream/netThread.cpp \
        UserInterface/NetworkStream/NetworkStream.cpp \
        UserInterface/DataElements/viewElement.cpp \
        UserInterface/DataElements/dyndocModel.cpp \
        UserInterface/DataElements/dyndocView.cpp \
        UserInterface/DataElements/dyndoc_delegate.cpp \
        Namespaces/netStruct.cpp

FORMS   += \
        UserInterface/dyndocForm/dyndoc_sync_form.ui 

QT += network
