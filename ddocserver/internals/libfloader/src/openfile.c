#include "floader.h"

FILE * openFile(FILE * f, const char * url, char * am)
{
    if(!url || !am)
        return NULL;

#ifdef WIN32
    if(fopen_s( &f, url, am ) != 0){
        return NULL; 
    }
#else
    f = fopen(url, am);
#endif

    return f;
}
