#include "core.h"
int Core::getCPid(){return cpid;} 
  int Core::getIdCore(){return id_core;}

  void Core::set_thread( handle<PThreads> *_pthread )
  {
      pthread = _pthread;
      (*pthread)->set_core(this);
  }

  void Core::run2( double t_cpu )  
  {
     if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
     //out<<"run"<<endl;
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
              estadisticas->hit_L2( (*pthread)->pid );
#endif
              //cout<<"hit_L2, "<<cpid<<" - "<<id_core<<endl;
              L2->update( str );//me falta este tiempo
              L1->insert( str );
              (*pthread)->phold2( Latencia_G_L1_L2 , cpid,id_core );
            if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
            }
            else
            {
              //cout<<"hit_Ram, "<<cpid<<" - "<<id_core<<endl;
#ifdef MIDE_ESTADISTICAS
              estadisticas->hit_Ram( (*pthread)->pid );
#endif
              L2->insert( str );
              L1->insert( str );
              (*pthread)->phold2( Latencia_G_L1_L2,cpid,id_core );
              (*pthread)->phold3( Latencia_G_L2_Ram);
            if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
              //if(id_core!=(*pthread)->getPid()%4) cout<<"pid "<< (*pthread)->getPid() << "core: "<<id_core<<endl;
            }

        } 

     }

  }
  void Core::runL3( double t_cpu, string base, int bytes )
  {
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
        else if ( L2->hit( str ) )
       {
        #ifdef MIDE_ESTADISTICAS
            estadisticas->hit_L2( (*pthread)->pid );
        #endif
          //cout<<"hit_L2, "<<cpid<<" - "<<id_core<<endl;
          L2->update( str );//me falta este tiempo
          L1->insert( str );
          (*pthread)->phold2( Latencia_G_L1_L2 , cpid,id_core );
        if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
        }
        else if (L3->hit( str ))
        {
          #ifdef MIDE_ESTADISTICAS
              estadisticas->hit_Ram( (*pthread)->pid );
          #endif
          L3->update( str );
          L2->insert( str );
          L1->insert( str );
        }
        else
        {

          #ifdef MIDE_ESTADISTICAS
          estadisticas->hit_Ram( (*pthread)->pid );
          #endif
          L3->insert( str );
          L2->insert( str );
          L1->insert( str );
          (*pthread)->phold2( Latencia_G_L1_L2,cpid,id_core );
          (*pthread)->phold3( Latencia_G_L2_Ram);
          if (t_cpu!=0.0) (*pthread)->phold( t_cpu );
          if(id_core!=(*pthread)->getPid()%4) cout<<"pid "<< (*pthread)->getPid() << "core: "<<id_core<<endl;

          }
        }
      }
      


  