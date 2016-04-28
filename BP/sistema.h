
#ifndef SISTEMA_H
#define SISTEMA_H

#include "glob.h"
#include "pthreads.h"
#include "dispatcher.h"
#include "chip.h"

class Sistema: public process 
{
private:
  int NT;
  handle<PThreads> *pthreads;
  handle<Dispatcher> *despachador;
  
public:
  Sistema( 
           const string& _name,
           handle<PThreads> *_pthreads,handle<Dispatcher> *_despachador, int _nt
  
         ) : process( _name )
  {
  
    
    NT= _nt; 
    pthreads= _pthreads;
    despachador=_despachador;
    
    
           
  }
  
protected:

  void inner_body( void );
};  



#endif
