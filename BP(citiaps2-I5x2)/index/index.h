#ifndef INDEX_H
#define INDEX_H

#include "../glob.h"
#include "../estruc.h"
#include "../metodos/metodos.h"

class Index
{
  private:
    Metodos *metodos;
    Indice *masBloques;
    int dimBloque;

  public:
    Index(Metodos *m, Indice *mb, int tb)
    {
      metodos = m; 
      masBloques = mb;
      dimBloque = tb; 

    }

    void insert(Bloque*,int,double);
    void newBloque(Bloque*,int,int);
   
};

#endif

