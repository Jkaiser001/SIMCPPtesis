#include "pthreads.h"

void PThreads::imprime()
  {
     printf("Ef= %.4lf \n", estadisticas->Eficiencia() );
     estadisticas->muestraFallas();
  }


 // definimos varios phold iguales para obtener
 // las estadisticas diferenciadas por core, L1 y L2.
  void PThreads::add_query(Query _query){
    listQuery_ptheards.push_back(_query);
    //cout<<"pid: "<<pid<<"tamaño COLA:"<<listQuery_ptheards.size()<<endl;
  }
  Query PThreads::pop_query(){
    Query query=listQuery_ptheards.front();
    listQuery_ptheards.pop_front();
    return query;
  }
  void PThreads::setStatus(){
    statusDispacher=0;
  }
  void PThreads::phold( double t )
  {
    
#ifdef MIDE_ESTADISTICAS
     
    //cout<<"--------L1 --------, Pid: "<<pid<<" , tiempo: "<<t<<endl;
         
        estadisticas->sumarTiemposL1(pid,t);
        estadisticas->mide( pid,pid/NCORE,pid%NCORE, time(), t ,ACTIVE,CPU);//Borrar


#endif

     hold(t);
     //cout<<"Para la hebra "<<pid<<" tiempo de holt1:"<<t<<endl;
  }

  void PThreads::phold2( double t )
  {
    
#ifdef MIDE_ESTADISTICAS
     
      //cout<<"--------L2 --------, Pid: "<<pid<<" , tiempo: "<<t<<endl;
     
      estadisticas->sumarTiemposL1(pid,t);
      estadisticas->sumarTiemposL2(pid,t);
      estadisticas->mide( pid , pid/NCORE ,pid%NCORE, time(), t , INACTIVE,CL1 );
      

#endif
     hold(t);
  }

  void PThreads::phold3( double t )
  {
    
#ifdef MIDE_ESTADISTICAS
    
      //cout<<"--------L3 --------, Pid: "<<pid<<" , tiempo: "<<t<<endl;
      estadisticas->sumarTiemposL3(pid,t);
      estadisticas->sumarTiemposL2(pid,t);
      estadisticas->mide( pid, pid/NCORE, pid%NCORE, time(), t, INACTIVE,CL2);
    
    
#endif
     hold(t);
  }
  void PThreads::pholdR( double t )
  {
    
#ifdef MIDE_ESTADISTICAS
  
      //cout<<"--------RAM--------, Pid: "<<pid<<" , tiempo: "<<t<<endl;
      estadisticas->sumarTiemposL3(pid,t);
      estadisticas->mide( pid, pid/NCORE, pid%NCORE, time(), t, INACTIVE,CL3);
 
    
#endif
     hold(t);
  }
  void PThreads::phold4( double t)
  {
    
#ifdef MIDE_ESTADISTICAS
    
      estadisticas->mide( pid, pid/NCORE, pid%NCORE, time(), t, INACTIVE,CPU);
     
#endif
     hold(t);
  }
   void PThreads::phold5( double t)
  {
    
#ifdef MIDE_ESTADISTICAS
     
      estadisticas->mide( pid, pid/NCORE, pid%NCORE, time(), t, INACTIVE,CPU);
     
    
#endif
     hold(t);
  }

  void PThreads::duerme() { (*htrd)->passivate( ); }
  void PThreads::despierta() { (*htrd)->activateAfter( current( ) ); }

  void PThreads::set_core( Core *_core)
  {
     core = _core;
  }

  void PThreads::set_lock( int term )
  {
     char str[128];
     sprintf(str,"%d",term);
     lock->set_lock(str,htrd,pid);
  }

  void PThreads::set_unlock( int term )
  {
     char str[128];
     sprintf(str,"%d",term);
     lock->set_unlock(str);
  }


  void PThreads::ovBarrier( int tid )
  {
     bool flag=true;

     set_lock( -tid );

     ov_barrier[tid]++;

     if ( pid == tid )
     {
       if ( ov_barrier[tid] < NT )
       { set_unlock( -tid ); duerme(); flag=false; }
       else
         ov_barrier[tid] = 0;
     }
     else
     if ( ov_barrier[tid] == NT )
     {
       ov_barrier[tid] = 0;
       (*htrd_barrier[tid]).despierta();
     }

     if (flag==true)
     set_unlock( -tid );
  }


  void PThreads::Barrier( )
  {
     full_barrier++;

     if ( full_barrier < NT )
       duerme();
     else
     {
       full_barrier = 0;

       for(int i=0;i<NT;i++)
       {
         if (i!=pid)
           (*htrd_barrier[i]).despierta();
       }
     }
  }
    void PThreads::setValSimBloque(int n)
{
   sizeBloque  = n;
   bytesItem   = sizeof(int)+sizeof(double);
   bytesBloque = sizeBloque*bytesItem;
}
  int PThreads::getPid(){
    return pid;
  }


