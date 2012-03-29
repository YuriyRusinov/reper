#include "strutils.h"
#include <stdarg.h>
#include <locale.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>

void XGISFree( void * pData )
{
    if( pData != NULL )
        free( pData );
}


/**********************************************************************
 *                       XGISDestroy()
 *
 * Free all memory used by a StringList.
 **********************************************************************/
void XGISDestroy(char **papszStrList)
{
    char **papszPtr;

    if (papszStrList)
    {
        papszPtr = papszStrList;
        while(*papszPtr != NULL)
        {
            XGISFree(*papszPtr);
            papszPtr++;
        }

        XGISFree(papszStrList);
    }
}

/************************************************************************/
/*                             XSICalloc()                              */
/************************************************************************/

void *XSICalloc( size_t nCount, size_t nSize )

{
    return( calloc( nCount, nSize ) );
}

/************************************************************************/
/*                             XGISCalloc()                              */
/************************************************************************/

/**
 * Safe version of calloc().
 *
 * This function is like the C library calloc(), but raises a CE_Fatal
 * error with XGISError() if it fails to allocate the desired memory.  It
 * should be used for small memory allocations that are unlikely to fail
 * and for which the application is unwilling to test for out of memory
 * conditions.  It uses XSICalloc() to get the memory, so any hooking of
 * XSICalloc() will apply to XGISCalloc() as well.  XGISFree() or XSIFree()
 * can be used free memory allocated by XGISCalloc().
 *
 * @param nCount number of objects to allocate.
 * @param nSize size (in bytes) of object to allocate.
 * @return pointer to newly allocated memory, only NULL if nSize * nCount is
 * NULL.
 */

void *XGISCalloc( size_t nCount, size_t nSize )

{
    void        *pReturn;

    if( nSize * nCount == 0 )
        return NULL;
    
    pReturn = XSICalloc( nCount, nSize );

    return pReturn;
}

/************************************************************************/
/*                            XGISGetField()                             */
/*                                                                      */
/*      Fetches the indicated field, being careful not to crash if      */
/*      the field doesn't exist within this string list.  The           */
/*      returned pointer should not be freed, and doesn't               */
/*      necessarily last long.                                          */
/************************************************************************/

const char * XGISGetField( char ** papszStrList, int iField )
{
    int         i;

    if( papszStrList == NULL || iField < 0 )
        return( "" );

    for( i = 0; i < iField+1; i++ )
    {
        if( papszStrList[i] == NULL )
            return "";
    }

    return( papszStrList[iField] );
}

/************************************************************************/
/*                             XSIMalloc()                              */
/************************************************************************/

void *XSIMalloc( size_t nSize )

{
    return( malloc( nSize ) );
}

/************************************************************************/
/*                             XSIRealloc()                             */
/************************************************************************/

void * XSIRealloc( void * pData, size_t nNewSize )

{
    return( realloc( pData, nNewSize ) );
}


/************************************************************************/
/*                             XGISMalloc()                              */
/************************************************************************/

/**
 * Safe version of malloc().
 *
 * This function is like the C library malloc(), but raises a CE_Fatal
 * error with XGISError() if it fails to allocate the desired memory.  It
 * should be used for small memory allocations that are unlikely to fail
 * and for which the application is unwilling to test for out of memory
 * conditions.  It uses XSIMalloc() to get the memory, so any hooking of
 * XSIMalloc() will apply to XGISMalloc() as well.  XGISFree() or XSIFree()
 * can be used free memory allocated by XGISMalloc().
 *
 * @param nSize size (in bytes) of memory block to allocate.
 * @return pointer to newly allocated memory, only NULL if nSize is zero.
 */

void *XGISMalloc( size_t nSize )

{
    void        *pReturn;

    if( nSize == 0 )
        return NULL;

    if( nSize < 0 )
    {
        XGISError( XE_Failure, XGISE_AppDefined,
                  "XGISMalloc(%d): Silly size requested.\n",
                  nSize );
        return NULL;
    }
    
    pReturn = XSIMalloc( nSize );
    if( pReturn == NULL )
    {
        XGISError( XE_Fatal, XGISE_OutOfMemory,
                  "XGISMalloc(): Out of memory allocating %d bytes.\n",
                  nSize );
    }

    return pReturn;
}

/************************************************************************/
/*                             XGISRealloc()                             */
/************************************************************************/

/**
 * Safe version of realloc().
 *
 * This function is like the C library realloc(), but raises a CE_Fatal
 * error with XGISError() if it fails to allocate the desired memory.  It
 * should be used for small memory allocations that are unlikely to fail
 * and for which the application is unwilling to test for out of memory
 * conditions.  It uses XSIRealloc() to get the memory, so any hooking of
 * XSIRealloc() will apply to XGISRealloc() as well.  XGISFree() or XSIFree()
 * can be used free memory allocated by XGISRealloc().
 *
 * It is also safe to pass NULL in as the existing memory block for
 * XGISRealloc(), in which case it uses XSIMalloc() to allocate a new block.
 *
 * @param pData existing memory block which should be copied to the new block.
 * @param nNewSize new size (in bytes) of memory block to allocate.
 * @return pointer to allocated memory, only NULL if nNewSize is zero.
 */
void * XGISRealloc( void * pData, size_t nNewSize )

{
    void        *pReturn;

    if ( nNewSize == 0 )
    {
        XGISFree(pData);
        return NULL;
    }

    if( nNewSize < 0 )
    {
        return NULL;
    }
    
    if( pData == NULL )
        pReturn = XSIMalloc( nNewSize );
    else
        pReturn = XSIRealloc( pData, nNewSize );
    
    return pReturn;
}


/************************************************************************/
/*                             XSIStrdup()                              */
/************************************************************************/

char *XSIStrdup( const char * pszString )

{
    return( strdup( pszString ) );
}


/************************************************************************/
/*                             XGISStrdup()                              */
/************************************************************************/

/**
 * Safe version of strdup() function.
 *
 * This function is similar to the C library strdup() function, but if
 * the memory allocation fails it will issue a CE_Fatal error with
 * XGISError() instead of returning NULL.  It uses XSIStrdup(), so any
 * hooking of that function will apply to XGISStrdup() as well.  Memory
 * allocated with XGISStrdup() can be freed with XGISFree() or XSIFree().
 *
 * It is also safe to pass a NULL string into XGISStrdup().  XGISStrdup()
 * will allocate and return a zero length string (as opposed to a NULL
 * string).
 *
 * @param pszString input string to be duplicated.  May be NULL.
 * @return pointer to a newly allocated copy of the string.  Free with
 * XGISFree() or XSIFree().
 */

char *XGISStrdup( const char * pszString )

{
    char        *pszReturn;

    if( pszString == NULL )
        pszString = "";

    pszReturn = XSIStrdup( pszString );
            
    return( pszReturn );
}


/**********************************************************************
 *                       XGISCount()
 *
 * Return the number of lines in a Stringlist.
 **********************************************************************/
int XGISCount(char **papszStrList)
{
    int nItems=0;

    if (papszStrList)
    {
        while(*papszStrList != NULL)
        {
            nItems++;
            papszStrList++;
        }
    }

    return nItems;
}


/**********************************************************************
 *                       XGISSPrintf()
 *
 * My own version of XGISSPrintf() that works with 10 static buffer.
 *
 * It returns a ref. to a static buffer that should not be freed and
 * is valid only until the next call to XGISSPrintf().
 *
 * NOTE: This function should move to cpl_conv.cpp. 
 **********************************************************************/
/* For now, assume that a 8000 chars buffer will be enough.
 */
#define XGISSPrintf_BUF_SIZE 8000
#define XGISSPrintf_BUF_Count 10
static char gszXGISSPrintfBuffer[XGISSPrintf_BUF_Count][XGISSPrintf_BUF_SIZE];
static int gnXGISSPrintfBuffer = 0;

const char *XGISSPrintf(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
#if defined(HAVE_VSNPRINTF)
    vsnprintf(gszXGISSPrintfBuffer[gnXGISSPrintfBuffer], XGISSPrintf_BUF_SIZE-1,
              fmt, args);
#else
    vsprintf(gszXGISSPrintfBuffer[gnXGISSPrintfBuffer], fmt, args);
#endif
    va_end(args);
    
   int nCurrent = gnXGISSPrintfBuffer;

    if (++gnXGISSPrintfBuffer == XGISSPrintf_BUF_Count)
      gnXGISSPrintfBuffer = 0;

    return gszXGISSPrintfBuffer[nCurrent];
}

/************************************************************************/
/*                             _XGISAssert()                             */
/*                                                                      */
/*      This function is called only when an assertion fails.           */
/************************************************************************/

/**
 * Report failure of a logical assertion.
 *
 * Applications would normally use the XGISAssert() macro which expands
 * into code calling _XGISAssert() only if the condition fails.  _XGISAssert()
 * will generate a CE_Fatal error call to XGISError(), indicating the file
 * name, and line number of the failed assertion, as well as containing
 * the assertion itself.
 *
 * There is no reason for application code to call _XGISAssert() directly.
 */

void _XGISAssert( const char * pszExpression, 
                 const char * pszFile,
                 int iLine )

{
    XGISError( XE_Fatal, XGISE_AssertionFailed,
              "Assertion `%s' failed\n"
              "in file `%s', line %d\n",
              pszExpression, pszFile, iLine );
}

/**********************************************************************
 *                          XGISError()
 **********************************************************************/

/**
 * Report an error.
 *
 * This function reports an error in a manner that can be hooked
 * and reported appropriate by different applications.
 *
 * The effect of this function can be altered by applications by installing
 * a custom error handling using XGISSetErrorHandler().
 *
 * The eErrClass argument can have the value CE_Warning indicating that the
 * message is an informational warning, CE_Failure indicating that the
 * action failed, but that normal recover mechanisms will be used or
 * CE_Fatal meaning that a fatal error has occured, and that XGISError()
 * should not return.  
 *
 * The default behaviour of XGISError() is to report errors to stderr,
 * and to abort() after reporting a CE_Fatal error.  It is expected that
 * some applications will want to supress error reporting, and will want to
 * install a C++ exception, or longjmp() approach to no local fatal error
 * recovery.
 *
 * Regardless of how application error handlers or the default error
 * handler choose to handle an error, the error number, and message will
 * be stored for recovery with XGISGetLastErrorNo() and XGISGetLastErrorMsg().
 *
 * @param eErrClass one of CE_Warning, CE_Failure or CE_Fatal.
 * @param err_no the error number (XGISE_*) from cpl_error.h.
 * @param fmt a printf() style format string.  Any additional arguments
 * will be treated as arguments to fill in this format in a manner
 * similar to printf().
 */

void    XGISError(XGISErr eErrClass, int err_no, const char *fmt, ...)
{
    fprintf(stderr, "An error was occured");

    /*    va_list args;

    va_start(args, fmt);
    XGISErrorV( eErrClass, err_no, fmt, args );
    va_end(args);
    */
}



/************************************************************************/
/*                         XGISTokenizeString2()                         */
/*                                                                      */
/*      The ultimate tokenizer?                                         */
/************************************************************************/

char ** XGISTokenizeString2( const char * pszString,
                            const char * pszDelimiters,
                            int nXSITFlags )

{
    char        **papszRetList = NULL;
    int         nRetMax = 0, nRetLen = 0;
    char        *pszToken;
    int         nTokenMax, nTokenLen;
    int         bHonourStrings    = (nXSITFlags & XSLT_HONOURSTRINGS);
    int         bAllowEmptyTokens = (nXSITFlags & XSLT_ALLOWEMPTYTOKENS);
    int         bUseSingleQuotes  = (nXSITFlags & XSLT_USESINGLEQUOTES);

    pszToken = (char *) XGISCalloc(10,1);
    nTokenMax = 10;
    
    while( pszString != NULL && *pszString != '\0' )
    {
        int     bInString = false;

        nTokenLen = 0;
        
        /* Try to find the next delimeter, marking end of token */
        for( ; *pszString != '\0'; pszString++ )
        {

            /* End if this is a delimeter skip it and break. */
            if( !bInString && strchr(pszDelimiters, *pszString) != NULL )
            {
                pszString++;
                break;
            }
            
            /* If this is a quote, and we are honouring constant
               strings, then process the constant strings, with out delim
               but don't copy over the quotes */
            if( (bHonourStrings && *pszString == '"') || 
                (bHonourStrings && bUseSingleQuotes && *pszString == '\'') 
              )
            {
                if( nXSITFlags & XSLT_PRESERVEQUOTES )
                {
                    pszToken[nTokenLen] = *pszString;
                    nTokenLen++;
                }

                if( bInString )
                {
                    bInString = false;
                    continue;
                }
                else
                {
                    bInString = true;
                    continue;
                }
            }

            /* Within string constants we allow for escaped quotes, but
               in processing them we will unescape the quotes */
            if( bInString && pszString[0] == '\\' && pszString[1] == '"' )
            {
                if( nXSITFlags & XSLT_PRESERVEESCAPES )
                {
                    pszToken[nTokenLen] = *pszString;
                    nTokenLen++;
                }

                pszString++;
            }

            /* Within string constants a \\ sequence reduces to \ */
            else if( bInString 
                     && pszString[0] == '\\' && pszString[1] == '\\' )
            {
                if( nXSITFlags & XSLT_PRESERVEESCAPES )
                {
                    pszToken[nTokenLen] = *pszString;
                    nTokenLen++;
                }
                pszString++;
            }

            if( nTokenLen >= nTokenMax-3 )
            {
                nTokenMax = nTokenMax * 2 + 10;
                pszToken = (char *) XGISRealloc( pszToken, nTokenMax );
            }

            pszToken[nTokenLen] = *pszString;
            nTokenLen++;
        }

        pszToken[nTokenLen] = '\0';

        /*
         * Add the token.
         */
        if( pszToken[0] != '\0' || bAllowEmptyTokens )
        {
            if( nRetLen >= nRetMax - 1 )
            {
                nRetMax = nRetMax * 2 + 10;
                papszRetList = (char **) 
                    XGISRealloc(papszRetList, sizeof(char*) * nRetMax );
            }

            papszRetList[nRetLen++] = XGISStrdup( pszToken );
            papszRetList[nRetLen] = NULL;
        }
    }

    /*
     * If the last token was empty, then we need to capture
     * it now, as the loop would skip it.
     */
    if( *pszString == '\0' && bAllowEmptyTokens && nRetLen > 0 
        && strchr(pszDelimiters,*(pszString-1)) != NULL )
    {
        if( nRetLen >= nRetMax - 1 )
        {
            nRetMax = nRetMax * 2 + 10;
            papszRetList = (char **) 
                XGISRealloc(papszRetList, sizeof(char*) * nRetMax );
        }

        papszRetList[nRetLen++] = XGISStrdup("");
        papszRetList[nRetLen] = NULL;
    }

    if( papszRetList == NULL )
        papszRetList = (char **) XGISCalloc(sizeof(char *),1);

    XGISFree( pszToken );

    return papszRetList;
}

/************************************************************************/
/*                          XGISEscapeString()                           */
/************************************************************************/

/**
 * Apply escaping to string to preserve special characters.
 *
 * This function will "escape" a variety of special characters
 * to make the string suitable to embed within a string constant
 * or to write within a text stream but in a form that can be
 * reconstitued to it's original form.  The escaping will even preserve
 * zero bytes allowing preservation of raw binary data.
 *
 * XGISES_BackslashQuotable(0): This scheme turns a binary string into 
 * a form suitable to be placed within double quotes as a string constant.
 * The backslash, quote, '\0' and newline characters are all escaped in 
 * the usual C style. 
 *
 * XGISES_XML(1): This scheme converts the '<', '<' and '&' characters into
 * their XML/HTML equivelent (&gt;, &lt; and &amp;) making a string safe
 * to embed as CDATA within an XML element.  The '\0' is not escaped and 
 * should not be included in the input.
 *
 * XGISES_URL(2): Everything except alphanumerics and the underscore are 
 * converted to a percent followed by a two digit hex encoding of the character
 * (leading zero supplied if needed).  This is the mechanism used for encoding
 * values to be passed in URLs.
 *
 * XGISES_SQL(3): All single quotes are replaced with two single quotes.  
 * Suitable for use when constructing literal values for SQL commands where
 * the literal will be enclosed in single quotes.
 *
 * XGISES_CSV(4): If the values contains commas, double quotes, or newlines it 
 * placed in double quotes, and double quotes in the value are doubled.
 * Suitable for use when constructing field values for .csv files.  Note that
 * XGISUnescapeString() currently does not support this format, only 
 * XGISEscapeString().  See cpl_csv.cpp for csv parsing support.
 *
 * @param pszInput the string to escape.  
 * @param nLength The number of bytes of data to preserve.  If this is -1
 * the strlen(pszString) function will be used to compute the length.
 * @param nScheme the encoding scheme to use.  
 *
 * @return an escaped, zero terminated string that should be freed with 
 * XGISFree() when no longer needed.
 */

char *XGISEscapeString( const char *pszInput, int nLength, 
                       int nScheme )

{
    char        *pszOutput;
    char        *pszShortOutput;

    if( nLength == -1 )
        nLength = strlen(pszInput);

    pszOutput = (char *) XGISMalloc( nLength * 6 + 1 );
    
    if( nScheme == XGISES_BackslashQuotable )
    {
        int iOut = 0, iIn;

        for( iIn = 0; iIn < nLength; iIn++ )
        {
            if( pszInput[iIn] == '\0' )
            {
                pszOutput[iOut++] = '\\';
                pszOutput[iOut++] = '0';
            }
            else if( pszInput[iIn] == '\n' )
            {
                pszOutput[iOut++] = '\\';
                pszOutput[iOut++] = 'n';
            }
            else if( pszInput[iIn] == '"' )
            {
                pszOutput[iOut++] = '\\';
                pszOutput[iOut++] = '\"';
            }
            else if( pszInput[iIn] == '\\' )
            {
                pszOutput[iOut++] = '\\';
                pszOutput[iOut++] = '\\';
            }
            else
                pszOutput[iOut++] = pszInput[iIn];
        }
        pszOutput[iOut] = '\0';
    }
    else if( nScheme == XGISES_URL ) /* Untested at implementation */
    {
        int iOut = 0, iIn;

        for( iIn = 0; iIn < nLength; iIn++ )
        {
            if( (pszInput[iIn] >= 'a' && pszInput[iIn] <= 'z')
                || (pszInput[iIn] >= 'A' && pszInput[iIn] <= 'Z')
                || (pszInput[iIn] >= '0' && pszInput[iIn] <= '9')
                || pszInput[iIn] == '_' )
            {
                pszOutput[iOut++] = pszInput[iIn];
            }
            else
            {
                sprintf( pszOutput, "%%%02X", pszInput[iIn] );
                iOut += 3;
            }
        }
        pszOutput[iOut] = '\0';
    }
    else if( nScheme == XGISES_XML )
    {
        int iOut = 0, iIn;

        for( iIn = 0; iIn < nLength; iIn++ )
        {
            if( pszInput[iIn] == '<' )
            {
                pszOutput[iOut++] = '&';
                pszOutput[iOut++] = 'l';
                pszOutput[iOut++] = 't';
                pszOutput[iOut++] = ';';
            }
            else if( pszInput[iIn] == '>' )
            {
                pszOutput[iOut++] = '&';
                pszOutput[iOut++] = 'g';
                pszOutput[iOut++] = 't';
                pszOutput[iOut++] = ';';
            }
            else if( pszInput[iIn] == '&' )
            {
                pszOutput[iOut++] = '&';
                pszOutput[iOut++] = 'a';
                pszOutput[iOut++] = 'm';
                pszOutput[iOut++] = 'p';
                pszOutput[iOut++] = ';';
            }
            else if( pszInput[iIn] == '"' )
            {
                pszOutput[iOut++] = '&';
                pszOutput[iOut++] = 'q';
                pszOutput[iOut++] = 'u';
                pszOutput[iOut++] = 'o';
                pszOutput[iOut++] = 't';
                pszOutput[iOut++] = ';';
            }
            else
                pszOutput[iOut++] = pszInput[iIn];
        }
        pszOutput[iOut] = '\0';
    }
    else if( nScheme == XGISES_SQL )
    {
        int iOut = 0, iIn;

        for( iIn = 0; iIn < nLength; iIn++ )
        {
            if( pszInput[iIn] == '\'' )
            {
                pszOutput[iOut++] = '\'';
                pszOutput[iOut++] = '\'';
            }
            else
                pszOutput[iOut++] = pszInput[iIn];
        }
        pszOutput[iOut] = '\0';
    }
    else if( nScheme == XGISES_CSV )
    {
        if( strchr( pszInput, '\"' ) == NULL
            && strchr( pszInput, ',') == NULL
            && strchr( pszInput, 10) == NULL 
            && strchr( pszInput, 13) == NULL )
        {
            strcpy( pszOutput, pszInput );
        }
        else
        {
            int iOut = 1, iIn;

            pszOutput[0] = '\"';

            for( iIn = 0; iIn < nLength; iIn++ )
            {
                if( pszInput[iIn] == '\"' )
                {
                    pszOutput[iOut++] = '\"';
                    pszOutput[iOut++] = '\"';
                }
                else if( pszInput[iIn] == 13 )
                    /* drop DOS LF's in strings. */;
                else
                    pszOutput[iOut++] = pszInput[iIn];
            }
            pszOutput[iOut++] = '\"';
            pszOutput[iOut++] = '\0';
        }
    }
    else
    {
        pszOutput[0] = '\0';
        XGISError( XE_Failure, XGISE_AppDefined, 
                  "Undefined escaping scheme (%d) in XGISEscapeString()",
                  nScheme );
    }

    pszShortOutput = XGISStrdup( pszOutput );
    XGISFree( pszOutput );

    return pszShortOutput;
}

/************************************************************************/
/*                         XGISUnescapeString()                          */
/************************************************************************/

/**
 * Unescape a string.
 *
 * This function does the opposite of XGISEscapeString().  Given a string
 * with special values escaped according to some scheme, it will return a
 * new copy of the string returned to it's original form. 
 *
 * @param pszInput the input string.  This is a zero terminated string.
 * @param pnLength location to return the length of the unescaped string, 
 * which may in some cases include embedded '\0' characters.
 * @param nScheme the escaped scheme to undo (see XGISEscapeString() for a
 * list). 
 * 
 * @return a copy of the unescaped string that should be freed by the 
 * application using XGISFree() when no longer needed.
 */

char *XGISUnescapeString( const char *pszInput, int *pnLength, int nScheme )

{
    char *pszOutput;
    int iOut=0, iIn;

    pszOutput = (char *) XGISMalloc(strlen(pszInput)+1);
    pszOutput[0] = '\0';

    if( nScheme == XGISES_XML )
    {
        for( iIn = 0; pszInput[iIn] != '\0'; iIn++ )
        {
            if( EQUAL_SN(pszInput+iIn,"&lt;",4) )
            {
                pszOutput[iOut++] = '<';
                iIn += 3;
            }
            else if( EQUAL_SN(pszInput+iIn,"&gt;",4) )
            {
                pszOutput[iOut++] = '>';
                iIn += 3;
            }
            else if( EQUAL_SN(pszInput+iIn,"&amp;",5) )
            {
                pszOutput[iOut++] = '&';
                iIn += 4;
            }
            else if( EQUAL_SN(pszInput+iIn,"&quot;",6) )
            {
                pszOutput[iOut++] = '"';
                iIn += 5;
            }
            else
            {
                pszOutput[iOut++] = pszInput[iIn];
            }
        }
    }
    else if( nScheme == XGISES_URL )
    {
        for( iIn = 0; pszInput[iIn] != '\0'; iIn++ )
        {
            if( pszInput[iIn] == '%' 
                && pszInput[iIn+1] != '\0' 
                && pszInput[iIn+2] != '\0' )
            {
                int nHexChar = 0;

                if( pszInput[iIn+1] >= 'A' && pszInput[iIn+1] <= 'F' )
                    nHexChar += 16 * (pszInput[iIn+1] - 'A' + 10);
                else if( pszInput[iIn+1] >= 'a' && pszInput[iIn+1] <= 'f' )
                    nHexChar += 16 * (pszInput[iIn+1] - 'a' + 10);
                else if( pszInput[iIn+1] >= '0' && pszInput[iIn+1] <= '9' )
                    nHexChar += 16 * (pszInput[iIn+1] - '0');
                else
                    ;
                    //XGISDebug( "XGIS", 
                    //          "Error unescaping XGISES_URL text, percent not "
                    //          "followed by two hex digits." );
                    
                if( pszInput[iIn+2] >= 'A' && pszInput[iIn+2] <= 'F' )
                    nHexChar += pszInput[iIn+2] - 'A' + 10;
                else if( pszInput[iIn+2] >= 'a' && pszInput[iIn+2] <= 'f' )
                    nHexChar += pszInput[iIn+2] - 'a' + 10;
                else if( pszInput[iIn+2] >= '0' && pszInput[iIn+2] <= '9' )
                    nHexChar += pszInput[iIn+2] - '0';
                else
                    ;
                    //XGISDebug( "XGIS", 
                    //          "Error unescaping XGISES_URL text, percent not "
                    //          "followed by two hex digits." );

                pszOutput[iOut++] = (char) nHexChar;
                iIn += 2;
            }
            else if( pszInput[iIn] == '+' )
            {
                pszOutput[iOut++] = ' ';
            }   
            else
            {
                pszOutput[iOut++] = pszInput[iIn];
            }
        }
    }
    else if( nScheme == XGISES_SQL )
    {
        for( iIn = 0; pszInput[iIn] != '\0'; iIn++ )
        {
            if( pszInput[iIn] == '\'' && pszInput[iIn+1] == '\'' )
            {
                iIn++;
                pszOutput[iOut++] = pszInput[iIn];
            }
            else
            {
                pszOutput[iOut++] = pszInput[iIn];
            }
        }
    }
    else /* if( nScheme == XGISES_BackslashQuoteable ) */
    {
        for( iIn = 0; pszInput[iIn] != '\0'; iIn++ )
        {
            if( pszInput[iIn] == '\\' )
            {
                iIn++;
                if( pszInput[iIn] == 'n' )
                    pszOutput[iOut++] = '\n';
                else if( pszInput[iIn] == '0' )
                    pszOutput[iOut++] = '\0';
                else 
                    pszOutput[iOut++] = pszInput[iIn];
            }
            else
            {
                pszOutput[iOut++] = pszInput[iIn];
            }
        }
    }

    pszOutput[iOut] = '\0';

    if( pnLength != NULL )
        *pnLength = iOut;

    return pszOutput;
}

/************************************************************************/
/*                      XGISTokenizeStringComplex()                      */
/*                                                                      */
/*      Obsolete tokenizing api.                                        */
/************************************************************************/

char ** XGISTokenizeStringComplex( const char * pszString,
                                  const char * pszDelimiters,
                                  int bHonourStrings, 
                                  int bAllowEmptyTokens )

{
    int         nFlags = 0;

    if( bHonourStrings )
        nFlags |= XSLT_HONOURSTRINGS;
    if( bAllowEmptyTokens )
        nFlags |= XSLT_ALLOWEMPTYTOKENS;

    return XGISTokenizeString2( pszString, pszDelimiters, nFlags );
}

static void XGISReplacePointByLocalePoint(char* pszNumber, char point)
{
#if defined(WIN32CE)
    static char byPoint = 0;
    if (byPoint == 0)
    {
        char szBuf[16];
        sprintf(szBuf, "%.1f", 1.0);
        byPoint = szBuf[1];
    }
    if (point != byPoint)
    {
        int     i = 0;

        while ( pszNumber[i] )
        {
            if ( pszNumber[i] == point )
            {
                pszNumber[i] = byPoint;
                break;
            }
            i++;
        }
    }
#else
    struct lconv *poLconv = localeconv();
    if ( poLconv
         && poLconv->decimal_point
         && strlen(poLconv->decimal_point) > 0 )
    {
        int     i = 0;
        char    byPoint = poLconv->decimal_point[0];

        if (point != byPoint)
        {
            while ( pszNumber[i] )
            {
                if ( pszNumber[i] == point )
                {
                    pszNumber[i] = byPoint;
                    break;
                }
                i++;
            }
        }
    }
#endif
}

double XGISAtof(const char *nptr)
{
  return XGISStrtod(nptr, 0);
}

double XGISStrtod(const char *nptr, char **endptr)
{
    return XGISStrtodDelim(nptr, endptr, '.');
}

double XGISStrtodDelim(const char *nptr, char **endptr, char point)
{
/* -------------------------------------------------------------------- */
/*  We are implementing a simple method here: copy the input string     */
/*  into the temporary buffer, replace the specified decimal delimiter  */
/*  with the one, taken from locale settings and use standard strtod()  */
/*  on that buffer.                                                     */
/* -------------------------------------------------------------------- */
    char        *pszNumber = XGISStrdup( nptr );
    double      dfValue;
    int         nError;

    XGISReplacePointByLocalePoint(pszNumber, point);

    dfValue = strtod( pszNumber, endptr );
    nError = errno;

    if ( endptr )
        *endptr = (char *)nptr + (*endptr - pszNumber);

    XGISFree( pszNumber );

    errno = nError;
    return dfValue;
}

/**********************************************************************
 *                       XGISAddString()
 *
 * Append a string to a StringList and return a pointer to the modified
 * StringList.
 * If the input StringList is NULL, then a new StringList is created.
 * Note that XSIAddString performance when building a list is in O(n^2)
 * which can cause noticable slow down when n > 10000.
 **********************************************************************/
char **XGISAddString(char **papszStrList, const char *pszNewString)
{
    int nItems=0;

    if (pszNewString == NULL)
        return papszStrList;    /* Nothing to do!*/

    /* Allocate room for the new string */
    if (papszStrList == NULL)
        papszStrList = (char**) XGISCalloc(2,sizeof(char*));
    else
    {
        nItems = XGISCount(papszStrList);
        papszStrList = (char**)XGISRealloc(papszStrList, 
                                          (nItems+2)*sizeof(char*));
    }

    /* Copy the string in the list */
    papszStrList[nItems] = XGISStrdup(pszNewString);
    papszStrList[nItems+1] = NULL;

    return papszStrList;
}
