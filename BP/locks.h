#ifndef LOCK_H
#define LOCK_H

#include "glob.h"

extern int *contador_locks;

class PThreads;

class Locks
{
private:

  map< string, queue< handle<PThreads>* > > cola;

public:

  Locks() {;}
  
  void set_lock( string, handle<PThreads>*,int );
  void set_unlock(string);
};

#endif
