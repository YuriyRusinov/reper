#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <stdio.h>
#include <postgres.h>
#include <executor/spi.h>
#include <utils/builtins.h>
#include "floader.h"

PG_FUNCTION_INFO_V1 (square_image);

Datum square_image (PG_FUNCTION_ARGS)
{
    bytea * imageData = PG_GETARG_BYTEA_P(0);
//    elog (INFO, "argument is %s\n", imageData);
//    text * tImageData = PG_GETARG_TEXT_P (0);
//    char * bImageStr = text_to_cstring (tImageData);

    int spi_res;
    spi_res = SPI_connect();
    if(spi_res != SPI_OK_CONNECT)
    {
        elog(ERROR, "Cannot connect via SPI");
        PG_RETURN_NULL();
    }
    int nr_length = strlen ("select length($1::bytea);");
    size_t n = sizeof (imageData);
    char * command_image_size = (char *)palloc(nr_length+1);
    const int nargs = 1;
    Oid * oids = (Oid *)palloc (nargs*sizeof (Oid));
    oids[0] = BYTEAOID;
    Datum * vals = (Datum *)palloc (nargs*sizeof (Datum));
    snprintf (command_image_size, nr_length, "select length($1::bytea);");
    vals[0] = PointerGetDatum (imageData);
    const char * nulls = 0;
    int rSize = SPI_execute_with_args (command_image_size, nargs, oids, vals, nulls, true, 1L);
    int idproc = SPI_processed;
    if (rSize != SPI_OK_SELECT || idproc != 1)
    {
        SPI_finish ();
        pfree (command_image_size);
        pfree (oids);
        pfree (vals);
        PG_RETURN_FLOAT8 (rSize);
    }
    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *tuptable = SPI_tuptable;
    HeapTuple tuple = tuptable->vals[0];
    char * n_str = SPI_getvalue (tuple, tupdesc, 1);
    n = atoi (n_str);
    elog(INFO, "image data, size is  %zu\n", n);
    pfree (n_str);
/*
    char * xstr = "\\x";

     if (position (xstr, imageData)==1)//strstr ((char *)imageData, xstr))
    {
        elog(INFO, "image data was hexadecimal, size is %zu\n", n);
        //PG_RETURN_FLOAT8 (1.0);
    }
    else
    {
        elog(INFO, "image data, size is  %zu\n", n);
        //PG_RETURN_FLOAT8 (-1.0);
    }
*/
    pfree (command_image_size);
    nr_length = strlen ("select convert_from($1::bytea, \'UTF8\');");
    char * command_convert = (char *)palloc (nr_length+1);
    snprintf (command_convert, nr_length, "select convert_from($1::bytea, \'UTF8\');");
    int rConv = SPI_execute_with_args (command_convert, nargs, oids, vals, nulls, true, 1L);
    idproc = SPI_processed;
    if (rConv != SPI_OK_SELECT || idproc != 1)
    {
        SPI_finish ();
        pfree (command_convert);
        pfree (oids);
        pfree (vals);
        PG_RETURN_FLOAT8 (rSize);
    }
    TupleDesc tupdescC = SPI_tuptable->tupdesc;
    SPITupleTable *tuptableC = SPI_tuptable;
    HeapTuple tupleC = tuptableC->vals[0];
    char * nconv_str = SPI_getvalue (tupleC, tupdescC, 1);
    int az_dd;
    int elev_dd;
    int nw;
    int nh;
    int depth;
    char * im_str = (char *) palloc (strlen (nconv_str)+1);
    sscanf (nconv_str, "%d %d %d %d %d %s", &az_dd, &elev_dd, &nw, &nh, &depth, im_str);
    elog (INFO, "converted data are azimuth=%d elevation_angle=%d width=%d height=%d depth=%d image string is %s \n", az_dd, elev_dd, nw, nh, depth, im_str);
    int n_image = strlen (im_str);
    double sq = 0.0;
    int i=0;
    for (; i<n_image; i++)
        if (im_str[i] != '0')
            sq++;

    pfree (im_str);
    pfree (nconv_str);

    pfree (command_convert);
    pfree (oids);
    pfree (vals);
    SPI_finish();

    PG_RETURN_FLOAT8 (sq);
}
