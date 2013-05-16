#include <stdio.h>
#include <postgres.h>
#include <executor/spi.h>
#include "floader.h"

PG_FUNCTION_INFO_V1 (testspi);

Datum testspi(PG_FUNCTION_ARGS)
{
    text * sql_text = PG_GETARG_TEXT_P(0);
    bool isReadOnly = PG_GETARG_BOOL(1);
    int cnt = PG_GETARG_INT32 (2);
    char * s_sql = NULL;
    int len = VARSIZE (sql_text)-VARHDRSZ;
    SPITupleTable * tuptable;
    s_sql = (char *)palloc(len + 2);
    snprintf(s_sql, len+1, "%s", (const char *)VARDATA(sql_text));

    SPI_connect();
    int ret = SPI_execute (s_sql, isReadOnly, cnt);
    elog (INFO, "sql is %s\n", s_sql);
    tuptable = SPI_tuptable;
    if (tuptable)
    {
        //elog (INFO, "tuptable is %s\n", tuptable->tupdesc);
        int proc = SPI_processed;
        elog (INFO, "row processed %d\n", proc);
        elog (INFO, "columns processed %d\n", tuptable->tupdesc->natts );
        //elog (INFO, "table name %s\n", tuptable->tuptabcxt->name);
//        elog (INFO, "table name %s\n", tuptable->tuptabcxt->name);
        int i, j;
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
    len = VARSIZE(name_text)-VARHDRSZ;
    name_str = (char *)palloc(len + 2);
    snprintf(name_str, len+1, "%s", (const char *)VARDATA(name_text));
    elog (INFO, "%s\n", name_str);

    int spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT)
    {
        elog(ERROR, "Cannot connect via SPI");
        PG_RETURN_INT32(spi_res);
    }
    const char* test_sql = "insert into ttt (id, name) values ($1, \'$2\');";
    const char* tSeqSql = "select getnextseq(\'ttt\', \'id\');";
    int rtseq = SPI_execute (tSeqSql, true, 1L);
    int idtproc = SPI_processed;
    if (rtseq != SPI_OK_SELECT || idtproc != 1)
    {
        SPI_finish ();
        PG_RETURN_INT32 (rtseq);
    }
    int8 tid;
    TupleDesc ttupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *ttuptable = SPI_tuptable;
    HeapTuple ttuple = ttuptable->vals[0];
    char * tid_str = SPI_getvalue (ttuple, ttupdesc, 1);
    tid = atol (tid_str);
    elog (INFO, "%d \n", tid);

    const int ntargs = 2;
    char * tnulls = '\0';
    Oid * toids = (Oid *)palloc (ntargs*sizeof (Oid));
    toids[0] = INT8OID;
    toids[1] = TEXTOID;//VARCHAROID;
    Datum * tvals = (Datum *)palloc (ntargs*sizeof (Datum));
    tvals[0] = Int32GetDatum (tid);
    tvals[1] = CStringGetDatum (name_str);
    elog (INFO, "%d %s \n", tid, name_str);
    int rtins = SPI_execute_with_args (test_sql, ntargs, toids, tvals, tnulls, false, 1L);
    SPI_finish ();
    if (rtins != SPI_OK_INSERT)
    {
        PG_RETURN_INT32 (-2);
    }
    pfree (toids);
    pfree (tvals);
    PG_RETURN_INT32 (tid);
}

