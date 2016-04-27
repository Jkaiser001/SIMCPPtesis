
/* editado desde mi notebook */

#include "lector.h"
#include <stdlib.h>

// -------------------------------------------
// --- lectura consultas/documentos

void Lector::loadQry(Query *query, char *archivo, int *nTerm, int *idTermMax)
{
  bool Fin=false;

  FILE *fp = fopen(archivo,"r");
  if (fp==NULL) { printf("No se puede abrir %s",archivo); exit(0); }
  char word[128], frase[128], c;

  /*
  formato entrada:

  READ: idQry tipoQry nTerms (termino iteracion)+
  WRITE/UPDATE: idQry tipoQry doc nTerm (termino frecuencia)+

  */

  int idq= -1;
  *nTerm=0; *idTermMax=0;

  while(1)
  {
    strcpy(word," ");

    while(1) // --- identificador de la consulta/documento
    {
      c = getc(fp); if (c == EOF) { Fin=true; break; }
      sprintf(frase,"%c",c);
      if (c!=' ') strcat(word,frase);
      else break;
    }

    if(Fin==true) break;
    //idq = atoi(word);
    idq++;
    strcpy(word," ");

    while(1) // --- tipo: read, insert, update
    {
      c = getc(fp); if (c == EOF) { Fin=true; break; }
      sprintf(frase,"%c",c);
      if (c!=' ') strcat(word,frase);
      else break;
    }

    if(Fin==true) break;
    query[idq].tipo = atoi(word);
    strcpy(word," ");

    // si es lectura se lee la cantidad de terminos. si es escritura se lee el idDoc

    while(1)
    {
      c = getc(fp); if (c == EOF) { Fin=true; break;}
      sprintf(frase,"%c",c);
      if (c!=' ') strcat(word,frase);
      else break;
    }

    if(Fin==true) break;

    if( query[idq].tipo==READ )
      query[idq].nt = atoi(word);
    else
     query[idq].idDoc = atoi(word);

    strcpy(word," ");

    // la cantidad de maxima de terminos que tiene una lectura
    if( query[idq].tipo==READ && *nTerm<query[idq].nt) *nTerm = query[idq].nt;

    // si es una escritura, despues de leer el idDoc, se lee el total del terminos del documento
    if( query[idq].tipo==WRITE )
    {
      while(1)  // nt
      {
        c = getc(fp); if (c == EOF) {Fin=true; break;}
        sprintf(frase,"%c",c);
        if (c!=' ') strcat(word,frase);
        else break;
      }

      if(Fin==true) break;
      query[idq].nt = atoi(word);
      strcpy(word," ");

    } // end if()


    query[idq].termino = new int[ query[idq].nt ];

    if( query[idq].tipo==READ )
      query[idq].iter = new int[ query[idq].nt ];
    else
      query[idq].frec = new double[ query[idq].nt ];


    /*
    si es lectura, lee los pares termino+iteracion
    si es escritura, lee los pares termino+frecuencia
    */

    for( int i=0; i<query[idq].nt; i++ )
    {
      strcpy(word," ");

      while(1) // termino
      {
        c= getc(fp); if (c==EOF) {Fin=true; break;}
        sprintf(frase,"%c",c);
        if (c!=' ') strcat(word,frase);
        else break;
      }
      if(Fin==true) break;
      query[idq].termino[i] = atoi(word);
      strcpy(word," ");

      // calcula el termino mayor de la consulta/documento
      if( *idTermMax<query[idq].termino[i] ) *idTermMax=query[idq].termino[i]+1;

      while(1) // iteracion o frecuencia
      {
        c= getc(fp);
        if(c==EOF || c== '\n' || c==' ') break;
        sprintf(frase,"%c",c);
        strcat(word,frase);
      }

      if( query[idq].tipo==READ )
      {          
        int nn= atoi(word);
        int delta = nn/128;
        nn = 128*delta + 128;        

        if (nn%dB == 0)
          query[idq].iter[i] = nn/dB;
        else
          query[idq].iter[i] = ( nn/dB ) + 1;          
      }
      else
        query[idq].frec[i] = atof(word);

    } // end for()

    if(idq==QT-1) break;

  } // end while(1)

  fclose(fp);
  //arryQuery=query;
} // end loadQry()

// -------------------------------------------
// --- lectura indice
void Lector::loadQry1( char *archivo, int *nTerm, int *idTermMax)
{
  string line;
  ifstream file (archivo);
  // READ: idQry tipoQry nTerms (termino iteracion)+
  // WRITE/UPDATE: idQry tipoQry doc nTerm (termino frecuencia)+

  if(file.is_open())
  {
    while(getline(file,line)){
      Query query;
      //cout<<line<<endl;
      stringstream split(line);
      string aux;
      split>>aux;
      split>>aux;
      query.tipo=atoi(aux.c_str());
      split>>aux;
      if(query.tipo==READ){
        query.nt=atoi(aux.c_str());
        if(*nTerm<query.nt) *nTerm=query.nt;
        query.termino =new int[query.nt];
        query.iter = new int[query.nt];

      }else
      {
        query.idDoc=atoi(aux.c_str());
        split>>aux;
        query.nt = atoi(aux.c_str());
        query.termino = new int[query.nt];
        query.frec = new  double[query.nt];
      }

      for (int i = 0; i < query.nt; ++i)
      {
          split>>aux;
          query.termino[i] = atoi(aux.c_str());
          if(*idTermMax<query.termino[i]) *idTermMax=query.termino[i];
          split>>aux;
          if(query.tipo == READ){
              int nn = atoi(aux.c_str());
              int delta = nn/128;
              nn = 128*delta + 128;        

              if (nn%dB == 0)
                query.iter[i] = nn/dB;
              else
                query.iter[i] = ( nn/dB ) + 1;  
          
          }else{
              
              query.frec[i]=atof(aux.c_str());
          
          }

      }

      vecQuery.push_back(query);
    }
  }
    cout<<"Cantidad"<<vecQuery.size()<<endl;

}
Query Lector::getQuery(int i){
   return vecQuery[i];
}
void Lector::loadIndice(Indice **indice, char *archivo, int idTermMax )
{
  bool Fin=false;

  char word[1024], frase[1024], c;
  FILE *fp = fopen(archivo,"r");
  if (fp==NULL) { printf("No se puede abrir %s",archivo); exit(0); }

  /*
  idTerm NdocAsociados doc frec doc frec ....
  */

  int termino, pares; int b, doc;
  double frec;
  Bloque *inicio, *fin;
  int jBloque = dimBloque;

  while(1)
  {
    strcpy(word," ");
    while(1) // termino
    {
      c = getc(fp); if (c == EOF) {Fin=true; break; }
      sprintf(frase,"%c",c);
      if (c!=' ') strcat(word,frase);
      else break;
    }
    if(Fin==true) break;

    termino = atoi(word);
    strcpy(word," ");

    while(1) // total de pares (documento,frecuencia)
    {
      c = getc(fp); if (c == EOF || c== '\n' || c==' ') break;
      sprintf(frase,"%c",c);
      if (c!=' ') strcat(word,frase);
      else break;
    }

    if(Fin==true) break;
    pares = atoi(word);
    pares = pares/4; // debido a dB/4
    strcpy(word," ");

    int delta = pares/128;
    pares = 128*delta + 128;

    if(termino<=idTermMax)  // lista de pares (documento, frecuencia)
    {
      for( int tid=0; tid<NT; tid++ )
      {
        indice[tid][termino].pares = pares/NT;

        if ( indice[tid][termino].pares<=jBloque )
          indice[tid][termino].nb= 1;
        else
        if ( indice[tid][termino].pares%jBloque == 0 )
          indice[tid][termino].nb=(indice[tid][termino].pares/jBloque);
        else
          indice[tid][termino].nb=(indice[tid][termino].pares/jBloque)+1;

        metodos->inic_Bloques( &inicio, &fin, indice[tid][termino].nb );

        indice[tid][termino].inicioBloque = inicio;
        indice[tid][termino].finBloque    = fin;
        indice[tid][termino].actual       = inicio;

        b=0; //  9.963000  maxima frecuencia en indexOr.idx
        double delta= 9.963/(indice[tid][termino].pares+1);
        double frec_acc= 10.0-drand48()*delta;

        for( int total=0; total<indice[tid][termino].pares; total++ )
        {
          doc  = 1+random()%530652; //mayor ID-doc detectado en indexOr.idx.
          frec = frec_acc; // listas ordenadas por frecuencia.
          frec_acc -= drand48()*delta;
          if (frec_acc<0.0) frec_acc= 0.0;

          indice[tid][termino].actual->doc[b]  = doc;
          indice[tid][termino].actual->frec[b] = frec;

          b++;
          if(b==jBloque)
          {
            b=0;
            indice[tid][termino].actual->jBloque = jBloque;
            indice[tid][termino].actual->estadoBloque = BLOQUE_NO_LLENO;

            indice[tid][termino].actual = indice[tid][termino].actual->sig;
          }
        }

        if(b!=0) // verifico el estado del ultimo bloque
        {
          indice[tid][termino].actual->jBloque = b;
          indice[tid][termino].actual->estadoBloque = BLOQUE_NO_LLENO;
        }

        indice[tid][termino].actual = inicio;
      }
    }
    else
     break;

  } // end while(1)

  fclose(fp);

} // end loadIndice()


// ----------------------------------------------
// --- grupo de bloques por threads

void Lector::inicMasBloques(Indice *masBloques)
{
  Bloque *inicio, *fin;

  for(int i=0; i<NT; i++)
  {
    metodos->inic_Bloques( &inicio, &fin, MAX_BLOQUES);

    masBloques[i].inicioBloque = inicio;
    masBloques[i].finBloque = fin;
    masBloques[i].actual = inicio;
    masBloques[i].libres = MAX_BLOQUES;
    masBloques[i].nb = MAX_BLOQUES;
  }
}

