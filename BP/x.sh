# /scratch/carolina/archivos/indiceOr/

#make todo

./simulador 200   64  1 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat >  xejec-R.dat

./simulador 200  128  2 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200  256  4 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200  512  8 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200 1024 16 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200 2048 32 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200 4096 64 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat >> xejec-R.dat

