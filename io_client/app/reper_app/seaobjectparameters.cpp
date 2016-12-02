#include "seaobjectparameters.h"

SeaObjectParameters :: SeaObjectParameters (QRect r, double l, double w, double d, double az, double elev, double res, QString sProp)
    : bRect (r),
    length (l),
    width (w),
    depth (d),
    azimuth (az),
    elevation_angle (elev),
    resolution (res),
    secProp (sProp)
{
}

SeaObjectParameters :: SeaObjectParameters (const SeaObjectParameters& SOP)
    : bRect (SOP.bRect),
    length (SOP.length),
    width (SOP.width),
    depth (SOP.depth),
    azimuth (SOP.azimuth),
    elevation_angle (SOP.elevation_angle),
    resolution (SOP.resolution),
    secProp (SOP.secProp)
{
}

SeaObjectParameters :: ~SeaObjectParameters (void)
{
}
