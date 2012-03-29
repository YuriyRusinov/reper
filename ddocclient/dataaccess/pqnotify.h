
#ifndef PQNOTIFY_H
#define PQNOTIFY_H

#include <list>
#include <string>

using namespace std;
/* использовать нотификацию*/
//#define USE_NOTIFY

//------------------------------------------------------------------------------
//
// Interface IListener
//
class IListener
{
public:
    virtual void notify( char* notify_name ) = 0;
    virtual ~IListener();
};

//------------------------------------------------------------------------------
//
// Class Notify
//
class PQnotify
{
public:
  PQnotify();
  ~PQnotify();
  string name;
  list<IListener*> listeners;
};

#endif
