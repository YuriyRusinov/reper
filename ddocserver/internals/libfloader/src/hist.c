#include <gsl/gsl_histogram.h>
#include <stdio.h>
#include <postgres.h>
#include <funcapi.h>
#include "floader.h"

PG_FUNCTION_INFO_V1 (histogram);

Datum histogram(PG_FUNCTION_ARGS)
{
    size_t nr_rand = strlen ( VARDATA (PG_GETARG_VARCHAR_P(0)));
    char * sql = (char *) palloc (nr_rand+1);
//    strncpy (sql, (char *)PG_GETARG_VARCHAR_P(0), nr_rand);
    snprintf (sql, nr_rand+1, "%s", VARDATA (PG_GETARG_VARCHAR_P (0)));
    float8 xmin = PG_GETARG_FLOAT8 (1);
    float8 xmax = PG_GETARG_FLOAT8 (2);
    unsigned int num = PG_GETARG_UINT32(3);

    //float8 * result = (float8 *) (palloc (sizeof(float8)));

    //*result= 5.0;

    gsl_histogram * gHist = gsl_histogram_alloc (num);
    elog (INFO, "query = %s\n", sql);
    gsl_histogram_set_ranges_uniform (gHist, (double)xmin, (double)xmax);
//    Datum d = PointerGetDatum (gHist);
    SPI_connect ();
    int retVal = SPI_execute (sql, true, 0);
    if (retVal != SPI_OK_SELECT)
    {
        elog (ERROR, "Cannot load values");
        SPI_finish ();
        PG_RETURN_NULL();
    }
    elog (INFO, "%d\n", retVal);
    unsigned long int nproc = SPI_processed;
    elog (INFO, "number of points is %lu, number of bins is %u", nproc, num);
//    SPI_finish ();
//    pfree (sql);
//    PG_RETURN_NULL();
    int i=0;

    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    for (; i< nproc; i++)
    {
        HeapTuple tuple = SPI_tuptable->vals[i];
        int ncolbuf = 1;
        char * x_str = SPI_getvalue (tuple, tupdesc, ncolbuf);
        if (!x_str)
            continue;
        double x= atof (x_str);
        //elog (INFO, "value=%f", x);
        int res = gsl_histogram_increment (gHist, x);
        if (res != 0)
        {
            elog (NOTICE, "Range error, value is %lf, min value is %lf, max value %lf", x, gsl_histogram_min(gHist), gsl_histogram_max (gHist) );
            continue;
        }
    }
    SPI_finish ();
    pfree (sql);
    elog (INFO, "Reading was completed\n");
//    Oid * oids = (Oid *)palloc (2*sizeof (Oid));
//    oids[0] = FLOAT8OID;
//    oids[1] = FLOAT8OID;
    if (get_call_result_type (fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
    {
        elog (ERROR, "Incorrect transformation");
        PG_RETURN_NULL ();
    }
    else
        elog (INFO, "Result init, number of attributes = %d", tupdesc->natts);

    FuncCallContext *funcctx;
    int call_cntr;
    int max_calls;
    AttInMetadata *attinmeta;
    unsigned long il;
    if (SRF_IS_FIRSTCALL())
    {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        funcctx->max_calls = num;
        if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                            "that cannot accept type record")));
        attinmeta = 0;//TupleDescGetAttInMetadata(tupdesc);
        funcctx->attinmeta = 0;//attinmeta;
        MemoryContextSwitchTo(oldcontext);
        funcctx->tuple_desc = BlessTupleDesc( tupdesc );
        elog (INFO, "1st row");
    }
    funcctx = SRF_PERCALL_SETUP();
    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    attinmeta = funcctx->attinmeta;
    HeapTuple tuple;
    if (call_cntr < max_calls)
    {
        elog (INFO, "Rows");
        il = call_cntr;
        Datum * hvalues = (Datum *)palloc (2*sizeof(Datum));
        double val = gsl_histogram_get (gHist, il);
        hvalues[0] = UInt32GetDatum (il);
        hvalues[1] = Float8GetDatum (val);
        bool * nulls = palloc( 2 * sizeof( bool ) );

        elog (INFO, "%lu", il);
        tuple = heap_form_tuple( tupdesc, hvalues, nulls);
        elog (INFO, "%lu", il);
        //BuildTupleFromCStrings (attinmeta, hvalues);
        pfree (nulls);
        pfree (hvalues);
        SRF_RETURN_NEXT(funcctx, HeapTupleGetDatum( tuple ) );
    }
    else
    {
        SRF_RETURN_DONE(funcctx);
    }

    //PG_RETURN_DATUM (d);//POINTER (result);//gHist);
    //PG_RETURN_DATUM( HeapTupleGetDatum( tuple ) );
}
