#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H
#include "glob.h"
#include "estruc.h"
#include "estadisticas/dataThread.h"
#include "estadisticas/dataChip.h"
#include "estadisticas/dataCache.h"
//#include "dataChip.h"

class Estadisticas
{
private:

  int NT; // numero de threads
  int nchips;
  int  Ncores;
  int *pasadas;
  double *minimo;
  double *maximo;
  double eficiencia, numEf;
  
  double *fallasCacheL1L2;
  double *fallasCacheL2Ram;
  double **acumuladoresTiempoRamL2;
  double **acumuladoresTiempoL2L1;
  double *hitL1, *hitL2, *hitRam;
  double **tiempoXThread;

  double deltaTiempo;
  double *tiempoAcumulado;
  vector<dataThread> vectorMuestreoT;
  map<int, map<double,dataChip > > mapMuestreoChip;
  map<int, map< int, map< double , dataCache > > > mapMuestreoCacheL1;
  map<int, map< double , dataCache  > > mapMuestreoCacheL2;
  //map<int, value> mapMuestreoCache;

public:

  Estadisticas(int nt,int delta){
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
       tiempoAcumulado[i]=delta; 
     }
      
      deltaTiempo=delta;
      
      
  }


  void hit_Ram( int pid ) { hitRam[pid]++; }
  void hit_L1( int pid ) { hitL1[pid]++; }
  void hit_L2( int pid ) { hitL2[pid]++; }

  void fallaL1L2( int pid ) { fallasCacheL1L2[pid]++; }
  void fallaL2Ram( int pid ) { fallasCacheL2Ram[pid]++;  }
  void graficar(){
   const char * configGnuplot[] = {  "set term png",
                                "set title \"Utilización vs Tiempo por thread\"", 
                                "set ylabel \"----Utilización--->\"",
                                //"set format y\"%2.f\"",
                                "set xlabel \"----Tiempo--->\"",
                                //"set multiplot",
                                //"set size 1,0.5"
                                "set autoscale",
                                "set grid",
                                "show grid"
                                 };
    FILE * ventanaGnuplot = popen ("gnuplot -persist", "w");
    for (int k=0;k<5;k++){
      fprintf(ventanaGnuplot, "%s \n", configGnuplot[k]);
    }

    for (int i = 0; i < NT; ++i)
    {
      string nameG="Utilizacion_Thread-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
      string instrucionP = "plot \"output/out/"+ nameG+".out\" with lines";
      string instrucionG = "set out \"output/graf/Grafico_"+nameG+".png\"";
      char  * instrucionPc = new char [instrucionP.length()+1];
      strcpy (instrucionPc, instrucionP.c_str());
      char *  instrucionGc = new char [instrucionG.length()+1];
      strcpy (instrucionGc, instrucionG.c_str());

      fprintf(ventanaGnuplot, "%s \n", instrucionGc);
      fprintf(ventanaGnuplot, "%s \n", instrucionPc);
    
    }
    fprintf(ventanaGnuplot, "%s \n", "exit");  
  }
  void graficarCacheL1(){
   const char * configGnuplot[] = {  "set term png",
                                "set title \"Utilización vs Tiempo por CacheL1\"", 
                                "set ylabel \"----Utilización--->\"",
                                //"set format y\"%2.f\"",
                                "set xlabel \"----Tiempo--->\"",
                                //"set multiplot",
                                //"set size 1,0.5"
                                "set autoscale",
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
      string nameG="Utilizacion_CacheL1_Chip-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i/4))->str()+"_Core-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i%4))->str();
      string instrucionP = "plot \"output/out/"+ nameG+".out\" with lines";
      string instrucionG = "set out \"output/graf/Grafico_"+nameG+".png\"";
      char  * instrucionPc = new char [instrucionP.length()+1];
      strcpy (instrucionPc, instrucionP.c_str());
      char *  instrucionGc = new char [instrucionG.length()+1];
      strcpy (instrucionGc, instrucionG.c_str());

      fprintf(ventanaGnuplot, "%s \n", instrucionGc);
      fprintf(ventanaGnuplot, "%s \n", instrucionPc);
    
    }
    fprintf(ventanaGnuplot, "%s \n", "exit");  
  }
  void graficarCacheL2(){
    const char * configGnuplot[] = {  "set term png",
                                 "set title \"Utilización CacheL2\"", 
                                 "set ylabel \"----Utilización--->\"",
                                 //"set format y\"%2.f\"",
                                 "set xlabel \"----Tiempo--->\"",
                                 //"set multiplot",
                                 //"set size 1,0.5"
                                 "set autoscale",
                                 "set grid",
                                 "show grid"
 
                                 //instrucionc
                                };
     FILE * ventanaGnuplot = popen ("gnuplot -persist", "w");
     for (int k=0;k<5;k++){
       fprintf(ventanaGnuplot, "%s \n", configGnuplot[k]);
     }
 
     for (int i = 0; i < nchips; ++i)
     {
       string nameG="Utilizacion_CacheL2_Chip-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
       string instrucionP = "plot \"output/out/"+ nameG+".out\" with lines";
       string instrucionG = "set out \"output/graf/Grafico_"+nameG+".png\"";
       char  * instrucionPc = new char [instrucionP.length()+1];
       strcpy (instrucionPc, instrucionP.c_str());
       char *  instrucionGc = new char [instrucionG.length()+1];
       strcpy (instrucionGc, instrucionG.c_str());
 
       fprintf(ventanaGnuplot, "%s \n", instrucionGc);
       fprintf(ventanaGnuplot, "%s \n", instrucionPc);
     
     }
     fprintf(ventanaGnuplot, "%s \n", "exit");  
   }
  void guardarIntervalosUtilizacion(){
    
     
    for (int j = 0; j < NT; ++j )
    {
      std::string name= "output/out/Utilizacion_Thread-"+static_cast<std::ostringstream*>(&(std::ostringstream() << j))->str()+".out" ;
      ofstream out;
      //const char *namec=name.c_str();
      char * namec = new char [name.length()+1];
      strcpy (namec, name.c_str());

      //namec = (char *)alloca(name.size() + 1);
    //memcpy(namec, name.c_str(), name.size() + 1);
      out.open(namec);
      //cout<<"-------------------------------------------------------------"<<endl;
      //cout<<"GRAFICAR"<<endl;
      for (int i = 0; (unsigned)i < vectorMuestreoT.size() ; ++i)
      {   
         
         if (vectorMuestreoT[i].getPid()==j && vectorMuestreoT[i].getTiempoThread()!=0)
         {
           //cout<<"El pid: "<<vectorMuestreoT[i].pid<<" tiempo thread:"<<vectorMuestreoT[i].tiempothread<<endl;
            //cout<<"Porcentaje de Utilización de la thread "<<vectorMuestreoT[i].utilizacion<<endl;
            out<< vectorMuestreoT[i].getTiempoThread()/1e6<<", "<<vectorMuestreoT[i].getUtilizacion()<<endl;
         }
       
      }

       out.close();
    }

  }
void guardarIntervalosUtilizacionCacheL1(){
    //int primero=0;

    for (map< int, map < int, map<double, dataCache> > >::iterator i = mapMuestreoCacheL1.begin(); i != mapMuestreoCacheL1.end(); ++i)
    {
      
      for (map<int ,map<double, dataCache> >::iterator j = i->second.begin(); j != i->second.end(); ++j)
      {

        std::string name= "output/out/Utilizacion_CacheL1_Chip-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i->first))->str()+"_Core-"+static_cast<std::ostringstream*>(&(std::ostringstream() << j->first))->str()+".out" ;
        ofstream out;
        //const char *namec=name.c_str();
        char * namec = new char [name.length()+1];
        strcpy (namec, name.c_str());
        out.open(namec);

        for (map<double, dataCache>::iterator k = j->second.begin(); k != j->second.end(); ++k)
        {

          dataCache dato=k->second;
          //cout<<"HOLAAAA"<<endl;
          //cout<<"core : "<<i->first<<", tiempo: "<<j->first<<", threads: "<<dato.NthreadCore<<endl;
          if (k->first!=0.0)
          {
            //cout<<"OJO "<<dato.utilizacionAcum<<", "<<dato.NthreadCore<<endl;
            out<< k->first / 1e6 << ", " << dato.getUtilizacion() << endl;

          //outp<< j->first / 1e6 << ", " << dato.utilizacionAcum / double(dato.NthreadChip) << endl;
          }

        }

      out.close();
        //cout<<"Tiempo Total"<<dato.tiempoTotal<<", tiempo activo : "<<dato.tiempoActivo<<endl;  
      //primero++;
      }
      
      //outp.close();
      
    }
  

 }
  int PromUtilizacionCacheL1(int cpid, double tiempothread){
/*      for (map<int ,map<double, dataCache> >::iterator i  = mapMuestreoCacheL1[cpid].begin(); i != mapMuestreoCacheL1[cpid].end(); ++i)
      {
           dataCache dato=i->second[tiempothread];


      }*/
  }

  void UtilizacionChip(){
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
    
    //cout<<"___________________________ENTRE____________________________"<<endl;
      for (int i = 0; (unsigned)i < vectorMuestreoT.size() ; ++i)
      {   
        dataThread dato=vectorMuestreoT[i];
        //if (dato.tiempothread!=0)
        //{
          //dataChip datoc;
          int chip=dato.pid/4;
          double tiempo=dato.getTiempoThread();
          if (mapMuestreoChip.find(chip)!=mapMuestreoChip.end())
          {
            //if(dato.tiempothread==0) cout<<"Voy por el 1° if "<<chip<<endl;
            if(mapMuestreoChip[chip].find(tiempo)==mapMuestreoChip[chip].end())
            {
              //if(dato.tiempothread==0)cout<<"Voy por el 2° if "<<chip<<endl;
               
                
                mapMuestreoChip[chip][tiempo].setUtilizacion(dato);
            }
            else
              {
                //if(dato.tiempothread==0) cout<<"Voy por el 2° else "<<chip<<endl;
                  dataChip *dataC=new dataChip(dato);
                  mapMuestreoChip[chip][tiempo]= *dataC;


              }
          }
          else
          {
                 
              dataChip *dataC=new dataChip(dato);
              mapMuestreoChip[chip][tiempo]= *dataC;
          }
          //mapMuestreoC[dato.pid\4][dato.tiempothread].push_back()     
       //}  
      }
       //printUtilizacionCore();
      guardarIntervalosUtilizacionChip();
      graficarChip();
      graficarPromUChip();
    //return salida; 
  }

  void printUtilizacionChip(){

    for (map< int,map < double, dataChip> >::iterator i = mapMuestreoChip.begin(); i != mapMuestreoChip.end(); ++i)
    {
      for (map<double, dataChip>::iterator j = i->second.begin(); j != i->second.end(); ++j)
      {
        dataChip dato=j->second;
        cout<<"chip : "<<i->first<<", tiempo: "<<j->first<<", threads: "<<dato.getNThreadChip()<<endl;
        //cout<<"Tiempo Total"<<dato.tiempoTotal<<", tiempo activo : "<<dato.tiempoActivo<<endl;  
      }
    }
  }
  void guardarIntervalosUtilizacionChip(){
    int primero=0;
    for (map< int,map < double, dataChip> >::iterator i = mapMuestreoChip.begin(); i != mapMuestreoChip.end(); ++i)
    {
      std::string name= "output/out/Utilizacion_Chip-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i->first))->str()+".out" ;
      ofstream out;
      //const char *namec=name.c_str();
      char * namec = new char [name.length()+1];
      strcpy (namec, name.c_str());
      out.open(namec);
      
      name= "output/out/PromUtilizacion_Chip_-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i->first))->str()+".out" ;
      ofstream outp;
      //const char *namec=name.c_str();
      namec = new char [name.length()+1];
      strcpy (namec, name.c_str());
      outp.open(namec);
      
      for (map<double, dataChip>::iterator j = i->second.begin(); j != i->second.end(); ++j)
      {
        dataChip dato=j->second;
        //cout<<"core : "<<i->first<<", tiempo: "<<j->first<<", threads: "<<dato.NthreadCore<<endl;
        if (j->first!=0)
        {
          //cout<<"OJO "<<dato.utilizacionAcum<<", "<<dato.NthreadCore<<endl;
          out<< j->first / 1e6 << ", " << dato.getUtilizacion() << endl;

          outp<< j->first / 1e6 << ", " << dato.getUtilizacionAcum() / double(dato.getNThreadChip()) << endl;
        }
        
        
        //cout<<"Tiempo Total"<<dato.tiempoTotal<<", tiempo activo : "<<dato.tiempoActivo<<endl;  
      primero++;
      }
      out.close();
      outp.close();
      
    }
  }
  void graficarChip(){
   const char * configGnuplot[] = {  "set term png",
                                  "set title \"Utilización vs Tiempo por chip\"", 
                                  "set ylabel \"----Utilización--->\"",
                                  //"set format y\"%2.f\"",
                                  "set xlabel \"----Tiempo--->\"",
                                  //"set multiplot",
                                  //"set size 1,0.5"
                                  "set autoscale",
                                  "set grid",
                                  "show grid"

                                  //instrucionc
                                 };
      FILE * ventanaGnuplot = popen ("gnuplot -persist", "w");
      for (int k=0;k<5;k++){
        fprintf(ventanaGnuplot, "%s \n", configGnuplot[k]);
      }

      for (int i = 0; i < nchips; ++i)
      {
        string nameG="Utilizacion_Chip-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
        string instrucionP = "plot \"output/out/"+ nameG+".out\" with lines";
        string instrucionG = "set out \"output/graf/Grafico_"+nameG+".png\"";
        char  * instrucionPc = new char [instrucionP.length()+1];
        strcpy (instrucionPc, instrucionP.c_str());
        char *  instrucionGc = new char [instrucionG.length()+1];
        strcpy (instrucionGc, instrucionG.c_str());

        fprintf(ventanaGnuplot, "%s \n", instrucionGc);
        fprintf(ventanaGnuplot, "%s \n", instrucionPc);
      
      }
      fprintf(ventanaGnuplot, "%s \n", "exit");  
  }
  void graficarPromUChip(){
   const char * configGnuplot[] = {  "set term png",
                                  "set title \"Utilización vs Tiempo por chip\"", 
                                  "set ylabel \"----Utilización--->\"",
                                  //"set format y\"%2.f\"",
                                  "set xlabel \"----Tiempo--->\"",
                                  //"set multiplot",
                                  //"set size 1,0.5"
                                  "set autoscale",
                                  "set grid",
                                  "show grid"

                                  //instrucionc
                                 };
      FILE * ventanaGnuplot = popen ("gnuplot -persist", "w");
      for (int k=0;k<5;k++){
        fprintf(ventanaGnuplot, "%s \n", configGnuplot[k]);
      }

      for (int i = 0; i < nchips; ++i)
      {
        string nameG="PromUtilizacion_Chip_-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
        string instrucionP = "plot \"output/out/"+ nameG+".out\" with lines";
        string instrucionG = "set out \"output/graf/Grafico_"+nameG+".png\"";
        char  * instrucionPc = new char [instrucionP.length()+1];
        strcpy (instrucionPc, instrucionP.c_str());
        char *  instrucionGc = new char [instrucionG.length()+1];
        strcpy (instrucionGc, instrucionG.c_str());

        fprintf(ventanaGnuplot, "%s \n", instrucionGc);
        fprintf(ventanaGnuplot, "%s \n", instrucionPc);
      
      }
      fprintf(ventanaGnuplot, "%s \n", "exit");  
  }

  void muestraFallas(){
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
        printf("Total_Time_L1<-L2 de chip %i, core %i: %lf\n",i,j,acumuladoresTiempoL2L1[i][j]);
      }
      printf("Total_Time_L1<-L2 de chip %i: %lf\n",i,sumaXcore);
      if (sumaXcore>max) max=sumaXcore;
      suma=suma+sumaXcore;

    }
      printf("Total_Time_L1<-L2=   %lf    max= %lf\n",suma,max);
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
    /*
    max=0,suma=0.0;
    for(int i=0;i<nchips;i++)
    {
      suma=suma+acumuladoresTiempoRamL2[i]/1e6;
      if (acumuladoresTiempoRamL2[i]/1e6>max) max=acumuladoresTiempoRamL2[i]/1e6;
      printf("Total_Time_L2<-RAM de chip %i: %6lf\n",i,acumuladoresTiempoRamL2[i]/1e6);
    }
      printf("Total_Time_L2<-Ram=   %.6lf    max= %.6lf\n",suma,max);
      printf("-------------------------------------------\n");
    */
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
    guardarIntervalosUtilizacion();
    guardarIntervalosUtilizacionCacheL1();
    guardarIntervalosUtilizacionCacheL2();
    graficar();
    UtilizacionChip();
    graficarCacheL1();
    graficarCacheL2();
  
  }
  /*
    Iniciualiza los acumuladores de tiempo para  transferir informacion de  Ram a L2
  */

  void iniciarAcumuladorTiempoRamL2(int nchip,int ncore)
  {
      nchips=nchip;
      Ncores=ncore;
      pasadas=new int[nchips];
      acumuladoresTiempoRamL2=(double **)malloc(nchip*sizeof(double *));
      for (int i = 0; i < nchip; ++i)
      {        
        
        acumuladoresTiempoRamL2[i]=(double *)malloc(4*sizeof(double));
        for (int j = 0; j < Ncores; ++j)
        {
          acumuladoresTiempoRamL2[i][j]=0;
        }
        pasadas[i]=0;

      }

      //cout<<"INICIADO!!!!! "<<Nchip<<endl;
  }
  /* Suma los valores de tiempo de tranferencia en cada chip
  */
  void sumarTiemposRamL2(int pid, double tiempo){

    acumuladoresTiempoRamL2[ pid/4 ][pid%4] = acumuladoresTiempoRamL2[pid/4][pid%4]+tiempo;
    //cout<<"Sume :"<< tiempo << " al chip "<< pid/4 <<", core: "<<pid%4<< ", Nuevo valor :" << acumuladoresTiempoRamL2[pid/4][pid%4]<<endl;

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
  void sumarTiemposL2L1(int cpid,int id_core, int pid, double tiempo){
    //cout<<"SUMO AL L1<-L2 "<<tiempo<<endl;
    
    acumuladoresTiempoL2L1[ cpid ] [ id_core ]= acumuladoresTiempoL2L1[cpid][id_core]+tiempo;
    //if (tiempoXThread>deltaTiempo) cout<<tiempoXThread[pid]<<"Sume :"<< tiempo << " al chip "<< cpid <<" core "<<id_core<< ", Nuevo valor :" << acumuladoresTiempoL2L1 [ cpid][id_core]<<endl;
    //count++;
    //dataCache cache;

  }

 void mideCacheL1(int cpid,int id_core,double diferencia,double tiempo_total, double tiempothread){
      
      double tiempoActivo=acumuladoresTiempoL2L1[cpid][id_core]-diferencia;
      dataCache *dataC = new dataCache(tiempoActivo,tiempo_total);
      mapMuestreoCacheL1[cpid][id_core][tiempothread]=*dataC;
      acumuladoresTiempoL2L1[cpid][id_core]=diferencia; 
      
 }
 void resetearAcumulador(int pid, double diferencia){
    for (int i = 0; i < 4; ++i)
    {
      if (i==0) acumuladoresTiempoRamL2[pid/4][i]=diferencia;
      else 
        acumuladoresTiempoRamL2[pid/4][i]=0;
    }

 }
 void mideCacheL2(int pid,double diferencia,double tiempo_total, double tiempothread){
      //
        //if(diferencia>0){
        pasadas[pid/4]++;
        
        cout<<"___________________________MIDE CACHE L2_________________________________"<<endl;
        cout<<"tiempo: "<<tiempothread<<", diferencia: "<<diferencia<<", pid: "<<pid<<endl;
        cout<<pid/4<<" , "<<pasadas[pid/4]<<endl;
        if(pasadas[pid/4]==Ncores){
          double tiempo_Total_chip=0.0;
          for (int i = 0; i < Ncores; ++i)
          {
            tiempo_Total_chip=tiempo_Total_chip+acumuladoresTiempoRamL2[pid/4][i];
          }
          cout<<"tiempo Acumulado chip: "<<tiempo_Total_chip<<endl;  
          //}
          double tiempoActivo=tiempo_Total_chip-diferencia;
          
          
          //acumuladoresTiempoRamL2[pid/4][0]=diferencia;
          
          dataCache *dataC= new dataCache(tiempoActivo,tiempo_total);
          mapMuestreoCacheL2[pid/4][tiempothread]=*dataC;
          //cout<<"Utilización: "<<utilizacion<<endl;
          pasadas[pid/4]=0;
          resetearAcumulador(pid,diferencia);

        }
        cout<<"____________________________________________________________________"<<endl;
          //}
        
       // }
        //else acumuladoresTiempoRamL2[cpid]=diferencia;  */    

 }
 
 void guardarIntervalosUtilizacionCacheL2(){
    //int primero=0;

    
      
      for (map<int ,map<double, dataCache> >::iterator i = mapMuestreoCacheL2.begin(); i != mapMuestreoCacheL2.end(); ++i)
      {

        std::string name= "output/out/Utilizacion_CacheL2_Chip-"+static_cast<std::ostringstream*>(&(std::ostringstream() << i->first))->str()+".out" ;
        ofstream out;
        //const char *namec=name.c_str();
        char * namec = new char [name.length()+1];
        strcpy (namec, name.c_str());
        out.open(namec);

        for (map<double, dataCache>::iterator j = i->second.begin(); j != i->second.end(); ++j)
        {

          dataCache dato=j->second;
          //cout<<"HOLAAAA"<<endl;
          //cout<<"core : "<<i->first<<", tiempo: "<<j->first<<", threads: "<<dato.NthreadCore<<endl;
          if (j->first!=0)
          {
            //cout<<"OJO "<<dato.utilizacionAcum<<", "<<dato.NthreadCore<<endl;
            out<< j->first / 1e6 << ", " << dato.getUtilizacion() << endl;

          //outp<< j->first / 1e6 << ", " << dato.utilizacionAcum / double(dato.NthreadChip) << endl;
          }

        }

      out.close();
        //cout<<"Tiempo Total"<<dato.tiempoTotal<<", tiempo activo : "<<dato.tiempoActivo<<endl;  
      //primero++;
      }
      
      //outp.close();
      
    graficarCacheL2();
  

 }
  void mide( int pid,int cpid,int id_core , double reloj, double retardo, int modo, int dispositivo )
  {
    tiempoXThread[pid][modo]=retardo+tiempoXThread[pid][modo];
    tiempoXThread[pid][TOTAL]=retardo+tiempoXThread[pid][TOTAL];
    
    //if(modo==ACTIVE){
      
                
        //cout<<"Porcentaje INACTIVO de la thread "<<(tiempoXThread[pid][INACTIVE]/(tiempoXThread[pid][ACTIVE]+tiempoXThread[pid][INACTIVE]))*100<<"%"<<endl;
        if (tiempoAcumulado[pid]<=tiempoXThread[pid][TOTAL])
        {
          double diferencia=tiempoXThread[pid][TOTAL]-tiempoAcumulado[pid];
          //dataT datos;
          
          double tiempoActivo=0.0,tiempoInactivo=0.0,utilizacion=0.0;
          double tiempoTotal= (tiempoXThread[pid][ACTIVE] + tiempoXThread[pid][INACTIVE] - diferencia);
          if (tiempoAcumulado[pid]==0) utilizacion=0.0;
          else if(diferencia>=0 && modo==ACTIVE) {
            tiempoActivo=tiempoXThread[pid][ACTIVE] - diferencia;
            tiempoInactivo=tiempoXThread[pid][INACTIVE];
            utilizacion= ((tiempoXThread[pid][ACTIVE] - diferencia) /tiempoTotal )*100;

          } 
          else {
            tiempoActivo=tiempoXThread[pid][ACTIVE];
            tiempoInactivo=tiempoXThread[pid][INACTIVE]-diferencia;
            utilizacion= (tiempoXThread[pid][ACTIVE] /tiempoTotal )*100;
            
          }
          //datos.tiempoTotal=tiempoTotal;
          double tiempothread = tiempoXThread[pid][TOTAL]- diferencia;
          //datos.tiempothread= tiempoThread;
            dataThread *datos= new dataThread(pid,tiempothread,tiempoActivo, tiempoInactivo,tiempoTotal,utilizacion);
          //cout<<"El pid: "<<datos.pid<<" tiempo thread:"<<datos.tiempothread<<", tiempo acumulado thread: "<< tiempoAcumulado[pid]<<endl;
          //cout<<"Porcentaje de Utilización de la thread "<<datos.utilizacion<<endl;

          vectorMuestreoT.push_back(*datos);
          
          
          tiempoXThread[pid][modo]=diferencia;
          //cout<<"___________________________MIDE EN TIEMPO "<<tiempoThread<<", "<<tiempoTotal<<"___________________________"<<endl;
          if(modo==ACTIVE){
            //cout<<"Core"<<endl;
            mideCacheL1(cpid,id_core,0.0,tiempoTotal,tiempothread);
            mideCacheL2(pid,0.0,tiempoTotal,tiempothread);
            //mideCacheL1(cpid,id_core,0,tiempoTotal,tiempoThread);
            tiempoXThread[pid][INACTIVE]=0;
          }
          else
          {
            //cout<<"HOLAAAA"<<endl;
            if (dispositivo==CL1){ 
                //cout<<"tiempo: "<<tiempoThread/1e6<<" CacheL1: "<<retardo<<", pid: "<<pid<<endl;

                mideCacheL1(cpid,id_core,diferencia,tiempoTotal,tiempothread);
                mideCacheL2(pid,0.0,tiempoTotal,tiempothread);
              }
            else if(dispositivo==CL2){

                  //cout<<"tiempo: "<<tiempoThread/1e6<<" CacheL2: "<<retardo<<", pid: "<<pid<<", acumulador :"<<acumuladoresTiempoRamL2[cpid]<<endl;

                  mideCacheL1(cpid,id_core,0.0,tiempoTotal,tiempothread);
                  mideCacheL2(pid,diferencia,tiempoTotal,tiempothread); 
              }
              else{
                  mideCacheL1(cpid,id_core,0.0,tiempoTotal,tiempothread);
                  mideCacheL2(pid,0.0,tiempoTotal,tiempothread);
              }
            //cout<<"HOLAAAA"<<endl;
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




