#include "floader.h"

char * text_to_cstring_ex(text * str)
{
	int len = 0;
	char * retValue = NULL;
	
	if(!str)
	  return NULL;
	
    len = VARSIZE(str)-VARHDRSZ;
    retValue = (char *)palloc(len + 2);
	
	//memcpy((void *) VARDATA(retValue), (void *) str, len);
    snprintf(retValue, len+1, "%s", (const char *)VARDATA(str));

    return retValue;
}
