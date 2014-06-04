#if !defined(__Utils_config_utils_h)
#define __Utils_config_utils_h
/* ==================================================================== */
/*      We will use WIN32 as a standard windows define.                 */
/* ==================================================================== */
#if defined(_WIN32) && !defined(WIN32)
    #define WIN32
#endif

#if defined(_WINDOWS) && !defined(WIN32)
    #define WIN32
#endif


#ifdef WIN32

#include <windows.h>
#pragma warning (disable:4251)
#pragma warning (disable:4530)
#ifdef __USE_DLL_KKSUTILS

	#define __EXPORT_UTILS __declspec(dllexport)
#else
	#define __EXPORT_UTILS __declspec(dllimport)
#endif
#else
	#define __EXPORT_UTILS
#endif

#include <math.h>
#include <stdio.h>

#ifndef MAX
    #define MIN(a,b)    ((a<b) ? a : b)
    #define MAX(a,b)    ((a>b) ? a : b)
#endif

#ifndef ABS
    #define ABS(x)      ((x<0) ? (-1*(x)) : x)
#endif

#ifndef EQUAL_F
    #define EQUAL_F(x,y) (fabs((x) - (y)) < 0.0000000000001)
#endif

#ifndef EQUAL_S
    #if defined(WIN32)
        #define EQUAL_SN(a,b,n) (_strnicmp(a,b,n)==0)
        #define EQUAL_S(a,b)    (_stricmp(a,b)==0)
    #else
        #define EQUAL_SN(a,b,n) (strncasecmp(a,b,n)==0)
        #define EQUAL_S(a,b)    (strcasecmp(a,b)==0)
    #endif
#endif

typedef unsigned int UINT;

#if defined(WIN32)
    #define snprintf _snprintf
#endif
#if defined(WIN32)
    #define strdup _strdup
#endif


#endif

