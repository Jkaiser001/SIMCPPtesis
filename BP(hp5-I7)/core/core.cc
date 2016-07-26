#include "core.h"
int Core::getCPid(){return cpid;} 
  int Core::getIdCore(){return id_core;}

  void Core::set_thread( handle<PThreads> *_pthread )
  {
      pthread = _pthread;
      (*pthread)->set_core(this);
  }
void Core::run( double t_cpu, string base, int bytes )
  {
    //cout<<"run"<<endl;
     int npag= (int)ceil( double(bytes) / double(PAG_CACHE) );
   
     t_cpu /= npag;

     for(int i=0; i< npag; i++)
     {
        sprintf(str,"%s %d", base.c_str(), i);

        if ( L1->hit( str ) )
        {
          //cout<<"hit_L1, "<<cpid<<" - "<<id_core<<endl;
#ifdef MIDE_ESTADISTICAS
           estadisticas->hit_L1( (*pthread)->pid );
#endif
           L1->update( str );// me falta este tiempo
           if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
        }
        else
        {
           if ( L2->hit( str ) )
           {
#ifdef MIDE_ESTADISTICAS
              estadisticas->fallaL1( (*pthread)->pid );
              estadisticas->hit_L2( (*pthread)->pid );
             
#endif
              //cout<<"hit_L2, "<<cpid<<" - "<<id_core<<endl;
              L2->update( str );//me falta este tiempo
              L1->insert( str );
              (*pthread)->phold2( Latencia_G_L1_L2 );
            if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
            }
            else
            {
              //cout<<"hit_Ram, "<<cpid<<" - "<<id_core<<endl;
#ifdef MIDE_ESTADISTICAS
              estadisticas->fallaL1( (*pthread)->pid );
              estadisticas->fallaL2( (*pthread)->pid );
              estadisticas->hit_Ram( (*pthread)->pid );
              
#endif
              L2->insert( str );
              L1->insert( str );
              (*pthread)->phold2( Latencia_G_L1_L2 );
              (*pthread)->pholdR( Latencia_G_L2_Ram);
            if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
              //if(id_core!=(*pthread)->getPid()%4) cout<<"pid "<< (*pthread)->getPid() << "core: "<<id_core<<endl;
            }

        } 

     }

  }
  void Core::run2( double t_cpu )  
  {
     if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
     //out<<"run"<<endl;
  }
  
  
  void Core::run3( double t_cpu, string base, int bytes )
  {
    int npag= (int)ceil( double(bytes) / double(PAG_CACHE) );
    //cout<<"Entro run3"<<endl;
     t_cpu /= npag;

     for(int i=0; i< npag; i++)
     {
        sprintf(str,"%s %d", base.c_str(), i);

        if ( L1->hit( str ) )
        {
          //cout<<"hit_L1, "<<cpid<<" - "<<id_core<<endl;
#ifdef MIDE_ESTADISTICAS
           estadisticas->hit_L1( (*pthread)->pid );
           estadisticas->suma_lecturas((*pthread)->pid%NCORE,0);
#endif
           L1->update( str );// me falta este tiempo
           if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
        }
        else if ( L2->hit( str ) )
       {
        #ifdef MIDE_ESTADISTICAS
            estadisticas->fallaL1( (*pthread)->pid  );
            estadisticas->hit_L2( (*pthread)->pid );
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,0); 
            estadisticas->suma_escrituras((*pthread)->pid%NCORE,0);
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,1);
        #endif
          //cout<<"hit_L2, "<<cpid<<" - "<<id_core<<endl;
          L2->update( str );//me falta este tiempo
          L1->insert( str );
          (*pthread)->phold2( Latencia_G_L1_L2);
        if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
        }
        else if (L3->hit( str ))
        {
          #ifdef MIDE_ESTADISTICAS
            estadisticas->fallaL1( (*pthread)->pid  );
            estadisticas->fallaL2( (*pthread)->pid  );
            estadisticas->hit_L3( (*pthread)->pid );
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,0);
            estadisticas->suma_escrituras((*pthread)->pid%NCORE,0);
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,1);
            estadisticas->suma_escrituras((*pthread)->pid%NCORE,1);
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,2);
          #endif
          
          L3->update( str );
          L2->insert( str );
          L1->insert( str );
          (*pthread)->phold3( Latencia_G_L2_L3);
          (*pthread)->phold2( Latencia_G_L1_L2);

          if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
        }
        else
        {

          #ifdef MIDE_ESTADISTICAS
            estadisticas->fallaL1( (*pthread)->pid );
            estadisticas->fallaL2( (*pthread)->pid );
            estadisticas->fallaL3( (*pthread)->pid );
            estadisticas->hit_Ram( (*pthread)->pid );
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,0);
            estadisticas->suma_escrituras((*pthread)->pid%NCORE,0);
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,1);
            estadisticas->suma_escrituras((*pthread)->pid%NCORE,1);
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,2);
            estadisticas->suma_escrituras((*pthread)->pid%NCORE,2);
            estadisticas->suma_lecturas((*pthread)->pid%NCORE,3);
          #endif
          L3->insert( str );
          L2->insert( str );
          L1->insert( str );
          (*pthread)->pholdR( Latencia_G_L3_Ram );
          (*pthread)->phold3( Latencia_G_L2_L3 );
          (*pthread)->phold2( Latencia_G_L1_L2 );
          if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
          }
        }
      }
      


  