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

public:
	

	 Dispatcher(Lector *_lector, const string& _name): process( _name ){
	 	lector=_lector;
	 	strcpy(nombre,_name.c_str());
	 }
	 void inner_body( void );
	
	
	
};

#endif 
