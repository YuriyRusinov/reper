#include "floader.h"

#define BUFSIZE 8192


PG_FUNCTION_INFO_V1(rgetfilebyurl);

Datum rgetfilebyurl(PG_FUNCTION_ARGS)
{
    FILE  *fFile;

    int    blockSize;
    int64  size;
    int64  position;     
    char * url;

    url = text_to_cstring_ex(PG_GETARG_TEXT_P(0));
    blockSize = PG_GETARG_INT32(1);
    position = PG_GETARG_INT64(2);

    if(blockSize < 1) 
        blockSize = BUFSIZE;



    fFile = fopen(url, "rb");

    if (fFile == NULL){
	  //pfree(url);
	  elog(NOTICE, "Cannot open file with given URL. \
                  File does not exist or permission denied. URL='%s'", 
                  url);
          pfree(url);
          elog(ERROR, "Exited because of previous error");
    }
    
    pfree(url);

    fseeko(fFile, 0, SEEK_END);
    size = ftello(fFile);
    if(position>=size){
        fclose(fFile);
        elog(NOTICE, "File is readed!");
        //pfree(url);
        PG_RETURN_NULL();
    }
    
    fseeko(fFile, (long)position, SEEK_SET);
    
    if ( ! feof(fFile) ){
        int64        readed;
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
