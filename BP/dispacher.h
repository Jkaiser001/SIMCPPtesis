#ifndef DISPACHER_H
#define DISPACHER_H
#include "glob.h"
#include "lector.h"
class Dispacher//:public process
{
public:
	Lector lector;


	void Dispacher(Lector *);
	
	~Dispacher();

	
};

#endif 
