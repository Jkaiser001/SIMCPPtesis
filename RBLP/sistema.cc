
#include "sistema.h"

void Sistema::inner_body( void )
{
    // crea objetos co-rutina de la simulación.
       
   for(int i=0; i<NT; i++)
      pthreads[i]->activate();

    
  //-----------------------------------------------------------
  //-----------------------------------------------------------     
   hold( 10000e100 );  // infinito   
  //-----------------------------------------------------------
  //-----------------------------------------------------------

   // cancela todos los objetos co-rutina de la simulacion.

   for(int i=0; i<NT; i++)
      pthreads[i]->cancel();
              
    end_simulation( );   
}
