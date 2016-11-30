#ifndef _SeaObjectParameters_H
#define _SeaObjectParameters_H

#include <QString>
#include <QRect>

class SeaObjectParameters
{
public:
    SeaObjectParameters (QRect r=QRect(), double l=-1.0, double w=-1.0, double d=-1.0, double az=-1.0, double elev=-1.0, QString sProp=QString());
    SeaObjectParameters (const SeaObjectParameters& SOP);
    ~SeaObjectParameters (void);

    QRect bRect;
    double length;
    double width;
    double depth;

    double azimuth;
    double elevation_angle;

    QString secProp;
};

#endif
