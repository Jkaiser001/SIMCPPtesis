
#include "sistema.h"

void Sistema::inner_body( void )
{
    // crea objetos co-rutina de la simulación.
   despachador[0]->activate();    
   for(int i=0; i<NT; i++)
      pthreads[i]->activate();
    
    
  //-----------------------------------------------------------
  //-----------------------------------------------------------     
   hold( 10000000000e10000);  // infinito   
   cout<<"SE ACABO"<<endl; 
  //-----------------------------------------------------------
  //-----------------------------------------------------------

   // cancela todos los objetos co-rutina de la simulacion.

   for(int i=0; i<NT; i++)
      pthreads[i]->cancel();
    despachador[0]->cancel(); 

              
    end_simulation( );   
}
