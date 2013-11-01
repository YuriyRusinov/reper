#ifndef NET_STRUCT_H
#define NET_STRUCT_H

#include <QString>
#include <QList>

namespace dyndoc_mainStructs
{
    struct hostInformation
    {
        QString address;
        int port;
    };

    typedef hostInformation hostInf;

    struct hostStructInformation
    {
        QString name;
        QString parentName;
        QString protocol;
        QString id;
    };

    typedef hostStructInformation hostStructInf;

    struct hostModelInformation
    {
        hostStructInf structInf;
        hostInf address;
    };

    typedef hostModelInformation hostModelInf;

    struct dataBaseInformation
    {
        QString dbName;
        QString userName;
        QString password;

        QString hostAddress;
        int port;
    };

    typedef dataBaseInformation dbInf;

    struct transportInformation
    {
        int transportProtocol;
        int serverport;

        hostInf gateway;
    };

    typedef transportInformation transportInf;

    struct timerInformation
    {
        bool timerFlag;
        int delay;
    };

    typedef timerInformation timerInf;

    struct networkThreadInitialInformation
    {
        dbInf dataBase;
        transportInf transport;
        timerInf timer;
    };

    typedef networkThreadInitialInformation netThreadInf;
}

#endif
