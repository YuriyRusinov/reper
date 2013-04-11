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
FLOADER_EXPORT Datum initrand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum droprand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum unirand (PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum gaussrand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum exprand(PG_FUNCTION_ARGS);
//FLOADER_EXPORT Datum rxpm2jpeg(PG_FUNCTION_ARGS);

FLOADER_EXPORT Datum initrand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum droprand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum unirand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum gaussrand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum exprand(PG_FUNCTION_ARGS);
FLOADER_EXPORT Datum saveRand(PG_FUNCTION_ARGS);






text* cstring_to_text_ex(const char *);
char* text_to_cstring_ex(text *);
FILE * openFile(FILE * f, char * url, char * am);

//int xpm2jpeg(char *xpm, unsigned long xpmsize,
//                            unsigned char **pjpeg, unsigned long *pjpegsize);

#endif

