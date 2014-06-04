#ifndef XGIS_STRUTILS_H
#define XGIS_STRUTILS_H

#include <string.h>
#include "configUtils.h"
#include "kks_port.h"

#define XSLT_HONOURSTRINGS      0x0001  //1
#define XSLT_ALLOWEMPTYTOKENS   0x0002  //2
#define XSLT_PRESERVEQUOTES     0x0004  //4
#define XSLT_PRESERVEESCAPES    0x0008  //8
#define XSLT_USESINGLEQUOTES    0x0010  //16

XGIS_C_START

//memory management (wrappers)
void  __EXPORT_UTILS           XGISFree( void * pData );
void  __EXPORT_UTILS           XGISDestroy(char **papszStrList);
void  __EXPORT_UTILS*          XGISRealloc( void * pData, size_t nNewSize );
void  __EXPORT_UTILS*          XGISCalloc( size_t nCount, size_t nSize );
void  __EXPORT_UTILS*          XGISMalloc( size_t );

//std memory management
void  __EXPORT_UTILS*          XSICalloc( size_t nCount, size_t nSize );
void  __EXPORT_UTILS*          XSIMalloc( size_t nSize );
void  __EXPORT_UTILS*          XSIRealloc( void * pData, size_t nNewSize );

XGIS_C_END

//string management
#define XGISES_BackslashQuotable 0
#define XGISES_XML               1
#define XGISES_URL               2   /* unescape only for now */
#define XGISES_SQL               3
#define XGISES_CSV               4

XGIS_C_START

char  __EXPORT_UTILS*          XGISEscapeString( const char *pszString, 
                                 int nLength, 
                                 int nScheme );
char  __EXPORT_UTILS*          XGISUnescapeString( const char *pszString, 
                                   int *pnLength,
                                   int nScheme );

char  __EXPORT_UTILS*          XSIStrdup( const char * pszString );

const char __EXPORT_UTILS*    XGISGetField( char ** papszStrList, int iField );
char  __EXPORT_UTILS*          XGISStrdup( const char * pszString );
int __EXPORT_UTILS            XGISCount(char **papszStrList);
const char __EXPORT_UTILS*    XGISSPrintf(const char *fmt, ...);

char __EXPORT_UTILS ** XGISAddString(char **papszStrList, const char *pszNewString);

double __EXPORT_UTILS         XGISAtof(const char *);

char __EXPORT_UTILS**         XGISTokenizeStringComplex(const char *pszString,
                                         const char *pszDelimiter,
                                         int bHonourStrings, 
                                         int bAllowEmptyTokens );

char __EXPORT_UTILS **         XGISTokenizeString2( const char * pszString,
                                    const char * pszDelimiters,
                                    int nCSLTFlags );



XGIS_C_END

//errors management
typedef enum
{
    XE_None = 0,
    XE_Debug = 1,
    XE_Warning = 2,
    XE_Failure = 3,
    XE_Fatal = 4
} XGISErr;

//Well known error codes
#define XGISE_None                       0
#define XGISE_AppDefined                 1
#define XGISE_OutOfMemory                2
#define XGISE_FileIO                     3
#define XGISE_OpenFailed                 4
#define XGISE_IllegalArg                 5
#define XGISE_NotSupported               6
#define XGISE_AssertionFailed            7
#define XGISE_NoWriteAccess              8
#define XGISE_UserInterrupt              9
#define XGISE_ObjectNull                 10

XGIS_C_START

void __EXPORT_UTILS XGISError(XGISErr eErrClass, int err_no, const char *fmt, ...);

void __EXPORT_UTILS _XGISAssert( const char *, const char *, int );

XGIS_C_END

#ifdef DEBUG
#  define XGISAssert(expr)  ((expr) ? (void)(0) : _XGISAssert(#expr,__FILE__,__LINE__))
#else
#  define XGISAssert(expr)
#endif

XGIS_C_START
//double management
double __EXPORT_UTILS XGISAtof(const char *);
double __EXPORT_UTILS XGISStrtod(const char *, char **);
double __EXPORT_UTILS XGISStrtodDelim(const char *, char **, char);

XGIS_C_END

#endif
