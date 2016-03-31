# /scratch/carolina/archivos/indiceOr/

#make todo

./simulador 100   64  1 ../Input/ traza-0-0-8.dat >  xejec-R.dat

./simulador 10000  128  2 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 10000  256  4 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 10000  512  8 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 10000 1024 16 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 10000 2048 32 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 1000 4096 64 ../Input/ traza-0-0-8.dat >> xejec-R.dat

