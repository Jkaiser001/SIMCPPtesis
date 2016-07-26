#ifndef DATACACHE_H_
#define DATACACHE_H_
#include "dataThread.h"
class dataCache
{

private:
	double tiempoActivo;
    double tiempoTotal;
    double lecturas;
    double escrituras;
    double energiaLec;
    double energiaEsc;
    double utilizacion;
public:

	

    dataCache(){}

    dataCache(const dataCache &copyin)   // Copy constructor to handle pass by value.
	{                            
   		
   		tiempoActivo = copyin.tiempoActivo;
   		tiempoTotal = copyin.tiempoTotal;
   		lecturas = copyin.lecturas;
   		escrituras = copyin.escrituras;
   		energiaLec = copyin.energiaLec;
   		energiaEsc = copyin.energiaEsc;

   		utilizacion = copyin.utilizacion;
	}

   	
   	dataCache& operator=(const dataCache &rhs)
	{
   
   	this->tiempoActivo=rhs.tiempoActivo;
    this->tiempoTotal=rhs.tiempoTotal;
    this->lecturas = rhs.lecturas;
    this->escrituras = rhs.escrituras;
    this->energiaLec = rhs.energiaLec;
    this->energiaEsc = rhs.energiaEsc;
    this->utilizacion=rhs.utilizacion;
    
   return *this;
	}
 
	int operator==(const dataCache &rhs) const
	{
   		if( this->tiempoActivo != rhs.tiempoActivo) return 0;
   		if( this->tiempoTotal != rhs.tiempoTotal) return 0;
   		if( this->escrituras != rhs.escrituras) return 0;
   		if( this->lecturas != rhs.lecturas) return 0;
   		if( this->energiaEsc != rhs.energiaEsc) return 0;
   		if( this->energiaLec != rhs.energiaLec) return 0;
   		if( this->utilizacion != rhs.utilizacion) return 0;
   		return 1;
	}
 
/*int operator<(const dataCache &rhs) const
{
   if( this->x == rhs.x && this->y == rhs.y && this->z < rhs.z) return 1;
   if( this->x == rhs.x && this->y < rhs.y) return 1;
   if( this->x < rhs.x ) return 1;
   return 0;
}*/

 
 	dataCache(double _tiempoActivo,double _tiempoTotal,double _lecturas,double _escrituras,double _enegia_escritura,double _enegia_lectura){

		tiempoActivo=_tiempoActivo;
		tiempoTotal=_tiempoTotal;
		lecturas=_lecturas;
		escrituras=_escrituras;
		energiaLec=_enegia_lectura;
		energiaEsc=_enegia_escritura;

		utilizacion=(_tiempoActivo/_tiempoTotal)*100;
	}
	
	
	
	void setUtilizacion(double _tiempoInactivo, double _tiempoActivo, double _utilizacion , double _tiempoTotal){
		tiempoActivo=tiempoActivo+_tiempoActivo ;
		tiempoTotal=tiempoTotal+_tiempoTotal;	
		utilizacion=(_tiempoActivo/_tiempoTotal)*100;
	
	}
	
	double getUtilizacion(){return utilizacion;}
	double getTiempoTotal(){ return  tiempoTotal;}
  double get_lecturas(){return lecturas;}
  double get_energia_lecturas(){return energiaLec;}
	
	
};

#endif