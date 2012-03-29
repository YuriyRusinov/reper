#include "floader.h"

PG_FUNCTION_INFO_V1(rwritefile);

Datum rwritefile(PG_FUNCTION_ARGS)
{
    text * data;    
    FILE * fFile;
    long size;
    char * url;
    char * raPath;
    char * absFilePath;
    int idUrl;
    int spi_res;
    char * sql;
    SPITupleTable * tuptable;
    int mode;

    idUrl = PG_GETARG_INT32(0);
    data = PG_GETARG_TEXT_P(1);
    mode = PG_GETARG_INT32(2);

    if(idUrl <= 0 ){
        elog(ERROR, "id_url cannot be zero or negative. Please enter valid value.");
        //PG_RETURN_INT32(0);
    }

    spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT){
        elog(ERROR, "Cannot connect via SPI");
        //PG_RETURN_INT32(-1);
    }

	
    sql = (char *) palloc(70*sizeof(char));
    snprintf(sql, 70*sizeof(char), "select rGetAbsUrlEx(%d);", idUrl);

    spi_res = SPI_exec(sql, 0);
    pfree(sql);
    if(spi_res != SPI_OK_SELECT){
        elog(ERROR, "Cannot execute function rGetAbsUrlEx. SPI_execute failed.");
        //PG_RETURN_INT32(-1); 
    }

    if(SPI_processed != 1){
        SPI_finish();
        elog(ERROR, "function rGetAbsUrlEx return unexpected count of rows. SPI_processed contain bad result.");
        //PG_RETURN_INT32(-1);
    }
    tuptable = SPI_tuptable;
    absFilePath = SPI_getvalue(tuptable->vals[0], tuptable->tupdesc, 1);
    
    if(absFilePath == NULL || 
       strcmp(absFilePath, "") || 
       strcmp(absFilePath, "not assigned") )
    {
//        elog(NOTICE, "In NULL LOOP");
        sql = (char *) palloc(70*sizeof(char));
        snprintf(sql, 70, "select raGetPath();");
        spi_res = SPI_exec(sql, 0);
        pfree(sql);
        if(spi_res != SPI_OK_SELECT){
            SPI_finish();
            elog(ERROR, "Cannot execute function raGetPath. SPI_execute failed.");
        }

        if(SPI_processed != 1){
            SPI_finish();
            elog(ERROR, "function raGetPath return unexpected count of rows. SPI_processed contain bad result.");
        }
        tuptable = SPI_tuptable;
        raPath = SPI_getvalue(tuptable->vals[0], tuptable->tupdesc, 1);
        if(raPath == NULL){
            elog(NOTICE, "path for files archive not assigned (is NULL)");
            SPI_finish();
            PG_RETURN_INT32(-2);
        }

        url = (char *)palloc(strlen(raPath) + 30*sizeof(char));
        sprintf(url, "kks_file_%d", idUrl);
          
        sql = (char *) palloc(strlen(url) + 30*sizeof(char));
        snprintf(sql, strlen(url) + 30*sizeof(char) - 1, "select rSetUrl(%d, '%s');", idUrl, url);
        spi_res = SPI_exec(sql, 0);
        pfree(sql);
        if(spi_res != SPI_OK_SELECT){
            SPI_finish();
            elog(ERROR, "Cannot execute function rSetUrl. SPI_execute failed.");
            //PG_RETURN_INT32(-1); 
        }

        if(SPI_processed != 1){
            SPI_finish();
            elog(ERROR, "function rSetUrl return unexpected count of rows. SPI_processed contain bad result.");
            //PG_RETURN_INT32(-1);
        }
        tuptable = SPI_tuptable;
        if(strcmp(SPI_getvalue(tuptable->vals[0], tuptable->tupdesc, 1), "1") != 0){
            elog(NOTICE, "cannot update URL for given file. Error was occured.");
            SPI_finish();
            PG_RETURN_INT32(-2);
        }
        sprintf(url, "%skks_file_%d", raPath, idUrl);
    }
    else{
        url = (char *)palloc(strlen(absFilePath)+1);
        strcpy(url, absFilePath);
    }
    
    elog(NOTICE, url);
    
    if(mode == 0){
        fFile = fopen(url, "wb");
    }
    else if(mode == 1){
        fFile = fopen(url, "ab");
    }
    else if(mode == 2){
        fFile = fopen(url, "rb");
        if(fFile != NULL){
            pfree(url);
            SPI_finish();
            elog(NOTICE, "Given file exist! Function exited because of mode = 2");
            fclose(fFile);
            PG_RETURN_INT32(-4);
        }
        
        fFile = fopen(url, "wb");
    }
    else{
        pfree(url);
        SPI_finish();
        elog(NOTICE, "parameter mode must be 0, 1 or 2. You entered incorrect value");
        PG_RETURN_INT32(-3);
    }

    if (fFile == NULL){
        SPI_finish();
        elog(ERROR, "FOPEN: %s, URL=%s", strerror(errno), url);
        //pfree(url);
        
        //PG_RETURN_INT32(-5);
    }

    elog(NOTICE, "aaaaaa");

    pfree(url);
    SPI_finish();
    
    elog(NOTICE, "bbbb");

    //PG_RETURN_INT32(0);

    size = fwrite(VARDATA(data), VARSIZE(data)-VARHDRSZ, 1, fFile);
    if(size == 0){
        fclose(fFile);
        elog(ERROR, "Data not writed!");
        //PG_RETURN_INT32(-6);
    }
    if(fclose(fFile) != 0){
        elog(ERROR, "FCLOSE: %s", strerror(errno));
    }

    PG_RETURN_INT32(size);
}
