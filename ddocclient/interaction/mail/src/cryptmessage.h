#ifndef CRYPT_MESSAGE_H
#define CRYPT_MESSAGE_H

#include <QHash>
#include <QBuffer>
#include <QDataStream>
#include "libgost.h"

unsigned long encrypt(QByteArray * message, unsigned long & crc, int key, int mode, int direction);

class CryptMessage
{
public:
    CryptMessage(const QByteArray& mess);
    QByteArray serialize () const;

    CryptMessage(QByteArray mes, int key, unsigned long crc) :
        message(mes),
        hash(qHash(mes) + key),
        CRC(crc){}

    QByteArray message;
    int hash;
    unsigned long CRC;

private:

    friend QDataStream & operator<< (QDataStream& out, const CryptMessage& cat);
    friend QDataStream & operator>> (QDataStream& in, CryptMessage& cat);
};

QDataStream& operator<< (QDataStream& out, const CryptMessage& cat);
QDataStream& operator>> (QDataStream& in, CryptMessage& cat);

#endif
