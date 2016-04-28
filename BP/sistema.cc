
#include "sistema.h"

void Sistema::inner_body( void )
{
    // crea objetos co-rutina de la simulaci�n.
       
   for(int i=0; i<NT; i++)
      pthreads[i]->activate();
    despachador[0]->activate();
    
  //-----------------------------------------------------------
  //-----------------------------------------------------------     
   hold( 1000e100 );  // infinito   
  //-----------------------------------------------------------
  //-----------------------------------------------------------

   // cancela todos los objetos co-rutina de la simulacion.

   for(int i=0; i<NT; i++)
      pthreads[i]->cancel();
   despachador[0]->cancel();           
    end_simulation( );   
}
