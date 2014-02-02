TEMPLATE = subdirs

include(../ddocclient.conf)

# Directories
SUBDIRS += \
	operator_app \
	http_connector \
#        sync_queue_viewer \
        kksdbmonitor \
        kksdaemon

#win32{
#        SUBDIRS += \
#                   db_synchronizer
#}

#SUBDIRS += kaps_manager
