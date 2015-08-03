#include <gsl/gsl_histogram.h>
#include <stdio.h>
#include <postgres.h>
#include <funcapi.h>
#include "floader.h"

PG_FUNCTION_INFO_V1 (histogram);

Datum histogram(PG_FUNCTION_ARGS)
{
    char * sql = text_to_cstring_ex(PG_GETARG_TEXT_P(0));
    elog (INFO, "query = %s\n", sql);

    float8 xmin = PG_GETARG_FLOAT8 (1);
    float8 xmax = PG_GETARG_FLOAT8 (2);
    unsigned int num = PG_GETARG_UINT32(3);


    gsl_histogram * gHist = gsl_histogram_alloc (num);
    gsl_histogram_set_ranges_uniform (gHist, (double)xmin, (double)xmax);

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

//    if (get_call_result_type (fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
//    {
//        elog (ERROR, "Incorrect transformation");
//        PG_RETURN_NULL ();
//    }
//    else
//        elog (INFO, "Result init, number of attributes = %d", tupdesc->natts);

    FuncCallContext *funcctx;
    int call_cntr;
    int max_calls;
    AttInMetadata *attinmeta;
    unsigned long il;
    TupleDesc tupdescRes;
    if (SRF_IS_FIRSTCALL())
    {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        funcctx->max_calls = num;
        Oid * oids = (Oid *)palloc (2*sizeof (Oid));
        oids[0] = INT8OID;
        oids[1] = FLOAT8OID;
        if (get_call_result_type(fcinfo, oids, &tupdescRes) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                            "that cannot accept type record")));
        attinmeta = TupleDescGetAttInMetadata(tupdescRes);
        funcctx->attinmeta = attinmeta;
        MemoryContextSwitchTo(oldcontext);
        //funcctx->tuple_desc = BlessTupleDesc( tupdescRes );
        pfree (oids);
        elog (INFO, "1st row");
    }
    funcctx = SRF_PERCALL_SETUP();
    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    elog (INFO, "number of rows = %d, number of columns are %d", max_calls, tupdescRes->natts );
    attinmeta = funcctx->attinmeta;
    HeapTuple tuple;
    if (call_cntr < max_calls)
    {
        elog (INFO, "Rows");
        il = call_cntr;
        //Datum * hvalues = (Datum *)palloc (2*sizeof(Datum));

        char ** svals = (char **) palloc (2*sizeof (char*));
        svals [0] = (char *) palloc(5 * sizeof(char));
        svals [1] = (char *) palloc(20 * sizeof(char));
        double val = gsl_histogram_get (gHist, il);
        snprintf (svals[0], 5, "%lu", il);
        snprintf (svals[1], 20, "%lf", val);
        //hvalues[0] = UInt32GetDatum (il);
        //hvalues[1] = Float8GetDatum (val);
        bool * nulls = palloc( 2 * sizeof( bool ) );

        elog (INFO, "%lu", il);
        tuple = BuildTupleFromCStrings (attinmeta, svals);//heap_form_tuple( tupdescRes, hvalues, nulls);
        elog (INFO, "%lu", il);
        Datum result = HeapTupleGetDatum(tuple);
        pfree (nulls);
        pfree (svals[1]);
        pfree (svals[0]);
        pfree (svals);
        //pfree (hvalues);
        SRF_RETURN_NEXT(funcctx, result);//HeapTupleGetDatum( tuple ) );
    }
    else
    {
        SRF_RETURN_DONE(funcctx);
    }

}
