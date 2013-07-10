#if !defined(__dyndoc_synch_h)
#define __dyndoc_synch_h

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
#ifdef DYNDOC_SYNCH_EXPORT

	#define __DYNDOC_SYNCH_DLL __declspec(dllexport)
#else
	#define __DYNDOC_SYNCH_DLL __declspec(dllimport)
#endif
#else
	#define __DYNDOC_SYNCH_DLL
#endif

#endif