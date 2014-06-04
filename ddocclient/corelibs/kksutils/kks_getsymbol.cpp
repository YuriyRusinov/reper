
#include "kks_getsymbol.h"
#include "strutils.h"

/* ==================================================================== */
/*                  Unix Implementation                                 */
/* ==================================================================== */
#if defined(HAVE_DLFCN_H)

#define GOT_GETSYMBOL

#include <dlfcn.h>

/************************************************************************/
/*                            XGISGetSymbol()                            */
/************************************************************************/

/**
 * Fetch a function pointer from a shared library / DLL.
 *
 * This function is meant to abstract access to shared libraries and
 * DLLs and performs functions similar to dlopen()/dlsym() on Unix and
 * LoadLibrary() / GetProcAddress() on Windows.
 *
 * If no support for loading entry points from a shared library is available
 * this function will always return NULL.   Rules on when this function
 * issues a XGISError() or not are not currently well defined, and will have
 * to be resolved in the future.
 *
 * Currently XGISGetSymbol() doesn't try to:
 * <ul>
 *  <li> prevent the reference count on the library from going up
 *    for every request, or given any opportunity to unload      
 *    the library.                                            
 *  <li> Attempt to look for the library in non-standard         
 *    locations.                                              
 *  <li> Attempt to try variations on the symbol name, like      
 *    pre-prending or post-pending an underscore.
 * </ul>
 * 
 * Some of these issues may be worked on in the future.
 *
 * @param pszLibrary the name of the shared library or DLL containing
 * the function.  May contain path to file.  If not system supplies search
 * paths will be used.
 * @param pszSymbolName the name of the function to fetch a pointer to.
 * @return A pointer to the function if found, or NULL if the function isn't
 * found, or the shared library can't be loaded.
 */

void *XGISGetSymbol( const char * pszLibrary, const char * pszSymbolName )

{
    void        *pLibrary;
    void        *pSymbol;

    pLibrary = dlopen(pszLibrary, RTLD_LAZY);
    if( pLibrary == NULL )
    {
        XGISError( XE_Failure, XGISE_AppDefined,
                  "%s", dlerror() );
        return NULL;
    }

    pSymbol = dlsym( pLibrary, pszSymbolName );

#if (defined(__APPLE__) && defined(__MACH__))
    /* On mach-o systems, C symbols have a leading underscore and depending
     * on how dlcompat is configured it may or may not add the leading
     * underscore.  So if dlsym() fails add an underscore and try again.
     */
    if( pSymbol == NULL )
    {
        char withUnder[strlen(pszSymbolName) + 2];
        withUnder[0] = '_'; withUnder[1] = 0;
        strcat(withUnder, pszSymbolName);
        pSymbol = dlsym( pLibrary, withUnder );
    }
#endif

    if( pSymbol == NULL )
    {
        XGISError( XE_Failure, XGISE_AppDefined,
                  "%s", dlerror() );
        return NULL;
    }
    
    return( pSymbol );
}

#endif /* def __unix__ && defined(HAVE_DLFCN_H) */

/* ==================================================================== */
/*                 Windows Implementation                               */
/* ==================================================================== */
#if defined(WIN32) && !defined(WIN32CE)

#define GOT_GETSYMBOL

#include <windows.h>

/************************************************************************/
/*                            XGISGetSymbol()                            */
/************************************************************************/

void *XGISGetSymbol( const char * pszLibrary, const char * pszSymbolName )

{
    void        *pLibrary;
    void        *pSymbol;

    pLibrary = LoadLibrary(pszLibrary);
    if( pLibrary == NULL )
    {
        LPVOID      lpMsgBuf = NULL;
        int         nLastError = GetLastError();
        
        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER 
                       | FORMAT_MESSAGE_FROM_SYSTEM
                       | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, nLastError,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                       (LPTSTR) &lpMsgBuf, 0, NULL );
 
        XGISError( XE_Failure, XGISE_AppDefined,
                  "Can't load requested DLL: %s\n%d: %s", 
                  pszLibrary, nLastError, (const char *) lpMsgBuf );
        return NULL;
    }

    pSymbol = (void *) GetProcAddress( (HINSTANCE) pLibrary, pszSymbolName );

    if( pSymbol == NULL )
    {
        XGISError( XE_Failure, XGISE_AppDefined,
                  "Can't find requested entry point: %s\n", pszSymbolName );
        return NULL;
    }
    
    return( pSymbol );
}

#endif /* def _WIN32 */

/* ==================================================================== */
/*                 Windows CE Implementation                               */
/* ==================================================================== */
#if defined(WIN32CE)

#define GOT_GETSYMBOL

#include "cpl_win32ce_api.h"

/************************************************************************/
/*                            XGISGetSymbol()                            */
/************************************************************************/

void *XGISGetSymbol( const char * pszLibrary, const char * pszSymbolName )

{
    void        *pLibrary;
    void        *pSymbol;

    pLibrary = CE_LoadLibraryA(pszLibrary);
    if( pLibrary == NULL )
    {
        XGISError( XE_Failure, XGISE_AppDefined,
                  "Can't load requested DLL: %s", pszLibrary );
        return NULL;
    }

    pSymbol = (void *) CE_GetProcAddressA( (HINSTANCE) pLibrary, pszSymbolName );

    if( pSymbol == NULL )
    {
        XGISError( XE_Failure, XGISE_AppDefined,
                  "Can't find requested entry point: %s\n", pszSymbolName );
        return NULL;
    }
    
    return( pSymbol );
}

#endif /* def WIN32CE */

/* ==================================================================== */
/*      Dummy implementation.                                           */
/* ==================================================================== */

#ifndef GOT_GETSYMBOL

/************************************************************************/
/*                            XGISGetSymbol()                            */
/*                                                                      */
/*      Dummy implementation.                                           */
/************************************************************************/

void *XGISGetSymbol(const char *pszLibrary, const char *pszEntryPoint)

{
    //XGISDebug( "XGIS", 
    //          "XGISGetSymbol(%s,%s) called.  Failed as this is stub"
    //          " implementation.", pszLibrary, pszEntryPoint );
    return NULL;
}
#endif
