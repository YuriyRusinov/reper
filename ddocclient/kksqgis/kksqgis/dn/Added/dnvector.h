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
     QString TypeField;
     QString NameField;
     QString VolPolygon;
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
