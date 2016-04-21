#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H
#include "glob.h"
#include "estruc.h"

typedef  struct DATAT
{ 
    int pid ;
    double tiempothread;
    double tiempoActivo;
    double tiempoInactivo;
    double tiempoTotal;
    double utilizacion;
  
}dataT;
typedef  struct DATAC
{ 
   // int datos;
    //int ncore;
    //double tiempocore;
    int NthreadCore;
    double tiempoActivo;
    double tiempoInactivo;
    double tiempoTotal;
    double utilizacion;
    double utilizacionAcum;
  
}dataC;



class Estadisticas
{
private:

  int NT; // numero de threads
  int nchips;
  int  Ncores;
  double *minimo;
  double *maximo;
  double eficiencia, numEf;

  double *fallasCacheL1L2;
  double *fallasCacheL2Ram;
  double *acumuladoresTiempoRamL2;
  double **acumuladoresTiempoL2L1;
  double *hitL1, *hitL2, *hitRam;
  double **tiempoXThread;

  double deltaTiempo;
  double *tiempoAcumulado;
  vector<dataT> vectorMuestreoT;
  map<int, map<double,dataC> > mapMuestreoC;

public:

  Estadisticas(int nt,int delta)
  {
     NT= nt;

     minimo = new double[nt];
     maximo  = new double[nt];
     for(int i=0;i<nt;i++) minimo[i]=maximo[i]=1000e100;

     eficiencia=0.0;
     numEf=0.0;

     fallasCacheL1L2 = new double[nt];
     for(int i=0;i<nt;i++) fallasCacheL1L2[i]=0;

     fallasCacheL2Ram = new double[nt];
     for(int i=0;i<nt;i++) fallasCacheL2Ram[i]=0;

     hitL1  = new double[nt];
     hitL2  = new double[nt];
     hitRam = new double[nt];
     tiempoAcumulado=new double[nt];
     tiempoXThread = (double **) malloc(nt *sizeof(double *));
     for (int i = 0; i < nt; ++i)
     {
       tiempoXThread[i]=(double *) malloc(nt*sizeof(double));
     }

     for(int i=0;i<nt;i++){
       hitL1[i]= hitL2[i]= hitRam[i]=tiempoXThread[i][0]=tiempoXThread[i][1]=tiempoXThread[i][2]=0;
       tiempoAcumulado[i]=0; 
     }
        
      deltaTiempo=delta;
      
  }


  void hit_Ram( int pid ) { hitRam[pid]++; }
  void hit_L1( int pid ) { hitL1[pid]++; }
  void hit_L2( int pid ) { hitL2[pid]++; }

  void fallaL1L2( int pid ) { fallasCacheL1L2[pid]++; }
  void fallaL2Ram( int pid ) { fallasCacheL2Ram[pid]++;  }
  void graficar(){
    char * configGnuplot[] = {  "set term png",
                                "set title \"Utilización vs Tiempo por thread\"", 
                                "set ylabel \"----Utilización--->\"",
                                //"set format y\"%2.f\"",
                                "set xlabel \"----Tiempo--->\"",
                                //"set multiplot",
                                //"set size 1,0.5"
                                "set yrange[80:100]",
                                "set grid",
                                "show grid"

                                //instrucionc
                               };
    FILE * ventanaGnuplot = popen ("gnuplot -persist", "w");
    for (int k=0;k<5;k++){
      fprintf(ventanaGnuplot, "%s \n", configGnuplot[k]);
    }

    for (int i = 0; i < NT; ++i)
    {
      string nameG="Utilizacion_Thread-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
      string instrucionP = "plot \"out/"+ nameG+".out\" with lines";
      string instrucionG = "set out \"graf/Grafico_"+nameG+".png\"";
      char  * instrucionPc = new char [instrucionP.length()+1];
      strcpy (instrucionPc, instrucionP.c_str());
      char *  instrucionGc = new char [instrucionG.length()+1];
      strcpy (instrucionGc, instrucionG.c_str());

      fprintf(ventanaGnuplot, "%s \n", instrucionGc);
      fprintf(ventanaGnuplot, "%s \n", instrucionPc);
    
    }
    fprintf(ventanaGnuplot, "%s \n", "exit");  
    //cout<<"-------------------------------------------------------------"<<endl;
  }
  void guardarIntervalosUtilizacion(){
    
     
    for (int j = 0; j < NT; ++j )
    {
      std::string name= "out/Utilizacion_Thread-"+static_cast<std::ostringstream*>(&(std::ostringstream() << j))->str()+".out" ;
      ofstream out;
      //const char *namec=name.c_str();
      char * namec = new char [name.length()+1];
      strcpy (namec, name.c_str());

      //namec = (char *)alloca(name.size() + 1);
    //memcpy(namec, name.c_str(), name.size() + 1);
      out.open(namec);
      //cout<<"-------------------------------------------------------------"<<endl;
      //cout<<"GRAFICAR"<<endl;
      for (int i = 0; i < vectorMuestreoT.size() ; ++i)
      {   
         
         if (vectorMuestreoT[i].pid==j && vectorMuestreoT[i].tiempothread!=0)
         {
           //cout<<"El pid: "<<vectorMuestreoT[i].pid<<" tiempo thread:"<<vectorMuestreoT[i].tiempothread<<endl;
            //cout<<"Porcentaje de Utilización de la thread "<<vectorMuestreoT[i].utilizacion<<endl;
            out<< vectorMuestreoT[i].tiempothread/1e6<<", "<<vectorMuestreoT[i].utilizacion<<endl;
         }
       
      }

       out.close();
    }

  }

  void UtilizacionCore(){
    //double **salida;
    //salida=(double **)malloc(Ncores * sizeof(double *));
    //int core=0;
    /*for (int i = 0; i < Ncores; ++i)
    {
        salida[i]=(double *)malloc(3*sizeof(double ));
        for (int j = 0; j < 3; ++i)
        {
          salida[i][j]=0;
        }
      
    }*/
    cout<<"___________________________ENTRE____________________________"<<endl;
      for (int i = 0; i < vectorMuestreoT.size() ; ++i)
      {   
        dataT dato=vectorMuestreoT[i];
        dataC datoc;
        int core=dato.pid/4;
        double tiempo=dato.tiempothread;
        
        if (mapMuestreoC.find(core)!=mapMuestreoC.end())
        {
          //cout<<"Voy por el 1° if"<<core<<endl;
          if(mapMuestreoC[core].find(tiempo)==mapMuestreoC[core].end())
          {
            //cout<<"Voy por el 2° if"<<core<<endl;
             
              double tiempoTotal = mapMuestreoC[core][tiempo].tiempoTotal+dato.tiempoTotal;
              double tiempoActivo = mapMuestreoC[core][tiempo].tiempoActivo+dato.tiempoActivo;
              double utilizacion = tiempoActivo/tiempoTotal*100;
              mapMuestreoC[core][tiempo].NthreadCore++;
              mapMuestreoC[core][tiempo].tiempoTotal=tiempoTotal;
              mapMuestreoC[core][tiempo].tiempoActivo=tiempoActivo;
              mapMuestreoC[core][tiempo].utilizacion=utilizacion;
              //mapMuestreoC[core][tiempo].tiempoInactivo=mapMuestreoC[core][tiempo].tiempoTotalInactivo+dato.tiempoInactivo;
              mapMuestreoC[core][tiempo].tiempoInactivo=mapMuestreoC[core][tiempo].tiempoInactivo+dato.tiempoInactivo;
              mapMuestreoC[core][tiempo].utilizacionAcum=mapMuestreoC[core][tiempo].utilizacionAcum+dato.utilizacion;

          }
          else
            {
              //cout<<"Voy por el 2° else"<<core<<endl;

                
                double tiempoTotal = dato.tiempoTotal;
                double tiempoActivo = dato.tiempoActivo;
                double utilizacion = tiempoActivo/tiempoTotal*100;
                //mapMuestreoC[core][tiempo].ncore=core;
                //mapMuestreoC[core][tiempo].tiempocore=tiempo;
                mapMuestreoC[core][tiempo].NthreadCore++; 
                mapMuestreoC[core][tiempo].tiempoTotal=tiempoTotal;
                mapMuestreoC[core][tiempo].tiempoActivo=tiempoActivo;
                mapMuestreoC[core][tiempo].utilizacion=mapMuestreoC[core][tiempo].utilizacionAcum+utilizacion;
                //mapMuestreoC[core][tiempo].tiempoInactivo=mapMuestreoC[core][tiempo].tiempoTotalInactivo+dato.tiempoInactivo;
                
                mapMuestreoC[core][tiempo].tiempoInactivo=dato.tiempoInactivo;
                mapMuestreoC[core][tiempo].utilizacionAcum=dato.utilizacion;

            }
        }
        else
        {
            //cout<<"Voy por el 1° else"<<core<<endl;   
            double tiempoTotal = dato.tiempoTotal;
            double tiempoActivo = dato.tiempoActivo;
            double utilizacion = tiempoActivo/tiempoTotal*100;
           
            // mapMuestreoC[core][tiempo].ncore=core;
            //mapMuestreoC[core][tiempo].tiempocore=tiempo;
            
            mapMuestreoC[core][tiempo].NthreadCore==1;
            mapMuestreoC[core][tiempo].tiempoTotal=tiempoTotal;
            mapMuestreoC[core][tiempo].tiempoActivo=tiempoActivo;
            mapMuestreoC[core][tiempo].utilizacion=utilizacion;
            
            //mapMuestreoC[core][tiempo].tiempoInactivo=mapMuestreoC[core][tiempo].tiempoTotalInactivo+dato.tiempoInactivo;
            
            mapMuestreoC[core][tiempo].tiempoInactivo=dato.tiempoInactivo;
            mapMuestreoC[core][tiempo].utilizacionAcum=dato.utilizacion;

        }
        //mapMuestreoC[dato.pid\4][dato.tiempothread].push_back()     
       
      }
       //printUtilizacionCore();
      guardarIntervalosUtilizacionCore();
      graficarCore();
      graficarPromUCore();
    //return salida; 
  }
  void printUtilizacionCore(){
    int core=0;
    for (map< int,map < double, dataC> >::iterator i = mapMuestreoC.begin(); i != mapMuestreoC.end(); ++i)
    {
      for (map<double, dataC>::iterator j = i->second.begin(); j != i->second.end(); ++j)
      {
        dataC dato=j->second;
        cout<<"core : "<<i->first<<", tiempo: "<<j->first<<", threads: "<<dato.NthreadCore<<endl;
        //cout<<"Tiempo Total"<<dato.tiempoTotal<<", tiempo activo : "<<dato.tiempoActivo<<endl;  
      }
    }
  }
  void guardarIntervalosUtilizacionCore(){
    for (map< int,map < double, dataC> >::iterator i = mapMuestreoC.begin(); i != mapMuestreoC.end(); ++i)
    {
      std::string name= "out/Utilizacion_Core-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i->first))->str()+".out" ;
      ofstream out;
      //const char *namec=name.c_str();
      char * namec = new char [name.length()+1];
      strcpy (namec, name.c_str());
      out.open(namec);
      
      name= "out/PromUtilizacion_Core_-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i->first))->str()+".out" ;
      ofstream outp;
      //const char *namec=name.c_str();
      namec = new char [name.length()+1];
      strcpy (namec, name.c_str());
      outp.open(namec);
      
      for (map<double, dataC>::iterator j = i->second.begin(); j != i->second.end(); ++j)
      {
        dataC dato=j->second;
        cout<<"core : "<<i->first<<", tiempo: "<<j->first<<", threads: "<<dato.NthreadCore<<endl;
        
        out<< j->first / 1e6 << " , " << dato.utilizacion << endl;
        outp<< j->first / 1e6 << " , " << dato.utilizacionAcum / dato.NthreadCore << endl;
        //cout<<"Tiempo Total"<<dato.tiempoTotal<<", tiempo activo : "<<dato.tiempoActivo<<endl;  
      }
      out.close();
      outp.close();
    }
  }
  void graficarCore(){
   char * configGnuplot[] = {  "set term png",
                                  "set title \"Utilización vs Tiempo por core\"", 
                                  "set ylabel \"----Utilización--->\"",
                                  //"set format y\"%2.f\"",
                                  "set xlabel \"----Tiempo--->\"",
                                  //"set multiplot",
                                  //"set size 1,0.5"
                                  "set yrange[80:100]",
                                  "set grid",
                                  "show grid"

                                  //instrucionc
                                 };
      FILE * ventanaGnuplot = popen ("gnuplot -persist", "w");
      for (int k=0;k<5;k++){
        fprintf(ventanaGnuplot, "%s \n", configGnuplot[k]);
      }

      for (int i = 0; i < Ncores; ++i)
      {
        string nameG="Utilizacion_Core-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
        string instrucionP = "plot \"out/"+ nameG+".out\" with lines";
        string instrucionG = "set out \"graf/Grafico_"+nameG+".png\"";
        char  * instrucionPc = new char [instrucionP.length()+1];
        strcpy (instrucionPc, instrucionP.c_str());
        char *  instrucionGc = new char [instrucionG.length()+1];
        strcpy (instrucionGc, instrucionG.c_str());

        fprintf(ventanaGnuplot, "%s \n", instrucionGc);
        fprintf(ventanaGnuplot, "%s \n", instrucionPc);
      
      }
      fprintf(ventanaGnuplot, "%s \n", "exit");  
  }
  void graficarPromUCore(){
   char * configGnuplot[] = {  "set term png",
                                  "set title \"Utilización vs Tiempo por core\"", 
                                  "set ylabel \"----Utilización--->\"",
                                  //"set format y\"%2.f\"",
                                  "set xlabel \"----Tiempo--->\"",
                                  //"set multiplot",
                                  //"set size 1,0.5"
                                  "set yrange[80:100]",
                                  "set grid",
                                  "show grid"

                                  //instrucionc
                                 };
      FILE * ventanaGnuplot = popen ("gnuplot -persist", "w");
      for (int k=0;k<5;k++){
        fprintf(ventanaGnuplot, "%s \n", configGnuplot[k]);
      }

      for (int i = 0; i < Ncores; ++i)
      {
        string nameG="PromUtilizacion_Core-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
        string instrucionP = "plot \"out/"+ nameG+".out\" with lines";
        string instrucionG = "set out \"graf/Grafico_"+nameG+".png\"";
        char  * instrucionPc = new char [instrucionP.length()+1];
        strcpy (instrucionPc, instrucionP.c_str());
        char *  instrucionGc = new char [instrucionG.length()+1];
        strcpy (instrucionGc, instrucionG.c_str());

        fprintf(ventanaGnuplot, "%s \n", instrucionGc);
        fprintf(ventanaGnuplot, "%s \n", instrucionPc);
      
      }
      fprintf(ventanaGnuplot, "%s \n", "exit");  
  }

  void muestraFallas()
  {
    double suma=0, max=0;
    printf("-------------------------------------------\n");
    printf("Total_L1<-L2=  ");
    for(int i=0;i<NT;i++)
    {
       //printf(" %.0lf", fallasCacheL1L2[i]);
       suma += fallasCacheL1L2[i];
       if (max<fallasCacheL1L2[i]) max= fallasCacheL1L2[i];
    }
    printf(" %.0lf    Ef= %.2lf\n",suma, (suma/NT)/max);
    suma=0.0, max=0.0;
    
    for(int i=0;i<nchips;i++)
    {
      double sumaXcore=0.0;
      //cout<<"sumaXcore: "<<sumaXcore<<endl;
      //cout<<"ncore: "<<Ncores<<endl;
      for (int j = 0; j < Ncores; ++j)
      {
        //cout<<"sumaXcore: "<<sumaXcore<<endl;
        sumaXcore=sumaXcore+acumuladoresTiempoL2L1[i][j];
        printf("Total_Time_L1<-L2 de chip %i, core %i: %6lf\n",i,j,acumuladoresTiempoL2L1[i][j]);
      }
      printf("Total_Time_L1<-L2 de chip %i: %6lf\n",i,sumaXcore);
      if (sumaXcore>max) max=sumaXcore;
      suma=suma+sumaXcore;

    }
      printf("Total_Time_L1<-L2=   %.6lf    max= %.6lf\n",suma,max);
    suma=0.0; max=0.0;
    printf("-------------------------------------------\n");
    printf("Total_L2<-Ram= ");
    for(int i=0;i<NT;i++)
    {
       //printf(" %.0lf", fallasCacheL2Ram[i]);
       suma += fallasCacheL2Ram[i];
       if (max<fallasCacheL2Ram[i]) max= fallasCacheL2Ram[i];
    }
    printf(" %.0lf    Ef= %.2lf\n",suma, (suma/NT)/max);
    
    max=0,suma=0.0;
    for(int i=0;i<nchips;i++)
    {
      suma=suma+acumuladoresTiempoRamL2[i]/1e6;
      if (acumuladoresTiempoRamL2[i]/1e6>max) max=acumuladoresTiempoRamL2[i]/1e6;
      printf("Total_Time_L2<-RAM de chip %i: %6lf\n",i,acumuladoresTiempoRamL2[i]/1e6);
    }
      printf("Total_Time_L2<-Ram=   %.6lf    max= %.6lf\n",suma,max);
      printf("-------------------------------------------\n");
    
    printf("Total_HitsL1=  ");
    for(int i=0;i<NT;i++)
    {
       //printf(" %.0lf", hitL1[i]);
       suma += hitL1[i];
       if (max<hitL1[i]) max= hitL1[i];
    }
    printf(" %.0lf    Ef= %.2lf\n",suma, (suma/NT)/max);

    suma=0; max=0;
    printf("Total_HitsL2=   ");
    for(int i=0;i<NT;i++)
    {
       //printf(" %.0lf", hitL2[i]);
       suma += hitL2[i];
       if (max<hitL2[i]) max= hitL2[i];
    }
    printf(" %.0lf    Ef= %.2lf\n",suma, (suma/NT)/max);

    suma=0; max=0;
    printf("Total_HitsRam= ");
    for(int i=0;i<NT;i++)
    {
       //printf(" %.0lf", hitRam[i]);
       suma += hitRam[i];
       if (max<hitRam[i]) max= hitRam[i];
    }
    printf(" %.0lf    Ef= %.2lf\n",suma, (suma/NT)/max);
    //guardarIntervalosUtilizacion();
    //graficar();
    UtilizacionCore();

  }
  /*
    Iniciualiza los acumuladores de tiempo para  transferir informacion de  Ram a L2
  */

  void iniciarAcumuladorTiempoRamL2(int nchip)
  {
      nchips=nchip;
      acumuladoresTiempoRamL2=new double[nchip];
      for (int i = 0; i < nchip; ++i)
      {
        acumuladoresTiempoRamL2[i]=0.0;
      }
      //cout<<"INICIADO!!!!! "<<Nchip<<endl;
  }
  /* Suma los valores de tiempo de tranferencia en cada chip
  */
  void sumarTiemposRamL2(int cpid, double tiempo){

    acumuladoresTiempoRamL2[ cpid ] = acumuladoresTiempoRamL2[cpid]+tiempo;
    //cout<<"Sume :"<< tiempo << " al chip "<< cpid << ", Nuevo valor :" << acumuladoresTiempoRamL2 [ cpid]/1e6<<endl;

  }
  void iniciarAcumuladorTiempoL2L1(int nchip,int ncore)
  {
      nchips=nchip;
      Ncores=ncore;

      
      acumuladoresTiempoL2L1=(double **)malloc(nchip *sizeof(double * ));
      for (int i = 0; i < nchip; ++i)
      {
        acumuladoresTiempoL2L1[i]= (double *)malloc(ncore*sizeof(double));
        for (int j = 0; j < ncore; ++j)
        {
          acumuladoresTiempoL2L1[i][j]=0.0;
        }
      }
      //cout<<"INICIADO!!!!! "<<Nchip<<endl;
  }
  /* Suma los valores de tiempo de tranferencia en cada chip
  */
  void sumarTiemposL2L1(int cpid,int id_core, double tiempo){
    //cout<<"SUMO AL L1<-L2 "<<tiempo<<endl;
    acumuladoresTiempoL2L1[ cpid ] [ id_core ]= acumuladoresTiempoL2L1[cpid][id_core]+tiempo/1e6;
    //cout<<"Sume :"<< tiempo << " al chip "<< cpid <<" core "<<id_core<< ", Nuevo valor :" << acumuladoresTiempoL2L1 [ cpid][id_core]<<endl;

  }

 
  void mide( int pid, double reloj, double retardo, int modo )
  {
    tiempoXThread[pid][modo]=retardo+tiempoXThread[pid][modo];
    tiempoXThread[pid][TOTAL]=retardo+tiempoXThread[pid][TOTAL];
    
    //if(modo==ACTIVE){
      
                
        //cout<<"Porcentaje INACTIVO de la thread "<<(tiempoXThread[pid][INACTIVE]/(tiempoXThread[pid][ACTIVE]+tiempoXThread[pid][INACTIVE]))*100<<"%"<<endl;
        if (tiempoAcumulado[pid]<=tiempoXThread[pid][TOTAL])
        {
          //cout<<"CUMPLIDO"<<endl;
          double diferencia=tiempoXThread[pid][TOTAL]-tiempoAcumulado[pid];
          dataT datos;
          datos.pid=pid;
          double tiempoTotal= (tiempoXThread[pid][ACTIVE] + tiempoXThread[pid][INACTIVE] - diferencia);
          if (tiempoAcumulado[pid]==0) datos.utilizacion=0.0;
          else if(diferencia>=0 && modo==ACTIVE) {
            datos.tiempoActivo=tiempoXThread[pid][ACTIVE] - diferencia;
            datos.tiempoInactivo=tiempoXThread[pid][INACTIVE];
            datos.utilizacion= ((tiempoXThread[pid][ACTIVE] - diferencia) /tiempoTotal )*100;
            
          } 
          else {
            datos.tiempoActivo=tiempoXThread[pid][ACTIVE];
            datos.tiempoInactivo=tiempoXThread[pid][INACTIVE]-diferencia;
            datos.utilizacion= (tiempoXThread[pid][ACTIVE] /tiempoTotal )*100;
          }
          datos.tiempoTotal=tiempoTotal;
          datos.tiempothread= tiempoXThread[pid][TOTAL]- diferencia;
        
          //cout<<"El pid: "<<datos.pid<<" tiempo thread:"<<datos.tiempothread<<", tiempo acumulado thread: "<< tiempoAcumulado[pid]<<endl;
          //cout<<"Porcentaje de Utilización de la thread "<<datos.utilizacion<<endl;

          vectorMuestreoT.push_back(datos);

          
          tiempoXThread[pid][modo]=diferencia;
          if(modo==ACTIVE){
            tiempoXThread[pid][INACTIVE]=0;
          }
          else
          {
            tiempoXThread[pid][ACTIVE]=0;            
          }
          tiempoAcumulado[pid]=tiempoAcumulado[pid]+deltaTiempo;
        }  
   //   }
      //else
        /*if (tiempoAcumulado[pid]<=tiempoXThread[pid][TOTAL])
          {
            cout<< "INACTIVE"<<endl;
          }
          */
     
     /*
     double avg=0.0, mx=0.0, delta;
     
    printf("Esto no debería ejecutarse\n");exit(0);     

    for(int i=0; i<NT; i++)
     {
        
        if (minimo[i]>=reloj) continue;

        if (minimo[i]==reloj && maximo[i]==reloj)
        {  minimo[i]=maximo[i]=1000e100; continue; }


        if (minimo[i]<reloj && maximo[i]>=reloj )
        {
           delta = reloj-minimo[i];
           minimo[i]= reloj;

          if (maximo[i]==reloj)
          {  minimo[i]=maximo[i]=1000e100;  }
        }

        else if (minimo[i]<reloj && maximo[i]<reloj)
        {
           delta = maximo[i]-minimo[i];
           minimo[i]=maximo[i]=1000e100;
        }

        else
        {  printf("ERROR mide()\n"); exit(0); }


        avg += delta;
        if ( mx < delta) mx= delta;

     } // end-for

     if (mx!=0.0)
       eficiencia +=  ( avg/NT ) / mx;

     numEf++;

     minimo[pid] = reloj;
     maximo[pid] = reloj+retardo;

     avg = 0.0;
     mx  = 0.0;
    */
  }

  double Eficiencia()
  {  if (numEf==0) return -1.0; else return eficiencia/numEf;  }

};

#endif




