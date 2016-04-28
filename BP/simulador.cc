/*
Ejecucion en etna

./simulador QT dimBloque NT ../archivos/ traza.dat

ESTRATEGIA: BP

Ejecucion:

./simulador 5000 256 4 /scratch/carolina/archivos/indiceOr/traza-0-0-8.dat

make;./simulador 200  256 8 /home2/cluster/carolina/datos-simulacion/indiceOr/ traza-0-0-8.dat

*/

#include <stdlib.h>
#include "glob.h"
#include "estadisticas.h"
#include "sistema.h"
#include "pthreads.h"
#include "lector.h"
#include "estruc.h"
#include "dispatcher.h"

//--------------------------------------------
// Thread del programa (Estrategias)

Estadisticas *estadisticas;

int  sizeBloque;
int  bytesBloque;
int  bytesItem;
bool flagPrimero;

void setValSimBloque(int n)
{
   sizeBloque  = n;
   bytesItem   = sizeof(int)+sizeof(double);
   bytesBloque = sizeBloque*bytesItem;
}

int *contador_locks;

int *ov_barrier;
int full_barrier=0;

double WORK[128];
double tiempos[128];

double  factor_MakeRanking[1024];
double  Factor_MakeRanking;
double  Factor_Sort;
double  Factor_ListaWrite;


void PThreads::inner_body( void )
{ 
  //printf("inicia thread (%d) dimBloque= %d\n",pid,dimBloque);
  int qry=0,qq;
  //cout<<"qry: "<<qry<<"QT: "<<QT<<endl;
  while(qry<QT)
  {
    /*qry++;
    cout<<"qry: "<<qry<<"QT: "<<QT<<endl;
    if( qry>=QT ) break;*/
    //(*despachador)->activateAfter( current( ) );
    qq = qry%QT;

    if( query[qq].tipo==READ )
      runRead(qq);
    else
      runWrite(qq);

    if (pid==0&&qq%100==0) { printf("%d ",qq); fflush(stdout); }

    ovBarrier(qry%NT);
    qry++;

  }

  double total= time();
  //cout<<"Timer of pid "<<pid<<" is "<< time()/1e6<<endl;

  Barrier();
  if (pid==0) { printf("\n"); fflush(stdout); }
  Barrier();
  tiempos[pid]= total/1e6;
  Barrier();

#ifdef MIDE_ESTADISTICAS

  if (pid==0)
  {
     double suma=0, max=0, min=tiempos[0];
     for(int i=0; i<NT; i++)
     {
        suma += tiempos[i];
        if (max<tiempos[i])  max=tiempos[i];
        if (min>tiempos[i])  min=tiempos[i];
     }

     printf("TiempoFinal= %.6lf   Ef= %.2lf  min= %.2lf max= %.2lf\n",
             suma/NT, (suma/NT)/max, min, max );

     imprime();
  }

#endif
 
  if (pid==0)
  {
     double suma=0.0,max=0.0;

     for(int i=0;i<NT;i++)
     {
        suma+=WORK[i];
        if (max<WORK[i]) max= WORK[i];

     }

     printf("WORK=  %.0lf   Ef= %.2lf\n",
              suma, (suma/NT)/max );
  }

  if( pid==0)
  {
    double suma=0.0,max=0.0;

    for(int i=0;i<NT;i++)
    {
      suma+=contador_locks[i];
      if (max<contador_locks[i]) max= contador_locks[i];

    }

    printf("TotalLocks=  %.0lf   Ef= %.2lf\n",
            suma, (suma/NT)/max );
  }
}


void PThreads::runRead(int idq)
{
  int maxIter=0, maxIterQry, termino;
  
  // busca la mayor iteracion de todos los terminos de idq
  for( int jTerm=0; jTerm<query[idq].nt; jTerm++ )
  {
    if ( maxIter<query[idq].iter[jTerm] )
      maxIter=query[idq].iter[jTerm];
  }

  for( int jTerm=0; jTerm<query[idq].nt; jTerm++ )
  {
    termino = query[idq].termino[jTerm];
    maxIterQry = query[idq].iter[jTerm];

    Indice *indx= &indice[pid][termino];
    Bloque *actual= indx->inicioBloque;

    for( int iter=0; iter<maxIter; iter++ )
    {
      if (indx->nb==0) 
      { printf("ERROR, esto no puede ser\n"); exit(0); continue; }

      if( iter<maxIterQry && iter<indx->nb)
      {
        if( iter==0 ) // primera iteracion
          actual = indx->inicioBloque;
        else
          actual = actual->sig;

        if( actual == NULL )
          actual = indx->finBloque;

//----------------

        runCore(0.0, long(&actual->doc[0]),
                (actual->jBloque)*sizeof(int) );

//----------------

        runCore(0.0, long(&actual->frec[0]),
               (actual->jBloque)*sizeof(double) );

//----------------

        runCore2( Factor_MakeRanking*double(actual->jBloque) );

//----------------
      }
    }
  }  
}

void PThreads::runWrite(int idq)
{
  int    termino;
  double frec;
  Indice *indx;
  int doc = query[idq].idDoc;
  int tid = doc%NT;

  for( int jTerm=pid; jTerm<query[idq].nt; jTerm+=NT )
  {
      termino = query[idq].termino[jTerm];
      frec    = query[idq].frec[jTerm];
      
//--------------------------------------------------------------------------
//----- Simulando

      indx=&indice[tid][termino];

      indx->actual =
             metodos->buscaBloque( indx->inicioBloque, frec );
             
//----- Simulador

      // simula en el multi-core lo que hace buscaBloque()
      for( Bloque *puntero= indx->inicioBloque;
           puntero!=NULL && puntero != indx->actual;
           puntero = puntero->sig )
      {
        runCore( 0*double(Factor_ListaWrite*1), // simula comparacion  frec[0]
                 long( puntero ),
                 PAG_CACHE );

        setValSimBloque(puntero->jBloque);
        int npaginasBloque = bytesBloque / PAG_CACHE ;

        runCore( 0*double(Factor_ListaWrite*1), // simula comparacion  frec[jb]
                 long(  ( (char*) puntero )  +  (npaginasBloque-1)*PAG_CACHE  ),
                 PAG_CACHE );
      }

// Simula insercion en un bloque.
      
      if( indx->actual==NULL ) indx->actual = indx->finBloque;
      
// OJO: Esto para ver si todas las estrategias hacen el mismo WORK
/*                  
      indx->actual= indx->inicioBloque;

      setValSimBloque(indx->actual->jBloque);

      runCore( double(Factor_ListaWrite*indx->actual->jBloque),
               long( indx->actual ),
               bytesBloque );
*/

      setValSimBloque(dimBloque);

      runCore( double(Factor_ListaWrite*dimBloque),
               long( indx->actual ),
               bytesBloque );

            
//--------------------------------------------------------------------------

  } // for terminos

}


void PThreads::runCore(double tiempo, long base, int bytes)
{
  char str[1014];
  sprintf(str,"%ld",base);
  WORK[pid] += tiempo;
  core->run(tiempo,str,bytes);
}

void PThreads::runCore2(double tiempo)
{
  WORK[pid] += tiempo;
  core->run2(tiempo);
}


//--------------------------------------------

int main( int argc, char* argv[] )
{
  srand48(1111L);
  srandom(3333L);

  //--- inicia la simulacion

  simulation::instance( )->begin_simulation( new sqsDll( ) );

  int NT;         // total de threads creados.
  int dimBloque;  // dimesion de los bloques/NT
  int dB;         // dimension real de los bloques
  int QT;         // total de consultas/documentos
  int delta=200;

  char home_indice[128], log[128];

  QT = atoi(argv[1]);
  dB = atoi(argv[2]);
  NT = atoi(argv[3]);
  delta=atoi(argv[4]);
  
  dB = dB/4; // se cae con mas de 4096 y no quiero cambiar los run*.sh 
  dimBloque = dB/NT;

  contador_locks = new int[NT];
  for(int i=0; i<NT; i++)
    contador_locks[i]=0;

//./simulador 50 256 4 50 /home2/cluster/carolina/datos-simulacion/indiceOr traza-0-0-8.dat
// ./simulador [QT] [dB] [NT] [delta] [homeindice] [traza] 
  printf("\n%s %s %s %s %s %s %s\n\n", argv[0],argv[1],argv[2],
                                    argv[3],argv[4],argv[5],argv[6] );

  sprintf(home_indice,"%s",argv[argc-2]);
  sprintf(log,"%s",argv[argc-1]);

  // ------------------------------

  Query *query;
  Indice **indice, *masBloques;

  // --- lectura log e indice

  Lector *lector = new Lector(dimBloque, QT, NT);


  char archivo[128];
  int nTerm, idTermMax;

  sprintf(archivo,"%s%s",home_indice,log ); // consultas/documentos
  query = new Query[ QT ];
  lector->loadQry(query, archivo, &nTerm, &idTermMax);
  lector->loadQry1(archivo,&nTerm,&idTermMax);
  handle<Dispatcher> *Despachador=new handle<Dispatcher>[1];
  char nombred[1024];
  sprintf( nombred, "Dispatcher");
  Despachador[0]= new Dispatcher(lector,Despachador,nombred);

printf("fin lectura consultas\n"); 

  indice = new Indice*[NT];
  for(int tid=0; tid<NT; tid++)
  {
    indice[tid] = new Indice[idTermMax+1];
    for(int i=0; i<idTermMax+1; i++)
    {
      indice[tid][i].inicioBloque = NULL;
      indice[tid][i].finBloque    = NULL;
      indice[tid][i].actual       = NULL;
    }
  }

  sprintf(archivo,"%sindexOr-x.idx",home_indice ); // indice

  lector->loadIndice( indice, archivo, idTermMax );
  
printf("fin lectura indice\n"); 
  
  masBloques = new Indice[NT]; // conjunto de bloques por thread
  lector->inicMasBloques(masBloques);

  for(int i=0;i<NT;i++) WORK[i]=0.0;

  // ------------------------------

  char nombre[1024];

  factor_MakeRanking[1]= 1.0*0.342256;
  factor_MakeRanking[2]= 1.0*0.330859;
  factor_MakeRanking[4]= 1.0*0.305229;
  factor_MakeRanking[8]= 1.0*0.273049;

  Factor_MakeRanking =  0.34;//factor_MakeRanking[NT];  
  Factor_MakeRanking =  Factor_MakeRanking*4.0; // debido a dB/4.
    
  Factor_Sort        =  Factor_MakeRanking/5.0;
  Factor_ListaWrite  =  Factor_MakeRanking/10.0;
  
  assert(PAG_CACHE==64); // valores Intel

  int entradas_L1= (32*1024)/PAG_CACHE; // total bytes = entradas_L1*PAG_CACHE
  int entradas_L2= (4*1024*1024)/PAG_CACHE; // total bytes = entradas_L2*PAG_CACHE
  
  entradas_L1 = entradas_L1/4; // debido a dB/4.
  entradas_L2 = entradas_L2/4;
  
  double  latencia_G_L1_L2[1024];
  // De donde vienen estos valores de latencia
  latencia_G_L1_L2[1] = 0.01*1.0000000;
  latencia_G_L1_L2[2] = 0.01*1.2903226;
  latencia_G_L1_L2[4] = 0.01*2.3609467;
  latencia_G_L1_L2[8] = 0.01*4.0634146 ;
  
  double Latencia_G_L1_L2  = 0.01;//latencia_G_L1_L2[NT];
  double Latencia_G_L2_Ram = Latencia_G_L1_L2*10.0;

  printf("cache L1= %dKB  L2= %dMB  Latencia gL1L2= %lf  gL2Ram= %lf\n",
         (entradas_L1*64)/1024, (entradas_L2*64)/(1024*1024),
         Latencia_G_L1_L2, Latencia_G_L2_Ram  );
  printf("Factor_MakeRanking= %lf \n",Factor_MakeRanking);

  handle<PThreads> *pthreads= new handle<PThreads>[NT];

  Locks *locks = new Locks();
  
  for(int i=0;i<NT;i++)
  {
    sprintf( nombre, "PThread_%d", i );

    pthreads[i]= new PThreads( i, NT, nombre, pthreads, Despachador,
                               locks, QT, dimBloque, nTerm,
                               query, indice, masBloques );
  }
   
  ov_barrier = new int[NT];
  for(int i=0;i<NT;i++) ov_barrier[i]= 0;

  estadisticas= new Estadisticas( NT , delta);

  if ( NT==1 )
  {
    Chip *chip = new Chip( 0, 1, entradas_L1,
                                 entradas_L2,
                                 Latencia_G_L1_L2,
                                 Latencia_G_L2_Ram );

    chip->set_thread( &pthreads[0], 0);
    estadisticas-> iniciarAcumuladorTiempoRamL2(1);
    estadisticas-> iniciarAcumuladorTiempoL2L1(1,1);
  }
  else if ( NT==2 )
  {
    Chip *chip = new Chip( 0, 2, entradas_L1,
                                 entradas_L2,
                                 Latencia_G_L1_L2,
                                 Latencia_G_L2_Ram );

    chip->set_thread( &pthreads[0], 0);
    chip->set_thread( &pthreads[1], 1);
    estadisticas-> iniciarAcumuladorTiempoRamL2(1);
    estadisticas-> iniciarAcumuladorTiempoL2L1(1,2);
  }
  else if ( NT==4 || NT==8 || NT==16 || NT==32 || NT==64 || NT==128 )
  {
     int ncores= 4;
     int nchips= NT/ncores;
     int nt=0;

     Chip **chip = new Chip*[nchips];

     for(int i=0; i<nchips; i++)
     {
       chip[i]= new Chip( i, ncores,
                             entradas_L1,
                             entradas_L2,
                             Latencia_G_L1_L2,
                             Latencia_G_L2_Ram );

       for(int j=0; j<ncores; j++, nt++)
       {
          chip[i]->set_thread( &pthreads[nt], j);
       }
     }
     estadisticas->iniciarAcumuladorTiempoRamL2(nchips);
     estadisticas-> iniciarAcumuladorTiempoL2L1(nchips,ncores);
     ASSERT( nt==NT );
  }
  else
  {
     printf("ERROR: valor NT=%d no permitido\n",NT);
     exit(0);
  }

  handle<Sistema> system(

        new Sistema( "System main", pthreads,Despachador,
                      NT
                   )

      );

  system->activate( );

  simulation::instance( )->run( );

  simulation::instance( )->end_simulation( );
  return 0;
}

