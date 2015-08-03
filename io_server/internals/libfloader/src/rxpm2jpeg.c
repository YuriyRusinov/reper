#include "floader.h"

PG_FUNCTION_INFO_V1(rxpm2jpeg);

Datum rxpm2jpeg(PG_FUNCTION_ARGS)
{

    text * xpmData;

    bytea        *data;
    
    unsigned char * jpegData = NULL;
    unsigned long jpegDataSize = 0;
    int ok = 0;
    
    xpmData = PG_GETARG_TEXT_P(0);

    ok = xpm2jpeg(VARDATA(xpmData), VARSIZE(xpmData)-VARHDRSZ, &jpegData, &jpegDataSize);
    if(ok < 0 || jpegData == NULL || jpegDataSize <= 0){
        elog(WARNING, "Cannot convert XPM data to JPEG format!");
        PG_RETURN_NULL();
    }
    
    //elog(WARNING, "111");


    data = (text*) palloc(VARHDRSZ + jpegDataSize);
    MemSet(data, 0, sizeof(VARHDRSZ + jpegDataSize));
    SET_VARSIZE(data, (VARHDRSZ + jpegDataSize));
    
    memcpy(VARDATA(data), jpegData, jpegDataSize);
    free(jpegData);

    PG_RETURN_BYTEA_P(data);
}
