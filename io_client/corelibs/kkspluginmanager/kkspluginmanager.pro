# kkspluginmanager
TEMPLATE = lib
TARGET = kkspluginmanager

DESTDIR = ../../build
include(../../io_client.conf)

CONFIG += warn_on dll
CONFIG += qt     # Also for Qtopia Core!
CONFIG += thread

DEFINES += __USE_DLL_KKSPLUGINMANAGER

INCLUDEPATH += .

include( kkspluginmanager.pri )


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

headers.path = $$KKS_INCLUDE_DIR

INSTALLS += target 
INSTALLS += headers
win32{
	INSTALLS += implib
}
