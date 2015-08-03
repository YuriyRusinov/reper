#ifndef DNVECTOR_H
#define DNVECTOR_H
#include <QString>
#include <QList>
struct GeoPoint
{
 double x;
 double y;
};
class DNVector
{
public:
    DNVector();
    QString EPSG;
    QString NameLayer;
    int TypeLayer;
    float Vol;
    QList <GeoPoint> GPt;
};

#endif // DNVECTOR_H
