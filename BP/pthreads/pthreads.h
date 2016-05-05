
#ifndef PTHREADS_H
#define PTHREADS_H

#include "../glob.h"
#include "../estadisticas.h"
#include "../estruc.h"
#include "../locks/locks.h"
#include "../metodos/metodos.h"
#include "../search.h"
#include "../index/index.h"
//#include "dispatcher.h"
//#include "core.h"

extern Estadisticas *estadisticas;
extern int *ov_barrier;
extern int full_barrier;

class Core;

class PThreads : public process
{
public:

  int pid, NT;
  int count[3];
  char nombre[1024];
  Core *core;
  handle<PThreads>* htrd;
  handle<PThreads>* htrd_barrier;
  list<Query> listQuery_ptheards;
  bool statusDispacher;
  bool terminado;
  //handle<Dispatcher> *despachador;
  Locks* lock;
  int QT, dimBloque, nTerm;

  Query   *query;
  Indice **indice;
  Metodos *metodos;
  Bloque  *inicio, *fin;
  Index   *index;

  int nscores; // similitud coseno (ranking)
  double *scores;
  int  sizeBloque;
  int  bytesBloque;
  int  bytesItem;

  PThreads(
             int _pid,
             int _NT,
             const string& _name,
             handle<PThreads>* handle_pthread,
             //handle<Dispatcher> *_despachador,
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
     //despachador= &(_despachador[0]);

     lock      = _locks;
     QT        = _qt;
     dimBloque = _dimBloque;
     nTerm     = _nTerm;
     query     = _query;
     indice    = _indice;

     nscores   = dimBloque*nTerm;
     for (int i = 0; i < 3; ++i)
     {
        count[i]=1;
     }
     statusDispacher=1;
/*     
     scores    = new double[nscores];
     
     metodos   = new Metodos(dimBloque);
     index     = new Index(metodos, masBloques, dimBloque);
*/     
  }

  void imprime();

  void inner_body( void );

 // definimos varios phold iguales para obtener
 // las estadisticas diferenciadas por core, L1 y L2.
  void add_query(Query );
  Query pop_query();
  void setStatus();
  void phold( double  );
  void phold2( double, int , int );
  void phold3( double );
  void phold4( double );
  void duerme(); 
  void despierta(); 
  void set_core( Core* );
  void runRead(Query);
  void runWrite(Query);
  void runCore(double,long,int);
  void runCore2(double);
  void set_lock( int );
  void set_unlock( int );
  void ovBarrier( int tid );
  void Barrier( );     
  int getPid();
  void setValSimBloque(int n);  

};

#endif

