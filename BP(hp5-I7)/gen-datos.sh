for i in ejec*.dat; 
do  cat $i | awk -v archivo=$i '{ 
  if ($1 == "./simulador"){ nt=$4;} 
  if ($1=="TiempoFinal=") print $1,$2,"NT= ",nt,archivo; 
  else print $0  
}' |grep Tiempo > tiempo-$i; 
done

for i in ejec*.dat; 
do  cat $i | awk -v archivo=$i '{ 
  if ($1 == "./simulador"){ nt=$4;} 
  if ($1=="WORK=") print $1,$2,"NT= ",nt,archivo; 
  else print $0  
}' |grep WORK   > work-$i; 
done

for i in ejec*.dat; 
do  cat $i | awk -v archivo=$i '{ 
  if ($1 == "./simulador"){ nt=$4;} 
  if ($1=="Total_HitsL1=") print $1,$2,"NT= ",nt,archivo; 
  else print $0  
}' |grep HitsL1 > hitsL1-$i; 
done

for i in ejec*.dat; 
do  cat $i | awk -v archivo=$i '{ 
  if ($1 == "./simulador"){ nt=$4;} 
  if ($1=="Total_HitsL2=") print $1,$2,"NT= ",nt,archivo; 
  else print $0  
}' |grep HitsL2 > hitsL2-$i; 
done

for i in ejec*.dat; 
do  cat $i | awk -v archivo=$i '{ 
  if ($1 == "./simulador"){ nt=$4;} 
  if ($1=="TotalLocks=") print $1,$2,"NT= ",nt,archivo; 
  else print $0  
}' |grep TotalLocks > totalLocks-$i; 
done

