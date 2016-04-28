
#include "dispatcher.h"

void Dispatcher::inner_body( void ){

	while(1){
		if(!lector->emptylistQuery()){
			Query query = lector->getQuery();
			cout<<"El tipo es: "<<query.tipo<<", vamos bien!!! "<<this->time()<<endl;
			double newRate = (1.0/NORMAL_RATE);
	      	arrival_time= new rngExp("Arrive Time", newRate);
	      	arrival_time->reset();
	      	double CTE=arrival_time->value();
	      	cout<<"retardo:  "<<CTE<<endl;
	      	hold(CTE);
	      	cout<<"llegue al final"<<endl;
		}else{
			duerme();
		}
	}

}

void Dispatcher::duerme(){
	(*despachador)->passivate( );
}

void Dispatcher::despierta() { (*despachador)->activateAfter( current( ) ); }
