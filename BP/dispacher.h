
<<<<<<< HEAD
#include "estadisticas.h"
#include "estruc.h"
#include "locks.h"
#include "metodos.h"
#include "search.h"
#include "index.h"
class Dispacher:public process
{
private:
	Lector *lector;
	char nombre[1024];

public:
	

	 Dispacher(Lector *_lector, const string& _name): process( _name ){
	 	lector=_lector;
	 	strcpy(nombre,_name.c_str());
	 }
	 void inner_body( void );
=======
class Dispacher//:public process
{
public:
	Lector lector;


	void Dispacher(Lector *);
>>>>>>> 2ae837822e5ecf7b7139133f8d2a5a81c4a2a632
	
	~Dispacher();

	
};

#endif 
