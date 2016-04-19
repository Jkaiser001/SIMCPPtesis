#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include "glob.h"

class Query
{
  public:
   int tipo,    // R/W
       nt,      // total de terminos de la consulta/documento
       idDoc,   // identificador documento a insertar 
       totIter; // total de iteraciones consulta

   int *termino, // termino de la consulta/documento
       *iter;    // iteraciones de una consulta

   double *frec; // frecuencias de los terminos de un documento
       
};


class Bloque
{
  public:
   int *doc;     // lista de documentos
   double *frec; // lista frecuencias

   int jBloque,      // ultima posicion utilizada dentro del bloque
       estadoBloque, // lleno o hasta la mitad
       idBloque;     // identificador de bloque

   Bloque *ant, *sig;

   void initBloque(int n) { doc = new int[n]; frec = new double[n]; }
};

class Indice
{
  public:
    int nb,    // total de bloques por termino
        pares, // lista de pares documento+frecuencia
        libres; // bloques disponibles

    Bloque *inicioBloque, *finBloque, *actual;
   
};


#endif


