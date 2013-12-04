#include <gsl/gsl_histogram.h>
#include <stdio.h>
#include <postgres.h>
#include "floader.h"

PG_FUNCTION_INFO_V1 (histogram);

Datum histogram(PG_FUNCTION_ARGS)
{
    unsigned long int num = PG_GETARG_UINT32(0);

    //gsl_histogram * gHist = gsl_histogram_alloc (num);

    PG_RETURN_POINTER (0);//gHist);
}
