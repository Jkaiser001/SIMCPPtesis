
#ifndef LRU_H
#define LRU_H

#include "glob.h"


//-------------------------------------------------

class Nodo
{
public:

  double prioridad;
  string key;
  bool marca; // marca item PQ como borrado.
 
  Nodo( double _prio, string _key )
  {
    prioridad = _prio;
    key       = _key;
    marca     = true;
  }
};

struct Compara: binary_function<Nodo*,Nodo*,bool>
{
  bool operator() (Nodo* a, Nodo* b)
  {
    return a->prioridad > b->prioridad;
  }
};

//-------------------------------------------------

class LRU
{
private:

  //--cache
  map< string /*key*/, Nodo* /*ptr*/ > cache;
  
  int NC;    // numero de entradas totales (capacidad del cache).
  int usado; // entradas utilizadas.
  double TS; // timestamp para prioridades.
  Nodo *ptr;
  
  priority_queue< Nodo*, vector<Nodo*>, Compara > cola_prioridad;  

public:

  double ver_prio( string str) 
  { 
     if( cache.count( str ) != 0 ) 
        return cache[str]->prioridad; 
     else
        return -1.0;
  }
  
  int ver_usado(){ return usado;}
  int ver_entradas(){ return NC;}  
  
  LRU( int _nc )
  {
     TS    = 0;      
     NC    = _nc;
     usado = 0;
  }

  ~LRU( )
  {
     cache.clear( );
  }

  bool hit( string/*key*/ );
  void insert( string/*key*/ );
  void update( string/*key*/ );
  bool queda_espacio( );
  void generar_espacio(  );
  
};

#endif

