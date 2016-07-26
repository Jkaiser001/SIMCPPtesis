#include "../pthreads/pthreads.h"
#include "locks.h"


// handle<PThreads>*
// map< string, list< handle<PThreads>* > > cola;

void Locks::set_lock( string nombre, handle<PThreads>* pth, int pid )
{
  contador_locks[pid]++;

  (*pth)->phold(2.0);
  cola[nombre].push(pth);

  if ( cola[nombre].size() > 1 )  
     (*pth)->duerme();
  
}

void Locks::set_unlock( string nombre )
{
  ASSERT(cola.count(nombre)!=0); // se hace unlock a un lock anterior.

  
  cola[nombre].pop();
  
  if ( cola[nombre].size() > 0 )
  {
     handle<PThreads>* pth= cola[nombre].front();
     (*pth)->despierta();
  }
  else
     cola.erase(nombre);
}

