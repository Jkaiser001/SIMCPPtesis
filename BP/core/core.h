/* Implementa un dispositivo CPU con varios cores y sus respectivas caches
 *
 * */

#ifndef CORE_H
#define CORE_H

#include "../glob.h"
#include "lru.h"
#include "../pthreads/pthreads.h"
#include <string.h>

extern Estadisticas *estadisticas;

class Core
{
public:
  int cpid; // id del chip al que pertenece
  int id_core;

private:
  handle<PThreads> *pthread; // un thread por core

  LRU *L1;
  LRU *L2;
  LRU *L3;

  double  Latencia_G_L1_L2;
  double  Latencia_G_L2_L3;
  double  Latencia_G_L3_Ram;
  double  Latencia_G_L2_Ram;

  // aux
  vector<string> tokens;
  char str[1024];

public:

  Core( int _id_core,
        int _cpid,
        LRU *_L2,
        int entradas_L1,
        double  _Latencia_G_L1_L2,
        double  _Latencia_G_L2_Ram
      )
  {
      cpid    = _cpid;
      id_core = _id_core;
      L2      = _L2;

      Latencia_G_L1_L2  = _Latencia_G_L1_L2;
      Latencia_G_L2_Ram = _Latencia_G_L2_Ram;

      L1 = new LRU( entradas_L1 );

     // cout<<"latencia L1<-L2 en core"<<Latencia_G_L1_L2<<endl;

  }

   Core( int _id_core,
        int _cpid,
        LRU *_L3,
        int entradas_L1,
        int entradas_L2,
        double  _Latencia_G_L1_L2,
        double  _Latencia_G_L2_L3,
        double  _Latencia_G_L3_Ram
      )
  {
      cpid    = _cpid;
      id_core = _id_core;
      L3      = _L3;

      Latencia_G_L1_L2 = _Latencia_G_L1_L2;
      Latencia_G_L2_L3 = _Latencia_G_L2_L3;
      Latencia_G_L3_Ram = _Latencia_G_L3_Ram;

      L1 = new LRU( entradas_L1 );
      L2 = new LRU( entradas_L2 );

     // cout<<"latencia L1<-L2 en core"<<Latencia_G_L1_L2<<endl;

  }

  int getCPid(); 
  int getIdCore();

  void set_thread( handle<PThreads> *);

  void run( double, string, int );
  void run2( double );
  void run3( double, string, int );
  

};

#endif

