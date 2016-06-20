/* Implementa un dispositivo CPU con varios cores y sus respectivas caches
 *
 * */

#ifndef CHIP_H
#define CHIP_H

#include "glob.h"
#include "core/lru.h"
#include "core/core.h"
#include "pthreads/pthreads.h"

class Chip
{
private:

  int cpid;    //pid del core
  int ncores; // total cores en el chip

  LRU *L2;    // puntero a cache L2.
  LRU *L3;
  Core **cores; // arreglo de cores que tiene el chip

public:

  Chip( int     _cpid,
        int     _ncores,
        int     entradas_L1, // total entradas de tamanho PAG_CACHE bytes
        int     entradas_L2, // total entradas de tamanho PAG_CACHE bytes
        double  Latencia_G_L1_L2,
        double  Latencia_G_L2_Ram        
       )
  {
     cpid    = _cpid;
     ncores  = _ncores;

     L2      = new LRU( entradas_L2 );
     //L3      = new LRU( entradas_L3 );

     cores = new Core*[ncores];

     for ( int i=0; i<ncores; i++ )
         cores[i] = new Core( i,
                              cpid,
                              L2,
                              entradas_L1,
                              Latencia_G_L1_L2,
                              Latencia_G_L2_Ram
                             );
  }
  Chip( int     _cpid,
        int     _ncores,
        int     entradas_L1, // total entradas de tamanho PAG_CACHE bytes
        int     entradas_L2, // total entradas de tamanho PAG_CACHE bytes
        int     entradas_L3,
        double  Latencia_G_L1_L2,
        double  Latencia_G_L2_L3,
        double  Latencia_G_L3_Ram        
       )
  {
     cpid    = _cpid;
     ncores  = _ncores;
     L3      = new LRU( entradas_L3 );

     cores = new Core*[ncores];
     //cout<<"latencia L1<-L2 en chip"<<Latencia_G_L1_L2<<endl;

     for ( int i=0; i<ncores; i++ )
         cores[i] = new Core( i,
                              cpid,
                              L3,
                              entradas_L1,
                              entradas_L2,
                              Latencia_G_L1_L2,
                              Latencia_G_L2_L3,
                              Latencia_G_L3_Ram
                             );
  }

  void set_thread( handle <PThreads> *pthread,
                   int id_core
                 )
  {
     cores[ id_core ]->set_thread( pthread );
  }

};

#endif
