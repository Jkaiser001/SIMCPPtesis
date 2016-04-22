/* Implementa un dispositivo CPU con varios cores y sus respectivas caches
 *
 * */

#ifndef CORE_H
#define CORE_H

#include "glob.h"
#include "lru.h"
#include "pthreads.h"
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

  double  Latencia_G_L1_L2;
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

  int getCPid(){return cpid;} 
  int getIdCore(){return id_core;}

  void set_thread( handle<PThreads> *_pthread )
  {
      pthread = _pthread;
      (*pthread)->set_core(this);
  }

  void run2( double t_cpu )  
  {
     if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
  }
  
  void run( double t_cpu, string base, int bytes )
  {
     int npag= (int)ceil( double(bytes) / double(PAG_CACHE) );
   
     t_cpu /= npag;

     for(int i=0; i< npag; i++)
     {
        sprintf(str,"%s %d", base.c_str(), i);

        if ( L1->hit( str ) )
        {
#ifdef MIDE_ESTADISTICAS
           estadisticas->hit_L1( (*pthread)->pid );
#endif
           L1->update( str );// me falta este tiempo
           if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
        }
        else
        {
           if ( L2->hit( str ) )
           {
#ifdef MIDE_ESTADISTICAS
              estadisticas->hit_L2( (*pthread)->pid );
#endif
              L2->update( str );//me falta este tiempo
              L1->insert( str );
              (*pthread)->phold2( Latencia_G_L1_L2 );
            if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
            }
            else
            {
#ifdef MIDE_ESTADISTICAS
              estadisticas->hit_Ram( (*pthread)->pid );
#endif
              L2->insert( str );
              L1->insert( str );
              (*pthread)->phold2( Latencia_G_L1_L2 );
              (*pthread)->phold3( Latencia_G_L2_Ram );
            if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
            }

        } 

     }

  }

};

#endif

