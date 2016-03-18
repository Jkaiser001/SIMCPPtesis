
#make todo

./simulador 10000   64  1 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >  ejec-64-w20.dat

./simulador 10000   64  2 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-64-w20.dat

./simulador 10000   64  4 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-64-w20.dat

./simulador 10000   64  8 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-64-w20.dat

./simulador 10000   64 16 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-64-w20.dat

./simulador 10000   64 32 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-64-w20.dat

./simulador 10000   64 64 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-64-w20.dat

./simulador 10000   64 128 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-64-w20.dat

