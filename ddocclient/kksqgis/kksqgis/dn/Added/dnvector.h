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
    struct ClassLayer
    {
     QString TypeField; // тип поля
     QString NameField; // имя поля
     QString VolPolygon; // значение Value
    };
    DNVector();
    QString EPSG;
    QString NameLayer;
    int TypeLayer;
    float Vol;
    QList <GeoPoint> GPt;
    QList <ClassLayer> Polygons;
};

#endif // DNVECTOR_H
