#include "floader.h"


text * cstring_to_text_ex(const char * str)
{
	int len = 0;
	text * retValue = NULL;
	
	if(!str)
	  return NULL;
	
	len = strlen(str);
	
    retValue = (text*) palloc(VARHDRSZ + len);
    MemSet( retValue, 0, len+VARHDRSZ );
    SET_VARSIZE(retValue, (VARHDRSZ + len));
    //VARATT_SIZEP(retValue) = VARHDRSZ + len;
    memcpy((void *) VARDATA(retValue), (void *) str, len);

    return retValue;
}
