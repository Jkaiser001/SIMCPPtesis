#ifndef LECTOR_H
#define LECTOR_H

#include "glob.h"
#include "estruc.h"
#include "metodos.h"

class Lector
{
  private:
    int dimBloque, // dimension de los bloques
        QT,        // total consultas/documentos
        NT;        // total de threads

    Metodos *metodos;

  public:
    Lector(int db, int qt, int nt)
    {
      dimBloque= db; // total de bloques
      QT = qt;
      NT = nt;
      metodos = new Metodos(dimBloque);
      
    }
    void loadQry(Query*,char*,int*,int*);
    void loadIndice(Indice*,char*,int);
    void inicMasBloques(Indice*);
};

#endif

