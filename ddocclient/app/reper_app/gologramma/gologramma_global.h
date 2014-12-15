#ifndef GOLOGRAMMA_GLOBAL_H
#define GOLOGRAMMA_GLOBAL_H

#ifdef WIN32

#include <windows.h>
#pragma warning (disable:4251)
#pragma warning (disable:4530)
#ifdef GOLOGRAMMA_LIBRARY
    #define _GOL_EXPORT __declspec(dllexport)
#else
    #define _GOL_EXPORT __declspec(dllimport)
#endif
#else
    #define _GOL_EXPORT
#endif

#endif // GOLOGRAMMA_GLOBAL_H
