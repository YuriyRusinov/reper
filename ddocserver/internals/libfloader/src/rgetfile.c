#include "floader.h"

#define BUFSIZE 8192


PG_FUNCTION_INFO_V1(rgetfile);

Datum rgetfile(PG_FUNCTION_ARGS)
{
    FILE  *fFile;

    int    blockSize;
    int64  size;
    int64  position;     
    char * url;
    int idUrl;

    int spi_res;
    text * sql1;
    Datum datum;

    idUrl = PG_GETARG_INT32(0);
    blockSize = PG_GETARG_INT32(1);
    position = PG_GETARG_INT64(2);

    if(blockSize < 1) 
        blockSize = BUFSIZE;


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
    snprintf(VARDATA(sql1), 100-VARHDRSZ, "select rGetAbsUrlEx(%d);", idUrl);
    
    datum = DirectFunctionCall2(exec_spi, PointerGetDatum(sql1), BoolGetDatum(true));
    if(!DatumGetCString(datum)){
        elog(NOTICE, "URL is NULL!");
        SPI_finish();
        PG_RETURN_NULL();
    }

    url = (char *)SPI_palloc(strlen(DatumGetCString(datum))+1);
    strcpy(url, DatumGetCString(datum));

    fFile = fopen(url, "rb");

    if (fFile == NULL){
        elog(NOTICE, "Cannot open file with given URL. File does not exist or permission denied. URL='%s'", url);
        pfree(url);
        SPI_finish();
        elog(ERROR, "Exited because of previous error");
    }

    pfree(url);
    
    SPI_finish();

    fseek(fFile, 0, SEEK_END);
    size = ftell(fFile);
    if(position>=size){
        fclose(fFile);
        elog(NOTICE, "File is readed!");
        //pfree(url);
        PG_RETURN_NULL();
    }

    fseek(fFile, (long)position, SEEK_SET);
    
    if ( ! feof(fFile) ){
        int          readed;
        bytea        *data;

        data = (text*) palloc(VARHDRSZ + blockSize);
        MemSet(data, 0, sizeof(VARHDRSZ+blockSize));
        SET_VARSIZE(data, (VARHDRSZ + blockSize));

        readed = fread(VARDATA(data), 1, blockSize, fFile);
        if ( readed != blockSize ){
            SET_VARSIZE(data, (VARHDRSZ + readed));
            elog(NOTICE,"Read from file less then requested");
        }
        

        fclose(fFile);
        //pfree(url);
        PG_RETURN_BYTEA_P(data);
    }
    else{
        fclose(fFile);
        elog(NOTICE, "feof is found!");
        //pfree(url);
        PG_RETURN_NULL();
    }
}
