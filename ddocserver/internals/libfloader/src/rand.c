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

Datum saveRand(PG_FUNCTION_ARGS)
{
    if (!r)
        PG_RETURN_INT32(-1);
}
