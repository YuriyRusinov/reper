
#ifndef PQEXCEPTION_H
#define PQEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

//------------------------------------------------------------------------------
//
// Class PQexception
//
class PQexception : public exception
{
public:
  PQexception( const string &message ) throw() ;
  ~PQexception() throw() {};
  const char* what() const throw();

private:
  string userMessage;
};

#endif
