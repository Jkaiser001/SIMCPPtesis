# /scratch/carolina/archivos/indiceOr/

#make todo

./simulador 200   64  1 ../Input/ traza-0-0-8.dat >  xejec-R.dat

./simulador 200  128  2 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200  256  4 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200  512  8 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200 1024 16 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200 2048 32 ../Input/ traza-0-0-8.dat >> xejec-R.dat

./simulador 200 4096 64 ../Input/ traza-0-0-8.dat >> xejec-R.dat

