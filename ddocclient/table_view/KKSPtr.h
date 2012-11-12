/***********************************************************************
 * Module:  KKSPtr.h
 ***********************************************************************/
 
#if !defined(__KKSPtr_h)
#define __KKSPtr_h

#include "kkspp_config.h"

template <class Type>
class _PP_EXPORT KKSPtr {
public:
    Type * operator->()
    {
	    return m_pP;
    }
    
    KKSPtr<Type> & operator=( const KKSPtr<Type> &cp )
    {
	    if ( this == &cp )
	        return *this;
	    
	    if ( m_pP )     
	        m_pP->release();    
	    
	    m_pP = cp.m_pP;    
	    m_pP->addRef();
	    
        return *this;
    }
    
public:
    KKSPtr( const KKSPtr<Type> &cp) : m_pP( cp.m_pP )
    {
	    if ( m_pP )
	        m_pP->addRef();
    }
    
    KKSPtr(Type *pP): m_pP( pP )
    {
        if ( pP )
	        pP->addRef();
    }
 
    ~KKSPtr()
    {
	    if ( m_pP )
	        m_pP->release();
    }
    
    Type *getPtr()
    {
	    return m_pP;
    }
    
private:
    Type *m_pP;
};

#endif
