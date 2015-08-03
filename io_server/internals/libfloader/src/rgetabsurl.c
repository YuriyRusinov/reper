
#include "floader.h"

PG_FUNCTION_INFO_V1(rgetabsurlex);

Datum rgetabsurlex(PG_FUNCTION_ARGS)
{
    FILE  *fFile;

    char * url;
    int idUrl;

    int spi_res;
    text * sql1;
    Datum datum;

    idUrl = PG_GETARG_INT32(0);
    if(idUrl <= 0){
      elog(ERROR, "id_url cannot be zero or negative. Please enter valid value.");
      PG_RETURN_NULL();
    }

    spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT){
      elog(ERROR, "Cannot connect via SPI");
    }


    sql1 = (text *) palloc(100);
    SET_VARSIZE(sql1, (100-VARHDRSZ));
    snprintf(VARDATA(sql1), 
            100-VARHDRSZ, 
            "select rGetAbsUrl(%d)", idUrl);
    
    datum = DirectFunctionCall2(exec_spi, 
                                PointerGetDatum(sql1), 
                                BoolGetDatum(true));
    if(!DatumGetCString(datum)){
        elog(NOTICE, "URL is NULL!");
        SPI_finish();
        PG_RETURN_NULL();
    }

    url = (char *)SPI_palloc(strlen(DatumGetCString(datum))+1);
    strcpy(url, DatumGetCString(datum));

        
    fFile = NULL;
    fFile = openFile(fFile, url, "rb");
    if(fFile == NULL){
        SPI_finish();
        PG_RETURN_NULL();
    }
    
    fclose(fFile);
    SPI_finish();

    PG_RETURN_TEXT_P(cstring_to_text_ex(url));
}

