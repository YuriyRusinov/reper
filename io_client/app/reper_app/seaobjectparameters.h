#ifndef _SeaObjectParameters_H
#define _SeaObjectParameters_H

#include <QString>

class SeaObjectParameters
{
public:
    SeaObjectParameters (double l=-1.0, double w=-1.0, double d=-1.0, double az=-1.0, double elev=-1.0, QString sProp=QString());
    SeaObjectParameters (const SeaObjectParameters& SOP);
    ~SeaObjectParameters (void);

    double length;
    double width;
    double depth;

    double azimuth;
    double elevation_angle;

    QString secProp;
};

#endif
