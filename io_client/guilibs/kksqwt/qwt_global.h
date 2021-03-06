/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

// vim: expandtab

#ifndef QWT_GLOBAL_H
#define QWT_GLOBAL_H

#include <qglobal.h>

#if defined(_WIN32) && !defined(WIN32)
    #define WIN32
#endif

#if defined(_WINDOWS) && !defined(WIN32)
    #define WIN32
#endif



#ifdef WIN32

#if defined(_MSC_VER) /* MSVC Compiler */
/* template-class specialization 'identifier' is already instantiated */
#pragma warning(disable: 4660)
#endif // _MSC_VER

#ifdef QWT_DLL
//#if defined(__USE_DLL_KKSQWT)
#if defined(QWT_MAKEDLL)     // create a Qwt DLL library 
#define QWT_EXPORT  __declspec(dllexport)
#define QWT_TEMPLATEDLL
#else                        // use a Qwt DLL library
#define QWT_EXPORT  __declspec(dllimport)
#endif

#endif // QWT_DLL

#endif // Q_WS_WIN || Q_WS_S60

#ifndef QWT_EXPORT
#define QWT_EXPORT
#endif

#endif // QWT_GLOBAL_H
