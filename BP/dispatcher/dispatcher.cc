
#include "dispatcher.h"

void Dispatcher::inner_body( void ){
int i=0,qry=-1;
while(1){

		qry++;
		if( qry>=QT ) break;
		if(!lector->emptylistQuery()){		
			Query query = lector->getQuery();	
			
			handle_pthread[i%nthreads]->add_query(query);
			//cout<<"pid= "<<i%nthreads<<"Cargo del tipo: "<<query.tipo<<", DISPATCHER "<<endl;		
			double newRate = (1.0/NORMAL_RATE);		
	      	arrival_time= new rngExp("Arrive Time", newRate);		
	      	arrival_time->reset();		
	      	double CTE=arrival_time->value();		
	      	//cout<<"retardo:  "<<CTE<<endl;		
	      	hold(CTE);		
	      	//cout<<"llegue al final"<<endl;		
		}else{		
			break;		
		}
	//cout<<"qry "<<qry<<endl;		
	
	i++;

	}
	for (int j = 0; j < nthreads; ++j)
	{
		handle_pthread[j]->setStatus();
	}
	duerme();

}
void Dispatcher::duerme(){		
 	(*despachador)->passivate( );		
}		

 void Dispatcher::despierta() { (*despachador)->activateAfter( current( ) ); }
