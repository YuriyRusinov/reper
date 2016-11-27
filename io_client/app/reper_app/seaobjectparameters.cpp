#include "seaobjectparameters.h"

SeaObjectParameters :: SeaObjectParameters (double l, double w, double d, double az, double elev, QString sProp)
    : length (l),
    width (w),
    depth (d),
    azimuth (az),
    elevation_angle (elev),
    secProp (sProp)
{
}

SeaObjectParameters :: SeaObjectParameters (const SeaObjectParameters& SOP)
    : length (SOP.length),
    width (SOP.width),
    depth (SOP.depth),
    azimuth (SOP.azimuth),
    elevation_angle (SOP.elevation_angle),
    secProp (SOP.secProp)
{
}

SeaObjectParameters :: ~SeaObjectParameters (void)
{
}
