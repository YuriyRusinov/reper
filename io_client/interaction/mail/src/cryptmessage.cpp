#include "cryptmessage.h"

CryptMessage::CryptMessage(const QByteArray& mess)
{
    QBuffer buffer;
    buffer.setData(mess);
    buffer.open(QIODevice::ReadOnly);
    QDataStream in(&buffer);
    in >> message;
    in >> hash;
    in >> (quint32)CRC;
}

QByteArray CryptMessage::serialize (void) const
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << message;
    out << hash;
    out << (quint32)CRC;
    return buffer.data();
}

QDataStream& operator<< (QDataStream& out, const CryptMessage& cryptMessage)
{
    out << cryptMessage.message;
    out << cryptMessage.hash;
    out << (quint32)cryptMessage.CRC;

    return out;
}

QDataStream& operator>> (QDataStream& in, CryptMessage& cryptMessage)
{
    in >> cryptMessage.message;
    in >> cryptMessage.hash;
    in >> (quint32)cryptMessage.CRC;

    return in;
}

//несекретный параметр - таблица замен
//unsigned long
unsigned long Table[16] =
    { 0x1d4675e4, 0xfbbcd8ba,
    0xd4a7a149, 0x01011dc2,
    0x53750a6d, 0x7f2f83d8,
    0xa51d94f0, 0x49d8f2ae,
    0x9034ee26, 0x2a6a4f3b,
    0x3e896c81, 0xe75ec71c,
    0x6690b607, 0xb8c3207f,
    0x82fb5955, 0xcce23b93};

//секретный параметр - ключ
//unsigned long
unsigned long Key[8] =
    { 0x733d2c20,
    0x65686573,
    0x74746769,
    0x79676120,
    0x626e7373,
    0x20657369,
    0x326c6568,
    0x33206d54 };

unsigned long encrypt(QByteArray * message, unsigned long & crc, int /*key*/, int mode, int direction)
{
    return EncryptMessageEx((unsigned char *)message->constData(), crc, message->size(), Key, Table, mode, direction);
}
