#if !defined(__Utils_Cast_h)
#define __Utils_Cast_h

#include "configUtils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
namespace Utils {
    
    class __EXPORT_UTILS StringConvertor
    {
    public:
        static long toLong(const char * str, int base = 10, bool * ok = NULL);
        static double toDouble(const char * str, bool * ok = NULL);

        static char * fromLong(int value);
        static char * fromDouble(double value);
    private:
        StringConvertor(){}
        ~StringConvertor(){}
    };

    inline int round( double d )
    {
        return d >= 0.0 ? int(d + 0.5) : int( d - ((int)d-1) + 0.5 ) + ((int)d-1);
    }

    inline char * trim(const char * str)
    {
        if(!str)
            return NULL;

        unsigned int length = strlen(str);
        
        unsigned int lIndex=0;
        while(lIndex < length && isspace(str[lIndex]))
            lIndex++;

        unsigned int rIndex = length-1;
        while(rIndex > lIndex && isspace(str[rIndex]))
            rIndex--;

        if( (rIndex - lIndex + 1) == 0)
            return NULL;

        char * trimmed = new char[rIndex - lIndex + 2];

        unsigned int a=0;
        for(unsigned int i=lIndex; i<=rIndex; i++){
            trimmed[a] = str[i];
            a++;
        }
        trimmed[a] = '\0';

        return trimmed;
    }
}

#endif
