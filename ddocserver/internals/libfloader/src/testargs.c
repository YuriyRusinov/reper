//#include <stdio.h>
//#include <postgres.h>
//#include <executor/spi.h>
#include "floader.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1 (testspi);

Datum testspi(PG_FUNCTION_ARGS)
{
    int ret=0;
    text * sql_text = PG_GETARG_TEXT_P(0);
    bool isReadOnly = PG_GETARG_BOOL(1);
    int cnt = PG_GETARG_INT32 (2);
    char * s_sql = NULL;
    int len = VARSIZE (sql_text)-VARHDRSZ;

    SPITupleTable * tuptable;
    s_sql = (char *)palloc(len + 2);
    snprintf(s_sql, len+1, "%s", (const char *)VARDATA(sql_text));

    SPI_connect();
    ret = SPI_execute (s_sql, isReadOnly, cnt);
    elog (INFO, "sql is %s\n", s_sql);
    tuptable = SPI_tuptable;

    if (tuptable)
    {
        int i=0, j=0;
        //elog (INFO, "tuptable is %s\n", tuptable->tupdesc);
        int proc = SPI_processed;
        elog (INFO, "row processed %d\n", proc);
        elog (INFO, "columns processed %d\n", tuptable->tupdesc->natts );
        //elog (INFO, "table name %s\n", tuptable->tuptabcxt->name);
//        elog (INFO, "table name %s\n", tuptable->tuptabcxt->name);
        for (i=0; i<proc; i++)
        {
            HeapTuple tuple = tuptable->vals[i];
            for (j=1; j<=tuptable->tupdesc->natts; j++)
            {
                elog (INFO, "\tcolumn type is %s\n", SPI_gettype(tuptable->tupdesc, j));
                elog (INFO, "\t%s\n", SPI_getvalue(tuple, tuptable->tupdesc, j));
            }
        }
    }

    SPI_finish();
    PG_RETURN_INT32 (ret);
}

PG_FUNCTION_INFO_V1 (testargs);

Datum testargs(PG_FUNCTION_ARGS)
{
    text * name_text = PG_GETARG_TEXT_P(0);
    int len = 0;
    char * name_str = 0;
    int spi_res = 0;
    int rtseq = 0;
    int idtproc = 0;
    int8 tid;
    TupleDesc ttupdesc;
    SPITupleTable *ttuptable;
    HeapTuple ttuple;
    char * tid_str;
    int rtins;
    Oid * toids;
    Datum * tvals;

    const char* test_sql = "insert into ttt (id, name) values ($1, \'$2\');";
    const char* tSeqSql = "select getnextseq(\'ttt\', \'id\');";

    len = VARSIZE(name_text)-VARHDRSZ;
    name_str = (char *)palloc(len + 2);
    snprintf(name_str, len+1, "%s", (const char *)VARDATA(name_text));
    elog (INFO, "%s\n", name_str);

    spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT)
    {
        elog(ERROR, "Cannot connect via SPI");
        PG_RETURN_INT32(spi_res);
    }
    
    
    rtseq = SPI_execute (tSeqSql, true, 1L);
    idtproc = SPI_processed;
    if (rtseq != SPI_OK_SELECT || idtproc != 1)
    {
        SPI_finish ();
        PG_RETURN_INT32 (rtseq);
    }
    
    ttupdesc = SPI_tuptable->tupdesc;
    ttuptable = SPI_tuptable;
    ttuple = ttuptable->vals[0];
    tid_str = SPI_getvalue (ttuple, ttupdesc, 1);
    tid = atol (tid_str);
    elog (INFO, "%d \n", tid);

    {
    const int ntargs = 2;
    char * tnulls = '\0';
    toids = (Oid *)palloc (ntargs*sizeof (Oid));
    tvals = (Datum *)palloc (ntargs*sizeof (Datum));

    toids[0] = INT8OID;
    toids[1] = TEXTOID;//VARCHAROID;
    tvals[0] = Int32GetDatum (tid);
    tvals[1] = CStringGetDatum (name_str);
    elog (INFO, "%d %s \n", tid, name_str);
    rtins = SPI_execute_with_args (test_sql, ntargs, toids, tvals, tnulls, false, 1L);
    }
    
    SPI_finish ();
    
    if (rtins != SPI_OK_INSERT)
    {
        PG_RETURN_INT32 (-2);
    }
    
    pfree (toids);
    pfree (tvals);
    PG_RETURN_INT32 (tid);
}

