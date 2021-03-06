#if !defined(__dataaccess_config_h)
#define __dataaccess_config_h
/* ==================================================================== */
/*      We will use WIN32 as a standard windows define.                 */
/* ==================================================================== */
#if defined(_WIN32) && !defined(WIN32)
    #define WIN32
#endif

#if defined(_WINDOWS) && !defined(WIN32)
    #define WIN32
#endif

/* ������������ �����������*/
#define USE_NOTIFICATION 1


#ifdef WIN32

#include <windows.h>
#pragma warning (disable:4251)
#pragma warning (disable:4530)
#ifdef __USE_DLL_KKSDATAACCESS

	#define __DA_EXPORT __declspec(dllexport)
#else
	#define __DA_EXPORT __declspec(dllimport)
#endif
#else
	#define __DA_EXPORT
#endif

#include <math.h>
#include <stdio.h>

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

#if defined( WIN32 )
	#define __PRETTY_FUNCTION__ __FUNCTION__
#else
	#define __FUNCTION__ __PRETTY_FUNCTION__
#endif

#if defined(WIN32)
    #define snprintf _snprintf
#endif
#if defined(WIN32)
    #define strdup _strdup
#endif


#endif

