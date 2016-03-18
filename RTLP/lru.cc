
#include "lru.h"

bool LRU::hit( string key )
{
  if( cache.empty( ) )
    return false;

  if( cache.count( key ) == 0 )
    return false;
  else
    return true;
}

//---------------------------------------------------

void LRU::insert( string key )
{
  if( !queda_espacio( ) )
  {
     generar_espacio( );
  }

  //verificar que no este

  ASSERT( cache.count( key ) == 0 );

  //poner en cache nuevo nodo
  ptr = new Nodo( TS, key );
  
  cola_prioridad.push( ptr );
  
  usado++;

  //enlazar
  cache[key] = ptr;

  //aumentar timestamp
  TS++;
}

//---------------------------------------------------

void LRU::update( string key )
{
  //marcar la entrada
  cache[key]->marca = false;

  //agregar una nueva
  ptr = new Nodo( TS , key );
  
  cola_prioridad.push( ptr );

  //enlazar
  cache[key] = ptr;

  //aumentar timestamp
  TS++;
}

//---------------------------------------------------

bool LRU::queda_espacio( )
{
   if( usado < NC )
     return true;
   return false;
}

//---------------------------------------------------

void LRU::generar_espacio( )
{
   ptr = cola_prioridad.top( );
    
   while( ptr->marca == false )
   {
      cola_prioridad.pop( );
      delete ptr;
      ptr = cola_prioridad.top( );
   }
    
   cola_prioridad.pop( );
   cache.erase( ptr->key );
   delete ptr;
   usado--;  
}

//---------------------------------------------------

