
#include "metodos.h"


void Metodos::inic_Bloques( Bloque **inicio, Bloque **fin, int nb )
{
  if (nb==0)
  {
    Bloque *bloque = new Bloque();
    bloque->initBloque(dimBloque);
    bloque->ant = NULL;
    bloque->sig = NULL;
    bloque->jBloque = 0;
    bloque->estadoBloque = BLOQUE_NO_LLENO;
    *inicio = bloque;  *fin    = bloque;
    return;
  }

  Bloque *bloque = new Bloque[nb];
  
  for(int i=0;i<nb; i++)
    bloque[i].initBloque(dimBloque);

  if (nb==1)
  {
    bloque[0].ant = NULL;
    bloque[0].sig = NULL;
    bloque[0].jBloque = 0;
    bloque[0].estadoBloque = BLOQUE_NO_LLENO;
    *inicio = &(bloque[0]);
    *fin    = &(bloque[0]);
    return;
  }

  bloque[0].ant  = NULL;
  bloque[0].sig = &(bloque[1]);
  bloque[0].jBloque = 0;
  bloque[0].estadoBloque = BLOQUE_NO_LLENO;

  for(int j=1; j<nb-1; j++)
  {
    bloque[j].ant = &(bloque[j-1]);
    bloque[j].sig = &(bloque[j+1]);
    bloque[j].jBloque = 0;
    bloque[j].estadoBloque = BLOQUE_NO_LLENO;
  }

  bloque[nb-1].ant = &(bloque[nb-2]);

  bloque[nb-1].sig = NULL;
  bloque[nb-1].jBloque = 0;
  bloque[nb-1].estadoBloque = BLOQUE_NO_LLENO;

  *inicio = &(bloque[0]);
  *fin    = &(bloque[nb-1]);
  
} // end inic_Bloques()

// ------------------------------------------
//--- Busca bloque segun la frecuencia

Bloque* Metodos::buscaBloque( Bloque *actual, double frec )
{
  int jb;
  while( actual!=NULL )
  {
    jb = actual->jBloque-1;

    if( actual->frec[0]>=frec && frec<=actual->frec[jb] )    
      return actual;
    else
      actual=actual->sig;
  }
  return actual;

} // end buscaBloque()

// ------------------------------------------
// --- Dentro del bloque busca la posicion 
// donde se debe insertar la nueva frec.

int Metodos::buscaPosicionBloque( Bloque *actual, double frec )
{
  for( int i=0; i<actual->jBloque; i++ )
  {
    if( frec >= actual->frec[i] )
      return i;
  }
  return actual->jBloque;

} // end buscaPosicionBloque()

// ------------------------------------------
// --- Se mueve una posicion todos los elementos
// apartir de la nueva frecuencia insertada

void Metodos::moverDerechaBloque( Bloque *actual, int doc, 
                                  double frec, int jBloque )
{
  int i, docActual, docAnterior=1;
  double frecActual, frecAnterior=0.0;
  bool primero=true;

  if( jBloque==actual->jBloque )
  {
    actual->doc[jBloque]=doc;
    actual->frec[jBloque]=frec;
  }

  else
  {
    for( i=jBloque; i<actual->jBloque; i++ )
    {
      docActual  = actual->doc[i];
      frecActual = actual->frec[i];

      if( primero==true )
      {
        actual->doc[i]  = doc;
        actual->frec[i] = frec;
        docAnterior = docActual; frecAnterior = frecActual; primero=false;
      }
      else
      {
        actual->doc[i]  = docAnterior;
        actual->frec[i] = frecAnterior;
        docAnterior = docActual; frecAnterior = frecActual;
      }
    }

    actual->doc[actual->jBloque]  = docAnterior;
    actual->frec[actual->jBloque] = frecAnterior;

  }

  actual->jBloque++;

} // end moverDerechaBloque()

// ------------------------------------------
// --- Mueven los documentos desde la mitad
// a un nuevo bloque intermedio

void Metodos::mueveSegundaMitad( Bloque *actual )
{
  int M=(actual->jBloque)/2;
  int j=0;

  for( int i=M; i<actual->jBloque; i++, j++)
  {
    actual->doc[i]  = actual->sig->doc[j];
    actual->frec[i] = actual->sig->frec[j];
  }

  actual->jBloque = M; // bloque actual
  actual->sig->jBloque=j; // nuevo bloque
}




