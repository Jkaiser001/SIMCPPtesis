
#include "index.h"

// ---------------------------------------------------
// --- Inserta dentro del bloque actual

void Index::insert(Bloque *actual, int doc, double frec)
{
  int jb = actual->jBloque;
  actual->doc[jb]  = doc;
  actual->frec[jb] = frec;
  actual->jBloque++;

  // comprueba estado del bloque
  if( actual->jBloque==dimBloque )
    actual->estadoBloque=BLOQUE_LLENO;

} // end insert()


// ---------------------------------------------------
// --- crea un nuevo Bloque al final "termino"

void Index::newBloque(Bloque *actual, int tid, int termino)
{
  if( masBloques[tid].libres==0 && masBloques[tid].actual == NULL)
  {
    Bloque *inicioMasBloques, *finMasBloques;

    masBloques[tid].libres = MAX_BLOQUES;
    masBloques[tid].nb+=MAX_BLOQUES;

    //pthread_mutex_lock(&mute);
    metodos->inic_Bloques( &inicioMasBloques, &finMasBloques, MAX_BLOQUES );
    //pthread_mutex_unlock(&mute);

    masBloques[tid].inicioBloque = inicioMasBloques;
    masBloques[tid].actual       = inicioMasBloques;
    masBloques[tid].finBloque    = finMasBloques;
  }

  /*
  pido mi bloque y dejo los punteros como corresponden. aun no inserto nada
  solo redirecciono los punteros de la lista de cada termino al nuevo bloque
  */

  Bloque *aux;
  Bloque *nuevo = masBloques[tid].actual;

  aux = actual->sig;
  actual->sig = nuevo;
  nuevo->ant = actual;

  masBloques[tid].libres--;
  masBloques[tid].actual = masBloques[tid].actual->sig;

  if (aux!=NULL)
  {
    nuevo->sig = aux;
    aux->ant = nuevo;
  }
  else 
  {
    nuevo->sig = aux;
  }

} // end newBloque()



