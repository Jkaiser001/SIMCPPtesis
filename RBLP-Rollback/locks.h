#ifndef LOCK_H
#define LOCK_H

#include "glob.h"

extern int *contador_locks;

class PThreads;

class Locks
{
private:

  double suma,num,suma_duerme;
  map< string, queue< handle<PThreads>* > > cola;

public:

  Locks()
  {
    suma=0;  
    num=0;
    suma_duerme=0;
  }
  
  void set_lock( string, handle<PThreads>*, int );
  void set_unlock(string);
  
  double promedio() { return suma/num; }
  double promedio_duerme() { return suma_duerme/num; }  
};

#endif
