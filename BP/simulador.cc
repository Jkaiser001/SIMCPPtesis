/*
Ejecucion en etna

./simulador QT dimBloque NT ../archivos/ traza.dat

ESTRATEGIA: BP

Ejecucion:

./simulador 5000 256 4 /scratch/carolina/archivos/indiceOr/traza-0-0-8.dat

make;
./simulador 200  256 8 /home2/cluster/carolina/datos-simulacion/indiceOr/ traza-0-0-8.dat

*/

#include <stdlib.h>
#include "glob.h"
#include "estadisticas.h"
#include "sistema/sistema.h"
#include "pthreads/pthreads.h"
#include "lector/lector.h"
#include "estruc.h"
#include "dispatcher/dispatcher.h"

//--------------------------------------------
// Thread del programa (Estrategias)

Estadisticas *estadisticas;

int *contador_locks;

int *ov_barrier;
int full_barrier=0;

double WORK[128];
double tiempos[128];

double  factor_MakeRanking[1024];
double  Factor_MakeRanking;
double  Factor_Sort;
double  Factor_ListaWrite;
int terminadas=0;


void PThreads::inner_body( void )
{ 
  //printf("inicia thread (%d) dimBloque= %d\n",pid,dimBloque);
  int qry=-1;
  //cout<<"qry: "<<qry<<"QT: "<<QT<<endl;
  while(1)
  {
    
    //cout<<"PID: "<<pid<<" qry: "<<qry<<" QT: "<<QT<<endl;
    qry++;
    if( qry>=QT/NT ) break;
     //(*despachador)->activateAfter( current( ) );
    //qq = qry%QT;

    int flag=0;
    while(flag==0&&qry<QT/NT){
      if(!listQuery_ptheards.empty()){
          Query query=pop_query();
          if(query.tipo==READ){
            //cout<<"PID: "<<pid<<", TAMAÑO COLA: "<<listQuery_ptheards.size()<<", LEOO"<<endl;
            runRead(query);
          }
          else   {
            //cout<<"PID: "<<pid<<", TAMAÑO COLA: "<<listQuery_ptheards.size()<<",ESCRIBO"<<endl;
            runWrite(query);
          }
          terminadas++;
          flag=1;  
      }else{
        //cout<<"Vacia"<<endl;
        phold4(2.0);

      }
      
      

    }
    
    /*if( query[qq].tipo==READ )
      runRead(qq);
    else
      runWrite(qq);
    */
    if (terminadas%100==0) { printf("%d ",terminadas); fflush(stdout); }
    //if (qq%100==0) { printf("%d ",qq); fflush(stdout); }

    ovBarrier(qry%NT);
    
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


void PThreads::runRead(Query query)
{
  int maxIter=0, maxIterQry, termino;
  
  // busca la mayor iteracion de todos los terminos de idq
  for( int jTerm=0; jTerm<query.nt; jTerm++ )
  {
    if ( maxIter<query.iter[jTerm] )
      maxIter=query.iter[jTerm];
  }

  for( int jTerm=0; jTerm<query.nt; jTerm++ )
  {
    termino = query.termino[jTerm];
    maxIterQry = query.iter[jTerm];

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

void PThreads::runWrite(Query query)
{
  int    termino;
  double frec;
  Indice *indx;
  int doc = query.idDoc;
  int tid = doc%NT;

  for( int jTerm=pid; jTerm<query.nt; jTerm+=NT )
  {
      termino = query.termino[jTerm];
      frec    = query.frec[jTerm];
      
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

bool valido(int NT){
  
  int aux=1;
  while(aux<=NT){
    cout<<"AUX: "<<aux<<endl;

    if(aux==NT) return true;
    aux=aux*2;
    

  }

  return false;
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
  //cout<<"NO Problem"<<endl;
  dB = dB/NCORE; // se cae con mas de 4096 y no quiero cambiar los run*.sh 
  //cout<<"dB: "<<dB<<endl;
  dimBloque = dB/NT;
  //cout<<"dimBloque: "<<dB<<endl;

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
 

  //lector->loadQry(query, archivo, &nTerm, &idTermMax);

  lector->loadQry1(archivo,&nTerm,&idTermMax);
  cout<<"Problem"<<endl;
  handle<Dispatcher> *Despachador=new handle<Dispatcher>[1];
 
  char nombred[1024]; 
  sprintf( nombred, "Dispatcher");    

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

  Factor_MakeRanking =  factor_MakeRanking[NT];  
  Factor_MakeRanking =  Factor_MakeRanking*4.0; // debido a dB/4.
    
  Factor_Sort        =  Factor_MakeRanking/5.0;
  Factor_ListaWrite  =  Factor_MakeRanking/10.0;
  
  assert(PAG_CACHE==64); // valores Intel

  int entradas_L1= (SIZE_L1)/PAG_CACHE; // total bytes = entradas_L1*PAG_CACHE
  int entradas_L2= (SIZE_L2)/PAG_CACHE; // total bytes = entradas_L2*PAG_CACHE
  int entradas_L3= (SIZE_L3)/PAG_CACHE;

  entradas_L1 = entradas_L1/NCORE; // debido a dB/4.
  entradas_L2 = entradas_L2/NCORE;  
  entradas_L3 = entradas_L3/NCORE;

  double  latencia_G_L1_L2[1024];
  // De donde vienen estos valores de latencia
  latencia_G_L1_L2[1] = 0.01*1.0000000;
  latencia_G_L1_L2[2] = 0.01*1.2903226;
  latencia_G_L1_L2[4] = 0.01*2.3609467;
  latencia_G_L1_L2[8] = 0.01*4.0634146 ;
  
  double Latencia_G_L1_L2  = 0.01;
  double Latencia_G_L2_Ram = Latencia_G_L1_L2*10.0;
  double Latencia_G_L2_L3 = Latencia_G_L1_L2*10.0;
  double Latencia_G_L3_Ram = Latencia_G_L1_L2*100.0;

  printf("cache L1= %dKB  L2= %dMB  Latencia gL1L2= %lf  gL2Ram= %lf\n",
         (entradas_L1*64)/1024, (entradas_L2*64)/(1024*1024),
         Latencia_G_L1_L2, Latencia_G_L2_Ram  );
  printf("Factor_MakeRanking= %lf \n",Factor_MakeRanking);

  handle<PThreads> *pthreads= new handle<PThreads>[NT];

  Locks *locks = new Locks();
  
  for(int i=0;i<NT;i++)
  {

    sprintf( nombre, "PThread_%d", i );

    pthreads[i]= new PThreads( i, NT, nombre, pthreads,
                               locks, QT, dimBloque, nTerm,
                               query, indice, masBloques );//aqui se pasan las query cambiar query por un handle<espachadorQuery> crean las query!!!
  }
    Despachador[0]= new Dispatcher(QT,NT,lector,Despachador,pthreads,nombred);
   
  ov_barrier = new int[NT];
  for(int i=0;i<NT;i++) ov_barrier[i]= 0;

  estadisticas= new Estadisticas( NT , delta);


  if(valido( NT )){
    cout<<"-.NT/NCORE: "<<NT/NCORE<<endl;
    if( NT/NCORE==0)
      {
        
        Chip *chip;
        if (LEVELCACHE==2)
        chip = new Chip( 0, NT, entradas_L1,
                                 entradas_L2,
                                 Latencia_G_L1_L2,
                                 Latencia_G_L2_Ram );
      else
        chip= new Chip( 0,NT,entradas_L1, 
                              entradas_L2, 
                              entradas_L3, 
                              Latencia_G_L1_L2, 
                              Latencia_G_L2_L3,
                              Latencia_G_L3_Ram);
      

        for(int i=0;i<NT;i++)
          chip->set_thread( &pthreads[i], i);   
        estadisticas-> iniciarAcumuladorTiempoL2L1(1,NT);
        estadisticas->iniciarAcumuladorTiempoRamL2(1,NT);

      }
    else
      {
      int ncores= NCORE;
      int nchips= NT/ncores;
      int nt=0;

       Chip **chip = new Chip*[nchips];

       for(int i=0; i<nchips; i++)
       {
        
        if (LEVELCACHE==2)
        
        chip[i]= new Chip( i, ncores,
                             entradas_L1,
                             entradas_L2,
                             Latencia_G_L1_L2,
                             Latencia_G_L2_Ram );
        else
          chip[i]= new Chip( i,ncores,
                              entradas_L1, 
                              entradas_L2, 
                              entradas_L3, 
                              Latencia_G_L1_L2, 
                              Latencia_G_L2_L3,
                              Latencia_G_L3_Ram );

         for(int j=0; j<ncores; j++, nt++)
         {
            chip[i]->set_thread( &pthreads[nt], j);
         }
       }
       estadisticas-> iniciarAcumuladorTiempoL2L1(nchips,ncores);
       estadisticas->iniciarAcumuladorTiempoRamL2(nchips,ncores);
       ASSERT( nt==NT );

      }
    }
  else{
    
    printf("ERROR: valor NT=%d no permitido\n",NT);
    exit(0);
    
    }
    
 
/*
  if ( NT==1 )
  {
    
    Chip *chip = new Chip( 0, 1, entradas_L1,
                                 entradas_L2,
                                 Latencia_G_L1_L2,
                                 Latencia_G_L2_Ram );

    chip->set_thread( &pthreads[0], 0);
    estadisticas-> iniciarAcumuladorTiempoL2L1(1,1);
    estadisticas-> iniciarAcumuladorTiempoRamL2(1,1);
  }
  else if ( NT==2 )
  {
    
    Chip *chip = new Chip( 0, 2, entradas_L1,
                                 entradas_L2,
                                 Latencia_G_L1_L2,
                                 Latencia_G_L2_Ram );
    
    chip->set_thread( &pthreads[0], 0);
    chip->set_thread( &pthreads[1], 1);


    estadisticas-> iniciarAcumuladorTiempoL2L1(1,2);    
    estadisticas-> iniciarAcumuladorTiempoRamL2(1,2);

  }

  else if ( NT==4 || NT==8 || NT==16 || NT==32 || NT==64 || NT==128 )
  {
     int ncores= NCORE;
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
     estadisticas-> iniciarAcumuladorTiempoL2L1(nchips,ncores);
     estadisticas->iniciarAcumuladorTiempoRamL2(nchips,ncores);
     ASSERT( nt==NT );
  }
  else
  {
     printf("ERROR: valor NT=%d no permitido\n",NT);
     exit(0);
  }
*/
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

