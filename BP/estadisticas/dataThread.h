#ifndef DATATHREAD_H_
#define DATATHREAD_H_
class dataThread
{

public:

	int pid ;
    double tiempothread;
    double tiempoActivo;
    double tiempoInactivo;
    double tiempoTotal;
    double utilizacion;

    dataThread();

	dataThread(int _pid,double _tiempothread,double _tiempoActivo,double _tiempoInactivo,double _tiempoTotal,double _utilizacion){

		pid=_pid;
		tiempothread=_tiempothread;
		tiempoActivo=_tiempoActivo;
		tiempoInactivo=_tiempoInactivo;
		tiempoTotal=_tiempoTotal;
		utilizacion=_utilizacion;
	}
	~dataThread()

	double getTiempoInactivo(){return tiempoInactivo;}
	double getTiempoActivo(){ return  tiempoActivo;}
	double getTiempoTotal(){ return  tiempoTotal;}
	double getUtilizacion(){return utilizacion;}
	int getPid(){return pid; }
	
	
};


#endif