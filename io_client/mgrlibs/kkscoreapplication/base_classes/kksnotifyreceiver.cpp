#include "kksnotifyreceiver.h"

#include <kksdatabase.h>


void KKSNotifyReceiver::run()
{
    if(!m_db)
        return;

    m_db->addListener(this, "gis_data_changed");

    while(1){
        //if(m_parent && m_parent->stopped())
        //    break;
        //if(m_parent && m_parent->paused())
        //    continue;

        m_db->checkNotifies();
    }
}

void KKSNotifyReceiver::notify( char* notify_name, char * payload )
{
    if(!m_db)
        return;
    
    if( !payload)
        return;

    if(strcmp(notify_name, "gis_data_changed") != 0)
        return;

    QString p = QString("%1").arg(payload);

    
    QString tableName;
    QString idRecord;
    QString recordUniqueId;
    QString whatHappens;

    QStringList sl = p.split("~_~_~");
    if(sl.count() != 4)
        return;

    tableName = sl.at(0);
    idRecord = sl.at(1);
    recordUniqueId = sl.at(2);
    whatHappens = sl.at(3);

    emit databaseNotifyReceived("gis_data_changed", tableName, recordUniqueId);
}
