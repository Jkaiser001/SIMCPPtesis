
#make todo

./simulador 10000   64  1 ../Input/ traza-30-0-8.dat >  wxejec-R.dat

./simulador 10000 128  2 ../Input/ traza-30-0-8.dat >> wxejec-R.dat

./simulador 10000 256  4 ../Input/ traza-30-0-8.dat >> wxejec-R.dat

./simulador 10000  512  8 ../Input/ traza-30-0-8.dat >> wxejec-R.dat

./simulador 10000 1024 16 ../Input/ traza-30-0-8.dat >> wxejec-R.dat

./simulador 10000 2048 32 ../Input/ traza-30-0-8.dat >> wxejec-R.dat

./simulador 10000 4096 64 ../Input/ traza-30-0-8.dat >> wxejec-R.dat