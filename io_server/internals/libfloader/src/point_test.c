#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdio.h>
#include <postgres.h>
#include "floader.h"

PG_FUNCTION_INFO_V1 (test_function_array);

Datum test_function_array (PG_FUNCTION_ARGS)
{
    text * imageData = PG_GETARG_TEXT_P(0);

    int spi_res;
    spi_res = SPI_connect();
    SPI_finish();


    PG_RETURN_FLOAT8 (0.0);
}
