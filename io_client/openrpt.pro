# main corelibs profile

TEMPLATE = subdirs

include(./io_client.conf)

SUBDIRS += ./openreport

RESOURCES = ./ddocs_ico/ddocs_icon_set.qrc

#transl
transl.files = ./transl/*.qm
transl.path = $$KKS_TRANSL_DIR
INSTALLS += transl
TRANSLATIONS = ./transl/ddocclient_ru.ts

unix{
    CONFIG_NAME_FILE = ./install_prefix.conf

    CURRENT_STRING = $$sprintf("printf %1KKS_PREFIX=%2%1 > %3", $$LITERAL_QUOTE, $$KKS_PREFIX, $$CONFIG_NAME_FILE)
    system($$CURRENT_STRING)
}
