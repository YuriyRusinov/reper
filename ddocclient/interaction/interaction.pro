TEMPLATE = subdirs

include(../ddocclient.conf)

SUBDIRS += \
            ./kksrunservice \
            ./kksinteractor \
            ./kkshttpconnector \
            ./sync_queue_viewer \
            ./http_connector \
            ./kksdaemon \
            ./kksdbmonitor
