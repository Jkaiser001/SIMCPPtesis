#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H
#include "glob.h"

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


public:

  Estadisticas(int nt)
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

     for(int i=0;i<nt;i++)
       hitL1[i]= hitL2[i]= hitRam[i]= 0;
  }


  void hit_Ram( int pid ) { hitRam[pid]++; }
  void hit_L1( int pid ) { hitL1[pid]++; }
  void hit_L2( int pid ) { hitL2[pid]++; }

  void fallaL1L2( int pid ) { fallasCacheL1L2[pid]++; }
  void fallaL2Ram( int pid ) { fallasCacheL2Ram[pid]++;  }

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

  void mide( int pid, double reloj, double retardo )
  {
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

  }

  double Eficiencia()
  {  if (numEf==0) return -1.0; else return eficiencia/numEf;  }

};

#endif




