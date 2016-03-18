#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H
#include "glob.h"

class Estadisticas
{
private:

  int NT; // numero de threads

  double *minimo;
  double *maximo;
  double eficiencia, numEf;

  double *fallasCacheL1L2;
  double *fallasCacheL2Ram;

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
    printf("Total_L1<-L2=  ");
    for(int i=0;i<NT;i++)
    {
       //printf(" %.0lf", fallasCacheL1L2[i]);
       suma += fallasCacheL1L2[i];
       if (max<fallasCacheL1L2[i]) max= fallasCacheL1L2[i];
    }
    printf(" %.0lf    Ef= %.2lf\n",suma, (suma/NT)/max);

    suma=0; max=0;
    printf("Total_L2<-Ram= ");
    for(int i=0;i<NT;i++)
    {
       //printf(" %.0lf", fallasCacheL2Ram[i]);
       suma += fallasCacheL2Ram[i];
       if (max<fallasCacheL2Ram[i]) max= fallasCacheL2Ram[i];
    }
    printf(" %.0lf    Ef= %.2lf\n",suma, (suma/NT)/max);

    suma=0; max=0;
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




