#ifndef LECTOR_H
#define LECTOR_H

#include "glob.h"
#include "estruc.h"
#include "metodos.h"

class Lector
{
  private:
    int dimBloque, dB, // dimension de los bloques
        QT,        // total consultas/documentos
        NT;        // total de threads

    Metodos *metodos;

  public:
    Lector();
    Lector(int _dimBloque, int qt, int nt)
    {
      dimBloque = _dimBloque; // total de bloques/nThreads
      dB        = _dimBloque*nt;      
      QT        = qt;
      NT        = nt;

      metodos   = new Metodos(dimBloque);
      
    }
    void loadQry(Query*,char*,int*,int*);
    void loadIndice(Indice**,char*, int);
    void inicMasBloques(Indice*);
};

#endif

