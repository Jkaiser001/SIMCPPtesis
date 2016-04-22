#ifndef DATACHIP_H
#define DATACHIP_H
class dataChip
{
private:
	int NthreadChip;
    double tiempoActivo;
    double tiempoInactivo;
    double tiempoTotal;
    double utilizacion;
    double utilizacionAcum;

public:
	dataChip(int nthreadchip,double tiempoActivo, double tiempoInactivo,double utilizacionAcum);
	~dataChip();
	
	
};
#endif
