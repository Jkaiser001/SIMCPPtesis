#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "glob.h"
#include "lector.h"
#include "estadisticas.h"
#include "estruc.h"
#include "locks.h"
#include "metodos.h"
#include "search.h"
#include "index.h"
class Dispatcher:public process
{
private:
	Lector *lector;
	char nombre[1024];
	handle<Dispatcher> *despachador;
	rng<double> *arrival_time;
public:
	

	 Dispatcher(Lector *_lector,handle<Dispatcher> *_despachador, const string& _name): process( _name ){
	 	lector=_lector;
	 	despachador=&(_despachador[0]);
	 	strcpy(nombre,_name.c_str());
	 }
	void inner_body( void );
	void duerme();
	void despierta();
	void espera();
	
	
};

#endif 
