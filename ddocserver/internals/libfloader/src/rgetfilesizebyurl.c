#include "floader.h"

PG_FUNCTION_INFO_V1(rgetfilesizebyurl);

Datum rgetfilesizebyurl(PG_FUNCTION_ARGS)
{
    FILE  *fFile;

    int64  size;
    char * url;

    url = text_to_cstring_ex(PG_GETARG_TEXT_P(0));

    fFile = fopen(url, "rb");

    if (fFile == NULL){
	  elog(NOTICE, "Cannot open file with given URL. \
                  File does not exist or permission denied. URL='%s'", 
                  url);
          pfree(url);
          elog(ERROR, "Exited because of previous error");
    }
    
    pfree(url);

    fseeko(fFile, 0, SEEK_END);
    size = ftello(fFile);
    fclose(fFile);
    
    
    PG_RETURN_INT64(size);

}
