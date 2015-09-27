# Digital Signature for IO plugin

TEMPLATE = lib
CONFIG += plugin dll
TARGET = send_as_xml

DESTDIR = ../../build/ddocplugins
DEPLIBS_DIR = ../../build

include(../../io_client.conf)

INCLUDEPATH += 	. \
		../../corelibs/dataaccess \
		../../corelibs/kksutils \
		../../corelibs/kksdatafactory \
		../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
                ../../guilibs/kksgui \
                ../../guilibs/kksqwt \
                ../../guilibs/kksguifactory \
                ../../mgrlibs/kkscoreapplication \
                ../../mgrlibs/kksapplication \
                ../../mgrlibs/kkscoreapplication/base_classes \
                ../../mgrlibs/kkscoreapplication/ui

DEPENDPATH += 	\
		../../corelibs/dataaccess \
		../../corelibs/kksutils \
		../../corelibs/kksdatafactory \
		../../corelibs/kkspp \
                ../../corelibs/kkspluginmanager \
                ../../guilibs/kksgui \
                ../../guilibs/kksqwt \
                ../../guilibs/kksguifactory \
                ../../mgrlibs/kkscoreapplication \
                ../../mgrlibs/kksapplication

LIBS += -L$$DEPLIBS_DIR -ldataaccess -lkksutils -lkkspp -lkksdatafactory -lkkspluginmanager -lkksgui -lkksqwt -lkksguifactory -lkksapplication -lkkscoreapplication

include(send_as_xml.pri)

win32 {
    # lib
    target.path = $$KKS_PLUGIN_DIR
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
    target.path = $$KKS_PLUGIN_DIR
}

INSTALLS += target

#win32 {
#    INSTALLS += implib
#}
