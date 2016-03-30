
#ifndef PTHREADS_H
#define PTHREADS_H

#include "glob.h"
#include "estadisticas.h"
#include "estruc.h"
#include "locks.h"
#include "metodos.h"
#include "search.h"
#include "index.h"

extern Estadisticas *estadisticas;
extern int *ov_barrier;
extern int full_barrier;

class Core;

class PThreads : public process
{
public:

   int pid, NT;
   char nombre[1024];
   Core *core;
   handle<PThreads>* htrd;
   handle<PThreads>* htrd_barrier;
   Locks* lock;
   int QT, dimBloque, nTerm;

   Query   *query;
   Indice **indice;
   Metodos *metodos;
   Bloque  *inicio, *fin;
   Index   *index;

   int nscores; // similitud coseno (ranking)
   double *scores;

   PThreads(
             int _pid,
             int _NT,
             const string& _name,
             handle<PThreads>* handle_pthread,
             Locks *_locks,
             int _qt,
             int _dimBloque,
             int _nTerm,
             Query *_query,
             Indice **_indice,
             Indice *masBloques
           ) : process( _name )
   {
     pid = _pid;
     NT  = _NT; // total de threads
     strcpy(nombre,_name.c_str());

     htrd = &(handle_pthread[pid]);
     htrd_barrier = handle_pthread;

     lock      = _locks;
     QT        = _qt;
     dimBloque = _dimBloque;
     nTerm     = _nTerm;
     query     = _query;
     indice    = _indice;

     nscores   = dimBloque*nTerm;
/*     
     scores    = new double[nscores];
     
     metodos   = new Metodos(dimBloque);
     index     = new Index(metodos, masBloques, dimBloque);
*/     
  }

  void imprime()
  {
     printf("Ef= %.4lf \n", estadisticas->Eficiencia() );
     estadisticas->muestraFallas();
  }

  void inner_body( void );

 // definimos varios phold iguales para obtener
 // las estadisticas diferenciadas por core, L1 y L2.

  void phold( double t )
  {
#ifdef MIDE_ESTADISTICAS
    //estadisticas->mide( pid, time(), t );//Borrar
#endif 

     hold(t);
     //cout<<"Para la hebra "<<pid<<" tiempo de holt1:"<<t<<endl;
  }

  void phold2( double t )
  {
#ifdef MIDE_ESTADISTICAS
//     estadisticas->mide( pid, time(), t );
     estadisticas->fallaL1L2( pid );
#endif
     hold(t);
  }

  void phold3( double t )
  {
#ifdef MIDE_ESTADISTICAS
//     estadisticas->mide( pid, time(), t );
     estadisticas->fallaL2Ram( pid );
#endif
     hold(t);
  }

  void duerme() { (*htrd)->passivate( ); }
  void despierta() { (*htrd)->activateAfter( current( ) ); }

  void set_core( Core *_core)
  {
     core = _core;
  }

  void runRead(int);
  void runWrite(int);
  void runCore(double,long,int);
  void runCore2(double);

  void set_lock( int term )
  {
     char str[128];
     sprintf(str,"%d",term);
     lock->set_lock(str,htrd,pid);
  }

  void set_unlock( int term )
  {
     char str[128];
     sprintf(str,"%d",term);
     lock->set_unlock(str);
  }


  void ovBarrier( int tid )
  {
     bool flag=true;

     set_lock( -tid );

     ov_barrier[tid]++;

     if ( pid == tid )
     {
       if ( ov_barrier[tid] < NT )
       { set_unlock( -tid ); duerme(); flag=false; }
       else
         ov_barrier[tid] = 0;
     }
     else
     if ( ov_barrier[tid] == NT )
     {
       ov_barrier[tid] = 0;
       (*htrd_barrier[tid]).despierta();
     }

     if (flag==true)
     set_unlock( -tid );
  }


  void Barrier( )
  {
     full_barrier++;

     if ( full_barrier < NT )
       duerme();
     else
     {
       full_barrier = 0;

       for(int i=0;i<NT;i++)
       {
         if (i!=pid)
           (*htrd_barrier[i]).despierta();
       }
     }
  }

};

#endif

