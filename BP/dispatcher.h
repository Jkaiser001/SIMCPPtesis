#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "glob.h"
#include "lector.h"
#include "estadisticas.h"
#include "estruc.h"
#include "pthreads.h"
#include "metodos.h"
#include "search.h"
#include "index.h"
class Dispatcher:public process
{
private:
	Lector *lector;
	char nombre[1024];
	int nthreads;
	handle<Dispatcher> *despachador;
	rng<double> *arrival_time;
	handle<PThreads>* handle_pthread;

public:
	
	Dispatcher(int _nthreads, Lector *_lector,handle<Dispatcher> *_despachador,handle<PThreads>* _handle_pthread, const string& _name): process( _name ){		 
  	 	lector=_lector;	
 	 	despachador=&(_despachador[0]);		
  	 	strcpy(nombre,_name.c_str());		  	 	
  	 	handle_pthread=_handle_pthread;
  	 	nthreads=_nthreads;
  	 }		  	 
 	void inner_body( void );		 
 	void duerme();
 	void despierta();		
 	void espera();
	
};

#endif 
