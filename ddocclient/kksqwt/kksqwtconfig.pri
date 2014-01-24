######################################################################
# Install paths
######################################################################

# kksqwt
TEMPLATE = lib
TARGET = kksqwt

DESTDIR = ../../build   #because used in ./src
include(../../ddocclient.conf)


#VER_MAJ      = 5
#VER_MIN      = 2
#VER_PAT      = 1
#VERSION      = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

######################################################################
# qmake internal options
######################################################################

CONFIG           += qt     # Also for Qtopia Core!
CONFIG           += warn_on
CONFIG           += thread
CONFIG           += silent

######################################################################
# Build the static/shared libraries.
# If QwtDll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

CONFIG           += QwtDll

######################################################################
# QwtPlot enables all classes, that are needed to use the QwtPlot 
# widget. 
######################################################################

CONFIG       += QwtPlot

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
