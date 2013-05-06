#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdio.h>
#include <postgres.h>
#include "floader.h"

gsl_rng * r = 0;

PG_FUNCTION_INFO_V1 (initrand);

Datum initrand(PG_FUNCTION_ARGS)
{
    r = gsl_rng_alloc (gsl_rng_default);

    unsigned long int seed0 = PG_GETARG_UINT32(0);
    gsl_rng_set (r, seed0);
//    gsl_rng_env_setup();
    PG_RETURN_INT32(0);
}

PG_FUNCTION_INFO_V1 (droprand);

Datum droprand(PG_FUNCTION_ARGS)
{
    if (r)
    {
        gsl_rng_free (r);
        r = 0;
    }
    PG_RETURN_INT32(0);
}

PG_FUNCTION_INFO_V1 (unirand);

Datum unirand(PG_FUNCTION_ARGS)
{
    double res = gsl_rng_uniform (r);
    PG_RETURN_FLOAT8(res);
}

PG_FUNCTION_INFO_V1 (gaussrand);

Datum gaussrand(PG_FUNCTION_ARGS)
{
    double sigma = PG_GETARG_FLOAT8 (0);
    if (sigma < 0.0)
    {
        elog(ERROR, "standard of normal distribution cannot be negative. Please enter valid value.");
        PG_RETURN_NULL();
    }
    double res = gsl_ran_gaussian (r, sigma);
    elog(NOTICE, "res=%lf", res);
    PG_RETURN_FLOAT8(res);
}

PG_FUNCTION_INFO_V1 (exprand);

Datum exprand(PG_FUNCTION_ARGS)
{
    double alamb = PG_GETARG_FLOAT8 (0);
    if (alamb < 0.1e-15)
    {
        elog(ERROR, "mean of exponential distribution cannot be zero or negative. Please enter valid value.");
        PG_RETURN_NULL();
    }
    double mu = 1.0/alamb;
    double res = gsl_ran_exponential (r, mu);
    PG_RETURN_FLOAT8(res);
}

Datum saverand(PG_FUNCTION_ARGS)
{
    if (!r)
        PG_RETURN_INT32(-1);
    size_t nr = gsl_rng_size (r);
    bytea * randBuf = (bytea *) palloc(nr*sizeof (bytea));
    elog(INFO, "size of gsl_rng_size = %zu\n", nr);
    FILE * fRand = fmemopen (randBuf, nr, "wb");
    if (!fRand)
        PG_RETURN_INT32(-2);
    int res = gsl_rng_fwrite (fRand, r);
    int spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT)
    {
        elog(ERROR, "Cannot connect via SPI");
        PG_RETURN_INT32(-3);
    }
    fclose (fRand);

//    size_t nr_ins = strlen ("insert into rand_state (id) values ($1);");
    const char* r_sql = "insert into rand_state (id, state_rand) values ($1, $2)";//(char *) palloc (nr_ins + 1);

/*    const char* test_sql = "insert into ttt (id, name) values ($1, $2);";
    const char* tSeqSql = "select getnextseq(\'ttt\', \'id\');";
    int rtseq = SPI_execute (tSeqSql, true, 1);
    int idtproc = SPI_processed;
    if (rtseq != SPI_OK_SELECT || idtproc != 1)
    {
        SPI_finish ();
        pfree (randBuf);
        PG_RETURN_INT32 (-4);
    }
    int8 tid;
    TupleDesc ttupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *ttuptable = SPI_tuptable;
    HeapTuple ttuple = ttuptable->vals[0];
    char * tid_str = SPI_getvalue (ttuple, ttupdesc, 1);
    tid = atol (tid_str);
    const int ntargs = 2;
    char * tnulls = "";
    Oid * toids = (Oid *)palloc (ntargs*sizeof (Oid));
    toids[0] = INT8OID;
    toids[1] = VARCHAROID;
    Datum * tvals = (Datum *)palloc (ntargs*sizeof (Datum));
    tvals[0] = Int32GetDatum (tid);
    char * name_str = (char *)palloc (25*sizeof(char));
    strcpy (name_str, "aagadfgadf");
    elog (INFO, "%s\n", name_str);
    tvals[1] = CStringGetDatum (name_str);
    int rtins = SPI_execute_with_args (test_sql, ntargs, toids, tvals, tnulls, false, 1L);
    SPI_finish ();
    if (rtins != SPI_OK_INSERT)
    {
        pfree (randBuf);
        PG_RETURN_INT32 (-4);
    }
    pfree (toids);
    pfree (tvals);
    PG_RETURN_INT32 (tid);
*/

    const int nargs = 2;
    char *nulls = '\0';//(char *)(palloc (nargs*sizeof(char)));
    Oid * oids = (Oid *)palloc (nargs*sizeof (Oid));
    oids[0] = INT8OID;
    oids[1] = BYTEAOID;
    Datum * vals = (Datum *)palloc (nargs*sizeof (Datum));
    vals[0] = Int32GetDatum(-1);
    const char* seqSql = "select getnextseq(\'rand_state\', \'id\');";
    int rseq = SPI_execute (seqSql, true, 1);
    int idproc = SPI_processed;
    if (rseq != SPI_OK_SELECT || idproc != 1)
    {
        SPI_finish ();
        //pfree (r_sql);
        pfree (randBuf);
        PG_RETURN_INT32 (-4);
    }
    int8 id;
    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *tuptable = SPI_tuptable;
    HeapTuple tuple = tuptable->vals[0];
    char * id_str = SPI_getvalue (tuple, tupdesc, 1);
    id = atol (id_str);
    elog (INFO, "id=%d\n", id);
    vals[0] = Int32GetDatum(id);
//    char * tst ="abc";
    vals[1] = PointerGetDatum (randBuf);
    elog (INFO, "value=%d id=%d\n", DatumGetInt32 (vals[0]), id);
//    oids[1] = BYTEAOID;
//    snprintf (r_sql, nr_ins+1, "insert into rand_state (id, state_rand) values ($1, $2);");
    elog (INFO, "%s\n", r_sql);
    int rins = SPI_execute_with_args (r_sql, nargs, oids, vals, nulls, false, 1L);
    if (rins != SPI_OK_INSERT)
    {
        SPI_finish ();
        elog(ERROR, "Cannot insert random numbers generation via SPI");
        pfree (oids);
//        pfree (nulls);
        pfree (vals);
//        pfree (r_sql);
        PG_RETURN_INT32 (rins);
    }
    pfree (oids);
//    pfree (nulls);
    pfree (vals);
//    pfree (r_sql);

    SPI_finish();

    PG_RETURN_INT32(res);

}

Datum loadrand(PG_FUNCTION_ARGS)
{
    if (!r)
        r = gsl_rng_alloc (gsl_rng_default);
    int spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT)
    {
        elog(ERROR, "Cannot connect via SPI");
        PG_RETURN_INT32(-1);
    }
    char * r_sql = (char *) palloc(strlen ("select state_rand from rand_state rs where rs.id=(select max(id) from rand_state);"));
    sprintf (r_sql, "select state_rand from rand_state rs where rs.id=(select max(id) from rand_state);");
    elog (WARNING, "read sql is %s\n", r_sql);
    int retVal = SPI_execute (r_sql, false, 1);
    if (retVal != SPI_OK_SELECT)
    {
        elog (ERROR, "Cannot load random generator state");
        PG_RETURN_INT32(-2);
    }
    int proc = SPI_processed;
    if (proc==0)
    {
        elog (WARNING, "No save states are inserted");
        SPI_finish();
        PG_RETURN_NULL();
    }
    int i;
    int res = -1;
    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    elog (INFO, "rows number=%d columns number=%d\n", proc, tupdesc->natts );
    size_t nr = gsl_rng_size (r);
    for (i=0; i<proc && res != 0; i++)
    {
        HeapTuple tuple = SPI_tuptable->vals[i];
        bool * isNull = (bool *) palloc (sizeof(bool));
        void * randBuf = (void *)SPI_getbinval (tuple, tupdesc, 1, isNull);
        if (*isNull || !randBuf)
        {
            elog (ERROR, "Incorrect data");
            PG_RETURN_INT32(-3);
        }
        pfree (isNull);
        FILE * fRand = fmemopen (randBuf, nr, "r");
        if (!fRand)
            PG_RETURN_INT32(-4);
        res = gsl_rng_fread (fRand, r);
    }
    SPI_finish();
    pfree (r_sql);
    PG_RETURN_INT32(res);
}
