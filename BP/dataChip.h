#ifndef DATA_H
#define DATA_H
class data
{
private:
	int NthreadChip;
    double tiempoActivo;
    double tiempoInactivo;
    double tiempoTotal;
    double utilizacion;
    double utilizacionAcum;

public:
	data(int nthreadchip,double tiempoActivo, double tiempoInactivo,double utilizacionAcum);
	~data();
	
	
};
#endif
