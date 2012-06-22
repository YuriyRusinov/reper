#ifndef _FLOADER_CONFIG_H_
#define _FLOADER_CONFIG_H_


/* ==================================================================== */
/*      We will use WIN32 as a standard windows define.                 */
/* ==================================================================== */
#if defined(_WIN32) && !defined(WIN32)
    #define WIN32
#endif

#if defined(_WINDOWS) && !defined(WIN32)
    #define WIN32
#endif

/* ==================================================================== */
/*      Standard include files.                                         */
/* ==================================================================== */

#ifdef WIN32
    #ifndef _ERRCODE_DEFINED
        #define _ERRCODE_DEFINED
    #endif
    typedef int errno_t;
#endif

#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS 64

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>


#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

// assertion suport
#include <assert.h>

#if defined( WIN32 )
    #if defined( FLOADER_DLL )
        #define FLOADER_EXPORT __declspec(dllexport)
    #else
        #define FLOADER_EXPORT __declspec(dllimport)
    #endif

    #ifndef DLLIMPORT
        #define DLLIMPORT __declspec(dllexport)
    #endif

#else
    #define FLOADER_EXPORT
#endif /* FLOADER_DLL */

#ifndef NULL
    #define NULL  0
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE  1
#endif

#ifndef MAX
    #define MIN(a,b)    ((a<b) ? a : b)
    #define MAX(a,b)    ((a>b) ? a : b)
#endif

#ifndef ABS
    #define ABS(x)      ((x<0) ? (-1*(x)) : x)
#endif

/* -------------------------------------------------------------------- */
/*      Macro to test equality of two floating point values.            */
/*      We use fabs() function instead of ABS() macro to avoid side     */
/*      effects.                                                        */
/* -------------------------------------------------------------------- */
#ifndef EQUAL_F
    #define EQUAL_F(x,y) (fabs((x) - (y)) < 0.0000000000001)
#endif

#ifndef EQUAL_S
    #if defined(WIN32)
        #define EQUAL_SN(a,b,n) (_strnicmp(a,b,n)==0)
        #define EQUAL_S(a,b)    (_stricmp(a,b)==0)
        #define MKDIR(a, b)     _mkdir(a)
    #else
        #define EQUAL_SN(a,b,n) (strncasecmp(a,b,n)==0)
        #define EQUAL_S(a,b)    (strcasecmp(a,b)==0)
        #define MKDIR(a, b)     mkdir(a, b)
    #endif
#endif

#if defined(WIN32)
    #define snprintf _snprintf
#endif

static const char floader_version[] = "LibFilesLoader Version 0.14.0";

#endif /* _FLOADER_CONFIG_H_ */
