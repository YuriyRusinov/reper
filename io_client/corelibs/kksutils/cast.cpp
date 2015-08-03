#include "cast.h"
#include <errno.h>
#include <limits.h>


using namespace Utils;

long StringConvertor::toLong(const char * str, int base, bool * ok)
{

    char *endptr;
    long value;

    if(ok)
        *ok = true;

    errno = 0;    /* To distinguish success/failure after call */
    value = strtol(str, &endptr, base);

    /* Check for various possible errors */
    if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))
            || (errno != 0 && value == 0)) {

        if(ok)
            *ok = false;
        return 0;
    }
    if(*endptr != '\0' && ok != NULL)
        *ok = false;
    
    return value;
}

double StringConvertor::toDouble(const char * str, bool * ok)
{
    double value = 0.0;
    char * endptr;
    errno = 0;    /* To distinguish success/failure after call */

    if(ok)
        *ok = true;

    value = strtod(str, &endptr);
    
    /* Check for various possible errors */
    if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))
            || (errno != 0 && value == 0)) {

        if(ok)
            *ok = false;
        return 0;
    }

    if(*endptr != '\0' && ok != NULL)
        *ok = false;
    
    return value;
}

char * StringConvertor::fromLong(int value)
{
    char * str = new char [100];
    snprintf(str, 99, "%d", value);

    return str;
}

char * StringConvertor::fromDouble(double value/*, int precision*/)
{
    char * str = new char [100];
    snprintf(str, 99, "%.6g", value);

    return str;
}
