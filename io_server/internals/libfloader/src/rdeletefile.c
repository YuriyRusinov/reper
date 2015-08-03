#include "floader.h"


PG_FUNCTION_INFO_V1(rdeletefile);

Datum rdeletefile(PG_FUNCTION_ARGS)
{
    FILE  *fFile;

    char * url;
    int idUrl;

    int rmResult;

    int spi_res;
    text * sql1;
    Datum datum;

    idUrl = PG_GETARG_INT32(0);

    if(idUrl <= 0){
      elog(ERROR, "id_url cannot be zero or negative. Please enter valid value.");
      PG_RETURN_INT32(-1);
    }


    spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT){
      elog(ERROR, "Cannot connect via SPI");
    }


    sql1 = (text *) palloc(100);
    SET_VARSIZE(sql1, (100-VARHDRSZ));
    snprintf(VARDATA(sql1), 100-VARHDRSZ, "select rGetAbsUrlEx(%d);", idUrl);
    
    datum = DirectFunctionCall2(exec_spi, PointerGetDatum(sql1), BoolGetDatum(true));
    if(!DatumGetCString(datum)){
        elog(WARNING, "URL is NULL!");
        SPI_finish();
        PG_RETURN_INT32(-1);
    }

    url = (char *)SPI_palloc(strlen(DatumGetCString(datum))+1);
    strcpy(url, DatumGetCString(datum));

    fFile = NULL;
    fFile = openFile(fFile, url, "rb");

    if (fFile == NULL){
        SPI_finish();
        elog(WARNING, "Cannot open file with given URL. File does not exist or permission denied. URL='%s'", url);
        PG_RETURN_INT32(-1);
    }

    fclose(fFile);

    rmResult = remove(url);

    //elog(NOTICE, "File %s removed!", url);
   
    pfree(url);
    SPI_finish();
    

    PG_RETURN_INT32(1);
}
