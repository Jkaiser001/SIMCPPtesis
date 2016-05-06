#ifndef DATACHIP_H_
#define DATACHIP_H_
#include "dataThread.h"
class dataChip
{

private:
	int nThreadChip;
    double tiempoActivo;
    double tiempoInactivo;
    double tiempoTotal;
    double utilizacion;
    double utilizacionAcum;
public:

	

    dataChip(){}

    dataChip(const dataChip &copyin)   // Copy constructor to handle pass by value.
	{                            
   		nThreadChip = copyin.nThreadChip;
   		tiempoActivo = copyin.tiempoActivo;
   		tiempoInactivo = copyin.tiempoInactivo;
   		tiempoTotal = copyin.tiempoTotal;
   		utilizacion = copyin.utilizacion;
   		utilizacionAcum = copyin.utilizacionAcum;
	}

   	
   	dataChip& operator=(const dataChip &rhs)
	{
   
   	this->nThreadChip=rhs.nThreadChip;
    this->tiempoActivo=rhs.tiempoActivo;
    this->tiempoInactivo=rhs.tiempoInactivo;
    this->tiempoTotal=rhs.tiempoTotal;
    this->utilizacion=rhs.utilizacion;
    this->utilizacionAcum=rhs.utilizacionAcum;

   return *this;
	}
 
	int operator==(const dataChip &rhs) const
	{
   		if( this->nThreadChip != rhs.nThreadChip) return 0;
   		if( this->tiempoActivo != rhs.tiempoActivo) return 0;
   		if( this->tiempoInactivo != rhs.tiempoInactivo) return 0;
   		if( this->tiempoTotal != rhs.tiempoTotal) return 0;
   		if( this->utilizacion != rhs.utilizacion) return 0;
   		if( this->utilizacionAcum != rhs.utilizacionAcum) return 0;
   		return 1;
	}
 
/*int operator<(const dataChip &rhs) const
{
   if( this->x == rhs.x && this->y == rhs.y && this->z < rhs.z) return 1;
   if( this->x == rhs.x && this->y < rhs.y) return 1;
   if( this->x < rhs.x ) return 1;
   return 0;
}*/

 
 	dataChip(double _tiempoActivo,double _tiempoInactivo,double _tiempoTotal,double _utilizacionAcum){

		nThreadChip=1;
		tiempoActivo=_tiempoActivo;
		tiempoInactivo=_tiempoInactivo;
		tiempoTotal=_tiempoTotal;
		utilizacion=(_tiempoActivo/_tiempoTotal)*100;
		utilizacionAcum=_utilizacionAcum;
	}
	dataChip(dataThread _dato){

		nThreadChip=1;
		tiempoActivo=_dato.getTiempoActivo();
		tiempoInactivo=_dato.getTiempoInactivo();
		tiempoTotal=_dato.getTiempoTotal();
		utilizacion=(tiempoActivo/tiempoTotal)*100;
		utilizacionAcum=_dato.getUtilizacion();
	}
	
	
	void setUtilizacion(double _tiempoInactivo, double _tiempoActivo, double _utilizacion , double _tiempoTotal){
		tiempoActivo=tiempoActivo+_tiempoActivo ;
		tiempoInactivo=tiempoInactivo+_tiempoInactivo;
		tiempoTotal=tiempoTotal+_tiempoTotal;	
		utilizacion=(_tiempoActivo/_tiempoTotal)*100;
		utilizacionAcum=utilizacionAcum+_utilizacion;
		nThreadChip++;

	}
	void setUtilizacion(dataThread _dato){
		tiempoActivo=tiempoActivo+_dato.getTiempoActivo() ;
		tiempoInactivo=tiempoInactivo+_dato.getTiempoInactivo();
		tiempoTotal=tiempoTotal+_dato.getTiempoTotal();	
		utilizacion=(tiempoActivo/tiempoTotal)*100;
		utilizacionAcum=utilizacionAcum+_dato.getUtilizacion();
		nThreadChip++;

	}
	double getUtilizacion(){return utilizacion;}
	double getUtilizacionAcum(){return utilizacionAcum;}
	void addNThreadChip(){ nThreadChip++; }
	int getNThreadChip(){return nThreadChip;}
	double getTiempoTotal(){ return  tiempoTotal;}
	
	
};


#endif