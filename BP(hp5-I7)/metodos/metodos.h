#ifndef METODOS_H
#define METODOS_H

#include "../glob.h"
#include "../estruc.h"

class Metodos
{
  private:
    int dimBloque;

  public: Metodos(int d) 
  { 
    dimBloque = d;
  }

  Bloque *buscaBloque(Bloque*,double);

  void inic_Bloques(Bloque**,Bloque**,int);
  void moverDerechaBloque(Bloque*,int,double,int);
  void mueveSegundaMitad(Bloque*);

  int buscaPosicionBloque(Bloque*,double);

};
#endif
