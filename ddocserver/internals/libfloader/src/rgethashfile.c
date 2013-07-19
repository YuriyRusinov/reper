#include "floader.h"

#define BUFSIZE 8192


PG_FUNCTION_INFO_V1(rgethashfile);

Datum rgethashfile(PG_FUNCTION_ARGS)
{
    char * url;
    int digestLength = 0;
    unsigned char * digest;
    text * dgstData;
    int i=0;

    url = text_to_cstring_ex(PG_GETARG_TEXT_P(0));

    if(url == NULL){
        elog(ERROR, "URL of file cannot be NULL!");
        PG_RETURN_NULL();
    }
    
    digest = hashFile(url, &digestLength);
    

    pfree(url);

    if(digestLength == 0 || digest == NULL){
        elog(ERROR, "Cannot compute file digest!");
        PG_RETURN_NULL();
    }

    dgstData = (text*) palloc(VARHDRSZ + digestLength*2);
    MemSet(dgstData, 0, sizeof(VARHDRSZ + digestLength*2));
    SET_VARSIZE(dgstData, (VARHDRSZ + digestLength*2));
    
    
    sprintf(VARDATA(dgstData), "%02x", digest[0]);
    
    for(i=1; i<digestLength; i++)
        sprintf(VARDATA(dgstData), "%s%02x", VARDATA(dgstData), digest[i]);
        
    free(digest);

    PG_RETURN_TEXT_P(dgstData);
}
