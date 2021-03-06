#ifndef _FLOADER_H_INCLUDED
#define _FLOADER_H_INCLUDED

#include "floader_config.h"

#include <postgres.h>
#include <funcapi.h>
#include <fmgr.h>
#include <catalog/pg_type.h>
#include <executor/spi.h>
#include <sys/stat.h>
#include <utils/builtins.h>

#include <stdio.h>

//hack for compatibility with 8.3 and <8.3
#ifndef SET_VARSIZE
#define SET_VARSIZE(PTR, len) (VARATT_SIZEP(PTR) = (len))
#endif

#ifndef VARSIZE
#define VARSIZE(PTR) (VARATT_SIZEP(PTR))
#endif


FLOADER_EXPORT Datum exec_spi(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum exec_spi_array(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum rgetfile(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum rgetfilebyurl(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum rgetfilesizebyurl(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum rwritefile(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum rgetabsurlex(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum rdeletefile(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum kkssitoversion(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum rxpm2jpeg(PG_FUNCTION_ARGS);

FLOADER_EXPORT Datum rhashfile(PG_FUNCTION_ARGS);

FLOADER_EXPORT Datum initrand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum droprand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum unirand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum gaussrand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum exprand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum saveRand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum loadRand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum testargs(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum testspi(PG_FUNCTION_ARGS);

FLOADER_EXPORT Datum histogram(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum square_image(PG_FUNCTION_ARGS);



FLOADER_EXPORT text* cstring_to_text_ex(const char *);
FLOADER_EXPORT char* text_to_cstring_ex(text *);
FLOADER_EXPORT FILE * openFile(FILE * f, const char * url, char * am);
FLOADER_EXPORT unsigned char * hashFile(const char * url, int * digestLength);

int xpm2jpeg(char *xpm, unsigned long xpmsize,
                            unsigned char **pjpeg, unsigned long *pjpegsize);

#endif

