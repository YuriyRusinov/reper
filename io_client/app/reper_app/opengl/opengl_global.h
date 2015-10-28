#ifndef OPENGL_GLOBAL_H
#define OPENGL_GLOBAL_H

#ifdef WIN32

#include <windows.h>
#pragma warning (disable:4251)
#pragma warning (disable:4530)
#ifdef OPENGL_LIBRARY
    #define _GL_EXPORT __declspec(dllexport)
#else
    #define _GL_EXPORT __declspec(dllimport)
#endif
#else
    #define _GL_EXPORT
#endif

#endif // OPENGL_GLOBAL_H
