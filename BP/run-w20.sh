
#make todo

./simulador 10000   64  1 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >  ejec-w20.dat

./simulador 10000  128  2 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-w20.dat

./simulador 10000  256  4 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-w20.dat

./simulador 10000  512  8 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-w20.dat

./simulador 10000 1024 16 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-w20.dat

./simulador 10000 2048 32 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-w20.dat

./simulador 10000 4096 64 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-w20.dat

./simulador 10000 8192 128 /scratch/carolina/archivos/indiceOr/ traza-20-0-8.dat >> ejec-w20.dat

