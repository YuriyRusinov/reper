
#ifndef PQNOTIFY_H
#define PQNOTIFY_H

#include <list>
#include <string>

#include "config_dataaccess.h"

using namespace std;
/* использовать нотификацию*/
//объ€влено в config_dataaccess.h
//#define USE_NOTIFICATION 1

//------------------------------------------------------------------------------
//
// Interface IListener
//
class __DA_EXPORT IKKSListener
{
public:
    virtual void notify( char* notify_name, char * payload ) = 0;
    virtual ~IKKSListener(){}
};

//------------------------------------------------------------------------------
//
// Class Notify
//
class __DA_EXPORT KKSNotify
{
public:
  KKSNotify();
  ~KKSNotify();
  string name;
  list<IKKSListener*> listeners;
};

#endif
