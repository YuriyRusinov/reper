#if !defined(__Utils_DeletePtr_h)
#define __Utils_DeletePtr_h

#include "configUtils.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

namespace Utils {
/*
struct DLL_EXPORT DeletePtr
{
    template <typename T> 
            void operator()(const T* ptr) const
    {
        delete ptr; 
    }
};
*/

 template<class T> struct EXPORT_UTILS deletePtr 
{
    void operator() (T ptr) 
    { 
        if ( ptr )
            delete ptr;
    }
};

}

#endif
