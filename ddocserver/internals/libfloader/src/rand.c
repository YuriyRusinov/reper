#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdio.h>
#include "floader.h"

gsl_rng * r = 0;

PG_FUNCTION_INFO_V1 (initrand);

Datum initrand(PG_FUNCTION_ARGS)
{
    r = gsl_rng_alloc (gsl_rng_default);
    gsl_rng_set (r, 1);
    gsl_rng_env_setup();
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
    void * randBuf = malloc (sizeof (gsl_rng));
    elog(WARNING, "size of is %lu\n", sizeof (gsl_rng));
    FILE * fRand = fmemopen (randBuf, sizeof(gsl_rng), "w");
    if (!fRand)
        PG_RETURN_INT32(-1);
    int res = gsl_rng_fwrite (fRand, r);
    int spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT)
    {
        elog(ERROR, "Cannot connect via SPI");
        PG_RETURN_INT32(-1);
    }

    char* r_sql = (char *) palloc (strlen ("insert into rand_state (state) values (") + sizeof (gsl_rng) + 2);
    sprintf (r_sql, "insert into rand_state (state_rand) values ('%s');", (char *)(randBuf));
    SPI_execute (r_sql, false, 1);
    SPI_finish();

    PG_RETURN_INT32(res);
}
