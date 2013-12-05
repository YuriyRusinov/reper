#include <gsl/gsl_histogram.h>
#include <stdio.h>
#include <postgres.h>
#include "floader.h"

PG_FUNCTION_INFO_V1 (histogram);

Datum histogram(PG_FUNCTION_ARGS)
{
    unsigned long int num = PG_GETARG_UINT32(0);

    float8 * result = (float8 *) (palloc (sizeof(float8)));

    *result= 5.0;

    //gsl_histogram * gHist = gsl_histogram_alloc (num);

    PG_RETURN_POINTER (result);//gHist);
}

typedef struct Complex {
    double      x;
    double      y;
} Complex;

Datum complex_in(PG_FUNCTION_ARGS)
{
    char       *str = PG_GETARG_CSTRING(0);
    double      x,
                y;
    Complex    *result;

    if (sscanf(str, " ( %lf , %lf )", &x, &y) != 2)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input syntax for complex: \"%s\"",
                        str)));

    result = (Complex *) palloc(sizeof(Complex));
    result->x = x;
    result->y = y;
    PG_RETURN_POINTER(result);
}
