#include <gsl/gsl_rng.h>
#include "floader.h"

gsl_rng * r = 0;

Datum initrand(PG_FUNCTION_ARGS)
{
    r = gsl_rng_alloc (gsl_rng_default);
    gsl_rng_set (r, 1);
    PG_RETURN_INT32(0);
}

Datum droprand(PG_FUNCTION_ARGS)
{
    if (r)
        gsl_rng_free (r);
    PG_RETURN_INT32(0);
}

Datum unirand(PG_FUNCTION_ARGS)
{
    double res = gsl_rng_uniform (r);
    PG_RETURN_FLOAT8(res);
}

Datum gaussrand(PG_FUNCTION_ARGS)
{
    double res = gsl_ran_ugaussian (r);
    PG_RETURN_FLOAT8(res);
}

Datum exprand(PG_FUNCTION_ARGS)
{
    double res = gsl_ran_exponential (r, 0.1e1l);
    PG_RETURN_FLOAT8(res);
}


