
#ifndef SISTEMA_H
#define SISTEMA_H

#include "glob.h"
#include "pthreads.h"
#include "chip.h"

class Sistema: public process 
{
private:
  int NT;
  handle<PThreads> *pthreads;
  
public:
  Sistema( 
           const string& _name,
           handle<PThreads> *_pthreads, int _nt
  
         ) : process( _name )
  {
  
    
    NT= _nt; 
    pthreads= _pthreads;
    
    
           
  }
  
protected:

  void inner_body( void );
};  



#endif
