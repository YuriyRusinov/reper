#ifndef NET_STRUCT_H
#define NET_STRUCT_H

#include <QString>
#include <QList>

namespace dyndoc_netThread
{
    struct hostInformation
    {
        QString hostAddress;
        int hostPort;
    };

    typedef hostInformation hostInf;

    struct dataBaseInformation
    {
        QString dbName;
        QString userName;
        QString password;

        QString hostAddress;
        int port;
    };

    typedef dataBaseInformation dbInf;

    struct timerInformation
    {
        bool timerFlag;
        int delay;
    };

    typedef timerInformation timerInf;

    struct localHostInformation
    {
        dbInf db;
        hostInf localhost;
        int transport;
    };

    typedef localHostInformation mainInf;

    struct synchronizationList
    {
        mainInf local;
        timerInf timer;
        QList<hostInf> hosts;
    };

    typedef synchronizationList synchList;
}

#endif
