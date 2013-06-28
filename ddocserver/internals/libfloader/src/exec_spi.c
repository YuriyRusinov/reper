#include "floader.h"

/*
 * This is required for builds against pgsql 8.2
 */
/*#include "pgmagic.h"*/
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(exec_spi);

Datum exec_spi(PG_FUNCTION_ARGS)
{
    int spi_res = 0;
    text * sql;
    char * s_sql = NULL;
    int len = 0;	
    SPITupleTable * tuptable;
	bool read_only = true;
    //text * retValue;
    
    sql = PG_GETARG_TEXT_P(0);
	read_only = PG_GETARG_BOOL(1);
    len = VARSIZE(sql)-VARHDRSZ;
    s_sql = (char *)palloc(len + 2);
    snprintf(s_sql, len+1, "%s", (const char *)VARDATA(sql));

    spi_res = SPI_exec((const char*)s_sql, 0);

    //pfree(s_sql);

    if(spi_res < 0){
        SPI_finish();
        elog(ERROR, "Cannot execute query %s. SPI_execute failed.", (const char *)s_sql);
    }

    if(SPI_processed < 1){
        SPI_finish();
        elog(ERROR, "query %s return unexpected count of rows. SPI_processed contain bad result.", (const char *)s_sql);
    }
    tuptable = SPI_tuptable;
    
    if(spi_res != SPI_OK_SELECT)
	  PG_RETURN_DATUM(Int32GetDatum(1));
	else
	  PG_RETURN_DATUM(CStringGetDatum(SPI_getvalue(tuptable->vals[0], tuptable->tupdesc, 1)));
}

PG_FUNCTION_INFO_V1(exec_spi_array);

Datum exec_spi_array(PG_FUNCTION_ARGS)
{
    int spi_res = 0;
    text * sql;
    char * s_sql = NULL;
    int len = 0;	
    SPITupleTable * tuptable;
    //text * retValue;
	//char * res;
	Datum datum;
	bool read_only = true;
    
    sql = PG_GETARG_TEXT_P(0);
	read_only = PG_GETARG_BOOL(1);
    len = VARSIZE(sql)-VARHDRSZ;
    s_sql = (char *)palloc(len + 2);
    snprintf(s_sql, len+1, "%s", (const char *)VARDATA(sql));

    spi_res = SPI_exec((const char*)s_sql, 0);

    //pfree(s_sql);

    if(spi_res != SPI_OK_SELECT){
        SPI_finish();
        elog(ERROR, "Cannot execute query %s. SPI_execute failed.", (const char *)s_sql);
    }

    if(SPI_processed < 1){
        SPI_finish();
        elog(ERROR, "query %s return unexpected count of rows. SPI_processed contain bad result.", (const char *)s_sql);
    }
    tuptable = SPI_tuptable;
    
	datum = DirectFunctionCall2(text_to_array, 
	                            PointerGetDatum(cstring_to_text_ex(SPI_getvalue(tuptable->vals[0], tuptable->tupdesc, 1))), 
	                            PG_GETARG_DATUM(2));
	PG_RETURN_DATUM(datum);
}
