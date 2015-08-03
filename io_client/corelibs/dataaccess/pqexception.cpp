
#include "pqexception.h"

//------------------------------------------------------------------------------
//
// PQexception::PQexception
//
PQexception::PQexception( const string &message ) throw() :
  userMessage( message )
{

}

//------------------------------------------------------------------------------
//
// PQexception::what
//
const char* PQexception::what() const throw ()
{
  return userMessage.c_str();
}

