#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdio.h>
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
    void * randBuf = malloc (nr);
//    elog(WARNING, "size of is %u\n", nr);
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

    size_t nr_ins = strlen ("insert into rand_state (state_rand) values ($1);");
    char* r_sql = (char *) palloc (nr_ins + 1);
    FILE * srSql = fmemopen (r_sql, nr_ins, "wb");
    fwrite ("insert into rand_state (state_rand) values ($1);\0", 1, nr_ins, srSql);
    elog (INFO, "insert query is '%s'\n", r_sql);

    fclose (srSql);
    elog (INFO, "insert query is '%s'\n", r_sql);
    Oid * oids = (Oid *)palloc (sizeof (Oid));
    Datum * val = (Datum *)palloc (sizeof (Datum));
    char * sval = (char *)palloc (nr);
    FILE * frStr = fmemopen (sval, nr, "wb");
    fwrite (randBuf, nr, 1, frStr);
    fclose (frStr);
    *val = PointerGetDatum (sval);//randBuf);
    elog (INFO, "value=%p randbuf=%p\n cvalue=%s\n", val[0], randBuf, sval);
    *oids = BYTEAOID;
    char nulls[1];//= (char *)(palloc (1));
    nulls[0] = '\0';
    int rins = SPI_execute_with_args (r_sql, 1, oids, val, nulls, false, 1);
    if (rins != SPI_OK_INSERT)
    {
        SPI_finish ();
        elog(ERROR, "Cannot insert random numbers generation via SPI");
        pfree (oids);
        pfree (r_sql);
        PG_RETURN_INT32 (rins);
    }
    SPI_finish();
    pfree (oids);
    pfree (r_sql);

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
        PG_RETURN_INT32(-1);
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
            PG_RETURN_INT32(-1);
        }
        pfree (isNull);
        FILE * fRand = fmemopen (randBuf, nr, "r");
        if (!fRand)
            PG_RETURN_INT32(-1);
        res = gsl_rng_fread (fRand, r);
    }
    SPI_finish();
    pfree (r_sql);
    PG_RETURN_INT32(res);
}
