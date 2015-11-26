#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdio.h>
#include <postgres.h>
#include "floader.h"

PG_FUNCTION_INFO_V1 (square_image);

Datum square_image (PG_FUNCTION_ARGS)
{
    bytea * imageData = PG_GETARG_BYTEA_P(0);

    int spi_res;
    spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT)
    {
        elog(ERROR, "Cannot connect via SPI");
        PG_RETURN_NULL();
    }
    int nr_length = strlen ("select length(null::bytea);");
    size_t n = length (imageData);
    char * xstr = "\\x";
    if (strstr ((char *)imageData, xstr))
    {
        elog(INFO, "image data was hexadecimal, size is %zu\n", n);
        PG_RETURN_FLOAT8 (1.0);
    }
    else
    {
        elog(INFO, "image data, size is  %zu\n", n);
        PG_RETURN_FLOAT8 (-1.0);
    }
    SPI_finish();

    PG_RETURN_FLOAT8 (0.0);
}
