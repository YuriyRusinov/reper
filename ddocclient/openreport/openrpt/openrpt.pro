TARGET = openrpt
# OpenRPT report writer and rendering engine
# Copyright (C) 2001-2008 by OpenMFG, LLC
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# Please contact info@openmfg.com with any questions on this license.


TEMPLATE = app

DESTDIR = ../../build

CONFIG += qt \
          warn_on \
          staticlib

CONFIG += debug

INCLUDEPATH = ../rptrenderer \
               ./images \
               ./ \
               ./src

LIBS *= -L$$DESTDIR -lrptrenderer

MOC_DIR = .tmp
UI_DIR = .tmp
OBJECTS_DIR = .tmp
RCC_DIR = .tmp

QT += xml \
      sql

# VERSION = 0.1.0

# The following line was changed from FORMS to FORMS3 by qt3to4
FORMS += barcodeeditor.ui \
    coloreditor.ui \
    colorlist.ui \
    crosstabeditor.ui \
    dbfiledialog.ui \
    detailgroupsectiondialog.ui \
    detailsectiondialog.ui \
    editpreferences.ui \
    fieldeditor.ui \
    grapheditor.ui \
    imageeditor.ui \
    labeleditor.ui \
    pagesetup.ui \
    queryeditor.ui \
    querylist.ui \
    reportparameter.ui \
    reportparameterlist.ui \
    reportparameterlistitem.ui \
    reportproperties.ui \
    sectioneditor.ui \
    src/booledit.ui \
    src/doubleedit.ui \
    src/intedit.ui \
    src/listedit.ui \
    src/login.ui \
    src/loginOptions.ui \
    src/newvariant.ui \
    src/parameteredit.ui \
    src/paramlistedit.ui \
    src/stringedit.ui \
    texteditor.ui

HEADERS += src/barcodeeditor.h \
    src/booledit.h \
    src/builtinSqlFunctions.h \
    src/builtinformatfunctions.h \
    src/coloreditor.h \
    src/colorlist.h \
    src/crosstabeditor.h \
    src/data.h \
    src/dbfiledialog.h \
    src/dbtools.h \
    src/detailgroupsectiondialog.h \
    src/detailsectiondialog.h \
    src/documentscene.h \
    src/documentview.h \
    src/documentwindow.h \
    src/doubleedit.h \
    src/editpreferences.h \
    src/fieldeditor.h \
    src/fontutils.h \
    src/grapheditor.h \
    src/graphicsitems.h \
    src/graphicssection.h \
    src/header.h \
    src/imageeditor.h \
    src/intedit.h \
    src/labeleditor.h \
    src/labelsizeinfo.h \
    src/languageoptions.h \
    src/listedit.h \
    src/login.h \
    src/loginOptions.h \
    src/memdbloader.h \
    src/newvariant.h \
    src/openreports.h \
    src/orprerender.h \
    src/orprintrender.h \
    src/pagesetup.h \
    src/pagesizeinfo.h \
    src/parameter.h \
    src/parameteredit.h \
    src/paramlistedit.h \
    src/parsexmlutils.h \
    src/previewdialog.h \
    src/querycombobox.h \
    src/queryeditor.h \
    src/querylist.h \
    src/querysource.h \
    src/quuencode.h \
    src/renderobjects.h \
    src/reportgridoptions.h \
    src/reporthandler.h \
    src/reportpageoptions.h \
    src/reportparameter.h \
    src/reportparameterlist.h \
    src/reportparameterlistitem.h \
    src/reportproperties.h \
    src/reportwriterwindow.h \
    src/sectioneditor.h \
    src/stringedit.h \
    src/texteditor.h \
    src/xsqlquery.h \
    src/xvariant.h

SOURCES += barcodeeditor.cpp \
    coloreditor.cpp \
    colorlist.cpp \
    crosstabeditor.cpp \
    data.cpp \
    dbfiledialog.cpp \
    detailgroupsectiondialog.cpp \
    detailsectiondialog.cpp \
    documentscene.cpp \
    documentview.cpp \
    documentwindow.cpp \
    editpreferences.cpp \
    fieldeditor.cpp \
    fontutils.cpp \
    grapheditor.cpp \
    graphicsitems.cpp \
    graphicssection.cpp \
    imageeditor.cpp \
    labeleditor.cpp \
    pagesetup.cpp \
    querycombobox.cpp \
    queryeditor.cpp \
    querylist.cpp \
    reportgridoptions.cpp \
    reporthandler.cpp \
    reportparameter.cpp \
    reportparameterlist.cpp \
    reportparameterlistitem.cpp \
    reportproperties.cpp \
    sectioneditor.cpp \
    src/booledit.cpp \
    src/builtinSqlFunctions.cpp \
    src/builtinformatfunctions.cpp \
    src/dbtools.cpp \
    src/doubleedit.cpp \
    src/intedit.cpp \
    src/labelsizeinfo.cpp \
    src/languageoptions.cpp \
    src/listedit.cpp \
    src/login.cpp \
    src/loginOptions.cpp \
    src/memdbloader.cpp \
    src/newvariant.cpp \
    src/pagesizeinfo.cpp \
    src/parameter.cpp \
    src/parameteredit.cpp \
    src/paramlistedit.cpp \
    src/parsexmlutils.cpp \
#    src/qrc_OpenRPTCommon.cpp \
#    src/qrc_writer.cpp \
    src/querysource.cpp \
    src/quuencode.cpp \
    src/reportpageoptions.cpp \
    src/reportwriterwindow.cpp \
    src/rptwrt.cpp \
    src/stringedit.cpp \
    src/xsqlquery.cpp \
    src/xvariant.cpp \
    texteditor.cpp


TRANSLATIONS = wrtembed_ru.qm

RESOURCES *= ./images/OpenRPTCommon.qrc

OPENEDFILES += src/rptwrt.cpp68 \
               src/reportwriterwindow.h0 \
               src/reportwriterwindow.cpp157 \
               src/reporthandler.h1 \
               reporthandler.cpp1812
